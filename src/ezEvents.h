#ifndef _EZEVENTS_H_
#define _EZEVENTS_H_

#include <Arduino.h>
#include <ezPointAndZone.h>

// doFunction just hides a 'lambda' a.k.a. anonyous function
#define doFunction         []()


/*
The below deserves a longer comment because otherwise I myself will not get
what is going on here pretty soon. This, ladies and gentlemen, is a dirty
but hopefully clever hack. It turns:

  if (eventWidget(ezButton, b)) {

into

  if (ez.e.widget && ez.e.widget->typeName() == "ezButton") \
  if (ezButton* b = static_cast<widget_type*>(ez.e.widget)) {

Note that the second if has an assignent in there, and the brackets inside the
macro do not match, it closes the one the user opened and opens another one
after the assignment which the user then closes.
*/
#define eventWidget(widget_type, variable) \
  ez.e.widget && strcmp(ez.e.widget->typeName(), #widget_type) == 0 ) \
  if (widget_type* variable = static_cast<widget_type*>(ez.e.widget)


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

class ezEvent {
 public:
                operator uint16_t();
  const char*   typeName();
  char*         c_str();
  uint16_t      direction();
  bool          isDirection(int16_t wanted, uint8_t plusminus = PLUSMINUS);
  uint16_t      distance();
  uint16_t      type            = E_NONE;
  uint8_t       finger          = 0;
  ezPoint       from            = ezPoint();
  ezPoint       to              = ezPoint();
  uint16_t      duration        = 0;
  ezWidget*     widget          = nullptr;
  ezGesture*    gesture         = nullptr;
};

struct ezEventHandler {
  uint16_t      eventMask;
  void          (*fn)();
  bool          offspring;
};

class Eventful {
 public:
  void          addHandler(void (*fn)(), uint16_t eventMask = E_ALL,
                bool offspring = false);
  void          on(uint16_t eventMask, void (*fn)());
  void          onOffspring(uint16_t eventMask, void (*fn)());
  void          delHandlers(void (*fn)());
  ezEvent       e = ezEvent();
 protected:
  void          fireEvent(bool offspring = false);
  std::vector<ezEventHandler> _eventHandlers;
};


#endif /* _EZEVENTS_H_ */
