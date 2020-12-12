#ifndef _EZHEADER_H_
#define _EZHEADER_H_

#import <ez.h>

class ezHeaderClass : public ezWidget {
 public:
  static ezHeaderClass& instance() {
    static ezHeaderClass INSTANCE;
    return INSTANCE;
  }
  ezHeaderClass(ezHeaderClass const&)  = delete;
  void operator=(ezHeaderClass const&) = delete;
 private:
  ezHeaderClass();

 public:
  virtual void  draw();
  void          settings();
  bool          displayed();
  ezLabel       title       = ezLabel(EZ_AUTO, 0, -1, EZ_PARENT, "",
                                    ezTheme.hdr_colors, ezTheme.hdr_font,
                                    EZ_LEFT, EZ_CENTER);

#ifdef _EZWIFI_H_
  WifiBars      wifi        = WifiBars(EZ_AUTO, 0, 35, EZ_PARENT);
#endif

  ezGesture      swipeDown   = ezGesture(100, EZ_DOWN);

 private:
  bool           _inSettings = false;

};

extern ezHeaderClass& ezHeader;


#endif /* _EZHEADER_H_ */
