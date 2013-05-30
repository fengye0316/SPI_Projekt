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
#include "stm32f4xx_tim.h"
#include "spi_config.h"
#include "tim_config.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint16_t CCR1_Val = 250;
__IO uint16_t CCR2_Val = 500;
__IO uint16_t CCR3_Val = 750;
__IO uint16_t CCR4_Val = 200;

uint16_t PrescalerValue = 0;
uint16_t CC1 = 250;
uint16_t CC2 = 500;
uint16_t CC3 = 900;
uint16_t CC4 = 900;

int cnt = 0;
int status = 0;
double temperature;
uint16_t data = 0;
uint8_t data1 = 0, data2 = 0;

/* Private typedef -----------------------------------------------------------*/
GPIO_InitTypeDef  GPIO_InitStructure;
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
EXTI_InitTypeDef   EXTI_InitStructure;
USART_InitTypeDef USART_InitStruct;

extern NVIC_InitTypeDef   NVIC_InitStructure;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void LED_Config(void);
void PA0_als_EXTI0(void);
void Delay(__IO uint32_t nCount);
void calculateTemp(uint16_t);

/* Private functions ---------------------------------------------------------*/
void PA0_als_EXTI0(void){

 /* GPIOA Periph clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

  /* Configure PA0 Input mode */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM5);
	
	
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

	/* Enable and set EXTI Line0 Interrupt to the lowest priority */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;	// Gruppe 	3
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					// Subprio 	3
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

}

void EXTI0_IRQHandler(void){
  if(EXTI_GetITStatus(EXTI_Line0) != RESET){
	}
	EXTI_ClearITPendingBit(EXTI_Line0);
}

int main(void){
	/* Init of NVIC */
	NVIC_PriorityGroupConfig (NVIC_PriorityGroup_2);
	
	/* PushButton Configuration */
	PA0_als_EXTI0();
  
	/* LED Configuration */
  LED_Config();
	
	/* Timer config */
	TIM_Config();

	/* If SPI is already configured by timer do nothing */
	if(SPI2->CR1 == 0x00000000) {
 			SPI_Config(2);
			/*  in SPI_Config() are more modes to be selected, look in spi_config.c
			 *	commandbyte is sent in SPI_Config() too.
			 */
 	}
	
  while (1){  
  }
}

/**
  * @brief  Setup an interval timer
  * @param  None
  * @retval None
  */

void LED_Config(void){
  /* GPIOD Periph clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

}

void TIM4_IRQHandler(void){
  if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET){
    GPIO_ToggleBits(GPIOD, GPIO_Pin_12);
		
			/* If SPI is already configured by main do nothing */
		if(SPI2->CR1 == 0x00000000) {
 			SPI_Config(2);
		}

		/* 
		 * 	Reading Data From ADT7320 SPI needs following definitions
		 *	First: Sending the CommandByte, 0x50: Read from Temperature Register 0x02
		 *	Second: Reading 16bit Temperature Sequence whilst pulling 16 dummy bits into the temperature register
		 */
		
		SPI_SendByte(0x50);
 		data = SPI_Send2Byte(0x0000);
			
		//if( data < 0x8000){ //Temperature is postive
			if( data > 0x0C80 ){ //0x0C80 are 3200d, 25.0°C
				GPIO_ResetBits(GPIOD, GPIO_Pin_15);
				GPIO_SetBits(GPIOD, GPIO_Pin_14);
			} else {
				GPIO_SetBits(GPIOD, GPIO_Pin_15);
				GPIO_ResetBits(GPIOD, GPIO_Pin_14);
			}
		//}
		//calculateTemp(data);
			
    TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
  }
}

void calculateTemp(uint16_t curTemp){
		uint16_t tmp = curTemp;
		if ( (tmp >> 15 & 0x0001) == 0x0001 ) {
			//temperature is negative
			temperature = (curTemp - 65536) / 128;
		} else {
			//temperature is positive
			temperature = curTemp / 128;
		}
}

void Delay(__IO uint32_t nCount)
{
  while(nCount--)
  {
		
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
