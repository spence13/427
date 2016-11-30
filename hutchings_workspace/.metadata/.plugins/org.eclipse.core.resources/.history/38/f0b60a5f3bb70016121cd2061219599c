<<<<<<< HEAD
#include "tank.h"
#include "stats.h"
#include "globals.h"

#define TANK_BITS_X 15
#define TANK_BITS_Y 8
#define TANK_MIDDLE 7
#define TANK_LEFT_TOP 4
#define TANK_LEFT_TWO 2
#define TANK_RIGHT_TOP 10
#define TANK_PIX_WIDTH 30
#define TANK_PIX_HEIGHT 16

// Packs each horizontal line of the figures into a single 15 bit word.
#define packword15(b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0) \
((b14 << 14) | (b13 << 13) | (b12 << 12) | (b11 << 11) | (b10 << 10) | (b9  << 9 ) | (b8  << 8 ) |	\
 (b7  << 7 ) | (b6  << 6 ) | (b5  << 5 ) | (b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ))

static const uint16_t tank_15x8[] = // This is the pixel pattern for the tank
{
	packword15(0,0,0,0,0,0,0,1,0,0,0,0,0,0,0),
	packword15(0,0,0,0,0,0,1,1,1,0,0,0,0,0,0),
	packword15(0,0,0,0,0,0,1,1,1,0,0,0,0,0,0),
	packword15(0,1,1,1,1,1,1,1,1,1,1,1,1,1,0),
	packword15(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
	packword15(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
	packword15(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
	packword15(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1)
};

static const uint16_t tank_dying1_15x8[] = // This is the pixel pattern for the tank
{
	packword15(0,1,0,0,0,0,0,1,0,0,0,0,0,0,0),
	packword15(0,0,1,0,0,0,0,0,0,1,0,0,0,0,0),
	packword15(0,0,0,0,1,0,1,1,0,0,1,0,1,0,0),
	packword15(0,1,0,1,0,0,0,1,1,0,0,1,0,1,0),
	packword15(0,0,0,0,1,0,1,1,0,1,0,1,0,0,0),
	packword15(0,0,0,1,0,1,0,1,1,1,1,0,1,0,0),
	packword15(0,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
	packword15(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1)
};

static const uint16_t tank_dying2_15x8[] = // This is the pixel pattern for the tank
{
	packword15(1,0,1,0,1,0,0,0,0,0,0,0,1,0,0),
	packword15(0,0,0,0,0,1,0,0,1,0,1,0,0,0,0),
	packword15(0,1,0,0,0,0,0,0,0,0,0,0,1,0,1),
	packword15(0,0,0,1,1,1,1,0,0,0,1,0,1,0,0),
	packword15(0,0,1,1,1,1,1,1,0,0,0,1,1,1,0),
	packword15(1,1,1,1,1,1,1,1,1,0,1,1,1,1,1),
	packword15(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
	packword15(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1)
};

static int16_t tank_x_position = GLOBALS_MAX_X/GLOBALS_TWO; // Top left corner of the tank
int8_t lives_remaining = 3;

uint8_t tank_get_lives_remaining(){
	return lives_remaining;
}

// Returns the value of the tank at a certain row
uint16_t tank_get_tank_bits(uint16_t row){
	return tank_15x8[row];
}

uint16_t tank_get_tank_dying2_bits(uint16_t row){
	return tank_dying2_15x8[row];
}

uint16_t tank_get_tank_dying1_bits(uint16_t row){
	return tank_dying1_15x8[row];
}

// Getter function for the global x position
uint16_t tank_get_tank_x_position(){
	return tank_x_position;
}

//Function that will erase a block of 4 pixels
static void erase_block(unsigned int * framePointer, uint16_t x, uint16_t y)
{
	 framePointer[(GLOBALS_TANK_Y_POS+(GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + tank_x_position + (GLOBALS_DOUBLE*x)] = GLOBALS_BLACK;
	 framePointer[(GLOBALS_TANK_Y_POS+(GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + tank_x_position + (GLOBALS_DOUBLE*x)] = GLOBALS_BLACK;
	 framePointer[(GLOBALS_TANK_Y_POS+(GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + tank_x_position + (GLOBALS_DOUBLE*x)+1] = GLOBALS_BLACK;
	 framePointer[(GLOBALS_TANK_Y_POS+(GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + tank_x_position + (GLOBALS_DOUBLE*x)+1] = GLOBALS_BLACK;
}

//Function that will draw a block of 4 pixels
static void draw_block(unsigned int * framePointer, uint16_t x, uint16_t y)
{
	 framePointer[(GLOBALS_TANK_Y_POS+(GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + tank_x_position + (GLOBALS_DOUBLE*x)] = GLOBALS_GREEN;
	 framePointer[(GLOBALS_TANK_Y_POS+(GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + tank_x_position + (GLOBALS_DOUBLE*x)] = GLOBALS_GREEN;
	 framePointer[(GLOBALS_TANK_Y_POS+(GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + tank_x_position + (GLOBALS_DOUBLE*x)+1] = GLOBALS_GREEN;
	 framePointer[(GLOBALS_TANK_Y_POS+(GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + tank_x_position + (GLOBALS_DOUBLE*x)+1] = GLOBALS_GREEN;
}

//This function will draw a green tank given the framePointer
void tank_draw(unsigned int * framePointer)
{
    uint16_t x, y; //Iterators
    for(y=0; y < TANK_BITS_Y; y++){ //Loop through the height of the tank
		for(x=0; x < TANK_BITS_X; x++){ //Loop through the width of the tank
			 if(tank_get_tank_bits(y) & (1<<x)){ //If the tank pixel is a 1 at this position, draw a 4 pixel green box
				 framePointer[(GLOBALS_TANK_Y_POS+(GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + tank_x_position + (GLOBALS_DOUBLE*x)] = GLOBALS_GREEN;
				 framePointer[(GLOBALS_TANK_Y_POS+(GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + tank_x_position + (GLOBALS_DOUBLE*x)] = GLOBALS_GREEN;
				 framePointer[(GLOBALS_TANK_Y_POS+(GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + tank_x_position + (GLOBALS_DOUBLE*x)+1] = GLOBALS_GREEN;
				 framePointer[(GLOBALS_TANK_Y_POS+(GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + tank_x_position + (GLOBALS_DOUBLE*x)+1] = GLOBALS_GREEN;
			 }
		}
    }
}

//This function will erase the green tank given the framePointer
static void tank_erase(unsigned int * framePointer)
{
    uint16_t x, y; //Iterators
    for(y=0; y < TANK_BITS_Y; y++){ //Loop through the height of the tank
		for(x=0; x < TANK_BITS_X; x++){ //Loop through the width of the tank
			 if(tank_get_tank_bits(y) & (1<<x)){ //If the tank pixel is a 1 at this position, draw a 4 pixel black box
				 framePointer[(GLOBALS_TANK_Y_POS+(GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + tank_x_position + (GLOBALS_DOUBLE*x)] = GLOBALS_BLACK;
				 framePointer[(GLOBALS_TANK_Y_POS+(GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + tank_x_position + (GLOBALS_DOUBLE*x)] = GLOBALS_BLACK;
				 framePointer[(GLOBALS_TANK_Y_POS+(GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + tank_x_position + (GLOBALS_DOUBLE*x)+1] = GLOBALS_BLACK;
				 framePointer[(GLOBALS_TANK_Y_POS+(GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + tank_x_position + (GLOBALS_DOUBLE*x)+1] = GLOBALS_BLACK;
			 }
		}
    }
}

//This function will shift the tank left a frame, takes the framePointer
void tank_shift_left(unsigned int * framePointer)
{
	if (tank_x_position-GLOBALS_SHIFT_VAL < GLOBALS_MIN_X) //Check if tank is going to shift off the left
		return;

	//hard code tank shift
    uint16_t x, y; //Iterators
	erase_block(framePointer, TANK_MIDDLE, 0);
    for(y=1; y <= TANK_LEFT_TWO; y++){ //Loop through the height of the tank
		for(x=(TANK_MIDDLE-1); x <= (TANK_MIDDLE+1); x++)//Loop through the width of the tank
			erase_block(framePointer, x, y);
    }
	for(x=(TANK_RIGHT_TOP-1); x <= (TANK_RIGHT_TOP+TANK_LEFT_TOP-1); x++) //Loop through the width of the tank
		erase_block(framePointer, x, (TANK_LEFT_TOP-1));
    for(y=TANK_LEFT_TOP; y <= TANK_MIDDLE; y++){ //Loop through the height of the tank
		for(x=TANK_RIGHT_TOP; x <= (TANK_RIGHT_TOP+TANK_LEFT_TOP); x++) //Loop through the width of the tank
			erase_block(framePointer, x, y);
    }

	tank_x_position = tank_x_position-GLOBALS_SHIFT_VAL; //Shift the pointer position left
	if (tank_x_position <= GLOBALS_MIN_X) //Make sure we don't pass the edge of the screen
		tank_x_position = GLOBALS_MIN_X;

	draw_block(framePointer, TANK_MIDDLE, 0);
    for(y=1; y <= TANK_LEFT_TWO; y++){ //Loop through the height of the tank
		for(x=(TANK_MIDDLE-1); x <= (TANK_MIDDLE+1); x++) //Loop through the width of the tank
			draw_block(framePointer, x, y);
    }
	for(x=1; x <= (TANK_LEFT_TOP+1); x++) //Loop through the width of the tank
		draw_block(framePointer, x, (TANK_LEFT_TOP-1));
    for(y=TANK_LEFT_TOP; y <= TANK_MIDDLE; y++){ //Loop through the height of the tank
		for(x=0; x <= TANK_LEFT_TOP; x++) //Loop through the width of the tank
			draw_block(framePointer, x, y);
    }
}

//This function will shift the tank right a frame, takes the framePointer
void tank_shift_right(unsigned int * framePointer)
{
	if (tank_x_position+GLOBALS_SHIFT_VAL > GLOBALS_MAX_X-GLOBALS_TANK_PIX_WIDTH) //check if tank will shift off the right side
		return;
	//hard code tank shift
    uint16_t x, y; //Iterators
	erase_block(framePointer, TANK_MIDDLE, 0);
    for(y=1; y <= TANK_LEFT_TWO; y++){ //Loop through the height of the tank
		for(x=(TANK_MIDDLE-1); x <= (TANK_MIDDLE+1); x++) //Loop through the width of the tank
			erase_block(framePointer, x, y);
    }
	for(x=1; x <= (TANK_LEFT_TOP+1); x++) //Loop through the width of the tank
		erase_block(framePointer, x, (TANK_LEFT_TOP-1));
    for(y=TANK_LEFT_TOP; y <= TANK_MIDDLE; y++){ //Loop through the height of the tank
		for(x=0; x <= TANK_LEFT_TOP; x++) //Loop through the width of the tank
			erase_block(framePointer, x, y);
    }

	tank_x_position = tank_x_position+GLOBALS_SHIFT_VAL; //Shift the pointer position left
	if (tank_x_position >= GLOBALS_MAX_X-GLOBALS_TANK_PIX_WIDTH) //Make sure we don't pass the edge of the screen
		tank_x_position = GLOBALS_MAX_X-GLOBALS_TANK_PIX_WIDTH;

	draw_block(framePointer, TANK_MIDDLE, 0);
    for(y=1; y <= TANK_LEFT_TWO; y++){ //Loop through the height of the tank
		for(x=(TANK_MIDDLE-1); x <= (TANK_MIDDLE+1); x++) //Loop through the width of the tank
			draw_block(framePointer, x, y);
    }
	for(x=(TANK_RIGHT_TOP-1); x <= (TANK_RIGHT_TOP+TANK_LEFT_TOP-1); x++) //Loop through the width of the tank
		draw_block(framePointer, x, (TANK_LEFT_TOP-1));
    for(y=TANK_LEFT_TOP; y <= TANK_MIDDLE; y++){ //Loop through the height of the tank
		for(x=TANK_RIGHT_TOP; x <= (TANK_RIGHT_TOP+TANK_LEFT_TOP); x++) //Loop through the width of the tank
			draw_block(framePointer, x, y);
    }
}

//Check if the tank gets hit given the bullet x and y position; Return 0 if not hit, 1 if hit
uint8_t tank_check_if_hit(unsigned int * framePointer, uint16_t bullet_x_pos, uint16_t bullet_y_pos)
{
	if(bullet_x_pos >= tank_x_position && bullet_x_pos <= (tank_x_position + TANK_PIX_WIDTH)){ //Check if bullet is within x boundries
		if(bullet_y_pos >= GLOBALS_TANK_Y_POS && bullet_y_pos <= (GLOBALS_TANK_Y_POS + TANK_PIX_HEIGHT)){ //Check if bullet is within y boundries
			stats_erase_lives_tank(framePointer, lives_remaining); //Erase a tank life
			lives_remaining--; //Decrement lives counter
			tank_erase(framePointer); //Erase the tank
			return 1;
		}
		else
			return 0;
	}
	else
		return 0;
}

//This function will draw a green tank given the framePointer
void tank_dying1_draw(unsigned int * framePointer)
{
    uint16_t x, y; //Iterators
    for(y=0; y < TANK_BITS_Y; y++){ //Loop through the height of the tank
		for(x=0; x < TANK_BITS_X; x++){ //Loop through the width of the tank
			 if(tank_get_tank_dying1_bits(y) & (1<<x)){ //If the tank pixel is a 1 at this position, draw a 4 pixel green box
				 framePointer[(GLOBALS_TANK_Y_POS+(GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + tank_x_position + (GLOBALS_DOUBLE*x)] = GLOBALS_GREEN;
				 framePointer[(GLOBALS_TANK_Y_POS+(GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + tank_x_position + (GLOBALS_DOUBLE*x)] = GLOBALS_GREEN;
				 framePointer[(GLOBALS_TANK_Y_POS+(GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + tank_x_position + (GLOBALS_DOUBLE*x)+1] = GLOBALS_GREEN;
				 framePointer[(GLOBALS_TANK_Y_POS+(GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + tank_x_position + (GLOBALS_DOUBLE*x)+1] = GLOBALS_GREEN;
			 }
		}
    }
}

//This function will draw a green tank given the framePointer
void tank_dying2_draw(unsigned int * framePointer)
{
    uint16_t x, y; //Iterators
    for(y=0; y < TANK_BITS_Y; y++){ //Loop through the height of the tank
		for(x=0; x < TANK_BITS_X; x++){ //Loop through the width of the tank
			 if(tank_get_tank_dying2_bits(y) & (1<<x)){ //If the tank pixel is a 1 at this position, draw a 4 pixel green box
				 framePointer[(GLOBALS_TANK_Y_POS+(GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + tank_x_position + (GLOBALS_DOUBLE*x)] = GLOBALS_GREEN;
				 framePointer[(GLOBALS_TANK_Y_POS+(GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + tank_x_position + (GLOBALS_DOUBLE*x)] = GLOBALS_GREEN;
				 framePointer[(GLOBALS_TANK_Y_POS+(GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + tank_x_position + (GLOBALS_DOUBLE*x)+1] = GLOBALS_GREEN;
				 framePointer[(GLOBALS_TANK_Y_POS+(GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + tank_x_position + (GLOBALS_DOUBLE*x)+1] = GLOBALS_GREEN;
			 }
		}
    }
}

//This function will erase the green tank given the framePointer
void tank_dying1_erase(unsigned int * framePointer)
{
    uint16_t x, y; //Iterators
    for(y=0; y < TANK_BITS_Y; y++){ //Loop through the height of the tank
		for(x=0; x < TANK_BITS_X; x++){ //Loop through the width of the tank
			 if(tank_get_tank_dying1_bits(y) & (1<<x)){ //If the tank pixel is a 1 at this position, draw a 4 pixel black box
				 framePointer[(GLOBALS_TANK_Y_POS+(GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + tank_x_position + (GLOBALS_DOUBLE*x)] = GLOBALS_BLACK;
				 framePointer[(GLOBALS_TANK_Y_POS+(GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + tank_x_position + (GLOBALS_DOUBLE*x)] = GLOBALS_BLACK;
				 framePointer[(GLOBALS_TANK_Y_POS+(GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + tank_x_position + (GLOBALS_DOUBLE*x)+1] = GLOBALS_BLACK;
				 framePointer[(GLOBALS_TANK_Y_POS+(GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + tank_x_position + (GLOBALS_DOUBLE*x)+1] = GLOBALS_BLACK;
			 }
		}
    }
}

//This function will erase the green tank given the framePointer
void tank_dying2_erase(unsigned int * framePointer)
{
    uint16_t x, y; //Iterators
    for(y=0; y < TANK_BITS_Y; y++){ //Loop through the height of the tank
		for(x=0; x < TANK_BITS_X; x++){ //Loop through the width of the tank
			 if(tank_get_tank_dying2_bits(y) & (1<<x)){ //If the tank pixel is a 1 at this position, draw a 4 pixel black box
				 framePointer[(GLOBALS_TANK_Y_POS+(GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + tank_x_position + (GLOBALS_DOUBLE*x)] = GLOBALS_BLACK;
				 framePointer[(GLOBALS_TANK_Y_POS+(GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + tank_x_position + (GLOBALS_DOUBLE*x)] = GLOBALS_BLACK;
				 framePointer[(GLOBALS_TANK_Y_POS+(GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + tank_x_position + (GLOBALS_DOUBLE*x)+1] = GLOBALS_BLACK;
				 framePointer[(GLOBALS_TANK_Y_POS+(GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + tank_x_position + (GLOBALS_DOUBLE*x)+1] = GLOBALS_BLACK;
			 }
		}
    }
}

//This function will draw the green line beneath the tank, takes the framePointer
void tank_draw_bottom_line(unsigned int * framePointer)
{
	uint16_t i; //Iterator
	for(i=0; i < GLOBALS_MAX_X; i++){ //Loop through the width of the screen, and set 2 pixels to green
		framePointer[GLOBALS_BOTTOM_LINE_Y_POS*GLOBALS_MAX_X + i] = GLOBALS_GREEN;
		framePointer[(GLOBALS_BOTTOM_LINE_Y_POS+1)*GLOBALS_MAX_X + i] = GLOBALS_GREEN;
	}
}
=======
#include "tank.h"
#include "stats.h"
#include "globals.h"

#define TANK_BITS_X 15
#define TANK_BITS_Y 8
#define TANK_MIDDLE 7
#define TANK_LEFT_TOP 4
#define TANK_LEFT_TWO 2
#define TANK_RIGHT_TOP 10
#define TANK_PIX_WIDTH 30
#define TANK_PIX_HEIGHT 16

// Packs each horizontal line of the figures into a single 15 bit word.
#define packword15(b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0) \
((b14 << 14) | (b13 << 13) | (b12 << 12) | (b11 << 11) | (b10 << 10) | (b9  << 9 ) | (b8  << 8 ) |	\
 (b7  << 7 ) | (b6  << 6 ) | (b5  << 5 ) | (b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ))

static const uint16_t tank_15x8[] = // This is the pixel pattern for the tank
{
	packword15(0,0,0,0,0,0,0,1,0,0,0,0,0,0,0),
	packword15(0,0,0,0,0,0,1,1,1,0,0,0,0,0,0),
	packword15(0,0,0,0,0,0,1,1,1,0,0,0,0,0,0),
	packword15(0,1,1,1,1,1,1,1,1,1,1,1,1,1,0),
	packword15(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
	packword15(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
	packword15(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
	packword15(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1)
};

static const uint16_t tank_dying1_15x8[] = // This is the pixel pattern for the tank
{
	packword15(0,1,0,0,0,0,0,1,0,0,0,0,0,0,0),
	packword15(0,0,1,0,0,0,0,0,0,1,0,0,0,0,0),
	packword15(0,0,0,0,1,0,1,1,0,0,1,0,1,0,0),
	packword15(0,1,0,1,0,0,0,1,1,0,0,1,0,1,0),
	packword15(0,0,0,0,1,0,1,1,0,1,0,1,0,0,0),
	packword15(0,0,0,1,0,1,0,1,1,1,1,0,1,0,0),
	packword15(0,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
	packword15(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1)
};

static const uint16_t tank_dying2_15x8[] = // This is the pixel pattern for the tank
{
	packword15(1,0,1,0,1,0,0,0,0,0,0,0,1,0,0),
	packword15(0,0,0,0,0,1,0,0,1,0,1,0,0,0,0),
	packword15(0,1,0,0,0,0,0,0,0,0,0,0,1,0,1),
	packword15(0,0,0,1,1,1,1,0,0,0,1,0,1,0,0),
	packword15(0,0,1,1,1,1,1,1,0,0,0,1,1,1,0),
	packword15(1,1,1,1,1,1,1,1,1,0,1,1,1,1,1),
	packword15(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
	packword15(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1)
};

static int16_t tank_x_position = GLOBALS_MAX_X/GLOBALS_TWO; // Top left corner of the tank
int8_t lives_remaining = 3;

uint8_t tank_get_lives_remaining(){
	return lives_remaining;
}

// Returns the value of the tank at a certain row
uint16_t tank_get_tank_bits(uint16_t row){
	return tank_15x8[row];
}

uint16_t tank_get_tank_dying2_bits(uint16_t row){
	return tank_dying2_15x8[row];
}

uint16_t tank_get_tank_dying1_bits(uint16_t row){
	return tank_dying1_15x8[row];
}

// Getter function for the global x position
uint16_t tank_get_tank_x_position(){
	return tank_x_position;
}

//Function that will erase a block of 4 pixels
static void erase_block(unsigned int * framePointer, uint16_t x, uint16_t y)
{
	 framePointer[(GLOBALS_TANK_Y_POS+(GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + tank_x_position + (GLOBALS_DOUBLE*x)] = GLOBALS_BLACK;
	 framePointer[(GLOBALS_TANK_Y_POS+(GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + tank_x_position + (GLOBALS_DOUBLE*x)] = GLOBALS_BLACK;
	 framePointer[(GLOBALS_TANK_Y_POS+(GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + tank_x_position + (GLOBALS_DOUBLE*x)+1] = GLOBALS_BLACK;
	 framePointer[(GLOBALS_TANK_Y_POS+(GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + tank_x_position + (GLOBALS_DOUBLE*x)+1] = GLOBALS_BLACK;
}

//Function that will draw a block of 4 pixels
static void draw_block(unsigned int * framePointer, uint16_t x, uint16_t y)
{
	 framePointer[(GLOBALS_TANK_Y_POS+(GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + tank_x_position + (GLOBALS_DOUBLE*x)] = GLOBALS_GREEN;
	 framePointer[(GLOBALS_TANK_Y_POS+(GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + tank_x_position + (GLOBALS_DOUBLE*x)] = GLOBALS_GREEN;
	 framePointer[(GLOBALS_TANK_Y_POS+(GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + tank_x_position + (GLOBALS_DOUBLE*x)+1] = GLOBALS_GREEN;
	 framePointer[(GLOBALS_TANK_Y_POS+(GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + tank_x_position + (GLOBALS_DOUBLE*x)+1] = GLOBALS_GREEN;
}

//This function will draw a green tank given the framePointer
void tank_draw(unsigned int * framePointer)
{
    uint16_t x, y; //Iterators
    for(y=0; y < TANK_BITS_Y; y++){ //Loop through the height of the tank
		for(x=0; x < TANK_BITS_X; x++){ //Loop through the width of the tank
			 if(tank_get_tank_bits(y) & (1<<x)){ //If the tank pixel is a 1 at this position, draw a 4 pixel green box
				 framePointer[(GLOBALS_TANK_Y_POS+(GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + tank_x_position + (GLOBALS_DOUBLE*x)] = GLOBALS_GREEN;
				 framePointer[(GLOBALS_TANK_Y_POS+(GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + tank_x_position + (GLOBALS_DOUBLE*x)] = GLOBALS_GREEN;
				 framePointer[(GLOBALS_TANK_Y_POS+(GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + tank_x_position + (GLOBALS_DOUBLE*x)+1] = GLOBALS_GREEN;
				 framePointer[(GLOBALS_TANK_Y_POS+(GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + tank_x_position + (GLOBALS_DOUBLE*x)+1] = GLOBALS_GREEN;
			 }
		}
    }
}

//This function will erase the green tank given the framePointer
static void tank_erase(unsigned int * framePointer)
{
    uint16_t x, y; //Iterators
    for(y=0; y < TANK_BITS_Y; y++){ //Loop through the height of the tank
		for(x=0; x < TANK_BITS_X; x++){ //Loop through the width of the tank
			 if(tank_get_tank_bits(y) & (1<<x)){ //If the tank pixel is a 1 at this position, draw a 4 pixel black box
				 framePointer[(GLOBALS_TANK_Y_POS+(GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + tank_x_position + (GLOBALS_DOUBLE*x)] = GLOBALS_BLACK;
				 framePointer[(GLOBALS_TANK_Y_POS+(GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + tank_x_position + (GLOBALS_DOUBLE*x)] = GLOBALS_BLACK;
				 framePointer[(GLOBALS_TANK_Y_POS+(GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + tank_x_position + (GLOBALS_DOUBLE*x)+1] = GLOBALS_BLACK;
				 framePointer[(GLOBALS_TANK_Y_POS+(GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + tank_x_position + (GLOBALS_DOUBLE*x)+1] = GLOBALS_BLACK;
			 }
		}
    }
}

//This function will shift the tank left a frame, takes the framePointer
void tank_shift_left(unsigned int * framePointer)
{
	if (tank_x_position-GLOBALS_SHIFT_VAL < GLOBALS_MIN_X) //Check if tank is going to shift off the left
		return;

	//hard code tank shift
    uint16_t x, y; //Iterators
	erase_block(framePointer, TANK_MIDDLE, 0);
    for(y=1; y <= TANK_LEFT_TWO; y++){ //Loop through the height of the tank
		for(x=(TANK_MIDDLE-1); x <= (TANK_MIDDLE+1); x++)//Loop through the width of the tank
			erase_block(framePointer, x, y);
    }
	for(x=(TANK_RIGHT_TOP-1); x <= (TANK_RIGHT_TOP+TANK_LEFT_TOP-1); x++) //Loop through the width of the tank
		erase_block(framePointer, x, (TANK_LEFT_TOP-1));
    for(y=TANK_LEFT_TOP; y <= TANK_MIDDLE; y++){ //Loop through the height of the tank
		for(x=TANK_RIGHT_TOP; x <= (TANK_RIGHT_TOP+TANK_LEFT_TOP); x++) //Loop through the width of the tank
			erase_block(framePointer, x, y);
    }

	tank_x_position = tank_x_position-GLOBALS_SHIFT_VAL; //Shift the pointer position left
	if (tank_x_position <= GLOBALS_MIN_X) //Make sure we don't pass the edge of the screen
		tank_x_position = GLOBALS_MIN_X;

	draw_block(framePointer, TANK_MIDDLE, 0);
    for(y=1; y <= TANK_LEFT_TWO; y++){ //Loop through the height of the tank
		for(x=(TANK_MIDDLE-1); x <= (TANK_MIDDLE+1); x++) //Loop through the width of the tank
			draw_block(framePointer, x, y);
    }
	for(x=1; x <= (TANK_LEFT_TOP+1); x++) //Loop through the width of the tank
		draw_block(framePointer, x, (TANK_LEFT_TOP-1));
    for(y=TANK_LEFT_TOP; y <= TANK_MIDDLE; y++){ //Loop through the height of the tank
		for(x=0; x <= TANK_LEFT_TOP; x++) //Loop through the width of the tank
			draw_block(framePointer, x, y);
    }
}

//This function will shift the tank right a frame, takes the framePointer
void tank_shift_right(unsigned int * framePointer)
{
	if (tank_x_position+GLOBALS_SHIFT_VAL > GLOBALS_MAX_X-GLOBALS_TANK_PIX_WIDTH) //check if tank will shift off the right side
		return;
	//hard code tank shift
    uint16_t x, y; //Iterators
	erase_block(framePointer, TANK_MIDDLE, 0);
    for(y=1; y <= TANK_LEFT_TWO; y++){ //Loop through the height of the tank
		for(x=(TANK_MIDDLE-1); x <= (TANK_MIDDLE+1); x++) //Loop through the width of the tank
			erase_block(framePointer, x, y);
    }
	for(x=1; x <= (TANK_LEFT_TOP+1); x++) //Loop through the width of the tank
		erase_block(framePointer, x, (TANK_LEFT_TOP-1));
    for(y=TANK_LEFT_TOP; y <= TANK_MIDDLE; y++){ //Loop through the height of the tank
		for(x=0; x <= TANK_LEFT_TOP; x++) //Loop through the width of the tank
			erase_block(framePointer, x, y);
    }

	tank_x_position = tank_x_position+GLOBALS_SHIFT_VAL; //Shift the pointer position left
	if (tank_x_position >= GLOBALS_MAX_X-GLOBALS_TANK_PIX_WIDTH) //Make sure we don't pass the edge of the screen
		tank_x_position = GLOBALS_MAX_X-GLOBALS_TANK_PIX_WIDTH;

	draw_block(framePointer, TANK_MIDDLE, 0);
    for(y=1; y <= TANK_LEFT_TWO; y++){ //Loop through the height of the tank
		for(x=(TANK_MIDDLE-1); x <= (TANK_MIDDLE+1); x++) //Loop through the width of the tank
			draw_block(framePointer, x, y);
    }
	for(x=(TANK_RIGHT_TOP-1); x <= (TANK_RIGHT_TOP+TANK_LEFT_TOP-1); x++) //Loop through the width of the tank
		draw_block(framePointer, x, (TANK_LEFT_TOP-1));
    for(y=TANK_LEFT_TOP; y <= TANK_MIDDLE; y++){ //Loop through the height of the tank
		for(x=TANK_RIGHT_TOP; x <= (TANK_RIGHT_TOP+TANK_LEFT_TOP); x++) //Loop through the width of the tank
			draw_block(framePointer, x, y);
    }
}

//Check if the tank gets hit given the bullet x and y position; Return 0 if not hit, 1 if hit
uint8_t tank_check_if_hit(unsigned int * framePointer, uint16_t bullet_x_pos, uint16_t bullet_y_pos)
{
	if(bullet_x_pos >= tank_x_position && bullet_x_pos <= (tank_x_position + TANK_PIX_WIDTH)){ //Check if bullet is within x boundries
		if(bullet_y_pos >= GLOBALS_TANK_Y_POS && bullet_y_pos <= (GLOBALS_TANK_Y_POS + TANK_PIX_HEIGHT)){ //Check if bullet is within y boundries
			stats_erase_lives_tank(framePointer, lives_remaining); //Erase a tank life
			lives_remaining--; //Decrement lives counter
			tank_erase(framePointer); //Erase the tank
			return 1;
		}
		else
			return 0;
	}
	else
		return 0;
}

//This function will draw a green tank given the framePointer
void tank_dying1_draw(unsigned int * framePointer)
{
    uint16_t x, y; //Iterators
    for(y=0; y < TANK_BITS_Y; y++){ //Loop through the height of the tank
		for(x=0; x < TANK_BITS_X; x++){ //Loop through the width of the tank
			 if(tank_get_tank_dying1_bits(y) & (1<<x)){ //If the tank pixel is a 1 at this position, draw a 4 pixel green box
				 framePointer[(GLOBALS_TANK_Y_POS+(GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + tank_x_position + (GLOBALS_DOUBLE*x)] = GLOBALS_GREEN;
				 framePointer[(GLOBALS_TANK_Y_POS+(GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + tank_x_position + (GLOBALS_DOUBLE*x)] = GLOBALS_GREEN;
				 framePointer[(GLOBALS_TANK_Y_POS+(GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + tank_x_position + (GLOBALS_DOUBLE*x)+1] = GLOBALS_GREEN;
				 framePointer[(GLOBALS_TANK_Y_POS+(GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + tank_x_position + (GLOBALS_DOUBLE*x)+1] = GLOBALS_GREEN;
			 }
		}
    }
}

//This function will draw a green tank given the framePointer
void tank_dying2_draw(unsigned int * framePointer)
{
    uint16_t x, y; //Iterators
    for(y=0; y < TANK_BITS_Y; y++){ //Loop through the height of the tank
		for(x=0; x < TANK_BITS_X; x++){ //Loop through the width of the tank
			 if(tank_get_tank_dying2_bits(y) & (1<<x)){ //If the tank pixel is a 1 at this position, draw a 4 pixel green box
				 framePointer[(GLOBALS_TANK_Y_POS+(GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + tank_x_position + (GLOBALS_DOUBLE*x)] = GLOBALS_GREEN;
				 framePointer[(GLOBALS_TANK_Y_POS+(GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + tank_x_position + (GLOBALS_DOUBLE*x)] = GLOBALS_GREEN;
				 framePointer[(GLOBALS_TANK_Y_POS+(GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + tank_x_position + (GLOBALS_DOUBLE*x)+1] = GLOBALS_GREEN;
				 framePointer[(GLOBALS_TANK_Y_POS+(GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + tank_x_position + (GLOBALS_DOUBLE*x)+1] = GLOBALS_GREEN;
			 }
		}
    }
}

//This function will erase the green tank given the framePointer
void tank_dying1_erase(unsigned int * framePointer)
{
    uint16_t x, y; //Iterators
    for(y=0; y < TANK_BITS_Y; y++){ //Loop through the height of the tank
		for(x=0; x < TANK_BITS_X; x++){ //Loop through the width of the tank
			 if(tank_get_tank_dying1_bits(y) & (1<<x)){ //If the tank pixel is a 1 at this position, draw a 4 pixel black box
				 framePointer[(GLOBALS_TANK_Y_POS+(GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + tank_x_position + (GLOBALS_DOUBLE*x)] = GLOBALS_BLACK;
				 framePointer[(GLOBALS_TANK_Y_POS+(GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + tank_x_position + (GLOBALS_DOUBLE*x)] = GLOBALS_BLACK;
				 framePointer[(GLOBALS_TANK_Y_POS+(GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + tank_x_position + (GLOBALS_DOUBLE*x)+1] = GLOBALS_BLACK;
				 framePointer[(GLOBALS_TANK_Y_POS+(GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + tank_x_position + (GLOBALS_DOUBLE*x)+1] = GLOBALS_BLACK;
			 }
		}
    }
}

//This function will erase the green tank given the framePointer
void tank_dying2_erase(unsigned int * framePointer)
{
    uint16_t x, y; //Iterators
    for(y=0; y < TANK_BITS_Y; y++){ //Loop through the height of the tank
		for(x=0; x < TANK_BITS_X; x++){ //Loop through the width of the tank
			 if(tank_get_tank_dying2_bits(y) & (1<<x)){ //If the tank pixel is a 1 at this position, draw a 4 pixel black box
				 framePointer[(GLOBALS_TANK_Y_POS+(GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + tank_x_position + (GLOBALS_DOUBLE*x)] = GLOBALS_BLACK;
				 framePointer[(GLOBALS_TANK_Y_POS+(GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + tank_x_position + (GLOBALS_DOUBLE*x)] = GLOBALS_BLACK;
				 framePointer[(GLOBALS_TANK_Y_POS+(GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + tank_x_position + (GLOBALS_DOUBLE*x)+1] = GLOBALS_BLACK;
				 framePointer[(GLOBALS_TANK_Y_POS+(GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + tank_x_position + (GLOBALS_DOUBLE*x)+1] = GLOBALS_BLACK;
			 }
		}
    }
}

//This function will draw the green line beneath the tank, takes the framePointer
void tank_draw_bottom_line(unsigned int * framePointer)
{
	uint16_t i; //Iterator
	for(i=0; i < GLOBALS_MAX_X; i++){ //Loop through the width of the screen, and set 2 pixels to green
		framePointer[GLOBALS_BOTTOM_LINE_Y_POS*GLOBALS_MAX_X + i] = GLOBALS_GREEN;
		framePointer[(GLOBALS_BOTTOM_LINE_Y_POS+1)*GLOBALS_MAX_X + i] = GLOBALS_GREEN;
	}
}
>>>>>>> 46133ae2d6a022a717e89ee90e8959352f12048c
