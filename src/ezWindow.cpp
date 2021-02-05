#include "ezWindow.h"

#include <ezTheme.h>
#include <ez.h>

/* static */ uint8_t ezWindow::_quitting = 0;

/* static */ void ezWindow::quit(uint8_t count /* = 1 */) {
  _quitting = count;
}

ezWindow::ezWindow(ezWidget& parentWidget,
                   int16_t x_ /* = 0 */, int16_t y_ /* = 0 */,
                   int16_t w_ /* = EZ_PARENT */, int16_t h_ /* = EZ_PARENT */,
                   WidgetColors colors_ /* = THEME_COLORS */) {
  init(&parentWidget, x_, y_, w_, h_, colors_);
}

ezWindow::ezWindow(int16_t x_ /* = 0 */, int16_t y_ /* = 0 */,
                   int16_t w_ /* = EZ_PARENT */, int16_t h_ /* = EZ_PARENT */,
                   WidgetColors colors_ /* = THEME_COLORS */) {
  init(nullptr, x_, y_, w_, h_, colors_);
}

ezWindow::~ezWindow() {
  blur();
}

void ezWindow::init(ezWidget* pwPtr,
                    int16_t x_, int16_t y_, int16_t w_, int16_t h_,
                    WidgetColors colors_) {
  type = W_WINDOW;
  set(x_, y_, w_, h_);
  ezThemeClass& t = ezThemeClass::instance();
  colors     = t.colors(colors_, t.wdw_colors);
  if (pwPtr) pwPtr->add(*this);
}

void ezWindow::focus() {
  ez.add(*this);
  ez.draw();
}

void ezWindow::blur() {
  ez.remove(*this);
}


bool ezWindow::hasFocus() {
  return (ez._widgets.back() == this);
}

void ezWindow::run() {
  if    (!_quitting) focus();
  while (!_quitting) loop();
  _quitting--;
  blur();
}

ezWindow ezScreen;
