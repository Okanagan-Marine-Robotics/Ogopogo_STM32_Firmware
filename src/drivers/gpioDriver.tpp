#include "gpioDriver.h"

template <uint8_t PIN>
GPIO<PIN>::GPIO()
{
    pinMode(PIN, INPUT_PULLUP);
}

template <uint8_t PIN>
void GPIO<PIN>::write(uint8_t state)
{
    digitalWrite(PIN, state);
}

template <uint8_t PIN>
float GPIO<PIN>::read()
{
    return digitalRead(PIN);
}