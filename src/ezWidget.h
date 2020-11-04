#ifndef _EZWIDGET_H_
#define _EZWIDGET_H_

#include <ezEvents.h>
#include <ezDisplayZone.h>


// Special color value, meaning we don't draw anything
#define NODRAW            0x0120


struct WidgetColors {
  bool operator==(WidgetColors a) {
    return (a.fill == fill && a.text == text && a.outline == outline);
  }
  bool operator!=(WidgetColors a) {
    return !(*this == a);
  }
  explicit operator bool() {
    return (fill != NODRAW || text != NODRAW || outline != NODRAW);
  }
  uint16_t                fill;
  uint16_t                text;
  uint16_t                outline;
};

class ezWidget : public ezDisplayZone, public Eventful {
 public:
  ezWidget*               parent();
  virtual void            add(ezWidget& w);
  virtual void            remove(ezWidget& w);
  virtual void            add(ezGesture& g);
  virtual void            remove(ezGesture& g);
  void                    event(Event& e);
  virtual void            draw();
  void                    drawChildren();
  void                    spriteBuffer(int16_t w_ = -1, int16_t h_ = -1);
  virtual void            eventPre(Event& e);
  virtual void            eventPost(Event& e);
  virtual void            cancel();
  bool                    isPressed();
  bool                    isReleased();
  bool                    wasPressed();
  bool                    wasReleased();

  WidgetColors            colors          = {TFT_WHITE, TFT_BLACK, TFT_BLACK};
  bool                    numb            = false;
  uint16_t                tapTime         = 200;
  uint16_t                dbltapTime      = 200;
  uint16_t                longPressTime   =   0;
  uint16_t                repeatDelay     =   0;
  uint16_t                repeatInterval  = 150;
  uint16_t                userData        =   0;
  bool                    glissando       = false;

 protected:
  void                    eventProcess(Event& e);
  std::vector<ezWidget*>  _widgets;
  std::vector<ezGesture*> _gestures;
  bool                    _state          = false;
  bool                    _changed        = false;
  bool                    _tapWait        = false;
  bool                    _pressing       = false;
  bool                    _longPressing   = false;
  bool                    _cancelled      = false;
  bool                    _touched[2]     = {false, false};
  uint32_t                _lastRepeat     = 0;
  uint32_t                _lastOnTime     = 0;
  uint32_t                _lastOffTime    = 0;
  Event                   _lastOnEvent    = Event();
  Event                   _lastOffEvent   = Event();
};

#endif /* _EZWIDGET_H_ */
