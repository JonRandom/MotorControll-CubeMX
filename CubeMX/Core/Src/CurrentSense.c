
/* *****************************************************************
 *
 * File:  currentSense.c
 * Autor: Jonathan Fulcher
 * Datum: 03.01.2021
 *
 * implementiert den SPI Current Sensor TLI4970-D050T5
 *
 * Benutzung:
 *
 * todo:
 *
 ***************************************************************** */

/* Includes ------------------------------------------------------------------*/
#include "SystemConfig.h"


SPI_InitTypeDef spiInitStruct =
{
  .SPI_Direction 					= SPI_Direction_2Lines_FullDuplex,
  .SPI_Mode 							= SPI_Mode_Master,
  .SPI_DataSize						= SPI_DataSize_8b,
  .SPI_CPOL								= SPI_CPOL_Low,
  .SPI_CPHA								= SPI_CPHA_1Edge,
  .SPI_NSS								= SPI_NSS_Soft,
  .SPI_BaudRatePrescaler	= SPI_BaudRatePrescaler_16,
  .SPI_FirstBit						= SPI_FirstBit_MSB,
  .SPI_CRCPolynomial			= 0
};


void currentSenseInit()
{
	SPI_Init(SPI2, &spiInitStruct);

}
