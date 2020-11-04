#ifndef _EZWINDOW_H_
#define _EZWINDOW_H_

#include <ezWidget.h>

class ezWindow : public ezWidget {
 public:
  ezWindow();
  ezWindow(uint16_t color_);
  ezWindow(uint16_t color_, int16_t x, int16_t y, int16_t w_, int16_t h_);
  ezWindow(int16_t x, int16_t y, int16_t w_, int16_t h_);

  void            focus();
  void            blur();
};

#endif /* _EZWINDOW_H_ */
