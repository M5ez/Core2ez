#include <ezGesture.h>
#include <ez.h>

ezGesture::ezGesture(uint16_t minDistance_,
                 int16_t direction_ /* = EZ_INVALID */,
                 uint8_t plusminus_ /* = PLUSMINUS */,
                 uint16_t maxTime_ /* = GESTURE_MAXTIME */
) {
  minDistance = minDistance_;
  direction = direction_;
  plusminus = plusminus_;
  maxTime = maxTime_;
  // ez.add(*this);
}

ezGesture::operator bool() { return (e); }

ezWidget* ezGesture::parent() { return _parent; }

void ezGesture::event() {
  if (ez.e != E_RELEASE ) return;
  if (ez.e.gesture) return;
  if (_parent && !_parent->contains(ez.e.from)) return;
  if (ez.e.from.distanceTo(ez.e.to) < minDistance) return;
  if (direction != EZ_INVALID &&
      !ez.e.isDirection(direction, plusminus)) return;
  if (ez.e.duration > maxTime) return;
  ez.e.gesture = this;
  fireEvent();
}
