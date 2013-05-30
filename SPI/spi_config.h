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


typedef enum {
  SPI_MODE_0 = 0,  // CPOL=0, CPHA=0
  SPI_MODE_1,      // CPOL=0, CPHA=1
  SPI_MODE_2,      // CPOL=1, CPHA=0
  SPI_MODE_3       // CPOL=1, CPHA=1 
} SPI2_Mode_t;

#define SPI2_Prescaler     SPI_BaudRatePrescaler_64  // Frq = 656.2 kHz

// typedef struct {
//   GPIO_TypeDef* PORT;     // Port
//   const uint16_t PIN;     // Pin
//   const uint32_t CLK;     // Clock
//   const uint8_t SOURCE;   // Source
// }SPI2_PIN_t; 


// typedef struct {
//   SPI2_PIN_t  SCK;        // SCK-Pin
//   SPI2_PIN_t  MOSI;       // MOSI-Pin
//   SPI2_PIN_t  MISO;       // MISO-Pin
// }SPI2_DEV_t;


void SPI_Config(int mode);
uint8_t SPI_SendByte(uint8_t val);
uint16_t SPI_Send2Byte(uint16_t val);


//--------------------------------------------------------------
#endif // __SPI_CONFIG_H
