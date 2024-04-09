// Music.h
// Driver for playing songs

#include <stdint.h>
#include <stdbool.h>

extern uint16_t Volume;
extern uint16_t DacData;

// Initialize music driver
void music_init(void);

// Load a song in the form of an array
// Inputs: 
// 1) song: An song in array form (converted from a wav file)
// 2) length: The length of the array
void load_song(const uint16_t* song, uint32_t length);

// Pause whatever song is currently playing
void pause_song(void);

// Unpause a song that is currently paused
void unpause_song(void);

// Rewind to the beginning of the currently playing(or queued) song
void rewind_song(void);

// Check whether a song is currently playing
// Returns true if a song is playing, returns false otherwise
bool is_playing(void);