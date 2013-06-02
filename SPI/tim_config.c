
/* 
 *	Timer Configuration
 */
#include "tim_config.h"
#include "spi_config.h"


extern GPIO_InitTypeDef  GPIO_InitStructure;
extern TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
extern NVIC_InitTypeDef   NVIC_InitStructure;

void TIM_Config(void){
	
	//NVIC_InitTypeDef NVIC_InitStructure;
  
	//PrescalerValue = (uint16_t) ((SystemCoreClock / 2) / 500000) - 1;
	
	/* Enable the TIM4 gloabal Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
	/* TIM4 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 10000 - 1;  // 168MHz to 168kHz
  TIM_TimeBaseStructure.TIM_Prescaler = 4200 - 1; // 168KHz to 1 Hz
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	
	/* TIM IT enable */
  TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
  	/* TIM2 enable counter */
  TIM_Cmd(TIM4, ENABLE);

}
