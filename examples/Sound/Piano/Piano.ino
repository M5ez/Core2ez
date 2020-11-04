/*

Polyphonic synthesizer. Lingering notes will continue to sound as new notes
are played. If you place one finger high on the screen and another low, you
can even play two notes at the same time. As you can see you can have many
synths going at the same time.

*/

#include <M5Core2.h>
#include <Core2ez.h>


#define KEYS                14
#define WHITE_KEY_WIDTH     40
#define WHITE_KEY_HEIGHT   240
#define BLACK_KEY_WIDTH     30
#define BLACK_KEY_HEIGHT   160
#define WHITE_KEY_COLORS    {WHITE, NODRAW, BLACK}
#define BLACK_KEY_COLORS    {BLACK, NODRAW, NODRAW}

ezButton keys[KEYS];

// Set up the synthesizers . Arguments:
//    waveform (SINE | SQUARE | TRIANGLE | SAWTOOTH | NOISE), freq (Hz),
//    attack (ms), decay (ms), sustain (float), release (ms), gain (float)
ezSynth synth[KEYS] = ezSynth(TRIANGLE, 0, 50, 300, 0.6, 1000, 0.4);


void setup() {
  ez.begin();

  ez.Screen.colors.fill = BLACK;

  ez.Screen.spriteBuffer();      // Prevents flicker, buffer until push()
  ez.Screen.glissando = true;

  // Set up white keys

  for (uint8_t n = 0; n < 8; n++) {
    keys[n].set(n * WHITE_KEY_WIDTH, 0, WHITE_KEY_WIDTH, WHITE_KEY_HEIGHT);
    keys[n].colors   = WHITE_KEY_COLORS;
    keys[n].onColors = {NODRAW, NODRAW, NODRAW};
    keys[n].userData = n;
    ez.Screen.add(keys[n]);
  }

  // Set up Black keys
  // ( not on every white key, hardcoding their X positions is easiest )
  const uint16_t blackKeyX[6]   = { 25, 65, 105, 185, 225, 305 };
  for (uint8_t n = 8; n < 14; n++) {
    keys[n].set(blackKeyX[n - 8], 0, BLACK_KEY_WIDTH, BLACK_KEY_HEIGHT);
    keys[n].colors   = BLACK_KEY_COLORS;
    keys[n].onColors = {NODRAW, NODRAW, NODRAW};
    keys[n].userData = n;
    ez.Screen.add(keys[n]);
  }

  ez.Screen.focus();
  ez.Screen.fillRect(0, 0, 320, 5, BLACK);   // top of keys as straight line
  ez.Screen.push();

  // Set up synths with their notes
  const float notes[KEYS] = { NOTE_F4, NOTE_G4, NOTE_A4, NOTE_B4, NOTE_C5,
                              NOTE_D5, NOTE_E5, NOTE_F5, NOTE_Gb4, NOTE_Ab4,
                              NOTE_Bb4, NOTE_Db5, NOTE_Eb5, NOTE_Gb5 };
  for (uint8_t n = 0; n < 14; n++) synth[n].freq = notes[n];

  ez.Screen.addHandler(keyHandler, E_TOUCH + E_RELEASE);
}

void loop() {
  ez.update();
}

void keyHandler(Event& e) {
  if (!e.widget) return;
  uint8_t key = e.widget->userData;
  if (e == E_TOUCH) {
    synth[key].start();
  } else {
    synth[key].stop();
  }
}
