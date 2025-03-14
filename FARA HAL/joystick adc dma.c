#include "stm32f10x.h"
#include <stdio.h>
#include <string.h>
#include "delay.h"
#include <stdlib.h> // pentru random
#include "lcd16x2.h"

#define ANALOG_BUFFER_SIZE 2

#define MAP_VERTICAL 2
#define MAP_HORIZONTAL 16
#define NR_OF_OBSTACLES 6

int X_Actual = 0;
int Y_Actual = 0;

uint8_t dot[] = {0x0C, 0x0C, 0x00, 0x0E, 0x1C, 0x0C, 0x1A, 0x13};
uint8_t obstacle[] = {0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E};

/**
 caracterul *
*/

volatile uint16_t ADC_Buffer[ANALOG_BUFFER_SIZE];
USART_InitTypeDef USART_InitStructure;
GPIO_InitTypeDef GPIO_InitStructure;
void initGPIO()
{
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
 	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //pp output - push-pull .
	 GPIO_Init(GPIOA, &GPIO_InitStructure);
		
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	 GPIO_Init(GPIOA, &GPIO_InitStructure);
}
void initUsart()
{
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

void sendString(char a[])
{
	int i,n;
	n=strlen(a);
	for (i=0;i<n;i++)
	{
		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);  
		USART_SendData(USART2, a[i]);
	}
}
void sendChar(char a)
{
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

void ADC_Config(void) 
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA, ENABLE); // Enable ADC1 and GPIOA clocks

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;  // ADC channel 0 (PA0) si pa1
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;  // Analog input
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    ADC_InitTypeDef ADC_InitStructure;
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;  // Enable scanning for multiple channels
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;  // Continuous mode
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;  // No external trigger
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 2;

    ADC_Init(ADC1, &ADC_InitStructure);

    // Configure ADC channel
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
		ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_55Cycles5);
	
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

void init(){
	DelayInit();
	initUsart();
	initGPIO();
	DMA_Config();
	ADC_Config();
}

int MAP[16][2];
int index[16];
int time = 0;
int procent = 900;
int main(){
	int xobs[NR_OF_OBSTACLES], yobs[NR_OF_OBSTACLES];
	init();
	lcd16x2_init(LCD16X2_DISPLAY_ON_CURSOR_OFF_BLINK_OFF);
	lcd16x2_gotoxy(0, 0);
	lcd16x2_create_custom_char(0, dot);
	lcd16x2_create_custom_char(1, obstacle);
	// initial
	for(int i = 0; i < NR_OF_OBSTACLES; i++){
		int rv = rand()%16;
		int parimpar = rand()%2;
		//int doitrei = (parimpar==0) ? 2 : 3;
		xobs[i] = rv;
		yobs[i] = parimpar;
		MAP[xobs[i]][yobs[i]]=1;
		index[i]=xobs[i];
		//
		for(int j = 0; j < MAP_HORIZONTAL; j++){
			if(MAP[j][0] == 1 && MAP[j][1] == 1){
				MAP[j][1] = 0;
				for(int k = 0; k < MAP_HORIZONTAL; k++)
					if(xobs[k] == j)
						yobs[k] = 1;
			}
			if(j > 1 && j < 15)
				if((MAP[j][0] == 1 && MAP[j+1][1] == 1) || (MAP[j][0] == 1 && MAP[j-1][1] == 1)){
					MAP[j][0] = 0;
				for(int k = 0; k < MAP_HORIZONTAL; k++)
					if(xobs[k] == j)
						xobs[k]++;
				}
		}
		
	}
	while(1){
		// moving
		int xvalue = ADC_Buffer[1];
		int yvalue = ADC_Buffer[0];
		if(xvalue < 2000){
			if(X_Actual > 0){
				X_Actual--;
			}
		}
		else if(xvalue > 3700){
			if(X_Actual < MAP_HORIZONTAL)
			{
				X_Actual++;
			}
		}
		if(yvalue < 2000){
			if(Y_Actual == 0){
				Y_Actual++;
			}
		}else if(yvalue > 3700){
			if(Y_Actual == 1)
			{
				Y_Actual--;
			}
		}
		// print caracter la (X_Actual, Y_Actual);
		lcd16x2_clrscr();
		lcd16x2_put_custom_char(X_Actual, Y_Actual, 0);
		for(int i = 0; i < NR_OF_OBSTACLES; i++){
			// move obstacles towards start position.
			if(time%procent==0){
				if(xobs[i] > 0)
					xobs[i]--;
				else{
					xobs[i] = 16;
				}
			}
			lcd16x2_put_custom_char(xobs[i], yobs[i], 1);
		}
				// check if obstacle was hit
		for(int i = 0; i < NR_OF_OBSTACLES; i++){
			if(X_Actual == xobs[i] && Y_Actual == yobs[i])
			{
				sendString("You lost, obstacle touched!\n");
				lcd16x2_clrscr();
				lcd16x2_puts("You lost!");
				return 0;
			}
		}
		// serial
		char str[32];
		sprintf(str, "x: %d, y: %d\n", xvalue, yvalue);
		sendString(str);
		
		// delay
		time+=300;
		if(time == 6000)
			procent-=300;
		if(time == 18000)
			procent-=300;
		DelayMs(300);
		//sendString("am ajuns dupa delay");
	}
	return 0;
}