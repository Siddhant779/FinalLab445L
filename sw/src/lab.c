/**
 * @file        lab.c
 * @author      your name (your_email@doman.com), Jonathan Valvano, Matthew Yu
 *              <TA NAME and LAB SECTION # HERE>
 * @brief       An empty main file for running your lab.
 * @version     0.1.0
 * @date        2022-10-08 <REPLACE WITH DATE OF LAST REVISION>
 * @copyright   Copyright (c) 2022
 * @note        Potential Pinouts:
 *                  Backlight (pin 10) connected to +3.3 V
 *                  MISO (pin 9) unconnected
 *                  SCK (pin 8) connected to PA2 (SSI0Clk)
 *                  MOSI (pin 7) connected to PA5 (SSI0Tx)
 *                  TFT_CS (pin 6) connected to PA3 (SSI0Fss)
 *                  CARD_CS (pin 5) unconnected
 *                  Data/Command (pin 4) connected to PA6 (GPIO)
 *                  RESET (pin 3) connected to PA7 (GPIO)
 *                  VCC (pin 2) connected to +3.3 V
 *                  Gnd (pin 1) connected to ground
 *
 *                  Center of 10k-ohm potentiometer connected to PE2/AIN1
 *                  Bottom of 10k-ohm potentiometer connected to ground
 *                  Top of 10k-ohm potentiometer connected to +3.3V
 *
 *              Warning. Initial code for the RGB driver creates bright flashing
 *              lights. Remove this code and do not run if you have epilepsy.
 */
 
 
//files to include 
/*
main one 
esp base - wifi one - arduino
sockets file - send receive functions in this 
menu file - fsm 
controls file - buttons deals with that 
*/

/** File includes. */
#include <stdint.h>
#include <stdio.h>
#include <string.h>
/* Register definitions. */
#include "./inc/tm4c123gh6pm.h"
/* Clocking. */
#include "./inc/PLL.h"
/* Clock delay and interrupt control. */
#include "./inc/CortexM.h"
/* Initialization of all the pins. */
#include "./inc/Unified_Port_Init.h"
/* Talking to PC via UART. */
#include "./inc/UART.h"
/* ST7735 display. */
#include "./inc/ST7735.h"
#include "ILI9341.h"


/* Add whatever else you need here! */
#include "./lib/RGB/RGB.h"
#include "Switch.h"
#include "Music.h"
#include "../lib/SD/eDisk.h"
#include "../lib/SD/ff.h"
#include "Display.h"
// port b 6

/** MMAP Pin definitions. */
#define PF0   (*((volatile uint32_t *)0x40025004)) // Left Button
#define PF1   (*((volatile uint32_t *)0x40025008)) // RED LED
#define PF2   (*((volatile uint32_t *)0x40025010)) // BLUE LED
#define PF3   (*((volatile uint32_t *)0x40025020)) // GREEN LED
#define PF4   (*((volatile uint32_t *)0x40025040)) // Right Button

void Pause(void);
void DelayWait10ms(uint32_t n);

static FATFS g_sFatFs;
FIL Handle2;
FRESULT MountFresult;
FRESULT Fresult;



const char LightsFilename[] = "Lights.bin";   // 8 characters or fewer

const char TakeFilename[] = "TakeFive.bin";   // 8 characters or fewer

//we are going to have to map the album cover to teh sonsg - we would ahve to use 

