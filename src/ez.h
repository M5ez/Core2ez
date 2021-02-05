#ifndef _EZ_H_
#define _EZ_H_

#include <Core2ez.h>
// #include <ezWidget.h>
// #include <ezWindow.h>
// #include <ezGesture.h>


#define MAX_WINDOWSTACK       15

class ezClass : public ezWidget {
 public:
  static ezClass& instance() {
    static ezClass INSTANCE;
    return INSTANCE;
  }
  ezClass(ezClass const&)         = delete;
  void operator=(ezClass const&)  = delete;
 private:
  ezClass();

 public:
  void                        begin();
  void                        update();
  void                        draw();
  void                        add(ezWidget& w);
  void                        remove(ezWidget& w);
  void                        add(ezWindow& w);
  void                        remove(ezWindow& w);
  void                        add(ezGesture& g);
  void                        remove(ezGesture& g);

 protected:
  uint8_t                     _finger         = 0;
  ezPoint                     _previous  [2]  = {ezPoint(), ezPoint()};
  ezPoint                     _startPoint[2]  = {ezPoint(), ezPoint()};
  uint32_t                    _startTime [2]  = {0, 0};

};

extern ezClass& ez;


#endif /* _EZ_H_ */
