#include "ezWindow.h"

#include <ezRoot.h>

ezWindow::ezWindow() {
  colors = ez.Theme.ezWindow_colors;
  set(0, 0, 320, 240);
}

ezWindow::ezWindow(uint16_t color_) {
  colors = ez.Theme.ezWindow_colors;
  colors.fill = color_;
  set(0, 0, 320, 240);
}

ezWindow::ezWindow(uint16_t color_,
                   int16_t x_, int16_t y_, int16_t w_, int16_t h_) {
  colors = ez.Theme.ezWindow_colors;
  colors.fill = color_;
  set(x_, y_, w_, h_);
}

ezWindow::ezWindow(int16_t x_, int16_t y_, int16_t w_, int16_t h_) {
  colors = ez.Theme.ezWindow_colors;
  set(x_, y_, w_, h_);
}

void ezWindow::focus() {
  ez.add(*this);
  ez.draw();
}

void ezWindow::blur() {
  ez.remove(*this);
  ez.draw();
}
