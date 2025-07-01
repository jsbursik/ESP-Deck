#include "lvgl_setup.h"
#include "config.h"
#include "esp_heap_caps.h"
#include "lcd_shared.h"
#include "touch.h"
#include <lvgl.h>

static lv_disp_draw_buf_t draw_buf;
static lv_color_t *buf = (lv_color_t *)heap_caps_aligned_alloc(32, BUFFER_SIZE, MALLOC_CAP_DMA);
static lv_disp_drv_t disp_drv;

// Display flush
void lv_flush_cb(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
  lcd.pushImageDMA(area->x1, area->y1, area->x2 - area->x1 + 1, area->y2 - area->y1 + 1,
                   (lgfx::rgb565_t *)&color_p->full);
  lv_disp_flush_ready(disp);
}

// Touch read function
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

// Generate column/row descriptors dynamically
lv_coord_t col_dsc[GRID_COLS + 1];
lv_coord_t row_dsc[GRID_ROWS + 1];

void setup_grid_dsc(void)
{
  lv_coord_t col_pct = 100 / GRID_COLS;
  lv_coord_t row_pct = 100 / GRID_ROWS;

  for (int i = 0; i < GRID_COLS; i++)
  {
    col_dsc[i] = LV_PCT(col_pct);
  }
  col_dsc[GRID_COLS] = LV_GRID_TEMPLATE_LAST;

  for (int i = 0; i < GRID_ROWS; i++)
  {
    row_dsc[i] = LV_PCT(row_pct);
  }
  row_dsc[GRID_ROWS] = LV_GRID_TEMPLATE_LAST;
}