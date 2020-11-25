#include <M5Core2.h>
#include <Core2ez.h>

void setup() {
  ez.begin();
  Screen.spriteBuffer();
}

void loop() {
  ez.update();
  if (Touch.changed) {
    Screen.clear();
    Screen.fillCircle(Touch.point[0], 50, RED);
    Screen.fillCircle(Touch.point[1], 50, BLUE);
    Screen.push();
  }
}
