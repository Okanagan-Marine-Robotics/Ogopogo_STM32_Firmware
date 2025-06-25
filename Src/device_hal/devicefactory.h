#pragma once
#include "devicebase.h"
#include "configuration.h"

Device device_array[NUM_DEVICES];
void SetupDevices();
Device* FindDeviceByName(const char* name);
size_t GetDevicesByType(DeviceType type, Device** devices, size_t max_devices);
