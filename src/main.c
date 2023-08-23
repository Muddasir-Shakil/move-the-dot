#include <device.h>
#include <drivers/display.h>
#include <lvgl.h>
#include <stdio.h>
#include <string.h>
#include <zephyr.h>
#include <dot.h>

#define LOG_LEVEL CONFIG_LOG_DEFAULT_LEVEL
#include <logging/log.h>
LOG_MODULE_REGISTER(app);
void main(void)
{
	const struct device *display_dev;
	display_dev = device_get_binding(CONFIG_LVGL_DISPLAY_DEV_NAME);

	if (display_dev == NULL)
	{
		LOG_ERR("device not found.  Aborting test.");
		return;
	}
	display_blanking_off(display_dev);
	
	lv_obj_t *dot = create_dot();
	reset_dot(dot);
	lv_task_handler();
	
	lv_coord_t x = 1;
	lv_coord_t y = 1;
	while (1)
	{
		if (x <= 128)
		{
			move_dot_x(dot, x);
			++x;
		}
		else
		{
			move_dot_y(dot, y);
			++y;
		}
		lv_task_handler();
		k_sleep(K_MSEC(10));
		if (x >= 120 && y >= 60)
		{
			reset_dot(dot);
			x = 0;
			y = 0;
		}
	}
}
