#include <stdint.h>
#include <stdbool.h>
#include "./inc/tm4c123gh6pm.h"
#include "../inc/esp8266_base.h"

typedef struct coord{
	int8_t row;
	int8_t col;
}coord_t;

void display_keys(void);

void change_coords(int row, int col);
void move_coords(uint8_t input);

void append_message(void);
void message_init(void);
void message_erase(void);
void keys_cursor(uint8_t input);
void add_history(Message msg);