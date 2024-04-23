#include "Switch.h"
#include "Messages.h"
#include "ILI9341.h"

char keys[4][10] = {{'1','2','3','4','5','6','7','8','9','0'},
										{'q','w','e','r','t','y','u','i','o','p'},
										{'a','s','d','f','g','h','j','k','l', ';'},
										{'z','x','c','v','b','n','m',',','.','/'}};

char* control_keys[4] = {"CAPS ","SPACE ", "<X ", "ENTER"};

										
coord_t character;
char type_message[64];
uint8_t message_index;

void message_init(void){
	for (int i = 0; i < 64; i++){
		type_message[i] = 0;
	}
	character.row = 0;
	character.col = 0;
	message_index = 0;
}
void display_keys(void){
	int x = 40;
	int y = 10;
	ILI9341_SetCursor(x, y);
	for(int i = 0; i < 4; i++){
		for (int j = 0; j < 10; j++){
			uint32_t color = ILI9341_BLACK;
			if ((i == character.row) && (j == character.col))
				color = ILI9341_RED;
			ILI9341_OutChar(keys[i][j], 1, color);
			ILI9341_OutChar(' ', 1, ILI9341_BLACK);
		}
		y += 1;
		ILI9341_SetCursor(x, y);
	}
	for (int i = 0; i < 4; i++){
		uint32_t color = ILI9341_BLACK;
		if ((character.row == 4) && (i == character.col))
				color = ILI9341_RED;
		ILI9341_OutString(control_keys[i], color);
	}
	y = 5;
	ILI9341_SetCursor(x, y);
	ILI9341_OutStringSize(type_message, ILI9341_BLACK, 2);
}

void change_coords(int row, int col){
	character.row = row;
	character.col = col;
}

void move_coords(uint8_t input){
	if (input == 1){ //up
		if (character.row == 4){ //bottom row to up
					if (character.col == 0) {
						character.col = 0;
					}
					else if (character.col == 1){
						character.col = 3;
					}
					else if (character.col == 2){
						character.col = 6;
					}
					else if (character.col == 3){
						character.col = 8;
					}			
		}
		if (character.row != 0)
				character.row -= 1;
	}
	else if (input == 2){//right
		if (character.row == 4){
			if (character.col != 3)
				character.col += 1;
		}
		else if (character.col != 9)
				character.col += 1;
	}
	else if (input == 3){//left
		if (character.col != 0)
				character.col -= 1;
	}
	else if (input == 4){//down
		if (character.row < 4){
				//for bottom row
				if (character.row == 3){
					if (character.col == 0 || character.col == 1 || character.col == 2){
						character.col = 0;
					}
					if (character.col == 3 || character.col == 4){
						character.col = 1;
					}
					if (character.col == 5 || character.col == 6){
						character.col = 2;
					}
					if (character.col == 7 || character.col == 8 || character.col == 9){
						character.col = 3;
					}			
				}
				character.row += 1;
		}

	}

}

void append_message(void){
	if (message_index < 63){
		if(character.row < 4){
			type_message[message_index] = keys[character.row][character.col];
			message_index++;
		}
		else if (character.row == 4){
			if (character.col == 0){//CAPS
			
			}
			else if (character.col == 1){//SPACE
				type_message[message_index] = ' ';
				message_index++;
			}
			else if (character.col == 2){//BACKSPACE
				message_index--;
				type_message[message_index] = 0;	
			}
			else if (character.col == 3){//ENTER
			
			}
		}
	}
}