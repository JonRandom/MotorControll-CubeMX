
/*
MOTOR CONTROL MODULE
*/


/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "motorcontrolsettings.h"

#define b0 1
#define b1 2
#define b2 4
#define b3 8
#define b4 16
#define b5 32
#define b6 64
#define b7 128
#define b8 256
#define b9 512
#define b10 1024
#define b11 2048
#define b12 4096
#define b13 8192
#define b14 16384
#define b15 32768


/*
#define phab b0+b2+b3+b11
#define phac b0+b2+b3+b7
#define phbc b3+b4+b6+b7
#define phba b4+b6+b7+b11
#define phca b7+b8+b10+b11
#define phcb b8+b10+b11+b3
*/

// add b12 to enable ch4 and b13 to invert ch4
#define phab b0+b2+b3+b11+b12+b13
#define phac b0+b2+b3+b7+b12+b13
#define phbc b3+b4+b6+b7+b12+b13
#define phba b4+b6+b7+b11+b12+b13
#define phca b7+b8+b10+b11+b12+b13
#define phcb b8+b10+b11+b3+b12+b13




#define dacout TIM1->CCR4

// lookup tables --------------------------------------------------


// converts raw Hall pattern to phase number for fwd direction
const unsigned char fwdphases[8]=     
{
0,3,5,4,1,2,0,0
};

// converts raw Hall pattern to phase number for rev direction
const unsigned char revphases[8]=     
{
0,5,1,0,3,4,2,0
};


// converts phase to mask for tim1 CCER 
//const unsigned short ccermask[6]=     
//{
//phab,phac,phbc,phba,phca,phcb
//};


// globals
unsigned char phase;
unsigned char reverse;
unsigned long speed;
unsigned short motorcurrent;
unsigned short maxdutycycle;
unsigned short dutycyclecmd;
unsigned short dutycycle;
unsigned short spdregtimer;
unsigned short spdcmd;
signed long spdregintegral;
unsigned long averagespeed;
unsigned long speedsum;
unsigned short speedcount;



void commutatebridge(void)
{
unsigned short word0;
unsigned char byte0;  // next expected phase

byte0=phase; // retain last phase

if(reverse) // start of reverse code
{
// read Halls and mask, then look up phase from table
word0 = ((GPIOB->IDR)>>6) & 0x0007;
phase = revphases[word0];
// predict expected next phase
if(byte0==0) byte0=5; 
else byte0--;
// re-read Halls if phase is not what was expected
if(phase!=byte0) 
{
word0 = ((GPIOB->IDR)>>6) & 0x0007;
phase = revphases[word0];
}
TIM1->CCER = ccermask[phase]; // commutate bridge from rev table 
} // end of reverse code
else // start of forward code
{
// read Halls and mask, then look up phase from table
word0 = ((GPIOB->IDR)>>6) & 0x0007;
phase = fwdphases[word0];
// predict expected next phase
if(byte0==5) byte0=0; 
else byte0++;
// re-read Halls if phase is not what was expected
//if(phase!=byte0) 

if(phase!=byte0) 
{
word0 = ((GPIOB->IDR)>>6) & 0x0007;
phase = fwdphases[word0];
}

if(phase!=byte0) 
{
word0 = ((GPIOB->IDR)>>6) & 0x0007;
phase = fwdphases[word0];
}

if(phase!=byte0) 
{
word0 = ((GPIOB->IDR)>>6) & 0x0007;
phase = fwdphases[word0];
}

if(phase!=byte0) 
{
word0 = ((GPIOB->IDR)>>6) & 0x0007;
phase = fwdphases[word0];
}


if(phase!=byte0) 
{
word0 = ((GPIOB->IDR)>>6) & 0x0007;
phase = fwdphases[word0];
}

if(phase!=byte0) 
{
word0 = ((GPIOB->IDR)>>6) & 0x0007;
phase = fwdphases[word0];
}

if(phase!=byte0) 
{
word0 = ((GPIOB->IDR)>>6) & 0x0007;
phase = fwdphases[word0];
}

if(phase!=byte0) 
{
word0 = ((GPIOB->IDR)>>6) & 0x0007;
phase = fwdphases[word0];
}


if(phase!=byte0) 
{
word0 = ((GPIOB->IDR)>>6) & 0x0007;
phase = fwdphases[word0];
}

if(phase!=byte0) 
{
word0 = ((GPIOB->IDR)>>6) & 0x0007;
phase = fwdphases[word0];
}

if(phase!=byte0) 
{
word0 = ((GPIOB->IDR)>>6) & 0x0007;
phase = fwdphases[word0];
}

if(phase!=byte0) 
{
word0 = ((GPIOB->IDR)>>6) & 0x0007;
phase = fwdphases[word0];
}


TIM1->CCER = ccermask[phase]; // commutate bridge from fwd table
} // end of forward code

dacout=phase*600;
  
} // end of commutate bridge function




