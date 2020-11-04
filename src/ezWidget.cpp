#include <ezWidget.h>
#include <ezRoot.h>
#include <ezGesture.h>

ezWidget* ezWidget::parent() {
  if (_parent) return static_cast<ezWidget*>(_parent);
  return nullptr;
}

/* virtual */ void ezWidget::add(ezWidget& w) {
  if (w.parent()) w.parent()->remove(w);
  _widgets.push_back(&w);
  w._parent = this;
}

/* virtual */ void ezWidget::remove(ezWidget& w) {
  for (int i = _widgets.size() - 1; i >= 0; --i) {
    if (_widgets[i] == &w) {
      _widgets.erase(_widgets.begin() + i);
      w._parent = nullptr;
    }
  }
}

/* virtual */ void ezWidget::add(ezGesture& g) {
  if (g.parent()) g.parent()->remove(g);
  _gestures.push_back(&g);
  g._parent = this;
}

/* virtual */ void ezWidget::remove(ezGesture& g) {
  for (int i = _gestures.size() - 1; i >= 0; --i) {
    if (_gestures[i] == &g) {
      _gestures.erase(_gestures.begin() + i);
      g._parent = nullptr;
    }
  }
}

void ezWidget::event(Event& e) {

  // Translate parent coordinates to widget origin
  e.from.x -= x;
  e.from.y -= y;
  e.to.x -= x;
  e.to.y -= y;

  // Pass event to all gestures for this widget
  for (auto gesture : _gestures) gesture->event(e);

  // Pass event by all widgets in this widget
  for (int i = _widgets.size() - 1; i >= 0; --i) {
    _widgets[i]->event(e);
  }

  eventPre(e);
  eventProcess(e);
  eventPost(e);
  fireEvent(e);

  // Translate back to parent coordinates
  e.from.x += x;
  e.from.y += y;
  e.to.x += x;
  e.to.y += y;

}

void ezWidget::eventProcess(Event& e) {

  // Reset publicly shown last event for this widget
  lastEvent = Event();

  // If E_NONE, check if we have anything that could ride in here
  if (!e) {
    if (_changed) {
      _changed = false;
      if (!_state && !_cancelled) {
        // Post-releaase events
        if (!contains(_lastOffEvent.to, true)) {
          e = _lastOffEvent;
          e.type = E_DRAGGED;
        } else if (_lastOffEvent.duration < tapTime) {
          if (_tapWait) {
            e = _lastOffEvent;
            e.type = E_DBLTAP;
          } else {
            _tapWait = true;
            return;
          }
        } else if (_longPressing) {
          e = _lastOffEvent;
          e.type = E_LONGPRESSED;
        } else {
          e = _lastOffEvent;
          e.type = E_PRESSED;
        }
        _tapWait = false;
        _pressing = false;
        _longPressing = false;
      }
      _cancelled = false;
    } else {
      if (!_cancelled) {
        // Timeout events
        uint16_t duration = millis() - _lastOnTime;
        if (_tapWait && millis() - _lastOffTime >= dbltapTime) {
          _lastRepeat = millis();
          e = _lastOffEvent;
          e.type = E_TAP;
          _pressing = false;
          _longPressing = false;
          _tapWait = false;
        } else if (_state) {
          if (!_pressing && duration > tapTime ||
              (repeatDelay && duration > repeatDelay &&
              millis() - _lastRepeat > repeatInterval)) {
            e = _lastOnEvent;
            e.type = E_PRESSING;
            e.duration = duration;
            _pressing = true;
            _lastRepeat = millis();
          } else if (longPressTime && !_longPressing &&
                     duration > longPressTime) {
            _longPressing = true;
            e = _lastOnEvent;
            e.type = E_LONGPRESSING;
            e.duration = duration;
          }
        }
      }
    }
    return;
  }

  // Some other widget is already dealing with this event or we're numb
  if (e.widget || numb) return;

  bool parentGlissando = (parent() && parent()->glissando);

  // See if this is ours and tag it as such if it is
  if      (e == E_TOUCH && contains(e.to, true)) {
    e.widget = this;
    _touched[e.finger] = true;
  }
  else if (e == E_MOVE && _touched[e.finger]) {
    e.widget = this;
    if (glissando || (parentGlissando && !contains(e.to, true))) {
      e.type = E_RELEASE;
    }
  }
  if (e == E_RELEASE && _touched[e.finger]) {
    e.widget = this;
    if (e.gesture) _cancelled = true;
    _touched[e.finger] = false;
  }

  if (e.widget != this) return;

  // Did the _state change?
  if      (_state && !_touched[0] && !_touched[1]) {
    _state   = false;
    _changed = true;
    _lastOffTime = millis();
    _lastOffEvent = e;
  }
  else if (!_state && (_touched[0] || _touched[1])) {
    _state   = true;
    _changed = true;
    _lastOnTime = millis();
    _lastOnEvent = e;
  }
}

/* virtual */ void ezWidget::eventPre(Event& e) { };

/* virtual */ void ezWidget::eventPost(Event& e) { };

/* virtual */ void ezWidget::draw() {
  if (!*this) return;
  if (colors.fill    != NODRAW) fillRect(colors.fill);
  drawChildren();
  if (colors.outline != NODRAW && !(w == 320 && h == 240)) {
    drawRect(colors.outline);
  }
}


void ezWidget::drawChildren() {
  for (auto widget : _widgets) if (widget && *widget) widget->draw();
}

void ezWidget::spriteBuffer(int16_t w_ /* = -1 */, int16_t h_ /* = -1 */) {
  ezDisplayZone::spriteBuffer(w_, h_);
  if (parent()) {
    sprite->fillSprite(parent()->colors.fill);
  } else {
    sprite->fillSprite(ez.Theme.ezWindow_colors.fill);
  }
}

void ezWidget::cancel() { _cancelled = true; }

bool ezWidget::isPressed() { return _state; }

bool ezWidget::isReleased() { return !_state; }

bool ezWidget::wasPressed() { return _state && _changed; }

bool ezWidget::wasReleased() { return (!_state && _changed); }

