#pragma once

#include <Arduino.h>
#include "deviceBase.h"

template <uint8_t PIN>
class GPIO : public DeviceBase
{
public:
    GPIO();
    void write(uint8_t state) override;
    float read() override;
};

#include "gpioDriver.tpp"