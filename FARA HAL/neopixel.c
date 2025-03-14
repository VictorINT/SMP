#include "stm32f10x.h"
#include "delay.h"

#define TIM1_CCR3_Address    0x40012C3C
#define MAX_LEDS 3
#define BUFSIZE (24*MAX_LEDS)

TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;
NVIC_InitTypeDef  NVIC_InitStructure;
EXTI_InitTypeDef   EXTI_InitStructure;
uint16_t LED_Buffer[BUFSIZE]; 

void RCC_Configuration(void);
void GPIO_Configuration(void);
void DMA_Configuration(void);

void DMA1_Channel5_IRQHandler(void)
{
    TIM1->CCR3 = 0;
    TIM_DMACmd(TIM1, TIM_DMA_Update, DISABLE);
    
    if(DMA_GetITStatus(DMA1_IT_TC5))
    {
        DMA_ClearITPendingBit(DMA1_IT_GL5);
    }
}

void set_LED_color(uint8_t led, uint8_t r, uint8_t g, uint8_t b)
{
    uint8_t i;
    for (i = 0; i < 8; i++) {
        LED_Buffer[led * 24 + i]      = (g & (1 << (7 - i))) ? 60 : 30;
        LED_Buffer[led * 24 + 8 + i]  = (r & (1 << (7 - i))) ? 60 : 30;
        LED_Buffer[led * 24 + 16 + i] = (b & (1 << (7 - i))) ? 60 : 30;
    }
}

int main(void)
{
    RCC_Configuration();
    GPIO_Configuration();
    DMA_Configuration();
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    DelayInit();
    
    TIM_TimeBaseStructure.TIM_Prescaler = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = 89;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 45;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
    TIM_OC3Init(TIM1, &TIM_OCInitStructure);
    
    TIM_DMACmd(TIM1, TIM_DMA_Update, DISABLE);
    TIM_Cmd(TIM1, ENABLE);
    TIM_CtrlPWMOutputs(TIM1, ENABLE);
    
    while (1)
    {
        set_LED_color(0, 255, 0, 0); // Rosu
        set_LED_color(1, 0, 255, 0); // Verde
        set_LED_color(2, 0, 0, 255); // Albastru
        
        DelayMs(1000);
        TIM_DMACmd(TIM1, TIM_DMA_Update, ENABLE);
        
        DelayMs(1000);
        
        set_LED_color(0, 255, 255, 0); // Galben
        set_LED_color(1, 0, 255, 255); // Cyan
        set_LED_color(2, 255, 0, 255); // Magenta
        
        DelayMs(1000);
        TIM_DMACmd(TIM1, TIM_DMA_Update, ENABLE);
        
        DelayMs(1000);
    }
}

void RCC_Configuration(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
}

void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void DMA_Configuration(void)
{
    DMA_InitTypeDef DMA_InitStructure;
    
    DMA_DeInit(DMA1_Channel5);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(TIM1->CCR3);
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)LED_Buffer;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = BUFSIZE;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    
    DMA_Init(DMA1_Channel5, &DMA_InitStructure);
    DMA_ITConfig(DMA1_Channel5, DMA_IT_TC, ENABLE);
    DMA_Cmd(DMA1_Channel5, ENABLE);
}
