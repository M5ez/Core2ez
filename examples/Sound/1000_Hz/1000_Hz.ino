#include <M5Core2.h>
#include <Core2ez.h>

ezSynth s(SINE, 1000);

//            x   y    w   h     label   when off      while pressed  font
ezButton btn(50, 80, 220, 80, "1000 Hz", THEME_COLORS, THEME_COLORS,  FSSB24);

void setup() {
	ez.begin();
}

void loop() {
	ez.update();
}

ON(btn, E_TOUCH  ) { s.start() ; }

ON(btn, E_RELEASE) { s.stop () ; }
