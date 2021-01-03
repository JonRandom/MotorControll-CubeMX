
/* Includes ------------------------------------------------------------------*/
#include "SystemConfig.h"
#include "motorcontrolsettings.h"
#include "definitions.h"
#include "motorState.h"



unsigned char overloadflag;
unsigned char flashcounter;
unsigned char overcurrent;
unsigned char zcfound;
unsigned char phase;
unsigned char autostep;
unsigned char risingdelay;
unsigned char ledstate;
unsigned char flashcount;
unsigned char fallingdelay;
unsigned char risingedge;
unsigned char startstate;
unsigned char bemfchannel;
unsigned char run;

unsigned int phaseTimeCounter;

unsigned short maxdc;
unsigned short overloaddclimit;
unsigned short activedclimit;
unsigned short ifboffset;
unsigned short commcounter;
unsigned short step;
unsigned short bemfsample;
unsigned short commcounter;
unsigned short demagcounter;
unsigned short zccounter;
unsigned short potvalue;
unsigned short runningdc;
unsigned short commthreshold;
unsigned short demagthreshold;

unsigned long transitioncounter;
unsigned long position;
unsigned long positionest;
unsigned long heartbeat1time;
unsigned long heartbeat2time;
unsigned long heartbeat3time;
unsigned long globalcounter;
unsigned long holdcounter;
unsigned long alignmentcounter;
unsigned long rampspeed;
unsigned long ledtime;

signed short speedest;
signed short rpm;
signed short rpmcmd;
signed short rpmref;
signed short ifbcount;
signed short ifbave;
signed short ifb;

signed long errorint;
signed long ifbsum;
signed long overloadcounter;

// lookup table to convert phase to mask for tim1 CCER 
const unsigned short ccermask[6]=     
    {
	phab,phac,phbc,phba,phca,phcb
    };

unsigned short adcread( unsigned char chnl);
void PWMISR(void);
void  motorstartinit(void);


ADC_InitTypeDef myAdcInitStruct =
{
    .ADC_Mode								= ADC_Mode_Independent,
    .ADC_ScanConvMode				= DISABLE,
    .ADC_ContinuousConvMode	= ENABLE,
    .ADC_ExternalTrigConv		= ADC_ExternalTrigConv_T1_CC1, // ADC_ExternalTrigConv_None,
    .ADC_DataAlign					= ADC_DataAlign_Right,
    .ADC_NbrOfChannel				= 1
};


/******************************************** ***********************************
 * Function Name  : main
 * Description    : Main program
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
int main(void)
{
  volatile unsigned long long0;
  // unsigned long long1;
  signed long slong0;
  signed long propterm;
  signed long intterm;

  // wait for power supply to stabilize
  long0=0;

  while(long0<1000000) long0++;

  // RCC system reset(for debug purpose)
  // sk RCC_DeInit();

  // sk FLASH_SetLatency(FLASH_Latency_2);        // Flash: 2 wait states (HCLK > 48MHz)
  // sk FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);  // Enable Prefetch Buffer

  debug(); // initializes all periphery pointers based on RCC_BASE

  RCC->CFGR = 0x00100000; // all dividers to minimum, PLL to *6, HSI selected

  long0 =1;

  RCC_PLLCmd(ENABLE);       /* Enable PLL */
  while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);        /* Wait till PLL is ready */
  RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);        /* Select PLL as system clock source */
  while(RCC_GetSYSCLKSource() != 0x08);        /* Wait till PLL is used as system clock source */

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,  ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

  // old: AFIO->MAPR= b25;  // free up PA15
  AFIO->MAPR= b25 + b6;  // free up PA15 and TIM1 = partial remap

  // port A setup
  GPIOA->CRH = 0x34443BBB; // 15,11 as PP, 8,9,10 AF PP, rest inputs
  // old: GPIOA->CRL = 0x00444440; // a7 and a6 and a0 ain
  GPIOA->CRL = 0xB0444000; // a7 is AF PP,  and a1,a2 and a0 are ain

  // port B setup
  // old: GPIOB->CRH = 0xBBB43444; // 15,14,13 AF PP, 11 PP, rest inputs
  GPIOB->CRH = 0x24443444; // 15,14,13 are now floating inputs, 11 PP, rest inputs
  // old: GPIOB->CRL = 0x43444440; // b0 ain, b6 GPIO PP
  GPIOB->CRL = 0x434444BB; // b0, b1 are AF PP, b6 GPIO PP
  GPIOB->ODR &= ~b11;  //clear pb11 to select bemf off time sampling

  // tim1 setup
  TIM1->SMCR = b15; // make ETR input active low
  TIM1->CR2= b9+b11+b13;
  TIM1->CCR1= 0;
  TIM1->CCR2= 0;
  TIM1->CCR3= 0;
  TIM1->CCR4= 40; // 2;		// 100;		// sk: neu, ISR Zeitpunkt in PWM-on Phase verschieben   alt: 1100;
  TIM1->ARR=1200;
  TIM1->CR1=0x0001;
  // b12 to enable brk input
  // TIM1->BDTR= b15+b12+b11+16;  // 2 usec dead time and set MOE
  TIM1->BDTR= b15 + b11 + 16;  // Break enable ausgeschaltet (Test only), 2 usec dead time and set MOE
  // note: b15 b7 and b7 are to enable   based current limit
  TIM1->CCMR1= 0x6868 +b15 + b7;
  TIM1->CCMR2= 0x6868 +b7;
  // b4 for cc4 and b7 for brk interrupt
  TIM1->DIER = b4+b7;  // enable cc4 interrupt

  // DAC setup
  // RCC->APB1ENR |= b29; // enable DAC peripheral clock

  // DAC->CR = b16+b0 ; // enable both DACs

  // ADC setup
