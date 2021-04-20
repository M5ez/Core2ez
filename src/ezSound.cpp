#include "ezSound.h"

#include <M5Core2.h>
#include <driver/i2s.h>


/////////////////////////////////////////////////////////////////////////////
//
// ezSoundSource
//
/////////////////////////////////////////////////////////////////////////////

/* static */ std::vector<ezSoundSource*> ezSoundSource::instances;

ezSoundSource::ezSoundSource() {
  instances.push_back(this);
}

ezSoundSource::~ezSoundSource() {
  for (int i = 0; i < instances.size(); ++i) {
    if (instances[i] == this) {
      instances.erase(instances.begin() + i);
      return;
    }
  }
}

/* virtual */ uint16_t ezSoundSource::read(int16_t* buffer, uint16_t size) {
  return 0;
}

bool ezSoundSource::playing() {
  return _playing;
}



/////////////////////////////////////////////////////////////////////////////
//
// ezSoundSink
//
/////////////////////////////////////////////////////////////////////////////

/* static */ ezSoundSink* ezSoundSink::recordingInstance = NULL;

ezSoundSink::~ezSoundSink() {
  if (recordingInstance == this) {
    recordingInstance = NULL;
  }
}

// virtual, overridden by inheriting classes
/* static */ void ezSoundSink::update() {}

bool ezSoundSink::recording() {
  return (recordingInstance == this);
}



/////////////////////////////////////////////////////////////////////////////
//
// ezRecord
//
/////////////////////////////////////////////////////////////////////////////

// ezRecord::ezRecord(const char* path, uint32_t samplerate /* = 0 */);
//
// ezRecord::ezRecord(const String& path, uint32_t samplerate /* = 0 */);
//
// void ezRecord::record();
//
// void ezRecord::stop();
//
// void ezRecord::update();
//
// float ezRecord::position();
//
// bool ezRecord::_write(uint8_t* buffer, uint32_t length);



/////////////////////////////////////////////////////////////////////////////
//
// ezSoundClass
//
/////////////////////////////////////////////////////////////////////////////

void ezSoundClass::begin() {
  // populate sinewave table
  for (uint8_t n = 0; n < 100; n++) {
    _sineTable[n] = sin((float)n * EZ_TWO_PI / 100);
  }
  start();
}

void ezSoundClass::start() {
  #ifndef I2S_FULL_DUPLEX
    EZSOUNDIN.stop();
  #endif
  // Set up I2S driver
  i2s_driver_uninstall(TX_DEV);
  i2s_config_t tx_config = {
    .mode = TX_MODE,
    .sample_rate = samplerate,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = TX_CHANNEL_FORMAT,
    .communication_format = I2S_COMM_FORMAT_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = TX_DMA_BUF_COUNT,
    .dma_buf_len = TX_DMA_BUF_LEN,
    .use_apll = true,
    .tx_desc_auto_clear = true
  };
  i2s_driver_install(TX_DEV, &tx_config, 0, NULL);
  // Configure pins
  i2s_pin_config_t tx_pin_config= {
    .bck_io_num = TX_BCLK,
    .ws_io_num = TX_LRC,
    .data_out_num = TX_DOUT,
    .data_in_num = I2S_PIN_NO_CHANGE
  };
  i2s_set_pin(TX_DEV, &tx_pin_config);
  i2s_set_sample_rates(TX_DEV, samplerate);
  _silentSince = millis();
  amplifier(!(AMP_STANDBY), true);
  running = true;
}

void ezSoundClass::stop() {
  i2s_driver_uninstall(TX_DEV);
  amplifier(false, true);
  running  = false;
}

void ezSoundClass::update() {
  bool silent = true;
  if (running) {
    do {
      // If last packet is gone, make a new one
      if (!_bytes_left) {
        // Ask synths what they have and mix in 32-bit signed mix buffer
        memset(_mixbuf, 0, TX_CHUNKSIZE * 4);   // 32 bits
        for (auto source : ezSoundSource::instances) {
          if (source->playing() && source->read(_tmpbuf, TX_CHUNKSIZE)) {
            silent = false;
            for (uint16_t i = 0; i < TX_CHUNKSIZE; i++) {
              _mixbuf[i] += _tmpbuf[i];
            }
          }
        }
        for (uint16_t i = 0; i < TX_CHUNKSIZE; i++) {
          int32_t m = _mixbuf[i];
          // clip to 16-bit signed so we get "real" distortion not noise
          m = max(min(m, 32767), -32768);
          _outbuf[i * TX_CHANNELS] = m;
          if (TX_CHANNELS == 2) {
            _outbuf[(i * TX_CHANNELS) + 1] = m;
          }
        }
        _bytes_left = TX_CHUNKSIZE * 2 * TX_CHANNELS;
      }
      if (_bytes_left) {
        // Send what can be sent but don't hang around, send rest next time.
        size_t bytes_written = 0;
        i2s_write(TX_DEV, _outbuf + (TX_CHUNKSIZE * 2 * TX_CHANNELS) - _bytes_left, _bytes_left,
                  &bytes_written, 0);
        _bytes_left -= bytes_written;
      }
    } while (!_bytes_left);
  }

  // Processing that happens even if not 'running': amplifier timeout
  if (!silent) {
    _silentSince = 0;
    amplifier(true);
  }
  if (silent && !_silentSince) _silentSince = millis();
  if (AMP_STANDBY && _silentSince && millis() - _silentSince > AMP_STANDBY) {
    amplifier(false);
  }

  // Call recording sink if present
  if(ezSoundSink::recordingInstance) {
    ezSoundSink::recordingInstance->update();
  }
}

