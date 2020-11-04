#ifndef _EZGESTURE_H_
#define _EZGESTURE_H_

#include <ezEvents.h>

#define GESTURE_MAXTIME          500
#define GESTURE_MINDIST           75
#define GESTURE_PLUSMINUS         30
#define ANYWHERE                Zone()


class ezGesture : public Eventful {
 public:
  ezGesture(Zone fromZone_,
            Zone toZone_,
            uint16_t minDistance_ = GESTURE_MINDIST,
            int16_t direction_ = INVALID_VALUE,
            uint8_t plusminus_ = PLUSMINUS,
            uint16_t maxTime_ = GESTURE_MAXTIME);
  ezGesture(uint16_t minDistance_ = GESTURE_MINDIST,
            int16_t direction_ = INVALID_VALUE,
            uint8_t plusminus_ = PLUSMINUS,
            uint16_t maxTime_ = GESTURE_MAXTIME);
  operator bool();
  void event(Event& e);
  ezWidget* parent();
  Zone fromZone;
  Zone toZone;
  int16_t direction             = INVALID_VALUE;
  uint8_t plusminus             = GESTURE_PLUSMINUS;
  uint16_t maxTime              = GESTURE_MAXTIME;
  uint16_t minDistance          = GESTURE_MINDIST;

 protected:
  friend class ezWidget;
  ezWidget*                     _parent;
};


#endif /* _EZGESTURE_H_ */
