#include <M5Core2.h>
#include <Core2ez.h>

void setup() {
  ez.begin();
  ez.Screen.spriteBuffer();
}

void loop() {
  ez.update();
  if (ez.Touch.changed) {
    ez.Screen.clear();
    ez.Screen.fillCircle(ez.Touch.point[0], 50, RED);
    ez.Screen.fillCircle(ez.Touch.point[1], 50, BLUE);
    ez.Screen.push();
  }
}
