#ifndef _EZEVENTS_H_
#define _EZEVENTS_H_

#include <Arduino.h>
#include <ezPointAndZone.h>


// The preprocessor magic for ON and WITH. Uses ezEventAdd class below.

// The multiple EXP rounds are necessary to correctly expand __LINE__
#define EXP3(x, y) x ## y
#define EXP2(x, y) EXP3(x, y)
#define EXP(x) EXP2(x, __LINE__)
#define ON(widget, ...) \
  ezEventAdd EXP(eventAdd) (EXP(ezHandler), widget, ##__VA_ARGS__); \
  void EXP(ezHandler)()

#define WITH(widget_type, variable) \
  if (!ez.e.widget || ez.e.widget->typeName() != #widget_type) return; \
  widget_type& variable = *static_cast<widget_type*>(ez.e.widget);


class ezWidget;
class ezGesture;

#define NUMEVENTS       12
#define E_NONE          0x0000
#define E_TOUCH         0x0001
#define E_RELEASE       0x0002
#define E_MOVE          0x0004
#define E_GESTURE       0x0008
#define E_TAPPED        0x0010
#define E_DBLTAPPED     0x0020
#define E_DRAGGED       0x0040
#define E_PRESSED       0x0080
#define E_PRESSING      0x0100
#define E_LONGPRESSED   0x0200
#define E_LONGPRESSING  0x0400
#define E_CHANGED       0x0800

#define E_ALL           0xFFFF

class Event {
 public:
                operator uint16_t();
  const char*   typeName();
  char*         c_str();
  uint16_t      direction();
  bool          isDirection(int16_t wanted, uint8_t plusminus = PLUSMINUS);
  uint16_t      distance();
  uint16_t      type            = E_NONE;
  uint8_t       finger          = 0;
  Point         from            = Point();
  Point         to              = Point();
  uint16_t      duration        = 0;
  ezWidget*     widget          = nullptr;
  ezGesture*    gesture         = nullptr;
};

struct ezEventHandler {
  uint16_t      eventMask;
  void          (*fn)();
  bool          includeDescendants;
};

class Eventful {
 public:
  void          addHandler(void (*fn)(), uint16_t eventMask = E_ALL,
                bool includeDescendants_ = false);
  void          delHandlers(void (*fn)());
  Event         e = Event();
 protected:
  void          fireEvent(bool descendant = false);
  std::vector<ezEventHandler> _eventHandlers;
};

// This is a dummy class that only uses its constructor to set up events
// so that event handlers can be set up from outside functions with ON.
class ezEventAdd {
 public:
  ezEventAdd(void(*func)(), ezWidget&  widget,  uint16_t event = E_ALL);
  ezEventAdd(void(*func)(), ezGesture& gesture, uint16_t event = E_ALL);
};


#endif /* _EZEVENTS_H_ */
