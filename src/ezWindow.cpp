#include "ezWindow.h"

#include <ezRoot.h>

ezWindow::ezWindow(ezWidget& parentWidget,
                   int16_t x_ /* = 0 */, int16_t y_ /* = 0 */,
                   int16_t w_ /* = 320 */, int16_t h_ /* = 240 */,
                   WidgetColors colors_ /* = THEME_COLORS */) {
  init(&parentWidget, x_, y_, w_, h_, colors_);
}

ezWindow::ezWindow(int16_t x_ /* = 0 */, int16_t y_ /* = 0 */,
                   int16_t w_ /* = 320 */, int16_t h_ /* = 240 */,
                   WidgetColors colors_ /* = THEME_COLORS */) {
  init(nullptr, x_, y_, w_, h_, colors_);
}

void ezWindow::init(ezWidget* pwPtr,
                    int16_t x_, int16_t y_, int16_t w_, int16_t h_,
                    WidgetColors colors_) {
  type = W_WINDOW;
  set(x_, y_, w_, h_);
  colors     = ez.Theme.colors(colors_,   ez.Theme.wdw_colors);
  if (pwPtr) pwPtr->add(*this);
}

void ezWindow::focus() {
  ez.add(*this);
  draw();
  push();
  _fire = E_FOCUS;
}

void ezWindow::blur() {
  ez.remove(*this);
  if (ez._widgets.size()) {
    static_cast<ezWindow*>(ez._widgets.back())->focus();
  }
  _fire = E_BLUR;
}

void ezWindow::eventPost() {
  if (_fire && ez.e == E_NONE) {
    ez.e.type = _fire;
    ez.e.widget = this;
    fireEvent();
    _fire = E_NONE;
  }
}

bool ezWindow::hasFocus() {
  return (ez._widgets.back() == this);
}
