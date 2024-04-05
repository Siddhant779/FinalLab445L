// Music.c

#include "Music.h"
#include "./inc/DAC.h"
#include "./inc/Timer2A.h"
#include "../inc/tm4c123gh6pm.h"

const uint16_t *SongArray;
uint32_t SongIndex;
uint32_t SongLength;

void send_to_dac(void);

void music_init(void) {
	dac_init();
	Timer2A_Init(&send_to_dac, 7256, 6);
	Timer2A_Stop();
}

void send_to_dac(void) {
	dac_output(SongArray[SongIndex]);
	if(SongIndex < SongLength) {
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