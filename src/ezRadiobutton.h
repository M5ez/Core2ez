#ifndef _EZRADIOBUTTON_H_
#define _EZRADIOBUTTON_H_

#include <ezWidget.h>
#include <ezLabel.h>

class ezRadiobutton : public ezWidget {
 public:

  ezRadiobutton(ezWidget& parentWidget,
                int16_t x_ = EZ_INVALID, int16_t y_ = EZ_INVALID,
                int16_t w_ = 0, int16_t h_ = 0,
                String text_           = "",
                bool value_            = false,
                WidgetColors colors_   = THEME_COLORS,
                ezFont font_   = THEME_FONT);

  ezRadiobutton(int16_t x_ = EZ_INVALID, int16_t y_ = EZ_INVALID,
                int16_t w_ = 0, int16_t h_ = 0,
                String text_           = "",
                bool value_            = false,
                WidgetColors colors_   = THEME_COLORS,
                ezFont font_   = THEME_FONT);

  void init(ezWidget* pwPtr,
            int16_t x_, int16_t y_, int16_t w_, int16_t h_, String text_,
            bool value_, WidgetColors colors_, ezFont font_);

                  explicit operator bool();
  ezRadiobutton&  operator=(bool value_);
  virtual void    draw();
  void            valueDraw();
  virtual void    eventPost();

  bool            value             = false;
  ezLabel         label;

};

#endif /* _EZRADIOBUTTON_H_ */
