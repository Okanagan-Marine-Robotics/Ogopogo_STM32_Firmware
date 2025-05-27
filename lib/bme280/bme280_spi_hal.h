#include "stm32g0xx_hal.h"

#define SPI_BUFFER_LEN 28
#define SPI_READ 0x80
#define BME280_DATA_INDEX 1

void BME280_Init();
void Read_BME280(float *temp, float *hum, float *press);
