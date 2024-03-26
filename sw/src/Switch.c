// Switch.c

#include "./inc/tm4c123gh6pm.h"
#include "Switch.h"

#define PB0   (*((volatile uint32_t *)0x40005004))	// 
#define PB1  	(*((volatile uint32_t *)0x40005008))	// 
#define PB2  	(*((volatile uint32_t *)0x40005010))	// 


volatile static unsigned long LastPlay;
volatile static unsigned long LastRewind;
volatile static unsigned long LastMode;


static void SwitchArm(void) {
	GPIO_PORTB_ICR_R = 0x07;
	GPIO_PORTB_IM_R |= 0x07;
	NVIC_PRI0_R = (NVIC_PRI0_R&0xFFFF00FF) | 0x0000A000;	//Set priority to 5
	NVIC_EN0_R = 1<<1;						// Enable interrupt 1 (PortB) in NVIC
}

static void Timer1Arm(void) {
	TIMER1_CTL_R = 0x00000000;		// Disable timer during setup
	TIMER1_CFG_R = 0x00000000;		// 32-bit mode
	TIMER1_TAMR_R = 0x0000001;		// One-shot mode
	TIMER1_TAILR_R = 160000;			// Period of 10ms
	TIMER1_TAPR_R = 0;						// Uses bus clock
	TIMER1_ICR_R = 0x00000001;    // Clear TIMER1A timeout flag
  TIMER1_IMR_R = 0x00000001;    // Arm timeout interrupt
  NVIC_PRI5_R = (NVIC_PRI5_R&0xFFFF00FF)|0x00008000;	// Priority 4
	NVIC_EN0_R = 1<<21;						// Enable interrupt 21 in NVIC
	TIMER1_CTL_R = 0x00000001;		// Enable TIMER1A
}

void switch_init(void) {
	GPIO_PORTB_DIR_R &= ~0x07;		// Make PB0,1,2 inputs
	GPIO_PORTB_AFSEL_R &= ~0x07;	// Disable alt function for PB0,1,2
	GPIO_PORTB_DEN_R |= 0x07;			// Enable digital I/O on PB0,1,2
	GPIO_PORTB_AMSEL_R &= ~0x07;	// Disable analog function for PB0,1,2
	GPIO_PORTB_PUR_R |= 0x07;			// Enable internal weak pull-up on PB0,1,2
	GPIO_PORTB_IS_R &= ~0x07;			// PB0,1,2 is edge-sensitive
	GPIO_PORTB_IBE_R |= 0x07;			// PB0,1,2 is both edges
	
	SwitchArm();									// Arm PortB buttons
	SYSCTL_RCGCTIMER_R |= 0x02;		// Activate timer1
	LastPlay = PB0;
	LastRewind = PB1;
	LastMode = PB2;
}


void GPIOPortB_Handler(void) {
	GPIO_PORTB_IM_R &= ~0x07;			// Disarm button interrupt
	
	if(GPIO_PORTB_RIS_R&0x01)	{		// Poll PB0
		if(LastPlay) {
			// Service play/pause
			TIMER2_CTL_R = 0x00000001;
		}
	}
	if(GPIO_PORTB_RIS_R&0x02)	{		// Poll PB1
		if(LastRewind) {

		}
	}
	if(GPIO_PORTB_RIS_R&0x04)	{		// Poll PB2
		if(LastMode) {
			// Service mode change

		}
	}
	Timer1Arm();
}

void Timer1A_Handler(void) {
	TIMER1_IMR_R = 0x00000000;		// Disarm interrupt
	LastPlay = PB0;			// Save values
	LastRewind = PB1;
	LastMode = PB2;
	SwitchArm();				// Clear PortB flags and rearm interrupts
}

void change_mode(void) {
	
}