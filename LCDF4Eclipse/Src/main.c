/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "spi.h"
#include "gpio.h"
#include "LCDLib.h"
#include "LCDGFX.h"
#include "STM32.h"
#include "CNC.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

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
  MX_SPI5_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */
  ILI9341_Init();
  ILI9341_Set_Rotation(SCREEN_VERTICAL_1);
  ILI9341_Fill_Screen(WHITE);
  /* USER CODE END 2 */

  /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    uint32_t adcValue = 0;
    uint32_t adcReadValue = 0;
    uint32_t pastadcValue = 0;

    while (1)
    {
  	  HAL_ADC_Start(&hadc1);
  	  	  if(HAL_ADC_PollForConversion(&hadc1, 5)==HAL_OK)
  	  	  {
  	  		  pastadcValue = adcReadValue;
  	  		  adcReadValue = HAL_ADC_GetValue(&hadc1);
  	  		  adcValue = (adcReadValue >> 2) - 320;
  	  		  /*
  	  		   * shift by two to bring range of values down to 0 to 1024
  	  		   * subtract by 320 t0 get range to -320 to 704
  	  		   */
  	  		  if ((adcReadValue - pastadcValue) <= -25 && (adcReadValue - pastadcValue) >= 25){
  	  		  	  	  ILI9341_Fill_Screen(WHITE);
  	  		  	  	  ILI9341_printImage(13, adcValue-650, 214, 161, CNC, sizeof(CNC));
  	  		  	  	  ILI9341_printImage(13, adcValue-300, 214, 161, CNC, sizeof(CNC));
  	  		  	  	  ILI9341_Draw_Text("Sujith Naapa Ramesh", 0, adcValue-90, BLACK, 2, WHITE);
  	  		  	  	  ILI9341_Draw_Text("2 Highland Ln", 0, adcValue-60, BLACK, 2, WHITE);
  	  		  	  	  ILI9341_Draw_Text("Littleton, MA 01460", 0, adcValue-30, BLACK, 2, WHITE);
  	  		  		  ILI9341_Draw_Text("Sujith Naapa Ramesh", 0, adcValue, BLACK, 2, WHITE);
  	  		  		  ILI9341_Draw_Text("2 Highland Ln", 0, adcValue+30, BLACK, 2, WHITE);
  	  		  		  ILI9341_Draw_Text("Littleton, MA 01460", 0, adcValue+60, BLACK, 2, WHITE);
  	  		  		  ILI9341_printImage(0, adcValue+90, 80, 130, STM32, sizeof(STM32));
  	  		  		  ILI9341_printImage(13, adcValue+240, 214, 161, CNC, sizeof(CNC));
  	  		  		  ILI9341_printImage(13, adcValue+450, 214, 161, CNC, sizeof(CNC));
  	  		  		  //HAL_GPIO_TogglePin(GPIOG, GPIO_PIN_13|GPIO_PIN_14);
  	  		  		  //HAL_Delay(1000);
  	  		  }
  	  	  }
    }
    /* USER CODE END 3 */}

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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 180;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Activate the Over-Drive mode 
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
