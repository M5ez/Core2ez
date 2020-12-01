#ifndef _EZLIST_H_
#define _EZLIST_H_

#include <ezWidget.h>
#include <ezLabel.h>
// #include <ez.h>

class ezListBox : public ezWidget {
 public:

  ezListBox(ezWidget& parentWidget,
           int16_t x_ = EZ_INVALID, int16_t y_ = EZ_INVALID,
           int16_t w_ = 0, int16_t h_ = 0,
           WidgetColors colors_ = THEME_COLORS,
           WidgetColors itemSelected_ = THEME_COLORS,
           ezFont font_ = THEME_FONT, uint8_t spacing_ = 0);

  ezListBox(int16_t x_ = EZ_INVALID, int16_t y_ = EZ_INVALID,
           int16_t w_ = 0, int16_t h_ = 0,
           WidgetColors colors_ = THEME_COLORS,
           WidgetColors itemSelected_ = THEME_COLORS,
           ezFont font_ = THEME_FONT, uint8_t spacing_ = 0);

  void init(ezWidget* pwPtr,
            int16_t x_, int16_t y_, int16_t w_, int16_t h_,
            WidgetColors colors_, WidgetColors itemSelected_,
            ezFont font_, uint8_t spacing_);

  ezFont          font;
  bool            multi         = false;
  uint8_t         spacing       = 0;
  WidgetColors    itemNormal;
  WidgetColors    itemSelected;

  friend class ezListItem;


};


class ezListItem : public ezLabel {
 public:
  ezListItem();
  ezListItem(ezListBox& parentListBox, String text_);
  void init(ezListBox* prnt, String text_);
  operator bool();
  virtual void eventPost();
  virtual void draw();
  bool selected = false;
};


#endif /* _EZLIST_H_ */
