#include "ezList.h"

#include <ezWindow.h>
#include <ezTheme.h>

ezListBox::ezListBox(ezWidget& parentWidget,
                     int16_t x_ /* = EZ_INVALID */,
                     int16_t y_ /* = EZ_INVALID */,
                     int16_t w_ /* = 0 */, int16_t h_ /* = 0 */,
                     WidgetColors colors_ /* = THEME_COLORS */,
                     WidgetColors itemSelected_ /* = THEME_COLORS */,
                     ezFont font_ /* = THEME_FONT */,
                     uint8_t spacing_ /* = 0 */) {
  init(&parentWidget, x_, y_, w_, h_, colors_, itemSelected_, font_, spacing_);
}

ezListBox::ezListBox(int16_t x_ /* = EZ_INVALID */,
                     int16_t y_ /* = EZ_INVALID */,
                     int16_t w_ /* = 0 */, int16_t h_ /* = 0 */,
                     WidgetColors colors_ /* = THEME_COLORS */,
                     WidgetColors itemSelected_ /* = THEME_COLORS */,
                     ezFont font_ /* = THEME_FONT */,
                     uint8_t spacing_ /* = 0 */) {
  init(nullptr, x_, y_, w_, h_, colors_, itemSelected_, font_, spacing_);
}

void ezListBox::init(ezWidget* pwPtr,
                     int16_t x_, int16_t y_, int16_t w_, int16_t h_,
                     WidgetColors colors_, WidgetColors itemSelected_,
                     ezFont font_, uint8_t spacing_) {
  type                = W_LISTBOX;
  set(x_, y_, w_, h_);
  colors              = ezTheme.colors(colors_, ezTheme.lst_colors);
  itemNormal          = colors;
  itemNormal.outline  = NODRAW;
  itemSelected        = ezTheme.colors(itemSelected_, ezTheme.lsl_colors);
  font                = font_ ? font_ : ezTheme.lst_font;
  spacing             = spacing_;
  autoSize            = true;
  if (pwPtr) pwPtr->add(*this); else ezScreen.add(*this);
}




ezListItem::ezListItem() { }

ezListItem::ezListItem(ezListBox& parentListBox, String text_) {
  init(&parentListBox, text_);
}

void ezListItem::init(ezListBox* prnt, String text_) {
  type = W_LISTITEM;
  font = prnt->font;
  colors = prnt->itemNormal;
  align = EZ_LEFT;
  valign = EZ_CENTER;
  set(0, EZ_AUTO, EZ_PARENT, font.height() + prnt->spacing);
  text = text_;
  numb = false;
  prnt->add(*this);
}

ezListItem::operator bool() { return selected; }

void ezListItem::eventPost() {
  if (ez.e.widget == this && ez.e == E_TAPPED) {
    selected = !selected;
    draw();
    refresh();
  }
}

void ezListItem::draw() {
  if (parent() && parent()->type == W_LISTBOX) {
    ezListBox& p = *static_cast<ezListBox*>(parent());
    if (!p.multi && selected) {
      for (auto item : p._widgets) {
        if (item->type != W_LISTITEM) continue;
        ezListItem& i = *static_cast<ezListItem*>(item);
        if (i.selected && &i != this) {
          i.selected = false;
          i.draw();
          i.refresh();
        }
      }
    }
    colors = selected ? p.itemSelected : p.itemNormal;
  }
  ezLabel::draw();
}
