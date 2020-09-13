/******************** (C) COPYRIGHT 2007 STMicroelectronics ********************
* File Name          : SystemConfig.h
* Author             : MCD Application Team
* Date First Issued  : 02/05/2007
* Description        : Configure clocks, memory and peripherals
********************************************************************************
* History:
* August 21, 2007: V1
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_lib.h"
#include "cortexm3_macro.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define  K3_CTRL    GPIO_Pin_7    // PC7
#define  K5_CTRL    GPIO_Pin_6    // PC6
#define  K7_CTRL    GPIO_Pin_15   // PD15
#define  K8_CTRL    GPIO_Pin_14   // PD14
#define  K9_CTRL    GPIO_Pin_13   // PD13
#define  K10_CTRL   GPIO_Pin_12   // PD12
#define  K2_CTRL    GPIO_Pin_11   // PD11
#define  K4_CTRL    GPIO_Pin_10   // PD10
#define  K6_CTRL    GPIO_Pin_9    // PD9
#define  K1_CTRL    GPIO_Pin_8    // PD8

#define ADC1_DR_Address    ((u32)0x4001244C)
#define ITM_Port32(n) (*((volatile unsigned long *)(0xE0000000+(4*n))))


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/
u32 SystemConfig_Startup(void);
void RCC_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);
void SysTick_Configuration(u32 HCLK_value);
void Delay(u32 Time_mS);
void printf_swv (int ch);


    

/******************* (C) COPYRIGHT 2007 STMicroelectronics *****END OF FILE****/
