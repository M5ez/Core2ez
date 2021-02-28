#include "ezDisplayZone.h"
#include <ezValues.h>
#include <ez.h>


ezFont::ezFont() {
  _font = 0;
  _gfxFont = nullptr;
}

ezFont::ezFont(uint8_t font_) {
  *this = font_;
}

ezFont::ezFont(const GFXfont* gfxFont_) {
  *this = gfxFont_;
}

ezFont::operator bool() { return (_font); }

ezFont& ezFont::operator=(const GFXfont* gfxFont_) {
  uint32_t ptrAsInt = (uint32_t)gfxFont_;
  textsize = 1;
  if (ptrAsInt <= 16) {
    if (ptrAsInt > 8) {
      ptrAsInt -= 8;
      textsize++;
    }
    _font = ptrAsInt;
    _gfxFont = nullptr;
  } else {
    _font = 1;
    _gfxFont = gfxFont_;
  }
  return *this;
}

ezFont& ezFont::operator=(uint8_t font_) {
  _gfxFont = nullptr;
  _font = font_;
  textsize = 1;
  return *this;
}

void ezFont::set(ezDisplayZone& dz) {
  dz.setTextSize(textsize);
  if (_gfxFont) {
    dz.setFreeFont(_gfxFont);
  } else if (_font) {
    dz.setTextFont(_font);
  } else {
    dz.setFreeFont(FSS9);
    log_e("Invalid font, replaced by FSS9");
  }
}

int16_t ezFont::height() {
  set(ez);
  return ez.fontHeight();
}



ezDisplayZone* ezDisplayZone::parent() {
  return _parent;
}

// Push this object's own sprite
/* virtual */ void ezDisplayZone::push() {
  if (!sprite) return;
  if (_parent)  _parent->push(sprite, offset.x, offset.y, w, h, x, y);
  else         spriteToDisplay(sprite, offset.x, offset.y, w, h, x, y);
}

// Get handed a push for a sprite by a child
void ezDisplayZone::push(TFT_eSprite* s, int16_t ox, int16_t oy, int16_t w_, int16_t h_, int16_t x_, int16_t y_) {
  // Serial.printf("push(%d, %d, %d, %d, %d, %d, %d)\n", (long)s, ox, oy, w_, h_, x_, y_);

  if      (sprite )  sprite  ->pushInSprite(s, ox, oy, w_, h_, x_, y_);
  else if (_parent) _parent ->push(s, ox, oy, w_, h_, x + x_, y + y_);
  else               spriteToDisplay(s, ox, oy, w_, h_, x_, y_);
}

void ezDisplayZone::spriteToDisplay(TFT_eSprite* s, int16_t ox, int16_t oy, int16_t w_, int16_t h_, int16_t x_, int16_t y_) {
  // Serial.printf("spriteToDisplay(%d, %d, %d, %d, %d, %d, %d)\n", (long)s, ox, oy, w_, h_, x_, y_);
  if (ox || oy || s->width() > w_ || s->height() > h_) {
    TFT_eSprite tmpspr(&DISPLAY);
    tmpspr.createSprite(w_, h_);
    tmpspr.pushInSprite(s, ox, oy, w_, h_, 0, 0);
    tmpspr.pushSprite(x_, y_);
    tmpspr.deleteSprite();
  } else {
    s->pushSprite(x_ , y_);
  }
}

void ezDisplayZone::spriteBuffer(int16_t w_ /* = -1 */, int16_t h_ /* = -1 */) {
  direct();
  sprite = new TFT_eSprite(&DISPLAY);
  sprite->setColorDepth(spriteBPP);
  sprite->createSprite(w_ == -1 ? w : w_, h_ == -1 ? h : h_);
  offset.x = 0;
  offset.y = 0;
}

void ezDisplayZone::direct() {
  if (sprite) {
    sprite->deleteSprite();
    delete sprite;
    sprite = nullptr;
    offset.x = 0;
    offset.y = 0;
  }
}

void ezDisplayZone::refresh() {
  push();
  if (_parent) _parent->refresh();
}

