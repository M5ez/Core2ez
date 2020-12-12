#include <M5Core2.h>
#include <Core2ez.h>

#define MARGIN            6

const uint16_t rowTones[] = { 697,  770,  852,  941};
const uint16_t colTones[] = {1209, 1336, 1477, 1633};
const char* keyLabels[]   = {"1", "2", "3", "A",
                             "4", "5", "6", "B",
                             "7", "8", "9", "C",
                             "*", "0", "#", "D"};
WidgetColors  offColors   = {BLUE, WHITE, WHITE};
WidgetColors  onColors    = {WHITE, BLACK, NODRAW};

ezButton      key[16];
ezSynth       row_gen;
ezSynth       col_gen;
uint32_t      rotationLastChecked = 0;
uint8_t       columns = 4;

void setup() {
  ez.begin();
  ezScreen.colors.fill = BLACK;
  ezScreen.glissando = true;
  M5.IMU.Init();
  row_gen.gain  = col_gen.gain   = 0.3;
  row_gen.decay = col_gen.decay = 50;   // min tone length

  ez.on(E_TOUCH, doFunction {
    if (eventWidget(ezButton, b)) {
      if (!b->userData) return;
      uint8_t k = b->userData - 1;
      ezSound.waitForSilence(50);
      row_gen.freq = rowTones[k / 4];
      col_gen.freq = colTones[k % 4];
      row_gen.start();
      col_gen.start();
    }
  });

  ez.on(E_RELEASE, doFunction {
    row_gen.stop();
    col_gen.stop();
  });

  doButtons();
}

void loop() {
  ez.update();
  if (checkRotation(1000)) doButtons();
}

void doButtons() {
  uint16_t scr_w = M5.Lcd.width();
  uint16_t scr_h = M5.Lcd.height();
  ezScreen.set(0, 0, scr_w, scr_h);
  uint8_t  btn_w = (scr_w / columns) - MARGIN;
  uint8_t  btn_h = (scr_h / 4) - MARGIN;

  // Hide the last column if needed
  for (uint8_t i = 3; i < 16; i += 4) {
     if (columns == 3) key[i].set();
  }

  // Set up all the keys
  for (uint8_t r = 0; r < 4; r++) {
    for (uint8_t c = 0; c < columns; c++) {
      uint8_t i = (r * 4) + c;
      key[i].set(
        c * (scr_w / columns) + (MARGIN / 2),
        r * (scr_h / 4) + (MARGIN / 2),
        btn_w,
        btn_h
      );
      key[i].colors     = offColors;
      key[i].onColors   =  onColors;
      key[i].label.font = FSSB18;
      key[i].label.text = keyLabels[i];
      key[i].userData   = i + 1;
    }
  }

  // Strictly cosmetic: "*" char in font too small and too high
  key[12].label.font = FSSB24;
  key[12].label.dy = 8;

  ezScreen.focus();
}

bool checkRotation(uint16_t msec) {
  if (millis() - rotationLastChecked < msec) return false;
  rotationLastChecked = millis();
  const float threshold = 0.85;
  float ax, ay, az;
  M5.IMU.getAccelData(&ax, &ay, &az);
  uint8_t newRotation;
  if      (ay >  threshold) newRotation = 1;
  else if (ay < -threshold) newRotation = 3;
  else if (ax >  threshold) newRotation = 2;
  else if (ax < -threshold) newRotation = 0;
  else return false;
  if (M5.Lcd.rotation == newRotation) return false;
  columns = newRotation % 2 ? 4 : 3;
  M5.Lcd.clearDisplay();
  M5.Lcd.setRotation(newRotation);
  return true;
}
