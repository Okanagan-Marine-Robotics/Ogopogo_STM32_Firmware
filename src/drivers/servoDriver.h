#pragma once

#include <Arduino.h>
#include "deviceBase.h"
#include <Servo.h>

template <uint8_t PIN>
class SERVO : public DeviceBase
{
private:
    Servo servo;

public:
    SERVO();
    void write(uint8_t state) override;
};

#include "servoDriver.tpp"