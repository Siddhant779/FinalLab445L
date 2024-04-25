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
#include "ILI9341.h"


/* Add whatever else you need here! */
#include "Switch.h"
#include "Music.h"
#include "../lib/SD/eDisk.h"
#include "../lib/SD/ff.h"
#include "Display.h"
#include "Messages.h"
#include "FSM.h"
#include "./inc/Timer5A.h"
#include "./inc/UART5.h"
#include "./inc/esp8266_base.h"
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
FRESULT MountFresult;


//we are going to have to map the album cover to teh sonsg - we would ahve to use 


int main(void) {
    /* Disable interrupts for initialization. */
    DisableInterrupts();

    /* Initialize clocking. */
    PLL_Init(Bus80MHz);

    /* Allow us to talk to the PC via PuTTy! Check device manager to see which
       COM serial port we are on. The baud rate is 115200 chars/s. */
    // UART_Init();
    // UART5_Init();
    // Reset_8266();
    // SetupWiFi();

    /* Initialize all ports. */
    Unified_Port_Init();
	  //SSI2_Init(4);
	//ST7735_InitR(INITR_REDTAB);     // Start up display.
    ILI9341_init();
	//setRotation(3);

    /* Start RGB flashing. WARNING! BRIGHT FLASHING COLORS. DO NOT RUN IF YOU HAVE EPILEPSY. */
    //RGBInit();
		
    /* Initialize buttons */
    switch_init();

    /* Initialize music driver */
    //SongStrIndex = 0;
    music_init();
    message_init();
		
    FSM_Init();

    /* Allows any enabled timers to begin running. */
    EnableInterrupts();
    ILI9341_fillScreen(ILI9341_WHITE);
	

    front8 = Buf2; // buffer being output to DAC
    back8 = Buf;   // buffer being written to from SDC
    Count8 = 0;
    flag8 = 0; // 1 means need data into back
    BufCount8 = 0;
    done_song = 0;
    stop_dac = 0;



    MountFresult = f_mount(&g_sFatFs, "", 0);
    if(MountFresult){
        //ILI9341_DrawString(52, 0, "f_mount error",0x03E0 , 2);
    }
    

    LoadBitmap(Songs[SongStrIndex].album_file);
    replacealbumCover(Get_State().name, false);
    load_song();
    pause_song();

    // Main while loop of system
    while(1){
        //could check if need to draw flag here. If so, Fill screen white
        //set needtoDraw in FSMController when state transitions to a new menu, like all the back button tran
        State_t current_state = Get_State();
        if (get_clear_flag()){
            ILI9341_fillScreen(ILI9341_WHITE);
            replacealbumCover(current_state.name, true);
            set_clear_flag(0);
        }
        switch(current_state.name){
            case menu_mus:
            case menu_msg:
            case menu_play:
            case menu_pl:
            case menu_fa:
            case menu_re:
                drawMainMenu(current_state.name);
                break;
            case song1:
            case song2:
            case song3:
            case song4:
            case song5:
            case song6:
            case song7:
            case mus_pl:
            case mus_fa:
            case mus_re:
            case mus_ba:
                drawMusicPage(current_state.name);
                break;
            case np_pl:
            case np_fa:
            case np_re:
            case np_ba:
                drawNowPlayingPage(current_state.name);
                break;
            case msg_bck:
            case msg_key:
                display_keys();
                break;
        }
        // other tasks
    }
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
