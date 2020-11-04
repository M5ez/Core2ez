#include "ezButton.h"
#include <ezRoot.h>

ezButton::ezButton() {
  init(nullptr, INVALID_VALUE, INVALID_VALUE, 0, 0, "");
}

ezButton::ezButton(int16_t x_, int16_t y_, int16_t w_, int16_t h_,
                   String text_ /* = "" */) {
 init(nullptr, x_, y_, w_, h_, text_);
}

void ezButton::init(ezWidget* pwPtr,
                    int16_t x_, int16_t y_, int16_t w_, int16_t h_,
                    String text_) {
  set(x_, y_, w_, h_);
  colors   = ez.Theme.ezButton_colors;
  onColors = ez.Theme.ezButton_onColors;
  label.text = text_;
  label.font = FSSB12;
  label.numb = true;
  add(label);
  ez.Screen.add(*this);
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
  uint8_t margin = max(4, r / 3);
  label.set(margin, margin, w - (2 * margin), h - (2 * margin));

  drawChildren();

}

void ezButton::eventPost(Event& e) {
  if (_changed && onColors && onColors != colors) draw();
}
