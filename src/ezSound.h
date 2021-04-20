#ifndef _EZSOUND_H_
#define _EZSOUND_H_

#include <Arduino.h>

// Full-duplex does not work on systems that share the clock pins between input and output.
// (Or so I believe. If you have it working, please file an issue to let me know how.)
// #define I2S_FULL_DUPLEX

#define SAMPLERATE  16000

#define TX_DEV               I2S_NUM_0
#define TX_DOUT              2
#define TX_BCLK             12
#define TX_LRC               0
#define TX_MODE              (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX)
#define TX_CHANNELS          1
#define TX_CHANNEL_FORMAT    I2S_CHANNEL_FMT_ONLY_RIGHT
#define TX_DMA_BUF_COUNT     4
#define TX_DMA_BUF_LEN     128
#define TX_CHUNKSIZE        32


#define RX_DEV               I2S_NUM_0
#define RX_DIN              34
#define RX_BCLK             12
#define RX_LRC               0
#define RX_MODE              (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX |I2S_MODE_PDM)
#define RX_CHANNELS          1
#define RX_CHANNEL_FORMAT    I2S_CHANNEL_FMT_ONLY_RIGHT
#define RX_DMA_BUF_COUNT     4
#define RX_DMA_BUF_LEN     128


#define EZ_TWO_PI            6.283185307179586

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


class ezSoundSource {
 public:
  static std::vector<ezSoundSource*> instances;
  ezSoundSource();
  ~ezSoundSource();
  virtual uint16_t read(int16_t* buffer, uint16_t size);
  bool playing();
 protected:
  bool _playing = false;
};

class ezSoundSink {
 public:
  static ezSoundSink* recordingInstance;
  ~ezSoundSink();
  virtual void update();
  bool recording();
};


// class ezPlay : public ezSoundSource {
//  public:
//   ezPlay(uint8_t* rawSamples, uint32_t length, uint32_t sampleRate);
//   ezPlay(uint8_t* ROMwav);
//   ezPlay(const char* path);
//   ezPlay(const String& path);
//   void play();
//   void pause();
//   void gain(float newGain);
//   uint32_t samplerate();
//   float length();
//   float position(float secs);
//   float position();
// }

// class ezRecord : public ezSoundSink {
//  public:
//   ezRecord(const char* path, uint32_t samplerate = 0);
//   ezRecord(const String& path, uint32_t samplerate = 0);
//   void record();
//   void stop();
//   void update();
// //  void gain(float newGain);
//   float position();
//   bool _write(uint8_t* buffer, uint32_t length);
// };

class ezSynth : public ezSoundSource {
 public:
  ezSynth(waveform_t waveform_ = SINE, float freq_ = 0,
          uint16_t attack_ = ATTACK, uint16_t decay_ = DECAY,
          float sustain_ = SUSTAIN, uint16_t release_ = RELEASE,
          float gain_ = GAIN);
  virtual uint16_t read(int16_t* buffer, uint16_t size);
  void start();
  void stop();
  void playFor(uint32_t msec);
  waveform_t waveform;
  float gain, freq, envelope, sustain, phase;
  uint16_t attack, decay, release;
  uint32_t startTime, stopTime;
 protected:
  float _startEnvelope;
};

#define EZSOUND ezSoundClass::instance()
class ezSoundClass{
 // Singleton stuff
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
  void start();
  void stop();
  void update();
  void reg(ezSoundSource* source);
  void dereg(ezSoundSource* source);
  void delay(uint16_t msec);
  void waitForSilence(uint16_t msec = 0);
  bool silence(uint16_t msec = 0);
  void amplifier(bool state, bool force = false);
  uint32_t samplerate = SAMPLERATE;
  bool running = false;

 protected:
  friend class ezSynth;   // for _sineTable
  float _sineTable[100];
  uint32_t _silentSince;
  int16_t _tmpbuf[TX_CHUNKSIZE];
  int16_t _outbuf[TX_CHUNKSIZE * TX_CHANNELS];
  int32_t _mixbuf[TX_CHUNKSIZE];
  size_t _bytes_left          = 0;
  bool _amp_on                = false;
};

#define EZSOUNDIN ezSoundInClass::instance()
class ezSoundInClass : public ezSoundSource {
 // Singleton stuff
 public:
  static ezSoundInClass& instance() {
    static ezSoundInClass INSTANCE;
    return INSTANCE;
  }
  ezSoundInClass(ezSoundInClass const&)  = delete;
  void operator=(ezSoundInClass const&)  = delete;
 private:
  ezSoundInClass() {}

 public:
  void begin();
  void start();
  void stop();
  void update();
  virtual uint16_t read(int16_t* buffer, uint16_t size);
  uint32_t samplerate = SAMPLERATE;
  bool running = false;

 protected:
  int16_t _read_tmp[RX_DMA_BUF_LEN / 2];
};

extern ezSoundClass& ezSound;
extern ezSoundInClass& ezSoundIn;

#endif /* _M5SOUND_H_ */
