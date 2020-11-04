#ifndef _EZBUTTON_H_
#define _EZBUTTON_H_

#include <ezWidget.h>
#include <ezLabel.h>

class ezButton : public ezWidget {
 public:
                  ezButton();
                  ezButton(ezWidget& parentWidget);
                  ezButton(int16_t x_, int16_t y_, int16_t w_, int16_t h_,
                           String text_ = "");
                  ezButton(ezWidget& parentWidget,
                           int16_t x_, int16_t y_, int16_t w_, int16_t h_,
                           String text_ = "");
  void            init(ezWidget* pwPtr,
                       int16_t x_, int16_t y_, int16_t w_, int16_t h_,
                       String text_);
                  operator bool();
  void            draw(WidgetColors wc);
  virtual void    draw();
  virtual void    eventPost(Event& e);

  WidgetColors    onColors        = { NODRAW, NODRAW, NODRAW };
  uint8_t         cornerRadius    = 0xFF;   // auto
  ezLabel         label;

};

#endif /* _EZBUTTON_H_ */
