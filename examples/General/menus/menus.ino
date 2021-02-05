#include <M5Core2.h>
#include <Core2ez.h>


void setup() {
  ez.begin();
  ezMenu main("Main menu");
  main.itemsPerRow = 1;
  main.rowsPerScreen = 2;
  main.addItem("Wifi Settings", doFunction {
    ezWifi.menu();
  });
  main.addItem("Item 2");
  main.addItem("Item 3");
  main.addItem("Item 4");
  main.addItem("Item 5");
  main.addItem("Item 6");
  main.addItem("Item 7");
  main.addItem("Item 8");
  main.addItem("Item 9");
  main.addItem("Item 10");
  main.addItem("Item 11");
  main.addItem("Item 12");
  main.addItem("Item 13");
  main.addItem("Item 14");
  main.addItem("Item 15");
  main.addItem("Item 16");
  main.addItem("Item 17");
  main.addItem("Item 18");
  main.addItem("Item 19");

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
  main.run(true);
}

void loop() {
  ez.update();
}
