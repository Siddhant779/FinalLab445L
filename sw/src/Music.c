// Music.c
// Uses Timer2A to iterate through an array of output values and send the outputs to the DAC

#include "Music.h"
#include "./inc/DAC.h"
#include "./inc/Timer2A.h"
#include "./inc/Timer3A.h"
#include "./inc/Timer4A.h"
#include "../inc/tm4c123gh6pm.h"
#include "../lib/SD/eDisk.h"
#include "../lib/SD/ff.h"
#include "Display.h"


const uint16_t *SongArray;
uint32_t SongIndex;
uint32_t SongLength;
uint16_t Volume;
uint32_t counterSong;

uint8_t Buf[BUFSIZE8];
uint8_t Buf2[BUFSIZE8];
uint32_t Count8;
uint8_t *front8; // buffer being output to DAC
uint8_t *back8;  // buffer being loaded from SDC
int flag8; // 1 means need data into back
uint32_t BufCount8; // 0 to NUMBUF8-1
uint8_t done_song;
uint8_t stop_dac;
uint8_t SongStrIndex;

uint16_t Bitmap[BITBUFSIZE16];

UINT successfulreads, successfulwrites;

void buffer_in(void);
void adc_init(void);
void send_to_dac(void);
void update_volume(void);
uint16_t ADC_In(void);

FIL Handle2;
FRESULT Fresult;


const Music Songs[3] = {
    {"weekIM.bin", "Lights.bin", "Blinding Lights", "The Weeknd", "After Hours", 2241504},
	{"creepIM.bin", "Creep.bin", "Creep", "Radiohead", "Pablo Honey", 2610720},
	{"takeFiIM.bin", "TakeFive.bin", "Take Five", "Dave Brubeck", "Time Out", 3603184}
	//{"denimIM.bin", "Denim.bin", "Japanese Denim", "Daniel Caesar", "Acoustic Break", 2986086},
	//{"hotelIM.bin", "Hotel.bin", "Hotel California", "Eagles", "Hotel California", 4302432},
	//{"startIM.bin", "Start.bin", "From the Start", "Laufey", "Bewitched", 1869546},
	//{"kesariyaIM.bin", "Kesariya.bin", "Kesariya", "Arijit Singh", "Brahmastra", 2956544},
};

void music_init(void) {
	// PB5 is initialized as an analog input in Unified_Port_Init, so there is no need to reinitialize it here
	SongStrIndex = 0;
	dac_init();
	adc_init();
	Timer2A_Init(&send_to_dac, 7256, 1);
	Timer2A_Stop();
	Timer4A_Init(&buffer_in, 371507, 7);
}

void buffer_in(void) {
	if(flag8 == 1) {
		flag8 = 0;
		// 1.5ms to 1.6ms to read 512 bytes
		Fresult = f_read(&Handle2, back8, BUFSIZE8,
			&successfulreads);
		if(Fresult){
			ILI9341_DrawString(52, 10, "read error ",0x03E0 , 2);
			while(1){};
		}
		BufCount8++;
		//if(BufCount8%COUNT) // Increasse length of progress bar
		if(BufCount8 == NUMBUF8){ // could have seeked
			Fresult = f_close(&Handle2);
			done_song = 1;
			Fresult = f_open(&Handle2, Songs[SongStrIndex].song_file, FA_READ);
			BufCount8 = 0;
		}
	}
}

void send_to_dac(void) {
	if(stop_dac == 0) {
		uint16_t data = front8[Count8]<<4; // shifted into MSbits
		dac_output((data*Volume)/2000); // 12 bit
	}
	Count8++;
	counterSong++;
 	if(Count8 == BUFSIZE8){
		if(done_song) {
			pause_song();
			done_song = 0;
			flag8 = 0;
			stop_dac = 1;
		}
		else {
			uint8_t *pt = front8;
			front8 = back8;
			back8 = pt; // swap buffers
			flag8 = 1;  // need more data
			stop_dac = 0;
		}
		Count8 = 0;
  }
}

void load_song(void) {
	//TO-DO: Open the song bin currently pointed to by SongStrIndex
	Fresult = f_open(&Handle2, Songs[SongStrIndex].song_file, FA_READ);
    if(Fresult){
        ILI9341_DrawString(52, 0, "testFile error",0x03E0 , 2);
    }
    if(Fresult == FR_OK) {
        //ILI9341_DrawString(52, 0, "opened music file ",0x03E0 , 2);
    }
	unpause_song();
}

void close_song(void) {
	Fresult = f_close(&Handle2);
}

void pause_song(void) {
	Timer2A_Stop();
}

void unpause_song(void) {
	Timer2A_Start();
}

void rewind_song(void) {
	Timer2A_Stop();
	Fresult = f_close(&Handle2);Fresult = f_open(&Handle2, Songs[SongStrIndex].song_file, FA_READ);
    if(Fresult){
        ILI9341_DrawString(52, 0, "testFile error",0x03E0 , 2);
    }
    if(Fresult == FR_OK) {
        //ILI9341_DrawString(52, 0, "opened music file ",0x03E0 , 2);
    }
	Timer2A_Start();
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



void LoadBitmap(char Filename[]) {
	Fresult = f_open(&Handle2, Filename, FA_READ);
	if(Fresult){
    ILI9341_DrawString(52, 0, "bitmap file error",0x03E0 , 2);
  }
	if(Fresult == FR_OK) {
		//ILI9341_DrawString(52, 0, "opened bitmap file ",0x03E0 , 2);
	}
	
	Fresult = f_read(&Handle2, Bitmap, BITBUFSIZE16*2, &successfulreads);
	if(Fresult){
		ILI9341_DrawString(52, 10, "read error ",0x03E0 , 2);
		while(1){};
	}
    Fresult = f_close(&Handle2);
}

void replacealbumCover(enum StateName menu, bool replace) {
	if(menu >= menu_mus && menu <= mus_ba) {
		if(replace) {
			ILI9341_fillRect(15, 30, 100, 100, ILI9341_WHITE);
		}
		ILI9341_DrawBitmap(15,145,Bitmap, 100, 100);
	}
	else if(menu >= np_pl && menu <= np_ba){
		if(replace) {
			ILI9341_fillRect(15, 30, 110, 110, ILI9341_WHITE);
		}
    	ILI9341_DrawBitmap(150,170,Bitmap, 100, 100);

		ILI9341_SetCursor(20, 6);
		ILI9341_OutStringSize("                  ",ILI9341_BLACK, 1);
		ILI9341_SetCursor(20, 6);
		ILI9341_OutStringSize(Songs[SongStrIndex].song_name,ILI9341_BLACK, 1);
			
		ILI9341_SetCursor(20, 7);
		ILI9341_OutStringSize("                  ",ILI9341_BLACK, 1);
		ILI9341_SetCursor(20, 7);
		ILI9341_OutStringSize(Songs[SongStrIndex].album_name,ILI9341_BLACK, 1);
			
		ILI9341_SetCursor(20, 8);
		ILI9341_OutStringSize("                  ",ILI9341_BLACK, 1);
		ILI9341_SetCursor(20, 8);
		ILI9341_OutStringSize(Songs[SongStrIndex].artist_name,ILI9341_BLACK, 1);
	}
    
}
