/******************** (C) COPYRIGHT 2007 STMicroelectronics ********************
* File Name          : SystemConfig.c
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
#include "SystemConfig.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
vu32 TimingDelay;   // Systick delay() global in milliseconds

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : SystemConfig_Startup
* Description    : Main program
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
u32 SystemConfig_Startup(void)
{
  RCC_Configuration();
//  NVIC_Configuration();
  GPIO_Configuration();
//  SysTick_Configuration(72000000);
///////////////////////////  SysTick_Configuration(64000000);
//  DMA_Configuration();
//  ADC1_Configuration();
///////////////////////////  Delay(100);   // soft start delay

  return (0);
}

/*******************************************************************************
* Function Name  : RCC_Configuration
* Description    : Configures the different system clocks.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RCC_Configuration(void)
{

  /* RCC system reset(for debug purpose) */
  RCC_DeInit();
  //RCC_HSEConfig(RCC_HSE_ON);      /* Enable HSE */
  //HSEStartUpStatus = RCC_WaitForHSEStartUp();   /* Wait till HSE is ready */

    RCC_HCLKConfig(RCC_SYSCLK_Div1);      // HCLK = SYSCLK
    RCC_PCLK2Config(RCC_HCLK_Div1);       // PCLK2 = HCLK
    //RCC_PCLK2Config(RCC_HCLK_Div2);       // PCLK2 = HCLK/2
    RCC_PCLK1Config(RCC_HCLK_Div2);       // PCLK1 = HCLK/2
    //RCC_ADCCLKConfig(RCC_PCLK2_Div6);     // ADCCLK = PCLK2/6 = 12MHz
    RCC_ADCCLKConfig(RCC_PCLK2_Div8);     // ADCCLK = PCLK2/8 = 9MHz or 4.5MHz
  
    FLASH_SetLatency(FLASH_Latency_2);        // Flash: 2 wait states (HCLK > 48MHz)
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);  // Enable Prefetch Buffer

//    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_7);        /* PLLCLK = 8MHz * 7 = 56 MHz */
    RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_16);        /* PLLCLK = 4MHz * 16 = 64 MHz */
    RCC_PLLCmd(ENABLE);       /* Enable PLL */ 
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);        /* Wait till PLL is ready */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);        /* Select PLL as system clock source */
    while(RCC_GetSYSCLKSource() != 0x08);        /* Wait till PLL is used as system clock source */

  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA, ENABLE);   // Enable DMA clock
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,  ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);


//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,  ENABLE);

//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,  ENABLE);
//  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,  ENABLE);


  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

}

/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : Configures the different GPIO ports.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  GPIO_DeInit(GPIOA);
  GPIO_DeInit(GPIOB);
  GPIO_DeInit(GPIOC);
  GPIO_DeInit(GPIOD);
  GPIO_DeInit(GPIOE);


  // Configure relay outputs:
  GPIO_InitStructure.GPIO_Pin = K3_CTRL | K5_CTRL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  // Configure relay outputs:
  GPIO_InitStructure.GPIO_Pin = K1_CTRL | K2_CTRL | K4_CTRL | K6_CTRL | K7_CTRL | K8_CTRL | K9_CTRL | K10_CTRL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_14 ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

}

/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : Configures Vector Table base location.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

#ifdef  VECT_TAB_RAM  
  /* Set the Vector Table base location at 0x20000000 */ 
  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
#else  /* VECT_TAB_FLASH  */
  /* Set the Vector Table base location at 0x08000000 */ 
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
#endif

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);   //Pre-emption priority: 2(0-3), Subpriority: 2(0-3), lower # is higher priority

  // Enable DMA CH1 Interrupt (ADC trigger)
  NVIC_InitStructure.NVIC_IRQChannel = DMAChannel1_IRQChannel;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

// enable tim4 interrupt
  NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQChannel;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);


}

/*******************************************************************************
* Function Name  : SysTick_Configuration
* Description    : Initialize Systick timer for 1mS ticks
* Input          : HCLK value, in Hz
* Output         : None
* Return         : None
*******************************************************************************/
void SysTick_Configuration(u32 HCLK_value)
{
  //u32 ReloadVal;

  //ReloadVal = HCLK_value / 8 / 1000;  // Get 1mS tick reload value
  SysTick_SetReload(HCLK_value/8/1000);     // 1mS SysTick end of count event (HCLK/8/xxxx=1KHz)
  SysTick_ITConfig(ENABLE);         // Enable SysTick interrupt
}


/*******************************************************************************
* Function Name  : Delay
* Description    : Inserts a delay time.
* Input          : nTime: specifies the delay time length, in milliseconds.
* Output         : None
* Return         : None
*******************************************************************************/
void Delay(u32 Time_mS)
{
  SysTick_CounterCmd(SysTick_Counter_Enable);  /* Enable the SysTick Counter */
  TimingDelay = Time_mS;

  while(TimingDelay != 0);

  SysTick_CounterCmd(SysTick_Counter_Disable);    /* Disable SysTick Counter */
  SysTick_CounterCmd(SysTick_Counter_Clear);      /* Clear SysTick Counter */
}



//int SendChar (int ch)
void printf_swv (int ch)            // Write character ch to SWV Serial Port
{ 
  //while (ITM_Port32(0) == 0);
//  ITM_Port8(0) = ch;
  //ITM_Port32(0) = ch;
  
  while (ITM_Port32(0) == 0) ITM_Port32(0) = ch;

  // return (ch);
}


/******************* (C) COPYRIGHT 2007 STMicroelectronics *****END OF FILE****/
