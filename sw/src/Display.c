// Display.c 


#include "./inc/tm4c123gh6pm.h"
#include "Display.h"
#include "inc/ST7735.h"
#include "Music.h"
#include "FSM.h"



void drawMainMenu(enum StateName menu){
    ILI9341_fillScreen(ILI9341_WHITE);
    ILI9341_drawVLine(160,12,240, ILI9341_BLACK, 2);

    ILI9341_DrawStringCord(318, 2, "Tpod",ILI9341_BLACK, 1);
    ILI9341_drawHLine(0, 12, 320, ILI9341_BLACK, 2);
	uint32_t color = ILI9341_BLACK;	

    ILI9341_SetCursor(52,2);
    color = (menu == menu_mus) ? ILI9341_BLUE : ILI9341_BLACK;
    ILI9341_OutStringSize("Music",color, 2);
    ILI9341_drawHLine(160, 44, 160, ILI9341_BLACK, 2);

    ILI9341_SetCursor(52,5);
    color = (menu == menu_msg) ? ILI9341_BLUE : ILI9341_BLACK;
    ILI9341_OutStringSize("Messages",color, 2);
    ILI9341_drawHLine(160, 74, 160, ILI9341_BLACK, 2);


    ILI9341_SetCursor(52,8);
    color = (menu == menu_set) ? ILI9341_BLUE : ILI9341_BLACK;
    ILI9341_OutStringSize("Settings",color, 2);
    ILI9341_drawHLine(160, 104, 160, ILI9341_BLACK, 2);


    ILI9341_SetCursor(52,11);
    color = (menu == menu_play) ? ILI9341_BLUE : ILI9341_BLACK;
    ILI9341_OutStringSize("Now Playing",color, 2);
    ILI9341_drawHLine(160, 134, 160, ILI9341_BLACK, 2);



    ILI9341_SetCursor(14,18);
    color = (menu == menu_pl) ? ILI9341_BLUE : ILI9341_BLACK;
    ILI9341_OutStringSize("||",color, 2);
		
	ILI9341_SetCursor(8,18);
    color = (menu == menu_fa) ? ILI9341_BLUE : ILI9341_BLACK;
    ILI9341_OutStringSize(">|",color, 2);
		
	ILI9341_SetCursor(20,18);
    color = (menu == menu_re) ? ILI9341_BLUE : ILI9341_BLACK;
    ILI9341_OutStringSize("|<",color, 2);
		
	ILI9341_drawHLine(25, 220, 100, ILI9341_BLACK, 2);
		// using hte bytes of the song you can see how much time is left 
		// do it in dac_out - have a counter 25+((1-(bytes_done/bytes_total))*100) - for the x cord for the length its just (bytes_done/bytes_total)*100

   // you can also do bytes_done/ bytes_total - decimal part (100 + 25) - how much finished is starting position 
	ILI9341_drawHLine(75, 217, 50, ILI9341_RED, 3);
		
	ILI9341_fillRect(15, 25, 120, 120, ILI9341_WHITE);

    // for drawing bitmap need to change this part 
    // if(SongStrIndex == 0) {
    //     ILI9341_DrawBitmap(15,145,weeknd, 120, 120);
    // }    
    // else if(SongStrIndex == 1) {
    //     ILI9341_DrawBitmap(15,145,creep, 120, 120);
    // }   
    // else if(SongStrIndex == 2) {
    //     ILI9341_DrawBitmap(15,145,takeFive, 120, 120);
    // }   


}

void drawNowPlayingPage(enum StateName menu) {
    uint32_t color = ILI9341_BLACK;	
    ILI9341_fillScreen(ILI9341_WHITE);
    ILI9341_SetCursor(28,19);
    color = (menu == np_pl) ? ILI9341_BLUE : ILI9341_BLACK;
    ILI9341_OutStringSize("||",color, 2);
		
    ILI9341_SetCursor(22,19);
    color = (menu == np_fa) ? ILI9341_BLUE : ILI9341_BLACK;
    ILI9341_OutStringSize(">|",color, 2);
		
	  ILI9341_SetCursor(34,19);
      color = (menu == np_re) ? ILI9341_BLUE : ILI9341_BLACK;
    ILI9341_OutStringSize("|<",color, 2);

    ILI9341_drawHLine(130, 217, 70, ILI9341_RED, 3);
    ILI9341_drawHLine(100, 220, 100, ILI9341_BLACK, 2);


    ILI9341_DrawStringCord(318, 2, "Now Playing",ILI9341_BLACK, 1);
    ILI9341_drawHLine(0, 10, 320, ILI9341_BLACK, 2);

    ILI9341_SetCursor(52,2);
    color = (menu == np_ba) ? ILI9341_BLUE : ILI9341_BLACK;
    ILI9341_OutStringSize("Back",color, 2);

    ILI9341_fillRect(150, 50, 120, 120, ILI9341_WHITE);    
    //ILI9341_DrawBitmap(150,170,weeknd, 120, 120);

    ILI9341_SetCursor(20, 6);
    ILI9341_OutStringSize(Songs[SongStrIndex].song_name,ILI9341_BLACK, 1);
		
	ILI9341_SetCursor(20, 7);
    ILI9341_OutStringSize(Songs[SongStrIndex].album_name,ILI9341_BLACK, 1);
		
	ILI9341_SetCursor(20, 8);
    ILI9341_OutStringSize(Songs[SongStrIndex].artist_name,ILI9341_BLACK, 1);
		
}

