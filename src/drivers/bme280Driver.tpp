#include "bme280Driver.h"

template <uint8_t CS_PIN>
BME280_SPI<CS_PIN>::BME280_SPI() : bme(CS_PIN)
{
    // With SPI, the chip-select is passed in the constructor.
    if (!bme.begin())
    {
        Serial.println("BME280 SPI initialization failed!");
    }
}

template <uint8_t CS_PIN>
float BME280_SPI<CS_PIN>::read()
{
    // For example, return the temperature reading.
    // You could also use readPressure() or readHumidity() based on your needs.
    return bme.readTemperature();
}

