#include "bme280_spi_hal.h"
#include "spi.h"
#include "gpio.h"
#include "bme280_defs.h"
#include "bme280.h"

int8_t user_spi_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, struct bme280_dev *dev)
{
    HAL_StatusTypeDef status = HAL_OK;
    uint8_t iError = BME280_OK;
    uint8_t txarray[SPI_BUFFER_LEN] = {
        0,
    };
    uint8_t rxarray[SPI_BUFFER_LEN] = {
        0,
    };
    uint8_t stringpos = 0;

    /*	For the SPI mode only 7 bits of register addresses are used.
    The MSB of register address is declared the bit what functionality it is
    read/write (read as 1/write as BME280_INIT_VALUE)*/
    // txarray[BME280_OK] = reg_addr | SPI_READ; /*read routine is initiated register address is mask with 0x80*/ should be done in bme280 sensor api
    txarray[0] = reg_addr;

    for (int i = 0; i < len - 1; i++)
    {
        txarray[i + 1] = txarray[i] + 0x01; // Increment the address for each byte read
    }
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET); // CS LOW

    status = HAL_SPI_TransmitReceive(&hspi1, (uint8_t *)(&txarray), (uint8_t *)(&rxarray), len + 1, HAL_MAX_DELAY);
    while (hspi1.State == HAL_SPI_STATE_BUSY)
    {
    };

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET); // CS HIGH

    for (stringpos = BME280_OK; stringpos < len; stringpos++)
    {
        *(reg_data + stringpos) = rxarray[stringpos + BME280_DATA_INDEX];
    }

    if (status != HAL_OK)
    {
        // The BME280 API calls for 0 return value as a success, and -1 returned as failure
        iError = (-1);
    }
    return (uint8_t)iError;
}

int8_t user_spi_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, void *intf_ptr)
{

    uint8_t txarray[SPI_BUFFER_LEN] = {
        0,
    };
    txarray[0] = reg_addr;
    for (uint32_t i = 0; i < len; i++)
    {
        if (i % 2 == 0)
            txarray[i + 1] = reg_addr + (i / 2) + 1;
        else
            txarray[i + 1] = reg_data[i];
    }

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET); // CS LOW
    HAL_StatusTypeDef status = HAL_SPI_Transmit(&hspi1, txarray, len + 1, HAL_MAX_DELAY);
    while (hspi1.State == HAL_SPI_STATE_BUSY)
    {
    };

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET); // CS HIGH
    return 0;
}

void user_delay_us(uint32_t period, void *intf_ptr)
{
    HAL_Delay(period / 1000); // crude delay in ms
}

struct bme280_dev bme;
struct bme280_data comp_data;

void BME280_Init()
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET); // CS HIGH

    bme.intf = BME280_SPI_INTF;
    bme.read = user_spi_read;
    bme.write = user_spi_write;
    bme.delay_us = user_delay_us;
    bme.intf_ptr = NULL;

    int8_t rslt = bme280_init(&bme);
    if (rslt != BME280_OK)
    {
        // Handle error
        while (1)
        {
            /* code */
        }
    }

    // Configure sensor
    // bme.settings.osr_h = BME280_OVERSAMPLING_1X;
    // bme.settings.osr_p = BME280_OVERSAMPLING_1X;
    // bme.settings.osr_t = BME280_OVERSAMPLING_1X;
    // bme.settings.filter = BME280_FILTER_COEFF_OFF;

    // rslt = bme280_set_sensor_settings(BME280_SEL_ALL_SETTINGS, &bme);
    // rslt = bme280_set_sensor_mode(BME280_POWERMODE_FORCED, &bme);
}

void Read_BME280(float *temp, float *hum, float *press)
{
    bme280_set_sensor_mode(BME280_POWERMODE_FORCED, &bme);
    bme.delay_us(50000, NULL); // 50ms delay for measurement

    bme280_get_sensor_data(BME280_ALL, &comp_data, &bme);
    // uint8_t temp_raw[2] = {0};
    // bme280_get_regs(0xFA, temp_raw, 2, &bme); // Read temperature raw data

    *temp = comp_data.temperature;
    *hum = comp_data.humidity;
    *press = comp_data.pressure;

    // Now you can use temp/hum/press
}