//  ADC1->CR2	=	0x00000001;  		// ADDON = 1: turn ADC on
//  ADC_StartCalibration(ADC1);
//  while (ADC_GetCalibrationStatus(ADC1) );
//  ADC1->CR1		= 0x00; 				// neu Single conversion mode, old: 0x00000800;  // enable discontinuous regular conversion
//  ADC1->SMPR2 = 0xDB;					// Sampletime = 28,5 Zyklen -> 011 binär => 0 1101 1011 binär = 0DB hex
//  ADC1->CR2		= 0x00000000;  	// ADDON = 0, CONT = 0: turn ADC on in continuous mode
//  ADC1->CR2		= 0x00000002;  	// ADDON = 0, CONT = 1: turn ADC on in continuous mode
//  ADC1->CR2		= 0x00000003;  	// ADDON = 1, CONT = 1: turn ADC on in continuous mode
//  ADC1->CR2		= 0x00000003 + b22;


  ADC_Init (ADC1, &myAdcInitStruct);
  int adcChannel = 0;
  ADC_RegularChannelConfig (ADC1, adcChannel, 1, ADC_SampleTime_1Cycles5);
  ADC_ExternalTrigConvCmd (ADC1, DISABLE);

  ADC_StartCalibration	 (ADC1);
  while (ADC_GetResetCalibrationStatus(ADC1) != RESET);
  ADC_SoftwareStartConvCmd (ADC1, ENABLE);
  ADC_Cmd (ADC1, ENABLE);

  // initialization
  globalcounter=0;
  heartbeat1time=0;
  heartbeat2time=0;
  heartbeat3time=0;
  run=0;
  motorstartinit();

  // get ifb average offset level
  long0=1;
