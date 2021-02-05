#ifndef _EZWINDOW_H_
#define _EZWINDOW_H_

#include <ezWidget.h>
#include <ezValues.h>

#define ezQuit(...)      ezWindow::quit(__VA_ARGS__)

class ezWindow : public ezWidget {

 public:
  static void              quit(uint8_t count = 1);
 protected:
  static uint8_t           _quitting;

 public:
  ezWindow(ezWidget& parentWidget,
           int16_t x_ = 0, int16_t y_ = 0,
           int16_t w_ = EZ_PARENT, int16_t h_ = EZ_PARENT,
           WidgetColors colors_   = THEME_COLORS );

  ezWindow(int16_t x_ = 0, int16_t y_ = 0,
           int16_t w_ = EZ_PARENT, int16_t h_ = EZ_PARENT,
           WidgetColors colors_   = THEME_COLORS );

  ~ezWindow();

  void init(ezWidget* pwPtr,
            int16_t x_, int16_t y_, int16_t w_, int16_t h_,
            WidgetColors colors_ );

  void            focus();
  void            blur();
  void            run();
  bool            hasFocus();
  String          title;

};

extern ezWindow ezScreen;

#endif /* _EZWINDOW_H_ */
