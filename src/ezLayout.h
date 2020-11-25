#ifndef _EZLAYOUT_H_
#define _EZLAYOUT_H_

#include <ezWidget.h>

class ezLayout : public ezWidget {
 public:
  ezLayout(ezWidget& parentWidget,
           int16_t x_ = EZ_INVALID, int16_t y_ = EZ_INVALID,
           int16_t w_ = 0, int16_t h_ = 0, int16_t gutter_ = 0);
  ezLayout(int16_t x_ = EZ_INVALID, int16_t y_ = EZ_INVALID,
           int16_t w_ = 0, int16_t h_ = 0, int16_t gutter_ = 0);
};

#endif /* _EZLAYOUT_H_ */
