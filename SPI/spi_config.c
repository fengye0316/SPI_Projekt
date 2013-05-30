//--------------------------------------------------------------
// File     : spi_config.c
// Datum    : 19.05.2013
// Autor    : Matthias Rohm
// Hinweis  : mögliche Pinbelegungen
//            SPI2 : SCK :[PB10, PB13] 
//                   MOSI:[PB15, PC3]
//                   MISO:[PB14, PC2]
//--------------------------------------------------------------

#include "spi_config.h"

extern GPIO_InitTypeDef  GPIO_InitStructure;
SPI_InitTypeDef  SPI_InitStructure;

void SPI_Config(int mode){

  // SPI-Clock enable
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);  

  // Clock Enable der Pins
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

  // SPI Alternative-Funktions mit den IO-Pins verbinden
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_SPI2);	//SCK PB13
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_SPI2);	//MISO PB14
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_SPI2);	//MOSI PB15

  // SPI als Alternative-Funktion mit PullDown
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  if( mode == 0 ) {
     SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
     SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
   }else if( mode == 1) {
     SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
     SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
   }else if( mode == 2) {
     SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
     SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
   }else {
     SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
     SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
   }
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI2_Prescaler;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  //SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI2, &SPI_InitStructure); 

  // SPI enable
  SPI_Cmd(SPI2, ENABLE); 

	/* Configure GPIO for CSE */
	/* GPIOB Periph clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

  /* Configure PDB11 in output pushpull mode */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/*
	 *	Configure Chip in 16 Bit mode
	 */
	 
	/* Send command byte */
	SPI_SendByte(0x08);
	
	/* Send data byte ( 16 bit, continous operation , comparator enabled ) */
	SPI_SendByte(0x90);

}


//--------------------------------------------------------------
// sendet und empfängt ein Byte per SPI2
// ChipSelect-Signal muss von rufender Funktion gemacht werden
//--------------------------------------------------------------
uint8_t SPI_SendByte(uint8_t val)
{ 
  uint8_t curData=0;
	
	/* wait until send is complete */
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);

	/* Enable CSE */
	GPIO_ResetBits(GPIOB, GPIO_Pin_11);
	
  /* Send Byte */
  SPI_I2S_SendData(SPI2, val);

  /* wait until receive is ready */
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET); 

  /* parse data to variable */
  curData=SPI_I2S_ReceiveData(SPI2);
	
	/* Disable CSE */
	GPIO_SetBits(GPIOB, GPIO_Pin_11);

  return(curData);
}

uint16_t SPI_Send2Byte(uint16_t val)
{ 
  uint16_t curData=0;
	
	/* Setting SPI interface to 16 Bit Data Size */
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;
	SPI_Init(SPI2, &SPI_InitStructure);

	/* wait until send is complete */
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);

	/* Enable CSE */
	GPIO_ResetBits(GPIOB, GPIO_Pin_11);
	
   /* Send Byte */
  SPI_I2S_SendData(SPI2, val);

  /* wait until receive is ready */
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET); 

  /* parse data to variable */
  curData=SPI_I2S_ReceiveData(SPI2);
	
	/* Disable CSE */
	
	GPIO_SetBits(GPIOB, GPIO_Pin_11);

	/* (Re)Setting SPI interface to 8 Bit Data Size */
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_Init(SPI2, &SPI_InitStructure);
	
  return(curData);
}

