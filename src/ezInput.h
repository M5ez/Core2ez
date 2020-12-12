#ifndef _EZINPUT_H_
#define _EZINPUT_H_

#include <Core2ez.h>

#define MAX_INPUT      80
#define MAX_KEYS       30
#define CHAR_WIDTH     14
#define TEXT_VOFFSET   15
#define TEXT_COLOR      TFT_GREEN
#define CARET_WIDTH     4
#define CARET_HEIGHT   30
#define CARET_VOFFSET  10
#define CARET_COLOR     TFT_RED
#define NUM_LAYOUTS     3
#define NUM_ROWS        3
#define MAX_PER_ROW    10
#define TOPBAR_TEXT     "swipe down from top for help"
#define DEFAULT_KB      0
#define SYMBOL_KB       1
#define SHIFT_SYMBOL_KB 2

#define HELP_1          "Keyboard help"
#define HELP_2_1        "Space"
#define HELP_2_2        "Swipe right over keys"
#define HELP_3_1        "Backspace"
#define HELP_3_2        "Swipe left over keys"
#define HELP_4_1        "Upper case"
#define HELP_4_2        "Swipe up from letter"


class ezInput : public ezLabel {
 public:
  ezInput(ezWidget& parentWidget,
          int16_t x_ = EZ_INVALID, int16_t y_ = EZ_INVALID,
          int16_t w_ = 0, int16_t h_ = 0,
          String text_         = "",
          String prompt_       = "",
          WidgetColors colors_ = THEME_COLORS,
          ezFont font_ = THEME_FONT,
          int16_t align_ = EZ_LEFT,
          int16_t valign_ = EZ_CENTER,
          int16_t dx_ = 0, int16_t dy_ = 0);

  ezInput(int16_t x_ = EZ_INVALID, int16_t y_ = EZ_INVALID,
          int16_t w_ = 0, int16_t h_ = 0,
          String text_         = "",
          String prompt_       = "",
          WidgetColors colors_ = THEME_COLORS,
          ezFont font_ = THEME_FONT,
          int16_t align_ = EZ_LEFT,
          int16_t valign_ = EZ_CENTER,
          int16_t dx_ = 0, int16_t dy_ = 0);

  void init(ezWidget* pwPtr,
            int16_t x_, int16_t y_, int16_t w_, int16_t h_, String text_,
            String prompt_, WidgetColors colors_, ezFont font_,
            int16_t align_, int16_t valign_, int16_t dx_, int16_t dy_);

 virtual void eventPost();

 public:
  String      prompt;
 protected:
  bool        _changed;



};


String keyboardInput(String prompt_ = "", String text_ = "");




#endif /* _EZINPUT_H_ */

