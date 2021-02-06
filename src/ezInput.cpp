#include "ezInput.h"


ezInput::ezInput(ezWidget& parentWidget,
                 int16_t x_ /* = EZ_INVALID */, int16_t y_ /* = EZ_INVALID */,
                 int16_t w_ /* = 0 */, int16_t h_ /* = 0 */,
                 String text_ /* = "" */,
                 String prompt_ /* = "" */,
                 WidgetColors colors_ /* = THEME_COLORS */,
                 ezFont font_ /* = THEME_FONT */,
                 int16_t align_ /* = EZ_LEFT */,
                 int16_t valign_ /* = EZ_CENTER */,
                 int16_t dx_ /* = 0 */, int16_t dy_ /* = 0 */) {
  init(&parentWidget, x_, y_, w_, h_, text_, prompt_, colors_, font_,
       align_, valign_, dx_, dy_);
}

ezInput::ezInput(int16_t x_ /* = EZ_INVALID */, int16_t y_ /* = EZ_INVALID */,
                 int16_t w_ /* = 0 */, int16_t h_ /* = 0 */,
                 String text_ /* = "" */,
                 String prompt_ /* = "" */,
                 WidgetColors colors_ /* = THEME_COLORS */,
                 ezFont font_ /* = THEME_FONT */,
                 int16_t align_ /* = EZ_LEFT */,
                 int16_t valign_ /* = EZ_CENTER */,
                 int16_t dx_ /* = 0 */, int16_t dy_ /* = 0 */) {
  init(nullptr, x_, y_, w_, h_, text_, prompt_, colors_, font_,
       align_, valign_, dx_, dy_);
}

void ezInput::init(ezWidget* pwPtr,
                   int16_t x_, int16_t y_, int16_t w_, int16_t h_,
                   String text_,String prompt_, WidgetColors colors_,
                   ezFont font_, int16_t align_, int16_t valign_,
                   int16_t dx_, int16_t dy_) {
  type    = W_INPUT;
  set(x_, y_, w_, h_);
  text    = text_;
  prompt  = prompt_;
  colors  = ezTheme.colors(colors_, ezTheme.inp_colors);
  font    = font_ ? font_ : ezTheme.inp_font;
  align   = align_;
  valign  = valign_;
  dx      = dx_;
  dy      = dy_;
  numb    = false;
  if (pwPtr) pwPtr->add(*this); else ezScreen.add(*this);
}

void ezInput::eventPost() {
  if (ez.e.widget == this && ez.e == E_TAPPED) {
    String oldText = text;
    text = keyboardInput(prompt, text);
    if (oldText != text) {
      draw();
      refresh();
      _changed = true;
    }
  }
  if (!ez.e && _changed) {
    ez.e.type = E_CHANGED;
    ez.e.widget = this;
    fireEvent();
    _changed = false;
  }
}


namespace {

  const char*    kbKeys[NUM_LAYOUTS][NUM_ROWS][MAX_PER_ROW] = {
    {
      { "q", "w", "e", "r", "t", "y", "u", "i", "o", "p" },
        { "a", "s", "d", "f", "g", "h", "j", "k", "l" },
          { "z", "x", "c", "v", "b", "n", "m" }
    }, {
      { "1", "2", "3", "4", "5", "6", "7", "8", "9", "0" },
      { "-", "/", ":", ";", "(", ")", "$", "&", "@", "\"" },
                  { ".", ",", "?", "!", "'" }
    }, {
      { "[", "]", "{", "}", "#", "%", "^", "*", "+" },
      { "_", "\\", "|", "~", "<", ">", "`", " ", "=" },
                { ".", ",", "?", "!", "'" }
    }
  };

  // starting x, total width and number of keys for each row.
  const int16_t kbRows[NUM_LAYOUTS][NUM_ROWS][3] = {
    {
      {  0, 320, 10 },
      { 16, 288,  9 },
      { 40, 224,  7 }
    }, {
      {  0, 320, 10 },
      {  0, 320, 10 },
      { 42, 224,  5 }
    }, {
      {  0, 320,  9 },
      {  0, 320,  9 },
      { 42, 224,  5 }
    }
  };

  struct kbGlobals {
    ezWindow  window      = ezWindow  (  0,  0, 320, 240, ezTheme.kb);
    ezLabel   topBar      = ezLabel   (  0,  0, 320, 20, "",
                                       ezTheme.kb_topBar, FSS9,
                                       EZ_CENTER, EZ_CENTER);
    ezGesture help        = ezGesture (120, EZ_DOWN);
    ezWindow  input       = ezWindow  (  0, 20, 320,  55, ezTheme.kb);
    ezWindow  keys        = ezWindow  (  0, 75, 320, 165, ezTheme.kb);
    ezGesture backspace   = ezGesture ( 40, EZ_LEFT);
    ezGesture space       = ezGesture ( 40, EZ_RIGHT);
    ezButton  sym         = ezButton  (  0, 110,  40, 55, "sym",
                                       ezTheme.kb_special);
    ezButton  done        = ezButton  (264, 110,  56, 55, "done",
                                       ezTheme.kb_special);
    ezButton  k[MAX_KEYS];
    String    text        = "";
    String    prompt      = "";
    uint8_t   current     = DEFAULT_KB;
    int16_t   caret       = 0;
    bool      doneFlag    = false;
  };
  kbGlobals* kb = nullptr;


