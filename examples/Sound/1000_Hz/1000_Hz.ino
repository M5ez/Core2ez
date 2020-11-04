#include <M5Core2.h>
#include <Core2ez.h>

ezSynth s(SINE, 1000);

ezButton btn(50, 80, 220, 80, "1000 Hz");

void setup() {
	ez.begin();
  btn.label.font = FSSB24;
}

void loop() {
	ez.update();
  if ( btn.wasPressed()  ) s.start();
  if ( btn.wasReleased() ) s.stop();
}
