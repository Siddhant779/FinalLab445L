#include "Switch.h"
#include "Messages.h"
#include "ILI9341.h"
#include <string.h>
#include "FSM.h"
#include "../inc/esp8266_base.h"

char (*keys)[4][10];


char lower[4][10] = {{'1','2','3','4','5','6','7','8','9','0'},
										{'q','w','e','r','t','y','u','i','o','p'},
										{'a','s','d','f','g','h','j','k','l', ';'},
										{'z','x','c','v','b','n','m',',','.','/'}};

char upper[4][10] = {{'!','@','#','$','%','^','&','*','(',')'},
										{'Q','W','E','R','T','Y','U','I','O','P'},
										{'A','S','D','F','G','H','J','K','L', ':'},
										{'Z','X','C','V','B','N','M','<','>','?'}};

char* control_keys[4] = {"CAPS ","SPACE ", "<X ", "ENTER"};


char hist_zero[40];
char hist_one[40];
char hist_two[40];
char hist_three[40];
char hist_four[40];
char hist_five[40];
char hist_six[40];
char hist_seven[40];
char hist_eight[40];
char hist_nine[40];
Message history[10] = {{hist_zero,0} , {hist_one,0}, {hist_two,0}, {hist_three,0}, {hist_four,0}, 
												{hist_five,0}, {hist_six,0}, {hist_seven,0}, {hist_eight,0}, {hist_nine,0}};

uint8_t history_idx;

										
coord_t character;
char type_message[40];
uint8_t message_index;
uint8_t shifted;
uint8_t need_erase_text;


void message_init(void){
	for (int i = 0; i < 40; i++){
		type_message[i] = 0;
	}
	character.row = 0;
	character.col = 0;
	message_index = 0;
	keys = &lower;
	shifted = 0;
	need_erase_text = 0;
	history_idx = 0;
}

void message_erase(void){
	for (int i = 0; i < 40; i++){
		type_message[i] = 0;
	}
	need_erase_text = 1;
	message_index = 0;
}
void display_keys(void){
	int x = 35;
	int y = 18;
	ILI9341_SetCursor(x, y);
	for(int i = 0; i < 4; i++){ //print keys
		for (int j = 0; j < 10; j++){
			uint32_t color = ILI9341_BLACK;
			if ((i == character.row) && (j == character.col))
				color = ILI9341_RED;
			ILI9341_OutChar((*keys)[i][j], 1, color);
			ILI9341_OutChar(' ', 1, ILI9341_BLACK);
		}
		y += 1;
		ILI9341_SetCursor(x, y);
	}
	for (int i = 0; i < 4; i++){ //last row
		uint32_t color = ILI9341_BLACK;
		if ((character.row == 4) && (i == character.col))
				color = ILI9341_RED;
		ILI9341_OutString(control_keys[i], color);
	}
	y = 17;
	x = 52;
	if (need_erase_text){
		ILI9341_SetCursor(x, y);
		ILI9341_fillRect(0,168, 313, 10, ILI9341_WHITE);
		need_erase_text = 0;
	}
	ILI9341_SetCursor(x, y);
	ILI9341_OutStringSize("Msg: ", ILI9341_BLACK, 1);//menu stuff
	ILI9341_OutStringSize(type_message, ILI9341_BLACK, 1);
	ILI9341_SetCursor(52,1);
	uint32_t color = ILI9341_BLACK;
	if ((character.row == -1) || (character.col == -1))
			color = ILI9341_BLUE;
	ILI9341_OutStringSize("Back", color, 2);
	y = 1;
	x = 35;
	ILI9341_SetCursor(x,y);
	ILI9341_OutStringSize("Messaging", ILI9341_BLACK, 2);
	y = 4;
	ILI9341_SetCursor(x,y);
	for (int i = 0; i < history_idx; i++){ //print the message history
		uint32_t color = (history[i].owner) ? ILI9341_GREEN : ILI9341_BLUE;
		ILI9341_OutString(history[i].msg, color);
		y++;
		ILI9341_SetCursor(x,y);
	}
}

void change_coords(int row, int col){
	character.row = row;
	character.col = col;
}

void keys_cursor(uint8_t input){
	if (input < 5 && input > 0){
		move_coords(input);
	}
	else if (input == 5){
		append_message();
	}


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
		if (character.row > 0)
				character.row -= 1;
		else if (character.row == 0 && character.col == 0){ //set to back
			character.row = -1;
			character.col = -1;
		}
	}
	else if (input == 2){//right

		if (character.row == 4){
			if (character.col != 3)
				character.col += 1;
		}
		else if (character.row == -1 && character.col == -1){ //set to 1 again
			character.row = 0;
			character.col = 0;
		}
		else if (character.col < 9)
			character.col += 1;
		
	}
	else if (input == 3){//left
		if (character.col > 0)
				character.col -= 1;
		else if (character.row == 0 && character.col == 0){ //set to back
			character.row = -1;
			character.col = -1;
		}
	}
	else if (input == 4){//down
		if (character.row == -1 && character.col == -1){ //set to back
			character.row = 0;
			character.col = 0;
		}
		else if (character.row < 4){
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
	if (message_index < 39){
		if(character.row < 4 && character.row >= 0){
			type_message[message_index] = (*keys)[character.row][character.col];
			message_index++;
		}
		else if (character.row == 4){
			if (character.col == 0){//CAPS
				shifted ^= 0x01;
				if (shifted == 0x00)
					keys = &lower;
				else
					keys = &upper;
				
			}
			else if (character.col == 1){//SPACE
				type_message[message_index] = ' ';
				message_index++;
			}
			else if (character.col == 2){//BACKSPACE
				message_index--;
				type_message[message_index] = 0;	
				need_erase_text = 1;
			}
			else if (character.col == 3){//ENTER
				Message msg = {type_message, 0};
//				uint8_t str_len = strlen(type_message);
//				strncpy(msg.msg, type_message, BUFFER_SIZE - 1);
//				msg.msg[str_len] = '\0';
//				msg.owner = 0;
				sendMessage(&msg);
				add_history(type_message, 0);
			}
		}
		else if (character.row == -1 && character.col == -1){//go back in the menu
			set_clear_flag(1);
			set_jump_flag(1);
			Set_state(menu_msg);
		}
	}
}

void add_history(char* msg, uint8_t owner){
				uint8_t length = strlen(msg);
				if (history_idx < 10){
					history[history_idx].msg = strncpy(history[history_idx].msg, msg, length);
					(history[history_idx].msg)[length] = 0;
					history[history_idx].owner = owner;
					history_idx++;
					message_erase();
				}
				if (history_idx == 10){
					Message temp = history[0];
					for (int i = 0; i < 9; i++){
						history[i] = history[i+1];
					}
					history[9] = temp;
					history[9].msg = strncpy(history[9].msg, msg, length);
					(history[9].msg)[length] = 0;
					history[9].owner = owner;
					set_clear_flag(1);
					message_erase();
				}
}