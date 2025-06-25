#include "gpio_input.h"
#include <stdlib.h>

void gpio_input_init(Device *dev) {
    GPIOInputContext *ctx = (GPIOInputContext *)dev->context;

    // Enable GPIO clock for the port
    if (ctx->port == GPIOA) __HAL_RCC_GPIOA_CLK_ENABLE();
    else if (ctx->port == GPIOB) __HAL_RCC_GPIOB_CLK_ENABLE();
    else if (ctx->port == GPIOC) __HAL_RCC_GPIOC_CLK_ENABLE();
    else if (ctx->port == GPIOD) __HAL_RCC_GPIOD_CLK_ENABLE();
    else if (ctx->port == GPIOF) __HAL_RCC_GPIOF_CLK_ENABLE();
    // Add more ports if needed

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = ctx->pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = (ctx->pull == GPIO_PULL_UP) ? GPIO_PULLUP :
                           (ctx->pull == GPIO_PULL_DOWN) ? GPIO_PULLDOWN : GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;  // Speed not crucial for inputs

    HAL_GPIO_Init(ctx->port, &GPIO_InitStruct);
}

void gpio_input_read(Device *dev, void *value) {
    GPIOInputContext *ctx = (GPIOInputContext *)dev->context;
    *(uint8_t *)value = HAL_GPIO_ReadPin(ctx->port, ctx->pin);
}

void GPIOInput_Create(Device *dev, GPIO_TypeDef *port, uint16_t pin, GPIOPullMode pull) {
    GPIOInputContext *ctx = malloc(sizeof(GPIOInputContext));
    ctx->port = port;
    ctx->pin = pin;
    ctx->pull = pull; // Set the pull mode

    dev->init = NULL; // Or write an init if needed
    dev->read = gpio_input_read;
    dev->write = NULL;
    dev->context = ctx;
    dev->direction = IO_DIR_INPUT;
    dev->name = "ButtonA";
}