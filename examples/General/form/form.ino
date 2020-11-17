#include <M5Core2.h>
#include <Core2ez.h>

// I used http://www.barth-dev.de/online/rgb565-color-picker/ to find colors
#define LIGHTYELLOW     0xFFFB
#define PINK            0xFE79


ezLabel       headerLbl   (  10,   0, 300,  50, "Demo Form", THEME_COLORS, FSSB24, EZ_CENTER);
ezRadiobutton ms          ( 150,  80,  90,  25, "Ms");
ezRadiobutton mr          ( 240,  80,  80,  25, "Mr");
ezLabel       nameLbl     (  10, 100,  90,  25, "Name");
ezInput       name        (  10, 125, 300,  45, "", "Enter name:", THEME_COLORS, FSSB12);
ezLabel       addressLbl  (  10, 170,  90,  25, "Address");
ezInput       address     (  10, 195, 300,  45, "", "Enter address:", THEME_COLORS, FSSB12);
ezLabel       zipLbl      (  10, 240,  90,  25, "ZIP");
ezInput       zip         (  10, 265,  90,  45, "", "Enter ZIP:", THEME_COLORS, FSSB12);
ezLabel       cityLbl     ( 110, 240, 200,  25, "City");
ezInput       city        ( 110, 265, 200,  45, "", "Enter city:", THEME_COLORS, FSSB12);
ezCheckbox    terms       (  10, 340, 300,  25, "I read the terms & conditions");
ezCheckbox    newsletter  (  10, 390, 300,  25, "Send me the newsletter");
ezButton      reset       ( 10 , 440, 100,  45, "reset",  {RED, WHITE, BLACK});
ezButton      submit      ( 210, 440, 100,  45, "submit");

ezWindow  thankyou;
ezLabel   thankyouLbl (thankyou, 0, 0, 320, 250, "Thank you!", THEME_COLORS, FSSB24, EZ_CENTER, EZ_CENTER);


void setup() {
  ez.begin();
  ez.Screen.spriteBuffer(320, 500);
  ez.Screen.colors.fill = LIGHTYELLOW;
  newsletter = true;
  ms = true;
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
  terms = false;
  newsletter = true;
  ez.Screen.draw();
  ez.Screen.push();
}

void submitHandler() {
  bool incomplete = false;
  name.colors.fill = address.colors.fill = zip.colors.fill = city.colors.fill = terms.colors.fill = WHITE;
  if (name.text    == "") { name   .colors.fill = PINK; incomplete = true; }
  if (address.text == "") { address.colors.fill = PINK; incomplete = true; }
  if (zip.text     == "") { zip    .colors.fill = PINK; incomplete = true; }
  if (city.text    == "") { city   .colors.fill = PINK; incomplete = true; }
  if (!terms)             { terms  .colors.fill = PINK; incomplete = true; }
  if (incomplete) {
    ez.Screen.draw();
    ez.Screen.push();
    return;
  }

  Serial.println("\nYou submitted:\n");
  Serial.print((ms) ? "Ms " : "Mr. ");
  Serial.println(name.text);
  Serial.println(address.text);
  Serial.print(zip.text); Serial.print("  ");Serial.println(city.text);
  if (newsletter) Serial.println("(wants newsletter)");

  name.text = address.text = zip.text = city.text = "";
  name.colors.fill = address.colors.fill = zip.colors.fill = city.colors.fill = WHITE;
  newsletter = true;
  terms = false;
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
    i.refresh();
  }
}
