#include "ez.h"


ezClass::ezClass() {
  type = W_ROOT;
  set(0, 0, 320, 240);
  // numb = true;
}

void ezClass::begin() {
  M5.begin();         // Never hurts: just returns on subsequent calls
  ezTouch.begin();

  #ifdef _EZSOUND_H_
    ezSound.begin();
  #endif

  #ifdef _EZWIFI_H_
    ezWifi.begin();
  #endif

}

void ezClass::add(ezWindow& w) {
  if (w.parent()) w.parent()->remove(w);
  _widgets.push_back(&w);
  while (_widgets.size() > MAX_WINDOWSTACK) _widgets.erase(_widgets.begin());
  w._parent = this;
}

void ezClass::remove(ezWindow& w) {
  for (int i = _widgets.size() - 1; i >= 0; --i) {
    if (_widgets[i] == &w) {
      _widgets.erase(_widgets.begin() + i);
      w._parent = nullptr;
    }
  }
}

void ezClass::add   (ezGesture& g) { ezWidget::add   (g); }
void ezClass::remove(ezGesture& g) { ezWidget::remove(g); }

// Only windows not other widgets can be added to the root.
void ezClass::add   (ezWidget& w) { }
void ezClass::remove(ezWidget& w) { }

void ezClass::draw() {
  // root is special in that it only draws the top widget
  if (_widgets.size()) {
    log_v("Widget on top: %d (%s)", (long)_widgets.back(), _widgets.back()->typeName());
    ezWidget* wdgt = _widgets.back();
    if (wdgt->setPos.w == EZ_PARENT) wdgt->w = w;
    if (wdgt->setPos.h == EZ_PARENT) wdgt->h = h;
    wdgt->draw();
    wdgt->push();
  }
}

void ezClass::update() {

  if (!_widgets.size()) ezScreen.focus();

  ezTouch.update();

#ifdef _EZSOUND_H_
  ezSound.update();
#endif

#ifdef _EZWIFI_H_
  ezWifi.update();
#endif

  e = ezEvent();

  ezPoint curr;
  curr = ezTouch.point[_finger];

  ezPoint& prev = _previous[_finger];

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
  if (e == E_RELEASE) _previous[_finger] = ezPoint();

  fireEvent();

  if (e) log_d("%s", e.c_str());

  _finger = !_finger;
}

ezClass& ez = ezClass::instance();
