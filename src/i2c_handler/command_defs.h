#define I2C_TX_BUFFER_SIZE 32

#define I2C_NO_OPERATION 0x00
#define I2C_NO_OPERATION_EXPECTED_BITS 0x01 // 0x00 command
#define I2C_WRITE_GPIO_OUTPUT 0x01
#define I2C_WRITE_GPIO_OUTPUT_EXPECTED_BITS 0x03 // 0x01, device address, data byte
#define I2C_READ_GPIO_INPUT 0x02
#define I2C_READ_GPIO_INPUT_EXPECTED_BITS 0x02 // 0x02, device address
#define I2C_READ_ANALOG_INPUT 0x03
#define I2C_READ_ANALOG_INPUT_EXPECTED_BITS 0x02 // 0x03, device address
#define I2C_READ_BME280 0x04
#define I2C_READ_BME280_EXPECTED_BITS 0x01 // 0x04 command
#define I2C_WRITE_LED 0x05
#define I2C_WRITE_LED_EXPECTED_BITS 0x04 // 0x05, red byte, green byte, blue byte