void ezDisplayZone::setFont(ezFont& f) {
  f.set(*this);
}

void ezDisplayZone::drawRect(uint32_t color) {
  drawRect(0, 0, w, h, color);
}

void ezDisplayZone::fillRect(uint32_t color) {
  fillRect(0, 0, w, h, color);
}

void ezDisplayZone::drawRoundRect(int32_t radius, uint32_t color) {
  drawRoundRect(0, 0, w, h, radius, color);
}

void ezDisplayZone::fillRoundRect(int32_t radius, uint32_t color) {
  fillRoundRect(0, 0, w, h, radius, color);
}

void ezDisplayZone::drawCircle(ezPoint p, int32_t r, uint32_t color) {
  if (p) drawCircle(p.x, p.y, r, color);
}

void ezDisplayZone::fillCircle(ezPoint p, int32_t r, uint32_t color) {
  if (p) fillCircle(p.x, p.y, r, color);
}

void ezDisplayZone::drawTriangle(ezPoint p0, ezPoint p1, ezPoint p2, uint32_t color) {
  if (p0 && p1 && p2) drawTriangle(p0.x, p0.y, p1.x, p1.y, p2.x, p2.y, color);
}

void ezDisplayZone::fillTriangle(ezPoint p0, ezPoint p1, ezPoint p2, uint32_t color) {
  if (p0 && p1 && p2) fillTriangle(p0.x, p0.y, p1.x, p1.y, p2.x, p2.y, color);
}

void ezDisplayZone::drawPixel(int32_t x_, int32_t y_, uint32_t color) {
  if      (sprite ) sprite  ->drawPixel(x_, y_, color);
  else if (_parent) _parent ->drawPixel(x + x_, y + y_, color);
  else              DISPLAY  .drawPixel(x + x_, y + y_, color);
}

void ezDisplayZone::drawChar(int32_t x_, int32_t y_, uint16_t c, uint32_t color, uint32_t bg, uint8_t size) {
  if      (sprite ) sprite  ->drawChar(x_, y_, c, color, bg, size);
  else if (_parent) _parent ->drawChar(x + x_, y + y_, c, color, bg, size);
  else              DISPLAY  .drawChar(x + x_, y + y_, c, color, bg, size);
}

void ezDisplayZone::drawLine(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint32_t color) {
  if      (sprite ) sprite  ->drawLine(x0, y0, x1, y1, color);
  else if (_parent) _parent ->drawLine(x + x0, y + y0, x + x1, y + y1, color);
  else              DISPLAY  .drawLine(x + x0, y + y0, x + x1, y + y1, color);
}

void ezDisplayZone::drawFastVLine(int32_t x_, int32_t y_, int32_t h_, uint32_t color) {
  if      (sprite ) sprite  ->drawFastVLine(x_, y_, h_, color);
  else if (_parent) _parent ->drawFastVLine(x + x_, y + y_, h_, color);
  else              DISPLAY  .drawFastVLine(x + x_, y + y_, h_, color);
}

void ezDisplayZone::drawFastHLine(int32_t x_, int32_t y_, int32_t w_, uint32_t color) {
  if      (sprite ) sprite  ->drawFastHLine(x_, y_, w_, color);
  else if (_parent) _parent ->drawFastHLine(x + x_, y + y_, w_, color);
  else              DISPLAY  .drawFastHLine(x + x_, y + y_, w_, color);
}

void ezDisplayZone::fillRect(int32_t x_, int32_t y_, int32_t w_, int32_t h_, uint32_t color) {
  if      (sprite ) sprite  ->fillRect(x_, y_, w_, h_, color);
  else if (_parent) _parent ->fillRect(x + x_, y + y_, w_, h_, color);
  else              DISPLAY  .fillRect(x + x_, y + y_, w_, h_, color);
}

