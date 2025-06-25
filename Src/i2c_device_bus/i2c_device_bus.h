#pragma once

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_hal.h"
#include "i2c.h"

void I2C_Slave_Init();
void I2C_Slave_PollingLoop();
void ProcessReceivedCommand(uint8_t *data);