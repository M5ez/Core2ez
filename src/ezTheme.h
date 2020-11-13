#ifndef _EZTHEME_H_
#define _EZTHEME_H_

#include <ezWidget.h>
#include <ezValues.h>

struct ezTheme {

                 // WidgetColors: fill,       text,      outline

  WidgetColors    wdw_colors    = {TFT_WHITE, TFT_BLACK, NODRAW};

  WidgetColors    btn_colors    = {TFT_BLUE,  TFT_WHITE, TFT_BLACK};
  WidgetColors    btn_onColors  = {TFT_CYAN,  TFT_BLACK, TFT_BLACK};
  const GFXfont*  btn_font      = FSSB12;

  WidgetColors    lbl_colors    = {NODRAW,    TFT_BLACK, NODRAW};
  const GFXfont*  lbl_font      = FSS9;

  WidgetColors    inp_colors    = {TFT_WHITE, TFT_BLACK, TFT_BLACK};
  const GFXfont*  inp_font      = FSSB9;

  WidgetColors    kb            = {TFT_BLACK, TFT_WHITE, NODRAW};
  const GFXfont*  kb_keyFont    = FSSB12;
  WidgetColors    kb_keys       = {TFT_WHITE, TFT_BLACK, TFT_BLACK};
  WidgetColors    kb_keysOn     = {TFT_BLACK, TFT_WHITE, NODRAW};
  WidgetColors    kb_special    = {LIGHTGREY, TFT_BLACK, TFT_BLACK};
  WidgetColors    kb_topBar     = {LIGHTGREY, TFT_BLACK, NODRAW};

  uint8_t         arrowLength   = 15;
  uint8_t         arrowWidth    = 15;
  uint8_t         arrowPadding  = 5;
  uint16_t        arrowFill     = TFT_RED;
  uint16_t        arrowOutline  = TFT_WHITE;
  int16_t         arrowValign   = EZ_BOTTOM;

  // Function to merge a WidgetColors object with a theme set.
  WidgetColors colors(WidgetColors w, WidgetColors t) {
    WidgetColors r = t;
    if (w.fill    != EZ_THEME) r.fill    = w.fill;
    if (w.text    != EZ_THEME) r.text    = w.text;
    if (w.outline != EZ_THEME) r.outline = w.outline;
    return r;
  }
};

#endif /* _EZTHEME_H_ */
