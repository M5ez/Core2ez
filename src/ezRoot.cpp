#include "ezRoot.h"


/* static */ ezRoot* ezRoot::instance = nullptr;

ezRoot::ezRoot() {
  if (!instance) instance = this;
  set(0, 0, 320, 280);
  // numb = true;
}

void ezRoot::begin() {
  M5.begin();         // Never hurts: just returns on subsequent calls
  Sound.begin();
  Touch.begin();
}

void ezRoot::add(ezWindow& w) {
  if (w.parent()) w.parent()->remove(w);
  _widgets.push_back(&w);
  while (_widgets.size() > MAX_WINDOWSTACK) _widgets.erase(_widgets.begin());
  w._parent = this;
}

void ezRoot::remove(ezWindow& w) {
  for (int i = _widgets.size() - 1; i >= 0; --i) {
    if (_widgets[i] == &w) {
      _widgets.erase(_widgets.begin() + i);
      w._parent = nullptr;
    }
  }
}

void ezRoot::add   (ezGesture& g) { ezWidget::add   (g); }
void ezRoot::remove(ezGesture& g) { ezWidget::remove(g); }

// Only windows not other widgets can be added to the root.
void ezRoot::add(ezWidget& w) { }
void ezRoot::remove(ezWidget& w) { }

void ezRoot::draw() {
  // root is special in that it only draws the top widget
  if (_widgets.size()) _widgets.back()->draw();
}

void ezRoot::update() {

  if (!_widgets.size()) {
    Screen.focus();
    draw();
    Screen.push();
  }

  Sound.update();
  Touch.update();

  e = Event();

  Point curr;
  curr = Touch.point[_finger];
  Point& prev = _previous[_finger];

  if (curr == prev) {
    e.type = E_NONE;
  } else if (curr && !prev) {
    e.type = E_TOUCH;
    e.from = curr;
    e.to   = curr;
    e.finger = _finger;
    _startTime[_finger] = millis();
    _startPoint[_finger] = curr;
  } else if (prev && !curr) {
    e.type = E_RELEASE;
    e.from = _startPoint[_finger];
    e.to   = prev;
    e.finger = _finger;
    e.duration = millis() - _startTime[_finger];
  } else {
    e.type = E_MOVE;
    e.from = prev;
    e.to   = curr;
    e.finger = _finger;
    e.duration = millis() - _startTime[_finger];
  }
  prev = curr;

  // Pass event to top window on our window stack
  if (_widgets.size()) _widgets.back()->event();

  // In case one of the widgets changed E_MOVE into E_RELEASE (glissando)
  // So we interpret next coordinate as an E_TOUCH again.
  if (e == E_RELEASE) _previous[_finger] = Point();

  fireEvent();

  if (e) {
    Serial.print((long)e.widget);
    Serial.print("  ");
    e.print();
  }

  _finger = !_finger;
}

ezRoot ez;
