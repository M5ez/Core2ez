#ifndef _EZTHEME_H_
#define _EZTHEME_H_

#include <ezWidget.h>
#include <ezValues.h>

class ezThemeClass {
 public:
  static ezThemeClass& instance() {
    static ezThemeClass INSTANCE;
    return INSTANCE;
  }
  ezThemeClass(ezThemeClass const&)    = delete;
  void operator=(ezThemeClass const&)  = delete;
 private:
  ezThemeClass() {}

 public:

                 // WidgetColors: fill,       text,      outline

  WidgetColors    wgt_colors    = {NODRAW,    NODRAW,    NODRAW};

  WidgetColors    wdw_colors    = {TFT_WHITE, TFT_BLACK, NODRAW};

  WidgetColors    hdr_colors    = {TFT_BLUE,  TFT_WHITE, NODRAW};
  ezFont          hdr_font      = FSSB9;
  int16_t         hdr_height    = 23;

  WidgetColors    btn_colors    = {TFT_BLUE,  TFT_WHITE, TFT_BLACK};
  WidgetColors    btn_onColors  = {TFT_CYAN,  TFT_BLACK, TFT_BLACK};
  ezFont          btn_font      = FSSB12;

  WidgetColors    lbl_colors    = {NODRAW,    TFT_BLACK, NODRAW};
  ezFont          lbl_font      = FSS9;

  WidgetColors    inp_colors    = {TFT_WHITE, TFT_BLACK, TFT_BLACK};
  ezFont          inp_font      = FSSB9;

  WidgetColors    lst_colors    = {TFT_WHITE, TFT_BLACK, TFT_BLACK};
  ezFont          lst_font      = FSS12;
  WidgetColors    lsl_colors    = {0xFD75,    TFT_BLACK, NODRAW};



  WidgetColors    chk_colors    = {TFT_WHITE, TFT_BLACK, TFT_BLACK};
  ezFont          chk_font      = FSS9;

  WidgetColors    rdb_colors    = {TFT_WHITE, TFT_BLACK, TFT_BLACK};
  ezFont          rdb_font      = FSS9;

  WidgetColors    kb            = {TFT_BLACK, TFT_WHITE, NODRAW};
  ezFont          kb_keyFont    = FSSB12;
  WidgetColors    kb_keys       = {TFT_WHITE, TFT_BLACK, TFT_BLACK};
  WidgetColors    kb_keysOn     = {TFT_BLACK, TFT_WHITE, NODRAW};
  WidgetColors    kb_special    = {LIGHTGREY, TFT_BLACK, TFT_BLACK};
  WidgetColors    kb_topBar     = {LIGHTGREY, TFT_BLACK, NODRAW};

  WidgetColors    mnu_colors    = {TFT_LIGHTGREY, TFT_BLACK, NODRAW};
  WidgetColors    mnu_onColors  = {TFT_DARKGREY, TFT_WHITE, NODRAW};
  ezFont          mnu_font      = FSS12;

  WidgetColors    msg_colors    = {NODRAW,    TFT_BLACK, NODRAW};
  ezFont          msg_font      = FSSB12;

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

extern ezThemeClass& ezTheme;

#endif /* _EZTHEME_H_ */
