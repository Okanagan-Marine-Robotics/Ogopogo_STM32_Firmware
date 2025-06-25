#include "bme280_io.h"
#include "stm32g0xx_hal.h"
#include <stdlib.h>
#include <string.h>

void BME280_Device_Create(Device *dev, SPI_HandleTypeDef *hspi, GPIO_TypeDef *cs_port, uint16_t cs_pin) {
    // Allocate context
    BME280Context *ctx = (BME280Context *)malloc(sizeof(BME280Context));
    if (!ctx) return;
    
    // Initialize context
    memset(ctx, 0, sizeof(BME280Context));
    ctx->hspi = hspi;
    ctx->cs_port = cs_port;
    ctx->cs_pin = cs_pin;
    ctx->min_read_interval = 100; // Default 100ms minimum interval
    
    // Initialize device structure
    dev->context = ctx;
    dev->init = bme280_device_init;
    dev->read = bme280_device_read;
    dev->write = bme280_device_write;
    dev->direction = IO_DIR_INPUT;
    dev->name = "BME280";
}

void bme280_device_init(Device *dev) {
    BME280Context *ctx = (BME280Context *)dev->context;
    if (!ctx) return;
    
    // Initialize CS pin
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = ctx->cs_pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(ctx->cs_port, &GPIO_InitStruct);
    
    // Set CS high initially
    HAL_GPIO_WritePin(ctx->cs_port, ctx->cs_pin, GPIO_PIN_SET);
    
    // Initialize BME280 device structure
    ctx->bme.intf = BME280_SPI_INTF;
    ctx->bme.read = user_spi_read;
    ctx->bme.write = user_spi_write;
    ctx->bme.delay_us = user_delay_us;
    ctx->bme.intf_ptr = ctx; // Pass context as interface pointer
    
    // Initialize the BME280
    int8_t rslt = bme280_init(&ctx->bme);
    if (rslt == BME280_OK) {
        // Configure sensor with default settings
        BME280_ConfigureSensor(ctx, BME280_POWERMODE_NORMAL, 
                              BME280_OVERSAMPLING_1X,  // Temperature oversampling
                              BME280_OVERSAMPLING_1X,  // Humidity oversampling  
                              BME280_OVERSAMPLING_1X); // Pressure oversampling
        ctx->initialized = 1;
    }

    // set device to sleep mode initially
    // rslt = bme280_set_sensor_mode(BME280_POWERMODE_SLEEP, &ctx->bme);

    
}

void bme280_device_read(Device *dev, void *value) {
    BME280Context *ctx = (BME280Context *)dev->context;
    if (!ctx || !ctx->initialized || !value) return;
    
    uint32_t current_time = HAL_GetTick();
    
    // Check if enough time has passed since last read
    if (current_time - ctx->last_read_time >= ctx->min_read_interval) {
        if (BME280_ReadSensorData(ctx) == HAL_OK) {
            BME280_ProcessRawData(ctx);
            ctx->last_read_time = current_time;
        }
    }
    
    // Copy the reading to the output
    memcpy(value, &ctx->reading, sizeof(BME280_Reading));
}

void bme280_device_write(Device *dev, const void *value) {
    BME280Context *ctx = (BME280Context *)dev->context;
    if (!ctx || !value) return;
    
    // This could be used for configuration changes
    // For example, changing the minimum read interval
    uint32_t *interval = (uint32_t *)value;
    if (*interval >= 10 && *interval <= 10000) { // Reasonable bounds
        ctx->min_read_interval = *interval;
    }
}

HAL_StatusTypeDef BME280_ReadSensorData(BME280Context *ctx) {
    // Set sensor to forced mode for single measurement
    int8_t rslt = bme280_set_sensor_mode(BME280_POWERMODE_FORCED, &ctx->bme);
    if (rslt != BME280_OK) return HAL_ERROR;
    
    // Wait for measurement to complete
    ctx->bme.delay_us(50000, ctx->bme.intf_ptr); // 50ms delay
    
    // Read the sensor data
    rslt = bme280_get_sensor_data(BME280_ALL, &ctx->raw_data, &ctx->bme);
    if (rslt != BME280_OK) return HAL_ERROR;

    // // put sensor into low power mode after reading to prevent self-heating
    // rslt = bme280_set_sensor_mode(BME280_POWERMODE_SLEEP, &ctx->bme);
    // if (rslt != BME280_OK) return HAL_ERROR;
    
    
    return HAL_OK;
}

