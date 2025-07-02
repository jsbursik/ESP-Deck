#pragma once
#include <lvgl.h>

// Display flush callback
void lv_flush_cb(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);

// Touch read callback
void lv_touch_cb(lv_indev_drv_t *indev_driver, lv_indev_data_t *data);

// Grid descriptor setup
void setup_grid_dsc(void);

extern lv_disp_draw_buf_t draw_buf;
extern lv_color_t *buf;
extern lv_disp_drv_t disp_drv;

extern lv_coord_t col_dsc[];
extern lv_coord_t row_dsc[];