int16_t ezDisplayZone::drawChar(uint16_t uniCode, int32_t x_, int32_t y_, uint8_t font) {
  if      (sprite ) return sprite  ->drawChar(uniCode, x_, y_, font);
  else if (_parent) return _parent ->drawChar(uniCode, x + x_, y + y_, font);
  else              return DISPLAY  .drawChar(uniCode, x + x_, y + y_, font);
}

int16_t ezDisplayZone::drawChar(uint16_t uniCode, int32_t x_, int32_t y_) {
  if      (sprite ) return sprite  ->drawChar(uniCode, x_, y_);
  else if (_parent) return _parent ->drawChar(uniCode, x + x_, y + y_);
  else              return DISPLAY  .drawChar(uniCode, x + x_, y + y_);
}

int16_t ezDisplayZone::height() {
  if      (sprite ) return sprite  ->height();
  else if (_parent) return _parent ->height();
  else              return DISPLAY  .height();
}

int16_t ezDisplayZone::width() {
  if      (sprite ) return sprite  ->width();
  else if (_parent) return _parent ->width();
  else              return DISPLAY  .width();
}

void ezDisplayZone::fillScreen(uint32_t color) {
  if      (sprite ) sprite  ->fillScreen(color);
  else if (_parent) _parent ->fillScreen(color);
  else              DISPLAY  .fillScreen(color);
}

void ezDisplayZone::drawRect(int32_t x_, int32_t y_, int32_t w_, int32_t h_, uint32_t color) {
  if      (sprite ) sprite  ->drawRect(x_, y_, w_, h_, color);
  else if (_parent) _parent ->drawRect(x + x_, y + y_, w_, h_, color);
  else              DISPLAY  .drawRect(x + x_, y + y_, w_, h_, color);
}

void ezDisplayZone::drawRoundRect(int32_t x0, int32_t y0, int32_t w_, int32_t h_, int32_t radius, uint32_t color) {
  if      (sprite ) sprite  ->drawRoundRect(x0, y0, w_, h_, radius, color);
  else if (_parent) _parent ->drawRoundRect(x + x0, y + y0, w_, h_, radius, color);
  else              DISPLAY  .drawRoundRect(x + x0, y + y0, w_, h_, radius, color);
}

void ezDisplayZone::fillRoundRect(int32_t x0, int32_t y0, int32_t w_, int32_t h_, int32_t radius, uint32_t color) {
  if      (sprite ) sprite  ->fillRoundRect(x0, y0, w_, h_, radius, color);
  else if (_parent) _parent ->fillRoundRect(x + x0, y + y0, w_, h_, radius, color);
  else              DISPLAY  .fillRoundRect(x + x0, y + y0, w_, h_, radius, color);
}

void ezDisplayZone::setRotation(uint8_t r) {
  if      (sprite ) sprite  ->setRotation(r);
  else if (_parent) _parent ->setRotation(r);
  else              DISPLAY  .setRotation(r);
}

void ezDisplayZone::drawCircle(int32_t x0, int32_t y0, int32_t r, uint32_t color) {
  if      (sprite ) sprite  ->drawCircle(x0, y0, r, color);
  else if (_parent) _parent ->drawCircle(x + x0, y + y0, r, color);
  else              DISPLAY  .drawCircle(x + x0, y + y0, r, color);
}

void ezDisplayZone::drawCircleHelper(int32_t x0, int32_t y0, int32_t r, uint8_t cornername, uint32_t color) {
  if      (sprite ) sprite  ->drawCircleHelper(x0, y0, r, cornername, color);
  else if (_parent) _parent ->drawCircleHelper(x + x0, y + y0, r, cornername, color);
  else              DISPLAY  .drawCircleHelper(x + x0, y + y0, r, cornername, color);
}

void ezDisplayZone::fillCircle(int32_t x0, int32_t y0, int32_t r, uint32_t color) {
  if      (sprite ) sprite  ->fillCircle(x0, y0, r, color);
  else if (_parent) _parent ->fillCircle(x + x0, y + y0, r, color);
  else              DISPLAY  .fillCircle(x + x0, y + y0, r, color);
}

