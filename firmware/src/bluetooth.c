#include "bluetooth.h"
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/gap.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(bluetooth, CONFIG_APP_LOG_LEVEL);

#define DEVICE_NAME CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LEN (sizeof(DEVICE_NAME) - 1)
#define NORDIC_COMPANY_ID 0x0059

typedef struct adv_mfg_data {
  uint16_t company_code; // Nordic Semiconductor
  uint16_t battery_level;
  uint8_t device_id;
  bool pressed;
} adv_mfg_data_t;

int bluetooth_init(void) {
  int error;
  if ((error = bt_enable(NULL))) { // synchronous load
    LOG_ERR("Bluetooth init failed: %d", error);
    return error;
  }
  LOG_DBG("Initialized bluetooth");
  return 0;
}
int bluetooth_advertise(int32_t battery_level, bool pressed) {
  const adv_mfg_data_t mfg_data = {
    .company_code = NORDIC_COMPANY_ID,
    .battery_level = battery_level,
    .device_id = CONFIG_TAPT_DEVICE_ID,
    .pressed = pressed
  };
  
  const struct bt_data ad[] = {
    BT_DATA_BYTES(BT_DATA_FLAGS, BT_LE_AD_NO_BREDR),
    BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN),
    BT_DATA(BT_DATA_MANUFACTURER_DATA, &mfg_data, sizeof(mfg_data))
  };
  int error;
  if ((error = bt_le_adv_start(BT_LE_ADV_NCONN, ad, ARRAY_SIZE(ad), NULL, 0))) {
    LOG_ERR("Bluetooth advertise failed: %d", error);
    return error;
  }
  return 0;
}
int bluetooth_advertise_stop(void) {
  int error;
  if ((error = bt_le_adv_stop())) {
    LOG_ERR("Bluetooth stop advertise failed: %d", error);
    return error;
  }
  return 0;
}
