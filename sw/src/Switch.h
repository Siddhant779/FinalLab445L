// Switch.h
// Siddhant Pandit 
// Wyatt Cole 
// This part of the module will be used for changing the mode between alarm and clock, setting the alarm time, setting current time, switching the stop the sound,
// enabling and disabling alarms 


#include <stdint.h>
#include <stdbool.h>
#include "./inc/tm4c123gh6pm.h"

// Initializes PF4, PF1, and PF0 as falling edge-triggered switches
void switch_init(void);

void switch_init2(void);
// Timer2A ISR for handling debouncing
void debounce(void);

// Changes mode (tbd) 
void change_mode(void);
