#include <M5Core2.h>
#include <Core2ez.h>

uint16_t counter = 0;
char tmp[5];

ezLabel label = ezLabel(0, 0, 320, 240, "0000", {WHITE, BLACK, NODRAW},
                        NUMONLY7SEG96, EZ_CENTER, EZ_CENTER);

void setup() {
  ez.begin();
  label.spriteBuffer();     // Prevents screen flicker when updating number
  ezScreen.longPressTime = 1500;
  label.draw();
  label.refresh();

  ezScreen.on(E_TOUCH, doFunction {
    counter++;
    sprintf(tmp, "%04d", counter);
    label.text = tmp;
    label.draw();
    label.refresh();
  });

  ezScreen.on(E_LONGPRESSING, doFunction {
    counter = 0;
    label.text = "0000";
    label.draw();
    label.refresh();
  });

}

void loop() {
  ez.update();
}