  void drawKeyboard(uint8_t kbnum) {

    // Prevent flicker when keys are redrawn
    kb->keys.spriteBuffer();

    // Hide all keys
    for (int8_t n = 0; n < MAX_KEYS; n++) kb->k[n].set();

    // Set and show those that need to be shown
    uint8_t k_idx = 0;
    for (uint8_t n = 0; n < NUM_ROWS; n++) {
      uint16_t initial_x  = kbRows[kbnum][n][0];
      uint16_t width      = kbRows[kbnum][n][1];
      uint8_t  numKeys    = kbRows[kbnum][n][2];
      uint16_t kw         = width / numKeys;
      uint16_t kh         = kb->keys.h / NUM_ROWS;
      for (uint8_t m = 0; m < numKeys; m++) {
        kb->k[k_idx].x          = initial_x + (m * kw);
        kb->k[k_idx].y          = n * kh;
        kb->k[k_idx].w          = kw;
        kb->k[k_idx].h          = kh;
        kb->k[k_idx].label.text = kbKeys[kbnum][n][m];
        k_idx++;
      }
    }
    kb->keys.draw();
    kb->keys.push();
    kb->keys.direct();
    kb->current = kbnum;
  }

  void updateInput(bool displayCaret = true) {
    ezPoint oldOffset = kb->input.offset;
    if (kb->caret > kb->text.length()) kb->caret = kb->text.length();
    uint16_t inputWidth = (CHAR_WIDTH * kb->text.length()) + CARET_WIDTH;
    if (inputWidth < kb->input.w) inputWidth = kb->input.w;
    kb->input.spriteBuffer(inputWidth, kb->input.h);
    kb->input.clear();
    kb->input.setFreeFont(FMB12);
    kb->input.setTextColor(TEXT_COLOR);
    kb->input.drawString(kb->text, 0, TEXT_VOFFSET);
    if (displayCaret) {
      kb->input.fillRect(max((CHAR_WIDTH * kb->caret) - (CARET_WIDTH / 2), 0),
                         CARET_VOFFSET, CARET_WIDTH, CARET_HEIGHT, CARET_COLOR);
    }
    kb->input.offset = oldOffset;
    kb->input.push();
  }

  void fixOffset() {
    int16_t stringWidth = kb->text.length() * CHAR_WIDTH;
    int16_t maxOffset = stringWidth - kb->input.w + CARET_WIDTH;
    if (kb->input.offset.x > maxOffset) kb->input.offset.x = maxOffset;
    if (kb->input.offset.x < 0) kb->input.offset.x = 0;
  }

  void insertStr(String str) {
    if (kb->text.length() >= MAX_INPUT) return;
    kb->text = kb->text.substring(0, kb->caret) + str + kb->text.substring(kb->caret);
    kb->caret++;
    fixOffset();
    if ((kb->caret * CHAR_WIDTH) + (CARET_WIDTH / 2) - kb->input.offset.x > kb->input.w) {
      kb->input.offset.x = (kb->caret * CHAR_WIDTH) - kb->input.w + CARET_WIDTH;
    }
    updateInput();
  }

  void keyHandler() {
    if (!ez.e.widget || !ez.e.widget->userData) return;
    ezButton& b = *static_cast<ezButton*>(ez.e.widget);
    String t = b.label.text;
    if (ez.e == E_DRAGGED) {
      if (ez.e.isDirection(EZ_UP)) {
        t.toUpperCase();
      } else {
        return;
      }
    }
    insertStr(t);
  }

  void spaceHandler() {
    insertStr(" ");
    if (kb->current != DEFAULT_KB) drawKeyboard(DEFAULT_KB);
  }

  void backspaceHandler() {
    if (kb->caret) {
      kb->text = kb->text.substring(0, kb->caret - 1) + kb->text.substring(kb->caret);
      kb->caret--;
      kb->input.offset.x -= CHAR_WIDTH;
      fixOffset();
      updateInput();
    }
  }

  void symHandler() {
    if      (kb->current == DEFAULT_KB) drawKeyboard(SYMBOL_KB);
    else if (kb->current == SYMBOL_KB ) drawKeyboard(SHIFT_SYMBOL_KB);
    else    drawKeyboard(DEFAULT_KB);
  }

  void doneHandler() {
    kb->doneFlag = true;
  }

