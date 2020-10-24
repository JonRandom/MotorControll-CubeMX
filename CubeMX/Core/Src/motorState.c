
/* *****************************************************************
 *
 * File:  motorState.c
 * Autor: Jonathan Fulcher
 * Datum: 24.11.2020
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

state_t motorState;
error_t stateError;
int stepCounter;		// wird von allen states benutzt
int phase;
int rampupTime;
int phaseTimeCounter;	// zählt, wie viele ISR Zyklen diese Phase schon eingeschaltet ist
int actualPhaseTime;	// aktuelle Einschaltdauer einer Phase
int pwmFromUser;		// dieser Wert steuert den Motor, 0 = Motor aus


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
	motorState 			= STATE_MOTOR_OFF;
	stateError			= 0;
	stepCounter			= 0;
	phase 				= 0;
	phaseTimeCounter	= 0;
	actualPhaseTime		= 0;
	pwmFromUser			= 500;	// todo
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
	phase++;
	if (phase > 5)
	{
		phase = 0;
	}

    TIM1->CCER = motorTimerCCER_Table[phase];
}


state_t  processMotorOff()
{
	state_t myNextState = 0;

	stepCounter 		= 0;
	phase 				= 0;
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
			myNextState = STATE_MOTOR_ZEROCROSS;
		}
	}

	return myNextState;
}


state_t processMotorZeroCross()
{
	state_t myNextState = 0;

	if (++phaseTimeCounter > rampupTime)
	{
		actualPhaseTime 	= phaseTimeCounter;
		phaseTimeCounter 	= 0;
		switchToNextPhase ();
	}

	// todo: activate Zerocrossing

//	setPWM (pwmFromUser);
//
//	if (zeroCrossDetected (phase))
//	{
//		switchToNextPhase ();
//		// actualSpeed = speedCounter;
//	}
//
//	if (pwmFromUser == 0)
//	{
//		myNextState = STATE_MOTOR_OFF;
//	}

	return myNextState;
}


void processMotorState()
{
	state_t nextState = 0;

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
}
