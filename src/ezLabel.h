#ifndef _EZLABEL_H_
#define _EZLABEL_H_

#include <ezWidget.h>
#include <ezValues.h>
#include <ezTheme.h>

struct line_t {
  int16_t position;
  String line;
};

class ezLabel : public ezWidget {
 public:
  ezLabel(ezWidget& parentWidget,
          int16_t x_ = EZ_INVALID, int16_t y_ = EZ_INVALID,
          int16_t w_ = 0, int16_t h_ = 0,
          String text_         = "",
          WidgetColors colors_ = THEME_COLORS,
          ezFont font_ = THEME_FONT,
          int16_t align_ = EZ_LEFT,
          int16_t valign_ = EZ_CENTER,
          int16_t dx_ = 0, int16_t dy_ = 0);

  ezLabel(int16_t x_ = EZ_INVALID, int16_t y_ = EZ_INVALID,
          int16_t w_ = 0, int16_t h_ = 0,
          String text_         = "",
          WidgetColors colors_ = THEME_COLORS,
          ezFont font_ = THEME_FONT,
          int16_t align_ = EZ_LEFT,
          int16_t valign_ = EZ_CENTER,
          int16_t dx_ = 0, int16_t dy_ = 0);

  void init(ezWidget* pwPtr,
            int16_t x_, int16_t y_, int16_t w_, int16_t h_, String text_,
            WidgetColors colors_, ezFont font_, int16_t align_,
            int16_t valign_, int16_t dx_, int16_t dy_);

  virtual void    draw();
  String          text        = "";
  ezFont          font        = FSS9;
  int16_t         align       = EZ_LEFT;
  int16_t         valign      = EZ_TOP;
  int16_t         dx          = 0;
  int16_t         dy          = 0;
  bool            wrap        = false;
  bool            snug        = false;
  float           lineSpacing = 1.0;

 protected:
  void _fitLines(String text, uint16_t max_width, uint16_t min_width,
                 std::vector<line_t>& lines);
  void _wrapLines(String text, uint16_t width, std::vector<line_t>& lines);
};


#endif /* _EZLABEL_H_ */
