#include "battery.h"
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(battery, CONFIG_APP_LOG_LEVEL);

int battery_sensor_init(battery_sensor_t *battery_sensor) {
  LOG_DBG("Initializing vbatt voltage_divider sensor");
  
  // Load sensor from devicetree
  battery_sensor->voltage_divider = DEVICE_DT_GET(DT_PATH(vbatt));
  if (!battery_sensor->voltage_divider) {
    LOG_ERR("vbatt voltage divider is not declared in the device tree");
    return -ENODEV;
  } else if (!device_is_ready(battery_sensor->voltage_divider)) {
    LOG_ERR("vbatt voltage divider device is not ready");
    return -EAGAIN;
  }

  // success
  return 0;
}
int battery_sensor_read(battery_sensor_t *battery_sensor, int32_t *value) {
  LOG_DBG("Reading value from vbatt voltage_divider");

  int error;

  // fetch reading
  if ((error = sensor_sample_fetch(battery_sensor->voltage_divider))) {
    LOG_ERR("failed to fetch sample from voltage divider: %d", error);
    return error;
  }

  // get reading value
  struct sensor_value reading;
  if ((error = sensor_channel_get(battery_sensor->voltage_divider, SENSOR_CHAN_VOLTAGE, &reading))) {
    LOG_ERR("failed to read data from voltage divider: %d", error);
    return error;
  }

  *value = (int32_t) sensor_value_to_milli(&reading);

  return 0;
}
