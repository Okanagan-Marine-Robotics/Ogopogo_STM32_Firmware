#include <Arduino.h>
#include <Wire.h>
#include <configuration.h>
#include <eventHandler.h>
#include <deviceRegistry/deviceRegistry.h>
#include <drivers/gpioDriver.h>
#include <drivers/servoDriver.h>
#include <drivers/bme280Driver.h>

void setup()
{
    Wire.begin(I2C_SLAVE_ADDRESS);
    Wire.onReceive(receiveEvent);
    Wire.onRequest(requestEvent);

    // Register devices
    registry.registerDevice(0x02, new GPIO<2>());                               // standard GPIO pin pull-up mode
    registry.registerDevice(0x03, new GPIO<3, INPUT_PULLDOWN, false, false>()); // standard GPIO pin pull-down mode
    registry.registerDevice(0x04, new GPIO<4, INPUT_PULLUP, true>());           // analog read
    registry.registerDevice(0x05, new SERVO<5>());
    registry.registerDevice(0x06, new SERVO<6>());
    registry.registerDevice(0x07, new BME280_SPI<PA4>());
}

void loop()
{
    delay(1000);
}