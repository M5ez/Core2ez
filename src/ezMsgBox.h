#ifndef _EZMSGBOX_H_
#define _EZMSGBOX_H_

#include <ezWindow.h>
#include <ezHeader.h>
#include <ezLabel.h>
#include <ezLayout.h>
#include <ezButton.h>

class ezMsgBox : public ezWindow {

 public:
  static String            retval;

                           ezMsgBox(String title_, String message_ = "",
                                    String b1 = "", String b2 = "",
                                    String b3 = "");
                           ~ezMsgBox();
  void                     message(String text_, ezFont font_ = THEME_FONT,
                                   int16_t align_  = EZ_CENTER,
                                   int16_t valign_ = EZ_CENTER);
  void                     addButton(String text, void (*func)() = nullptr);
  String                   run();
  String                   pressed();
  bool                     somethingPressed();


  ezLabel                  label     = ezLabel  (0, EZ_AUTO, EZ_PARENT, -1);
  ezLayout                 buttonbox = ezLayout (0, EZ_AUTO, EZ_PARENT, 40);
  std::vector<ezButton*>   buttons;

};

String msgBox(String title_, String message_,
              String b1 = "OK", String b2 = "", String b3 = "");




#endif /* _EZMSGBOX_H_ */
