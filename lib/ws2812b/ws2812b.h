#include "stm32g0xx_hal.h"

#define MAX_LED 1

void Set_LED(int LEDnum, int Red, int Green, int Blue);
void WS2812_Send(void);
