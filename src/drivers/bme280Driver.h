#pragma once

#include <Adafruit_BME280.h>
#include <SPI.h>
#include "deviceBase.h"

template <uint8_t CS_PIN>
class BME280_SPI : public DeviceBase
{
private:
    Adafruit_BME280 bme; // BME280 sensor instance

public:
    BME280_SPI();
    // For a sensor, reading is the primary function.
    float read() override;
    // If your DeviceBase interface requires a write(), you can provide an empty implementation.
    void write(uint8_t) override {}
};

#include "bme280Driver.tpp"