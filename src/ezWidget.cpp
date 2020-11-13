#include <ezWidget.h>
#include <ezRoot.h>
#include <ezGesture.h>
#include <ezValues.h>
#include <Arduino.h>

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

/* virtual */ void ezWidget::push() {
  if (!sprite) return;
  bool ol = (colors.outline != NODRAW);
  if (_parent)  _parent->push (sprite, offset.x + ol, offset.y + ol, w - (2 * ol), h - (2 * ol), x + ol, y + ol);
  else         spriteToDisplay(sprite, offset.x + ol, offset.y + ol, w - (2 * ol), h - (2 * ol), x + ol, y + ol);
  _updateBox();
}

void ezWidget::_updateBox() {
  if (!parent()) return;
  if (scroll && showArrows) {
    if (offset.x)                        _drawArrow(EZ_LEFT );
    if (offset.y)                        _drawArrow(EZ_UP   );
    if (sprite->width()  - offset.x > w) _drawArrow(EZ_RIGHT);
    if (sprite->height() - offset.y > h) _drawArrow(EZ_DOWN );
  }
  if (colors.outline != NODRAW) {
    parent()->drawRect(x, y, w, h, colors.outline);
  }
}

void ezWidget::_drawArrow(int16_t direction) {
  if (!parent()) return;
  Point tip, tail1, tail2;

  if        (ez.Theme.arrowValign == EZ_TOP   ) {
    tip.y = y + ez.Theme.arrowPadding + (ez.Theme.arrowWidth / 2);
  } else if (ez.Theme.arrowValign == EZ_BOTTOM) {
    tip.y = y + h - ez.Theme.arrowPadding - (ez.Theme.arrowWidth / 2);
  } else {
    tip.y = y + (h / 2);
  }
  tail1.y = tip.y - (ez.Theme.arrowWidth / 2);
  tail2.y = tip.y + (ez.Theme.arrowWidth / 2);

  if        (direction == EZ_LEFT) {
    tip.x = x + ez.Theme.arrowPadding;
    tail1.x = tail2.x = tip.x + ez.Theme.arrowLength;
  } else if (direction == EZ_RIGHT) {
    tip.x = x + w - ez.Theme.arrowPadding;
    tail1.x = tail2.x = tip.x - ez.Theme.arrowLength;
  } else if (direction == EZ_UP) {
    tip.x = x + (w / 2);
    tip.y = y + ez.Theme.arrowPadding;
    tail1.y = tail2.y = tip.y + ez.Theme.arrowLength;
    tail1.x = tip.x - (ez.Theme.arrowWidth / 2);
    tail2.x = tip.x + (ez.Theme.arrowWidth / 2);
  } else {
    tip.x = x + (w / 2);
    tip.y = y + h - ez.Theme.arrowPadding;
    tail1.y = tail2.y = tip.y - ez.Theme.arrowLength;
    tail1.x = tip.x - (ez.Theme.arrowWidth / 2);
    tail2.x = tip.x + (ez.Theme.arrowWidth / 2);
  }
  parent()->fillTriangle(tip, tail1, tail2, ez.Theme.arrowFill);
  parent()->drawTriangle(tip, tail1, tail2, ez.Theme.arrowOutline);
}


void ezWidget::event() {

  // This will add one for each of a widget or a gesture pointer present
  // on the event. See below
  uint8_t claims = (!!ez.e.widget) + (!!ez.e.gesture);

  // Translate parent coordinates to widget origin
  ez.e.from.x -= x;
  ez.e.from.y -= y;
  ez.e.to.x -= x;
  ez.e.to.y -= y;

  // Pass event to all gestures for this widget
  for (auto gesture : _gestures) gesture->event();

  // Pass event by all widgets in this widget
  for (int i = _widgets.size() - 1; i >= 0; --i) {
    _widgets[i]->event();
  }

  eventPre();
  _eventProcess();

  // Scroll if set
  if (sprite && scroll && ez.e.widget == this && ez.e == E_MOVE) {
    Point moveBy;
    moveBy.x = ez.e.from.x - ez.e.to.x;
    moveBy.y = ez.e.from.y - ez.e.to.y;
    if (offset.x + moveBy.x < 0) moveBy.x = -offset.x;
    if (offset.y + moveBy.y < 0) moveBy.y = -offset.y;
    if (offset.x + moveBy.x > sprite->width()  - w)
      moveBy.x = sprite->width()  - w - offset.x;
    if (offset.y + moveBy.y > sprite->height() - h)
      moveBy.y = sprite->height() - h - offset.y;

    if (moveBy != Point(0,0)) {
      offset.x += moveBy.x;
      offset.y += moveBy.y;
      ez.e.to.x -= moveBy.x;
      ez.e.to.y -= moveBy.y;
      push();
    }
  }

  eventPost();
  e = Event();
  if (ez.e.widget == this) {
    fireEvent();
  } else if ((!!ez.e.widget) + (!!ez.e.gesture) > claims) {
    // Something added a widget or gesture pointer, so one of our descendants
    // must have claimed this event. So we fire it as a descendant event.
    fireEvent(true);
  }

  // Translate back to parent coordinates
  ez.e.from.x += x;
  ez.e.from.y += y;
  ez.e.to.x += x;
  ez.e.to.y += y;

}

