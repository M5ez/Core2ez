#include "ezButton.h"
#include <ezRoot.h>

ezButton::ezButton(ezWidget& parentWidget,
                   int16_t x_ /* = EZ_INVALID */, int16_t y_ /* = EZ_INVALID */,
                   int16_t w_ /* = 0 */, int16_t h_ /* = 0 */,
                   String text_ /* = "" */,
                   WidgetColors colors_ /* = THEME_COLORS */,
                   WidgetColors onColors_ /* = THEME_COLORS */,
                   const GFXfont* font_ /* = THEME_FONT */) {
  init(&parentWidget, x_, y_, w_, h_, text_, colors_, onColors_, font_);
}

ezButton::ezButton(int16_t x_ /* = EZ_INVALID */, int16_t y_ /* = EZ_INVALID */,
                   int16_t w_ /* = 0 */, int16_t h_ /* = 0 */,
                   String text_ /* = "" */,
                   WidgetColors colors_ /* = THEME_COLORS */,
                   WidgetColors onColors_ /* = THEME_COLORS */,
                   const GFXfont* font_ /* = THEME_FONT */) {
  init(nullptr, x_, y_, w_, h_, text_, colors_, onColors_, font_);
}

void ezButton::init(ezWidget* pwPtr,
                    int16_t x_, int16_t y_, int16_t w_, int16_t h_,
                    String text_, WidgetColors colors_, WidgetColors onColors_,
                    const GFXfont* font_) {
  set(x_, y_, w_, h_);
  colors     = ez.Theme.colors(colors_,   ez.Theme.btn_colors);
  onColors   = ez.Theme.colors(onColors_, ez.Theme.btn_onColors);
  label.font = font_ ? font_ : ez.Theme.btn_font;
  label.text = text_;
  add(label);
  if (pwPtr) pwPtr->add(*this); else ez.Screen.add(*this);
}


ezButton::operator bool() { return _state; }

void ezButton::draw() { draw( _state ? onColors : colors); }

void ezButton::draw(WidgetColors wCol) {

  if (!wCol) return;

  uint8_t r = (cornerRadius == 0xFF) ? min(w, h) / 4 : cornerRadius;

  if (wCol.fill != NODRAW) {
    if (r >= 2) {
      fillRoundRect(r, wCol.fill);
    } else {
      fillRect(wCol.fill);
    }
  }

  if (wCol.outline != NODRAW) {
    if (r >= 2) {
      drawRoundRect(r, wCol.outline);
    } else {
      drawRect(wCol.outline);
    }
  }

  label.colors.text = wCol.text;
  label.align = EZ_CENTER;
  uint8_t margin = max(4, r / 3);
  label.set(margin, margin, w - (2 * margin), h - (2 * margin));

  drawChildren();

}

void ezButton::eventPost() {
  if (_changed && onColors && onColors != colors) draw();
}
