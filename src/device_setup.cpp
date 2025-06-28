#include "device_setup.h"

void gpioSetup()
{
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
}