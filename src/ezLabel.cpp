#include "ezLabel.h"

ezLabel::ezLabel() {
  init(nullptr, INVALID_VALUE, INVALID_VALUE, 0, 0, "");
}

ezLabel::ezLabel(ezWidget& parentWidget) {
  init(&parentWidget, INVALID_VALUE, INVALID_VALUE, 0, 0, "");
}

ezLabel::ezLabel(int16_t x_, int16_t y_, int16_t w_, int16_t h_,
                 String text_ /* = "" */) {
 init(nullptr, x_, y_, w_, h_, text_);
}

ezLabel::ezLabel(ezWidget& parentWidget,
                 int16_t x_, int16_t y_, int16_t w_, int16_t h_,
                 String text_ /* = "" */) {
  init(&parentWidget, x_, y_, w_, h_, text_);
}

void ezLabel::init(ezWidget* pwPtr,
                   int16_t x_, int16_t y_, int16_t w_, int16_t h_,
                   String text_) {
  set(x_, y_, w_, h_);
  text = text_;
  colors.outline = NODRAW;
  colors.fill    = NODRAW;
  if (pwPtr) pwPtr->add(*this);
}


void ezLabel::draw() {

  if (colors.fill    != NODRAW) fillRect(colors.fill);

  if (colors.outline != NODRAW) drawRect(colors.outline);

  if (colors.text    != NODRAW && text != "") {

    // Figure out where the text goes
    uint8_t datum;
    if (valign == TOP   ) {
      if      (align == LEFT  ) datum = TL_DATUM;
      else if (align == CENTER) datum = TC_DATUM;
      else                      datum = TR_DATUM;
    } else if (valign == MIDDLE) {
      if      (align == LEFT  ) datum = CL_DATUM;
      else if (align == CENTER) datum = CC_DATUM;
      else                      datum = CR_DATUM;
    } else {
      if      (align == LEFT  ) datum = BL_DATUM;
      else if (align == CENTER) datum = BC_DATUM;
      else                      datum = BR_DATUM;
    }
    uint16_t tx;
    if      (align == LEFT   )  tx = padding;
    else if (align == CENTER )  tx = w / 2;
    else                        tx = w - padding;
    uint16_t ty;
    if      (valign == TOP   )  ty = padding;
    else if (valign == MIDDLE)  ty = h / 2;
    else                        ty = h - padding;

    // Actual drawing of text
    setTextColor(colors.text);
    setTextSize(textSize);
    ezFont(font);
    setTextDatum(datum);
    setTextPadding(0);
    drawString(text, tx + dx, ty + dy);

  }

  // Boilerplate draw() stuff
  drawChildren();

}
