#include <stdint.h>
#include <stdbool.h>
#include "./inc/tm4c123gh6pm.h"

typedef struct coord{
	uint8_t row;
	uint8_t col;
}coord_t;

void display_keys(void);

void change_coords(int row, int col);
void move_coords(uint8_t input);

void append_message(void);
void message_init(void);