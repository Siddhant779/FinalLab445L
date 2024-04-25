// Display.h 

#ifndef _DISPLAYH_
#define _DISPLAYH_
#include <stdint.h>
#include "ILI9341.h"
#include "FSM.h"

// void drawSettingsPage(enum StateName menu);

void drawMusicPage(enum StateName menu);

void drawNowPlayingPage(enum StateName menu);

void drawMainMenu(enum StateName menu);

uint8_t get_top(void);

uint8_t inc_top(void);
uint8_t dec_top(void);
 
#endif
