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
const State_t FSM[27] = {
    {menu_mus, &screen_clear, {menu_play, menu_re, menu_mus, menu_msg, song1}},
    {menu_msg, &screen_clear, {menu_mus, menu_re, menu_msg, menu_set, msg_key}},
    {menu_set, &screen_clear, {menu_msg, menu_re, menu_set, menu_play, set_col}},
    {menu_play, &screen_clear, {menu_set, menu_re, menu_play, menu_mus, np_pl}},
    {menu_pl, &Play_pause, {menu_pl, menu_fa, menu_re, menu_pl, menu_pl}},
    {menu_fa, &Next_song, {menu_fa, menu_fa, menu_pl, menu_fa, menu_fa}},
    {menu_re, &Rewind_song, {menu_re, menu_pl, menu_mus, menu_re, menu_re}},
    {song1, &Do_Nothing, {song7, mus_re, song1, song2, song1}},
    {song2, &Do_Nothing, {song1, mus_re, song2, song3, song2}},
    {song3, &Do_Nothing, {song2, mus_re, song3, song4, song3}},
    {song4, &Do_Nothing, {song3, mus_re, song4, song5, song4}},
    {song5, &Do_Nothing, {song4, mus_re, song5, song6, song5}},
    {song6, &Do_Nothing, {song5, mus_re, song6, song7, song6}},
    {song7, &Do_Nothing, {song6, mus_re, song7, song1, song7}},
    {mus_pl, &Play_pause, {mus_ba, mus_fa, mus_re, mus_pl, mus_pl}},
    {mus_fa, &Next_song, {mus_ba, mus_fa, mus_pl, mus_fa, mus_fa}},
    {mus_re, &Rewind_song, {mus_ba, mus_pl, song1, mus_re, mus_re}},
    {mus_ba, &screen_clear, {mus_ba, mus_ba, song1, mus_fa, menu_mus}},
    {np_pl, &Play_pause, {np_ba, np_fa, np_re, np_pl, np_pl}},
    {np_fa, &Next_song, {np_ba, np_fa, np_pl, np_fa, np_fa}},
    {np_re, &Rewind_song, {np_ba, np_pl, np_ba, np_re, np_re}},
    {np_ba, &screen_clear, {np_ba, np_re, np_ba, np_re, menu_mus}},
    {set_col, &Do_Nothing, {menu_mus, menu_mus, menu_mus, menu_mus, menu_mus}}, // DO THIS LATER
    {set_wifi, &Do_Nothing, {menu_mus, menu_mus, menu_mus, menu_mus, menu_mus}}, // DO THIS LATER
    {set_bck, &screen_clear, {menu_mus, menu_mus, menu_mus, menu_mus, menu_mus}}, // DO THIS LATER
    {msg_key, &keys_cursor, {msg_key, msg_key, msg_key, msg_key, msg_key}},
    {msg_bck, &screen_clear, {menu_mus, msg_key, menu_mus, msg_key, menu_mus}}
};

State_t current_state;
uint8_t clear_flag;


void FSM_Init(){
    current_state = FSM[0];
		clear_flag = 1;
}

uint8_t get_clear_flag(void){
	return clear_flag;
}

void set_clear_flag(uint8_t flag){
	clear_flag = flag;
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
    current_state = FSM[current_state.Next[input-1]]; //change state
}

void Do_Nothing(uint8_t input){
	return;
}

void Play_pause(uint8_t input) {
    if(is_playing()) pause_song();
    else unpause_song();
}

void Rewind_song(uint8_t input) {
    rewind_song();
}

void Next_song(uint8_t input) {
    // Load and start the next song in the list
}

void Start_song(uint8_t input) {
    //first set the SongStrIndex based on the state 
    SongStrIndex =  (uint8_t)(Get_State().name) - 7; // replaces with the song 
    //get the album file and song_file based on the struct
    
    // Load album cover from SD and load into bitmap array
    // Call function that opens song file and sets flag8
    flag8 = 1;
    // depending if your in the main menu or music state  - you redraw only the right side 
    // if you are in the now playing state then you redraw that whole thing 
};

void screen_clear(uint8_t input){
	if (input == 5)
		set_clear_flag(1); //we need to erase the screen
}