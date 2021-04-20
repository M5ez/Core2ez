#include <ezWidget.h>
#include <ez.h>
#include <ezGesture.h>
#include <ezValues.h>
#include <ezWindow.h>
#include <Arduino.h>


ezWidget& ezWidget::operator=(ezZone& z) {
  x = z.x;
  y = z.y;
  w = z.w;
  h = z.h;
  return *this;
}

/* virtual */
void ezWidget::set(int16_t x_ /* = EZ_INVALID */,
                   int16_t y_ /* = EZ_INVALID */,
                   int16_t w_ /* = 0 */, int16_t h_ /* = 0 */) {
  ezZone::set(x_, y_, w_, h_);
  setPos.set(x_, y_, w_, h_);
  if (w == EZ_PARENT) w = 320;
  if (h == EZ_PARENT) h = 240;
}

ezWidget* ezWidget::parent() {
  if (_parent) return static_cast<ezWidget*>(_parent);
  return nullptr;
}

bool ezWidget::isMyDescendant(ezWidget& w) {
  ezWidget* current = &w;
  ezWidget* parent;
  while (true) {
    parent = current->parent();
    if (!parent) return false;
    if (parent == this) return true;
    current = parent;
  }
  return false;
}

bool ezWidget::inVirtual(ezPoint& p) {
  if (sprite) {
    return (p.y >= 0 && p.y < sprite->height() &&
            p.x >= 0 && p.x < sprite->width() );
  } else {
    return (p.y >= 0 && p.y < h && p.x >= 0 && p.x < w);
  }
}

const char* ezWidget::typeName() {
  const char* typeNames[14] = { "ezWidget", "ez", "ezWindow", "ezButton",
                                "ezLabel", "ezCheckBox", "ezRadiobutton",
                                "ezInput", "ezLayout", "ezListBox",
                                "ezListItem", "ezMenu", "ezHeader",
                                "ezMsgBox" };
  return typeNames[type];
}

const char* ezWidget::ident() {
  String r = typeName();
//   if      (r == "ezWindow" || r == "ezMsgBox" || r == "ezMenu")
//     r = r + ":" + static_cast<ezWindow*>(this)->title;
//   else if (r == "ezButton")
//     r = r + ":" + static_cast<ezButton*>(this)->label.text;
//   else
  r = r + ":" + String((long)this);
  return r.substring(0,24).c_str();
}

