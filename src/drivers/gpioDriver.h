#pragma once

#include <Arduino.h>
#include "deviceBase.h"

template <uint8_t PIN, uint8_t MODE = INPUT_PULLUP, bool ANALOG = false, bool INVERTING = false>
class GPIO : public DeviceBase
{
public:
    GPIO();
    void write(uint8_t state) override;
    float read() override;
};

#include "gpioDriver.tpp"