//long1=0;
//  while(long0<1025)
//    {
//      ADC1->SQR3=0x00000000; // current sense adc channel
//      ADC1->CR2=0x00000001;  // start ADC conversion of bemf
//      while((ADC1->SR & b1)==0) ; // wait for conversion to complete
//      long1 = long1 + ADC1->DR;
//      long0++; // inc loop counter
//    }
//  ifboffset = long1>>10; // average = sum div by 1024

  NVIC_InitTypeDef NVIC_InitStructure;
  // enable tim1 CC interrupt
  NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQChannel;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  // enable tim1 brk interrupt
  NVIC_InitStructure.NVIC_IRQChannel = TIM1_BRK_IRQChannel;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  initMotorState ();

  while (1);


  while (1) // main background loop
  {
    long0 = globalcounter - heartbeat1time;
    if (long0 > 1)
	// ******************** 100 microsecond routine ************************
	{
	  heartbeat1time=globalcounter;

	  // ***** state variable speed observer *****
	  slong0= position - (positionest>>12);// observer position error
	  speedest = slong0;  // store speed estimate
	  // integrate estimated speed into estimated position
	  positionest = positionest + speedest ;

	  // catch eventual rollover of positionest (and position) and prevent
	  if( positionest & 0x80000000 ) // if positionest is greater than half range
	  {
	    positionest= positionest & 0x7FFFFFFF; // subtract 0x80000000
	    position=position-524288; // subtract (0x80000000>>12)
	  }
	} // ***************** end of 100 usec routine **********************

    long0 = globalcounter - heartbeat2time;
    if (long0 > 19)
	// ******************** 1 millisecond routine *************************
	{
	  heartbeat2time=globalcounter;

	  // overload function
	  if ((ifbave > OVERLOAD_THRESHOLD) && !overloadflag)
      {
	    overloadcounter = overloadcounter + OVERLOAD_UP_RATE;
	  }
	  else
	  {
	    overloadcounter = overloadcounter - OVERLOAD_DOWN_RATE;
	  }

	  if (overloadcounter < 0)
	  {
		overloadcounter=0;
		overloadflag=0;
	  }

	  if (overloadcounter > 1000000)
	  {
   	    overloadflag=255;
	  }

	  if ((ifb>OVERLOAD_SETPOINT) && overloadflag)
	  {
	    overloaddclimit = overloaddclimit - 3;
	  }
	  else
	  {
	    overloaddclimit = overloaddclimit + 1;
	  }

	  if (overloaddclimit > 1000)
	  {
		overloaddclimit = 1000;
	  }

	  if(overloaddclimit < 100)
	  {
	    overloaddclimit=100;
	  }

	  // calculate rpm from speedest
	  long0 = (speedest *6250) >> 8;
	  rpm = long0 / POLE_PAIRS;

	  // *************** speed regulator  *********************************
	  slong0 		= rpmref - rpm;  // speed error
	  propterm 		= (slong0 * PROP_GAIN) >> 8;
	  errorint 		= errorint + slong0;
	  // set activedc to the lower of maxdc or overloaddclimit
	  if (maxdc < overloaddclimit)
	  {
		activedclimit = maxdc;
	  }
	  else
	  {
	    activedclimit = overloaddclimit;
	  }

	  slong0 = activedclimit - propterm;
	  if (slong0 < 0)
	  {
		slong0=0; // slong0 holds maximum allowable intterm
	  }

	  slong0 = slong0 * INT_CLAMP_SCALER; // slong0 holds max error integral

	  if (errorint > slong0)
	  {
		 errorint=slong0;
	  }

	  if (errorint < 0)
	  {
	    errorint=0;
	  }

	  intterm = (errorint*INT_GAIN) >> 10;

	  slong0 = propterm + intterm;

	  if (slong0 > activedclimit)
	  {
		slong0 = activedclimit;
	  }

	  if (slong0 < 100)
	  {
	    slong0 = 100;
	  }

	  if ((zcfound) && (transitioncounter < 100))
	  {
		transitioncounter++;
	  }

	  if (transitioncounter < 100)
	  {
	    runningdc 	= 500;
	   	errorint 	= 500 * INT_CLAMP_SCALER;
	   	rpmref 		= rpm;
	  }
	  else
	  {
	    runningdc = slong0;
	  }

	  if (zcfound)
	  {
	    TIM1->CCR1= runningdc;
	    TIM1->CCR2= runningdc;
	    TIM1->CCR3= runningdc;
	  }
	} // ************** end of 1 millisecond routine *************************


    long0 = globalcounter - heartbeat3time;
    if (long0 > 199)
	{
	  // ******************** 10 millisecond routine *************************

	  heartbeat3time=globalcounter;

	  potvalue = 4095 - adcread(2);  // read pot channel

	  //sk: next line
	  potvalue = 2000;

	  if(potvalue>200) run=255;
	  if(potvalue<100) run=0;

	  rpmcmd = potvalue>>2;
	  if (rpmcmd < 100)
	  {
	    rpmcmd = 100;
	  }

	  // accel/decel control
	  slong0 = rpmcmd - rpmref;
	  if (slong0 >  ACCLIM) slong0 = ACCLIM;
	  if (slong0 < -DECLIM) slong0 = -DECLIM;
	  rpmref = rpmref + slong0;

	} // ************** end of 10 millisecond routine *************************


    // background code that runs with every loop
    if(overcurrent)
 	{
   	  flashcount=2;
   	}

    switch(ledstate)
    {
	  case 0:
	    if(flashcount==0)
	    {
	      LED_SWITCH_ON;
	      break;
	    }// led on and break

	    LED_SWITCH_OFF;
	    ledtime		= 0;
	    ledstate	= 10;
	    break;

	  case 10:  // waiting out long pause
	    if((ledtime)<30000)
	   	{
	      break;
	    }

	    ledstate		=20;
	    flashcounter	= 0;
	    break;

  	  case 20: // long pause over
  		LED_SWITCH_ON;
	    ledtime		= 0,
	    ledstate	= 30;
	    break;

	  case 30: // waiting out short on time
	    if (ledtime < 8000)
	    {
	      break;
	    }

	    LED_SWITCH_OFF;
	    ledtime		= 0;
	    ledstate	= 40;
	    break;

	  case 40: // waiting out short off time
	    if (ledtime < 8000)
	    {
	      break;
	    }

	    flashcounter++;
	    if(flashcounter >= flashcount)
	    {
	      ledstate	= 0;
	      break;
	    }

	    ledstate	= 20;
	    break;
      } // end of ledstate switch statement

    } // end of background loop

} // end of main



