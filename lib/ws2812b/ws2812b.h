#include "stm32g0xx_hal.h"

#define MAX_LED 1
#define USE_BRIGHTNESS 0

#define PI 3.14159265

void Set_LED(int LEDnum, int Red, int Green, int Blue);
void Set_Brightness(int brightness);
void WS2812_Send(void);
