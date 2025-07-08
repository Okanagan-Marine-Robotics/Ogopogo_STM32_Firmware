#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_BME280.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_PWMServoDriver.h>
#include <Wire.h>
#include <configuration.h>
#include "device_setup.h"
#include <i2c_handler/i2c_handler.h>

#if LED_COUNT > 0
Adafruit_NeoPixel led(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
#endif

// Create custom SPI instance
SPIClass mySPI(BME_MOSI, BME_MISO, BME_SCK);
Adafruit_BME280 bme(BME_CS, &mySPI);

// Create an instance of the I2C handler
I2CHandler i2cHandler;

void setup()
{
#if LED_COUNT > 0
    GPIOA->OSPEEDR |= (0b11 << (LED_PIN * 2));
    led.begin();
    led.show();
    led.clear();
#endif

    gpioSetup(); // Initialize GPIO pins

    // wait for BME280 to be ready
    if (!bme.begin())
    {
        while (1)
            ;
    }
#if NUM_SERVOS > 0
    // Initialize Servo pins if defined
    for (uint8_t pin : SERVO_PINS)
    {
        pinMode(pin, OUTPUT); // Assuming servo pins are used as outputs
    }
#endif

    i2cHandler.initializeI2C();

    // Setup succeeded quickly flash led green 3 times
#if LED_COUNT > 0
    for (int i = 0; i < 3; i++)
    {
        led.setPixelColor(0, 0, 75, 0);
        led.show();
        delay(50);
        led.clear();
        led.show();
        delay(50);
    }
#endif
}

// these get read from the interrupt handler
volatile float hum;
volatile float temp;
volatile float press;

volatile uint8_t ledColor[LED_COUNT][3] = {};                               // RGB values for the LED
volatile uint16_t analogValues[NUM_ANALOG_INPUT_PINS] = {0};                // Array to hold analog values
const uint8_t input_analog_pins[NUM_ANALOG_INPUT_PINS] = INPUT_ANALOG_PINS; // Analog input pins

void loop()
{
    hum = bme.readHumidity();
    temp = bme.readTemperature();
    press = bme.readPressure();

#if NUM_ANALOG_INPUT_PINS > 0
    for (uint8_t i = 0; i < NUM_ANALOG_INPUT_PINS; i++)
    {
        analogValues[i] = analogRead(input_analog_pins[i]);
    }
#endif

#if LED_COUNT > 0
    for (uint8_t i = 0; i < LED_COUNT; i++)
    {
        // Set the LED color based on the ledColor array
        led.setPixelColor(i, led.Color(ledColor[i][0], ledColor[i][1], ledColor[i][2]));
    }
    led.show();
#endif
}