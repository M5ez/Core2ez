#include <ezGesture.h>
#include <ezRoot.h>

ezGesture::ezGesture(Zone fromZone_, Zone toZone_,
                 uint16_t minDistance_ /* = GESTURE_MINDIST */,
                 int16_t direction_ /* = INVALID_VALUE */,
                 uint8_t plusminus_ /* = PLUSMINUS */,
                 uint16_t maxTime_ /* = GESTURE_MAXTIME */
) {
  fromZone = fromZone_;
  toZone = toZone_;
  minDistance = minDistance_;
  direction = direction_;
  plusminus = plusminus_;
  maxTime = maxTime_;
}

ezGesture::ezGesture(uint16_t minDistance_,
                 int16_t direction_ /* = INVALID_VALUE */,
                 uint8_t plusminus_ /* = PLUSMINUS */,
                 uint16_t maxTime_ /* = GESTURE_MAXTIME */
) {
  fromZone = ANYWHERE;
  toZone = ANYWHERE;
  minDistance = minDistance_;
  direction = direction_;
  plusminus = plusminus_;
  maxTime = maxTime_;
  ez.add(*this);
}

ezGesture::operator bool() { return (lastEvent); }

ezWidget* ezGesture::parent() { return _parent; }

void ezGesture::event(Event& e) {
  if (e != E_RELEASE ) return;
  if (e.from.distanceTo(e.to) < minDistance) return;
  if (fromZone && !fromZone.contains(e.from)) return;
  if (toZone && !toZone.contains(e.to)) return;
  if (direction != INVALID_VALUE &&
      !e.isDirection(direction, plusminus)) return;
  if (e.duration > maxTime) return;
  if (e.gesture) return;

  e.gesture = this;
}
