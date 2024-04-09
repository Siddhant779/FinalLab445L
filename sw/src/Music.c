// Music.c
// Uses Timer2A to iterate through an array of output values and send the outputs to the DAC

#include "Music.h"
#include "./inc/DAC.h"
#include "./inc/Timer2A.h"
#include "./inc/Timer3A.h"
#include "../inc/tm4c123gh6pm.h"

//#define PB5		(*((volatile uint32_t *)0x40005080))

const uint16_t *SongArray;
uint32_t SongIndex;
uint32_t SongLength;
uint16_t Volume;
uint16_t DacData;


void adc_init(void);
void send_to_dac(void);
void update_volume(void);
uint16_t ADC_In(void);

void music_init(void) {
	// PB5 is initialized as an analog input in Unified_Port_Init, so there is no need to reinitialize it here
	dac_init();
	adc_init();
	Timer2A_Init(&send_to_dac, 7256, 5);
	Timer2A_Stop();
}

void send_to_dac(void) {
	if(SongIndex < SongLength) {
		DacData = (SongArray[SongIndex]*Volume)/2000;
		dac_output(DacData);
		SongIndex++;
	}
	else {
		Timer2A_Stop();
		SongIndex = 0;
		// TO-DO: Handle case for when the song ends
	}
}

void load_song(const uint16_t* song, uint32_t length) {
	SongIndex = 0;
	SongLength = length;
	SongArray = song;
}

void pause_song(void) {
	Timer2A_Stop();
}

void unpause_song(void) {
	Timer2A_Start();
}

void rewind_song(void) {
	Timer2A_Stop();
	SongIndex = 0;
}

bool is_playing(void) {
	return (TIMER2_CTL_R == 0x00000001);
}

void adc_init(void) {
	//TO-DO: Initialize ADC using PB5; AIN11
	Timer3A_Init(&update_volume, 800000, 6);
	SYSCTL_RCGCADC_R 			|= 	0x01; // Enable ADC0 Clock
	GPIO_PORTB_DIR_R      &= ~0x20;	// make PB5 input
  GPIO_PORTB_AFSEL_R    |=  0x20;	// enable alternate function on PB5
  GPIO_PORTB_DEN_R      &= ~0x20;	// disable digital I/O on PB5
  GPIO_PORTB_AMSEL_R    |=  0x20;	// enable analog functionality on PB5
	ADC0_PC_R = 0x01; // 125kHz conversion speed
	ADC0_SSPRI_R = 0x0123; // Set sequencer priority with sequencer 3 at highest
	ADC0_ACTSS_R &= ~0x0008; // Disable sequencer 3 while initializing
	ADC0_EMUX_R &= ~0xF000; // Trigger with software
	ADC0_SSMUX3_R |= 0x000B; // Input from channel 11
	ADC0_SSCTL3_R = 0x0006; // Enable IE0 and END0; Disable TS0 and D0
	ADC0_IM_R &= ~0x0008; // Disable interrupts for sequencer 3
	ADC0_ACTSS_R |= 0x0008; // Enable sequencer 3
}

void update_volume(void) {
	Volume = (ADC_In()/2);
}

uint16_t ADC_In(void){
	uint16_t data;
	ADC0_PSSI_R = 0x0008; // Start ADC SS3
	while((ADC0_RIS_R&0x08) == 0){}; // Busy-wait until flag is set
	data = ADC0_SSFIFO3_R&0xFFF; // Read data
	ADC0_ISC_R = 0x0008; // Clear flag to signal done with reading data
	return data;
}
