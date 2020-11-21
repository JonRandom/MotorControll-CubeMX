
/* *****************************************************************
 *
 * File:  motorState.c
 * Autor: Jonathan Fulcher
 * Datum: 24.10.2020
 *
 * implementiert die StateMachine für den Motorbetrieb
 * Die StateMachine läuft im PWM-ISR
 *
 * Benutzung:
 * initMotorState() muss zur Initialisierung vor der Freigabe des PWM_ISR aufgerufen werden.
 * processMotorState() wird zyklisch aus dem PWM_ISR gestartet
 *
 * todo:
 * TIM1 in initMotorState() initialisieren, komplette Timer-Initialisierung hierher
 * ADC Messungen initialisieren
 * Zerocrossing State implementieren
 *
 ***************************************************************** */

#include "motorState.h"
#include "stm32f10x_lib.h"
#include "trigger.h"
#include <limits.h>

state_t motorState;
error_t stateError;		// wird gesetzt, wenn ein ungültiger MotorState auftritt
int stepCounter;		// wird von allen states benutzt
int motorPhase;
int rampupTime;
int phaseTimeCounter;	// zählt, wie viele ISR Zyklen diese Phase schon eingeschaltet ist
int actualPhaseTime;	// aktuelle Einschaltdauer einer Phase
int pwmFromUser;		// dieser Wert steuert den Motor, 0 = Motor aus

int zerocrossIdx;		// zählt die Aufrufe von processMotorZeroCross hoch, wird bei Phasenwechsel NULL gesetzt
int lastBemfValue	= 0;

#define BEMF_ARRAY_SIZE	16

int bemfArray[PHASEN_ANZ][BEMF_ARRAY_SIZE];

int bemfTempArray[BEMF_ARRAY_SIZE];

const unsigned int motorTimerCCER_Table[6] =
{
	phab,
	phac,
	phbc,
	phba,
	phca,
	phcb
};

const unsigned int motorTimerCCER_PinsOff = alloff;



void initMotorState()
{
	motorState 				= STATE_MOTOR_OFF;
	stateError				= 0;
	stepCounter				= 0;
	motorPhase 				= 0;
	phaseTimeCounter	= 0;
	actualPhaseTime		= 0;
	pwmFromUser				= PWM_USER_DEFAULT;	// todo

	triggerInit();
	triggerAdcInit();
}


void setPWM (int pwm)
{
    TIM1->CCR1	= pwm;
    TIM1->CCR2	= pwm;
    TIM1->CCR3	= pwm;
}


void switchOffPhases()
{
    TIM1->CCER = motorTimerCCER_PinsOff;
}


void switchToNextPhase ()
{
	motorPhase++;
	if (motorPhase > 5)
	{
		motorPhase = 0;
		triggerOn();
	}
	else if (motorPhase == 1)
	{
		triggerOff();
	}

  TIM1->CCER = motorTimerCCER_Table[motorPhase];

  changeAdcChannel (motorPhase);	// ADC zur nächsten Phase weiterschalten
}


state_t  processMotorOff()
{
	state_t myNextState = 0;

	stepCounter 		= 0;
	motorPhase 			= 0;
	actualPhaseTime		= 0;
	phaseTimeCounter	= 0;

	switchOffPhases ();						// Treiber ausschalten
	setPWM (0);								// PWM ausschalten

	if (pwmFromUser > 0)
	{
		myNextState = STATE_MOTOR_START;
		switchToNextPhase ();				// auf Phase 1 schalten, Treiber einschalten
	}

	return myNextState;
}


state_t processMotorStart()
{
	state_t myNextState = 0;
	setPWM (PWM_INIT);
	rampupTime = STARTUP_PHASE_TIME;

	if (++phaseTimeCounter > STARTUP_PHASE_TIME)
	{
		actualPhaseTime 	= phaseTimeCounter;
		phaseTimeCounter 	= 0;
		switchToNextPhase();
	}

	if (++stepCounter > START_STEP_ANZ)
	{
		myNextState = STATE_MOTOR_RAMP_UP;
	}

	return myNextState;
}


state_t processMotorRampUp()
{
	state_t myNextState = 0;
	setPWM (PWM_RAMPUP);

	if (++phaseTimeCounter > rampupTime)
	{
		rampupTime 			= rampupTime * RAMPUP_PHASE_TIME_ACC / 1000;
		actualPhaseTime 	= phaseTimeCounter;
		phaseTimeCounter 	= 0;
		switchToNextPhase ();
		if (rampupTime < RAMPUP_PHASE_TIME_END)
		{
			myNextState 	= STATE_MOTOR_ZEROCROSS;
			zerocrossIdx	= 0;
		}
	}

	return myNextState;
}