unsigned short adcread( unsigned char chnl)
{
  unsigned short result;

  ADC1->SQR3=chnl; 			// set adc channel
  ADC1->CR2=1;  			// start ADC conversion of bemf
  while((ADC1->SR & b1)==0) if(ADC1->SQR3!=chnl) break; // wait for conversion to complete
  result= ADC1->DR;

  if(ADC1->SQR3!=chnl)		// read again if conversion was interrupted by foreground
    {
      ADC1->SQR3=chnl; 		// set adc channel
      ADC1->CR2=1;  		// start ADC conversion of bemf
      while((ADC1->SR & b1)==0) if(ADC1->SQR3!=chnl) break; // wait for conversion to complete
      result= ADC1->DR;
    }

  if(ADC1->SQR3!=chnl) 		// must check third time
    {
      ADC1->SQR3=chnl; 		// set adc channel
      ADC1->CR2=1;  		// start ADC conversion of bemf
      while((ADC1->SR & b1)==0) if(ADC1->SQR3!=chnl) break; // wait for conversion to complete
      result= ADC1->DR;
    }
  return result;
} // end of adcread function


// interrupt service routine run just at the end of each PWM cycle
void PWMISR (void)
{
	processMotorState ();
    TIM1->SR	= 0;  		// clear interrupt

}


