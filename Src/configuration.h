#pragma once
#include "device_hal/devicebase.h"
#include "spi.h"
#include "tim.h"

// 7 bit address, shifted 1 bit to the left to match the 7-bit address format
#define I2C_SLAVE_ADDRESS 0x42

#define NUM_DEVICES 7
extern const DeviceConfig device_configs[];

size_t get_num_devices(void);