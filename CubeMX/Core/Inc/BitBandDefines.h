/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : BitBandDefines.h
* Author             : Alec Bath
* Date First Issued  : Feb 11, 2008
* Description        : Defines for Bit-band addressing



Example usage in C source file:
================================
void TIM3_IRQHandler(void)
{

  if (TIM3_CC3IF_BB)        // TIM3-CC3 irq? 
  {
    TIM3_CC3IF_BB = 0;      // Clear irq flag bit
    TIM3->CCR3 += 50000;    // update CC3 register
    ...
  }

  if (TIM3_CC4IF_BB)        // TIM3-CC4 irq?
  {
    TIM3_CC4IF_BB = 0;      // Clear irq flag bit
    ...
  }
}



********************************************************************************/



//#define TIM2_CEN_BB       *(vu16 *) (PERIPH_BB_BASE + (0x0000*32) + (0*4))   // TIM2 CEN bit:   BBADR + TIM2 CR1 + bit#0
#define TIM2_CEN_BB       *(vu16 *) (PERIPH_BB_BASE + (((TIM2_BASE - PERIPH_BASE)+0x0000)*32) + (0*4))   // TIM2 CEN bit: TIM2_CR1-bit#0


#define TIM2_UIF_BB       *(vu16 *) (PERIPH_BB_BASE + (0x0010*32) + (0*4))   // TIM2 UIF bit:   BBADR + TIM2 SR + bit#0
#define TIM2_CC1IF_BB     *(vu16 *) (PERIPH_BB_BASE + (0x0010*32) + (1*4))   // TIM2 CC1IF bit: BBADR + TIM2 SR + bit#1
#define TIM2_CC2IF_BB     *(vu16 *) (PERIPH_BB_BASE + (0x0010*32) + (2*4))   // TIM2 CC2IF bit: BBADR + TIM2 SR + bit#2
#define TIM2_CC3IF_BB     *(vu16 *) (PERIPH_BB_BASE + (0x0010*32) + (3*4))   // TIM2 CC3IF bit: BBADR + TIM2 SR + bit#3
#define TIM2_CC4IF_BB     *(vu16 *) (PERIPH_BB_BASE + (0x0010*32) + (4*4))   // TIM2 CC4IF bit: BBADR + TIM2 SR + bit#4

#define TIM2_UG_BB        *(vu16 *) (PERIPH_BB_BASE + (0x0014*32) + (0*4))   // TIM2 UG bit:    BBADR + TIM2 EGR + bit#0
#define TIM2_CC2G_BB      *(vu16 *) (PERIPH_BB_BASE + (0x0014*32) + (2*4))   // TIM2 CC2G bit:  BBADR + TIM2 EGR + bit#2


#define TIM3_CEN_BB       *(vu16 *) (PERIPH_BB_BASE + (0x0400*32) + (0*4))   // TIM3 CEN bit:   BBADR + TIM3 CR1 + bit#0
#define TIM3_UG_BB        *(vu16 *) (PERIPH_BB_BASE + (0x0414*32) + (0*4))   // TIM3 UG bit:    BBADR + TIM3 EGR + bit#0
#define TIM3_CC1G_BB      *(vu16 *) (PERIPH_BB_BASE + (0x0414*32) + (1*4))   // TIM3 CC1G bit:  BBADR + TIM3 EGR + bit#1
#define TIM3_CC2G_BB      *(vu16 *) (PERIPH_BB_BASE + (0x0414*32) + (2*4))   // TIM3 CC2G bit:  BBADR + TIM3 EGR + bit#2
#define TIM3_CC3G_BB      *(vu16 *) (PERIPH_BB_BASE + (0x0414*32) + (3*4))   // TIM3 CC3G bit:  BBADR + TIM3 EGR + bit#3
#define TIM3_CC4G_BB      *(vu16 *) (PERIPH_BB_BASE + (0x0414*32) + (4*4))   // TIM3 CC4G bit:  BBADR + TIM3 EGR + bit#4

#define TIM3_CC1IF_BB     *(vu16 *) (PERIPH_BB_BASE + (0x0410*32) + (1*4))   // TIM3 CC1IF bit: BBADR + TIM3 SR + bit#1
#define TIM3_CC2IF_BB     *(vu16 *) (PERIPH_BB_BASE + (0x0410*32) + (2*4))   // TIM3 CC2IF bit: BBADR + TIM3 SR + bit#2
#define TIM3_CC3IF_BB     *(vu16 *) (PERIPH_BB_BASE + (0x0410*32) + (3*4))   // TIM3 CC3IF bit: BBADR + TIM3 SR + bit#3
#define TIM3_CC4IF_BB     *(vu16 *) (PERIPH_BB_BASE + (0x0410*32) + (4*4))   // TIM3 CC4IF bit: BBADR + TIM3 SR + bit#4

#define TIM4_UIF_BB       *(vu16 *) (PERIPH_BB_BASE + (0x0810*32) + (0*4))   // TIM4 UIF bit:   BBADR + TIM4 SR + bit#0
#define TIM4_CC1IF_BB     *(vu16 *) (PERIPH_BB_BASE + (0x0810*32) + (1*4))   // TIM4 CC1IF bit: BBADR + TIM4 SR + bit#1
#define TIM4_CC2IF_BB     *(vu16 *) (PERIPH_BB_BASE + (0x0810*32) + (2*4))   // TIM4 CC2IF bit: BBADR + TIM4 SR + bit#2
#define TIM4_CC3IF_BB     *(vu16 *) (PERIPH_BB_BASE + (0x0810*32) + (3*4))   // TIM4 CC3IF bit: BBADR + TIM4 SR + bit#3
#define TIM4_CC4IF_BB     *(vu16 *) (PERIPH_BB_BASE + (0x0810*32) + (4*4))   // TIM4 CC4IF bit: BBADR + TIM4 SR + bit#4


