#pragma once
#include <Arduino.h>

class DeviceBase
{
public:
    virtual ~DeviceBase(); // Ensure proper cleanup in derived classes

    // Default read function, overridden by sensors
    virtual float read();

    // Default write function, overridden by actuators
    virtual void write(uint8_t value);
};
