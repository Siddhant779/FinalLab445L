// FSM.c
#include "FSM.h"
#include "./inc/tm4c123gh6pm.h"
#include "Display.h"
#include <stdint.h>

/*
Inputs:
0 = Up
1 = Right
2 = Left
3 = Down
4 = Enter
*/ 
State_t FSM[26] = {
    {menu_mus, &Do_Nothing, {menu_play, menu_re, menu_mus, menu_msg, song1}},
    {menu_msg, &Do_Nothing, {menu_mus, menu_re, menu_msg, menu_set, msg_key}},
    {menu_set, &Do_Nothing, {menu_msg, menu_re, menu_set, menu_play, menu_mus}},
    {menu_play, &Do_Nothing, {menu_set, menu_re, menu_play, menu_mus, menu_mus}},
    {menu_pl, &Do_Nothing, {menu_mus, menu_mus, menu_mus, menu_mus, menu_mus}},
    {menu_fa, &Do_Nothing, {menu_mus, menu_mus, menu_mus, menu_mus, menu_mus}},
    {menu_re, &Do_Nothing, {menu_mus, menu_mus, menu_mus, menu_mus, menu_mus}},
    {song1, &Do_Nothing, {song7, mus_re, song1, song2, song1}},
    {song2, &Do_Nothing, {song1, mus_re, song2, song3, song2}},
    {song3, &Do_Nothing, {song2, mus_re, song3, song4, song3}},
    {song4, &Do_Nothing, {song3, mus_re, song4, song5, song4}},
    {song5, &Do_Nothing, {song4, mus_re, song5, song6, song5}},
    {song6, &Do_Nothing, {song5, mus_re, song6, song7, song6}},
    {song7, &Do_Nothing, {song6, mus_re, song7, song1, song7}},
    {mus_pl, &Do_Nothing, {mus_pl, menu_fa, menu_re, mus_pl, mus_pl}},
    {mus_fa, &Do_Nothing, {mus_fa, menu_fa, mus_pl, mus_fa, mus_fa}},
    {mus_re, &Do_Nothing, {menu_mus, menu_mus, menu_mus, menu_mus, menu_mus}},
    {mus_ba, &Do_Nothing, {menu_mus, menu_mus, menu_mus, menu_mus, menu_mus}},
    {np_pl, &Do_Nothing, {menu_mus, menu_mus, menu_mus, menu_mus, menu_mus}},
    {np_fa, &Do_Nothing, {menu_mus, menu_mus, menu_mus, menu_mus, menu_mus}},
    {np_re, &Do_Nothing, {menu_mus, menu_mus, menu_mus, menu_mus, menu_mus}},
    {np_ba, &Do_Nothing, {menu_mus, menu_mus, menu_mus, menu_mus, menu_mus}},
    {set_col, &Do_Nothing, {menu_mus, menu_mus, menu_mus, menu_mus, menu_mus}},
    {set_wifi, &Do_Nothing, {menu_mus, menu_mus, menu_mus, menu_mus, menu_mus}},
    {msg_key, &Do_Nothing, {msg_key, msg_key, msg_key, msg_key, msg_key}},
    {msg_bck, &Do_Nothing, {menu_mus, msg_key, menu_mus, msg_key, menu_mus}},
};

State_t current_state;


void FSM_Init(){
    current_state = FSM[0];
}

State_t Get_State(){
		return current_state;
}

void Set_state(StateName name){
    current_state = nme
}

void FSM_Controller(uint8_t input){
    (*(current_state.func))(input); //do function based on input
    current_state = FSM[current_state.Next[input-1]]; //change state
}
void Do_Nothing(uint8_t input){
		return;
}