void tim4inthandler(void)
{
volatile unsigned short word0;
volatile unsigned short word1;

word0=TIM4->CCR1; // clear the interrupt flag

//word1=3200;
//while(word1>0) word1--;

//////////////commutatebridge();

//speed=8333333;  // to give RPM X 10
speed=8066666;  // to give RPM X 10
speed= speed / word0;

speedsum=speedsum+speed;
speedcount++;


if(TIM4->SR & 0x0001) // if tim4 overflowed
{
speed=0;
TIM4->SR &= 0xFFFE;
}

if(speed<50) speed=50;
if(speed>3000) speed=3000;
} // end of tim4 interrupt handler


void limitcurrent(void)
{
motorcurrent= ADC1->DR;
ADC1->CR2=0x00000001;  // start ADC conversion

if(motorcurrent>maxcurrent) maxdutycycle=maxdutycycle-10;
else maxdutycycle=maxdutycycle+1;
if(maxdutycycle>3100) maxdutycycle=3100;
if(maxdutycycle<100) maxdutycycle=100;

maxdutycycle=1600; // bm1 50% limit

if(dutycyclecmd>maxdutycycle) dutycycle=maxdutycycle;
else dutycycle=dutycyclecmd;



TIM1->CCR1= dutycycle;
TIM1->CCR2= dutycycle;
TIM1->CCR3= dutycycle;
}




void regulatespeed(void)
{
signed long slong0;
signed long slong1;
#define intlimit 48000

spdregtimer=10;
if(TIM4->SR & 0x0001) speed=0; // tim4 overflowed

if(speedcount!=0)
{
averagespeed=speedsum/speedcount;
speedsum=0;
speedcount=0;
}
else averagespeed=speed;



slong0= spdcmd-averagespeed; // speed error calculation


spdregintegral= spdregintegral + slong0;
if(spdregintegral> intlimit ) spdregintegral= intlimit ;
if(spdregintegral< -intlimit ) spdregintegral= -intlimit ;

slong1= (slong0) + (spdregintegral>>4) ;

if(slong1>2400) slong1=2400;
if(slong1<0) slong1=0;

dutycyclecmd=slong1;

dutycyclecmd=1600;




} // end of speed regulator function




void motorcontrolinit(void)
{
reverse=initialreversesetting;
speed=0;
motorcurrent=0;
maxdutycycle=3100;
maxdutycycle=3100;
spdregtimer=10;
spdcmd=initialspdcmd; 
spdregintegral=0;
averagespeed=0;
speedsum=0;
speedcount=0;
}


#define alloff b3+b7+b11


//****************************************************************************************************
void bkg_commutatebridge(void)
{
unsigned short word0;

// read Halls and mask, then look up phase from table
//word0 = ((GPIOA->IDR)) & 0x0007;
//phase = fwdphases[word0];
TIM1->CCER = ccermask[phase]; // commutate bridge from fwd table


} // end of commutate bridge function
//**************************************************************************************************