#define ADC1_JEOC_BB      *(vu16 *) (PERIPH_BB_BASE + (0x12400*32) + (2*4))   // ADC bit: BBADR + ADC1 SR + bit#2


#define BITBAND_SRAM_REF   0x20000000
#define BITBAND_SRAM_BASE  0x22000000
#define BITBAND_SRAM(a,b) ((BITBAND_SRAM_BASE + (a-BITBAND_SRAM_REF)*32 + (b*4)))  // Convert SRAM address
#define BITBAND_PERI_REF   0x40000000
#define BITBAND_PERI_BASE  0x42000000
#define BITBAND_PERI(a,b) ((BITBAND_PERI_BASE + (a-BITBAND_PERI_REF)*32 + (b*4)))  // Convert PERI address

//#define MAILBOX   0x20003000
//#define StateBitBand0    *((volatile unsigned int *)  (BITBAND_SRAM(MAILBOX,0)))     // Bit 0
//#define StateBitBand1    *((volatile unsigned int *)  (BITBAND_SRAM(MAILBOX,1)))     // Bit 1
//#define StateBitBand2    *((volatile unsigned int *)  (BITBAND_SRAM(MAILBOX,2)))     // Bit 2
//    MBX_B0 = 1;      // Word write
//    temp = MBX_B7;   // Word read

// StateBitBand = 0x20004100 --> 0x22000000 + (0x4100*32) + (b*4) = 0x22082000 - 0x22082080
#define StateBitBandBase  0x20004C00    // top 1KB of SRAM reserved: 0x20004C00-0x20004FFF (in linker settings)
#define StateBitBand0   *((volatile unsigned int *)  (BITBAND_SRAM(StateBitBandBase,0)))     // Bit 0
#define StateBitBand1   *((volatile unsigned int *)  (BITBAND_SRAM(StateBitBandBase,1)))     // Bit 1
//#define StateBitBand0   0x22082000




/*   *****  FROM stm32f10x_map.h file defines:  *****

*********************************************************
NOTE:  You cannot perform bit-banding on DMA registers!
  It will generate a hard fault exception
*********************************************************

#define PERIPH_BB_BASE        ((u32)0x42000000)
#define SRAM_BB_BASE          ((u32)0x22000000)

#define SRAM_BASE             ((u32)0x20000000)
#define PERIPH_BASE           ((u32)0x40000000)

#define FLASH_BASE            ((u32)0x40022000) // Flash registers base address
#define OB_BASE               ((u32)0x1FFFF800)     // Flash Option Bytes base address

#define APB1PERIPH_BASE       PERIPH_BASE                 // Peripheral memory map
#define APB2PERIPH_BASE       (PERIPH_BASE + 0x10000)
#define AHBPERIPH_BASE        (PERIPH_BASE + 0x20000)

#define TIM2_BASE             (APB1PERIPH_BASE + 0x0000)
#define TIM3_BASE             (APB1PERIPH_BASE + 0x0400)
#define TIM4_BASE             (APB1PERIPH_BASE + 0x0800)
#define RTC_BASE              (APB1PERIPH_BASE + 0x2800)
#define WWDG_BASE             (APB1PERIPH_BASE + 0x2C00)
#define IWDG_BASE             (APB1PERIPH_BASE + 0x3000)
#define SPI2_BASE             (APB1PERIPH_BASE + 0x3800)
#define USART2_BASE           (APB1PERIPH_BASE + 0x4400)
#define USART3_BASE           (APB1PERIPH_BASE + 0x4800)
#define I2C1_BASE             (APB1PERIPH_BASE + 0x5400)
#define I2C2_BASE             (APB1PERIPH_BASE + 0x5800)
#define CAN_BASE              (APB1PERIPH_BASE + 0x6400)
#define BKP_BASE              (APB1PERIPH_BASE + 0x6C00)
#define PWR_BASE              (APB1PERIPH_BASE + 0x7000)

#define AFIO_BASE             (APB2PERIPH_BASE + 0x0000)
#define EXTI_BASE             (APB2PERIPH_BASE + 0x0400)
#define GPIOA_BASE            (APB2PERIPH_BASE + 0x0800)
#define GPIOB_BASE            (APB2PERIPH_BASE + 0x0C00)
#define GPIOC_BASE            (APB2PERIPH_BASE + 0x1000)
#define GPIOD_BASE            (APB2PERIPH_BASE + 0x1400)
#define GPIOE_BASE            (APB2PERIPH_BASE + 0x1800)
#define ADC1_BASE             (APB2PERIPH_BASE + 0x2400)
#define ADC2_BASE             (APB2PERIPH_BASE + 0x2800)
#define TIM1_BASE             (APB2PERIPH_BASE + 0x2C00)
#define SPI1_BASE             (APB2PERIPH_BASE + 0x3000)
#define USART1_BASE           (APB2PERIPH_BASE + 0x3800)

#define DMA_BASE              (AHBPERIPH_BASE + 0x0000)
#define DMA_Channel1_BASE     (AHBPERIPH_BASE + 0x0008)
#define DMA_Channel2_BASE     (AHBPERIPH_BASE + 0x001C)
#define DMA_Channel3_BASE     (AHBPERIPH_BASE + 0x0030)
#define DMA_Channel4_BASE     (AHBPERIPH_BASE + 0x0044)
#define DMA_Channel5_BASE     (AHBPERIPH_BASE + 0x0058)
#define DMA_Channel6_BASE     (AHBPERIPH_BASE + 0x006C)
#define DMA_Channel7_BASE     (AHBPERIPH_BASE + 0x0080)
#define RCC_BASE              (AHBPERIPH_BASE + 0x1000)
*/
