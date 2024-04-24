// Music.h
// Driver for playing songs

#include <stdint.h>
#include <stdbool.h>

extern uint16_t Volume;
extern uint16_t DacData;

#define BUFSIZE8 512
extern uint8_t Buf[BUFSIZE8];
extern uint8_t Buf2[BUFSIZE8];
extern uint32_t Count8;
extern uint8_t *front8; // buffer being output to DAC
extern uint8_t *back8;  // buffer being loaded from SDC
extern int flag8; // 1 means need data into back
#define NUMBUF8 (2241504/BUFSIZE8)
extern uint32_t BufCount8; // 0 to NUMBUF8-1
extern uint8_t done_song;
extern uint8_t stop_dac;
extern uint32_t counterSong;

typedef struct Music{
    char *album_file; // name of the file to read from sd card for album bitmap image
    char *song_file; // name of the file to read from sd card for song 
    char *song_name; // name of song
    char *artist_name; // name of artist
    char *album_name; // name of album

    uint32_t byte_size; // size of the song

} Music;

extern Music Songs[3];

extern uint8_t SongStrIndex;
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