void ezDisplayZone::fillCircleHelper(int32_t x0, int32_t y0, int32_t r, uint8_t cornername, int32_t delta, uint32_t color) {
  if      (sprite ) sprite  ->fillCircleHelper(x0, y0, r, cornername, delta, color);
  else if (_parent) _parent ->fillCircleHelper(x + x0, y + y0, r, cornername, delta, color);
  else              DISPLAY  .fillCircleHelper(x + x0, y + y0, r, cornername, delta, color);
}

void ezDisplayZone::drawEllipse(int16_t x0, int16_t y0, int32_t rx, int32_t ry, uint16_t color) {
  if      (sprite ) sprite  ->drawEllipse(x0, y0, rx, ry, color);
  else if (_parent) _parent ->drawEllipse(x + x0, y + y0, rx, ry, color);
  else              DISPLAY  .drawEllipse(x + x0, y + y0, rx, ry, color);
}

void ezDisplayZone::fillEllipse(int16_t x0, int16_t y0, int32_t rx, int32_t ry, uint16_t color) {
  if      (sprite ) sprite  ->fillEllipse(x0, y0, rx, ry, color);
  else if (_parent) _parent ->fillEllipse(x + x0, y + y0, rx, ry, color);
  else              DISPLAY  .fillEllipse(x + x0, y + y0, rx, ry, color);
}

void ezDisplayZone::drawTriangle(int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint32_t color) {
  if      (sprite ) sprite  ->drawTriangle(x0, y0, x1, y1, x2, y2, color);
  else if (_parent) _parent ->drawTriangle(x + x0, y + y0, x + x1, y + y1, x + x2, y + y2, color);
  else              DISPLAY  .drawTriangle(x + x0, y + y0, x + x1, y + y1, x + x2, y + y2, color);
}

void ezDisplayZone::fillTriangle(int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint32_t color) {
  if      (sprite ) sprite  ->fillTriangle(x0, y0, x1, y1, x2, y2, color);
  else if (_parent) _parent ->fillTriangle(x + x0, y + y0, x + x1, y + y1, x + x2, y + y2, color);
  else              DISPLAY  .fillTriangle(x + x0, y + y0, x + x1, y + y1, x + x2, y + y2, color);
}

// void ezDisplayZone::drawBitmap(int16_t x_, int16_t y_, const uint8_t *bitmap, int16_t w_, int16_t h_, uint16_t color) {
//   if      (sprite ) sprite  ->drawBitmap(x_, y_, bitmap, w_, h_, color);
//   else if (_parent) _parent ->drawBitmap(x + x_, y + y_, bitmap, w_, h_, color);
//   else              DISPLAY  .drawBitmap(x + x_, y + y_, bitmap, w_, h_, color);
// }
//
// void ezDisplayZone::drawXBitmap(int16_t x_, int16_t y_, const uint8_t *bitmap, int16_t w_, int16_t h_, uint16_t color) {
//   if      (sprite ) sprite  ->drawXBitmap(x_, y_, bitmap, w_, h_, color);
//   else if (_parent) _parent ->drawXBitmap(x + x_, y + y_, bitmap, w_, h_, color);
//   else              DISPLAY  .drawXBitmap(x + x_, y + y_, bitmap, w_, h_, color);
// }
//
// void ezDisplayZone::drawXBitmap(int16_t x_, int16_t y_, const uint8_t *bitmap, int16_t w_, int16_t h_, uint16_t fgcolor, uint16_t bgcolor) {
//   if      (sprite ) sprite  ->drawXBitmap(x_, y_, bitmap, w_, h_, fgcolor, bgcolor);
//   else if (_parent) _parent ->drawXBitmap(x + x_, y + y_, bitmap, w_, h_, fgcolor, bgcolor);
//   else              DISPLAY  .drawXBitmap(x + x_, y + y_, bitmap, w_, h_, fgcolor, bgcolor);
// }
//
// void ezDisplayZone::setBitmapColor(uint16_t fgcolor, uint16_t bgcolor) {
//   if      (sprite ) sprite  ->setBitmapColor(fgcolor, bgcolor);
//   else if (_parent) _parent ->setBitmapColor(fgcolor, bgcolor);
//   else              DISPLAY  .setBitmapColor(fgcolor, bgcolor);
// }

