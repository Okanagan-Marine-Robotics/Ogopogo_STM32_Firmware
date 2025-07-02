#include "i2c_handler.h"
#include "command_defs.h"
#include <Wire.h>
#include <Adafruit_BME280.h>

const uint8_t outputPinsArray[] = DIGITAL_OUTPUT_PINS;
const uint8_t inputAnalogArray[] = INPUT_ANALOG_PINS;

// Global variables for BME280 data
extern float hum;
extern float temp;
extern float press;

// external array for analog values
extern uint16_t analogValues[NUM_ANALOG_INPUT_PINS];

// Global variables for LED control
extern uint8_t ledColor[3]; // RGB values for each LED

void I2CHandler::initializeI2C()
{
    // Initialize I2C as slave
    Wire.begin(I2C_SLAVE_ADDRESS);

    // Register callback functions
    Wire.onReceive(I2CHandler::receiveEventStatic); // Called when master sends data
    Wire.onRequest(I2CHandler::requestEventStatic); // Called when master requests data
}

void I2CHandler::receiveEvent(int numBytes)
{
    // Handle I2C data reception
    if (numBytes > 0)
    {
        // Read the command byte
        uint8_t command = Wire.read();

        // Process the command
        switch (command)
        {
        case I2C_NO_OPERATION:
            // No operation command, just acknowledge
            txBytes = 1;
            lastResponseCode[0] = I2C_SLAVE_ADDRESS; // Default response code (success) this way it is novel and can be used to check if the slave is alive
            break;
        case I2C_WRITE_GPIO_OUTPUT:
            // Check if the expected number of bytes is correct
            // response should always be 1 byte for success or error code
            txBytes = 1;
            if (numBytes == I2C_WRITE_GPIO_OUTPUT_EXPECTED_BITS)
            {
                // Read remaining bytes
                uint8_t pinIndex = Wire.read();
                uint8_t value = Wire.read();

                // Validate value (should be 0 or 1, or HIGH/LOW)
                if (value != HIGH && value != LOW && value != 0 && value != 1)
                {
                    // Handle error: invalid value
                    lastResponseCode[0] = 0xFF; // Error code
                    break;
                }

                // Check if pinIndex is within bounds
                if (pinIndex < NUM_DIGITAL_OUTPUT_PINS)
                {
                    // Write the value to the specified pin
                    digitalWrite(outputPinsArray[pinIndex], value ? HIGH : LOW);
                    lastResponseCode[0] = 0x00; // Success code
                    break;
                }
                else
                {
                    // Handle error: pin index out of bounds
                    lastResponseCode[0] = 0xFF; // Error code
                    break;
                }
            }
            else
            {
                // Handle error: unexpected number of bytes
                lastResponseCode[0] = 0xFF; // Error code
            }
            break;
        case I2C_READ_GPIO_INPUT:
            // Check if the expected number of bytes is correct
            txBytes = 2; // 2 bytes for pin index and value
            if (numBytes == I2C_READ_GPIO_INPUT_EXPECTED_BITS)
            {
                // Read the pin index
                uint8_t pinIndex = Wire.read();

                // Validate pin index
                if (pinIndex < NUM_DIGITAL_INPUT_PINS)
                {
                    // Read the value from the specified pin
                    uint8_t value = digitalRead(outputPinsArray[pinIndex]);
                    lastResponseCode[0] = pinIndex; // First byte is pin index
                    lastResponseCode[1] = value;    // Second byte is value
                }
                else
                {
                    // Handle error: pin index out of bounds
                    lastResponseCode[0] = 0xFF; // Error code
                    lastResponseCode[1] = 0x00; // Default value
                }
            }
            else
            {
                // Handle error: unexpected number of bytes
                lastResponseCode[0] = 0xFF; // Error code
                lastResponseCode[1] = 0x00; // Default value
            }
            break;
        case I2C_READ_ANALOG_INPUT:
            // Check if the expected number of bytes is correct
            txBytes = 2; // 2 bytes for analog value
            if (numBytes == I2C_READ_ANALOG_INPUT_EXPECTED_BITS)
            {
                // Read the pin index
                uint8_t pinIndex = Wire.read();

                // Validate pin index
                if (pinIndex < NUM_ANALOG_INPUT_PINS)
                {
// Read the analog value from the specified pin
#if NUM_ANALOG_INPUT_PINS > 0
                    lastResponseCode[0] = (analogValues[pinIndex] >> 8) & 0xFF; // High byte
                    lastResponseCode[1] = analogValues[pinIndex] & 0xFF;        // Low byte
#else
                    {
                        // Handle error: no analog pins defined
                        lastResponseCode[0] = 0xFF; // Error code
                        lastResponseCode[1] = 0x00; // Default value
                    }
#endif
                }
                else
                {
                    // Handle error: pin index out of bounds
                    lastResponseCode[0] = 0xFF; // Error code
                    lastResponseCode[1] = 0x00; // Default value
                }
            }
            else
            {
                // Handle error: unexpected number of bytes
                lastResponseCode[0] = 0xFF; // Error code
                lastResponseCode[1] = 0x00; // Default value
            }
            break;
        case I2C_READ_BME280:
            txBytes = 12; // 12 bytes for BME280 data (3 floats: humidity, temperature, pressure)
            if (numBytes == I2C_READ_BME280_EXPECTED_BITS)
            {
                // Prepare the response
                memcpy((void *)lastResponseCode, &hum, sizeof(hum));           // Humidity
                memcpy((void *)(lastResponseCode + 4), &temp, sizeof(temp));   // Temperature
                memcpy((void *)(lastResponseCode + 8), &press, sizeof(press)); // Pressure
            }
            else
            {
                // Handle error: unexpected number of bytes
                txBytes = 1;                // Only send error code
                lastResponseCode[0] = 0xFF; // Error code
            }
            break;
        case I2C_WRITE_LED:
            // Check if the expected number of bytes is correct
            txBytes = 1; // 1 byte for response code
            if (numBytes == I2C_WRITE_LED_EXPECTED_BITS)
            {
                // Read RGB values
                // update the LED color
                ledColor[0] = Wire.read(); // Red
                ledColor[1] = Wire.read(); // Green
                ledColor[2] = Wire.read(); // Blue

                lastResponseCode[0] = 0x00; // Success code
            }
            else
            {
                // Handle error: unexpected number of bytes
                lastResponseCode[0] = 0xFF; // Error code
            }
            break;
        case I2C_DISCOVER_DEVICES:
            // Check if the expected number of bytes is correct
            txBytes = 1; // 1 byte for response code
            if (numBytes == I2C_DISCOVER_DEVICES_EXPECTED_BITS)
            {
                // respond with the different devices and how many there are
                // we will do 0x01 for GPIO, 0x02 for Analog, 0x03 for BME280, and 0x04 for LED
                // then after the address we will send the number of devices
                lastResponseCode[0] = 0x01;                    // GPIO output device
                lastResponseCode[1] = NUM_DIGITAL_OUTPUT_PINS; // Number of GPIO output pins
                lastResponseCode[2] = 0x02;                    // GPIO input device
                lastResponseCode[3] = NUM_DIGITAL_INPUT_PINS;  // Number of GPIO input pins
                lastResponseCode[4] = 0x03;                    // Analog input device
                lastResponseCode[5] = NUM_ANALOG_INPUT_PINS;   // Number of Analog input pins
                lastResponseCode[6] = 0x04;                    // BME280 device
                lastResponseCode[7] = 0x01;                    // Number of BME280 devices
                lastResponseCode[8] = 0x05;                    // LED device
                lastResponseCode[9] = LED_COUNT;               // Number of LEDs
                txBytes = 10;                                  // Total bytes sent in response
            }
            else
            {
                // Handle error: unexpected number of bytes
                lastResponseCode[0] = 0xFF; // Error code
            }
            break;
        default:
            // Handle unknown command
            lastResponseCode[0] = 0xFF; // Error code
            // Read and discard any remaining bytes
            while (Wire.available())
            {
                Wire.read();
            }
            break;
        }

        // Clear any remaining bytes that weren't processed
        while (Wire.available())
        {
            Wire.read();
        }
    }
}

void I2CHandler::requestEvent()
{
    // Send the response code when master requests data
    for (uint8_t i = 0; i < txBytes; ++i)
    {
        Wire.write(lastResponseCode[i]);
    }
}