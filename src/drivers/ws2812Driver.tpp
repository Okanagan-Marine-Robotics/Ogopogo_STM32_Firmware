#include "ws2812Driver.h"

template <uint8_t PIN, uint16_t NUM_LEDS>
WS2812<PIN, NUM_LEDS>::WS2812()
{
}

template <uint8_t PIN, uint16_t NUM_LEDS>
void WS2812<PIN, NUM_LEDS>::write(uint8_t brightness)
{
}

template <uint8_t PIN, uint16_t NUM_LEDS>
void WS2812<PIN, NUM_LEDS>::setColor(uint8_t index, uint8_t r, uint8_t g, uint8_t b)
{
}

// TODO: Implement ws2812 driver for an STM32G030K8 which fastled does not support
// instead we can use the library called Adafruit_NeoPixel