void drawMusicPage(enum StateName menu){
    // for the music page 
    ILI9341_fillScreen(ILI9341_WHITE);
    ILI9341_drawVLine(160,12,240, ILI9341_BLACK, 2);
    ILI9341_DrawStringCord(318, 2, "Tpod",ILI9341_BLACK, 1);
    ILI9341_drawHLine(0, 12, 320, ILI9341_BLACK, 2);
	uint32_t color = ILI9341_BLACK;	

    ILI9341_SetCursor(52,2);
    color = (menu == song1) ? ILI9341_BLUE : ILI9341_BLACK;
    ILI9341_OutStringSize(Songs[0].song_name,color, 1);
    ILI9341_SetCursor(52,3);
    ILI9341_OutStringSize(Songs[0].artist_name,color, 1);
    ILI9341_drawHLine(160, 44, 160, ILI9341_BLACK, 2);

    ILI9341_SetCursor(52,5);
    color = (menu == song2) ? ILI9341_BLUE : ILI9341_BLACK;
    ILI9341_OutStringSize(Songs[1].song_name,color, 1);
    ILI9341_SetCursor(52,6);
    ILI9341_OutStringSize(Songs[1].artist_name,color, 1);
    ILI9341_drawHLine(160, 74, 160, ILI9341_BLACK, 2);


    ILI9341_SetCursor(52,8);
    color = (menu == song3) ? ILI9341_BLUE : ILI9341_BLACK;
    ILI9341_OutStringSize(Songs[2].song_name,color, 1);
    ILI9341_SetCursor(52,9);
    ILI9341_OutStringSize(Songs[2].artist_name,color, 1);
    ILI9341_drawHLine(160, 104, 160, ILI9341_BLACK, 2);

    /*
    11 - 
    12
    13

    14
    15
    16

    17
    18
    19

    20
    21
    22
    */
    ILI9341_SetCursor(14,18);
    color = (menu == mus_pl) ? ILI9341_BLUE : ILI9341_BLACK;
    ILI9341_OutStringSize("||",color, 2);
		
	ILI9341_SetCursor(8,18);
     color = (menu == mus_fa) ? ILI9341_BLUE : ILI9341_BLACK;
    ILI9341_OutStringSize(">|",color, 2);
		
	ILI9341_SetCursor(20,18);
     color = (menu == mus_re) ? ILI9341_BLUE : ILI9341_BLACK;
    ILI9341_OutStringSize("|<",color, 2);
		
		ILI9341_drawHLine(25, 220, 100, ILI9341_BLACK, 2);
		// using hte bytes of the song you can see how much time is left 
		// do it in dac_out - have a counter 25+((1-(bytes_done/bytes_total))*100) - for the x cord for the length its just (bytes_done/bytes_total)*100

   // you can also do bytes_done/ bytes_total - decimal part (100 + 25) - how much finished is starting position 
	ILI9341_drawHLine(75, 217, 50, ILI9341_RED, 3);
	
	ILI9341_fillRect(15, 25, 120, 120, ILI9341_WHITE);    
	// if(SongStrIndex == 0) {
    //     ILI9341_DrawBitmap(15,145,weeknd, 120, 120);
    // }    
    // else if(SongStrIndex == 1) {
    //     ILI9341_DrawBitmap(15,145,creep, 120, 120);
    // }   
    // else if(SongStrIndex == 2) {
    //     ILI9341_DrawBitmap(15,145,takeFive, 120, 120);
    // }   


    ILI9341_SetCursor(52,23);
     color = (menu == mus_ba) ? ILI9341_BLUE : ILI9341_BLACK;
    ILI9341_OutStringSize("Back",color, 1);

}

void drawSettingsPage(enum StateName menu) {
    ILI9341_fillScreen(ILI9341_WHITE);
    ILI9341_drawVLine(160,12,240, ILI9341_BLACK, 2);
    ILI9341_DrawStringCord(318, 2, "Tpod",ILI9341_BLACK, 1);
    ILI9341_drawHLine(0, 12, 320, ILI9341_BLACK, 2);
    uint32_t color = ILI9341_BLACK;	
    
    ILI9341_SetCursor(52,2);
    color = (menu == set_col) ? ILI9341_BLUE : ILI9341_BLACK;
    ILI9341_OutStringSize("Colors",color, 2);
    ILI9341_drawHLine(160, 44, 160, ILI9341_BLACK, 2);

    ILI9341_SetCursor(52,5);
    color = (menu == set_wifi) ? ILI9341_BLUE : ILI9341_BLACK;
    ILI9341_OutStringSize("WiFi",color, 2);
    ILI9341_drawHLine(160, 74, 160, ILI9341_BLACK, 2);

    ILI9341_SetCursor(52,23);
    color = (menu == set_bck) ? ILI9341_BLUE : ILI9341_BLACK;
    ILI9341_OutStringSize("Back",color, 1);
}