void ezDisplayZone::setPivot(int16_t x_, int16_t y_) {
  if      (sprite ) sprite  ->setPivot(x, y);
  else if (_parent) _parent ->setPivot(x + x_, y + y_);
  else              DISPLAY  .setPivot(x + x_, y + y_);
}

void ezDisplayZone::setCursor(int16_t x_, int16_t y_) {
  if      (sprite ) sprite  ->setCursor(x_, y_);
  else if (_parent) _parent ->setCursor(x + x_, y + y_);
  else              DISPLAY  .setCursor(x + x_, y + y_);
}

void ezDisplayZone::setCursor(int16_t x_, int16_t y_, uint8_t font) {
  if      (sprite ) sprite  ->setCursor(x_, y_, font);
  else if (_parent) _parent ->setCursor(x + x_, y + y_, font);
  else              DISPLAY  .setCursor(x + x_, y + y_, font);
}

void ezDisplayZone::setTextColor(uint16_t color) {
  if      (sprite ) sprite  ->setTextColor(color);
  else if (_parent) _parent ->setTextColor(color);
  else              DISPLAY  .setTextColor(color);
}

void ezDisplayZone::setTextColor(uint16_t fgcolor, uint16_t bgcolor) {
  if      (sprite ) sprite  ->setTextColor(fgcolor, bgcolor);
  else if (_parent) _parent ->setTextColor(fgcolor, bgcolor);
  else              DISPLAY  .setTextColor(fgcolor, bgcolor);
}

void ezDisplayZone::setTextSize(uint8_t size) {
  if      (sprite ) sprite  ->setTextSize(size);
  else if (_parent) _parent ->setTextSize(size);
  else              DISPLAY  .setTextSize(size);
}

void ezDisplayZone::setTextWrap(boolean wrapX, boolean wrapY /* = false */) {
  if      (sprite ) sprite  ->setTextWrap(wrapX, wrapY);
  else if (_parent) _parent ->setTextWrap(wrapX, wrapY);
  else              DISPLAY  .setTextWrap(wrapX, wrapY);
}

void ezDisplayZone::setTextDatum(uint8_t datum) {
  if      (sprite ) sprite  ->setTextDatum(datum);
  else if (_parent) _parent ->setTextDatum(datum);
  else              DISPLAY  .setTextDatum(datum);
}

void ezDisplayZone::setTextPadding(uint16_t x_width) {
  if      (sprite ) sprite  ->setTextPadding(x_width);
  else if (_parent) _parent ->setTextPadding(x_width);
  else              DISPLAY  .setTextPadding(x_width);
}

void ezDisplayZone::setFreeFont(const GFXfont *f /* = NULL */) {
  if      (sprite ) sprite  ->setFreeFont(f);
  else if (_parent) _parent ->setFreeFont(f);
  else              DISPLAY  .setFreeFont(f);
}

void ezDisplayZone::setTextFont(uint8_t font) {
  if      (sprite ) sprite  ->setTextFont(font);
  else if (_parent) _parent ->setTextFont(font);
  else              DISPLAY  .setTextFont(font);
}

uint8_t ezDisplayZone::getRotation() {
  if      (sprite ) return sprite  ->getRotation();
  else if (_parent) return _parent ->getRotation();
  else              return DISPLAY  .getRotation();
}

uint8_t ezDisplayZone::getTextDatum() {
  if      (sprite ) return sprite  ->getTextDatum();
  else if (_parent) return _parent ->getTextDatum();
  else              return DISPLAY  .getTextDatum();
}

uint8_t ezDisplayZone::color16to8(uint16_t color565) {
  if      (sprite ) return sprite  ->color16to8(color565);
  else if (_parent) return _parent ->color16to8(color565);
  else              return DISPLAY  .color16to8(color565);
}

