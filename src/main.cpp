#include <lvgl.h>

// Define backlight pin number
#define TFT_BL 2

// Setup LoyvanGFX for the Elecrow Display
#include "LGFX.h"
LGFX lcd;

// Define screen sizes
#define LCD_WIDTH 800
#define LCD_HEIGHT 480

// Setup touch on the screen
#include "touch.h"

////////////////
// LVGL SETUP //
////////////////

// Buffer size
#define BUFFER_SIZE ((LCD_WIDTH * LCD_HEIGHT * 2) / 6)

// LVGL buffer and driver
#include "esp_heap_caps.h"

static lv_disp_draw_buf_t draw_buf;
static lv_color_t *buf = (lv_color_t *)heap_caps_aligned_alloc(32, BUFFER_SIZE, MALLOC_CAP_DMA);
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

// Grid Setup
#define GRID_COLS 8
#define GRID_ROWS 4

// Generate column/row descriptors dynamically
static lv_coord_t col_dsc[GRID_COLS + 1];
static lv_coord_t row_dsc[GRID_ROWS + 1];

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

  size_t pixel_count = BUFFER_SIZE / sizeof(lv_color_t);
  lv_disp_draw_buf_init(&draw_buf, buf, NULL, pixel_count);

  lv_disp_drv_init(&disp_drv);
  disp_drv.flush_cb = lv_flush_cb;
  disp_drv.draw_buf = &draw_buf;
  disp_drv.hor_res = LCD_WIDTH;
  disp_drv.ver_res = LCD_HEIGHT;
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
  setup_grid_dsc();
  lv_obj_t *grid = lv_obj_create(page1);
  lv_obj_set_size(grid, LCD_WIDTH, LCD_HEIGHT);
  lv_obj_center(grid);
  lv_obj_set_layout(grid, LV_LAYOUT_GRID);
  lv_obj_set_grid_dsc_array(grid, col_dsc, row_dsc);

  // Add buttons with icons
  for (int i = 0; i < (GRID_COLS * GRID_ROWS); i++)
  {
    lv_obj_t *btn = lv_btn_create(grid);
    lv_obj_set_size(btn, 80, 80);
    lv_obj_set_grid_cell(btn, LV_GRID_ALIGN_CENTER, i % GRID_COLS, 1, LV_GRID_ALIGN_CENTER, i / GRID_COLS, 1);

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