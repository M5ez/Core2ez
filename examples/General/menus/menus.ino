/*
 * Rough cut: there's no header and hence no visbile menu title and things could be prettier. Soon...
 */

#include <M5Core2.h>
#include <Core2ez.h>


void setup() {
  ez.begin();
  ezMenu main("Main menu");
  main.addItem("Item 1");
  main.addItem("Item 2");
  main.addItem("Item 3");
  main.addItem("subMenu", doFunction {
    ezMenu sub("Submenu");
    sub.addItem("SubItem 1", doFunction {
      Serial.println("SubItem 1");
    });
    sub.addItem("SubItem 2");
    sub.addItem("SubItem 3");
    sub.addItem("SubItem 4");
    sub.addItem("back to main", doFunction {
      exitMenu(); 
    });
    sub.run();
  });
  main.run();
}

void loop() {
  ez.update();
}
