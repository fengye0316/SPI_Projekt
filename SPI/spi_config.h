//--------------------------------------------------------------
// File     : spi_config.h
//--------------------------------------------------------------

//--------------------------------------------------------------
#ifndef __SPI_CONFIG_H
#define __SPI_CONFIG_H

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_spi.h"

#define SPI2_Prescaler     SPI_BaudRatePrescaler_64  // Frq = 656.2 kHz

void SPI_Config(void);
uint8_t SPI_SendByte(uint8_t val);
void SPI_Reset(void);
void SPI_SendConfig(void);


//--------------------------------------------------------------
#endif // __SPI_CONFIG_H
