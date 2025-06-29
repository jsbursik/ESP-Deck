#include <lvgl.h>

// Define backlight pin number
#define TFT_BL 2

// Setup LoyvanGFX for the Elecrow Display
#include "LGFX.h"
LGFX lcd;

// Setup touch on the screen
#include "touch.h"

////////////////
// LVGL SETUP //
////////////////

// LVGL buffer and driver
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[800 * 80];
static lv_disp_drv_t disp_drv;

// Display flush function
void lv_flush_cb(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
  lcd.pushImageDMA(area->x1, area->y1, area->x2 - area->x1 + 1, area->y2 - area->y1 + 1,
                   (lgfx::rgb565_t *)&color_p->full);
  lv_disp_flush_ready(disp);
}

// Touch read function for LVGL
void lv_touch_cb(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
  if (touch_touched())
  {
    data->state = LV_INDEV_STATE_PR;
    data->point.x = touch_last_x;
    data->point.y = touch_last_y;
  }
  else
  {
    data->state = LV_INDEV_STATE_RELEASED;
  }
}

////////////////////
// Setup and Loop //
////////////////////

void setup()
{
  Serial.begin(9600);

  pinMode(38, OUTPUT);
  digitalWrite(38, LOW);

  lcd.begin();
  ledcSetup(1, 300, 8);
  ledcAttachPin(TFT_BL, 1);
  ledcWrite(1, 128);

  lv_init();

  lv_disp_draw_buf_init(&draw_buf, buf, NULL, sizeof(buf) / sizeof(lv_color_t));

  lv_disp_drv_init(&disp_drv);
  disp_drv.flush_cb = lv_flush_cb;
  disp_drv.draw_buf = &draw_buf;
  disp_drv.hor_res = 800;
  disp_drv.ver_res = 480;
  lv_disp_drv_register(&disp_drv);

  touch_init();

  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = lv_touch_cb;
  lv_indev_drv_register(&indev_drv);

  lv_obj_t *tileview = lv_tileview_create(lv_scr_act());
  lv_obj_t *page1 = lv_tileview_add_tile(tileview, 0, 0, LV_DIR_RIGHT);
  lv_obj_t *page2 = lv_tileview_add_tile(tileview, 1, 0, LV_DIR_LEFT | LV_DIR_RIGHT);

  // Create a grid on page1
  static lv_coord_t col_dsc[] = {LV_PCT(33), LV_PCT(33), LV_PCT(33), LV_GRID_TEMPLATE_LAST};
  static lv_coord_t row_dsc[] = {LV_PCT(50), LV_PCT(50), LV_GRID_TEMPLATE_LAST};

  lv_obj_t *grid = lv_obj_create(page1);
  lv_obj_set_size(grid, 800, 480);
  lv_obj_center(grid);
  lv_obj_set_layout(grid, LV_LAYOUT_GRID);
  lv_obj_set_grid_dsc_array(grid, col_dsc, row_dsc);

  // Add buttons with icons
  for (int i = 0; i < 6; i++)
  {
    lv_obj_t *btn = lv_btn_create(grid);
    lv_obj_set_size(btn, 80, 80);
    lv_obj_set_grid_cell(btn, LV_GRID_ALIGN_CENTER, i % 3, 1, LV_GRID_ALIGN_CENTER, i / 3, 1);

    lv_obj_t *label = lv_label_create(btn);
    lv_label_set_text_fmt(label, "App %d", i + 1);
    lv_obj_center(label);
  }
}

void loop()
{
  lv_timer_handler();
  delay(5);
}