int main(void) {
    /* Disable interrupts for initialization. */
    DisableInterrupts();

    /* Initialize clocking. */
    PLL_Init(Bus80MHz);

    /* Allow us to talk to the PC via PuTTy! Check device manager to see which
       COM serial port we are on. The baud rate is 115200 chars/s. */
    UART_Init();

    /* Initialize all ports. */
    Unified_Port_Init();
	  SSI2_Init(4);
	//ST7735_InitR(INITR_REDTAB);     // Start up display.
    ILI9341_init();
	//setRotation(3);

    /* Start RGB flashing. WARNING! BRIGHT FLASHING COLORS. DO NOT RUN IF YOU HAVE EPILEPSY. */
    RGBInit();
		
		/* Initialize buttons */
		switch_init();
	
		/* Initialize music driver */
		music_init();

    /* Allows any enabled timers to begin running. */
    EnableInterrupts();
    ILI9341_fillScreen(ILI9341_BLACK);
    //x is up to 320 and y is up to 240 - for drawing pixel
	
    // ILI9341_OutStringSize("Song 1:hello by TPOD gang\n""Song 2\n", 2);
	//   ILI9341_OutStringSize("Song 1\n""Song 2", 1);
	  uint8_t c, x, y;
		UINT successfulreads, successfulwrites;


// this is for settings page 
    // ILI9341_drawVLine(160,12,240, ILI9341_BLACK, 2);
    // ILI9341_DrawStringCord(318, 2, "Tpod",ILI9341_BLACK, 1);
    // ILI9341_drawHLine(0, 12, 320, ILI9341_BLACK, 2);
    
    // ILI9341_SetCursor(52,2);
    // ILI9341_OutStringSize("Colors",ILI9341_BLACK, 2);
    // ILI9341_drawHLine(160, 44, 160, ILI9341_BLACK, 2);

    // ILI9341_SetCursor(52,5);
    // ILI9341_OutStringSize("WiFi",ILI9341_BLACK, 2);
    // ILI9341_drawHLine(160, 74, 160, ILI9341_BLACK, 2);

    // ILI9341_SetCursor(52,23);
    // ILI9341_OutStringSize("Back",ILI9341_BLACK, 1);

		






// for the music page 
//     ILI9341_drawVLine(160,12,240, ILI9341_BLACK, 2);
//     ILI9341_DrawStringCord(318, 2, "Tpod",ILI9341_BLACK, 1);
//     ILI9341_drawHLine(0, 12, 320, ILI9341_BLACK, 2);
		

//     ILI9341_SetCursor(52,2);
//     ILI9341_OutStringSize("Blinding Lights",ILI9341_BLACK, 1);
//     ILI9341_SetCursor(52,3);
//     ILI9341_OutStringSize("The Weeknd",ILI9341_BLACK, 1);
//     ILI9341_drawHLine(160, 44, 160, ILI9341_BLACK, 2);

//     ILI9341_SetCursor(52,5);
//     ILI9341_OutStringSize("Creep",ILI9341_BLACK, 1);
//     ILI9341_SetCursor(52,6);
//     ILI9341_OutStringSize("Radiohead",ILI9341_BLACK, 1);
//     ILI9341_drawHLine(160, 74, 160, ILI9341_BLACK, 2);


//     ILI9341_SetCursor(52,8);
//     ILI9341_OutStringSize("Take Five",ILI9341_BLACK, 1);
//     ILI9341_SetCursor(52,9);
//     ILI9341_OutStringSize("Dave Brubeck",ILI9341_BLACK, 1);
//     ILI9341_drawHLine(160, 104, 160, ILI9341_BLACK, 2);

//     ILI9341_SetCursor(14,18);
//     ILI9341_OutStringSize("||",ILI9341_BLACK, 2);
		
// 		ILI9341_SetCursor(8,18);
//     ILI9341_OutStringSize(">|",ILI9341_BLACK, 2);
		
// 		ILI9341_SetCursor(20,18);
//     ILI9341_OutStringSize("|<",ILI9341_BLACK, 2);
		
// 		ILI9341_drawHLine(25, 220, 100, ILI9341_BLACK, 2);
// 		// using hte bytes of the song you can see how much time is left 
// 		// do it in dac_out - have a counter 25+((1-(bytes_done/bytes_total))*100) - for the x cord for the length its just (bytes_done/bytes_total)*100

//    // you can also do bytes_done/ bytes_total - decimal part (100 + 25) - how much finished is starting position 
// 		ILI9341_drawHLine(75, 217, 50, ILI9341_RED, 3);
		
// 		ILI9341_fillRect(15, 25, 120, 120, ILI9341_WHITE);    
// 		ILI9341_DrawBitmap(15,145,weeknd, 120, 120);

//     ILI9341_SetCursor(52,23);
//     ILI9341_OutStringSize("Back",ILI9341_BLACK, 1);





// this is for now playing page 

    // ILI9341_SetCursor(28,19);
    // ILI9341_OutStringSize("||",ILI9341_BLACK, 2);
		
    // ILI9341_SetCursor(22,19);
    // ILI9341_OutStringSize(">|",ILI9341_BLACK, 2);
		
	//   ILI9341_SetCursor(34,19);
    // ILI9341_OutStringSize("|<",ILI9341_BLACK, 2);

    // ILI9341_drawHLine(160, 217, 70, ILI9341_RED, 3);
    // ILI9341_drawHLine(90, 220, 140, ILI9341_BLACK, 2);


    // ILI9341_DrawStringCord(318, 2, "Now Playing",ILI9341_BLACK, 1);
    // ILI9341_drawHLine(0, 10, 320, ILI9341_BLACK, 2);

    // ILI9341_SetCursor(52,2);
    // ILI9341_OutStringSize("Back",ILI9341_BLACK, 2);

    // ILI9341_fillRect(150, 50, 120, 120, ILI9341_WHITE);    
    // ILI9341_DrawBitmap(150,170,weeknd, 120, 120);

    // ILI9341_SetCursor(20, 6);
    // ILI9341_OutStringSize("Blinding Lights",ILI9341_BLACK, 1);
		
	// ILI9341_SetCursor(20, 7);
    // ILI9341_OutStringSize("After Hours",ILI9341_BLACK, 1);
		
	// ILI9341_SetCursor(20, 8);
    // ILI9341_OutStringSize("The Weeknd",ILI9341_BLACK, 1);
		
		






        // this is for main menu 

//     ILI9341_drawVLine(160,12,240, ILI9341_BLACK, 2);

// ILI9341_DrawStringCord(318, 2, "Tpod",ILI9341_BLACK, 1);
//     ILI9341_drawHLine(0, 12, 320, ILI9341_BLACK, 2);
		

//     ILI9341_SetCursor(52,2);
//     ILI9341_OutStringSize("Messages",ILI9341_BLACK, 2);
//     ILI9341_drawHLine(160, 44, 160, ILI9341_BLACK, 2);

//     ILI9341_SetCursor(52,5);
//     ILI9341_OutStringSize("Music",ILI9341_BLACK, 2);
//     ILI9341_drawHLine(160, 74, 160, ILI9341_BLACK, 2);


//     ILI9341_SetCursor(52,8);
//     ILI9341_OutStringSize("Settings",ILI9341_BLACK, 2);
//     ILI9341_drawHLine(160, 104, 160, ILI9341_BLACK, 2);


//     ILI9341_SetCursor(52,11);
//     ILI9341_OutStringSize("Now Playing",ILI9341_BLACK, 2);
//     ILI9341_drawHLine(160, 134, 160, ILI9341_BLACK, 2);



//     ILI9341_SetCursor(14,18);
//     ILI9341_OutStringSize("||",ILI9341_BLACK, 2);
		
// 		ILI9341_SetCursor(8,18);
//     ILI9341_OutStringSize(">|",ILI9341_BLACK, 2);
		
// 		ILI9341_SetCursor(20,18);
//     ILI9341_OutStringSize("|<",ILI9341_BLACK, 2);
		
// 		ILI9341_drawHLine(25, 220, 100, ILI9341_BLACK, 2);
// 		// using hte bytes of the song you can see how much time is left 
// 		// do it in dac_out - have a counter 25+((1-(bytes_done/bytes_total))*100) - for the x cord for the length its just (bytes_done/bytes_total)*100

//    // you can also do bytes_done/ bytes_total - decimal part (100 + 25) - how much finished is starting position 
// 		ILI9341_drawHLine(75, 217, 50, ILI9341_RED, 3);
		
// 		ILI9341_fillRect(15, 25, 120, 120, ILI9341_WHITE);    
// 		ILI9341_DrawBitmap(15,145,weeknd, 120, 120);

    //drawMainMenu();
    //Delay1ms(10000);
    //drawSettingsPage();
    // Delay1ms(10000);
    //drawMusicPage();
    // Delay1ms(10000);
    drawNowPlayingPage();
    // Delay1ms(10000);




   front8 = Buf2; // buffer being output to DAC
  back8 = Buf;   // buffer being written to from SDC
  Count8 = 0;
  flag8 = 1; // 1 means need data into back
  BufCount8 = 0;
  done_song = 0;
  stop_dac = 0;
    //ILI9341_SetCursor(50,6);
    //ILI9341_OutStringSize("testing color",ILI9341_BLACK, 2);


    MountFresult = f_mount(&g_sFatFs, "", 0);
    if(MountFresult){
        //ILI9341_DrawString(52, 0, "f_mount error",0x03E0 , 2);
    }
    Fresult = f_open(&Handle2, LightsFilename, FA_READ);
    if(Fresult){
        //ILI9341_DrawString(52, 0, "testFile error",0x03E0 , 2);
    }
		if(Fresult == FR_OK) {
			//ILI9341_DrawString(52, 0, "opened music file ",0x03E0 , 2);
		}


    // x = 300;
    // y = 150;
    // uint8_t *read_str;
	// char *string_part;
    // if(Fresult == FR_OK){
    //     ILI9341_DrawString(52, 0, "opened testfile ",0x03E0 , 2);
	// 			Fresult = f_read(&Handle2, &c, 1, &successfulreads);
	// 				if((Fresult == FR_OK) && (successfulreads == 1)) {
	// 						ILI9341_DrawCharS(x, y, c, 0x03E0, ILI9341_BLACK, 1);
	// 						//string_part+="/0";
    //           //ILI9341_DrawString(52, 10, string_part,0x03E0, 2);
	// 				}
	// 				y-=15;
    // }



    // this is code for music part 
        while(1){
            if(flag8){ // 1 means need data
            flag8 = 0;
        // 1.5ms to 1.6ms to read 512 bytes 
            Fresult = f_read(&Handle2, back8, BUFSIZE8,
                &successfulreads);
            if(Fresult){
                //ILI9341_DrawString(52, 10, "read error ",0x03E0 , 2);
                while(1){};
            }
            BufCount8++;
            if(BufCount8 == NUMBUF8){ // could have seeked
                Fresult = f_close(&Handle2);
								done_song = 1;
                Fresult = f_open(&Handle2, LightsFilename, FA_READ);
                BufCount8 = 0;
								//memset(Buf2, 0, 512);
            }
            }
        // other tasks
  }



    /* Print starting message to the PC and the ST7735. */
    //ST7735_FillScreen(ST7735_BLACK);
    //ST7735_SetCursor(0, 0);
    //ST7735_OutString(
    //    "ECE445L Final lab.\n"
     //   "Press SW1 to start.\n");
    UART_OutString(
        "ECE445L Final lab.\r\n"
        "Press SW1 to start.\r\n");
    Pause();

    /* Stop RGB and turn off any on LEDs. */
    RGBStop();
    PF1 = 0;
    PF2 = 0;
    PF3 = 0;

    /* Reset screen. */
    //ST7735_FillScreen(ST7735_BLACK);
    //ST7735_SetCursor(0, 0);
    //ST7735_OutString("Starting...\n");
    UART_OutString("Starting...\r\n");
		
		//load_song(TakeFive, 79749);
		unpause_song();
    while (1) {
			/* TODO: Write your code here! */
			//DelayWait10ms(50);
			//printf("DacOut: %u\n", DacData);
    }
    return 1;
}

/** Function Implementations. */
void DelayWait10ms(uint32_t n) {
    uint32_t volatile time;
    while (n){
        time = 727240 * 2 / 91;  // 10msec
        while (time){
            --time;
        }
        --n;
    }
}

void Pause(void) {
    while (PF4 == 0x00) {
        DelayWait10ms(10);
    }
    while (PF4 == 0x10) {
        DelayWait10ms(10);
    }
}
