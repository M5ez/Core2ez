#ifndef _EZWINDOW_H_
#define _EZWINDOW_H_

#include <ezWidget.h>
#include <ezValues.h>

class ezWindow : public ezWidget {
 public:
  ezWindow(ezWidget& parentWidget,
           int16_t x_ = 0, int16_t y_ = 0,
           int16_t w_ = 320, int16_t h_ = 240,
           WidgetColors colors_   = THEME_COLORS );

  ezWindow(int16_t x_ = 0, int16_t y_ = 0,
           int16_t w_ = 320, int16_t h_ = 240,
           WidgetColors colors_   = THEME_COLORS );

  void init(ezWidget* pwPtr,
            int16_t x_, int16_t y_, int16_t w_, int16_t h_,
            WidgetColors colors_ );

  void            focus();
  void            blur();
  bool            focussed();
};

#endif /* _EZWINDOW_H_ */
