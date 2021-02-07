#ifndef _EZBUTTON_H_
#define _EZBUTTON_H_

#include <ezWidget.h>
#include <ezLabel.h>
#include <ezTheme.h>

class ezButton : public ezWidget {
 public:

  ezButton(ezWidget& parentWidget,
           int16_t x_ = EZ_INVALID, int16_t y_ = EZ_INVALID,
           int16_t w_ = 0, int16_t h_ = 0,
           String text_           = "",
           WidgetColors colors_   = THEME_COLORS,
           WidgetColors onColors_ = THEME_COLORS,
           ezFont font_   = THEME_FONT);

  ezButton(int16_t x_ = EZ_INVALID, int16_t y_ = EZ_INVALID,
           int16_t w_ = 0, int16_t h_ = 0,
           String text_           = "",
           WidgetColors colors_   = THEME_COLORS,
           WidgetColors onColors_ = THEME_COLORS,
           ezFont font_   = THEME_FONT);

  void init(ezWidget* pwPtr,
            int16_t x_, int16_t y_, int16_t w_, int16_t h_, String text_,
            WidgetColors colors_, WidgetColors onColors_,
            ezFont font_);

                  operator bool();
  void            draw(WidgetColors wc);
  virtual void    draw();             /**< (re)draws the button */
  virtual void    eventPost();

  WidgetColors    onColors        = { NODRAW, NODRAW, NODRAW };
  uint8_t         cornerRadius    = 0xFF;   // auto
  ezLabel         label;

};

#endif /* _EZBUTTON_H_ */
