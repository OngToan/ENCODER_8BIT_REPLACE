/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define READ_BUT_1		HAL_GPIO_ReadPin(BUTTON_1_GPIO_Port, BUTTON_1_Pin)
#define READ_BUT_2		HAL_GPIO_ReadPin(BUTTON_2_GPIO_Port, BUTTON_2_Pin)
#define READ_BUT_3		HAL_GPIO_ReadPin(BUTTON_3_GPIO_Port, BUTTON_3_Pin)

#define ON 1
#define OFF 0

#define REFdebounce 200 // Buton debounce parameter in milisecond
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim17;

/* USER CODE BEGIN PV */
uint16_t counter = 0;

volatile uint16_t in0 = 3;
volatile uint16_t in0_0 = 0;
volatile uint16_t in0_1 = 0;
volatile uint8_t state_of_in0;

volatile uint16_t in1 = 3;
volatile uint16_t in1_0 = 0;
volatile uint16_t in1_1 = 0;
volatile uint8_t state_of_in1;

volatile uint16_t in2 = 3;
volatile uint16_t in2_0 = 0;
volatile uint16_t in2_1 = 0;
volatile uint8_t state_of_in2;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM17_Init(void);
/* USER CODE BEGIN PFP */
void  HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
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
  MX_TIM1_Init();
  MX_TIM17_Init();
  /* USER CODE BEGIN 2 */
	GPIOA->BSRR = GPIO_PIN_All;

	
	
  while (1)
  {
		if (in0 == ON ){
			HAL_TIM_Base_Start_IT(&htim17);
		}
		else if ( in1 == ON ) {
			HAL_TIM_Base_Stop_IT(&htim17);
			GPIOA->BSRR = GPIO_PIN_All;
		}
//		else if ( counter > 255 ) {
//			counter = 0;
//			HAL_TIM_Base_Stop_IT(&htim17);
//			GPIOA->BSRR = GPIO_PIN_All;
//		}
		HAL_Delay(5);
  }
  /* USER CODE END 3 */
}


void  HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if (htim->Instance==TIM17) {
			++counter;
		if(counter <=255){
			GPIOA->BSRR = GPIO_PIN_All;
			GPIOA->BRR = counter;
			
		}
		else if (counter > 255){
			HAL_TIM_Base_Stop_IT(&htim17);
			counter = 0;
			GPIOA->BSRR = GPIO_PIN_All;
			GPIOA->BRR = counter;
		}
	}
	
}

void HAL_SYSTICK_Callback(){  // Need to have this code in the stm32f0xx_it.c  HAL_SYSTICK_IRQHandler();
	
	in0 = READ_BUT_1;
	in1 = READ_BUT_2;
	in2 = READ_BUT_3;
	////////////TEST Button 0 /////////////
	if(in0 == 0)
	{
		in0_0++;
		in0_1 = 0 ;
		if (in0_0 > REFdebounce)
		{
			in0_0 = REFdebounce + 1;
			state_of_in0 = 0;
		}
	}
	else if (in0 == 1)
	{
		in0_0 = 0;
		in0_1++;
		if (in0_1 > REFdebounce)
		{
			in0_1 = REFdebounce + 1;
			state_of_in0 = 1;
		}
	}
		////////////TEST Button 1 /////////////
	if(in1 == 0)
	{
		in1_0++;
		in1_1 = 0 ;
		if (in1_0 > REFdebounce)
		{
			in1_0 = REFdebounce + 1;
			state_of_in1 = 0;
		}
	}
	else if (in1 == 1)
	{
		in1_0 = 0;
		in1_1++;
		if (in1_1 > REFdebounce)
		{
			in1_1 = REFdebounce + 1;
			state_of_in1 = 1;
		}
	}
			////////////TEST Button 2 /////////////
	if(in2 == 0)
	{
		in2_0++;
		in2_1 = 0 ;
		if (in2_0 > REFdebounce)
		{
			in2_0 = REFdebounce + 1;
			state_of_in2 = 0;
		}
	}
	else if (in2 == 1)
	{
		in2_0 = 0;
		in2_1++;
		if (in2_1 > REFdebounce)
		{
			in2_1 = REFdebounce + 1;
			state_of_in2 = 1;
		}
	}
	
}


/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL8;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 49;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 959;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief TIM17 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM17_Init(void)
{

  /* USER CODE BEGIN TIM17_Init 0 */

  /* USER CODE END TIM17_Init 0 */

  /* USER CODE BEGIN TIM17_Init 1 */

  /* USER CODE END TIM17_Init 1 */
  htim17.Instance = TIM17;
  htim17.Init.Prescaler = 1000;
  htim17.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim17.Init.Period = 2000;
  htim17.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim17.Init.RepetitionCounter = 0;
  htim17.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim17) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM17_Init 2 */

  /* USER CODE END TIM17_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, BIT_0_Pin|BIT_1_Pin|BIT_2_Pin|BIT_3_Pin 
                          |BIT_4_Pin|BIT_5_Pin|BIT_6_Pin|BIT_7_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : BIT_0_Pin BIT_1_Pin BIT_2_Pin BIT_3_Pin 
                           BIT_4_Pin BIT_5_Pin BIT_6_Pin BIT_7_Pin */
  GPIO_InitStruct.Pin = BIT_0_Pin|BIT_1_Pin|BIT_2_Pin|BIT_3_Pin 
                          |BIT_4_Pin|BIT_5_Pin|BIT_6_Pin|BIT_7_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : BUTTON_1_Pin */
  GPIO_InitStruct.Pin = BUTTON_1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BUTTON_1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : BUTTON_2_Pin BUTTON_3_Pin */
  GPIO_InitStruct.Pin = BUTTON_2_Pin|BUTTON_3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

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
void assert_failed(char *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
