#include <lvgl.h>
#include "config.h"

// Setup LoyvanGFX for the Elecrow Display
#include "LGFX.h"
#include "lcd_shared.h"

// Setup LVGL
#include "lvgl_setup.h"
#include "touch.h"

// Styling baby
#include "styling.h"

////////////////////
// Setup and Loop //
////////////////////

void setup()
{
  Serial.begin(9600);

  // LCD init, Backlight on
  lcd.begin();
  ledcSetup(1, 300, 8);
  ledcAttachPin(TFT_BL, 1);
  ledcWrite(1, 128);

  // LVGL init
  lv_init();

  // LVGL style init
  grid_style_init();
  btn_style_init();

  // Buffer setup
  size_t pixel_count = BUFFER_SIZE / sizeof(lv_color_t);
  lv_disp_draw_buf_init(&draw_buf, buf, NULL, pixel_count);

  // Display driver init
  lv_disp_drv_init(&disp_drv);
  disp_drv.flush_cb = lv_flush_cb;
  disp_drv.draw_buf = &draw_buf;
  disp_drv.hor_res = LCD_WIDTH;
  disp_drv.ver_res = LCD_HEIGHT;
  lv_disp_drv_register(&disp_drv);

  // Touch init
  touch_init();

  // Touch driver init
  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = lv_touch_cb;
  lv_indev_drv_register(&indev_drv);

  // Create a grid on the active screen
  setup_grid_dsc();
  lv_obj_t *grid = lv_obj_create(lv_scr_act());
  lv_obj_add_style(grid, &grid_style, 0);
  lv_obj_set_size(grid, LCD_WIDTH, LCD_HEIGHT);
  lv_obj_center(grid);
  lv_obj_set_layout(grid, LV_LAYOUT_GRID);
  lv_obj_set_grid_dsc_array(grid, col_dsc, row_dsc);

  // Add buttons with labels based on grid size
  for (int i = 0; i < (GRID_COLS * GRID_ROWS); i++)
  {
    lv_obj_t *btn = lv_btn_create(grid);
    lv_obj_set_size(btn, 80, 80);
    lv_obj_set_grid_cell(btn, LV_GRID_ALIGN_CENTER, i % GRID_COLS, 1, LV_GRID_ALIGN_CENTER, i / GRID_COLS, 1);
    lv_obj_add_style(btn, &btn_style, 0);

    lv_obj_t *label = lv_label_create(btn);
    lv_label_set_text_fmt(label, "App %d", i + 1);
    lv_obj_center(label);
    lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), 0);
  }
}

// Let lvgl do its thing
void loop()
{
  lv_timer_handler();
  delay(5);
}