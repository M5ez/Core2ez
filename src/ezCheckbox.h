#ifndef _EZCHECKBOX_H_
#define _EZCHECKBOX_H_

#include <ezWidget.h>
#include <ezLabel.h>

class ezCheckbox : public ezWidget {
 public:

  ezCheckbox(ezWidget& parentWidget,
             int16_t x_ = EZ_INVALID, int16_t y_ = EZ_INVALID,
             int16_t w_ = 0, int16_t h_ = 0,
             String text_           = "",
             bool value_            = false,
             WidgetColors colors_   = THEME_COLORS,
             ezFont font_   = THEME_FONT);

  ezCheckbox(int16_t x_ = EZ_INVALID, int16_t y_ = EZ_INVALID,
             int16_t w_ = 0, int16_t h_ = 0,
             String text_           = "",
             bool value_            = false,
             WidgetColors colors_   = THEME_COLORS,
             ezFont font_   = THEME_FONT);

  void init(ezWidget* pwPtr,
            int16_t x_, int16_t y_, int16_t w_, int16_t h_, String text_,
            bool value_, WidgetColors colors_, ezFont font_);

                  explicit operator bool();
  ezCheckbox&     operator=(bool value_);
  virtual void    draw();
  void            valueDraw();
  virtual void    eventPost();

  bool            value             = false;
  ezLabel         label;

};

#endif /* _EZCHECKBOX_H_ */
