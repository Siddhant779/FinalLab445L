// Display.c 


#include "./inc/tm4c123gh6pm.h"
#include "Display.h"
#include "Music.h"
#include "FSM.h"



void drawMainMenu(enum StateName menu){
    //ILI9341_fillScreen(ILI9341_WHITE);
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
    color = (menu == menu_play) ? ILI9341_BLUE : ILI9341_BLACK;
    ILI9341_OutStringSize("Now Playing",color, 2);
    ILI9341_drawHLine(160, 104, 160, ILI9341_BLACK, 2);


    // ILI9341_SetCursor(52,11);
    // color = (menu == menu_play) ? ILI9341_BLUE : ILI9341_BLACK;
    // ILI9341_OutStringSize("Now Playing",color, 2);
    // ILI9341_drawHLine(160, 134, 160, ILI9341_BLACK, 2);



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
   if(progress_length == 0) {
        ILI9341_drawHLine(25, 217, 100, ILI9341_WHITE, 3);
    }
	ILI9341_drawHLine((100-progress_length)+25, 217, progress_length, ILI9341_RED, 3);
		
	//ILI9341_fillRect(15, 25, 120, 120, ILI9341_WHITE);
    //replacealbumCover(menu, false);
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
    //ILI9341_fillScreen(ILI9341_WHITE);
    ILI9341_SetCursor(28,19);
    color = (menu == np_pl) ? ILI9341_BLUE : ILI9341_BLACK;
    ILI9341_OutStringSize("||",color, 2);
		
    ILI9341_SetCursor(22,19);
    color = (menu == np_fa) ? ILI9341_BLUE : ILI9341_BLACK;
    ILI9341_OutStringSize(">|",color, 2);
		
	  ILI9341_SetCursor(34,19);
      color = (menu == np_re) ? ILI9341_BLUE : ILI9341_BLACK;
    ILI9341_OutStringSize("|<",color, 2);

    if(progress_length == 0) {
        ILI9341_drawHLine(100, 217, 100, ILI9341_WHITE, 3);
    }
    ILI9341_drawHLine((100-progress_length)+100, 217, progress_length, ILI9341_RED, 3);
    ILI9341_drawHLine(100, 220, 100, ILI9341_BLACK, 2);


    ILI9341_DrawStringCord(318, 2, "Now Playing",ILI9341_BLACK, 1);
    ILI9341_drawHLine(0, 10, 320, ILI9341_BLACK, 2);

    ILI9341_SetCursor(52,2);
    color = (menu == np_ba) ? ILI9341_BLUE : ILI9341_BLACK;
    ILI9341_OutStringSize("Back",color, 2);


    // ILI9341_SetCursor(21, );
    // ILI9341_OutStringSize(Songs[SongStrIndex].song_name,ILI9341_BLACK, 1);
		
	// ILI9341_SetCursor(21, 7);
    // ILI9341_OutStringSize(Songs[SongStrIndex].album_name,ILI9341_BLACK, 1);
		
	// ILI9341_SetCursor(21, 8);
    // ILI9341_OutStringSize(Songs[SongStrIndex].artist_name,ILI9341_BLACK, 1);
		
}

void drawMusicPage(enum StateName menu){
    // for the music page 
    //ILI9341_fillScreen(ILI9341_WHITE);
    ILI9341_drawVLine(160,12,240, ILI9341_BLACK, 2);
    ILI9341_DrawStringCord(318, 2, "Tpod",ILI9341_BLACK, 1);
    ILI9341_drawHLine(0, 12, 320, ILI9341_BLACK, 2);
	uint32_t color = ILI9341_BLACK;	

    uint8_t x = 52;
    uint8_t y = 2;

    for(int i = 0; i < 7; i++) {
       ILI9341_SetCursor(52,2);
       color = ((menu-6)== song1) ? ILI9341_BLUE : ILI9341_BLACK;
    }

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

    
    ILI9341_SetCursor(52,11);
    color = (menu == song4) ? ILI9341_BLUE : ILI9341_BLACK;
    ILI9341_OutStringSize(Songs[3].song_name,color, 1);
    ILI9341_SetCursor(52,12);
    ILI9341_OutStringSize(Songs[3].artist_name,color, 1);
    ILI9341_drawHLine(160, 134, 160, ILI9341_BLACK, 2);


    ILI9341_SetCursor(52,14);
    color = (menu == song5) ? ILI9341_BLUE : ILI9341_BLACK;
    ILI9341_OutStringSize(Songs[4].song_name,color, 1);
    ILI9341_SetCursor(52,15);
    ILI9341_OutStringSize(Songs[4].artist_name,color, 1);
    ILI9341_drawHLine(160, 164, 160, ILI9341_BLACK, 2);


    ILI9341_SetCursor(52,17);
    color = (menu == song6) ? ILI9341_BLUE : ILI9341_BLACK;
    ILI9341_OutStringSize(Songs[5].song_name,color, 1);
    ILI9341_SetCursor(52,18);
    ILI9341_OutStringSize(Songs[5].artist_name,color, 1);
    ILI9341_drawHLine(160, 194, 160, ILI9341_BLACK, 2);


    ILI9341_SetCursor(52,20);
    color = (menu == song7) ? ILI9341_BLUE : ILI9341_BLACK;
    ILI9341_OutStringSize(Songs[6].song_name,color, 1);
    ILI9341_SetCursor(52,21);
    ILI9341_OutStringSize(Songs[6].artist_name,color, 1);
    ILI9341_drawHLine(160, 224, 160, ILI9341_BLACK, 2);
    

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
    if(progress_length == 0) {
        ILI9341_drawHLine(25, 217, 100, ILI9341_WHITE, 3);
    }
	ILI9341_drawHLine((100-progress_length)+25, 217, progress_length, ILI9341_RED, 3);
	
    ILI9341_SetCursor(4,2);
     color = (menu == mus_ba) ? ILI9341_BLUE : ILI9341_BLACK;
    ILI9341_OutStringSize("Back",color, 1);

}

// void drawSettingsPage(enum StateName menu) {
//     //ILI9341_fillScreen(ILI9341_WHITE);
//     ILI9341_drawVLine(160,12,240, ILI9341_BLACK, 2);
//     ILI9341_DrawStringCord(318, 2, "Tpod",ILI9341_BLACK, 1);
//     ILI9341_drawHLine(0, 12, 320, ILI9341_BLACK, 2);
//     uint32_t color = ILI9341_BLACK;	
    
//     ILI9341_SetCursor(52,2);
//     color = (menu == set_col) ? ILI9341_BLUE : ILI9341_BLACK;
//     ILI9341_OutStringSize("Colors",color, 2);
//     ILI9341_drawHLine(160, 44, 160, ILI9341_BLACK, 2);

//     ILI9341_SetCursor(52,5);
//     color = (menu == set_wifi) ? ILI9341_BLUE : ILI9341_BLACK;
//     ILI9341_OutStringSize("WiFi",color, 2);
//     ILI9341_drawHLine(160, 74, 160, ILI9341_BLACK, 2);

//     ILI9341_SetCursor(52,23);
//     color = (menu == set_bck) ? ILI9341_BLUE : ILI9341_BLACK;
//     ILI9341_OutStringSize("Back",color, 1);
// }
