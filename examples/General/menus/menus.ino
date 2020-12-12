/*
 * Rough cut: there's no header and hence no visbile menu title and things could be prettier. Soon...
 */

#include <M5Core2.h>
#include <Core2ez.h>


void setup() {
  ez.begin();
  ezMenu main("Main menu");
  main.addItem("Wifi Settings", doFunction {
    ezWifi.menu();
  });
  main.addItem("Item 2");
  main.addItem("Item 3");

  main.addItem("Submenu", doFunction {
    ezMenu sub("Submenu");
    sub.addItem("SubItem 1", doFunction {
      if (msgBox("ezMessageBox Title", "This is an ezMessageBox message.\nIs this cool?", "Yes", "No") == "Yes") {
        msgBox ("Yup...", "Damn right!");
      }
    });
    sub.addItem("SubItem 2");
    sub.addItem("Wifi Settings", doFunction {
      ezWifi.menu();
    });
    sub.addItem("back to main");
    sub.run();
  });
  main.run();
}

void loop() {
  ez.update();
}
