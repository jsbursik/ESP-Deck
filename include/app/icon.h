#ifndef ICON_OBJECT_H
#define ICON_OBJECT_H

#include <lvgl.h>

enum IconType
{
  ICON_TYPE_BLANK,
  ICON_TYPE_WIDGET_RESERVED,
  ICON_TYPE_WIDGET,
  ICON_TYPE_TEXT_LABEL,
  ICON_TYPE_ICON_ACTION,
  ICON_TYPE_ICON_TOGGLE
};

struct IconObject
{
  IconType type;

  const char *label = nullptr;
  const char *action_id = nullptr;
  const char *name = nullptr;

  lv_color_t bg_color = lv_color_black();
  lv_color_t bg_color_active = lv_color_black();

  bool toggle_state = false; // Only for ICON_TYPE_ICON_TOGGLE
};

#endif