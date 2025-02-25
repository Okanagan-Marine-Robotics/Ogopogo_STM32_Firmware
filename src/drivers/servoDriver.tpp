#include "servoDriver.h"

template <uint8_t PIN>
SERVO<PIN>::SERVO()
{
    servo.attach(PIN);
}
template <uint8_t PIN>
void SERVO<PIN>::write(uint8_t angle)
{
    servo.write(angle);
}
