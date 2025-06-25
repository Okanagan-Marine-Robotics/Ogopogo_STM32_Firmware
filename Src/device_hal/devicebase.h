#pragma once
#include "stm32g0xx_hal.h"

typedef enum
{
    IO_DIR_INPUT,
    IO_DIR_OUTPUT,
    IO_DIR_BIDIR
} IODirection;

typedef struct Device
{
    void (*init)(struct Device *self);
    void (*read)(struct Device *self, void *value);
    void (*write)(struct Device *self, const void *value);
    void *context;
    IODirection direction;
    const char *name; // Optional: for debugging or lookup
} Device;

typedef enum
{
    DEVICE_TYPE_GPIO_INPUT,
    DEVICE_TYPE_GPIO_OUTPUT,
    DEVICE_TYPE_BME280,
    // DEVICE_TYPE_WS2812,
    // DEVICE_TYPE_SERVO,
    DEVICE_TYPE_CURRENT_SENSOR,
    // Add more types
} DeviceType;

typedef struct
{
    DeviceType type;
    const char *name;
    union
    {
        struct
        {
            GPIO_TypeDef *port;
            uint16_t pin;
            uint8_t pull; // Optional: for pull-up/pull-down configuration
        } gpio;
        struct
        {
            SPI_HandleTypeDef *hspi;
            GPIO_TypeDef *cs_port; // Chip select port
            uint16_t cs_pin;       // Chip select pin
        } bme280;
        struct
        {
            TIM_HandleTypeDef *htim;
            uint32_t channel;
        } pwm;
        struct
        {
            GPIO_TypeDef *port;
            uint16_t pin;
            uint16_t count;
        } ws2812;
        struct
        {
            GPIO_TypeDef *port;
            uint16_t pin;
        } analog;
        // Add more config structs
    };
} DeviceConfig;