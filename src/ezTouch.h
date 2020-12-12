/*

== M5Touch - The M5Stack Core2 Touch Library ==

  This is the library behind the M5.Touch object that you can use to read from
  the touch sensor on the M5Stack Core2.


== About the Touch Sensor in the M5Stack Core2 ==

  Touchpanel interfacing is done by a FocalTech FT6336 chip, which supports
  two simultaneous touches. However, the M5Stack Core2 touch display is only
  multi-touch in one dimension. What that means is that it can detect two
  separate touches only if they occur on different vertical positions. This
  has to do with the way the touch screen is wired, it's not something that
  can be changed in software. So you will only ever see two points if they do
  not occur side-by-side. Touches that do happen side-by-side blend into one
  touch that is detected somewhere between the actual touches.

  While this limits multi-touch somewhat, you can still create multiple
  buttons and see two that are not on the same row simultaneously. You could
  also use one of the buttons below the screen as a modifier for something
  touched on the screen.

  The touch sensor extends to below the screen of the Core2: the sensor maps
  to 320x280 pixels, the screen is 320x240. The missing 40 pixels are placed
  below the screen, where the printed circles are. This is meant to simulate
  the three hardware buttons on the original M5Stack units. Note that on some
  units the touch sensor in this area only operates properly if the USB cable
  is plugged in or if the unit is placed firmly in your hand on a metal
  surface.

  For a quick view of how the sensor sees the world, try the 'touch' example
  that comes with this library. (File / Examples / M5Core2 / Basics/ touch)


== TouchPoint ==

  TouchPoint is a variable type to desxribe a point on the touch sensor. It
  has members x and y and can be intialised as follows:

    Holds a point on the screen. Has members x and y that hold the coordinates
    of a touch. Values EZ_INVALID for x and y indicate an invalid value,
    and that's what a point starts out with if you declare it without
    parameters. The 'valid()' method tests if a point is valid. If you
    explicitly evaluate a TouchPoint as a boolean ("if (p) ..."), you also get
    whether the point is valid, so this is equivalent to writing "if
    (p.valid()) ...".

  The 'set' method allows you to change the properties of an existing
  TouchPoint:


== Touch API ==

  The Touch API provides a way to read the data from the touch sensor.


  M5.update() In the loop() part of your sketch, call "M5.update()". This will
  in turn call M5.Touch.update(), which is the only part that talks to the
  touch interface. It updates the data used by the rest of the API.

  M5.Touch.changed

    Is true if anything changed on the touchpad since the last time
    M5.update() was called.

  M5.Touch.points

    Contains the number of touches detected: 0, 1 or 2.

  M5.Touch.point[0], M5.Touch.point[1]

    M5.Touch.point[0] and M5.Touch.point[1] are TouchPoint variables that hold
    the detected touches. Below is a very simple sketch to print the location
    where the screen is touched. As you can see this only prints where the
    first finger touches, a second finger is ignored.

      #include <M5Core2.h>

      void setup() {
        M5.begin();
      }

      void loop() {
        M5.update();
        if (M5.Touch.changed) {
          TouchPoint p = M5.Touch.point[0];
          Serial.printf("(%d, %d)\n", p.x, p.y);
        }
      }


== Screen Rotation ==

  If you rotate the screen with M5.Lcd.setRotation, the touch coordinates will
  rotate along with it.

  * What that means is that either x or y for the area below the screen may go
  negative. Say you use the screen upside-down with M5.Lcd.setRotation(3). In
  that case the off-screen touch area (Y coordinates 240 through 279) that was
  below the screen now becomes above the screen and has Y coordinates -40
  through -1.


== Advanced Uses of the Touch Library ==

  You should never need any of the below features in everyday use. But
  they're there just in case...

  M5.Touch.wasRead
    True if the sensor was actually read. The sensor can only provide updates
    every 13 milliseconds or so. M5.update() can loop as quick as once every
    20 MICROseconds, meaning it would continually read the sensor when there
    was nothing to read. So M5.Touch.read() only really reads when it's time
    to do so, and returns with M5.Touch.wasRead false otherwise.

  M5.Touch.interval()
    Without arguments returns the current interval between sensor updates in
    milliseconds. If you supply a number that's the new interval. The default
    of 13 seems to give the most updates per second.

  M5.Touch.ft6336(reg)
  M5.Touch.ft6336(reg, value)
  M5.Touch.ft6336(reg, size, *data)
    Allows you to read and write registers on the ft6336 touch interface chip
    directly. The first form reads one byte, the second form writes one and
    the third form reads a block of 'size' bytes starting at 'reg' into a
    buffer at '*data'.

  M5.Touch.dump()
    M5.Touch.dump() dumps the entire register space on the FT6336 chip as a
    formatted hexdump to the serial port.


== Legacy API ==

  There was a previous version of this library, and it provided a number of
  functions that were single touch only. The older version did not have
  M5.update(). Instead it used ispressed() and getPressedPoint() functions as
  well as HotZones. This older API is still supported (the M5Core2 Factory
  Test sketch still works), but you should not use it for new programs. The
  ispressed() function specifically does not mix well with code that uses
  M5.update().

*/

#ifndef _EZTOUCH_H_
#define _EZTOUCH_H_

#include <Arduino.h>
#include <ezPointAndZone.h>

#define TOUCH_W 320
#define TOUCH_H 280
#define CST_DEVICE_ADDR 0x38
#define CST_INT 39

// Strangely, the value 13 leads to slightly more frequent updates than 10
#define DEFAULT_INTERVAL  13

class ezTouchClass {
 public:
  static ezTouchClass& instance() {
    static ezTouchClass INSTANCE;
    return INSTANCE;
  }
  ezTouchClass(ezTouchClass const&)         = delete;
  void operator=(ezTouchClass const&)  = delete;
 private:
  ezTouchClass() {}

 public:
  void begin();
  uint8_t ft6336(uint8_t reg);
  void ft6336(uint8_t reg, uint8_t value);
  void ft6336(uint8_t reg, uint8_t size, uint8_t* data);
  uint8_t interval(uint8_t ivl);
  uint8_t interval();
  void update();
  bool read();
  bool ispressed();
  void dump();
  ezPoint getPressPoint();
  uint8_t points;
  bool changed, wasRead;
  bool pchanged[2];
  ezPoint point[2];
 protected:
  uint8_t _interval;
  uint32_t _lastRead;
};

extern ezTouchClass& ezTouch;

#endif /* _EZTOUCH_H_ */
