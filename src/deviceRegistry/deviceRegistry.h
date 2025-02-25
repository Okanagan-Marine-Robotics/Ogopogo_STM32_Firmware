#pragma once

#include <map>
#include <cstdint>
#include "./drivers/deviceBase.h"

class DeviceRegistry
{
private:
    std::map<uint8_t, DeviceBase *> devices; // Maps device ID to device instances

public:
    void registerDevice(uint8_t id, DeviceBase *device);
    float readDevice(uint8_t id);
    void writeDevice(uint8_t id, uint8_t value);
};

// create an instance of the DeviceRegistry class
extern DeviceRegistry registry;