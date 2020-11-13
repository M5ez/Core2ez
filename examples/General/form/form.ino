#include <M5Core2.h>
#include <Core2ez.h>

// I used http://www.barth-dev.de/online/rgb565-color-picker/ to find colors
#define LIGHTYELLOW     0xFFFB
#define PINK            0xFE79


ezLabel   nameLbl     (  10,  0,   90,  25, "Name");
ezInput   name        (  10,  25, 300,  35, "", "Enter name:", THEME_COLORS, FSSB12);
ezLabel   addressLbl  (  10,  60,  90,  25, "Address");
ezInput   address     (  10,  85, 300,  35, "", "Enter address:", THEME_COLORS, FSSB12);
ezLabel   ZipLbl      (  10, 120,  90,  25, "ZIP");
ezInput   zip         (  10, 145,  90,  35, "", "Enter ZIP:", THEME_COLORS, FSSB12);
ezLabel   CityLbl     ( 110, 120, 200,  25, "City");
ezInput   city        ( 110, 145, 200,  35, "", "Enter city:", THEME_COLORS, FSSB12);
ezButton  reset       ( 10 , 200, 100,  30, "reset",  {RED,   WHITE, BLACK});
ezButton  submit      ( 210, 200, 100,  30, "submit", {GREEN, BLACK, BLACK});

ezWindow  thankyou;
ezLabel   thankyouLbl (thankyou, 0, 0, 320, 240, "Thank you!", THEME_COLORS, FSSB24, EZ_CENTER, EZ_CENTER);


void setup() {
  ez.begin();
  ez.Screen.colors.fill = LIGHTYELLOW;
  reset.   addHandler(resetHandler ,   E_TAP + E_PRESSED);
  submit.  addHandler(submitHandler,   E_TAP + E_PRESSED);
  thankyou.addHandler(thankyouHandler, E_TAP + E_PRESSED);
  ez.Screen.addHandler(changeHandler, E_CHANGED, true);
}

void loop() {
  ez.update();
}

void resetHandler() {
  name.text = address.text = zip.text = city.text = "";
  name.colors.fill = address.colors.fill = zip.colors.fill = city.colors.fill = WHITE;
  ez.draw();
}

void submitHandler() {
  bool incomplete = false;
  name.colors.fill = address.colors.fill = zip.colors.fill = city.colors.fill = WHITE;
  if (name.text    == "") { name   .colors.fill = PINK; incomplete = true; }
  if (address.text == "") { address.colors.fill = PINK; incomplete = true; }
  if (zip.text     == "") { zip    .colors.fill = PINK; incomplete = true; }
  if (city.text    == "") { city   .colors.fill = PINK; incomplete = true; }
  if (incomplete) {
    ez.draw();
    return;
  }
  
  Serial.println("\nYou submitted:\n");
  Serial.println(name.text);
  Serial.println(address.text);
  Serial.print(zip.text); Serial.print("  ");Serial.println(city.text);
  
  name.text = address.text = zip.text = city.text = "";
  name.colors.fill = address.colors.fill = zip.colors.fill = city.colors.fill = WHITE;
  thankyou.focus();
}

void thankyouHandler() {
  thankyou.blur();
}

void changeHandler() {
  EVENTWIDGET(ezInput, i);
  if (i.text != "") {
    i.colors.fill = WHITE;
    i.draw();
  }
}
