#pragma once
#include "stm32g0xx.h"
#include "device_hal/devicebase.h"

typedef struct {
    ADC_HandleTypeDef *hadc;
    uint32_t channel;
    float vref;
    float sensitivity; // e.g., 0.2 for 200mV/A
    float zero_offset; // in volts (typically vref / 2)
} CurrentSensorContext;

void current_sens_read(Device *dev, void *value);
void current_sens_init(Device *dev);
void CurrentSensor_Create(Device *dev, ADC_HandleTypeDef *hadc, uint32_t channel, float vref, float sensitivity, float zero_offset);