int16_t ezDisplayZone::getCursorX() {
  if      (sprite ) return sprite  ->getCursorX();
  else if (_parent) return _parent ->getCursorX();
  else              return DISPLAY  .getCursorX();
}

int16_t ezDisplayZone::getCursorY() {
  if      (sprite ) return sprite  ->getCursorY();
  else if (_parent) return _parent ->getCursorY();
  else              return DISPLAY  .getCursorY();
}

uint16_t ezDisplayZone::fontsLoaded() {
  if      (sprite ) return sprite  ->fontsLoaded();
  else if (_parent) return _parent ->fontsLoaded();
  else              return DISPLAY  .fontsLoaded();
}

uint16_t ezDisplayZone::color565(uint8_t red, uint8_t green, uint8_t blue) {
  if      (sprite ) return sprite  ->color565(red, green, blue);
  else if (_parent) return _parent ->color565(red, green, blue);
  else              return DISPLAY  .color565(red, green, blue);
}

uint16_t ezDisplayZone::color8to16(uint8_t color332) {
  if      (sprite ) return sprite  ->color8to16(color332);
  else if (_parent) return _parent ->color8to16(color332);
  else              return DISPLAY  .color8to16(color332);
}

int16_t ezDisplayZone::drawNumber(long long_num, int32_t poX, int32_t poY, uint8_t font) {
  if      (sprite ) return sprite  ->drawNumber(long_num, poX, poY, font);
  else if (_parent) return _parent ->drawNumber(long_num, x + poX, y + poY, font);
  else              return DISPLAY  .drawNumber(long_num, x + poX, y + poY, font);
}

int16_t ezDisplayZone::drawNumber(long long_num, int32_t poX, int32_t poY) {
  if      (sprite ) return sprite  ->drawNumber(long_num, poX, poY);
  else if (_parent) return _parent ->drawNumber(long_num, x + poX, y + poY);
  else              return DISPLAY  .drawNumber(long_num, x + poX, y + poY);
}

int16_t ezDisplayZone::drawFloat(float floatNumber, uint8_t decimal, int32_t poX, int32_t poY, uint8_t font) {
  if      (sprite ) return sprite  ->drawFloat(floatNumber, decimal, poX, poY, font);
  else if (_parent) return _parent ->drawFloat(floatNumber, decimal, x + poX, y + poY, font);
  else              return DISPLAY  .drawFloat(floatNumber, decimal, x + poX, y + poY, font);
}

int16_t ezDisplayZone::drawFloat(float floatNumber, uint8_t decimal, int32_t poX, int32_t poY) {
  if      (sprite ) return sprite  ->drawFloat(floatNumber, decimal, poX, poY);
  else if (_parent) return _parent ->drawFloat(floatNumber, decimal, x + poX, y + poY);
  else              return DISPLAY  .drawFloat(floatNumber, decimal, x + poX, y + poY);
}

int16_t ezDisplayZone::drawString(const char *string, int32_t poX, int32_t poY, uint8_t font) {
  if      (sprite ) return sprite  ->drawString(string, poX, poY, font);
  else if (_parent) return _parent ->drawString(string, x + poX, y + poY, font);
  else              return DISPLAY  .drawString(string, x + poX, y + poY, font);
}

int16_t ezDisplayZone::drawString(const char *string, int32_t poX, int32_t poY) {
  if      (sprite ) return sprite  ->drawString(string, poX, poY);
  else if (_parent) return _parent ->drawString(string, x + poX, y + poY);
  else              return DISPLAY  .drawString(string, x + poX, y + poY);
}

int16_t ezDisplayZone::drawCentreString(const char *string, int32_t dX, int32_t poY, uint8_t font) {
  if      (sprite ) return sprite  ->drawCentreString(string, dX, poY, font);
  else if (_parent) return _parent ->drawCentreString(string, x + dX, y + poY, font);
  else              return DISPLAY  .drawCentreString(string, x + dX, y + poY, font);
}

