#pragma once
#include "stm32g0xx.h"
#include "device_hal/devicebase.h"
#include "bme280_defs.h"
#include "bme280.h"

#define SPI_BUFFER_LEN 28
#define SPI_READ 0x80
#define BME280_DATA_INDEX 1

// Structure to hold BME280 sensor readings
typedef struct {
    float temperature;  // Temperature in Celsius
    float humidity;     // Humidity in %RH
    float pressure;     // Pressure in hPa
    uint8_t valid;      // Flag to indicate if data is valid
} BME280_Reading;

// BME280 device context
typedef struct {
    SPI_HandleTypeDef *hspi;     // SPI handle for communication
    GPIO_TypeDef *cs_port;       // Chip select port
    uint16_t cs_pin;             // Chip select pin
    struct bme280_dev bme;       // BME280 device structure
    struct bme280_data raw_data; // Raw BME280 data structure
    BME280_Reading reading;      // Processed reading data
    uint8_t initialized;         // Initialization flag
    uint32_t last_read_time;     // Last read timestamp (for caching)
    uint32_t min_read_interval;  // Minimum interval between reads (ms)
} BME280Context;

// Function prototypes
void BME280_Device_Create(Device *dev, SPI_HandleTypeDef *hspi, GPIO_TypeDef *cs_port, uint16_t cs_pin);
void bme280_device_init(Device *dev);
void bme280_device_read(Device *dev, void *value);
void bme280_device_write(Device *dev, const void *value); // For configuration

// SPI communication functions
int8_t user_spi_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr);
int8_t user_spi_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, void *intf_ptr);
void user_delay_us(uint32_t period, void *intf_ptr);

// Utility functions
HAL_StatusTypeDef BME280_ReadSensorData(BME280Context *ctx);
void BME280_ProcessRawData(BME280Context *ctx);
HAL_StatusTypeDef BME280_ConfigureSensor(BME280Context *ctx, uint8_t mode, uint8_t temp_os, uint8_t hum_os, uint8_t press_os);
