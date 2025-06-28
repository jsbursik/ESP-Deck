#include <LovyanGFX.hpp>
#include <lgfx/v1/platforms/esp32s3/Panel_RGB.hpp>
#include <lgfx/v1/platforms/esp32s3/Bus_RGB.hpp>
#include <lvgl.h>

// Define backlight pin number
#define TFT_BL 2

// Create display driver instance
class LGFX : public lgfx::LGFX_Device
{
public:
  lgfx::Bus_RGB _bus_instance;
  lgfx::Panel_RGB _panel_instance;

  LGFX(void)
  {

    {
      auto cfg = _bus_instance.config();
      cfg.panel = &_panel_instance;

      cfg.pin_d0 = GPIO_NUM_15; // B0
      cfg.pin_d1 = GPIO_NUM_7;  // B1
      cfg.pin_d2 = GPIO_NUM_6;  // B2
      cfg.pin_d3 = GPIO_NUM_5;  // B3
      cfg.pin_d4 = GPIO_NUM_4;  // B4

      cfg.pin_d5 = GPIO_NUM_9;  // G0
      cfg.pin_d6 = GPIO_NUM_46; // G1
      cfg.pin_d7 = GPIO_NUM_3;  // G2
      cfg.pin_d8 = GPIO_NUM_8;  // G3
      cfg.pin_d9 = GPIO_NUM_16; // G4
      cfg.pin_d10 = GPIO_NUM_1; // G5

      cfg.pin_d11 = GPIO_NUM_14; // R0
      cfg.pin_d12 = GPIO_NUM_21; // R1
      cfg.pin_d13 = GPIO_NUM_47; // R2
      cfg.pin_d14 = GPIO_NUM_48; // R3
      cfg.pin_d15 = GPIO_NUM_45; // R4

      cfg.pin_henable = GPIO_NUM_41;
      cfg.pin_vsync = GPIO_NUM_40;
      cfg.pin_hsync = GPIO_NUM_39;
      cfg.pin_pclk = GPIO_NUM_0;
      cfg.freq_write = 15000000;

      cfg.hsync_polarity = 0;
      cfg.hsync_front_porch = 40;
      cfg.hsync_pulse_width = 48;
      cfg.hsync_back_porch = 40;

      cfg.vsync_polarity = 0;
      cfg.vsync_front_porch = 1;
      cfg.vsync_pulse_width = 31;
      cfg.vsync_back_porch = 13;

      cfg.pclk_active_neg = 1;
      cfg.de_idle_high = 0;
      cfg.pclk_idle_high = 0;

      _bus_instance.config(cfg);
    }
    {
      auto cfg = _panel_instance.config();
      cfg.memory_width = 800;
      cfg.memory_height = 480;
      cfg.panel_width = 800;
      cfg.panel_height = 480;
      cfg.offset_x = 0;
      cfg.offset_y = 0;
      _panel_instance.config(cfg);
    }
    _panel_instance.setBus(&_bus_instance);
    setPanel(&_panel_instance);
  }
};

LGFX lcd;

#include "touch.h"

// LVGL buffer and driver
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[800 * 60]; // Partial buffer: 60 lines
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

  // Example UI:
  lv_obj_t *label = lv_label_create(lv_scr_act());
  lv_label_set_text(label, "Hello Elecrow!");
  lv_obj_align(label, LV_ALIGN_CENTER, 0, -40);

  lv_obj_t *btn = lv_btn_create(lv_scr_act());
  lv_obj_align(btn, LV_ALIGN_CENTER, 0, 40);
  lv_obj_t *btn_label = lv_label_create(btn);
  lv_label_set_text(btn_label, "Press me!");
}

void loop()
{
  lv_timer_handler();
  delay(5);
}