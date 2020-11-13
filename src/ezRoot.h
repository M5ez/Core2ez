#ifndef _EZROOT_H_
#define _EZROOT_H_

#include <Core2ez.h>

#define MAX_WINDOWSTACK       10

class ezRoot : public ezWidget {
 public:
  static ezRoot*              instance;
                              ezRoot();
  void                        begin();
  void                        update();
  void                        draw();
  void                        add(ezWidget& w);
  void                        remove(ezWidget& w);
  void                        add(ezWindow& w);
  void                        remove(ezWindow& w);
  void                        add(ezGesture& g);
  void                        remove(ezGesture& g);

  ezSound                     Sound;
  ezTouch                     Touch;
  ezTheme                     Theme;
  ezWindow                    Screen     = ezWindow(0, 0, 320, 240);

 protected:
  uint8_t                     _finger           = 0;
  Point                       _previous  [2]    = {Point(), Point()};
  Point                       _startPoint[2]    = {Point(), Point()};
  uint32_t                    _startTime [2]    = {0, 0};
};

extern ezRoot ez;

#define ROOT                  ezRoot::instance


#endif /* _EZROOT_H_ */
