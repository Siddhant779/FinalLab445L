#ifndef _SDH_
#define _SDH_
#include <stdint.h>

//init function for starting it up 
extern void SD_initSDCard(void);

//for opening the file based on the filename 
extern void SD_openFile(char* filename);

//for closing the file based on the filename
extern void SD_closeFile(char *filename);

//for reading the file - pass in what buffer you want and how many bytes for reading 
extern void SD_read(uint8_t* buffer, uint32_t numBytes);

//for reading the next Char 
extern char SD_readNextChar();


#endif
