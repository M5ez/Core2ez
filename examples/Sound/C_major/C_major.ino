#include <M5Core2.h>
#include <Core2ez.h>

// Waveform, Frequency, Attack, Decay, Sustain, Release, Gain
ezSynth c5(SINE, NOTE_C5, 50, 300, 0.7, 1000);
ezSynth e5(SINE, NOTE_E5, 50, 300, 0.7, 1000);
ezSynth g5(SINE, NOTE_G5, 50, 300, 0.7, 1000);

ezButton playButton(50, 80, 220, 80, "C major");

void setup() {
  ez.begin();
  playButton.setFont(FSSB24);
}

void loop() {
  ez.update();
  if (playButton.wasPressed()) playChord();
}

void playChord() {
  c5.playFor(5000);
  ez.Sound.delay(1000);
  e5.playFor(4000);
  ez.Sound.delay(1000);
  g5.playFor(3000);
  ez.Sound.waitForSilence();
}
