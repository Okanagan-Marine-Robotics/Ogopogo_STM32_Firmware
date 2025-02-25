#include "deviceRegistry.h"

void DeviceRegistry::registerDevice(uint8_t id, DeviceBase *device)
{
    devices[id] = device; // Store device pointer
}

float DeviceRegistry::readDevice(uint8_t id)
{
    if (devices.find(id) != devices.end())
    {
        return devices[id]->read();
    }
    return -1; // Default error value
}

void DeviceRegistry::writeDevice(uint8_t id, uint8_t value)
{
    if (devices.find(id) != devices.end())
    {
        devices[id]->write(value);
    }
}

DeviceRegistry registry;