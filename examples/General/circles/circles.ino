#include <M5Core2.h>
#include <Core2ez.h>

void setup() {
  ez.begin();
  ezScreen.spriteBuffer();
}

void loop() {
  ez.update();
  if (ezTouch.changed) {
    ezScreen.clear();
    ezScreen.fillCircle(ezTouch.point[0], 50, RED);
    ezScreen.fillCircle(ezTouch.point[1], 50, BLUE);
    ezScreen.push();
  }
}
