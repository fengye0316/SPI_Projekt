/**
  ******************************************************************************
  * @file    TIM_TimeBase/main.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    19-September-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f4_discovery.h"
#include "stm32f4xx.h"
#include "spi_config.h"

/** @addtogroup STM32F4_Discovery_Peripheral_Examples
  * @{
  */

/** @addtogroup TIM_TimeBase
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;
__IO uint16_t CCR1_Val = 54618;
__IO uint16_t CCR2_Val = 27309;
__IO uint16_t CCR3_Val = 13654;
__IO uint16_t CCR4_Val = 6826;

int cnt = 0;
int cnt2= 0;
int status = 0;

/* Private typedef -----------------------------------------------------------*/
GPIO_InitTypeDef  GPIO_InitStructure;
TIM_TimeBaseInitTypeDef  TIM_TimeBasecStructure;
EXTI_InitTypeDef   EXTI_InitStructure;
SPI_InitTypeDef	SPI_InitStructure;

extern NVIC_InitTypeDef   NVIC_InitStructure;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void LED_Config(void);
void TIMER_Config(void);
void PA0_als_EXTI0(void);
void Delay(__IO uint32_t nCount);
void GPIO_Config(void);


/* Private functions ---------------------------------------------------------*/

void TIMER_Config(void){
// 	NVIC_InitTypeDef NVIC_InitStructure;
//   /* Enable the TIM2 gloabal Interrupt */
//   NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
//   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//   NVIC_Init(&NVIC_InitStructure);
//   /* TIM2 clock enable */
//   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
//   /* Time base configuration */
//   TIM_TimeBaseStructure.TIM_Period = 2000 - 1;  // 1 MHz down to 1 KHz (1 ms)
//   TIM_TimeBaseStructure.TIM_Prescaler = 65535; // 24 MHz Clock down to 1 MHz (adjust per your clock)
//   TIM_TimeBaseStructure.TIM_ClockDivision = 4;
//   TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
//   TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
//   /* TIM IT enable */
//   TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
//   	/* TIM2 enable counter */
//   TIM_Cmd(TIM2, ENABLE);
// 	
// 	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
//   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//   NVIC_Init(&NVIC_InitStructure);
//   /* TIM2 clock enable */
//   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
//   /* Time base configuration */
//   TIM_TimeBaseStructure.TIM_Period = 2000-1;  // 1 MHz down to 1 KHz (1 ms)
//   TIM_TimeBaseStructure.TIM_Prescaler = 65535; // 24 MHz Clock down to 1 MHz (adjust per your clock)
//   TIM_TimeBaseStructure.TIM_ClockDivision = 4;
//   TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
//   TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
//   /* TIM IT enable */
//   TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
//   /* TIM2 enable counter */
//   TIM_Cmd(TIM4, ENABLE);
// 	
}

void PA0_als_EXTI0(void){

 /* GPIOA Periph clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

  /* Configure PA0 Input mode */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// ------ PA0 als EXTI0 -------------------
  /* Enable SYSCFG clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	/* Connect EXTI Line0 to PA0 pin */
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);
  // ------ PA0 als EXTI0 -------------------	
	
	/* Configure EXTI Line0 */
  EXTI_InitStructure.EXTI_Line = EXTI_Line0;  
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  /* Configure EXTI Line0 */

/* Enable and set EXTI Line0 Interrupt to the lowest priority */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;	// Gruppe 	3
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					// Subprio 	3
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

/* Enable and set EXTI Line0 Interrupt to the lowest priority */

}

void EXTI0_IRQHandler(void){
 if(EXTI_GetITStatus(EXTI_Line0) != RESET)
  {
		if(TIM_TimeBaseStructure.TIM_Prescaler == 5164){
		TIM_TimeBaseStructure.TIM_Prescaler = 65535;
		TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
		GPIO_ResetBits(GPIOD, GPIO_Pin_14);
		cnt = status = 0;
		}
    EXTI_ClearITPendingBit(EXTI_Line0);
  }
}

void TIM2_IRQHandler(void){
  if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
  {
    GPIO_ToggleBits(GPIOD, GPIO_Pin_14);
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
  }
}
void TIM4_IRQHandler(void){
  if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
  {
		cnt++;
		if (cnt == 5 && status == 0){
			TIM_TimeBaseStructure.TIM_Prescaler = 16383;
			TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
			cnt = 0;
			status++;
		}
		if (cnt == 5 && status == 1) {
			TIM_TimeBaseStructure.TIM_Prescaler = 5164;
			TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
			cnt = 0;
			status++;
		}
    TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
  }
}

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */

int main(void){
	uint16_t data;
	NVIC_PriorityGroupConfig (NVIC_PriorityGroup_2);
	//	int cnt = 0;
	//TIMER_Config();
	//PA0_als_EXTI0();
	SPI1_Config();

	
	// Testing send Data
// 	SPI_SSOutputCmd(SPI1, ENABLE);
// 	SPI_I2S_SendData(SPI1, 0x50);
// 	SPI_SSOutputCmd(SPI1, DISABLE);
// 	data = SPI_I2S_ReceiveData(SPI1);

	
	//selecting Chip1 of 4

  while (1)
  {  
			GPIO_ToggleBits(GPIOD, GPIO_Pin_12);
			SPI_SSOutputCmd(SPI1, ENABLE);
			SPI_I2S_SendData(SPI1, 0x5000);
			data = SPI_I2S_ReceiveData(SPI1);
			SPI_SSOutputCmd(SPI1, DISABLE);

			Delay(0x8FFFFF);
  }
}

/**
  * @brief  Setup an interval timer
  * @param  None
  * @retval None
  */




void Delay(__IO uint32_t nCount)
{
  while(nCount--){
		//GPIO_WriteBit (GPIOD, GPIO_Pin_13, 1);
	}
}



#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  while (1)
  {}
}
#endif

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