// interrupt service routine run just at the end of each PWM cycle
void PWMISR_nichtMehrBenutzt(void)
{
  // unsigned long long0;
  unsigned long stepTime;

  ADC1->SQR3	= bemfchannel;
  ADC1->CR2		= 0x00000001;  // start ADC conversion of back emf (bemf)
  zccounter++;  // housekeeping increment of some timers while adc is converting
  alignmentcounter++;
  holdcounter++;
  ledtime++;
  while ((ADC1->SR & b1) == 0) ; // wait for conversion to complete
  bemfsample	= ADC1->DR;

  // sk DAC->DHR12R2 = bemfsample;
  // sk DAC->SWTRIGR=0x00000003;

  if (autostep)
  {
    commcounter++;
    if (commcounter > step)
    {
      commcounter	= 0;
      phase++;
      position++;
      if (phase > 5)
      {
        phase = 0;
      }

      TIM1->CCER = ccermask[phase]; // commutate bridge from table
    }
  } 	  // end of if(autostep)



#define STATE_MOTOR_OFF						0
#define STATE_MOTOR_INIT					5
#define STATE_MOTOR_ALIGNMENT				10
#define STATE_MOTOR_RAMP_UP_X				20
#define STATE_MOTOR_WAIT_HOLD				100
#define STATE_MOTOR_WAIT_PHASE_5			110
#define STATE_MOTOR_WAIT_PHASE0				120
#define STATE_MOTOR_WAIT_DEMAGNET_TIME		130
#define STATE_MOTOR_WAIT_SEARCH_ZEROCROSS	140
#define STATE_MOTOR_WAIT_SEARCH_COMM_DELAY  150

#define	PHASE_0		0
#define	PHASE_1		1
#define	PHASE_2		2
#define	PHASE_3		3
#define	PHASE_4		4
#define	PHASE_5		5

  if (run == 0)
  {
	startstate = STATE_MOTOR_OFF;
  }

  switch(startstate)
  {
    case STATE_MOTOR_OFF:
      TIM1->CCER = alloff;
      if (run)
	{
	  motorstartinit();
	  startstate = STATE_MOTOR_INIT;
	  phase = 0;
	}
      break;

    case STATE_MOTOR_INIT: 				// setup alignment
      TIM1->CCR1	= ALIGNMENT_DC;
      TIM1->CCR2	= ALIGNMENT_DC;
      TIM1->CCR3	= ALIGNMENT_DC;

      // phase=0;

      TIM1->CCER = ccermask[phase]; 	// commutate bridge from table
      alignmentcounter = 0;

      startstate = STATE_MOTOR_ALIGNMENT;

      if (phaseTimeCounter++ > 40)
      {
    	  phaseTimeCounter = 0;
    	  phase++;
    	  if (phase > 5)
    	  {
    		  phase = 0;
    	  }
      }
      break;

    case STATE_MOTOR_ALIGNMENT: 		// timing out alignment
      if (alignmentcounter > ALIGNMENT_TIME)
      {
    	  rampspeed 	= 1;
    	  commcounter	= 0;
    	  autostep		= 255;
    	  TIM1->CCR1	= RAMPUP_DC;
    	  TIM1->CCR2	= RAMPUP_DC;
    	  TIM1->CCR3	= RAMPUP_DC;
    	  startstate	= STATE_MOTOR_RAMP_UP_X;
      }
      break;

    case STATE_MOTOR_RAMP_UP_X:
      rampspeed = rampspeed + RAMPUP_RATE;
      stepTime 	= 4000000000;
      stepTime 	= stepTime / rampspeed;
      if (stepTime > 10000) // 30000)
      {
    	  stepTime = 10000; // 30000;
      }

      step 		= stepTime;

      if (step <= MIN_STEP)	// zur Zeit nach 4336 Durchläufen, d,h, nach 0,216s
      {
    	  holdcounter	= 0;
    	  startstate	= STATE_MOTOR_WAIT_HOLD;
      }
      break;

    case STATE_MOTOR_WAIT_HOLD: // wait for hold time
      if (holdcounter > HOLD_TIME)	// nach 2000 * 50us = 100ms
      {
    	  startstate = STATE_MOTOR_WAIT_PHASE_5;
      }
      break;

    case STATE_MOTOR_WAIT_PHASE_5: // wait to get into phase 5
      if (phase == PHASE_5)
      {
    	  // startstate	= STATE_MOTOR_WAIT_PHASE0;
      }
      break;

    case STATE_MOTOR_WAIT_PHASE0: // wait for leading edge of phase 0 (commutation)
      if (phase ==  PHASE_0)
      {
    	  demagcounter 		= 0;
    	  demagthreshold 	= (step * DEMAG_ALLOWANCE) >> 8;
    	  startstate		= STATE_MOTOR_WAIT_DEMAGNET_TIME;
      }
      break;

    case STATE_MOTOR_WAIT_DEMAGNET_TIME: // wait out demag time
      demagcounter++;
      if (demagcounter > demagthreshold)
      {
    	  // test only sk: startstate	= STATE_MOTOR_WAIT_SEARCH_ZEROCROSS;
      }
      break;

    case STATE_MOTOR_WAIT_SEARCH_ZEROCROSS: // looking for zero crossing of bemf
      if (risingedge)
      {
    	if (bemfsample > ZC_THRESHOLD)
    	{
    	  if (zcfound)
    	  {
    	    step = zccounter;
    	  }
    	  commthreshold = (step * risingdelay) >> 8;
    	  zccounter		= 0;
    	  commcounter	= 0;
    	  startstate	= STATE_MOTOR_WAIT_SEARCH_COMM_DELAY;
    	  risingedge	= 0;
    	  zcfound		= 255;
    	  autostep		= 0;
	    }
	  }
      else
	  {
    	if (bemfsample < ZC_THRESHOLD)
	    {
    	  if (zcfound)
    	  {
    		step = zccounter;
    	  }
    	  commthreshold 	= (step * fallingdelay) >> 8;
    	  zccounter		= 0;
    	  commcounter		= 0;
    	  startstate		= STATE_MOTOR_WAIT_SEARCH_COMM_DELAY;
    	  risingedge 		= 255;
    	  zcfound			= 255;
    	  autostep		= 0;
	    }
	}
      break;

    case STATE_MOTOR_WAIT_SEARCH_COMM_DELAY:  // wait out commutation delay
      commcounter++;
      if (commcounter > commthreshold)
      {
    	  position++;
    	  phase++; // commutate
    	  if (phase > 5)
    	  {
    		  phase=0;
    	  }
    	  TIM1->CCER 	= ccermask[phase]; // commutate bridge from fwd table

    	  // sk DAC->DHR12R1 = phase*810;
    	  // sk DAC->SWTRIGR=0x00000003;

    	  demagcounter		=0;
    	  demagthreshold 	= (step * DEMAG_ALLOWANCE) >> 8;
    	  if(phase==0)
    	  {
    		  ifbave 	= (ifbsum << 2) / ifbcount;
    		  ifbcount	= 0;
    		  ifbsum	= 0;
    	  }
    	  startstate	= STATE_MOTOR_WAIT_DEMAGNET_TIME;  // go back to wait out demag
      }
      break;
  } // end of startstate state machine

  switch(phase)
  {
    case PHASE_0:		// ab
    case PHASE_3: 		// ba
      bemfchannel=2; 	// read phase c
      break;
    case PHASE_1: 		// ac
    case PHASE_4:  		// ca
      bemfchannel=1; 	// read phase b
      break;
    case PHASE_2:  		// bc
    case PHASE_5:  		// cb
      bemfchannel=0; 	// read phase a
      break;
//    case PHASE_3:  // ba
//      bemfchannel=8; // read phase c
//      break;
//    case PHASE_4:  // ca
//      bemfchannel=7; // read phase b
//      break;
//    case PHASE_5:  // cb
//      bemfchannel=6; // read phase a
//      break;
  } // end of phase switch statement

  ADC1->SQR3	= 0x00000000; 		// current sense adc channel
  ADC1->CR2		= 0x00000001;  		// start ADC conversion of bemf
  globalcounter++;
  while  ((ADC1->SR & b1) == 0) ; 	// wait for conversion to complete
  ifb		= ADC1->DR - ifboffset;
  ifbsum 	= ifbsum + ifb;
  ifbcount++;

  TIM1->SR	= 0;  // clear interrupt
}


