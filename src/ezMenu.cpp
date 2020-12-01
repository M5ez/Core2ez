#include "ezMenu.h"

uint8_t ezMenu::_quitting = 0;

void ezMenu::quit(uint8_t count /* = 1 */) {
  _quitting = count;
}

ezMenu::ezMenu(String title) {
  // ezHeader.title.text = title;
  // add(ezHeader);
  type                 = W_MENU;
  gutter               = 20;
  add(listbox);
  listbox.colors       = ezTheme.mnu_colors;
  listbox.itemNormal   = colors;
  listbox.itemSelected = colors;
  listbox.spacing      = 10;
  listbox.font         = FSSB12;
  _quitting = false;
}

ezMenu::~ezMenu() {
  for (auto item: items) delete item;
}

void ezMenu::addItem(String text, void (*func)() /* = nullptr */) {
  items.push_back(new ezListItem(listbox, text));
  if (func) items.back()->addHandler(func, E_TAPPED);
}

void ezMenu::run() {
  if    (!_quitting) focus();
  while (!_quitting) loop();
  _quitting--;
  blur();
}

