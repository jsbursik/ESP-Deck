#pragma once

#define TOUCH_GT911
#define TOUCH_GT911_SCL 20 // 20
#define TOUCH_GT911_SDA 19 // 19
#define TOUCH_GT911_INT -1 //-1
#define TOUCH_GT911_RST -1 // 38
#define TOUCH_GT911_ROTATION ROTATION_NORMAL
#define TOUCH_MAP_X1 800 // 480
#define TOUCH_MAP_X2 0
#define TOUCH_MAP_Y1 480 // 272
#define TOUCH_MAP_Y2 0

extern int touch_last_x, touch_last_y;

void touch_init();

bool touch_has_signal();

bool touch_touched();

bool touch_released();
