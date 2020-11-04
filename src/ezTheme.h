#ifndef _EZTHEME_H_
#define _EZTHEME_H_

#include <ezWidget.h>

struct ezTheme {
  WidgetColors  ezWindow_colors     = {TFT_WHITE, TFT_BLACK, TFT_BLACK};
  WidgetColors  ezButton_colors     = {TFT_BLUE,  TFT_WHITE, TFT_BLACK};
  WidgetColors  ezButton_onColors   = {TFT_CYAN,  TFT_BLACK, TFT_BLACK};
};

#endif _EZTHEME_H_
