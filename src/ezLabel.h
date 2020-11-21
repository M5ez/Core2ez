#ifndef _EZLABEL_H_
#define _EZLABEL_H_

#include <ezWidget.h>
#include <ezValues.h>

class ezLabel : public ezWidget {
 public:
  ezLabel(ezWidget& parentWidget,
          int16_t x_ = EZ_INVALID, int16_t y_ = EZ_INVALID,
          int16_t w_ = 0, int16_t h_ = 0,
          String text_         = "",
          WidgetColors colors_ = THEME_COLORS,
          const GFXfont* font_ = THEME_FONT,
          int16_t align_ = EZ_LEFT,
          int16_t valign_ = EZ_CENTER,
          uint8_t padding_ = 2, int16_t dx_ = 0, int16_t dy_ = 0);

  ezLabel(int16_t x_ = EZ_INVALID, int16_t y_ = EZ_INVALID,
          int16_t w_ = 0, int16_t h_ = 0,
          String text_         = "",
          WidgetColors colors_ = THEME_COLORS,
          const GFXfont* font_ = THEME_FONT,
          int16_t align_ = EZ_LEFT,
          int16_t valign_ = EZ_CENTER,
          uint8_t padding_ = 2, int16_t dx_ = 0, int16_t dy_ = 0);

  void init(ezWidget* pwPtr,
            int16_t x_, int16_t y_, int16_t w_, int16_t h_, String text_,
            WidgetColors colors_, const GFXfont* font_, int16_t align_,
            int16_t valign_, uint8_t padding_, int16_t dx_, int16_t dy_);

  virtual void    draw();
  virtual void    eventPre();
  virtual void    eventPost();
  String          text       = "";
  const GFXfont*  font       = FSS9;
  int16_t         align      = EZ_LEFT;
  int16_t         valign     = EZ_TOP;
  uint8_t         padding    = 0;
  int16_t         dx         = 0;
  int16_t         dy         = 0;
};


#endif /* _EZLABEL_H_ */