void ezWidget::_eventProcess() {

  // Reset last event for this widget
  e = Event();

  // If E_NONE, check if we have anything that could ride in here
  if (!ez.e) {
    if (_changed) {
      _changed = false;
      if (!_state && !_cancelled) {
        // Post-releaase events
        if (!contains(_lastOffEvent.to, true)) {
          ez.e = _lastOffEvent;
          ez.e.type = E_DRAGGED;
        } else if (_lastOffEvent.duration < tapTime) {
          if (_tapWait) {
            ez.e = _lastOffEvent;
            ez.e.type = E_DBLTAP;
          } else {
            _tapWait = true;
            return;
          }
        } else if (_longPressing) {
          ez.e = _lastOffEvent;
          ez.e.type = E_LONGPRESSED;
        } else {
          ez.e = _lastOffEvent;
          ez.e.type = E_PRESSED;
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
          ez.e = _lastOffEvent;
          ez.e.type = E_TAP;
          _pressing = false;
          _longPressing = false;
          _tapWait = false;
        } else if (_state) {
          if (!_pressing && duration > tapTime ||
              (repeatDelay && duration > repeatDelay &&
              millis() - _lastRepeat > repeatInterval)) {
            ez.e = _lastOnEvent;
            ez.e.type = E_PRESSING;
            ez.e.duration = duration;
            _pressing = true;
            _lastRepeat = millis();
          } else if (longPressTime && !_longPressing &&
                     duration > longPressTime) {
            _longPressing = true;
            ez.e = _lastOnEvent;
            ez.e.type = E_LONGPRESSING;
            ez.e.duration = duration;
          }
        }
      }
    }
    return;
  }

  // Some other widget is already dealing with this event or we're numb
  if (ez.e.widget || numb) return;

  bool parentGlissando = (parent() && parent()->glissando);

  // See if this is ours and tag it as such if it is
  if      (ez.e == E_TOUCH && contains(ez.e.to, true)) {
    ez.e.widget = this;
    _touched[ez.e.finger] = true;
  }
  else if (ez.e == E_MOVE && _touched[ez.e.finger]) {
    ez.e.widget = this;
    if (glissando || (parentGlissando && !contains(ez.e.to, true))) {
      ez.e.type = E_RELEASE;
    }
  }
  if (ez.e == E_RELEASE && _touched[ez.e.finger]) {
    ez.e.widget = this;
    if (ez.e.gesture) _cancelled = true;
    _touched[ez.e.finger] = false;
  }

  if (ez.e.widget != this) return;

  // Did the _state change?
  if      (_state && !_touched[0] && !_touched[1]) {
    _state   = false;
    _changed = true;
    _lastOffTime = millis();
    _lastOffEvent = ez.e;
  }
  else if (!_state && (_touched[0] || _touched[1])) {
    _state   = true;
    _changed = true;
    _lastOnTime = millis();
    _lastOnEvent = ez.e;
  }
}

/* virtual */ void ezWidget::eventPre() { };

/* virtual */ void ezWidget::eventPost() { };

/* virtual */ void ezWidget::clear() {
  if (sprite) {
    // faster, uses memset if possible
    sprite->fillSprite(colors.fill);
  } else {
    fillRect(colors.fill);
  }
}

/* virtual */ void ezWidget::draw() {
  if (!*this) return;
  if (colors.fill    != NODRAW) clear();
  drawChildren();
  if (colors.outline != NODRAW && !sprite) drawRect(colors.outline);
}

void ezWidget::drawChildren() {
  for (auto widget : _widgets) if (widget && *widget) widget->draw();
}

void ezWidget::spriteBuffer(int16_t w_ /* = -1 */, int16_t h_ /* = -1 */) {
  ezDisplayZone::spriteBuffer(w_, h_);
  if (parent()) {
    sprite->fillSprite(parent()->colors.fill);
  } else {
    sprite->fillSprite(colors.fill);
  }
}

void ezWidget::cancel() { _cancelled = true; }

bool ezWidget::isPressed() { return _state; }

bool ezWidget::isReleased() { return !_state; }

bool ezWidget::wasPressed() { return _state && _changed; }

bool ezWidget::wasReleased() { return (!_state && _changed); }

