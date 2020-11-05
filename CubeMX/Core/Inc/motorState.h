


#define STATE_MOTOR_OFF						0
#define STATE_MOTOR_START					1
#define STATE_MOTOR_RAMP_UP					2
#define STATE_MOTOR_ZEROCROSS				3

#define STATE_DOES_NOT_EXIST				1

#define STARTUP_PHASE_TIME					200		// zur Zeit 5ms
#define START_STEP_ANZ						100		// Anzahl ISR Zyklen, die im state STATE_MOTOR_START verbracht werden
#define RAMPUP_PHASE_TIME_ACC				999		// neueZeit =alteZeit * RAMPUP_PHASE_TIME_ACC / 1000
#define RAMPUP_PHASE_TIME_END				17		// ab dieser Zeit (Anzahl in ISR-Zyklen) wird auf Zerocross Betrieb umgeschaltet

#define PWM_INIT							100
#define PWM_RAMPUP							300

typedef int state_t;
typedef int error_t;


void initMotorState();
void processMotorState();

// Bit Definitionen:
#define b0              (1 << 0)
#define b1              (1 << 1)
#define b2              (1 << 2)
#define b3              (1 << 3)
#define b4              (1 << 4)
#define b5              (1 << 5)
#define b6              (1 << 6)
#define b7              (1 << 7)
#define b8              (1 << 8)
#define b9              (1 << 9)
#define b10             (1 << 10)
#define b11             (1 << 11)
#define b12             (1 << 12)
#define b13             (1 << 13)
#define b14             (1 << 14)
#define b15             (1 << 15)
#define b16             (1 << 16)
#define b17             (1 << 17)
#define b18             (1 << 18)
#define b19             (1 << 19)
#define b20             (1 << 20)
#define b21             (1 << 21)
#define b22             (1 << 22)
#define b23             (1 << 23)
#define b24             (1 << 24)
#define b25             (1 << 25)
#define b26             (1 << 26)
#define b27             (1 << 27)
#define b28             (1 << 28)
#define b29             (1 << 29)
#define b30             (1 << 30)
#define b31             (1 << 31)


#define	PHASE_0		0
#define	PHASE_1		1
#define	PHASE_2		2
#define	PHASE_3		3
#define	PHASE_4		4
#define	PHASE_5		5

#define PHASEN_ANZ	6


//#define MARK_SET 		GPIOB->ODR |= b6
//#define MARK_TOGGLE 	GPIOB->ODR ^= b6
//#define MARK_CLEAR 		GPIOB->ODR &= ~b6
//
//#define LED_SWITCH_OFF 	GPIOA->ODR |= b11
//#define LED_TOGGLE 		GPIOA->ODR ^= b11
//#define LED_SWITCH_ON 	GPIOA->ODR &= ~b11


// Advanced-control timers (TIM1 and TIM8) RM0008 Reference Manual S.354
// add b12 to enable ch4 and b13 to invert ch4
//#define phab b0 + b2+b3 +                       b11    + b12+b13
//#define phac b0 + b2+b3 +         b7                   + b12+b13
//#define phbc         b3 + b4 + b6+b7                   + b12+b13
//#define phba              b4 + b6+b7 +          b11    + b12+b13
//#define phca                      b7 + b8 + b10+b11    + b12+b13
//#define phcb         b3 +              b8 + b10+b11    + b12+b13
//#define alloff       b3 +         b7 +          b11    + b12+b13

//Keine PWM mehr auf der Lowside
//#define phab b0 +    b3 +                       b11    + b12+b13
//#define phac b0 +    b3 +         b7                   + b12+b13
//#define phbc         b3 + b4 +    b7                   + b12+b13
//#define phba              b4 +    b7 +          b11    + b12+b13
//#define phca                      b7 + b8 +     b11    + b12+b13
//#define phcb         b3 +              b8 +     b11    + b12+b13
//#define alloff       b3 +         b7 +          b11    + b12+b13

// Phasen Definitionen:
#define phab b0 +                  b7                     + b12+b13
#define phac b0 +                                  b11    + b12+b13
#define phbc                 b4 +                  b11    + b12+b13
#define phba         b3  +   b4                           + b12+b13
#define phca         b3  +                b8              + b12+b13
#define phcb                       b7 +   b8              + b12+b13

// ??? #define alloff       b3 +          b7 +            b11    + b12+b13
#define alloff      0
