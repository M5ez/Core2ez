#include <ezEvents.h>
#include <ez.h>


// Eventful class

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
      "E_TOUCH",    "E_RELEASE",     "E_MOVE",         "E_GESTURE",
      "E_TAPPED",   "E_DBLTAPPED",   "E_DRAGGED",      "E_PRESSED",
      "E_PRESSING", "E_LONGPRESSED", "E_LONGPRESSING", "E_CHANGED" };
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

char* Event::c_str() {
  static char r[80];
  sprintf(r, "%-10d %-13s %-14s finger%d  %s .. %s %4d ms",
                widget, widget ? widget->typeName() : "", typeName(), finger,
                (char*)from, (char*)to, duration);
  return r;
}


// ezEventAdd class

ezEventAdd::ezEventAdd(void(*func)(), ezWidget&  widget,
                       uint16_t event /* = E_ALL */) {
  widget.addHandler(func, event);
}

ezEventAdd::ezEventAdd(void(*func)(), ezGesture& gesture,
                       uint16_t event /* = E_ALL */) {
  gesture.addHandler(func, event);
}
