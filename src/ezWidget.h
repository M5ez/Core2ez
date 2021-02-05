#ifndef _EZWIDGET_H_
#define _EZWIDGET_H_

#include <ezEvents.h>
#include <ezDisplayZone.h>


// Special color value, meaning we don't draw anything
#define NODRAW            0x0120

enum widget_t {
  W_WIDGET,
  W_ROOT,
  W_WINDOW,
  W_BUTTON,
  W_LABEL,
  W_CHECKBOX,
  W_RADIOBUTTON,
  W_INPUT,
  W_LAYOUT,
  W_LISTBOX,
  W_LISTITEM,
  W_MENU,
  W_HEADER,
  W_MSGBOX
};


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
  ezWidget&               operator=(ezZone&);
  virtual void            set(int16_t x_ = EZ_INVALID, int16_t y_ = EZ_INVALID,
                          int16_t w_ = 0 , int16_t h_ = 0);
  ezWidget*               parent();
  bool                    isMyDescendant(ezWidget& w);
  bool                    inVirtual(ezPoint& p);
  const char*             typeName();
  const char*             ident();
  virtual void            add(ezWidget& w);
  virtual void            remove(ezWidget& w);
  virtual void            add(ezGesture& g);
  virtual void            remove(ezGesture& g);
  virtual void            push();
  uint16_t                findFill();
  void                    turnOffRadiobuttons();
  void                    event();
  virtual void            clear();
  virtual void            draw();
  void                    drawChildren();
  void                    spriteBuffer(int16_t w_ = -1, int16_t h_ = -1);
  virtual void            eventPre();
  virtual void            eventPost();
  virtual void            cancel();
  bool                    isPressed();
  bool                    isReleased();
  bool                    wasPressed();
  bool                    wasReleased();
  void                    margins(uint8_t margin);

  widget_t                type            = W_WIDGET;
  ezZone                  setPos          = ezZone();
  bool                    autoSize        = false;
  WidgetColors            colors          = {NODRAW, NODRAW, NODRAW};
  int16_t                 gutter          = 0;
  int16_t                 lmargin         = 0;
  int16_t                 rmargin         = 0;
  int16_t                 tmargin         = 0;
  int16_t                 bmargin         = 0;
  bool                    numb            = false;
  uint16_t                tapTime         = 200;
  uint16_t                dbltapTime      = 200;
  uint16_t                longPressTime   =   0;
  uint16_t                repeatDelay     =   0;
  uint16_t                repeatInterval  = 150;
  uint16_t                userData        =   0;
  bool                    glissando       = false;
  bool                    scroll          = true;
  bool                    showArrows      = true;

 protected:
  friend class ezWindow;
  friend class ezHeaderClass;
  void                    _eventProcess();
  void                    _updateBox();
  void                    _drawArrow(int16_t direction);
  std::vector<ezWidget*>  _widgets;
  std::vector<ezGesture*> _gestures;
  bool                    _state          = false;
  bool                    _changed        = false;
  bool                    _tapWait        = false;
  bool                    _pressing       = false;
  bool                    _longPressing   = false;
  bool                    _cancelled      = false;
  bool                    _touched[2]     = {false, false};
  ezPoint                 _posRelParent   = ezPoint();
  uint32_t                _lastRepeat     = 0;
  uint32_t                _lastOnTime     = 0;
  uint32_t                _lastOffTime    = 0;
  ezEvent                 _lastOnEvent    = ezEvent();
  ezEvent                 _lastOffEvent   = ezEvent();
};

#endif /* _EZWIDGET_H_ */
