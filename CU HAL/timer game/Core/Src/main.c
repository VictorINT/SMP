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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "lcd.h"
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
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
volatile uint8_t flag1=0;
volatile uint8_t flag2=0;
volatile uint16_t count1=0;
volatile uint16_t count2=0;
volatile uint8_t ButtonPressed1 = 0;
volatile uint8_t ButtonPressed2 = 0;
volatile uint16_t timp1 = 0;
volatile uint16_t timp2 = 0;

uint8_t custom_char2[] = { 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F }; // caracter 2

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM4_Init(void);
/* USER CODE BEGIN PFP */
void SendToSerial(char* message);
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
	char s[30];
	uint8_t p1r, p2r;
	float scor1;
	float scor2;
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
  MX_USART2_UART_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  HAL_TIM_Base_Start(&htim3);
  HAL_TIM_Base_Start(&htim4);
  /* USER CODE BEGIN 2 */
  LCD16X2_Init(LCD16X2_DISPLAY_ON_CURSOR_OFF_BLINK_OFF);
  LCD16X2_GotoXY(0, 0);
  LCD16X2_CreateCustomChar(0, custom_char2);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if(flag1==0 && flag2==0)
	  	{
	  			ButtonPressed1=0;
	  			ButtonPressed2=0;
	  				//Alegere timp random player 1
	  			p1r=rand()%5;
	  			while(p1r==0)
	  			{
	  				p1r=rand()%5; //

	  			}
	  				//Alegere timp random player 2
	  			p2r=rand()%5;
	  			while(p2r==0)
	  			{
	  				p2r=rand()%5;
	  			}

	  			while(flag1==0 && flag2==0)
	  			{
	  						//partitionare ecran in doua
	  					LCD16X2_GotoXY(0, 0);
	  					LCD16X2_PutCustomChar(7, 0, 0);
	  					LCD16X2_PutCustomChar(8, 0, 0);
	  					LCD16X2_PutCustomChar(7, 1, 0);
	  					LCD16X2_PutCustomChar(8, 1, 0);

	  					  //Scriere nume jucatori.
	  					LCD16X2_GotoXY(0, 0);
	  					LCD16X2_Puts("Player1");
	  					  //Afisare timp intre cele doua apasari
	  					LCD16X2_GotoXY(3, 1);
	  					sprintf(s, "%ds",p1r);
	  					LCD16X2_Puts(s);

	  					LCD16X2_GotoXY(9, 0);
	  					LCD16X2_Puts("Player2");


	  					LCD16X2_GotoXY(12, 1);
	  					sprintf(s, "%ds",p2r);
	  					LCD16X2_Puts(s);

	  					HAL_Delay(1000);
	  			}
	  	}

	  		if(flag1==1 && flag2==1)
	  		{
	  			LCD16X2_ClrScr();
	  			scor1=fabs(p1r-(float)timp1/10000);
	  			scor2=fabs(p2r-(float)timp2/10000);


	  			sprintf(s, "%.2fs  S1=%.2f\n",(float)timp1/10000,scor1);
	  			LCD16X2_GotoXY(0, 0);
	  			LCD16X2_Puts(s);

	  			sprintf(s, "%.2fs  S2=%.2f\n",(float)timp2/10000,scor2);
	  			LCD16X2_GotoXY(0, 1);
	  			LCD16X2_Puts(s);

	  			HAL_Delay(3000);

	  			if(scor1>scor2)
	  			{
	  				LCD16X2_ClrScr();
	  				LCD16X2_GotoXY(0, 0);
	  				LCD16X2_Puts("Player2 a ");
	  				LCD16X2_GotoXY(0, 1);
	  				LCD16X2_Puts("castigat!!");
	  				HAL_Delay(5000);
	  			}
	  			else
	  			{
	  				LCD16X2_ClrScr();
	  				LCD16X2_GotoXY(0, 0);
	  				LCD16X2_Puts("Player1 a ");
	  				LCD16X2_GotoXY(0, 1);
	  				LCD16X2_Puts("castigat!!");
	  				HAL_Delay(5000);
	  			}

	  			LCD16X2_ClrScr();
	  			flag1=0;
	  			flag2=0;
	  			ButtonPressed1 = 0;
	  			ButtonPressed2 = 0;
	  			timp1=0;
	  			timp2=0;
	  			count1=0;
	  			count2=0;
	  		}
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 7200;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 65535;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 7200;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 65535;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

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
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : B1_Pin PC0 */
  GPIO_InitStruct.Pin = B1_Pin|GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PA1 */
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);

  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

  /* USER CODE BEGIN MX_GPIO_Init_2 */


  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
/**
  * @brief  EXTI callback
  * @param  GPIO_Pin: Specifies the pin connected to EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  SendToSerial("am intrat in nvic callback!\r\n");
  char str[32];
  sprintf(str, "GPIO_Pin:%d\r\n",GPIO_Pin);
  SendToSerial(str);
  sprintf(str, "GPIO_Pin_0:%d\r\n",GPIO_PIN_0);
  SendToSerial(str);
  sprintf(str, "GPIO_Pin_1:%d\r\n",GPIO_PIN_1);
  SendToSerial(str);
  if(GPIO_Pin == GPIO_PIN_0) // Player 2 button (PC0)
  {
    /* Debouncing delay */
	SendToSerial("PC0 interrupt!\r\n");
    //HAL_Delay(30); // ? nu trece de HAL_Delay()???
	for(int i=0;i<30000;i++)			// debouncing pt. buton
	    __NOP();
    if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_0) == GPIO_PIN_SET)
    {
    	SendToSerial("Pressed!\r\n");
      if(ButtonPressed2 == 0)
      {
        ButtonPressed2 = 1;
        __HAL_TIM_SET_COUNTER(&htim4, 0); // Reset counter
      }
      else
      {
        ButtonPressed2 = 0;
        flag2 = 1;
        timp2 = __HAL_TIM_GET_COUNTER(&htim4); // Get counter value
      }
    }
  }
  else if(GPIO_Pin == GPIO_PIN_1) // Player 1 button (PA1)
  {
    /* Debouncing delay */
	SendToSerial("PA1 interrupt!\r\n");
    //HAL_Delay(30);
	for(int i=0;i<30000;i++)			// debouncing pt. buton
	    __NOP();
    if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1) == GPIO_PIN_SET)
    {
    	SendToSerial("Pressed!\r\n");
      if(ButtonPressed1 == 0)
      {
//    	HAL_TIM_Base_Start(&htim3);
        ButtonPressed1 = 1;
        __HAL_TIM_SET_COUNTER(&htim3, 0); // Reset counter
      }
      else
      {
        ButtonPressed1 = 0;
        flag1 = 1;
        timp1 = __HAL_TIM_GET_COUNTER(&htim3); // Get counter value
      }
    }
  }
}

void SendToSerial(char* message)
  {
    HAL_UART_Transmit(&huart2, (uint8_t*)message, strlen(message), 100);
  }
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
