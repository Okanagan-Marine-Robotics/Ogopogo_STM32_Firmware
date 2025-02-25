#pragma once

#include <Arduino.h>
#include "deviceBase.h"

template <uint8_t PIN, uint16_t NUM_LEDS = 1>
class WS2812 : public DeviceBase
{
private:
public:
    WS2812();
    void write(uint8_t brightness) override;
    void setColor(uint8_t index, uint8_t r, uint8_t g, uint8_t b);
    void show();
};

#include "servoDriver.tpp"