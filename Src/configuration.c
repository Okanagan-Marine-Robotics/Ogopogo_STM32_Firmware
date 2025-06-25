#include "configuration.h"


const DeviceConfig device_configs[] = {
    {DEVICE_TYPE_GPIO_INPUT, "button1", { .gpio = {GPIOA, GPIO_PIN_0, GPIO_NOPULL} }},
    {DEVICE_TYPE_GPIO_OUTPUT, "led1", { .gpio = {GPIOB, GPIO_PIN_5} }},
    {DEVICE_TYPE_GPIO_OUTPUT, "led2", { .gpio = {GPIOB, GPIO_PIN_6} }},
    {DEVICE_TYPE_GPIO_OUTPUT, "led3", { .gpio = {GPIOB, GPIO_PIN_7} }},
    {DEVICE_TYPE_GPIO_OUTPUT, "led4", { .gpio = {GPIOB, GPIO_PIN_8} }},
    
    // BME280 sensor configuration
    {DEVICE_TYPE_BME280, "bme280_sensor", { .bme280 = {&hspi1, GPIOA, GPIO_PIN_4} }},

    {DEVICE_TYPE_CURRENT_SENSOR, "current_sensor", { .analog = {GPIOB, GPIO_PIN_1} }},
    
    // Additional device configurations can be added here
    // {DEVICE_TYPE_SERVO, "servo1", { .pwm = {&htim2, TIM_CHANNEL_1} }},
};

size_t get_num_devices(void) {
    return sizeof(device_configs) / sizeof(device_configs[0]);
}