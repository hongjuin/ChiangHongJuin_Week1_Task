/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "stdlib.h"
#include "stdio.h"
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
ADC_HandleTypeDef hadc1;

TIM_HandleTypeDef htim2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void TurnOnLed (int ledNumber) {
		switch (ledNumber) {
		  case 0:
		    HAL_GPIO_WritePin (GPIOA, GPIO_PIN_2, GPIO_PIN_SET);
		    break;
		  case 1:
			  HAL_GPIO_WritePin (GPIOA, GPIO_PIN_3, GPIO_PIN_SET);
			  break;
		  case 2:
			  HAL_GPIO_WritePin (GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
			  break;
		  case 3:
			  HAL_GPIO_WritePin (GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
			  break;
		  case 4:
			  HAL_GPIO_WritePin (GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
			  break;
		  default:
			  break;
		}
	}

void TurnOffAllLeds(void) {
    HAL_GPIO_WritePin(
        GPIOA,
        GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 |
        GPIO_PIN_5 | GPIO_PIN_6,
        GPIO_PIN_RESET
    );
}

void LoseSequence () {
	TurnOffAllLeds ();
	//Blink all LED
	for (int i= 0; i< 5; i++) {
		//Turn on LED.
		HAL_GPIO_WritePin (GPIOA,
				           GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6,
						   GPIO_PIN_SET);
		HAL_Delay (500);
		//Turn off LED.
		HAL_GPIO_WritePin (GPIOA,
						           GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6,
								   GPIO_PIN_RESET);
		HAL_Delay (500);
	}
}
void WinSequence () {
	for (int i= 0; i< 5; i++) {
		//Turn on sixth LED.
		HAL_GPIO_WritePin (GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
		HAL_Delay (500);
		//Turn off sixth LED.
		HAL_GPIO_WritePin (GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
		HAL_Delay (500);
	}
}

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
  MX_ADC1_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */




  int win_count= 0;
  int lose_count= 0;
  while (1)
  {
	int led1;
	int led2;
	int led3;

	int led1State= 0;
	int led2State= 0;
	int led3State= 0;
	int led4State= 0;
	int led5State= 0;

    //Turn off all led before new selection.
	TurnOffAllLeds ();
	//Randomly select LED.
	led1= rand() %5;
	do {
		led2= rand() %5;
	}
	while (led2== led1);
	do {
		led3= rand() %5;
	}
	while (led3== led1 || led3== led2);
	//Track with turn on LED.
	if (led1== 0 || led2== 0 || led3== 0) {
		led1State= 1;
	}
	if (led1== 1 || led2== 1 || led3== 1) {
		led2State= 1;
	}
	if (led1== 2 || led2== 2 || led3== 2) {
		led3State= 1;
	}
	if (led1== 3 || led2== 3 || led3== 3) {
		led4State= 1;
	}
	if (led1== 4 || led2== 4 || led3== 4) {
		led5State= 1;
	}
	//Call out function
	TurnOnLed (led1);
	TurnOnLed (led2);
	TurnOnLed (led3);


	//Potentiometer
	HAL_ADC_Start (&hadc1);
	//Wait for conversion.
	HAL_ADC_PollForConversion (&hadc1, HAL_MAX_DELAY);
	//Get ADC value.
	int adcValue= HAL_ADC_GetValue (&hadc1);
	//Convert ADC to time limit.
	int timeLimit= 1000U+ (9000U* adcValue)/ 4095U;
	HAL_ADC_Stop (&hadc1);

	//Check whether the button has been pressed within time limit.
	uint32_t startTime= HAL_GetTick ();
	while (((HAL_GetTick ()- startTime)< timeLimit)&& (led1State== 1 || led2State== 1 || led3State== 1 || led4State== 1 || led5State== 1)) {
	    //If fall over time limit, then it will exit this WHILE loops.
		//Get the button state.
		int button1State= HAL_GPIO_ReadPin (GPIOA, GPIO_PIN_7);
		int button2State= HAL_GPIO_ReadPin (GPIOB, GPIO_PIN_0);
		int button3State= HAL_GPIO_ReadPin (GPIOB, GPIO_PIN_1);
		int button4State= HAL_GPIO_ReadPin (GPIOB, GPIO_PIN_2);
		int button5State= HAL_GPIO_ReadPin (GPIOB, GPIO_PIN_10);
		//Button activating.
		if (led1State== 1 && button1State== GPIO_PIN_RESET) {
			HAL_GPIO_WritePin (GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);
			led1State= 0;
		}
		if (led2State== 1 && button2State== GPIO_PIN_RESET) {
			HAL_GPIO_WritePin (GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
			led2State= 0;
		}
		if (led3State== 1 && button3State== GPIO_PIN_RESET) {
			HAL_GPIO_WritePin (GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
			led3State= 0;
		}
		if (led4State== 1 && button4State== GPIO_PIN_RESET) {
			HAL_GPIO_WritePin (GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
			led4State= 0;
		}
		if (led5State== 1 && button5State== GPIO_PIN_RESET) {
			HAL_GPIO_WritePin (GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
			led5State= 0;
		}
	}
	if (led1State== 1 || led2State== 1 || led3State== 1 || led4State== 1 || led5State== 1) {
	   lose_count++;


	}
	else {
		//All three led light off successfully.
		win_count++;
	}
	//If user lose three times
	if (lose_count== 3) {
	   LoseSequence ();
	   lose_count= 0;
	}
	//If user win three time.
	if (win_count== 3) {
		WinSequence ();
		win_count= 0;
	}

    HAL_Delay (1000);
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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 4294967295;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5
                          |GPIO_PIN_6, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PA2 PA3 PA4 PA5
                           PA6 */
  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5
                          |GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PA7 */
  GPIO_InitStruct.Pin = GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB1 PB2 PB10 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
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
#ifdef USE_FULL_ASSERT
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
