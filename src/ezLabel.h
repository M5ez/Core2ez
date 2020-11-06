#ifndef _EZLABEL_H_
#define _EZLABEL_H_

#include <ezWidget.h>

enum align_t {
  LEFT,
  CENTER,
  RIGHT
};

enum valign_t {
  TOP,
  MIDDLE,
  BOTTOM
};


class ezLabel : public ezWidget {
 public:
                  ezLabel();
                  ezLabel(ezWidget& parentWidget);
                  ezLabel(int16_t x_, int16_t y_, int16_t w_, int16_t h_,
                          String text_ = "");
                  ezLabel(ezWidget& s_,
                          int16_t x_, int16_t y_, int16_t w_, int16_t h_,
                          String text_ = "");
  void            init(ezWidget* pwPtr,
                       int16_t x_, int16_t y_, int16_t w_, int16_t h_,
                       String text_);
  virtual void    draw();
  String          text       = "";
  uint16_t        color      = TFT_WHITE;
  align_t         align      = CENTER;
  valign_t        valign     = MIDDLE;
  uint8_t         padding    = 0;
  int16_t         dx         = 0;
  int16_t         dy         = 0;
  const GFXfont*  font       = FSS9;
};


#endif /* _EZLABEL_H_ */