int16_t ezDisplayZone::drawRightString(const char *string, int32_t dX, int32_t poY, uint8_t font) {
  if      (sprite ) return sprite  ->drawRightString(string, dX, poY, font);
  else if (_parent) return _parent ->drawRightString(string, x + dX, y + poY, font);
  else              return DISPLAY  .drawRightString(string, x + dX, y + poY, font);
}

int16_t ezDisplayZone::drawString(const String& string, int32_t poX, int32_t poY, uint8_t font) {
  if      (sprite ) return sprite  ->drawString(string, poX, poY, font);
  else if (_parent) return _parent ->drawString(string, x + poX, y + poY, font);
  else              return DISPLAY  .drawString(string, x + poX, y + poY, font);
}

int16_t ezDisplayZone::drawString(const String& string, int32_t poX, int32_t poY) {
  if      (sprite ) return sprite  ->drawString(string, poX, poY);
  else if (_parent) return _parent ->drawString(string, x + poX, y + poY);
  else              return DISPLAY  .drawString(string, x + poX, y + poY);
}

int16_t ezDisplayZone::drawCentreString(const String& string, int32_t dX, int32_t poY, uint8_t font) {
  if      (sprite ) return sprite  ->drawCentreString(string, dX, poY, font);
  else if (_parent) return _parent ->drawCentreString(string, x + dX, y + poY, font);
  else              return DISPLAY  .drawCentreString(string, x + dX, y + poY, font);
}

int16_t ezDisplayZone::drawRightString(const String& string, int32_t dX, int32_t poY, uint8_t font) {
  if      (sprite ) return sprite  ->drawRightString(string, dX, poY, font);
  else if (_parent) return _parent ->drawRightString(string, x + dX, y + poY, font);
  else              return DISPLAY  .drawRightString(string, x + dX, y + poY, font);
}

int16_t ezDisplayZone::textWidth(const char *string, uint8_t font) {
  if      (sprite ) return sprite  ->textWidth(string, font);
  else if (_parent) return _parent ->textWidth(string, font);
  else              return DISPLAY  .textWidth(string, font);
}

int16_t ezDisplayZone::textWidth(const char *string) {
  if      (sprite ) return sprite  ->textWidth(string);
  else if (_parent) return _parent ->textWidth(string);
  else              return DISPLAY  .textWidth(string);
}

int16_t ezDisplayZone::textWidth(const String& string, uint8_t font) {
  if      (sprite ) return sprite  ->textWidth(string, font);
  else if (_parent) return _parent ->textWidth(string, font);
  else              return DISPLAY  .textWidth(string, font);
}

int16_t ezDisplayZone::textWidth(const String& string) {
  if      (sprite ) return sprite  ->textWidth(string);
  else if (_parent) return _parent ->textWidth(string);
  else              return DISPLAY  .textWidth(string);
}

int16_t ezDisplayZone::fontHeight(int16_t font) {
  if      (sprite ) return sprite  ->fontHeight(font);
  else if (_parent) return _parent ->fontHeight(font);
  else              return DISPLAY  .fontHeight(font);
}

int16_t ezDisplayZone::fontHeight() {
  if      (sprite ) return sprite  ->fontHeight();
  else if (_parent) return _parent ->fontHeight();
  else              return DISPLAY  .fontHeight();
}

uint16_t ezDisplayZone::decodeUTF8(uint8_t *buf, uint16_t *index, uint16_t remaining) {
  if      (sprite ) return sprite  ->decodeUTF8(buf, index, remaining);
  else if (_parent) return _parent ->decodeUTF8(buf, index, remaining);
  else              return DISPLAY  .decodeUTF8(buf, index, remaining);
}

uint16_t ezDisplayZone::decodeUTF8(uint8_t c) {
  if      (sprite ) return sprite  ->decodeUTF8(c);
  else if (_parent) return _parent ->decodeUTF8(c);
  else              return DISPLAY  .decodeUTF8(c);
}

size_t  ezDisplayZone::write(uint8_t utf8) {
  if      (sprite ) return sprite  ->write(utf8);
  else if (_parent) return _parent ->write(utf8);
  else              return DISPLAY  .write(utf8);
}



