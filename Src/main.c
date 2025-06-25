/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ws2812b.h"
#include "color.h"
#include <math.h>
#include "device_hal/devicefactory.h"
#include "i2c_device_bus/i2c_device_bus.h"
#include "stm32g0xx_hal_i2c.h"
#include "stm32g0xx_hal_i2c_ex.h"
#include "stm32g0xx_it.h"
#include "devices/bme280_io.h"
#include "configuration.h"
#include "stm32g0xx.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
  BME280_Reading bme280_reading = {0};
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
// #define SYSCFG_CFGR1_EN_VREFINT (1U << 22)
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* Global buffers */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_I2C1_Init();
  MX_SPI1_Init();
  MX_ADC1_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
  auto status = HAL_ADCEx_Calibration_Start(&hadc1);


  SetupDevices(); // Initialize devices based on configuration

  // HAL_I2C_EnableListen_IT(&hi2c1);
  // get the current sensor device
  // Device *current_sensor_device = FindDeviceByName("current_sensor");

  // just read from current sensor in a loop to test it
  // while (true)
  // {
  //   if (current_sensor_device)
  //   {
  //     float current_value = 0.0f;
  //     current_sensor_device->read(current_sensor_device, &current_value);
  //   } 
  // }
  // get the bme280 device
  Device *bme280_device = FindDeviceByName("bme280_sensor");

  // configure ws2812b

Set_LED(0, 0, 0, 0);
// WS2812_Send();

// Test 1: Just red
Set_LED(0, 0, 0, 0);
Set_LED(0, 25, 0, 0); 
HAL_Delay(100);


Set_LED(0, 0, 0, 0);
Set_LED(0, 0, 25, 0);
HAL_Delay(100);

// Test 3: Just blue
Set_LED(0, 0, 0, 0);
Set_LED(0, 0, 0, 25);
HAL_Delay(100);

  
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  uint32_t last_led_time = 0;
  I2C_Slave_Init();
  // Initialize I2C slave communication
  while (1)
  {
    I2C_Slave_PollingLoop(); // Poll for I2C commands

    // create a place to store the reading from the bme280
    // // read the bme280 sensor
    // if (bme280_device)
    // {
    //   bme280_device->read(bme280_device, &bme280_reading);
    // }

    // // Process the reading if valid
    // float temp = bme280_reading.temperature;
    // float humidity = bme280_reading.humidity;
    // float pressure = bme280_reading.pressure;
    // processI2C(); // Process any I2C commands received

    // flash the led between red and green every 1 second
    // if (HAL_GetTick() - last_led_time >= 1000)
    // {
    //   last_led_time = HAL_GetTick();
    //   static int led_state = 0; // 0 for red, 1 for green
    //   if (led_state == 0)
    //   {
    //     Set_LED(0, 25, 0, 0); // Set first LED to Red
    //   }
    //   else
    //   {
    //     Set_LED(0, 0, 25, 0); // Set first LED to Green
    //   }
    //   WS2812_Send(); // Send the data to the WS2812B LEDs
    //   led_state = !led_state; // Toggle state
    // }



    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
  RCC_OscInitStruct.PLL.PLLN = 8;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */



/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