void BME280_ProcessRawData(BME280Context *ctx) {
    // Convert raw data to floating point values
    // #ifdef BME280_FLOAT_ENABLE
    // If floating point is enabled in BME280 library
    ctx->reading.temperature = ctx->raw_data.temperature;
    ctx->reading.humidity = ctx->raw_data.humidity;
    ctx->reading.pressure = ctx->raw_data.pressure / 100.0f; // Convert Pa to hPa
    // #else
    // // Convert from fixed point to floating point
    // ctx->reading.temperature = (float)ctx->raw_data.temperature / 100.0f;
    // ctx->reading.humidity = (float)ctx->raw_data.humidity / 1024.0f;
    // ctx->reading.pressure = (float)ctx->raw_data.pressure / 25600.0f; // Convert to hPa
    // #endif
    
    ctx->reading.valid = 1;
}

HAL_StatusTypeDef BME280_ConfigureSensor(BME280Context *ctx, uint8_t mode, uint8_t temp_os, uint8_t hum_os, uint8_t press_os) {
    struct bme280_settings settings;
    
    // Configure oversampling settings
    settings.osr_t = temp_os;      // Temperature oversampling
    settings.osr_p = press_os;     // Pressure oversampling  
    settings.osr_h = hum_os;       // Humidity oversampling
    
    // Apply the settings to the sensor
    int8_t rslt = bme280_set_sensor_settings(BME280_SEL_ALL_SETTINGS, &settings, &ctx->bme);
    if (rslt != BME280_OK) return HAL_ERROR;
    
    // Set sensor mode
    rslt = bme280_set_sensor_mode(mode, &ctx->bme);
    if (rslt != BME280_OK) return HAL_ERROR;
    
    return HAL_OK;
}

// SPI Communication Functions
int8_t user_spi_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr) {
    BME280Context *ctx = (BME280Context *)intf_ptr;
    if (!ctx) return -1;
    
    uint8_t tx_buf[SPI_BUFFER_LEN] = {0};
    uint8_t rx_buf[SPI_BUFFER_LEN] = {0};
    
    // Prepare read command (MSB = 1 for read)
    tx_buf[0] = reg_addr | 0x80;
    
    // Pull CS low
    HAL_GPIO_WritePin(ctx->cs_port, ctx->cs_pin, GPIO_PIN_RESET);
    
    // Transmit and receive
    HAL_StatusTypeDef status = HAL_SPI_TransmitReceive(ctx->hspi, tx_buf, rx_buf, len + 1, HAL_MAX_DELAY);
    
    // Wait for completion
    while (ctx->hspi->State == HAL_SPI_STATE_BUSY);
    
    // Pull CS high
    HAL_GPIO_WritePin(ctx->cs_port, ctx->cs_pin, GPIO_PIN_SET);
    
    // Copy received data (skip first byte which is dummy)
    for (uint32_t i = 0; i < len; i++) {
        reg_data[i] = rx_buf[i + 1];
    }
    
    return (status == HAL_OK) ? 0 : -1;
}

int8_t user_spi_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, void *intf_ptr) {
    BME280Context *ctx = (BME280Context *)intf_ptr;
    if (!ctx) return -1;
    
    uint8_t tx_buf[SPI_BUFFER_LEN] = {0};
    
    // Prepare write command (MSB = 0 for write)
    tx_buf[0] = reg_addr & 0x7F;
    
    // Copy data to transmit buffer
    for (uint32_t i = 0; i < len; i++) {
        tx_buf[i + 1] = reg_data[i];
    }
    
    // Pull CS low
    HAL_GPIO_WritePin(ctx->cs_port, ctx->cs_pin, GPIO_PIN_RESET);
    
    // Transmit data
    HAL_StatusTypeDef status = HAL_SPI_Transmit(ctx->hspi, tx_buf, len + 1, HAL_MAX_DELAY);
    
    // Wait for completion
    while (ctx->hspi->State == HAL_SPI_STATE_BUSY);
    
    // Pull CS high
    HAL_GPIO_WritePin(ctx->cs_port, ctx->cs_pin, GPIO_PIN_SET);
    
    return (status == HAL_OK) ? 0 : -1;
}

void user_delay_us(uint32_t period, void *intf_ptr) {
    // Convert microseconds to milliseconds (crude but functional)
    uint32_t delay_ms = (period + 999) / 1000; // Round up
    if (delay_ms == 0) delay_ms = 1;
    HAL_Delay(delay_ms);
}