#include <ezEvents.h>

void Eventful::fireEvent(Event& e) {
  lastEvent = e;
  if (e) {
    for (auto h : _eventHandlers) {
      if (!(h.eventMask & e.type)) continue;
      h.fn(e);
    }
  }
}

void Eventful::addHandler(void (*fn)(Event&),
                           uint16_t eventMask /* = E_ALL */) {
  EventHandler handler;
  handler.fn = fn;
  handler.eventMask = eventMask;
  _eventHandlers.push_back(handler);
}

void Eventful::delHandlers(void (*fn)(Event&) /* = nullptr */) {
  for (int i = _eventHandlers.size() - 1; i >= 0; --i) {
    if (fn && fn != _eventHandlers[i].fn) continue;
    _eventHandlers.erase(_eventHandlers.begin() + i);
  }
}



// Event class

Event::operator uint16_t() { return type; }

const char* Event::typeName() {
  const char* unknown = "E_UNKNOWN";
  const char* none = "E_NONE";
  const char* eventNames[NUM_EVENTS] = {
      "E_TOUCH",    "E_RELEASE",     "E_MOVE",        "E_GESTURE",
      "E_TAP",      "E_DBLTAP",      "E_DRAGGED",     "E_PRESSED",
      "E_PRESSING", "E_LONGPRESSED", "E_LONGPRESSING"};
  if (!type) return none;
  for (uint8_t i = 0; i < NUM_EVENTS; i++) {
    if ((type >> i) & 1) return eventNames[i];
  }
  return unknown;
}

uint16_t Event::direction() {
  return from.directionTo(to);
}

bool Event::isDirection(int16_t wanted, uint8_t plusminus /* = PLUSMINUS */) {
  return from.isDirectionTo(to, wanted, plusminus);
}

uint16_t Event::distance() { return from.distanceTo(to); }

void Event::print() {
  Serial.printf("%-12s finger%d  (%3d, %3d) --> (%3d, %3d)  %4d ms\n",
                typeName(), finger, from.x, from.y, to.x, to.y, duration);
}
