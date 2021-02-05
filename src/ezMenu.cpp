#include "ezMenu.h"

#include <Arduino.h>
#include <ezList.h>

int16_t ezMenu::retval = EZ_INVALID;

ezMenu* ezMenu::caller = nullptr;

ezMenu::ezMenu(String title_,
               WidgetColors colors_   /* = THEME_COLORS */,
               WidgetColors onColors_ /* = THEME_COLORS */,
               ezFont font_           /* = THEME_FONT */) {
  set(0, 0, EZ_PARENT, EZ_PARENT);
  add(ezHeader);
  title         = title_;
  colors        = ezTheme.colors(colors_,   ezTheme.mnu_colors);
  onColors      = ezTheme.colors(onColors_, ezTheme.mnu_onColors);
  font          = font_ ? font_ : ezTheme.mnu_font;
}

void ezMenu::addItem(String text_) {
  ezMenuItem mi;
  mi.text = text_;
  mi.func = nullptr;
  items.push_back(mi);
}

void ezMenu::addItem(String text_, void (*func_)()) {
  ezMenuItem mi;
  mi.text = text_;
  mi.func = func_;
  items.push_back(mi);
}

void ezMenu::addItem(String text_, int16_t retval_) {
  ezMenuItem mi;
  mi.text = text_;
  mi.retval = retval_;
  items.push_back(mi);
}

int16_t ezMenu::run(bool noBreak /* = false */) {
  while (true) {
    int16_t r = runOnce();
    if (r == EZ_BREAK && !noBreak) return EZ_BREAK;
    if (r == EZ_BACK  && !noBreak) return EZ_INVALID;
  }
}

int16_t ezMenu::runOnce() {
  selected = doMenu();
  if (selected == EZ_BREAK) {
    return EZ_BREAK;
  }
  ezMenuItem& item = items[selected];
  if (item.func) {
    caller = this;
    item.func();
  }
  return (item.retval != EZ_INVALID) ? item.retval : selected;
}

int16_t ezMenu::doMenu() {

  ezLayout canvas(*this, 0, EZ_AUTO, EZ_PARENT, -1);
  canvas.autoSize     = true;
  canvas.spriteBPP    = spriteBPP;
  canvas.tmargin      = margin;
  canvas.gutter       = margin;

  if (itemsPerRow   == EZ_AUTO) {
    if      (items.size() <= 3) itemsPerRow = 1;
    else if (items.size() <= 6) itemsPerRow = 2;
    else                        itemsPerRow = 3;
  }

  log_v("itemsPerRow: %d", itemsPerRow);

  uint8_t numRows     = ((items.size() + 1) / itemsPerRow);
  log_v("numRows: %d", numRows);

  if (rowsPerScreen == EZ_AUTO) rowsPerScreen = min(numRows, 3);
  log_v("rowsPerScreen: %d", rowsPerScreen);

  uint16_t screenHeight = ez.h - ezHeader.h;
  uint16_t rowHeight    = (screenHeight - ((rowsPerScreen + 1) * margin)) /
                           rowsPerScreen;
  log_v("rowHeight: %d", rowHeight);

  ezLayout* rows[numRows];
  for (uint8_t r = 0; r < numRows; r++) {
    rows[r] = new ezLayout(0, EZ_AUTO, EZ_PARENT, rowHeight);
    rows[r]->lmargin = margin;
    rows[r]->rmargin = margin;
    rows[r]->gutter = margin;
    canvas.add(*rows[r]);
  }

  ezButton* btns[items.size()];
  for (uint8_t i = 0; i < items.size(); i++) {
    uint8_t r = i / itemsPerRow;
    btns[i] = new ezButton(EZ_AUTO, 0, -1, EZ_PARENT, items[i].text);
    btns[i]->label.font   = font;
    btns[i]->label.wrap   = true;
    btns[i]->label.snug   = true;
    btns[i]->colors       = colors;
    btns[i]->onColors     = onColors;
    btns[i]->userData     = i;
    btns[i]->cornerRadius = cornerRadius;
    rows[r]->add(*btns[i]);
  }

  bool scrolling = (numRows > rowsPerScreen);

  // Only E_TAPPED if the menu scrolls, so options are not selected when
  // moving the menu up or down.
  canvas.onOffspring(scrolling ? E_TAPPED : E_TAPPED + E_PRESSED, doFunction {
    if (eventWidget(ezButton, b)) {
      ezMenu::retval = b->userData;
    }
  });

  if (!scrolling) {
    spriteBuffer();
    focus();
    direct();
  } else {
    focus();
  }

  uint16_t loopnum = 0;
  retval = EZ_INVALID;
  while (retval == EZ_INVALID) {
    loop();
  }

  for (auto row: rows) delete row;
  for (auto btn: btns) delete btn;
  remove(canvas);

  return retval;

}



void ezBreak() { ezMenu::retval = EZ_BREAK; }

void ezBack()  { ezMenu::retval = EZ_BACK; }