int zeroCrossDetected (int actual, int last)
{
  int zeroCrossFound = 0;

  if (actual  > 0)
  {
    if (last < 0)
    {
      zeroCrossFound = 1;
    }
  }
  else
  {
    if (last > 0)
    {
      zeroCrossFound = 1;
    }
  }
  return zeroCrossFound;
}

void changeAdcChannel (int phase)
{
  int adcChannel;

	switch(phase)
	{
		case PHASE_0:		// ab
		case PHASE_3: 		// ba
			// ADC1->SQR3 = 2; 	// read phase c
		  adcChannel = 2;
			break;
		case PHASE_1: 		// ac
		case PHASE_4:  		// ca
			// ADC1->SQR3 = 1; 	// read phase b
		  adcChannel = 1;
			break;
		case PHASE_2:  		// bc
		case PHASE_5:  		// cb
			// ADC1->SQR3 = 0; 	// read phase a
		  adcChannel = 0;
			break;
		default:
			break;
	}
  ADC_RegularChannelConfig (ADC1, adcChannel, 1, ADC_SampleTime_1Cycles5);
}


int getEmfADCvalue (int phase)
{
  #define ADC_VAL_ANZ 6
  int i;

  triggerAdcOn();
  // ADC1->SR = 0;
  // ADC1->CR2		= 0x00000003;  		// start ADC conversion of back emf (bemf)
  // while ((ADC1->SR & b1) == 0) ; 	// wait for conversion to complete

  // adcValue	= ADC1->DR;
  int sum = 0;
//  int maxValue = INT_MIN;
//  int minValue = INT_MAX;
//  int adcValue;
//
//  for (i=0; i < ADC_VAL_ANZ; i++)
//  {
//  	while (ADC_GetFlagStatus (ADC1, ADC_FLAG_EOC) != SET);
//  	adcValue = ADC_GetConversionValue(ADC1);
//  	sum += adcValue;
//  	if (adcValue > maxValue) maxValue = adcValue;
//  	if (adcValue < minValue) minValue = adcValue;
//  }
//	triggerAdcOff();
//
//  sum -= minValue;
//  sum -= maxValue;

	while (ADC_GetFlagStatus (ADC1, ADC_FLAG_EOC) != SET);
	sum = ADC_GetConversionValue(ADC1);
	triggerAdcOff();

  return sum;
}


void copyBemfArray(int size, int phase)
{
	int i;
	for (i = 0; i < BEMF_ARRAY_SIZE; i++)
	{
		if (i < size)
		{
			bemfArray[phase][i] = bemfTempArray[i];
		}
		else
		{
			bemfArray[phase][i] = -9999;	// mark entry as unused
		}
	}
}


state_t processMotorZeroCross()
{
	int bemfValue;
	state_t myNextState = 0;

	bemfValue 	= getEmfADCvalue(motorPhase);	// get adc value
	// bemfValue 	= zerocrossIdx * 100 + (motorPhase+1) * 100000;	// test values for array debug

	setPWM (pwmFromUser);

	if (zerocrossIdx < BEMF_ARRAY_SIZE)			// store analog data in array for debug purposes
	{
	  bemfTempArray[zerocrossIdx] = bemfValue;
	  zerocrossIdx++;
	  bemfTempArray[0] = zerocrossIdx;
	}

	if (++phaseTimeCounter > rampupTime)
	{
		actualPhaseTime 	= phaseTimeCounter;
		phaseTimeCounter 	= 0;

//	if (zeroCrossDetected (bemfValue, lastBemfValue))
//	{
		copyBemfArray (zerocrossIdx, motorPhase);
		switchToNextPhase ();
		zerocrossIdx 		= 1;
		bemfTempArray[1] 	= zerocrossIdx;
		// actualSpeed 		= speedCounter;
	}

	if (pwmFromUser == 0)
	{
		myNextState = STATE_MOTOR_OFF;
	}

	lastBemfValue = bemfValue;

	return myNextState;
}


void processMotorState()
{
	state_t nextState = 0;

	// triggerAdcOn();

	switch (motorState)
	{
		case STATE_MOTOR_OFF:
			nextState = processMotorOff();
			break;
		case STATE_MOTOR_START:
			nextState = processMotorStart();
			break;
		case STATE_MOTOR_RAMP_UP:
			nextState = processMotorRampUp();
			break;
		case STATE_MOTOR_ZEROCROSS:
			nextState = processMotorZeroCross();
			break;
		default:
			stateError = STATE_DOES_NOT_EXIST;
	}

	if (nextState != 0)
	{
		motorState = nextState;
	}

	// triggerAdcOff();

}