void  motorstartinit(void)
{
  TIM1->CCER = alloff;
  // TIM1->BDTR= b15+b12+b11+16;  // 2 usec dead time and set MOE
  TIM1->BDTR		= b15 + b11 + 16;  // Break enable ausgeschaltet (Test only), 2 usec dead time and set MOE
  maxdc				= 1000;
  rpmcmd			= 500;
  rpmref			= 0;
  transitioncounter	= 0;
  errorint			= 0;
  position			= 520000;
  positionest		= position * 4096;
  speedest			= 0;
  rpm				= 0;
  phase 			= 0;
  holdcounter		= 0;
  startstate		= 0;
  commcounter		= 0;
  step				= 3670;
  autostep 			= 0;
  risingedge 		= 0;
  zcfound			= 0;
  alignmentcounter	= 0;
  rampspeed			= 1;
  runningdc 		= 500;
  commthreshold		= 0;
  risingdelay		= 127;
  fallingdelay		= 127;
  LED_SWITCH_OFF;
  ledstate			= 0;
  flashcount		= 0;
  overcurrent		= 0;
  ifb				= 0;
  overloaddclimit	= 1200;
  overloadcounter	= 0;
  overloadflag		= 0;
  ifbsum			= 0;
  ifbave			= 0;
  ifbcount			= 0;
} // end of motor start init function


#ifdef  DEBUG
/*******************************************************************************
 * Function Name  : assert_failed
 * Description    : Reports the name of the source file and the source line number
 *                  where the assert error has occurred.
 * Input          : - file: pointer to the source file name
 *                  - line: assert error line source number
 * Output         : None
 * Return         : None
 *******************************************************************************/
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
    {
    }
}
#endif


/******************* (C) COPYRIGHT 2007 STMicroelectronics *****END OF FILE****/
