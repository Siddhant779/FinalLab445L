#include <stdint.h>
#include <stdbool.h>
#include "./inc/tm4c123gh6pm.h"

typedef struct coord{
	uint8_t x;
	uint8_t y;
}coord_t;

void display_keys(void);

void change_coords(int x, int y);
void move_coords(uint8_t input);