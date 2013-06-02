//--------------------------------------------------------------
// File     : tim_config.h
//--------------------------------------------------------------

//--------------------------------------------------------------
#ifndef __MAIN_H
#define __MAIN_H

//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_spi.h"
#include "stm32f4xx_tim.h"



//--------------------------------------------------------------
// Globale Funktionen
//--------------------------------------------------------------
void LED_Config(void);
void PA0_als_EXTI0(void);
int main(void);
void TIM4_IRQHandler(void);

int i;

union { 
	uint16_t word; 
	uint8_t byte[2]; 
} data;

uint8_t id;

// Debugging Variables 
uint8_t status, conf, id, Thyst;
union { 
	uint16_t word; 
	uint8_t byte[2]; 
} Tcrit, Thigh, Tlow;



//--------------------------------------------------------------
#endif // __SPI_CONFIG_H
