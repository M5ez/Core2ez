#ifndef _EZEVENTS_H_
#define _EZEVENTS_H_

#include <Arduino.h>
#include <ezPointAndZone.h>

class ezWidget;
class ezGesture;

#define NUM_EVENTS      11
#define E_NONE          0x0000
#define E_TOUCH         0x0001
#define E_RELEASE       0x0002
#define E_MOVE          0x0004
#define E_GESTURE       0x0008
#define E_TAP           0x0010
#define E_DBLTAP        0x0020
#define E_DRAGGED       0x0040
#define E_PRESSED       0x0080
#define E_PRESSING      0x0100
#define E_LONGPRESSED   0x0200
#define E_LONGPRESSING  0x0400

#define E_ALL           0x0FFF

class Event {
 public:
  operator uint16_t();
  const char* typeName();
  void print();
  uint16_t direction();
  bool isDirection(int16_t wanted, uint8_t plusminus = PLUSMINUS);
  uint16_t distance();
  uint16_t type = E_NONE;
  uint8_t finger = 0;
  Point from = Point();
  Point to = Point();
  uint16_t duration = 0;
  ezWidget* widget = nullptr;
  ezGesture* gesture = nullptr;
};

struct EventHandler {
  uint16_t eventMask;
  void (*fn)(Event&);
};

class Eventful {

 public:
  void addHandler(void (*fn)(Event&), uint16_t eventMask = E_ALL);
  void delHandlers(void (*fn)(Event&));
  Event lastEvent = Event();

 protected:
  void fireEvent(Event& e);
  std::vector<EventHandler> _eventHandlers;

};

#endif /* _EZEVENTS_H_ */
