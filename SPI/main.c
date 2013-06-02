/* Includes ------------------------------------------------------------------*/
#include "stm32f4_discovery.h"
#include "stm32f4xx_tim.h"
#include "spi_config.h"
#include "tim_config.h"
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
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
/* Private functions ---------------------------------------------------------*/

int main(void){
	/* Init of NVIC */
	NVIC_PriorityGroupConfig (NVIC_PriorityGroup_2);
  
	/* LED Configuration */
  	LED_Config();
	
	/* Timer config */
	TIM_Config();

	/* If SPI is already configured by timer do nothing */
	if(SPI2->CR1 == 0 ) {
 			SPI_Config();
 	}
  	while (1){  
  	}
}

/*
 * TIM4 triggers this interrupt, we got a couple of LEDS
 * 12: Green, toggles every time the interrupt is triggered
 * 13: Orange: is set when SPI is NOT Configured
 * 14: Red: is set when Temperature is above value
 * 15: Blue: is set when Temperature is below value
 */

void TIM4_IRQHandler(void){
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET){
		/* check if SPI2 is already configured */
		if (SPI2->CR1 == 0 ){	
			/* Configure SPI, if not happend until now */
 			SPI_Config();
 			GPIO_SetBits(GPIOD, GPIO_Pin_13);
		} else {
			/* Do some blinky for "seeing" something */
			GPIO_ResetBits(GPIOD, GPIO_Pin_13);
			GPIO_ToggleBits(GPIOD, GPIO_Pin_12);
	
			/* read id of SPI Sensor */
			SPI_SendByte(0x58);
			id = SPI_SendByte(0x00);
		
			if( id == 0xC3 ){ // if id is shown correctly do read			
			// Start Debug
				SPI_SendByte(0x40);
				status = SPI_SendByte(0x00);
				SPI_SendByte(0x48);
				conf = SPI_SendByte(0x00);
				SPI_SendByte(0x58);
				id = SPI_SendByte(0x00);
				SPI_SendByte(0x60);
				Tcrit.byte[1] = SPI_SendByte(0x00);
				Tcrit.byte[0] = SPI_SendByte(0x00);
				SPI_SendByte(0x68);
				Thyst = SPI_SendByte(0x00);
				SPI_SendByte(0x70);
				Thigh.byte[1] = SPI_SendByte(0x00);
				Thigh.byte[0] = SPI_SendByte(0x00);
				SPI_SendByte(0x78);
				Tlow.byte[1] = SPI_SendByte(0x00);
				Tlow.byte[0] = SPI_SendByte(0x00);
			// End Debug
		
			/* 
			 * 	Reading Data From ADT7320 SPI needs following definitions
			 *	First: Sending the CommandByte, 0x50: Read from Temperature Register 0x02
			 *	Second: Reading 16bit Temperature Sequence whilst pulling 16 dummy bits into the temperature register
			 */
				SPI_SendByte(0x50);
				data.byte[1] = SPI_SendByte(0x00);
				data.byte[0] = SPI_SendByte(0x00);	
			
				if( data.word > 0x0D00 ){ //0x0D00 are 26�C
					GPIO_ResetBits(GPIOD, GPIO_Pin_15);
					GPIO_SetBits(GPIOD, GPIO_Pin_14);
				} else {
					GPIO_SetBits(GPIOD, GPIO_Pin_15);
					GPIO_ResetBits(GPIOD, GPIO_Pin_14);
				}
				calTemp(data.word);
				
			/*
			 * Does STM32F4 has floating point extension?
			 * if yes, how? float and double are not working
			 * if no: value of PS/2 has to be multiplied with 128.
			 */
			} else {
				SPI_Reset();
				SPI_SendConfig();
			} 
 		}
	TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	}
}

void calTemp(uint16_t temp){
	uint16_t tmp = temp;
	/* check whether temp is positive or negative */
	if( tmp >> 15 & 0x0001 ){
		temperature_here = ( (double) temp - 65536) / 128;
	} else {
		temperature_here = (double) temp / 128;
	}
}

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
