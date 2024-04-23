#include "Switch.h"
#include "Messages.h"
#include "ILI9341.h"

char keys[4][10] = {{'1','2','3','4','5','6','7','8','9','0'},
										{'q','w','e','r','t','y','u','i','o','p'},
										{'a','s','d','f','g','h','j','k','l', ';'},
										{'z','x','c','v','b','n','m',',','.','/'}};

										
coord_t character = {1,1};
void display_keys(void){
	int x = 40;
	int y = 10;
	ILI9341_SetCursor(x, y);
	for(int i = 0; i < 4; i++){
		for (int j = 0; j < 10; j++){
			uint32_t color = ILI9341_BLACK;
			if ((i == character.x) && (j == character.y))
				color = ILI9341_RED;
			ILI9341_OutChar(keys[i][j], 1, color);
			ILI9341_OutChar(' ', 1, ILI9341_BLACK);
		}
		y += 1;
		ILI9341_SetCursor(x, y);
	}
}

void change_coords(int x, int y){
	character.x = x;
	character.y = y;
}

void move_coords(uint8_t input){
	if (input == 1){ //up
		if (character.y != 0)
				character.y -= 1;
	}
	else if (input == 2){//right
		if (character.x != 9)
				character.x += 1;
	}
	else if (input == 3){//left
		if (character.x != 0)
				character.x -= 1;
	}
	else if (input == 4){//down
		if (character.y != 3)
				character.y += 1;
	}

}