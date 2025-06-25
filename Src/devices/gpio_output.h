#pragma once
#include "stm32g0xx.h"
#include "device_hal/devicebase.h"

typedef struct
{
    GPIO_TypeDef *port;
    uint16_t pin;
} GPIOOutputContext;

void gpio_output_init(Device *dev);
void gpio_output_write(Device *dev, const void *value);
void GPIOOutput_Create(Device *dev, GPIO_TypeDef *port, uint16_t pin);
