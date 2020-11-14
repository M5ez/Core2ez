#include "ezLabel.h"
#include <ezRoot.h>

ezLabel::ezLabel(ezWidget& parentWidget,
                 int16_t x_ /* = EZ_INVALID */, int16_t y_ /* = EZ_INVALID */,
                 int16_t w_ /* = 0 */, int16_t h_ /* = 0 */,
                 String text_ /* = "" */,
                 WidgetColors colors_ /* = THEME_COLORS */,
                 const GFXfont* font_ /* = THEME_FONT */,
                 int16_t align_ /* = EZ_LEFT */,
                 int16_t valign_ /* = EZ_CENTER */, uint8_t padding_ /* = 2 */,
                 int16_t dx_ /* = 0 */, int16_t dy_ /* = 0 */) {
  init(&parentWidget, x_, y_, w_, h_, text_, colors_, font_, align_, valign_,
       padding_, dx_, dy_);
}

ezLabel::ezLabel(int16_t x_ /* = EZ_INVALID */, int16_t y_ /* = EZ_INVALID */,
                 int16_t w_ /* = 0 */, int16_t h_ /* = 0 */,
                 String text_ /* = "" */,
                 WidgetColors colors_ /* = THEME_COLORS */,
                 const GFXfont* font_ /* = THEME_FONT */,
                 int16_t align_ /* = EZ_LEFT */,
                 int16_t valign_ /* = EZ_CENTER */, uint8_t padding_ /* = 2 */,
                 int16_t dx_ /* = 0 */, int16_t dy_ /* = 0 */) {
  init(nullptr, x_, y_, w_, h_, text_, colors_, font_, align_, valign_,
       padding_, dx_, dy_);
}

void ezLabel::init(ezWidget* pwPtr,
                   int16_t x_, int16_t y_, int16_t w_, int16_t h_,
                   String text_, WidgetColors colors_, const GFXfont* font_,
                   int16_t align_, int16_t valign_, uint8_t padding_,
                   int16_t dx_, int16_t dy_) {
  type    = W_LABEL;
  set(x_, y_, w_, h_);
  text    = text_;
  colors  = ez.Theme.colors(colors_, ez.Theme.lbl_colors);
  font    = (font_ != THEME_FONT) ? font_ : ez.Theme.lbl_font;
  align   = align_;
  valign  = valign_;
  padding = padding_;
  dx      = dx_;
  dy      = dy_;
  numb    = true;
  if (pwPtr) pwPtr->add(*this); else ez.Screen.add(*this);
}

void ezLabel::eventPre() { };

void ezLabel::eventPost() { };

void ezLabel::draw() {

  if (colors.fill    != NODRAW) fillRect(colors.fill);

  if (colors.outline != NODRAW) drawRect(colors.outline);

  if (colors.text    != NODRAW && text != "") {

    // Figure out where the text goes
    uint8_t datum;
    if        (valign == EZ_TOP   ) {
      if      (align  == EZ_LEFT  ) datum = TL_DATUM;
      else if (align  == EZ_CENTER) datum = TC_DATUM;
      else                          datum = TR_DATUM;
    } else if (valign == EZ_CENTER) {
      if      (align  == EZ_LEFT  ) datum = CL_DATUM;
      else if (align  == EZ_CENTER) datum = CC_DATUM;
      else                          datum = CR_DATUM;
    } else {
      if      (align  == EZ_LEFT  ) datum = BL_DATUM;
      else if (align  == EZ_CENTER) datum = BC_DATUM;
      else                          datum = BR_DATUM;
    }
    uint16_t tx;
    if        (align == EZ_LEFT   )  tx = padding;
    else if   (align == EZ_CENTER )  tx = w / 2;
    else                             tx = w - padding;
    uint16_t ty;
    if        (valign == EZ_TOP   )  ty = padding;
    else if   (valign == EZ_CENTER)  ty = h / 2;
    else                             ty = h - padding;


    // Actual drawing of text
    setTextColor(colors.text);
    ezFont(font);
    setTextDatum(datum);
    setTextPadding(0);
    if (!sprite && textWidth(text) > w - padding) {
      spriteBuffer();
      setTextColor(colors.text);
      ezFont(font);
      setTextDatum(datum);
      setTextPadding(0);
      drawString(text, tx + dx, ty + dy);
      push();
      direct();
    } else {
      drawString(text, tx + dx, ty + dy);
    }

  }

  // Boilerplate draw() stuff
  drawChildren();
}
