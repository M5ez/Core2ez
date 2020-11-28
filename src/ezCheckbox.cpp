#include "ezButton.h"
#include <ez.h>

ezCheckbox::ezCheckbox(ezWidget& parentWidget,
                       int16_t x_ /* = EZ_INVALID */,
                       int16_t y_ /* = EZ_INVALID */,
                       int16_t w_ /* = 0 */, int16_t h_ /* = 0 */,
                       String text_ /* = "" */,
                       bool value_ /* = false */,
                       WidgetColors colors_ /* = THEME_COLORS */,
                       ezFont font_ /* = THEME_FONT */) {
  init(&parentWidget, x_, y_, w_, h_, text_, value_, colors_, font_);
}

ezCheckbox::ezCheckbox(int16_t x_ /* = EZ_INVALID */,
                       int16_t y_ /* = EZ_INVALID */,
                       int16_t w_ /* = 0 */, int16_t h_ /* = 0 */,
                       String text_ /* = "" */,
                       bool value_ /* = false */,
                       WidgetColors colors_ /* = THEME_COLORS */,
                       ezFont font_ /* = THEME_FONT */) {
  init(nullptr, x_, y_, w_, h_, text_, value_, colors_, font_);
}

void ezCheckbox::init(ezWidget* pwPtr,
                      int16_t x_, int16_t y_, int16_t w_, int16_t h_,
                      String text_, bool value_, WidgetColors colors_,
                      ezFont font_) {
  type       = W_CHECKBOX;
  set(x_, y_, w_, h_);
  colors     = ezTheme.colors(colors_,   ezTheme.chk_colors);
  value      = value_;
  label.font = font_ ? font_ : ezTheme.chk_font;
  label.text = text_;
  add(label);
  if (pwPtr) pwPtr->add(*this); else ezScreen.add(*this);
}


ezCheckbox::operator bool() { return value; }

ezCheckbox& ezCheckbox::operator=(bool value_) {
  value = !value;
  valueDraw();
  refresh();
  return *this;
}

void ezCheckbox::draw() {

  if (!colors) return;

  valueDraw();

  drawRect(0, 0, h, h, colors.outline);

  label.colors.text = colors.text;
  label.align = EZ_LEFT;
  uint8_t margin = h / 4;
  label.set(h + margin, 0, w - (h + margin), h);

  drawChildren();
}

void ezCheckbox::eventPost() {
  if (ez.e.widget == this && ez.e == E_TAPPED) {
    value = !value;
    valueDraw();
    refresh();
  }
}

void ezCheckbox::valueDraw() {
  uint8_t m = h / 4;
  fillRect(m, m, h - (2 * m), h - (2 * m), value ? colors.text : colors.fill);
}

