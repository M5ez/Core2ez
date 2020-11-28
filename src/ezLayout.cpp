#include "ezLayout.h"
#include <ezWindow.h>

ezLayout::ezLayout(ezWidget& parentWidget,
                   int16_t x_ /* = EZ_INVALID */, int16_t y_ /* = EZ_INVALID */,
                   int16_t w_ /* = 0 */, int16_t h_ /* = 0 */,
                   int16_t gutter_ /* = 0 */) {
  type = W_LAYOUT;
  set(x_, y_, w_, h_);
  gutter = gutter_;
  parentWidget.add(*this);
}

ezLayout::ezLayout(int16_t x_ /* = EZ_INVALID */, int16_t y_ /* = EZ_INVALID */,
                   int16_t w_ /* = 0 */, int16_t h_ /* = 0 */,
                   int16_t gutter_ /* = 0 */) {
  type = W_LAYOUT;
  set(x_, y_, w_, h_);
  gutter = gutter_;
  ezScreen.add(*this);
}
