#include <Arduino.h>
#include <Wire.h>
#include <configuration.h>
#include <eventHandler.h>
#include <deviceRegistry/deviceRegistry.h>
#include <drivers/gpioDriver.h>
#include <drivers/servoDriver.h>

void setup()
{
    Wire.begin(I2C_SLAVE_ADDRESS);
    Wire.onReceive(receiveEvent);
    Wire.onRequest(requestEvent);

    // Register devices
    registry.registerDevice(0x02, new GPIO<2>());
    registry.registerDevice(0x03, new GPIO<3>());
    registry.registerDevice(0x04, new GPIO<4>());
    registry.registerDevice(0x05, new SERVO<5>());
    registry.registerDevice(0x06, new SERVO<6>());
}

void loop()
{
    delay(1000);
}