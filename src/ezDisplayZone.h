#ifndef _EZDISPLAYZONE_H_
#define _EZDISPLAYZONE_H_

#include <ezPointAndZone.h>

#include <M5Core2.h>
#define DISPLAY M5.Lcd


// Special fake font pointers to access the older non FreeFonts
// in a unified way. Only valid if passed to ezFont
#define MONO6x8          (GFXfont*) 1
#define SANS16          (GFXfont*) 2
#define SANS26          (GFXfont*) 4
#define NUMONLY48        (GFXfont*) 6
#define NUMONLY7SEG48    (GFXfont*) 7
#define NUMONLY75        (GFXfont*) 8
// The following fonts are just scaled up from previous ones (textSize 2)
// But they might still be useful.
#define mono12x16        (GFXfont*) 9
#define SANS32          (GFXfont*) 10
#define SANS52          (GFXfont*) 12
#define NUMONLY96        (GFXfont*) 14
#define NUMONLY7SEG96    (GFXfont*) 15
#define NUMONLY150      (GFXfont*) 16

#include <Free_Fonts.h>

class ezDisplayZone;

class ezFont {
 public:
  ezFont();
  ezFont(uint8_t font_);
  ezFont(const GFXfont* gfxFont_);
  operator bool();
  ezFont& operator=(const GFXfont* gfxFont_) ;
  ezFont& operator=(uint8_t font_);
  void set(ezDisplayZone& dz);
  int16_t height();
  uint8_t textsize;
 private:
  const GFXfont* _gfxFont;
  uint8_t _font;
};



class ezDisplayZone : public ezZone {
 public:
  ezDisplayZone* parent();
  virtual void push();
  void     push(TFT_eSprite* s, int16_t ox, int16_t oy, int16_t w_, int16_t h_, int16_t x_, int16_t y_);
  void     spriteToDisplay(TFT_eSprite* s, int16_t ox, int16_t oy, int16_t w_, int16_t h_, int16_t x_, int16_t y_);
  virtual void spriteBuffer(int16_t w_ = -1, int16_t h_ = -1);
  void     direct();
  void     refresh();
  void     setFont(ezFont& f);
  void     drawRect(uint32_t color);
  void     fillRect(uint32_t color);
  void     drawRoundRect(int32_t radius, uint32_t color);
  void     fillRoundRect(int32_t radius, uint32_t color);
  void     drawCircle(ezPoint p, int32_t r, uint32_t color);
  void     fillCircle(ezPoint p, int32_t r, uint32_t color);
  void     drawTriangle(ezPoint p0, ezPoint p1, ezPoint p2, uint32_t color);
  void     fillTriangle(ezPoint p0, ezPoint p1, ezPoint p2, uint32_t color);

  ///@{   @name testgroup

  void     drawPixel(int32_t x_, int32_t y_, uint32_t color);
  void     drawChar(int32_t x_, int32_t y_, uint16_t c, uint32_t color, uint32_t bg, uint8_t size);
  void     drawLine(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint32_t color);
  void     drawFastVLine(int32_t x_, int32_t y_, int32_t h_, uint32_t color);
  void     drawFastHLine(int32_t x_, int32_t y_, int32_t w_, uint32_t color);
  void     fillRect(int32_t x_, int32_t y_, int32_t w_, int32_t h_, uint32_t color);

  ///@}