  void inputHandler() {
    if (ez.e == E_TAPPED) {
      kb->caret = ez.e.to.x / CHAR_WIDTH;
      updateInput();
    }
  //   if (e == E_MOVE) {
  //     int16_t stringWidth = kb->text.length() * CHAR_WIDTH;
  //     if (e.to.y < 160 && stringWidth > 320) {
  //       int16_t oldOffset = kb->input.offset.x;
  //       int16_t moveBy = moveStart - e.to.x;
  //       kb->input.offset.x = moveOffset + moveBy;
  //       fixOffset();
  //       if (kb->input.offset.x != oldOffset) kb->input.pushSprite(-kb->input.offset.x, 30);
  //     } else {
  //       kb->caret = ((kb->input.offset.x + e.to.x) / CHAR_WIDTH);
  //       updateInput();
  //     }
  //   }
  }


  void showHelpBar() {
    kb->topBar.text = TOPBAR_TEXT;
    kb->topBar.align = EZ_CENTER;
    kb->topBar.draw();
  }

  void showPrompt() {
    kb->topBar.text = kb->prompt;
    kb->topBar.align = EZ_LEFT;
    kb->topBar.draw();
  }

  void helpScreen() {
    ezWindow  helpW;
    helpW.colors = ezTheme.kb;

    ezLabel help_1   = ezLabel   (helpW,  10,  10, 300,  40, HELP_1,
                                  ezTheme.kb, FSSB18);
    ezLabel help_2_1 = ezLabel   (helpW,  10,  75, 110,  20, HELP_2_1,
                                  ezTheme.kb, FSS9);
    ezLabel help_2_2 = ezLabel   (helpW, 120,  75, 200,  20, HELP_2_2,
                                  ezTheme.kb, FSS9);
    ezLabel help_3_1 = ezLabel   (helpW,  10, 105, 110,  20, HELP_3_1,
                                  ezTheme.kb, FSS9);
    ezLabel help_3_2 = ezLabel   (helpW, 120, 105, 200,  20, HELP_3_2,
                                  ezTheme.kb, FSS9);
    ezLabel help_4_1 = ezLabel   (helpW,  10, 135, 110,  20, HELP_4_1,
                                  ezTheme.kb, FSS9);
    ezLabel help_4_2 = ezLabel   (helpW, 120, 135, 200,  20, HELP_4_2,
                                  ezTheme.kb, FSS9);
    helpW.on(E_TOUCH, doFunction { ezQuit(); });
    helpW.run();
    ez.draw();
  }

}

String keyboardInput(String prompt_ /* = "" */, String text_ /* = "" */) {

  kb = new kbGlobals;

  kb->prompt = prompt_;
  kb->text = text_;

  kb->topBar.dy = -2;
  kb->topBar.dx = 5;
  kb->window.add(kb->topBar);
  kb->topBar.add(kb->help);
  kb->help.addHandler(helpScreen);

  kb->window.add(kb->input);
  kb->input.addHandler(inputHandler);

  kb->window.add(kb->keys);
  kb->keys.colors = ezTheme.kb;
  for (int8_t n = 0; n < MAX_KEYS; n++) {
    kb->k[n].label.font   = ezTheme.kb_keyFont;
    kb->k[n].colors       = ezTheme.kb_keys;
    kb->k[n].onColors     = ezTheme.kb_keysOn;
    kb->k[n].dbltapTime   = 0;
    kb->k[n].userData     = 1;
    kb->keys.add(kb->k[n]);
  }
  ez.addHandler(keyHandler, E_TAPPED | E_PRESSED | E_DRAGGED);
  kb->sym.dbltapTime  = 0;
  kb->sym.label.font  = FSS9;
  kb->keys.add(kb->sym);
  kb->sym.addHandler(symHandler, E_TAPPED | E_PRESSED);
  kb->done.dbltapTime = 0;
  kb->done.label.font = FSS9;
  kb->keys.add(kb->done);
  kb->done.addHandler(doneHandler, E_TAPPED | E_PRESSED);
  kb->keys.add(kb->space);
  kb->space.addHandler(spaceHandler);
  kb->keys.add(kb->backspace);
  kb->backspace.addHandler(backspaceHandler);

  kb->caret = kb->text.length();

  kb->window.focus();
  showPrompt();
  updateInput();
  drawKeyboard(0);

  uint32_t startTime = millis();
  uint16_t ticks = 0;
  while (!kb->doneFlag) {
    // One tick every 10 ms, so 1000 ticks is 10 seconds.
    uint16_t newticks = ((millis() - startTime) / 10) % 1000;
    if (newticks != ticks) {
      ticks = newticks;
      // Blinks cursor every second
      if (ticks % 50 == 0) updateInput((ticks % 100));
      // Toggles between prompt and help text in top bar
      if (ticks ==   0) showPrompt();
      if (ticks == 500) showHelpBar();
    }
    loop();
  }

  ez.delHandlers(keyHandler);
  kb->window.blur();
  String r = kb->text;

  ez.draw();

  delete kb;

  return r;
}
