#include "Switch.h"
#include "Messages.h"
#include "ILI9341.h"

char keys[4][10] = {{'1','2','3','4','5','6','7','8','9','0'},
										{'q','w','e','r','t','y','u','i','o','p'},
										{'a','s','d','f','g','h','j','k','l', ';'},
										{'z','x','c','v','b','n','m',',','.','/'}};

void displayKeys(void){
	int x = 40;
	int y = 10;
	ILI9341_SetCursor(x, y);
	for(int i = 0; i < 4; i++){
		for (int j = 0; j < 10; j++){
			ILI9341_OutChar(keys[i][j], 1, ILI9341_SKYBLUE);
			ILI9341_OutChar(' ', 1, ILI9341_SKYBLUE);
		}
		y += 1;
		ILI9341_SetCursor(x, y);
	}
}