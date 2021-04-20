/*

== The PointAndZone Library ==

  This library was written to supply ezPoint and ezZone, common primitives for
  M5Display and M5Button, libraries originally written for the M5Stack series
  of devices. They could be useful for many other applications, especially
  anything based on a TFT_eSPI display driver.


== ezPoint and ezZone: Describing Points and Areas on the Screen ==

  The ezPoint and ezZone classes allow you to create variables that hold a point
  or an area on the screen.

  ezPoint(x, y)

    Holds a point on the screen. Has members x and y that hold the coordinates
    of a touch. Values EZ_INVALID for x and y indicate an invalid value,
    and that's what a point starts out with if you declare it without
    parameters. The 'valid()' method tests if a point is valid. If you
    explicitly evaluate a ezPoint as a boolean ("if (p) ..."), you also get
    whether the point is valid, so this is equivalent to writing "if
    (p.valid()) ...".

  ezZone(x, y, w, h)

    Holds a rectangular area on the screen. Members x, y, w and h are for the
    x and y coordinate of the top-left corner and the width and height of the
    rectangle.

  The 'set' method allows you to change the properties of an existing ezPoint
  or ezZone. Using the 'in' or 'contains' method you can test if a point lies
  in a zone.

  The PointAndZone library also provides the low-level support for direction
  from one point to another and for screen rotation translations.


== Looking for Directions? ==

  This library allows you to find the distance in pixels between two ezPoint
  objects with "A.distanceTo(B)". Using "A.directionTo(B)" will output a
  compass course in degrees from A to B. That is, if A lies directly above A
  on the screen the output will be 0, if B lies to the left of A, the output
  will be 270. You can also test whether a direction matches some other
  direction by using "A.isDirectionTo(B, 0, 30)". What this does is take the
  direction from A to B and output 'true' if it is 0, plus or minus 30
  degrees. (So either between 330 and 359 or between 0 and 30).

  Do note that unlike in math, on a display the Y-axis points downwards. So
  pixel coordinates (10, 10) are at direction 135 deg from (0, 0).

  Distance and direction functionality is used in Gesture recognition in the
  M5Button highler level library. Its 'ezEvent' objects have methods that look
  very much like these, except the 'To' in the name is missing because Events
  have a starting and ending point so you can just print
  "myEvent.direction()" or state "if (myEvent.isDirection(0,30) ..."


== Some Examples ==

  ezPoint a;
  ezPoint b(50, 120);
  Serial.println(a.valid());                    // 0
  Serial.println(a);                            // (invalid)
  a.set(10, 30);
  Serial.println(a);                            // (10,30)
  Serial.println(a.valid());                    // 1
  Serial.println(b.y);                          // 120
  Serial.println(a.distanceTo(b));              // 98
  Serial.println(a.directionTo(b));             // 156
  Serial.println(a.isDirectionTo(b, 0, 30));    // 0
  ezZone z(0,0,100, 100);
  Serial.println(z.w);                          // 100
  Serial.println(z.contains(a));                // 1
  Serial.println(b.in(z));                      // 0

*/

#ifndef _EZPOINTANDZONE_H_
#define _EZPOINTANDZONE_H_

#include <Arduino.h>
#include <ezValues.h>

#define PLUSMINUS 45          // default value for isDirectionTo
#define HIGHEST_X 319         // Can't trust TFT_WIDTH, driver is portrait
#define HIGHEST_Y 239

class ezZone;


/// Stores a point on the screen.
/**
  Holds the coordinates of a point on the screen. Has methods to do various
  determinations and calculations.
*/
class ezPoint {
 public:
  /**
    As you declare a point, you can provide x and y coordinates. If you don't
    specify, the point will be invalid. ( Which means both its `x` and `y`
    coordinates are set to the special value `EZ_INVALID`. )
  */
  ezPoint(int16_t x_ = EZ_INVALID, int16_t y_ = EZ_INVALID);

  /**
    `True` if this point has same coordinates as point `p`
  */
  bool Equals(const ezPoint& p);

  bool operator==(const ezPoint& p);
  /**
    `operator==` and `operator!=` cause `if (a == b) ...` and `if (a != b) ...`
    to compare coordinates instead of checking whether the variables point to
    the same object instance. Shortcut for `if (a.Equals(b)) ...` and
    `if (!a.Equals(b)) ...`
  */
  bool operator!=(const ezPoint& p);


  ezPoint operator+(const ezPoint &p);
  ezPoint operator-(const ezPoint &p);
  ezPoint& operator+=(const ezPoint& p);
  /**
    `operator+`, `operator-`, `operator+=` and `operator-=` are there so you
    can add and subtract ezPoints to do vector calculations. Simply adds or
    subtracts the coordinates for each axis.
  */
  ezPoint& operator-=(const ezPoint& p);

  bool valid();         ///< `True` if this point is valid.

  /**
    returns a character representation of the point, such as `( 10, 200)` or
    `(invalid )`. Note that the coordinates are left-padded with spaces such
    that the returned string is always 10 characters in size.
  */
  char* c_str();

  /**
    Allows you to say `if (somePoint) ...` to test if a point is valid.
    Equivalent to `if (somePoint.valid() ...`
  */
  explicit operator bool();

  operator char*(); ///< Converting a point to a string calls c_str().

  /**
    Allows you to change the coordinates of an existing point. `.set()` will
    mark the point as invalid.
  */
  void set(int16_t x_ = EZ_INVALID, int16_t y_ = EZ_INVALID);

  bool in(ezZone& z);   ///< `True` if this point lies inside ezZone `z`.
  /**
    Returns the distance from here to point `p`, in whole pixels using
    pythagoras.
    \param p the point towards which we are calculating distance
  */
  uint16_t distanceTo(const ezPoint& p);
  /**
    Returns the compass direction from here to point `p`, in whole degrees.
    \param p the point towards which we are calculating direction
  */
  uint16_t directionTo(const ezPoint& p);
  /**
    `True` if the compass direction from here to point `p` equals `wanted`,
    plus or minus `plusminus`.
    \param p the point towards which we are calculating direction
    \param wanted the compass direction to test for in whole degrees
    \param plusminus how much deviation the test still accepts. \
           Defaults to 45 degrees.
  */
  bool isDirectionTo(const ezPoint& p, int16_t wanted,
                     uint8_t plusminus = PLUSMINUS);
  /**
    Rewrites the coordinates to the screen rotation indicated.
    \param m Screen rotation (0-7)
  */
  void rotate(uint8_t m);

  int16_t x;      ///< x-coordinate of point, or EZ_INVALID
  int16_t y;      ///< y-coordinate of point, or EZ_INVALID

 protected:
  char _text[12];
};

class ezZone {
 public:
  ezZone(int16_t x_ = EZ_INVALID, int16_t y_ = EZ_INVALID, int16_t w_ = 0,
       int16_t h_ = 0);
  explicit operator bool();
  bool valid();
  virtual void set(int16_t x_ = EZ_INVALID, int16_t y_ = EZ_INVALID,
                   int16_t w_ = 0 , int16_t h_ = 0);
  bool contains(const ezPoint& p);
  bool contains(int16_t x, int16_t y);
  void rotate(uint8_t m);
  int16_t x, y, w, h;
};

#endif /* _EZPOINTANDZONE_H_ */
