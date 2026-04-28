#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/hwinfo.h>
#include <zephyr/sys/poweroff.h>
#include <zephyr/sys/util.h>
#include <zephyr/device.h>
#include "battery.h"
#include "bluetooth.h"

LOG_MODULE_REGISTER(main, CONFIG_APP_LOG_LEVEL);

void power_off(void) {
  hwinfo_clear_reset_cause();
  sys_poweroff();
}

int main(void) {
  // test SWD logging (non trivial?)
  LOG_DBG("Hello, world!");

  const struct gpio_dt_spec status_led = GPIO_DT_SPEC_GET(DT_ALIAS(status_led), gpios);
  if (!gpio_is_ready_dt(&status_led)) {
    LOG_ERR("Status LED is not ready");
    return -ENODEV;
  }

  int error;

  // apply cfg
  if ((error = gpio_pin_configure_dt(&status_led, GPIO_OUTPUT_ACTIVE))) {
    LOG_ERR("Status LED config error: %d", error);
    return error;
  }

  // flash for 0.2s
  gpio_pin_set_dt(&status_led, 1);
  k_sleep(K_MSEC(200));
  gpio_pin_set_dt(&status_led, 0);

  // configure button for wakeup
  const struct gpio_dt_spec limit_switch = GPIO_DT_SPEC_GET(DT_ALIAS(limit_switch), gpios);
  if (!gpio_is_ready_dt(&limit_switch)) {
    LOG_ERR("Limit switch is not ready");
    return -ENODEV;
  }

  // apply cfg
  if ((error = gpio_pin_configure_dt(&limit_switch, GPIO_INPUT))) {
    LOG_ERR("Limit switch config error: %d", error);
    return error;
  }

  // enable interrupt
  if ((error = gpio_pin_interrupt_configure_dt(&limit_switch, GPIO_INT_LEVEL_ACTIVE))) {
    LOG_ERR("Limit switch interrupt enable error: %d", error);
    return error;
  }

  battery_sensor_t battery;
  if ((error = battery_sensor_init(&battery))) return error;
  int32_t battery_level;
  if ((error = battery_sensor_read(&battery, &battery_level))) return error;

  // determine if button pressed (power on due to gpio interrupt)
  uint32_t reset_cause;
  hwinfo_get_reset_cause(&reset_cause);
  bool button_pressed = (reset_cause & RESET_LOW_POWER_WAKE) != 0;

  // send bluetooth advertisment, with button pressed if applicable
  if ((error = bluetooth_init())) return error;
  if ((error = bluetooth_advertise(battery_level, button_pressed))) return error;

  // wait 1s then power off
  k_sleep(K_MSEC(500));
  
  if ((error = bluetooth_advertise_stop())) return error;
  power_off();
  
  return 0;
}
