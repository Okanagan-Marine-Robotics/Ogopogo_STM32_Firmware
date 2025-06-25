#include "current_sens.h"
#include <stdlib.h>
#include "stm32g0xx.h"

#define VREFINT_CAL_ADDR  ((uint16_t*) ((uint32_t)0x1FFF75AA))
#define VREFINT_CAL_VOLTAGE 3.0f

// For STM32G0 series, ADC_RANK_CHANNEL_NUMBER is not valid. Use ADC_REGULAR_RANK_1.
// Also, VREFINT_CAL_VOLTAGE is 3.0V only if VDDA during calibration was 3.0V (check RM0444).
// The VREFINT_CAL value is a 12-bit value stored at VREFINT_CAL_ADDR.

static float get_vrefint(ADC_HandleTypeDef *hadc) {
    ADC_ChannelConfTypeDef sConfig = {0};
    sConfig.Channel = ADC_CHANNEL_VREFINT;
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_12CYCLES_5;
    HAL_ADC_ConfigChannel(hadc, &sConfig);

    HAL_ADC_Start(hadc);
    HAL_ADC_PollForConversion(hadc, HAL_MAX_DELAY);
    uint16_t val = HAL_ADC_GetValue(hadc);
    HAL_ADC_Stop(hadc);

    uint16_t vrefint_cal = *VREFINT_CAL_ADDR;
    // VDDA = 3.0V * VREFINT_CAL / measured VREFINT
    return 3.0f * ((float)vrefint_cal / (float)val);
}

void current_sensor_init(Device *dev) {
    CurrentSensorContext *ctx = (CurrentSensorContext *)dev->context;
    HAL_ADCEx_Calibration_Start(ctx->hadc);
}

void current_sens_read(Device *dev, void *value) {
    CurrentSensorContext *ctx = (CurrentSensorContext *)dev->context;

    ADC_ChannelConfTypeDef sConfig = {0};
    sConfig.Channel = ADC_CHANNEL_9;
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_12CYCLES_5; // Adjust as needed
    HAL_ADC_ConfigChannel(ctx->hadc, &sConfig);

    HAL_ADC_Start(ctx->hadc);
    HAL_ADC_PollForConversion(ctx->hadc, HAL_MAX_DELAY);
    uint32_t adc_val = HAL_ADC_GetValue(ctx->hadc);
    HAL_ADC_Stop(ctx->hadc);

    float voltage = ((float)adc_val / 4095.0f) * get_vrefint(ctx->hadc); // Assuming 12-bit ADC resolution
    float current = (voltage - ctx->zero_offset) / ctx->sensitivity;

    *((float *)value) = current;
}

void CurrentSensor_Create(Device *dev, ADC_HandleTypeDef *hadc, uint32_t channel, float vref, float sensitivity, float zero_offset) {
    CurrentSensorContext *ctx = malloc(sizeof(CurrentSensorContext));
    ctx->hadc = hadc;
    ctx->channel = channel;
    ctx->vref = vref;
    ctx->sensitivity = sensitivity;
    ctx->zero_offset = zero_offset;

    dev->init = NULL;
    dev->read = current_sens_read;
    dev->write = NULL;
    dev->context = ctx;
    dev->direction = IO_DIR_INPUT;
    dev->name = "CurrentSensor";
}