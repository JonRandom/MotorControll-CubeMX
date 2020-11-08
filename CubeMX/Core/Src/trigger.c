
/* *****************************************************************
 *
 * File:  trigger.c
 * Autor: Jonathan Fulcher
 * Datum: 29.10.2020
 *
 * implementiert einen Trigger Pin
 *
 ***************************************************************** */

#include "stm32f10x_lib.h"


#define TRIGGER_PORT			GPIOB
#define TRIGGER_PIN				15

#define TRIGGER_ADC_PORT	GPIOB
#define TRIGGER_ADC_PIN		6


void triggerInit()
{
	GPIOB->CRH &= 0x0FFFFFFF; // Bits von Pin15 löschen
	GPIOB->CRH |= 0x20000000; // Bits von Pin15 als Output 2Mhz
}


void triggerAdcInit()
{
	GPIOB->CRL &= 0xF0FFFFFF; // Bits von Pin6 löschen
	GPIOB->CRL |= 0x02000000; // Bits von Pin6 als Output 2Mhz
}


void triggerOn()
{
	TRIGGER_PORT->ODR |= (1 << TRIGGER_PIN);
}


void triggerAdcOn()
{
	TRIGGER_ADC_PORT->ODR |= (1 << TRIGGER_ADC_PIN);
}


void triggerOff()
{
	TRIGGER_PORT->ODR &= ~(1 << TRIGGER_PIN);
}


void triggerAdcOff()
{
	TRIGGER_ADC_PORT->ODR &= ~(1 << TRIGGER_ADC_PIN);
}