void ezSoundClass::delay(uint16_t msec) {
  uint32_t start = millis();
  while (millis() - start < msec) {
    update();
    yield();
  }
}

void ezSoundClass::waitForSilence(uint16_t msec /* = 0 */) {
  while (!silence(msec)) {
    update();
    yield();
  }
}

bool ezSoundClass::silence(uint16_t msec /* = 0 */) {
  if (!_silentSince) return false;
  if (millis() - _silentSince >= msec) return true;
  return false;
}

void ezSoundClass::amplifier(bool state, bool force /* = false */) {
  if (state && (!_amp_on || force)) {
    log_d("Audio amplifier on");
    M5.Axp.SetSpkEnable(true);
    _amp_on = true;
  }
  if (!state && (_amp_on || force)) {
    log_d("Audio amplifier off");
    M5.Axp.SetSpkEnable(false);
    _amp_on = false;
  }
}



/////////////////////////////////////////////////////////////////////////////
//
// ezSoundInClass
//
/////////////////////////////////////////////////////////////////////////////

void ezSoundInClass::begin() {
  #ifndef I2S_FULL_DUPLEX
    start();
  #endif
}

void ezSoundInClass::start() {
  #ifndef I2S_FULL_DUPLEX
    EZSOUND.stop();
  #endif
  i2s_driver_uninstall(RX_DEV);
  // Set up I2S driver
  i2s_config_t rx_config = {
    .mode = RX_MODE,
    .sample_rate = samplerate,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = RX_CHANNEL_FORMAT,
    .communication_format = I2S_COMM_FORMAT_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = RX_DMA_BUF_COUNT,
    .dma_buf_len = RX_DMA_BUF_LEN,
    .use_apll = true
  };
  i2s_driver_install(RX_DEV, &rx_config, 0, NULL);
  // Configure pins
  i2s_pin_config_t rx_pin_config= {
    .bck_io_num = RX_BCLK,
    .ws_io_num = RX_LRC,
    .data_out_num = I2S_PIN_NO_CHANGE,
    .data_in_num = RX_DIN
  };
  i2s_set_pin(RX_DEV, &rx_pin_config);
  i2s_set_sample_rates(RX_DEV, samplerate);
  running = true;
}

void ezSoundInClass::stop() {
  i2s_driver_uninstall(RX_DEV);
  running = false;
}

uint16_t ezSoundInClass::read(int16_t* buffer, uint16_t size) {
  if (!running) return 0;
  size_t bytes_read = 0;
  i2s_read(RX_DEV, buffer, size, &bytes_read, 0);
  return (uint16_t)bytes_read / 2;
}



/////////////////////////////////////////////////////////////////////////////
//
// ezSynth
//
/////////////////////////////////////////////////////////////////////////////

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
  startTime = stopTime = 0;
}

uint16_t ezSynth::read(int16_t* buffer, uint16_t size) {
  if (!freq || !startTime || size != TX_CHUNKSIZE) return 0;

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
      _playing = false;
      return 0;
    }
  }

  // Waveform
  // Gain efffect quadratic. 181 is (almost) the square root of 32768.
  uint16_t amplitude = pow((float)((gain * envelope) * 181), 2);
  float steps = ((float)freq / EZSOUND.samplerate);
  switch (waveform) {
    case SINE:
      for(uint16_t i = 0; i < size; i++) {
        uint8_t t = (uint16_t)((phase + (i * steps)) * 100) % 100;
        buffer[i] = EZSOUND._sineTable[t] * amplitude;
      }
      break;
    case SQUARE:
      for(uint16_t i = 0; i < size; i++) {
        phase + (i * steps);
        buffer[i] = (phase + (i * steps) > 50 ? -1 : 1) * amplitude;
      }
      break;
    case TRIANGLE:
      for(uint16_t i = 0; i < size; i++) {
        float t = phase + (i * steps);
        t -= (int)t;
        if (t < 0.25) t *= 4;
         else if (t < 0.75) t = 2 - (t * 4);
          else t = -4 + (t * 4);
        buffer[i] = t * amplitude;
      }
      break;
    case SAWTOOTH:
      for(uint16_t i = 0; i < size; i++) {
        float t = phase + (i * steps);
        t -= (int)t;
        if (t < 0.5) t *= 2;
         else t = -2 + (t * 2);
        buffer[i] = t * amplitude;
      }
      break;
    case NOISE:
      for(uint16_t i = 0; i < size; i++) {
        buffer[i] = (rand() % (2 * amplitude)) - amplitude;
      }
      break;
  }
  phase += (float)(size) * steps;
  phase -= (int)phase;
  return size;
}

void ezSynth::start() {
  startTime = millis();
  _startEnvelope = envelope;
  stopTime = 0;
  _playing = true;
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
  _playing = true;
}


ezSoundClass& ezSound = EZSOUND;
ezSoundInClass& ezSoundIn = EZSOUNDIN;
