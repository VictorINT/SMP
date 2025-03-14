#include "stm32f10x.h"
#include "delay.h"
#include <string.h>
#include <stdio.h>

#define ANALOG_BUFFER_SIZE 1

TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;
uint16_t CCR1_Val = 75;
uint16_t PrescalerValue = 0;

GPIO_InitTypeDef GPIO_InitStructure;
USART_InitTypeDef USART_InitStructure;

volatile uint16_t ADC_Buffer[ANALOG_BUFFER_SIZE];

void RCC_Configuration(void){

  /* TIM3 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

  /* GPIOA and GPIOB clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
}

void ADC_Config(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA, ENABLE); // Enable ADC1 and GPIOA clocks

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;  // ADC channel 0 (PA0)
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;  // Analog input
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    ADC_InitTypeDef ADC_InitStructure;
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;  // Enable scanning for multiple channels
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;  // Continuous mode
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;  // No external trigger
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;

    ADC_Init(ADC1, &ADC_InitStructure);

    // Configure ADC channel
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
	
    // Enable ADC DMA
    ADC_DMACmd(ADC1, ENABLE);

    // Enable ADC
    ADC_Cmd(ADC1, ENABLE);

    // Calibration (recommended after enabling ADC)
    ADC_ResetCalibration(ADC1);
    while (ADC_GetResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1);
    while (ADC_GetCalibrationStatus(ADC1));

    // Start ADC conversion
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

void GPIO_Configuration(void){
  GPIO_InitTypeDef GPIO_InitStructure;

  /* GPIOA Configuration:TIM3 Channel1  as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(GPIOA, &GPIO_InitStructure);
}


void initUsart(){
		//GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

		USART_InitStructure.USART_BaudRate = 115200;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		USART_Init(USART2, &USART_InitStructure);
		USART_Cmd(USART2, ENABLE);
}
void initGPIO(){
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
 	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //pp output - push-pull .
	 GPIO_Init(GPIOA, &GPIO_InitStructure);

	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	 GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void sendString(char a[]){
	int i,n;
	n=strlen(a);
	for (i=0;i<n;i++)
	{
		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);  
		USART_SendData(USART2, a[i]);
	}
}
void sendChar(char a){
		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);  
		USART_SendData(USART2, a);
}
void DMA_Config(void) {
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);  // Enable DMA1 clock

    DMA_InitTypeDef DMA_InitStructure;
    DMA_DeInit(DMA1_Channel1);  // Reset DMA1 channel 1 (ADC1 uses Channel 1)

    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;  // ADC data register
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADC_Buffer;  // Destination buffer
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  // Read from peripheral (ADC)
    DMA_InitStructure.DMA_BufferSize = ANALOG_BUFFER_SIZE;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; // 16-bit
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; // 16-bit
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;  // Continuous mode
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
    DMA_Cmd(DMA1_Channel1, ENABLE);  // Enable DMA channel
}


int main(void)
{ 
	// inits
  initUsart();
	initGPIO();
  RCC_Configuration();
	ADC_Config();
  GPIO_Configuration();
	DMA_Config();
	DelayInit();
	
	// pwm
  /* Compute the prescaler value */
  PrescalerValue = (uint16_t) (SystemCoreClock / 50000) - 1;
  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 999;
  TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  /* PWM1 Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR1_Val;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

  TIM_OC1Init(TIM3, &TIM_OCInitStructure);

  TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);

  
  TIM_ARRPreloadConfig(TIM3, ENABLE);

  /* TIM3 enable counter */
  TIM_Cmd(TIM3, ENABLE);

  while (1)
  {
		int val = ADC_Buffer[0];
		TIM3->CCR1 = (val * 999) / 4095;		
		char str[32];
		sprintf(str, "ADC_Buffer: %d\n", val);
		sendString(str);
		DelayMs(300);
	}
}