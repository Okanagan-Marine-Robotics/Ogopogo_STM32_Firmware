#pragma once
#include <Arduino.h>
#include <configuration.h>

typedef enum
{
    I2C_READY_TO_SEND,
    I2C_READY_TO_RECEIVE,
} I2CStatus;

class I2CHandler
{
private:
    volatile uint8_t lastResponseCode[I2C_TX_BUFFER_SIZE] = {0x00};
    volatile uint8_t txBytes = 1;

    static I2CHandler &getInstance()
    {
        static I2CHandler instance; // Created once, persists
        return instance;
    }

public:
    void initializeI2C();
    void receiveEvent(int numBytes);
    void requestEvent();

    static void receiveEventStatic(int numBytes)
    {
        getInstance().receiveEvent(numBytes);
    }
    static void requestEventStatic()
    {
        getInstance().requestEvent();
    }
};