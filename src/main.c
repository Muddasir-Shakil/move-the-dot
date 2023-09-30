#include <device.h>
#include <drivers/display.h>
#include <drivers/sensor.h>
#include <lvgl.h>
#include <stdio.h>
#include <string.h>
#include <zephyr.h>
#include <dot.h>
#include <math.h>

#define LOG_LEVEL CONFIG_LOG_DEFAULT_LEVEL
#include <logging/log.h>
LOG_MODULE_REGISTER(app);

float roll = 0;
float pitch = 0;
void init_display()
{
	const struct device *display_dev;
	display_dev = device_get_binding(CONFIG_LVGL_DISPLAY_DEV_NAME);

	if (display_dev == NULL)
	{
		LOG_ERR("device not found.  Aborting test.");
		return;
	}
	display_blanking_off(display_dev);
}

const struct device *init_mpu6050(void)
{
	const char *const label = DT_LABEL(DT_INST(0, invensense_mpu6050));
	const struct device *mpu6050 = device_get_binding(label);

	if (!mpu6050)
	{
		printf("Failed to find sensor %s\n", label);
		return NULL;
	}
	return mpu6050;
}

static float process_roll(struct sensor_value *accel)
{
	double x = sensor_value_to_double(&accel[0]);
	double y = sensor_value_to_double(&accel[1]);
	double z = sensor_value_to_double(&accel[2]);
	roll = atan(y / sqrt((x * x) + (z * z)));
	roll = roll * (180.0 / 3.14);
	printf("roll = %f \n", roll);
	return roll;
}

static float process_pitch(struct sensor_value *accel)
{
	double x = sensor_value_to_double(&accel[0]);
	double y = sensor_value_to_double(&accel[1]);
	double z = sensor_value_to_double(&accel[2]);
	pitch = atan(x / sqrt((y * y) + (z * z)));
	pitch = pitch * (180.0 / 3.14);
	printf("pitch = %f \n", pitch);
	return pitch;
}

static int process_mpu6050(const struct device *dev)
{
	struct sensor_value accel[3];
	int rc = sensor_sample_fetch(dev);

	if (rc == 0)
	{
		rc = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ,
								accel);
	}
	if (rc == 0)
	{
		printf("  accel %f %f %f m/s/s\n",
			   sensor_value_to_double(&accel[0]),
			   sensor_value_to_double(&accel[1]),
			   sensor_value_to_double(&accel[2]));
		process_roll(accel);
		process_pitch(accel);
	}
	else
	{
		printf("sample fetch/get failed: %d\n", rc);
	}

	return rc;
}

static lv_coord_t calculate_cord_x()
{
	lv_coord_t x = 64;
	if(roll > 64)
		x = 128;
	if(roll < -64)
		x = 0;
	if(roll < 64 && roll > -64)
		x = (lv_coord_t)roll + 64;
	return x;
}

static lv_coord_t calculate_cord_y()
{
	lv_coord_t y = 32;
	if(pitch > 32)
		y = 64;
	if(pitch < -32)
		y = 0;
	if(pitch < 32 && pitch > -32)
		y = (lv_coord_t)pitch + 32;
	return y;
}

void main(void)
{

	init_display();
	const struct device *mpu6050 = init_mpu6050();
	if (!mpu6050)
		return;

	lv_obj_t *dot = create_dot();
	reset_dot(dot);
	lv_task_handler();

	lv_coord_t x = 64;
	lv_coord_t y = 32;
	while (1)
	{
		move_dot_x(dot, x);
		move_dot_y(dot, y);
		lv_task_handler();
		k_sleep(K_MSEC(100));
		process_mpu6050(mpu6050);
		x = calculate_cord_x();
		y = calculate_cord_y();
	}
}
