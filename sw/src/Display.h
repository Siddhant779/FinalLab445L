// Display.h 

#ifndef _DISPLAYH_
#define _DISPLAYH_
#include <stdint.h>
#include "ILI9341.h"
#include "FSM.h"

extern void drawSettingsPage(enum StateName menu);

extern void drawMusicPage(enum StateName menu);

extern void drawNowPlayingPage(enum StateName menu);

extern void drawMainMenu(enum StateName menu);



 
#endif