/* virtual */ void ezWidget::add(ezWidget& w) {
  ezScreen.remove(w);
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

uint16_t ezWidget::findFill() {
  if (colors.fill != NODRAW) return colors.fill;
  if (_parent) return static_cast<ezWidget*>(_parent)->findFill();
  return TFT_WHITE;
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
  ezPoint tip, tail1, tail2;

  if        (ezTheme.arrowValign == EZ_TOP   ) {
    tip.y = y + ezTheme.arrowPadding + (ezTheme.arrowWidth / 2);
  } else if (ezTheme.arrowValign == EZ_BOTTOM) {
    tip.y = y + h - ezTheme.arrowPadding - (ezTheme.arrowWidth / 2);
  } else {
    tip.y = y + (h / 2);
  }
  tail1.y = tip.y - (ezTheme.arrowWidth / 2);
  tail2.y = tip.y + (ezTheme.arrowWidth / 2);

  if        (direction == EZ_LEFT) {
    tip.x = x + ezTheme.arrowPadding;
    tail1.x = tail2.x = tip.x + ezTheme.arrowLength;
  } else if (direction == EZ_RIGHT) {
    tip.x = x + w - ezTheme.arrowPadding;
    tail1.x = tail2.x = tip.x - ezTheme.arrowLength;
  } else if (direction == EZ_UP) {
    tip.x = x + (w / 2);
    tip.y = y + ezTheme.arrowPadding;
    tail1.y = tail2.y = tip.y + ezTheme.arrowLength;
    tail1.x = tip.x - (ezTheme.arrowWidth / 2);
    tail2.x = tip.x + (ezTheme.arrowWidth / 2);
  } else {
    tip.x = x + (w / 2);
    tip.y = y + h - ezTheme.arrowPadding;
    tail1.y = tail2.y = tip.y - ezTheme.arrowLength;
    tail1.x = tip.x - (ezTheme.arrowWidth / 2);
    tail2.x = tip.x + (ezTheme.arrowWidth / 2);
  }
  parent()->fillTriangle(tip, tail1, tail2, ezTheme.arrowFill);
  parent()->drawTriangle(tip, tail1, tail2, ezTheme.arrowOutline);
}


void ezWidget::event() {

  // Pass event by all gestures for this widget
  for (auto gesture : _gestures) gesture->event();

  // This will add one for each of a widget or a gesture pointer present
  // on the event. See below
  uint8_t claims = (!!ez.e.widget) + (!!ez.e.gesture);

  _posRelParent = ez.e.to;

  // Translate parent coordinates to widget origin
  ezPoint origin(x, y);
  origin -= offset;

  ez.e.from -= origin;
  ez.e.to   -= origin;


  // Pass event by all widgets in this widget
  for (int i = _widgets.size() - 1; i >= 0; --i) {
    _widgets[i]->event();
  }

  eventPre();
  if (!numb) _eventProcess();

  // Scroll if set
  if (ez.e.widget && sprite && scroll && ez.e == E_MOVE &&
    (ez.e.widget == this || isMyDescendant(*ez.e.widget))) {
    ezPoint moveBy;
    moveBy = ez.e.from - ez.e.to;
    if (offset.x + moveBy.x < 0) moveBy.x = -offset.x;
    if (offset.y + moveBy.y < 0) moveBy.y = -offset.y;
    if (offset.x + moveBy.x > sprite->width()  - w)
      moveBy.x = sprite->width()  - w - offset.x;
    if (offset.y + moveBy.y > sprite->height() - h)
      moveBy.y = sprite->height() - h - offset.y;

    if (moveBy != ezPoint(0,0)) {
      offset  += moveBy;
      ez.e.to -= moveBy;
      refresh();
    }
  }

  eventPost();
  e = ezEvent();
  if (ez.e.widget == this) {
    fireEvent();
  } else if ((!!ez.e.widget) + (!!ez.e.gesture) > claims) {
    // Something added a widget or gesture pointer and it's not us, so
    // one of our offspring must have claimed this event.
    // So we fire it as an offspring event.
    fireEvent(true);
  }

  // Translate back to parent coordinates
  ez.e.from += origin;
  ez.e.to   += origin;

}

void ezWidget::_eventProcess() {

  // Reset last event for this widget
  e = ezEvent();

  // If E_NONE, check if we have anything that could ride in here
  if (!ez.e) {
    if (_changed) {
      _changed = false;
      if (!_state && !_cancelled) {
        // Post-releaase events
        if (!inVirtual(_lastOffEvent.to)) {
          ez.e = _lastOffEvent;
          ez.e.type = E_DRAGGED;
        } else if (_lastOffEvent.duration < tapTime) {
          if (_tapWait) {
            ez.e = _lastOffEvent;
            ez.e.type = E_DBLTAPPED;
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
          ez.e.type = E_TAPPED;
          _pressing = false;
          _longPressing = false;
          _tapWait = false;
        } else if (_state) {
          if ((!_pressing && duration > tapTime) ||
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

  // Some other widget is already dealing with this event
  if (ez.e.widget) return;

  bool parentGlissando = (parent() && parent()->glissando);

  // See if this is ours and tag it as such if it is
  if      (ez.e == E_TOUCH && contains(_posRelParent)) {
    ez.e.widget = this;
    _touched[ez.e.finger] = true;
  }
  else if (ez.e == E_MOVE && _touched[ez.e.finger]) {
    ez.e.widget = this;
    if (glissando || (parentGlissando && !contains(_posRelParent))) {
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
  // Serial.println("ezWidget.draw(), type: " + (String)type + ", ptr: " + (String)(long)this);
  if (colors.fill    != NODRAW) clear();
  drawChildren();
  if (colors.outline != NODRAW && !sprite) drawRect(colors.outline);
  push();
}

void ezWidget::drawChildren() {

  uint16_t pixels_w         = 0;
  uint16_t shares_w         = 0;
  uint16_t pixels_h         = 0;
  uint16_t shares_h         = 0;
  uint16_t per_share_w      = 0;
  uint16_t per_share_h      = 0;
  uint8_t count_w           = 0;
  uint8_t count_h           = 0;

  if (lmargin < 0) shares_w += abs(lmargin); else pixels_w += lmargin;
  if (rmargin < 0) shares_w += abs(rmargin); else pixels_w += rmargin;
  if (tmargin < 0) shares_h += abs(tmargin); else pixels_h += tmargin;
  if (bmargin < 0) shares_h += abs(bmargin); else pixels_h += bmargin;

  for (auto wdgt : _widgets) {

    // adopt widgets that may have multiple parents (ezHeader e.g.)
    wdgt->_parent = this;

    if (wdgt->setPos.x == EZ_AUTO) {
      if (wdgt->setPos.w > 0) pixels_w += wdgt->setPos.w;
      if (wdgt->setPos.w < 0 && wdgt->setPos.w > -100)  shares_w += abs(wdgt->setPos.w);
      count_w++;
    }
    if (wdgt->setPos.y == EZ_AUTO) {
      if (wdgt->setPos.h > 0) pixels_h += wdgt->setPos.h;
      if (wdgt->setPos.h < 0 && wdgt->setPos.h > -100)  shares_h += abs(wdgt->setPos.h);
      count_h++;
    }
  }

  if (gutter >= 0) {
    if (count_w) pixels_w += --count_w * gutter;
    if (count_h) pixels_h += --count_h * gutter;
  } else {
    if (count_w) shares_w += --count_w * abs(gutter);
    if (count_h) shares_h += --count_h * abs(gutter);
  }

  if (autoSize) {
    if (pixels_w > w || pixels_h > h) {
      spriteBuffer(pixels_w ? pixels_w : w, pixels_h ? pixels_h : h);
    } else {
      direct();
    }
  } else {
    if (shares_w && w - pixels_w > 0) per_share_w = (w - pixels_w) / shares_w;
    if (shares_h && h - pixels_h > 0) per_share_h = (h - pixels_h) / shares_h;
  }

  uint16_t gutter_w = gutter;
  uint16_t gutter_h = gutter;
  if (gutter < 0) {
    gutter_w = abs(gutter) * per_share_w;
    gutter_h = abs(gutter) * per_share_h;
  }

  uint16_t current_x;
  if (lmargin >= 0) current_x = lmargin;
    else current_x = abs(lmargin) * per_share_w;

  uint16_t current_y;
  if (tmargin >= 0) current_y = tmargin;
    else current_y = abs(tmargin) * per_share_h;

  for (auto wdgt : _widgets) {
    if (wdgt->setPos.w == EZ_PARENT) wdgt->w = w - current_x - rmargin;
    if (wdgt->setPos.w < 0 && wdgt->setPos.w > -100) {
      wdgt->w = abs(wdgt->setPos.w) * per_share_w;
    }
    if (wdgt->setPos.x == EZ_AUTO) {
      wdgt->x = current_x;
      current_x += wdgt->w + gutter_w;
    }

    if (wdgt->setPos.h == EZ_PARENT) wdgt->h = h - current_y - bmargin;
    if (wdgt->setPos.h < 0 && wdgt->setPos.h > -100) {
      wdgt->h = abs(wdgt->setPos.h) * per_share_h;
    }
    if (wdgt->setPos.y == EZ_AUTO) {
      wdgt->y = current_y;
      current_y += wdgt->h + gutter_h;
    }
  }

  for (auto wdgt : _widgets) {
    wdgt->draw();
  }
}

void ezWidget::spriteBuffer(int16_t w_ /* = -1 */, int16_t h_ /* = -1 */) {
  ezDisplayZone::spriteBuffer(w_, h_);
  sprite->fillSprite(findFill());
}

void ezWidget::turnOffRadiobuttons() {
  for (auto wdgt: _widgets) {
    if (wdgt->type == W_RADIOBUTTON) {
      ezRadiobutton* rb = static_cast<ezRadiobutton*>(wdgt);
      rb->value = false;
      rb->valueDraw();
    }
  }
}

void ezWidget::cancel() { _cancelled = true; }

bool ezWidget::isPressed() { return _state; }

bool ezWidget::isReleased() { return !_state; }

bool ezWidget::wasPressed() { return _state && _changed; }

bool ezWidget::wasReleased() { return (!_state && _changed); }

void ezWidget::margins(uint8_t margin) {
  lmargin = margin;
  rmargin = margin;
  tmargin = margin;
  bmargin = margin;
}

