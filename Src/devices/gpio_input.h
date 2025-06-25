#pragma once
#include "stm32g0xx.h"
#include "device_hal/devicebase.h"

typedef enum
{
    GPIO_PULL_NONE = 0,
    GPIO_PULL_UP,
    GPIO_PULL_DOWN
} GPIOPullMode;

typedef struct
{
    GPIO_TypeDef *port;
    uint16_t pin;
    GPIOPullMode pull; // Optional: for pull-up/pull-down configuration

} GPIOInputContext;

void gpio_input_read(Device *dev, void *value);
void GPIOInput_Create(Device *dev, GPIO_TypeDef *port, uint16_t pin, GPIOPullMode pull);
void gpio_input_init(Device *dev);