#pragma once

#include <zephyr/drivers/sensor.h>

typedef struct {
  const struct device *voltage_divider;
} battery_sensor_t;

int battery_sensor_init(battery_sensor_t *battery_sensor);
/// Read the current voltage level on the battery sensor in millivolts
int battery_sensor_read(battery_sensor_t *battery_sensor, int32_t *value);
