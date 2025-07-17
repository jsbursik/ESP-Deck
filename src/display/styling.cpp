#include <lvgl.h>

lv_style_t grid_style;
lv_style_t btn_style;

void grid_style_init()
{
  lv_style_init(&grid_style);
  lv_style_set_bg_color(&grid_style, lv_color_hex(0x292929));
  lv_style_set_radius(&grid_style, 0);
  lv_style_set_border_width(&grid_style, 0);
}

void btn_style_init()
{
  lv_style_init(&btn_style);
  lv_style_set_bg_color(&btn_style, lv_color_hex(0x4d4d4d));
  lv_style_set_radius(&btn_style, 10);
  lv_style_set_border_width(&btn_style, 0);
  lv_style_set_shadow_color(&btn_style, lv_color_hex(0x000000));
  lv_style_set_shadow_width(&btn_style, 2);
}