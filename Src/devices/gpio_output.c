#include "gpio_output.h"
#include <stdlib.h>

void gpio_output_init(Device *dev)
{
    GPIOOutputContext *ctx = (GPIOOutputContext *)dev->context;

    if (ctx->port == GPIOA) __HAL_RCC_GPIOA_CLK_ENABLE();
    else if (ctx->port == GPIOB) __HAL_RCC_GPIOB_CLK_ENABLE();
    else if (ctx->port == GPIOC) __HAL_RCC_GPIOC_CLK_ENABLE();
    else if (ctx->port == GPIOD) __HAL_RCC_GPIOD_CLK_ENABLE();
    else if (ctx->port == GPIOF) __HAL_RCC_GPIOF_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = ctx->pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

    HAL_GPIO_Init(ctx->port, &GPIO_InitStruct);
}

void gpio_output_write(Device *dev, const void *value)
{
    GPIOOutputContext *ctx = (GPIOOutputContext *)dev->context;
    HAL_GPIO_WritePin(ctx->port, ctx->pin, *(uint8_t *)value);
}

void GPIOOutput_Create(Device *dev, GPIO_TypeDef *port, uint16_t pin)
{
    GPIOOutputContext *ctx = malloc(sizeof(GPIOOutputContext));
    ctx->port = port;
    ctx->pin = pin;

    dev->init = gpio_output_init;
    dev->read = NULL;
    dev->write = gpio_output_write;
    dev->context = ctx;
    dev->direction = IO_DIR_OUTPUT;
}