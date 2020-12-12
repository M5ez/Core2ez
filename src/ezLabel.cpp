#include "ezLabel.h"
#include <ez.h>

ezLabel::ezLabel(ezWidget& parentWidget,
                 int16_t x_ /* = EZ_INVALID */, int16_t y_ /* = EZ_INVALID */,
                 int16_t w_ /* = 0 */, int16_t h_ /* = 0 */,
                 String text_ /* = "" */,
                 WidgetColors colors_ /* = THEME_COLORS */,
                 ezFont font_ /* = THEME_FONT */,
                 int16_t align_ /* = EZ_LEFT */,
                 int16_t valign_ /* = EZ_CENTER */,
                 int16_t dx_ /* = 0 */, int16_t dy_ /* = 0 */) {
  init(&parentWidget, x_, y_, w_, h_, text_, colors_, font_, align_, valign_,
       dx_, dy_);
}

ezLabel::ezLabel(int16_t x_ /* = EZ_INVALID */, int16_t y_ /* = EZ_INVALID */,
                 int16_t w_ /* = 0 */, int16_t h_ /* = 0 */,
                 String text_ /* = "" */,
                 WidgetColors colors_ /* = THEME_COLORS */,
                 ezFont font_ /* = THEME_FONT */,
                 int16_t align_ /* = EZ_LEFT */,
                 int16_t valign_ /* = EZ_CENTER */,
                 int16_t dx_ /* = 0 */, int16_t dy_ /* = 0 */) {
  init(nullptr, x_, y_, w_, h_, text_, colors_, font_, align_, valign_,
       dx_, dy_);
}

void ezLabel::init(ezWidget* pwPtr,
                   int16_t x_, int16_t y_, int16_t w_, int16_t h_,
                   String text_, WidgetColors colors_, ezFont font_,
                   int16_t align_, int16_t valign_,
                   int16_t dx_, int16_t dy_) {
  type    = W_LABEL;
  set(x_, y_, w_, h_);
  lmargin = rmargin = tmargin = bmargin = 2;
  text    = text_;
  colors  = ezTheme.colors(colors_, ezTheme.lbl_colors);
  font    = font_ ? font_ : ezTheme.lbl_font;
  align   = align_;
  valign  = valign_;
  dx      = dx_;
  dy      = dy_;
  numb    = true;
  if (pwPtr) pwPtr->add(*this); else ezScreen.add(*this);
}

void ezLabel::draw() {

  if (colors.fill    != NODRAW) clear();

  if (colors.outline != NODRAW) drawRect(colors.outline);

  if (colors.text    != NODRAW && text != "") {

    // Figure out where the text goes
    uint8_t datum;
    if        (valign == EZ_TOP   ) {
      if      (align  == EZ_LEFT  ) datum = TL_DATUM;
      else if (align  == EZ_CENTER) datum = TC_DATUM;
      else                          datum = TR_DATUM;
    } else if (valign == EZ_CENTER) {
      if      (align  == EZ_LEFT  ) datum = CL_DATUM;
      else if (align  == EZ_CENTER) datum = CC_DATUM;
      else                          datum = CR_DATUM;
    } else {
      if      (align  == EZ_LEFT  ) datum = BL_DATUM;
      else if (align  == EZ_CENTER) datum = BC_DATUM;
      else                          datum = BR_DATUM;
    }
    uint16_t tx;
    if        (align == EZ_LEFT   )  tx = lmargin;
    else if   (align == EZ_CENTER )  tx = w / 2;
    else                             tx = w - rmargin;
    uint16_t ty;


    // Actual drawing of text
    setTextColor(colors.text);
    setFont(font);
    setTextDatum(datum);
    setTextPadding(0);

    if (!wrap) {
      if        (valign == EZ_TOP   )  ty = tmargin;
      else if   (valign == EZ_CENTER)  ty = h / 2;
      else                             ty = h - bmargin;

      if (!sprite && textWidth(text) > w - lmargin) {
        spriteBuffer();
        setTextColor(colors.text);
        setFont(font);
        setTextDatum(datum);
        setTextPadding(0);
        drawString(text, tx + dx, ty + dy);
        push();
        direct();
      } else {
        drawString(text, tx + dx, ty + dy);
     }

    } else {
      std::vector<line_t> lines;
      if (snug) {
        _fitLines(text, w - (lmargin + rmargin),
                  (w / 2) - (lmargin + rmargin), lines);
      } else {
        _wrapLines(text, w - (lmargin + rmargin), lines);
      }
      uint16_t interLine = fontHeight() * lineSpacing;
      uint8_t numLines = min((h - (tmargin + bmargin)) / interLine,
                             lines.size());
      for (uint8_t n = 0; n < numLines; n++) {
        if      (valign == EZ_TOP   ) ty = tmargin + (n * interLine);
        else if (valign == EZ_BOTTOM) ty = h - bmargin - (n * interLine);
        else ty = (h / 2) + (n * interLine) -
                  (((numLines - 1) * interLine) / 2);
        drawString(lines[n].line, tx + dx, ty + dy);
      }
    }
  }

  // Boilerplate draw() stuff
  drawChildren();
  push();
}

void ezLabel::_fitLines(String text, uint16_t max_width, uint16_t min_width,
                        std::vector<line_t>& lines) {
  uint8_t  prev_num_lines = 100;
  for (int16_t n = max_width; n > min_width; n -= 10) {
    _wrapLines(text, n, lines);
    if (lines.size() > prev_num_lines) {
      _wrapLines(text, n + 10, lines);
      return;
    }
    prev_num_lines = lines.size();
  }
}

void ezLabel::_wrapLines(String text, uint16_t width,
                         std::vector<line_t>& lines) {
  lines.clear();
  int16_t offset = 0;
  int16_t last_space = 0;
  int16_t cur_space = 0;
  int16_t newline = 0;
  bool all_done = false;
  line_t new_line;

  //If there are no return chars, it's either a single line,
  //Or it's using linux/mac line endings which are a single char
  char nlchar = 13;
  if (text.indexOf(13)==-1) nlchar = 10;

  while (!all_done) {
    cur_space = text.indexOf(" ", last_space + 1);
    if (cur_space == -1) {
      cur_space = text.length();
      all_done = true;
    }
    newline = text.indexOf(char(nlchar), last_space + 1);
    if (newline != -1 && newline < cur_space) cur_space = newline;
    if (textWidth(text.substring(offset, cur_space)) > width ||
        text.substring(last_space, last_space + 1) == (String)char(nlchar)) {
      if (textWidth(text.substring(offset, last_space)) <= width) {
        new_line.position = offset;
        new_line.line = text.substring(offset, last_space);
        lines.push_back(new_line);
        offset = last_space + 1;
        last_space = cur_space;
      } else {
        for (uint16_t n = offset; n < text.length(); n++) {
          if (textWidth(text.substring(offset, n + 1)) > width) {
            new_line.position = offset;
            new_line.line = text.substring(offset, n);
            lines.push_back(new_line);
            offset = n;
            break;
          }
        }
      }
    } else {
      last_space = cur_space;
    }

    //Special case handle the last line
    if (all_done && offset < text.length()) {
      while(text.indexOf(char(nlchar), offset) > offset) {
        if(offset < text.length()) {
          new_line.line = text.substring(offset, text.indexOf(char(nlchar),
                                         offset));
          new_line.position = offset;
          offset = text.indexOf(char(nlchar), offset);
          lines.push_back(new_line);
        } else {
          break;
        }
      }
    }

    if (all_done && offset < text.length()) {
      new_line.position = offset;
      new_line.line = text.substring(offset);
      lines.push_back(new_line);
    }
  }
}
