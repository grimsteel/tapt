#pragma once

#include <stdint.h>
#include <stdbool.h>

int bluetooth_init(void);
int bluetooth_advertise(int32_t battery_level, bool pressed);
int bluetooth_advertise_stop(void);
