#include "i2c_device_bus.h"

#define I2C_BUFFER_SIZE 32
#define I2C_TIMEOUT 1000


uint8_t rxBuffer[I2C_BUFFER_SIZE];
uint8_t txBuffer[2];

void I2C_Slave_Init()
{
    // Prepare default transmit buffer
    txBuffer[0] = 0xDE;
    txBuffer[1] = 0xAD;
    
    // Clear receive buffer
    memset(rxBuffer, 0, sizeof(rxBuffer));
}

void I2C_Slave_PollingLoop()
{
    HAL_StatusTypeDef status;

    // 1. Wait for master write (transmit to us)
    status = HAL_I2C_Slave_Receive(&hi2c1, rxBuffer, 1, I2C_TIMEOUT);

    if (status == HAL_OK)
    {
        // Process the command
        ProcessReceivedCommand(rxBuffer);
    }
    else if (status != HAL_TIMEOUT)
    {
        // Recover on error
        HAL_I2C_DeInit(&hi2c1);
        HAL_I2C_Init(&hi2c1);
        return;
    }

    // 2. Wait for master read request (we transmit)
    HAL_I2C_DeInit(&hi2c1);
    HAL_I2C_Init(&hi2c1);
    status = HAL_I2C_Slave_Transmit(&hi2c1, txBuffer, 2, I2C_TIMEOUT);

    if (status != HAL_OK && status != HAL_TIMEOUT)
    {
        // Recover on error
        HAL_I2C_DeInit(&hi2c1);
        HAL_I2C_Init(&hi2c1);
    }
}

void ProcessReceivedCommand(uint8_t *data)
{
    switch (data[0])
    {
        case 0x01:
            txBuffer[0] = 0x01;
            txBuffer[1] = 0x23;
            break;

        case 0x02:
            // LED control example
            txBuffer[0] = 0x02;
            txBuffer[1] = 0x34;
            break;

        case 0x10:
            // Return fixed sensor data
            txBuffer[0] = 0x10;
            txBuffer[1] = 0xAB;
            break;

        default:
            txBuffer[0] = 0xDE;
            txBuffer[1] = 0xAD;
            break;
    }
}
