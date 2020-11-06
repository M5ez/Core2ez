#ifndef _EZTHEME_H_
#define _EZTHEME_H_

#include <ezWidget.h>
#include <ezValues.h>

struct ezTheme {
  WidgetColors  ezWindow_colors       = {TFT_WHITE, TFT_BLACK, NODRAW};
  WidgetColors  ezButton_colors       = {TFT_BLUE,  TFT_WHITE, TFT_BLACK};
  WidgetColors  ezButton_onColors     = {TFT_CYAN,  TFT_BLACK, TFT_BLACK};
  uint16_t      ezKeyboard_background = TFT_BLACK;
  WidgetColors  ezKeyboard_keys       = {TFT_WHITE, TFT_BLACK, TFT_BLACK};
  WidgetColors  ezKeyboard_keysOn     = {TFT_BLACK, TFT_WHITE, NODRAW};
  WidgetColors  ezKeyboard_special    = {LIGHTGREY, TFT_BLACK, TFT_BLACK};

  uint8_t       arrowLength           = 15;
  uint8_t       arrowWidth            = 15;
  uint8_t       arrowPadding          = 5;
  uint16_t      arrowFill             = TFT_RED;
  uint16_t      arrowOutline          = TFT_WHITE;
  int16_t       arrowValign           = EZ_BOTTOM;
};

#endif /* _EZTHEME_H_ */
