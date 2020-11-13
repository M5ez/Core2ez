#include <ezEvents.h>
#include <ezRoot.h>

void Eventful::fireEvent(bool descendant /* = false */) {
  e = ez.e;
  if (e) {
    for (auto h : _eventHandlers) {
      if (descendant && !h.includeDescendants) return;
      if (h.eventMask & e.type) {
        h.fn();
      }
    }
  }
}

void Eventful::addHandler(void (*fn)(), uint16_t eventMask /* = E_ALL */,
                          bool includeDescendants_ /* = false */) {
  ezEventHandler handler;
  handler.fn = fn;
  handler.includeDescendants = includeDescendants_;
  handler.eventMask = eventMask;
  _eventHandlers.push_back(handler);
}

void Eventful::delHandlers(void (*fn)() /* = nullptr */) {
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
  const char* eventNames[NUMEVENTS] = {
      "E_TOUCH",    "E_RELEASE",     "E_MOVE",        "E_GESTURE",
      "E_TAP",      "E_DBLTAP",      "E_DRAGGED",     "E_PRESSED",
      "E_PRESSING", "E_LONGPRESSED", "E_LONGPRESSING", "E_CHANGED"};
  if (!type) return none;
  for (uint8_t i = 0; i < NUMEVENTS; i++) {
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
