#include "ezHeader.h"

#include <ez.h>

ezHeaderClass::ezHeaderClass() {
  set(0, EZ_AUTO, EZ_PARENT, ezTheme.hdr_height);
  type       = W_HEADER;
  colors     = ezTheme.hdr_colors;
  title.font = ezTheme.hdr_font;

  add(title);
#ifdef _EZWIFI_H_
  add(wifi);
#endif

//   add(swipeDown);
//   swipeDown.on(E_GESTURE, doFunction {
//     Serial.println("Settings!!");
//     ezHeaderClass::instance().settings();
//   });
}

void ezHeaderClass::draw() {
  if (parent()) title.text = static_cast<ezWindow*>(parent())->title;
  ezWidget::draw();
}

bool ezHeaderClass::displayed() {
  if (ez._widgets.size() && ez._widgets.back()->_widgets[0] == this) return true;
  return false;
}

void ezHeaderClass::settings() {
//   if (_inSettings) return;
//   _inSettings = true;
//   ezMenu settingsMenu("Settings");
//
// #ifdef _EZWIFI_H_
//   settingsMenu.addItem("Wifi", doFunction { ezWifi.menu(); });
// #endif
//
//   settingsMenu.addItem("Exit Settings", doFunction { ezQuit(); });
//   settingsMenu.run();
//   _inSettings = false;
}

ezHeaderClass& ezHeader = ezHeaderClass::instance();
