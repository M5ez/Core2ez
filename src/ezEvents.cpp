#include <ezEvents.h>
#include <ez.h>


// Eventful class

void Eventful::fireEvent(bool offspring /* = false */) {
  e = ez.e;
  if (e) {
    for (auto h : _eventHandlers) {
      if (h.eventMask & e.type && offspring == h.offspring) {
        h.fn();
      }
    }
  }
}

void Eventful::addHandler(void (*fn)(), uint16_t eventMask /* = E_ALL */,
                          bool offspring /* = false */) {
  ezEventHandler handler;
  handler.fn = fn;
  handler.offspring = offspring;
  handler.eventMask = eventMask;
  _eventHandlers.push_back(handler);
}

void Eventful::on(uint16_t eventMask, void (*fn)()) {
  addHandler(fn, eventMask, false);
}

void Eventful::onOffspring(uint16_t eventMask, void (*fn)()) {
  addHandler(fn, eventMask, true);
}

void Eventful::delHandlers(void (*fn)() /* = nullptr */) {
  for (int i = _eventHandlers.size() - 1; i >= 0; --i) {
    if (fn && fn != _eventHandlers[i].fn) continue;
    _eventHandlers.erase(_eventHandlers.begin() + i);
  }
}



// ezEvent class

ezEvent::operator uint16_t() { return type; }

const char* ezEvent::typeName() {
  const char* unknown = "E_UNKNOWN";
  const char* none = "E_NONE";
  const char* eventNames[NUMEVENTS] = {
      "E_TOUCH",    "E_RELEASE",     "E_MOVE",         "E_GESTURE",
      "E_TAPPED",   "E_DBLTAPPED",   "E_DRAGGED",      "E_PRESSED",
      "E_PRESSING", "E_LONGPRESSED", "E_LONGPRESSING", "E_CHANGED" };
  if (!type) return none;
  for (uint8_t i = 0; i < NUMEVENTS; i++) {
    if ((type >> i) & 1) return eventNames[i];
  }
  return unknown;
}

uint16_t ezEvent::direction() {
  return from.directionTo(to);
}

bool ezEvent::isDirection(int16_t wanted, uint8_t plusminus /* = PLUSMINUS */) {
  return from.isDirectionTo(to, wanted, plusminus);
}

uint16_t ezEvent::distance() { return from.distanceTo(to); }

char* ezEvent::c_str() {
  static char r[80];
  sprintf(r, "%-25s %-14s finger%d  %s .. %s %4d ms",
                widget ? widget->ident() : "", typeName(), finger,
                (char*)from, (char*)to, duration);
  return r;
}
