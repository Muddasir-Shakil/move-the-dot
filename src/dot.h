#ifndef DOT_H
#define DOT_H

#include <device.h>
#include <drivers/display.h>
#include <lvgl.h>
#include <stdio.h>
#include <string.h>
#include <zephyr.h>

lv_obj_t *create_dot(void);
void move_dot_x(lv_obj_t *dot, lv_coord_t x);
void move_dot_y(lv_obj_t *dot, lv_coord_t y);
void reset_dot(lv_obj_t *dot);

#endif