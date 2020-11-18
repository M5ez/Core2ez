#include "ezSound.h"

#include <M5Core2.h>
// #include <FreeRTOS.h>
#include <driver/i2s.h>


/* static */ ezSound* ezSound::instance;

ezSound::ezSound() {
  if (!instance) instance = this;
  _bytes_left = 0;
}

void ezSound::begin() {
  // populate sinewave table
  for (uint8_t n = 0; n < 100; n++) {
    _sineTable[n] = sin((float)n * TWO_PI / 100);
  }
  // Set up I2S driver
  i2s_driver_uninstall(I2S_NUM_0);
  i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
    .sample_rate = SAMPLERATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
    .communication_format = I2S_COMM_FORMAT_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = DMA_BUF_COUNT,
    .dma_buf_len = DMA_BUF_LEN,
    .use_apll = false,
    .tx_desc_auto_clear = true
  };
  i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
  // Configure pins
  i2s_pin_config_t pin_config= {
    .bck_io_num = I2S_BCLK,
    .ws_io_num = I2S_LRC,
    .data_out_num = I2S_DOUT,
    .data_in_num = I2S_PIN_NO_CHANGE
  };
  i2s_set_pin(I2S_NUM_0, &pin_config);
  // Set up clock
  i2s_set_clk(I2S_NUM_0, SAMPLERATE, I2S_BITS_PER_SAMPLE_16BIT,
              I2S_CHANNEL_STEREO);
  // Turn on system speaker
  _amp_on = false;
  _silentSince = millis();
}

void ezSound::delay(uint16_t msec) {
  uint32_t start = millis();
  while (millis() - start < msec) {
    update();
    yield();
  }
}

void ezSound::waitForSilence(uint16_t msec /* = 0 */) {
  while (!silence(msec)) {
    update();
    yield();
  }
}

bool ezSound::silence(uint16_t msec /* = 0 */) {
  if (!_silentSince) return false;
  if (millis() - _silentSince >= msec) return true;
  return false;
}


void ezSound::update() {
  bool silent = true;
  // If last packet is gone, make a new one
  if (!_bytes_left) {
    // Ask synths what they have and mix in 32-bit signed mix buffer
    memset(_mixbuf, 0, BUFLEN * 4);   // 32 bits
    for (auto synth : ezSynth::instances) {
      if (synth->fillSbuf()) {
        silent = false;
        for (uint16_t i = 0; i < BUFLEN; i++) {
          _mixbuf[i] += synth->_sbuf[i];
        }
      }
    }
    for (uint16_t i = 0; i < BUFLEN; i++) {
      int32_t m = _mixbuf[i];
      // clip to 16-bit signed so we get "real" distortion not noise
      if (m <= -32768) m = -32768;
       else if (m >= 32767) m = 32767;
      _buf[i * 2] = m;
      _buf[(i * 2) + 1] = m;
    }
    _bytes_left = BUFLEN * 4;
  }

  if (!silent || !AMP_STANDBY) {
    _silentSince = 0;
    if (!_amp_on) {
      log_d("%s", "Audio amplifier on");
      M5.Axp.SetSpkEnable(true);
      _amp_on = true;
    }
  }
  if ( silent && !_silentSince) _silentSince = millis();
  if (_amp_on && AMP_STANDBY && _silentSince &&
      millis() - _silentSince > AMP_STANDBY) {
    log_d("%s", "Audio amplifier off");
    M5.Axp.SetSpkEnable(false);
    _amp_on = false;
  }

  if (_bytes_left) {
    // Send what can be sent but don't hang around, send rest next time.
    size_t bytes_written = 0;
    i2s_write(I2S_NUM_0, _buf + (BUFLEN * 4) - _bytes_left, _bytes_left,
              &bytes_written, 0);
    _bytes_left -= bytes_written;
  }
}


/////////////////////////////////////////////////////////////////////////////
//
// ezSynth
//
/////////////////////////////////////////////////////////////////////////////

