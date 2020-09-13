/******************** (C) COPYRIGHT 2003-2004 STMicroelectronics ********************
* File Name          : STM32_LP_Board.h
* Author             : Alec Bath
* Date First Issued  : Oct 15, 2007
* Description        : Hardware Description Header File
********************************************************************************/

#include "stm32f10x_lib.h"


#define SWITCH_PORT			GPIOA				    // PA
#define SWITCH_SWA			GPIO_Pin_0			// PA.00 (Wakeup)
#define SWITCH_SWB			GPIO_Pin_1			// PA.01

#define LED_PORT			  GPIOB				    // PB
#define LED1	  		    GPIO_Pin_8			// PB.08
#define LED2  			    GPIO_Pin_9			// PB.09

#define POT_PORT			  GPIOC				    // PC
#define POT_RV1	        GPIO_Pin_2			// PC2 (ADC Ch12)
#define POT_RV2	        GPIO_Pin_1			// PC1 (ADC Ch11)

#define CTRL_3V_PORT		GPIOB				    // PB
#define CTRL_3V	  		  GPIO_Pin_5		  // PB.05

#define SPKR_PORT	  	  GPIOA				    // PA
#define SPKR    			  GPIO_Pin_8		  // PA.08 (TIM1CH1)

#define UART1_PORT		  GPIOA				    // PA (USART1 - Debug UART)
#define UART1_TX			  GPIO_Pin_9		  // PA.09
#define UART1_RX			  GPIO_Pin_10		  // PA.10

#define IRDA_PORT		    GPIOB				    // PB (USART3)
#define IRDA_TX	  		  GPIO_Pin_10		  // PB.10
#define IRDA_RX		  	  GPIO_Pin_11		  // PB.11
#define IRDA_SD		  	  GPIO_Pin_1		  // PB.01

#define SPI1_PORT			  GPIOA				    // PA
#define SPI1_nSS 			  GPIO_Pin_4			// PA.04: SPI1 nSS
#define LIS_nCS 			  SPI1_nSS			  // LIS3L02DS CS (PA.04: SPI1 nSS)
#define SPI1_CLK 			  GPIO_Pin_5			// PA.05: SPI1 SCK
#define SPI1_MISO			  GPIO_Pin_6			// PA.06: SPI1 MISO
#define SPI1_MOSI 			GPIO_Pin_7			// PA.07: SPI1 MOSI
                                               
#define SPI2_PORT			  GPIOB				    // PB
#define SPI2_nSS 			  GPIO_Pin_12			// PB.12: SPI2 nSS
#define M25_nCS 			  SPI2_nSS		  	// M25Pxx CS (PB.12: SPI2 nSS)
#define SPI2_CLK 			  GPIO_Pin_13			// PB.13: SPI2 SCK
#define SPI2_MISO			  GPIO_Pin_14			// PB.14: SPI2 MISO
#define SPI2_MOSI 			GPIO_Pin_15			// PB.15: SPI2 MOSI

#define TIM_PORT	      GPIOB				    // PB
#define TIM0CH1	  		  GPIO_Pin_0		  // PB.00  (J106-3)
#define TIM4CH1	  		  GPIO_Pin_6		  // PB.06 (J106-6)
#define TIM4CH2	  		  GPIO_Pin_7		  // PB.07 (J106-5)

#define AIN_PORT	      GPIOA				    // PA
#define AIN2	    		  GPIO_Pin_2		  // PA.02 (ADC2 - J106-2)
#define AIN3  	  		  GPIO_Pin_3		  // PA.03 (ADC3 - J106-1)
#define PC13_PORT	      GPIOC				    // PC
#define PC13	    		  GPIO_Pin_13		  // PC.13 (Anti Tamper - J106-4)



