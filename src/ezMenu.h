#ifndef _EZMENU_H_
#define _EZMENU_H_

#include <ezWindow.h>
#include <ezList.h>

#define exitMenu(...)      ezMenu::quit(__VA_ARGS__)

class ezMenu : public ezWindow {

 public:
  static void              quit(uint8_t count = 1);
 protected:
  static uint8_t           _quitting;

 public:
                           ezMenu(String title);
                           ~ezMenu();
  void                     addItem(String text, void (*func)() = nullptr);
  void                     run();
  ezListBox                listbox = ezListBox(0, EZ_AUTO, EZ_PARENT, -1);
  std::vector<ezListItem*> items;

};

#endif /* _EZMENU_H_ */
