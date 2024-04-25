// FSM.c
#include "FSM.h"
#include "./inc/tm4c123gh6pm.h"
#include "Display.h"
#include "Music.h"
#include <stdint.h>
#include "Messages.h"

/*
Inputs:
0 = Up
1 = Right
2 = Left
3 = Down
4 = Enter
*/ 
const State_t FSM[22] = {
    {menu_mus, &screen_clear, {menu_play, menu_re, menu_mus, menu_msg, song1}},
    {menu_msg, &screen_clear, {menu_mus, menu_re, menu_msg, menu_play, msg_key}},
    {menu_play, &screen_clear, {menu_msg, menu_re, menu_play, menu_mus, np_pl}},
    {menu_pl, &Play_pause, {menu_pl, menu_fa, menu_re, menu_pl, menu_pl}},
    {menu_fa, &Next_song, {menu_fa, menu_fa, menu_pl, menu_fa, menu_fa}},
    {menu_re, &Rewind_song, {menu_re, menu_pl, menu_mus, menu_re, menu_re}},
    {song1, &Start_song, {song1, mus_re, song1, song2, song1}},
    {song2, &Start_song, {song1, mus_re, song2, song3, song2}},
    {song3, &Start_song, {song2, mus_re, song3, song4, song3}},
    {song4, &Start_song, {song3, mus_re, song4, song5, song4}},
    {song5, &Start_song, {song4, mus_re, song5, song6, song5}},
    {song6, &Start_song, {song5, mus_re, song6, song7, song6}},
    {song7, &Start_song, {song6, mus_re, song7, song7, song7}},
    {mus_pl, &Play_pause, {mus_ba, mus_fa, mus_re, mus_pl, mus_pl}},
    {mus_fa, &Next_song, {mus_ba, mus_fa, mus_pl, mus_fa, mus_fa}},
    {mus_re, &Rewind_song, {mus_ba, mus_pl, song1, mus_re, mus_re}},
    {mus_ba, &screen_clear, {mus_ba, mus_ba, song1, mus_fa, menu_mus}},
    {np_pl, &Play_pause, {np_ba, np_fa, np_re, np_pl, np_pl}},
    {np_fa, &Next_song, {np_ba, np_fa, np_pl, np_fa, np_fa}},
    {np_re, &Rewind_song, {np_ba, np_pl, np_ba, np_re, np_re}},
    {np_ba, &screen_clear, {np_ba, np_re, np_ba, np_re, menu_mus}},
    {msg_key, &keys_cursor, {msg_key, msg_key, msg_key, msg_key, msg_key}}
    
};
//{msg_bck, &screen_clear, {menu_mus, msg_key, menu_mus, msg_key, menu_mus}}
State_t current_state;
uint8_t clear_flag;
uint8_t jump_state_flag;

void FSM_Init(){
    current_state = FSM[0];
	clear_flag = 0;
	jump_state_flag = 0;
}

uint8_t get_clear_flag(void){
	return clear_flag;
}

void set_clear_flag(uint8_t flag){
	clear_flag = flag;
}

uint8_t get_jump_flag(void){
	return jump_state_flag;
}

void set_jump_flag(uint8_t flag){
	jump_state_flag = flag;
}

State_t Get_State(){
		return current_state;
}

void Set_state(enum StateName name){
    current_state = FSM[name];
}

/*
Inputs:
1 = Up
2 = Right
3 = Left
4 = Down
5 = Enter
*/ 
void FSM_Controller(uint8_t input){
    (*(current_state.func))(input); //do function based on input
		if (jump_state_flag == 0)//if we hijacked
			current_state = FSM[current_state.Next[input-1]]; //change state
		else
			jump_state_flag = 0;
}

void Do_Nothing(uint8_t input){
	return;
}

void Play_pause(uint8_t input) {
	if (input == 5){
    if(is_playing()) pause_song();
    else unpause_song();
	}
}

void Rewind_song(uint8_t input) {
    //CALL LoadBitmap AND replacealbumCover TO REPLACE THE IMAGE
	if (input == 5){
		rewind_song();
	}
}

void Next_song(uint8_t input) {
	if (input == 5){
        // Close current song file
        close_song();
        // Increment song index
        SongStrIndex = (SongStrIndex+1)%15;
        // Load in new album cover bitmap
        LoadBitmap(Songs[SongStrIndex].album_file);
        replacealbumCover(Get_State().name, true);
        // Open new song file
        load_song();
	}
    // Load and start the next song in the list
}

void Start_song(uint8_t input) {
	if (input == 5){
			//first set the SongStrIndex based on the state
            close_song();
			SongStrIndex =  ((uint8_t)(Get_State().name) - 6 + top)%15; // REPLACE 8 WITH NUMBER OF SONGS IN STRUCT
			LoadBitmap(Songs[SongStrIndex].album_file);
            replacealbumCover(Get_State().name, true);
			// Call function that opens song file and sets flag8
            //CLOSE THE OLD SONG - CALL THE FUNCTION FOR THAT - USING OLDSONG VARIABLE
            // OPEN THE NEW SONG USING THE NEW SONGSTRINDEX
            load_song();
			// depending if your in the main menu or music state  - you redraw only the right side 
			// if you are in the now playing state then you redraw that whole thing 
	}
    else if (input == 1 && current_state.name == song1){//scroll up
        uint8_t stat = dec_top();
        if (stat)
            set_clear_flag(1);
    }
    else if (input == 4 && current_state.name == song7){//scroll down
        uint8_t stat = inc_top();
        if (stat)
            set_clear_flag(1);
    }
};

void screen_clear(uint8_t input){
	if (input == 5)
		set_clear_flag(1); //we need to erase the screen
}