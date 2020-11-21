#include <M5Core2.h>
#include <Core2ez.h>

uint16_t counter = 0;
char tmp[5];

ezLabel ctrLbl = ezLabel(0, 0, 320, 240, "0000", {WHITE, BLACK, NODRAW}, NUMONLY7SEG96, EZ_CENTER, EZ_CENTER);
ezGesture swipeDown = ezGesture(100, EZ_DOWN);

void setup() {
  ez.begin();
  ez.Screen.spriteBuffer();     // Prevents screen flicker when updating number
  ez.Screen.longPressTime = 1500;
}

void loop() {
  ez.update();
}

ON(ez.Screen, E_TOUCH) {
  counter++;
  sprintf(tmp, "%04d", counter);
  ctrLbl.text = tmp;
  ctrLbl.draw();
  ctrLbl.refresh();
}

ON(ez.Screen, E_LONGPRESSING) {
  counter = 0;
  ctrLbl.text = "0000";
  ctrLbl.draw();
  ctrLbl.refresh();
}