/* static */ std::vector<ezSynth*> ezSynth::instances;

ezSynth::ezSynth(waveform_t waveform_ /* = SINE */, float freq_ /* = 0 */,
             uint16_t attack_ /* = ATTACK */, uint16_t decay_ /* = DECAY */,
             float sustain_ /* = SUSTAIN */, uint16_t release_ /* = RELEASE */,
             float gain_ /* = GAIN */) {
  waveform = waveform_;
  freq = freq_;
  attack = attack_;
  decay = decay_;
  sustain = sustain_;
  release = release_;
  gain = gain_;
  phase = 0;
  instances.push_back(this);
  startTime = stopTime = 0;
}

ezSynth::~ezSynth() {
  for (int i = 0; i < instances.size(); ++i) {
    if (instances[i] == this) {
      instances.erase(instances.begin() + i);
      return;
    }
  }
}

bool ezSynth::fillSbuf() {
  if (!freq || !startTime) return false;

  // Envelope
  uint32_t duration = millis() - startTime;
  if (duration < attack) {
    envelope = _startEnvelope +
               (((float)duration / attack) * (1 - _startEnvelope));
  } else if (decay && sustain < 1 && duration < attack + decay) {
    envelope = 1 - (((float)(duration - attack) / decay) * (1 - sustain));
  } else {
    envelope = sustain;
  }
  if (stopTime && millis() > stopTime) {
    uint32_t stopping_for = millis() - stopTime;
    if (stopping_for < release) {
      envelope = sustain - (((float)stopping_for / release) * sustain);
    } else {
      startTime = stopTime = 0;
      envelope = 0;
      phase = 0;
    }
  }

  // Waveform
  uint16_t amplitude = scaleAmplitude(gain * envelope);
  float steps = ((float)freq / SAMPLERATE);
  switch (waveform) {
    case SINE:
      for(uint16_t i = 0; i < BUFLEN; i++) {
        uint8_t t = (uint16_t)((phase + (i * steps)) * 100) % 100;
        _sbuf[i] = ezSound::instance->_sineTable[t] * amplitude;
      }
      break;
    case SQUARE:
      for(uint16_t i = 0; i < BUFLEN; i++) {
        phase + (i * steps);
        _sbuf[i] = (phase + (i * steps) > 50 ? -1 : 1) * amplitude;
      }
      break;
    case TRIANGLE:
      for(uint16_t i = 0; i < BUFLEN; i++) {
        float t = phase + (i * steps);
        t -= (int)t;
        if (t < 0.25) t *= 4;
         else if (t < 0.75) t = 2 - (t * 4);
          else t = -4 + (t * 4);
        _sbuf[i] = t * amplitude;
      }
      break;
    case SAWTOOTH:
      for(uint16_t i = 0; i < BUFLEN; i++) {
        float t = phase + (i * steps);
        t -= (int)t;
        if (t < 0.5) t *= 2;
         else t = -2 + (t * 2);
        _sbuf[i] = t * amplitude;
      }
      break;
    case NOISE:
      for(uint16_t i = 0; i < BUFLEN; i++) {
        _sbuf[i] = (rand() % (2 * amplitude)) - amplitude;
      }
      break;
  }
  phase += (float)(BUFLEN) * steps;
  phase -= (int)phase;
  return true;
}

void ezSynth::start() {
  startTime = millis();
  _startEnvelope = envelope;
  stopTime = 0;
}

void ezSynth::stop() {
  // At least attack, decay and release get to play
  stopTime = startTime + attack + decay;
  if (millis() > stopTime) stopTime = millis() + 10;
}

void ezSynth::playFor(uint32_t msec) {
  uint32_t duration = attack + decay + release;
  if (msec > duration) duration = msec;
  startTime = millis();
  stopTime  = millis() + duration;
}

int16_t ezSynth::scaleAmplitude(float gain) {
  // 181 is (almost) the square root of 32768.
  return pow(gain * 181, 2);
}
