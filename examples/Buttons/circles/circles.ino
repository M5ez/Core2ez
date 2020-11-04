#include <M5Core2.h>
#include <Core2ez.h>

void setup() {
  ez.begin();
  M5.Lcd.fillScreen(TFT_WHITE);
}

void loop() {
  ez.update();
  Event& e = ez.lastEvent;
  if (e & (E_MOVE | E_RELEASE)) circle(e & E_MOVE ? e.from : e.to, WHITE);
  if (e & (E_TOUCH | E_MOVE)) circle(e.to, e.finger ? BLUE : RED);
}

void circle(Point p, uint16_t c) {
  M5.Lcd.drawCircle(p.x, p.y, 50, c);
  M5.Lcd.drawCircle(p.x, p.y, 52, c);
}
