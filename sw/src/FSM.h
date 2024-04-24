#ifndef _FSMH_
#define _FSMH_
#include <stdint.h>


enum StateName{
    menu_mus, // Music option within menu
    menu_msg, // Message option within menu
    menu_set, // Settings option within menu
    menu_play, // Now-Playing option within menu
    menu_pl, // Play/Pause button within menu
    menu_fa, // Next-Song within menu
    menu_re, // Rewind within menu
    song1, //song_ within the music page
    song2, //for all songs above and below once you press the song it restarts it - stay at the same state
    song3,
    song4,
    song5,
    song6,
    song7,
    mus_pl, // Play/Pause button within music page 
    mus_fa, // Next-song within music page
    mus_re, // Rewind within music page 
    mus_ba, // Back button to menu within music page 
    np_pl, // Play/Pause option within Now-Playing
    np_fa, // Next-Song within Now-Playing
    np_re, // Rewind within Now-Playing
    np_ba, // Back button to menu within Now-Playing
    set_col, // Changing color option for setting page
    set_wifi, // Wifi option for setting page 
    set_bck, // Exits settings
    msg_key, // Keyboard button for message page 
    msg_bck // Back button to menu within message page 

};


typedef struct State{
    enum StateName name;
    void (*func)(uint8_t input);
    enum StateName Next[5];

} State_t;

void FSM_Init();

State_t Get_State();

void FSM_Controller(uint8_t input);

void Do_Nothing(uint8_t input);

void Set_state(enum StateName name);

void Start_song(uint8_t input);

void Play_pause(uint8_t input);

void Rewind_song(uint8_t input);

void Next_song(uint8_t input);

#endif