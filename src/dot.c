#include "dot.h"

lv_obj_t *create_dot(void)
{
    /*Create style*/
    static lv_style_t style_line;
    lv_style_init(&style_line);
    lv_style_set_line_width(&style_line, LV_STATE_DEFAULT, 10);
    lv_style_set_line_color(&style_line, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_line_rounded(&style_line, LV_STATE_DEFAULT, true);

    static lv_point_t dot_points[] = {{0, 0}, {1, 0}, {1, 1}, {0, 1}, {0, 0}};
    lv_obj_t *dot = lv_line_create(lv_scr_act(), NULL);
    lv_line_set_points(dot, dot_points, 5); /*Set the points*/
    lv_obj_add_style(dot, LV_LINE_PART_MAIN, &style_line);
    lv_obj_align(dot, NULL, LV_ALIGN_CENTER, 0, 0);
    return dot;
}

void move_dot_x(lv_obj_t *dot, lv_coord_t x)
{
    lv_obj_set_x(dot, x);
}

void move_dot_y(lv_obj_t *dot, lv_coord_t y)
{
    lv_obj_set_y(dot, y);
}

void reset_dot(lv_obj_t *dot)
{
    lv_obj_set_pos(dot, 1, 1);
}
