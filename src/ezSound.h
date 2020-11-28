#ifndef _M5SOUND_H_
#define _M5SOUND_H_

#include <Arduino.h>

#define I2S_DOUT           2
#define I2S_BCLK          12
#define I2S_LRC            0
#define DMA_BUF_COUNT      4
#define DMA_BUF_LEN      256
#define SAMPLERATE     24000
#define BUFLEN            32
#define TWO_PI             6.283185307179586

// switches off amplifier chip after 1 sec. Set to 0 to keep amplifier on.
#define AMP_STANDBY     1000


// defaults
#define ATTACK            10      // prevents clicks
#define DECAY              0
#define SUSTAIN            1.0
#define RELEASE           10      // prevents clicks
#define GAIN               0.5


// Musical notes, 440 Hz tuning
#define NOTE_C3     130.81
#define NOTE_Db3    138.59
#define NOTE_D3     146.83
#define NOTE_Eb3    155.56
#define NOTE_E3     164.81
#define NOTE_F3     174.61
#define NOTE_Gb3    185.00
#define NOTE_G3     196.00
#define NOTE_Ab3    207.65
#define NOTE_A3     220
#define NOTE_Bb3    233.08
#define NOTE_B3     246.94
#define NOTE_C4     261.63
#define NOTE_Db4    277.18
#define NOTE_D4     293.66
#define NOTE_Eb4    311.13
#define NOTE_E4     329.63
#define NOTE_F4     349.23
#define NOTE_Gb4    369.99
#define NOTE_G4     392.00
#define NOTE_Ab4    415.30
#define NOTE_A4     440
#define NOTE_Bb4    466.19
#define NOTE_B4     493.88
#define NOTE_C5     523.25
#define NOTE_Db5    554.37
#define NOTE_D5     587.33
#define NOTE_Eb5    622.25
#define NOTE_E5     659.25
#define NOTE_F5     698.46
#define NOTE_Gb5    739.99
#define NOTE_G5     783.99
#define NOTE_Ab5    830.61
#define NOTE_A5     880
#define NOTE_Bb5    932.33
#define NOTE_B5     987.77
#define NOTE_C6    1046.50
#define NOTE_Db6   1108.73
#define NOTE_D6    1174.66
#define NOTE_Eb6   1244.51
#define NOTE_E6    1318.51
#define NOTE_F6    1396.91
#define NOTE_Gb6   1479.98
#define NOTE_G6    1567.98
#define NOTE_Ab6   1661.22
#define NOTE_A6    1760
#define NOTE_Bb6   1864.66
#define NOTE_B6    1975.53



enum waveform_t {
  SINE,
  SQUARE,
  TRIANGLE,
  SAWTOOTH,
  NOISE
};

class ezSoundClass {
 public:
  static ezSoundClass& instance() {
    static ezSoundClass INSTANCE;
    return INSTANCE;
  }
  ezSoundClass(ezSoundClass const&)    = delete;
  void operator=(ezSoundClass const&)  = delete;
 private:
  ezSoundClass() {}

 public:
  void begin();
  void update();
  void delay(uint16_t msec);
  void waitForSilence(uint16_t msec = 0);
  bool silence(uint16_t msec = 0);
 protected:
  friend class ezSynth;   // for _sineTable
  float _sineTable[100];
  uint32_t _silentSince;
  int16_t _buf[BUFLEN * 2];
  int32_t _mixbuf[BUFLEN];
  size_t _bytes_left          = 0;
  bool _amp_on                = false;
};

class ezSynth {
 public:
  static std::vector<ezSynth*> instances;
  ezSynth(waveform_t waveform_ = SINE, float freq_ = 0,
          uint16_t attack_ = ATTACK, uint16_t decay_ = DECAY,
          float sustain_ = SUSTAIN, uint16_t release_ = RELEASE,
          float gain_ = GAIN);
  ~ezSynth();
  bool fillSbuf();
  void start();
  void stop();
  void playFor(uint32_t msec);
  waveform_t waveform;
  float freq, gain, envelope, sustain, phase;
  uint16_t attack, decay, release;
  uint32_t startTime, stopTime;
 protected:
  friend class ezSoundClass;
  inline int16_t scaleAmplitude(float gain);
  float _startEnvelope;
  int16_t _sbuf[BUFLEN];
};

extern ezSoundClass& ezSound;

#endif /* _M5SOUND_H_ */
