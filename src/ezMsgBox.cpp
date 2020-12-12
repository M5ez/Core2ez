#include "ezMsgBox.h"
#include "ez.h"

String ezMsgBox::retval = "";

ezMsgBox::ezMsgBox(String title_, String message_ /* = "" */,
                   String b1 /* = "" */, String b2 /* = "" */,
                   String b3 /* = "" */) {
  type              = W_MSGBOX;
  title             = title_;
  gutter            = 10;
  add(ezHeader);
  add(label);
  add(buttonbox);
  buttonbox.lmargin = -1;
  buttonbox.rmargin = -1;
  buttonbox.tmargin = 5;
  buttonbox.gutter  = -1;
  label.colors      = ezTheme.msg_colors;
  label.font        = ezTheme.msg_font;
  label.align       = EZ_CENTER;
  label.valign      = EZ_CENTER;
  label.wrap        = true;
  label.snug        = true;
  retval            = "";

  if (message_ != "") label.text = message_;
  if (b1       != "") addButton(b1);
  if (b2       != "") addButton(b2);
  if (b3       != "") addButton(b3);
}

ezMsgBox::~ezMsgBox() {
  for (auto button: buttons) delete button;
}

void ezMsgBox::message(String text_, ezFont font_ /* = THEME_FONT */,
                       int16_t align_ /* = EZ_CENTER */,
                       int16_t valign_ /* = EZ_CENTER */) {
  if (font_ != THEME_FONT) label.font = font_;
  label.text    = text_;
  label.align   = align_;
  label.valign  = valign_;
  label.draw();
  label.refresh();
}

void ezMsgBox::addButton(String text_, void (*func)() /* = nullptr */) {
  buttons.push_back(new ezButton(buttonbox, EZ_AUTO, 0, -3, EZ_PARENT, text_));
  if (func) buttons.back()->addHandler(func, E_TAPPED + E_PRESSED);
}

String ezMsgBox::pressed() {
  return ezMsgBox::retval;
}

bool ezMsgBox::somethingPressed() {
  return (ezMsgBox::retval != "");
}

String ezMsgBox::run() {
  onOffspring(E_TAPPED + E_PRESSED, doFunction {
    if (eventWidget(ezButton, b)) {
      ezMsgBox::retval = b->label.text;
    }
  });

  focus();

  while (ezMsgBox::retval == "") {
    loop();
  }
  return ezMsgBox::retval;
}


String msgBox(String title_, String message_, String b1 /* = "OK" */,
              String b2 /* = "" */, String b3 /* = "" */) {
  ezMsgBox mb(title_, message_, b1, b2, b3);
  return mb.run();
}

