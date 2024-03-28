// Switch.c

#include "./inc/tm4c123gh6pm.h"
#include "Switch.h"
	
#define PC4		(*((volatile uint32_t *) 0x40006040))	// UP
#define PC5		(*((volatile uint32_t *) 0x40006080))	// RIGHT
#define PC6		(*((volatile uint32_t *) 0x40006100))	// LEFT
#define PC7		(*((volatile uint32_t *) 0x40006200))	// DOWN
#define PF4		(*((volatile uint32_t *) 0x40025040))	// MODE


volatile static unsigned long LastUp;
volatile static unsigned long LastRight;
volatile static unsigned long LastLeft;
volatile static unsigned long LastDown;
volatile static unsigned long LastMode;

// NEEDS TO BE CHANGED TO PORTC AND PORTF
// STILL NEEDS WORK
static void SwitchArm(void) {
	// Clear and reset interrupts
	GPIO_PORTC_ICR_R = 0xF0;
	GPIO_PORTC_IM_R |= 0xF0;
	GPIO_PORTF_ICR_R = 0x08;
	GPIO_PORTF_IM_R |= 0x08;
	NVIC_PRI0_R = (NVIC_PRI0_R&0xFF00FFFF) | 0x00A00000;	// Set priority to 5 on PortC
	NVIC_PRI7_R = (NVIC_PRI0_R&0xFF00FFFF) | 0x00A00000;	// Set priority to 5 on PortF
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
	GPIO_PORTC_DIR_R &= ~0xF0;		// Make PC4,5,6,7 inputs
	GPIO_PORTC_AFSEL_R &= ~0xF0;	// Disable alt function for PC4,5,6,7
	GPIO_PORTC_DEN_R |= 0xF0;			// Enable digital I/O on PC4,5,6,7
	GPIO_PORTC_AMSEL_R &= ~0xF0;	// Disable analog function for PC4,5,6,72
	GPIO_PORTC_PUR_R |= 0xF0;			// Enable internal weak pull-up on PC4,5,6,7
	GPIO_PORTC_IS_R &= ~0xF0;			// PC4,5,6,7 is edge-sensitive
	GPIO_PORTC_IBE_R |= 0xF0;			// PC4,5,6,7 is both edges
	
	// Same initializations for PF4
	GPIO_PORTF_DIR_R &= ~0x08;		
	GPIO_PORTF_AFSEL_R &= ~0x08;	
	GPIO_PORTF_DEN_R |= 0x08;			
	GPIO_PORTF_AMSEL_R &= ~0x08;	
	GPIO_PORTF_PUR_R |= 0x08;			
	GPIO_PORTF_IS_R &= ~0x08;			
	GPIO_PORTF_IBE_R |= 0x08;			
	
	SwitchArm();									// Arm PortC buttons
	SYSCTL_RCGCTIMER_R |= 0x02;		// Activate timer1
	// Set initial values of all buttons
	LastUp = PC4;
	LastRight = PC5;
	LastLeft = PC6;
	LastDown = PC7;
	LastMode = PF4;
}


void GPIOPortC_Handler(void) {
	GPIO_PORTC_IM_R &= ~0x07;			// Disarm button interrupt
	// Determine which button(s) were pressed
	if(GPIO_PORTC_RIS_R&0x10)	{		// Poll PC4
		if(LastUp) {
			// Service up button
		}
	}
	if(GPIO_PORTC_RIS_R&0x20)	{		// Poll PC5
		if(LastRight) {
			// Service right button
		}
	}
	if(GPIO_PORTC_RIS_R&0x40)	{		// Poll PC6
		if(LastLeft) {
			// Service left button

		}
	}
	if(GPIO_PORTC_RIS_R&0x80)	{		// Poll PC7
		if(LastDown) {
			// Service down button
		}
	}
	Timer1Arm();
}

void Timer1A_Handler(void) {
	// Debouncing is complete
	TIMER1_IMR_R = 0x00000000;		// Disarm interrupt
	// Save all switch values 
	LastUp = PC4;
	LastRight = PC5;
	LastLeft = PC6;
	LastDown = PC7;
	LastMode = PF4;
	SwitchArm();				// Clear PortB flags and rearm interrupts
}