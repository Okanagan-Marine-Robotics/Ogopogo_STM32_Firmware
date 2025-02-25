#include <Arduino.h>
#include <Wire.h>
#include <./deviceRegistry/deviceRegistry.h>

struct I2CCommand
{
    uint8_t deviceID;
    uint8_t commandID;
};

uint8_t txBuffer[8];

void receiveEvent(int bytes)
{
    I2CCommand cmd;
    Wire.readBytes((uint8_t *)&cmd, sizeof(I2CCommand));

    switch (cmd.commandID)
    {
    case 0x01:
    { // Read
        float value = registry.readDevice(cmd.deviceID);
        memcpy(txBuffer, &value, sizeof(value));
        break;
    }
    case 0x02:
    { // Write
        uint8_t writeValue = Wire.read();
        registry.writeDevice(cmd.deviceID, writeValue);
        break;
    }
    }
}

void requestEvent()
{
    Wire.write(txBuffer, sizeof(txBuffer));
    return;
}
