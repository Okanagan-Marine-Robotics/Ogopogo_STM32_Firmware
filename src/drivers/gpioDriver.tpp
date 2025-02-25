#include "gpioDriver.h"

template <uint8_t PIN, uint8_t MODE, bool ANALOG, bool INVERTING>
GPIO<PIN, MODE, ANALOG, INVERTING>::GPIO()
{
    pinMode(PIN, MODE);
}

template <uint8_t PIN, uint8_t MODE, bool ANALOG, bool INVERTING>
void GPIO<PIN, MODE, ANALOG, INVERTING>::write(uint8_t state)
{
    digitalWrite(PIN, state);
}

template <uint8_t PIN, uint8_t MODE, bool ANALOG, bool INVERTING>
float GPIO<PIN, MODE, ANALOG, INVERTING>::read()
{
    static_assert(!(INVERTING && ANALOG), "Invalid configuration: INVERTING and ANALOG cannot both be true");

    if constexpr (ANALOG)
    {
        return analogRead(PIN);
    }
    else
    {
        return INVERTING ? !digitalRead(PIN) : digitalRead(PIN);
    }
}