  int16_t  drawChar(uint16_t uniCode, int32_t x_, int32_t y_, uint8_t font);
  int16_t  drawChar(uint16_t uniCode, int32_t x_, int32_t y_);
  int16_t  height(void);
  int16_t  width(void);
  void     fillScreen(uint32_t color);
  void     drawRect(int32_t x_, int32_t y_, int32_t w_, int32_t h_, uint32_t color);
  void     drawRoundRect(int32_t x0, int32_t y0, int32_t w_, int32_t h_, int32_t radius, uint32_t color);
  void     fillRoundRect(int32_t x0, int32_t y0, int32_t w_, int32_t h_, int32_t radius, uint32_t color);
  void     setRotation(uint8_t r);
  void     drawCircle(int32_t x0, int32_t y0, int32_t r, uint32_t color);
  void     drawCircleHelper(int32_t x0, int32_t y0, int32_t r, uint8_t cornername, uint32_t color);
  void     fillCircle(int32_t x0, int32_t y0, int32_t r, uint32_t color);
  void     fillCircleHelper(int32_t x0, int32_t y0, int32_t r, uint8_t cornername, int32_t delta, uint32_t color);
  void     drawEllipse(int16_t x0, int16_t y0, int32_t rx, int32_t ry, uint16_t color);
  void     fillEllipse(int16_t x0, int16_t y0, int32_t rx, int32_t ry, uint16_t color);
  void     drawTriangle(int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint32_t color);
  void     fillTriangle(int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint32_t color);
//   void     drawBitmap(int16_t x_, int16_t y_, const uint8_t *bitmap, int16_t w_, int16_t h_, uint16_t color);
//   void     drawXBitmap(int16_t x_, int16_t y_, const uint8_t *bitmap, int16_t w_, int16_t h_, uint16_t color);
//   void     drawXBitmap(int16_t x_, int16_t y_, const uint8_t *bitmap, int16_t w_, int16_t h_, uint16_t fgcolor, uint16_t bgcolor);
//   void     setBitmapColor(uint16_t fgcolor, uint16_t bgcolor);
  void     setPivot(int16_t x_, int16_t y_);
  void     setCursor(int16_t x_, int16_t y_);
  void     setCursor(int16_t x_, int16_t y_, uint8_t font);
  void     setTextColor(uint16_t color);
  void     setTextColor(uint16_t fgcolor, uint16_t bgcolor);
  void     setTextSize(uint8_t size);
  void     setTextWrap(boolean wrapX, boolean wrapY = false);
  void     setTextDatum(uint8_t datum);
  void     setTextPadding(uint16_t x_width);
  void     setFreeFont(const GFXfont *f = NULL);
  void     setTextFont(uint8_t font);
  uint8_t  getRotation(void);
  uint8_t  getTextDatum(void);
  uint8_t  color16to8(uint16_t color565); // Convert 16 bit colour to 8 bits
  int16_t  getCursorX(void);
  int16_t  getCursorY(void);
  uint16_t fontsLoaded(void);
  uint16_t color565(uint8_t red, uint8_t green, uint8_t blue);   // Convert 8 bit red, green and blue to 16 bits
  uint16_t color8to16(uint8_t color332);  // Convert 8 bit colour to 16 bits
  int16_t  drawNumber(long long_num, int32_t poX, int32_t poY, uint8_t font);
  int16_t  drawNumber(long long_num, int32_t poX, int32_t poY);
  int16_t  drawFloat(float floatNumber, uint8_t decimal, int32_t poX, int32_t poY, uint8_t font);
  int16_t  drawFloat(float floatNumber, uint8_t decimal, int32_t poX, int32_t poY);
  int16_t  drawString(const char *string, int32_t poX, int32_t poY, uint8_t font);
  int16_t  drawString(const char *string, int32_t poX, int32_t poY);
  int16_t  drawCentreString(const char *string, int32_t dX, int32_t poY, uint8_t font); // Deprecated, use setTextDatum() and drawString()
  int16_t  drawRightString(const char *string, int32_t dX, int32_t poY, uint8_t font);  // Deprecated, use setTextDatum() and drawString()
  int16_t  drawString(const String& string, int32_t poX, int32_t poY, uint8_t font);
  int16_t  drawString(const String& string, int32_t poX, int32_t poY);
  int16_t  drawCentreString(const String& string, int32_t dX, int32_t poY, uint8_t font); // Deprecated, use setTextDatum() and drawString()
  int16_t  drawRightString(const String& string, int32_t dX, int32_t poY, uint8_t font);  // Deprecated, use setTextDatum() and drawString()
  int16_t  textWidth(const char *string, uint8_t font);
  int16_t  textWidth(const char *string);
  int16_t  textWidth(const String& string, uint8_t font);
  int16_t  textWidth(const String& string);
  int16_t  fontHeight(int16_t font);
  int16_t  fontHeight(void);
  uint16_t decodeUTF8(uint8_t *buf, uint16_t *index, uint16_t remaining);
  uint16_t decodeUTF8(uint8_t c);
  size_t   write(uint8_t utf8);

  TFT_eSprite*      sprite     = nullptr;
  uint8_t           spriteBPP  = 16;
  ezPoint           offset     = ezPoint(0, 0);
  ezDisplayZone*    _parent    = nullptr;
};


#endif /* _EZDISPLAYZONE_H_ */
