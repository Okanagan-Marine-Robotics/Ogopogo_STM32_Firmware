#include "deviceBase.h"

// Virtual destructor implementation
DeviceBase::~DeviceBase() {}

// Default read implementation
float DeviceBase::read()
{
    return 0.0;
}

// Default write implementation
void DeviceBase::write(uint8_t value)
{
    // No action by default
}