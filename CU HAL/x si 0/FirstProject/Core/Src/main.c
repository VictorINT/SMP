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
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
uint8_t rxData;
char tabla[]={'-','-','-','-','-','-','-','-','-'};

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART1_UART_Init(void);
static void afisare(void);
static _Bool victorie(void);
static void sendString(char *str);
static void mutareX();
static void mutare0();
char readChar();

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
  MX_USART2_UART_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  sendString("Salut!\n");
	  sendString("Tu vei muta primul - esti cu X.\n");
	  sendString("Tabla de joc are pozitiile numerotate de la 1 la 9.\n");
	  sendString("1 2 3\n");
	  sendString("4 5 6\n");
	  sendString("7 8 9\n");

	  do{
		  mutareX();
		  if (victorie()) break;
		  mutare0();
		  afisare();
	  } while(!victorie());

	  HAL_Delay(5000);

	  for (int i=0; i<100; i++){
		  sendString("\n");
	  }
	  for (int i=0; i<9; i++){
		  	 tabla[i]='-';
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
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
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

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

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

static void sendString(char *str)
{
	HAL_UART_Transmit(&huart2, (uint8_t*)str, strlen(str), HAL_MAX_DELAY);
}

void afisare()
{
    char msg[20];
    int index = 0;

    for (int i = 0; i < 9; i++)
    {
        msg[index++] = tabla[i];
        msg[index++] = ' ';

        if (i == 2 || i == 5 || i == 8)
        {
            msg[index - 1] = '\n';
        }
    }
    msg[index] = '\0';

    sendString(msg);
}

char readChar()
{
    char chr;

    if (HAL_UART_Receive(&huart2, (uint8_t*)&chr, 1, 10) == HAL_OK)  // Timeout 10ms
    {
        return chr;
    }

    return '\0';
}

_Bool victorie()
{
  char winner = '-';
	uint8_t i;

  if (tabla[0]==tabla[1] && tabla[1]==tabla[2]) winner=tabla[0]; // linie 1
  if (tabla[3]==tabla[4] && tabla[4]==tabla[5]) winner=tabla[3]; // linie 2
  if (tabla[6]==tabla[7] && tabla[7]==tabla[8]) winner=tabla[6]; // linie 3
  if (tabla[0]==tabla[3] && tabla[3]==tabla[6]) winner=tabla[0]; // coloana 1
  if (tabla[1]==tabla[4] && tabla[4]==tabla[7]) winner=tabla[1]; // coloana 2
  if (tabla[2]==tabla[5] && tabla[5]==tabla[8]) winner=tabla[2]; // coloana 3
  if (tabla[0]==tabla[4] && tabla[4]==tabla[8]) winner=tabla[0]; // diagonala principala
  if (tabla[2]==tabla[4] && tabla[4]==tabla[6]) winner=tabla[2]; // diagonala secundara

  if (winner =='-')
	{
    winner = '*';
    for (i=0; i<9; i++)
      if (tabla[i]=='-')
				winner='-';
  }

  if (winner=='-')
		return 0;

  else
	{
     if (winner == 'X')
		 {
       afisare();
       sendString("Ati castigat! Felicitari!\n");
     }
     if (winner == '0')
			 sendString("Nucleo a castigat! Mai incercati!\n");

     if (winner == '*')
			 sendString("Remiza! Plictisitor!\n");

     return 1;
   }
}

static void mutareX()
{
   _Bool valid = 0;
   uint8_t m0 = 0;
   while (!valid)
	 {
     sendString("Alege o pozitie de la 1 la 9.\n");
     while (m0<'1' || m0>'9')
		 {
       m0 = readChar();
       HAL_Delay(1000);
     }

     sendString("Ati ales pozitia: \n");
     char mut[2];
     mut[0] = m0;
     mut[1] = '\0';
     sendString(mut);
		 m0 = m0 - 48;
     sendString("\n");

     if (tabla[m0-1]!='-')
				sendString("Pozitie eronata!\n");

		 else
		 {
       tabla[m0-1]='X';
       valid = 1;
     }

   }
}

static void mutare0()
{
	int plasat;
	for(int i = 0; i < 9; i++){
			plasat = 0;
			if(tabla[i] == '-')
			{
					tabla[i] = 'X';
				  char winner = '-';
					if (tabla[0]==tabla[1] && tabla[1]==tabla[2] && tabla[0] == 'X') {winner='X';}// linie 1
					if (tabla[3]==tabla[4] && tabla[4]==tabla[5] && tabla[3] == 'X') winner='X'; // linie 2
					if (tabla[6]==tabla[7] && tabla[7]==tabla[8] && tabla[6] == 'X') winner='X'; // linie 3
					if (tabla[0]==tabla[3] && tabla[3]==tabla[6] && tabla[0] == 'X') winner='X'; // coloana 1
					if (tabla[1]==tabla[4] && tabla[4]==tabla[7] && tabla[1] == 'X') winner='X'; // coloana 2
					if (tabla[2]==tabla[5] && tabla[5]==tabla[8] && tabla[2] == 'X') winner='X'; // coloana 3
					if (tabla[0]==tabla[4] && tabla[4]==tabla[8] && tabla[0] == 'X') winner='X'; // diagonala principala
					if (tabla[2]==tabla[4] && tabla[4]==tabla[6] && tabla[2] == 'X') winner='X'; // diagonala secundara
					if(winner == 'X')
					{
						tabla[i] = '0';
						plasat = 1;
						break;
					}
					else
					{
						tabla[i] = '-';
					}
			}

	}
	if(plasat == 0){
		int i=-1;
		while (i==-1)
		{
			i = rand()%9;
			if (tabla[i]=='-')
			{
					tabla[i]='0';
					sendString("Nucleo a ales pozitia: \n");
					char aux[2];
					aux[0] = i + 1 + '0';
					aux[1] = '\0';
					sendString(aux);
					sendString("\n");
					break;
			}
			else i=-1;
		}
	}


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
