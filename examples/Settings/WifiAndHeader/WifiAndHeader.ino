#include <M5Core2.h>
#include <Core2ez.h>


void setup() {
  ez.begin();

  while (true) {
    msgBox(
      "Wifi / status bar",
      "As you can see there is a status bar above this message.",
      "next"
    );
    msgBox(
      "Wifi / status bar",
      "If you swipe down from it, you're in the settings menu, "
      "which right now only has the wifi settings in it.",
      "next"
    );
    msgBox(
      "Wifi / status bar",
      "Try and see if you can connect to your router.",
      "back"
    );
  }
         
}

void loop() {
  ez.update();
}
