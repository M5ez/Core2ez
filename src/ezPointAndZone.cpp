#include "ezPointAndZone.h"
#include <Arduino.h>


// Point class

Point::Point(int16_t x_ /* = EZ_INVALID */,
             int16_t y_ /* = EZ_INVALID */) {
  x = x_;
  y = y_;
}

bool Point::operator==(const Point& p) { return (Equals(p)); }

bool Point::operator!=(const Point& p) { return (!Equals(p)); }

Point Point::operator+(const Point& p) {
  Point r;
  r.x = x += p.x;
  r.y = y += p.y;
  return r;
}

Point Point::operator-(const Point& p) {
  Point r;
  r.x = x -= p.x;
  r.y = y -= p.y;
  return r;
}

Point& Point::operator+=(const Point& p) {
  x += p.x;
  y += p.y;
  return *this;
}

Point& Point::operator-=(const Point& p) {
  x -= p.x;
  y -= p.y;
  return *this;
}

Point::operator char*() {
  if (valid()) {
    sprintf(_text, "(%3d, %3d)", x, y);
  } else {
    sprintf(_text, "(invalid )");
  }
  return _text;
}

Point::operator bool() { return valid(); }

void Point::set(int16_t x_ /* = EZ_INVALID */,
                int16_t y_ /* = EZ_INVALID */) {
  x = x_;
  y = y_;
}

bool Point::Equals(const Point& p) { return (x == p.x && y == p.y); }

bool Point::in(Zone& z) {
  return (z.contains(x, y));
}

bool Point::valid() { return !(x == EZ_INVALID || y == EZ_INVALID); }

uint16_t Point::distanceTo(const Point& p) {
  int16_t dx = x - p.x;
  int16_t dy = y - p.y;
  return sqrt(dx * dx + dy * dy) + 0.5;
}

uint16_t Point::directionTo(const Point& p) {
  // 57.29578 =~ 180/pi, see https://stackoverflow.com/a/62486304
  uint16_t a = (uint16_t)(450.5 + (atan2(p.y - y, p.x - x) * 57.29578));
  a = (360 + a) % 360;
  return a;
}

bool Point::isDirectionTo(const Point& p, int16_t wanted,
                          uint8_t plusminus /* = PLUSMINUS */) {
  if      (wanted == EZ_UP   ) wanted = 0;
  else if (wanted == EZ_RIGHT) wanted = 90;
  else if (wanted == EZ_DOWN ) wanted = 180;
  else if (wanted == EZ_LEFT ) wanted = 270;

  uint16_t a = directionTo(p);
  return (min(abs(wanted - a), 360 - abs(wanted - a)) <= plusminus);
}

void Point::rotate(uint8_t m) {
  if (m == 1 || !valid()) return;
  int16_t normal_x = x;
  int16_t normal_y = y;
  int16_t inv_x = HIGHEST_X - x;
  int16_t inv_y = HIGHEST_Y - y;
  // inv_y can be negative for area below screen of M5Core2
  switch (m) {
    case 0:
      x = inv_y;
      y = normal_x;
      break;
    case 2:
      x = normal_y;
      y = inv_x;
      break;
    case 3:
      x = inv_x;
      y = inv_y;
      break;
    // rotations 4-7 are mirrored
    case 4:
      x = inv_y;
      y = inv_x;
      break;
    case 5:
      x = normal_x;
      y = inv_y;
      break;
    case 6:
      x = normal_y;
      y = normal_x;
      break;
    case 7:
      x = inv_x;
      y = normal_y;
      break;
  }
}

// Zone class

Zone::Zone(int16_t x_ /* = EZ_INVALID */, int16_t y_ /* = EZ_INVALID */,
           int16_t w_ /* = 0 */, int16_t h_ /* = 0 */
) {
  set(x_, y_, w_, h_);
}

Zone::operator bool() { return !(x == EZ_INVALID && y == EZ_INVALID); }

/*virtual */ void Zone::set(int16_t x_ /* = EZ_INVALID */,
                            int16_t y_ /* = EZ_INVALID */,
                            int16_t w_ /* = 0 */, int16_t h_ /* = 0 */) {
  x = x_;
  y = y_;
  w = w_;
  h = h_;
}

bool Zone::valid() { return !(x == EZ_INVALID && y == EZ_INVALID); }

bool Zone::contains(const Point& p) {
  return contains(p.x, p.y);
}

bool Zone::contains(int16_t x_, int16_t y_) {
  return (y_ >= y && y_ < y + h && x_ >= x && x_ < x + w);
}

void Zone::rotate(uint8_t m) {
  if (m == 1) return;
  int16_t normal_x = x;
  int16_t normal_y = y;
  int16_t inv_x = HIGHEST_X - x - w;
  int16_t inv_y = HIGHEST_Y - y - h;  // negative for area below screen
  switch (m) {
    case 0:
      x = inv_y;
      y = normal_x;
      break;
    case 2:
      x = normal_y;
      y = inv_x;
      break;
    case 3:
      x = inv_x;
      y = inv_y;
      break;
    // rotations 4-7 are mirrored
    case 4:
      x = inv_y;
      y = inv_x;
      break;
    case 5:
      x = normal_x;
      y = inv_y;
      break;
    case 6:
      x = normal_y;
      y = normal_x;
      break;
    case 7:
      x = inv_x;
      y = normal_y;
      break;
  }
  if (!(m % 2)) std::swap(w, h);
}
