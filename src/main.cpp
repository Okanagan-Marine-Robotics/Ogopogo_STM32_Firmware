#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_BME280.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_PWMServoDriver.h>
#include <Wire.h>
#include <configuration.h>
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

    // wait for BME280 to be ready
    if (!bme.begin())
    {
        while (1)
            ;
    }

// Initialize Output GPIO
#if NUM_OUTPUT_PINS > 0
    for (uint8_t pin : OUTPUT_PINS)
    {
        pinMode(pin, OUTPUT);
    }
#endif

// configure digital input pins
#if NUM_INPUT_PINS > 0
    for (uint8_t pin : INPUT_DIGITAL_PINS)
    {
        pinMode(pin, INPUT);
    }
#endif

// configure analog input pins
#if NUM_ANALOG_INPUT_PINS > 0
    for (uint8_t pin : INPUT_ANALOG_PINS)
    {
        pinMode(pin, INPUT_ANALOG);
    }
#endif

#if NUM_SERVOS > 0
    // Initialize Servo pins if defined
    for (uint8_t pin : SERVO_PINS)
    {
        pinMode(pin, OUTPUT); // Assuming servo pins are used as outputs
    }
#endif

    i2cHandler.initializeI2C();
}

volatile float hum;
volatile float temp;
volatile float press;

volatile uint8_t ledColor[3] = {0, 0, 0}; // RGB values for the LED

void loop()
{
    hum = bme.readHumidity();
    temp = bme.readTemperature();
    press = bme.readPressure();

    // read analog value from PB1
    volatile int analogValue = analogRead(PB1);
    volatile int analogValue2 = analogRead(PA0);
    led.setPixelColor(0, led.Color(ledColor[0], ledColor[1], ledColor[2]));
    led.show();
}

// // Called when master sends 3 byte to this slave
// void receiveEvent(int numBytes)
// {
//     uint8_t rgb[3] = {0, 0, 0}; // RGB values

//     for (int i = 0; i < 3 && Wire.available(); ++i)
//     {
//         rgb[i] = Wire.read();
//     }

//     // Optionally store the last received byte
//     if (numBytes > 0)
//     {
//         received_byte = rgb[0];
//     }

//     // Set the LED color
//     led.setPixelColor(0, led.Color(rgb[0], rgb[1], rgb[2]));
//     led.show();

//     // Ignore any extra bytes
//     // while (Wire.available())
//     // {
//     //     Wire.read();
//     // }
//     // Wire.flush();
// }

// Called when master requests 1 byte from this slave
// void requestEvent()
// {
//     Wire.write(send_byte);
// }