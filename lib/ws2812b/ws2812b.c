#include "ws2812b.h"
#include "stm32g0xx_hal_tim.h"

static uint8_t LED_Data[MAX_LED][4];
volatile uint8_t LED_Mod[MAX_LED][4]; // for brightness
volatile uint8_t datasentflag = 0;

extern TIM_HandleTypeDef htim3;

void Set_LED(int LEDnum, int Red, int Green, int Blue)
{
    LED_Data[LEDnum][0] = LEDnum;
    LED_Data[LEDnum][1] = Green;
    LED_Data[LEDnum][2] = Red;
    LED_Data[LEDnum][3] = Blue;
    WS2812_Send(); // Send data immediately
}

volatile uint16_t pwmData[(24 * MAX_LED + 64)];
void WS2812_Send(void)
{
    uint32_t indx = 0;
    uint32_t color;

    for (int i = 0; i < MAX_LED; i++)
    {
        // Read volatile data into local variables to ensure fresh reads
        uint8_t green = LED_Data[i][1];
        uint8_t red = LED_Data[i][2]; 
        uint8_t blue = LED_Data[i][3];
        
        color = ((green << 16) | (red << 8) | blue);
        
        for (int bit = 23; bit >= 0; bit--)
        {
            if (color & (1 << bit))
                pwmData[indx] = 55;
            else
                pwmData[indx] = 21;
            indx++;
        }
    }
    
    // Add reset signal
    for (int i = 24*MAX_LED; i < (64 + 24*MAX_LED); i++)
    {
        pwmData[i] = 0; // Reset signal
        indx++;
    }
    
    datasentflag = 0;
    __HAL_TIM_SET_COUNTER(&htim3, 0); // Reset the counter to ensure it starts from 0
    HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_1, (uint32_t *)pwmData, indx);
    while (!datasentflag) {};
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM3) // safer
    {
        HAL_TIM_PWM_Stop_DMA(htim, TIM_CHANNEL_1);
        // Reset the timer to ensure it is ready for the next transmission
        datasentflag = 1;
    }
}