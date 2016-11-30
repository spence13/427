#include "aliens.h"
#include "globals.h"
#include "stats.h"
#include "bunker.h"
#include "tank.h"
#include <stdio.h>
#include <stdlib.h>

#define ALIENS_ROWS 5 // The number of rows of aliens
#define ALIENS_COLS 11 // The number of columns of aliens
#define ALIENS_BITS_X 12 // The number of pixels wide the aliens are
#define ALIENS_BITS_Y 8 // The number of pixels high the aliens are
#define ALIENS_SPACER_X 5 // The number of pixels wide the space between aliens are
#define ALIENS_SPACER_Y 10 // The number of pixels high the space between aliens are
#define ALIENS_FRAME_SIZE_X (11*12)*2 + 10*ALIENS_SPACER_X // Length of the frame for the aliens
#define ALIENS_FRAME_SIZE_Y (5*8)*2 + 4*ALIENS_SPACER_Y // Height of the frame for the aliens
#define ALIENS_X_MOVEMENT ALIENS_SPACER_X // Number of pixels to move aliens in the x direction
#define ALIENS_Y_MOVEMENT 12 // Number of pixels to move aliens in the y direction
#define ALIENS_MAX_NUM 54 // Total number of aliens
#define ALIENS_START_Y 72 // Number of pixels from the top to start the alien frame
#define ALIENS_MAX_NUM_BULLETS 4 //Maximum number of bullets that the aliens can shoot at a time
#define ALIENS_BULLET_HEIGHT 5 // Number of pixels high the alien bullet is
#define ALIENS_BULLET_WIDTH 3 //Number of pixels wide each bullet is
#define ALIENS_BULLET_Y_MOVEMENT 4 //Number of pixels the bullets will shift
#define ALIENS_BULLET_DISAPPEAR 45 // Number of pixels from the bottom for bullet to disappear
#define ALIENS_BULLET_STATUS 0 //Index of the array in the struct that keeps track of bullet status
#define ALIENS_BULLET_X 1 //Index of the array in the struct that keeps track of bullet x position
#define ALIENS_BULLET_Y 2 //Index of the array in the struct that keeps track of bullet y position
#define ALIENS_BULLET_TYPE 3 //Index of the array in the struct the keeps track of bullet type
#define ALIENS_BULLET_NOT_FOUND -1 //Value that represents a bullet being off screen
#define ALIENS_LEFT_COL 0x400 //100 0000 0000
#define ALIENS_FIRST 0 //First row of aliens
#define ALIENS_SECOND 1 //Second row of aliens
#define ALIENS_THIRD 2 //Third row of aliens
#define ALIENS_FOURTH 3 //Fourth row of aliens
#define ALIENS_FIFTH 4 //Fifth row of aliens
#define ALIENS_TOP_SCORE 40 //Score for top row of aliens
#define ALIENS_MIDDLE_SCORE 20 //Score for middle 2 rows of aliens
#define ALIENS_BOTTOM_SCORE 10 //Score for bottom 2 rows of aliens
#define ALIENS_EXPLOSION_SIZE_Y 10 //Number of pixels high the alien explosion is
#define ALIENS_EXPLOSION_SIZE_X 12 //Number of pixels wide the alien explosion is
#define ALIENS_BULLET0 0
#define ALIENS_BULLET1 1
#define ALIENS_BULLET2 2
#define ALIENS_BULLET3 3

// Packs each horizontal line of the figures into a single 12 bit word.
#define packword12(b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0) \
 ((b11 << 11) | (b10 << 10) | (b9  << 9 ) | (b8  << 8 ) | (b7  << 7 ) | (b6  << 6 ) | \
 (b5  << 5 ) | (b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ))

// Packs each horizontal line of the figures into a single 11 bit word.
#define packword11(b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0) \
 ((b10 << 10) | (b9  << 9 ) | (b8  << 8 ) | (b7  << 7 ) | (b6  << 6 ) | \
 (b5  << 5 ) | (b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ))

// Packs each horizontal line of the figures into a single 11 bit word.
#define packword3(b2,b1,b0) \
 ((b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ))

static const uint16_t alien_explosion_12x10[] =
{
	packword12(0,0,0,0,0,0,1,0,0,0,0,0),
	packword12(0,0,0,1,0,0,1,0,0,0,1,0),
	packword12(1,0,0,1,0,0,0,0,0,1,0,0),
	packword12(0,1,0,0,1,0,0,0,1,0,0,0),
	packword12(0,0,0,0,0,0,0,0,0,0,1,1),
	packword12(1,1,0,0,0,0,0,0,0,0,0,0),
	packword12(0,0,0,1,0,0,0,1,0,0,1,0),
	packword12(0,0,1,0,0,0,0,0,1,0,0,1),
	packword12(0,1,0,0,0,1,0,0,1,0,0,0),
	packword12(0,0,0,0,0,1,0,0,0,0,0,0)
};

static const uint16_t alien_top_in_12x8[] =
{
	packword12(0,0,0,0,0,1,1,0,0,0,0,0),
	packword12(0,0,0,0,1,1,1,1,0,0,0,0),
	packword12(0,0,0,1,1,1,1,1,1,0,0,0),
	packword12(0,0,1,1,0,1,1,0,1,1,0,0),
	packword12(0,0,1,1,1,1,1,1,1,1,0,0),
	packword12(0,0,0,1,0,1,1,0,1,0,0,0),
	packword12(0,0,1,0,0,0,0,0,0,1,0,0),
	packword12(0,0,0,1,0,0,0,0,1,0,0,0)
};

static const uint16_t alien_top_out_12x8[] =
{
	packword12(0,0,0,0,0,1,1,0,0,0,0,0),
	packword12(0,0,0,0,1,1,1,1,0,0,0,0),
	packword12(0,0,0,1,1,1,1,1,1,0,0,0),
	packword12(0,0,1,1,0,1,1,0,1,1,0,0),
	packword12(0,0,1,1,1,1,1,1,1,1,0,0),
	packword12(0,0,0,0,1,0,0,1,0,0,0,0),
	packword12(0,0,0,1,0,1,1,0,1,0,0,0),
	packword12(0,0,1,0,1,0,0,1,0,1,0,0)
};

static const uint16_t alien_middle_in_12x8[] =
{
	packword12(0,0,0,1,0,0,0,0,0,1,0,0),
	packword12(0,0,0,0,1,0,0,0,1,0,0,0),
	packword12(0,0,0,1,1,1,1,1,1,1,0,0),
	packword12(0,0,1,1,0,1,1,1,0,1,1,0),
	packword12(0,1,1,1,1,1,1,1,1,1,1,1),
	packword12(0,1,1,1,1,1,1,1,1,1,1,1),
	packword12(0,1,0,1,0,0,0,0,0,1,0,1),
	packword12(0,0,0,0,1,1,0,1,1,0,0,0)
};

static const uint16_t alien_middle_out_12x8[] =
{
	packword12(0,0,0,1,0,0,0,0,0,1,0,0),
	packword12(0,1,0,0,1,0,0,0,1,0,0,1),
	packword12(0,1,0,1,1,1,1,1,1,1,0,1),
	packword12(0,1,1,1,0,1,1,1,0,1,1,1),
	packword12(0,1,1,1,1,1,1,1,1,1,1,1),
	packword12(0,0,1,1,1,1,1,1,1,1,1,0),
	packword12(0,0,0,1,0,0,0,0,0,1,0,0),
	packword12(0,0,1,0,0,0,0,0,0,0,1,0)
};

static const uint16_t alien_bottom_in_12x8[] =
{
	packword12(0,0,0,0,1,1,1,1,0,0,0,0),
	packword12(0,1,1,1,1,1,1,1,1,1,1,0),
	packword12(1,1,1,1,1,1,1,1,1,1,1,1),
	packword12(1,1,1,0,0,1,1,0,0,1,1,1),
	packword12(1,1,1,1,1,1,1,1,1,1,1,1),
	packword12(0,0,1,1,1,0,0,1,1,1,0,0),
	packword12(0,1,1,0,0,1,1,0,0,1,1,0),
	packword12(0,0,1,1,0,0,0,0,1,1,0,0)
};

static const uint16_t alien_bottom_out_12x8[] =
{
	packword12(0,0,0,0,1,1,1,1,0,0,0,0),
	packword12(0,1,1,1,1,1,1,1,1,1,1,0),
	packword12(1,1,1,1,1,1,1,1,1,1,1,1),
	packword12(1,1,1,0,0,1,1,0,0,1,1,1),
	packword12(1,1,1,1,1,1,1,1,1,1,1,1),
	packword12(0,0,0,1,1,0,0,1,1,0,0,0),
	packword12(0,0,1,1,0,1,1,0,1,1,0,0),
	packword12(1,1,0,0,0,0,0,0,0,0,1,1)
};

static uint8_t alien_bullet_arrow_up[] =
{
	packword3(0,1,0),
	packword3(0,1,0),
	packword3(0,1,0),
	packword3(1,1,1),
	packword3(0,1,0)
};

static uint8_t alien_bullet_arrow_down[] =
{
	packword3(0,1,0),
	packword3(1,1,1),
	packword3(0,1,0),
	packword3(0,1,0),
	packword3(0,1,0)
};

static uint8_t alien_bullet_zigzag_left[] =
{
	packword3(1,0,0),
	packword3(0,1,0),
	packword3(0,0,1),
	packword3(0,1,0),
	packword3(1,0,0)
};

static uint8_t alien_bullet_zigzag_right[] =
{
	packword3(0,0,1),
	packword3(0,1,0),
	packword3(1,0,0),
	packword3(0,1,0),
	packword3(0,0,1)
};

static uint16_t aliens_alive[] = // This global will keep track of which aliens are alive. 1 = alive, 0 = dead
{
	packword11(1,1,1,1,1,1,1,1,1,1,1),
	packword11(1,1,1,1,1,1,1,1,1,1,1),
	packword11(1,1,1,1,1,1,1,1,1,1,1),
	packword11(1,1,1,1,1,1,1,1,1,1,1),
	packword11(1,1,1,1,1,1,1,1,1,1,1)
};


static int16_t dead_alien_space_left = 0; //Number of pixels that have been killed off the left side of frame
static int16_t dead_alien_space_right = 0; //Number of pixels that have been killed off the right side of the frame
static int16_t dead_alien_space_bottom = 0; //Number of pixles that have been killed off the bottom of the frame
static int16_t alien_explosion_x = -1; //Keeps track of the x pos of the last alien explosion
static int16_t alien_explosion_y = -1; //Keeps track of the y pos of the last alien explosion
static int16_t alien_frame_pos_x = GLOBALS_MAX_X/GLOBALS_TWO - ALIENS_FRAME_SIZE_X/GLOBALS_TWO; // Current x position of alien frame
static int16_t alien_frame_pos_y = ALIENS_START_Y; // Current y position of alien frame
static uint8_t alien_frame_direction = 0; // Right when 0, Left when 1
static uint8_t aliens_legs = 0; // 0 when legs closed, 1 when legs opened
static uint16_t alien_bullets[ALIENS_MAX_NUM_BULLETS][ALIENS_MAX_NUM_BULLETS] = {
// 4 arrays, one for each bullet. 4 numbers in each array,
		//index 0 is the status (0 off - 1 on),
		//index 1 is the x position,
		//index 2 is the y position
		//index 3 is the current bullet type (0 for arrow up, 1 for arrow down, 2 for zigzag left, 3 for zigzag right
		{0,0,0,0},
		{0,0,0,0},
		{0,0,0,0},
		{0,0,0,0}
};


int8_t aliens_hit_bottom(unsigned int * framepointer) //check to see if the alien frame has hit the tank
{
	uint8_t row1_flag = 0;
	uint8_t row2_flag = 0;
	uint8_t row3_flag = 0;

	uint16_t bottom_pos = ((alien_frame_pos_y) + (ALIENS_FRAME_SIZE_Y) - (dead_alien_space_bottom));//save the bottom position of alien block
	if(((bottom_pos) > (GLOBALS_ERODE_BLOCK_Y_POS_1)) && ((bottom_pos) <= (GLOBALS_ERODE_BLOCK_Y_POS_2)) && (row1_flag == 0)){
		row1_flag = 1;
		bunker_set_erosion_state(0,0,5);//bunker_erode(framepointer, bunker_number, erosion_block);//eliminate top row of erosion sections
		bunker_set_erosion_state(0,1,5);//eliminate top row of erosion sections
		bunker_set_erosion_state(0,2,5);//eliminate top row of erosion sections
		bunker_set_erosion_state(0,3,5);//eliminate top row of erosion sections
		bunker_set_erosion_state(1,0,5);//eliminate top row of erosion sections
		bunker_set_erosion_state(1,1,5);//eliminate top row of erosion sections
		bunker_set_erosion_state(1,2,5);//eliminate top row of erosion sections
		bunker_set_erosion_state(1,3,5);//eliminate top row of erosion sections
		bunker_set_erosion_state(2,0,5);//eliminate top row of erosion sections
		bunker_set_erosion_state(2,1,5);//eliminate top row of erosion sections
		bunker_set_erosion_state(2,2,5);//eliminate top row of erosion sections
		bunker_set_erosion_state(2,3,5);//eliminate top row of erosion sections
		bunker_set_erosion_state(3,0,5);//eliminate top row of erosion sections
		bunker_set_erosion_state(3,1,5);//eliminate top row of erosion sections
		bunker_set_erosion_state(3,2,5);//eliminate top row of erosion sections
		bunker_set_erosion_state(3,3,5);//eliminate top row of erosion sections
	}
	if(((bottom_pos) > (GLOBALS_ERODE_BLOCK_Y_POS_2)) && (bottom_pos <= (GLOBALS_ERODE_BLOCK_Y_POS_3)) && (row2_flag == 0)){
		row2_flag = 1;
		bunker_set_erosion_state(0,4,5);//eliminate middle row of erosion sections
		bunker_set_erosion_state(0,5,5);//eliminate middle row of erosion sections
		bunker_set_erosion_state(0,6,5);//eliminate middle row of erosion sections
		bunker_set_erosion_state(0,7,5);//eliminate middle row of erosion sections
		bunker_set_erosion_state(1,4,5);//eliminate middle row of erosion sections
		bunker_set_erosion_state(1,5,5);//eliminate middle row of erosion sections
		bunker_set_erosion_state(1,6,5);//eliminate middle row of erosion sections
		bunker_set_erosion_state(1,7,5);//eliminate middle row of erosion sections
		bunker_set_erosion_state(2,4,5);//eliminate middle row of erosion sections
		bunker_set_erosion_state(2,5,5);//eliminate middle row of erosion sections
		bunker_set_erosion_state(2,6,5);//eliminate middle row of erosion sections
		bunker_set_erosion_state(2,7,5);//eliminate middle row of erosion sections
		bunker_set_erosion_state(3,4,5);//eliminate middle row of erosion sections
		bunker_set_erosion_state(3,5,5);//eliminate middle row of erosion sections
		bunker_set_erosion_state(3,6,5);//eliminate middle row of erosion sections
		bunker_set_erosion_state(3,7,5);//eliminate middle row of erosion sections
	}
	if(((bottom_pos) > (GLOBALS_ERODE_BLOCK_Y_POS_3)) && ((bottom_pos) <= (GLOBALS_ERODE_BLOCK_Y_POS_4)) && (row3_flag == 0)){
		row3_flag = 1;
		bunker_set_erosion_state(0,8,5);//eliminate bottom row of erosion sections
		bunker_set_erosion_state(0,9,5);//eliminate bottom row of erosion sections
		bunker_set_erosion_state(1,8,5);//eliminate bottom row of erosion sections
		bunker_set_erosion_state(1,9,5);//eliminate bottom row of erosion sections
		bunker_set_erosion_state(2,8,5);//eliminate bottom row of erosion sections
		bunker_set_erosion_state(2,9,5);//eliminate bottom row of erosion sections
		bunker_set_erosion_state(3,8,5);//eliminate bottom row of erosion sections
		bunker_set_erosion_state(3,9,5);//eliminate bottom row of erosion sections
	}
	if ((bottom_pos) >= (GLOBALS_ERODE_BLOCK_Y_POS_4+ALIENS_BITS_Y))//end the game if aliens are below bottom of bunkers
		return 1;
	else
		return 0;
}

int8_t aliens_all_dead() //check to see if all aliens have been killed
{
	if (aliens_alive[ALIENS_FIRST] | aliens_alive[ALIENS_SECOND] | aliens_alive[ALIENS_THIRD] | aliens_alive[ALIENS_FOURTH] | aliens_alive[ALIENS_FIFTH])
		return 0;
	else
		return 1;
}

// Function that will draw one single alien. Parameters are the framePointer, the alien struct, current x position, current y position
static void draw_single_alien(unsigned int * framePointer, const uint16_t * alien_sprite, uint16_t current_pos_x, uint16_t current_pos_y)
{
	uint8_t x, y; // Iterators
	for (y=0; y < ALIENS_BITS_Y; y++){ // Loop through the y coordinates
		for (x=0; x < ALIENS_BITS_X; x++){ // Loop through the x coordinates
			 if(alien_sprite[y] & (1<<x)){ // If the alien struct is 1 in this location, write a box of 4 white pixels
				 framePointer[(current_pos_y + (GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + current_pos_x + (GLOBALS_DOUBLE*x)] = GLOBALS_WHITE;
				 framePointer[(current_pos_y + (GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + current_pos_x + (GLOBALS_DOUBLE*x)+1] = GLOBALS_WHITE;
				 framePointer[(current_pos_y + (GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + current_pos_x + (GLOBALS_DOUBLE*x)] = GLOBALS_WHITE;
				 framePointer[(current_pos_y + (GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + current_pos_x + (GLOBALS_DOUBLE*x)+1] = GLOBALS_WHITE;
			 }
			 else{ // If the alien struct is 1 in this location, write a box of 4 white pixels
				 framePointer[(current_pos_y + (GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + current_pos_x + (GLOBALS_DOUBLE*x)] = GLOBALS_BLACK;
				 framePointer[(current_pos_y + (GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + current_pos_x + (GLOBALS_DOUBLE*x)+1] = GLOBALS_BLACK;
				 framePointer[(current_pos_y + (GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + current_pos_x + (GLOBALS_DOUBLE*x)] = GLOBALS_BLACK;
				 framePointer[(current_pos_y + (GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + current_pos_x + (GLOBALS_DOUBLE*x)+1] = GLOBALS_BLACK;
			 }
		}
	}
}

// Function that will erase one single alien. Parameters are the framePointer, the alien struct, current x position, current y position
static void erase_single_alien(unsigned int * framePointer, const uint16_t * alien_sprite, uint16_t current_pos_x, uint16_t current_pos_y)
{
	uint8_t x, y; // Iterators
	for (y=0; y < ALIENS_BITS_Y; y++){ // Loop through the y coordinates
		for (x=0; x < ALIENS_BITS_X; x++){ // Loop through the x coordinates
			 if(alien_sprite[y] & (1<<x)){ // If the alien struct is 1 in this location, write a box of 4 black pixels
				 framePointer[(current_pos_y + (GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + current_pos_x + (GLOBALS_DOUBLE*x)] = GLOBALS_BLACK;
				 framePointer[(current_pos_y + (GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + current_pos_x + (GLOBALS_DOUBLE*x)+1] = GLOBALS_BLACK;
				 framePointer[(current_pos_y + (GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + current_pos_x + (GLOBALS_DOUBLE*x)] = GLOBALS_BLACK;
				 framePointer[(current_pos_y + (GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + current_pos_x + (GLOBALS_DOUBLE*x)+1] = GLOBALS_BLACK;
			 }
		}
	}
}

//Helper function to erase a certain bullet. Takes framePointer, current x_pos, and y_pos of the bullet
static void erase_alien_bullet(unsigned int * framePointer, uint16_t current_pos_x, uint16_t current_pos_y, uint16_t bullet_type)
{
	//bullet type (0 for arrow up, 1 for arrow down, 2 for zigzag left, 3 for zigzag right
	uint8_t * alien_bullet_sprite;
	switch(bullet_type) //Find the sprite for each alien bullet type
	{
		case 0:
			alien_bullet_sprite = alien_bullet_arrow_up;
			break;
		case 1:
			alien_bullet_sprite = alien_bullet_arrow_down;
			break;
		case 2:
			alien_bullet_sprite = alien_bullet_zigzag_left;
			break;
		case 3:
			alien_bullet_sprite = alien_bullet_zigzag_right;
			break;
		default:
			return;

	}
	uint8_t x, y; // Iterators
	for (y=0; y < ALIENS_BULLET_HEIGHT+1; y++){ // Loop through the y coordinates
		for (x=0; x < ALIENS_BULLET_WIDTH+1; x++){ // Loop through the x coordinates
			 if(alien_bullet_sprite[y] & (1<<x)){ // If the alien struct is 1 in this location, write a box of 4 black pixels
				 framePointer[(current_pos_y + (GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + current_pos_x + (GLOBALS_DOUBLE*x)] = GLOBALS_BLACK;
				 framePointer[(current_pos_y + (GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + current_pos_x + (GLOBALS_DOUBLE*x)+1] = GLOBALS_BLACK;
				 framePointer[(current_pos_y + (GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + current_pos_x + (GLOBALS_DOUBLE*x)] = GLOBALS_BLACK;
				 framePointer[(current_pos_y + (GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + current_pos_x + (GLOBALS_DOUBLE*x)+1] = GLOBALS_BLACK;
			 }
		}
	}
}

//Helper function to draw a certain bullet. Takes framePointer, current x_pos, and y_pos of the bullet
static void draw_alien_bullet(unsigned int * framePointer, uint16_t current_pos_x, uint16_t current_pos_y, uint16_t bullet_type)
{
	//bullet type (0 for arrow up, 1 for arrow down, 2 for zigzag left, 3 for zigzag right
	uint8_t * alien_bullet_sprite;
	switch(bullet_type) //Find the sprite for each alien bullet type
	{
		case 0:
			alien_bullet_sprite = alien_bullet_arrow_up;
			break;
		case 1:
			alien_bullet_sprite = alien_bullet_arrow_down;
			break;
		case 2:
			alien_bullet_sprite = alien_bullet_zigzag_left;
			break;
		case 3:
			alien_bullet_sprite = alien_bullet_zigzag_right;
			break;
		default:
			return;

	}
	uint8_t x, y; // Iterators
	for (y=0; y < ALIENS_BULLET_HEIGHT+1; y++){ // Loop through the y coordinates
		for (x=0; x < ALIENS_BULLET_WIDTH+1; x++){ // Loop through the x coordinates
			 if(alien_bullet_sprite[y] & (1<<x)){ // If the alien struct is 1 in this location, write a box of 4 black pixels
				 framePointer[(current_pos_y + (GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + current_pos_x + (GLOBALS_DOUBLE*x)] = GLOBALS_WHITE;
				 framePointer[(current_pos_y + (GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + current_pos_x + (GLOBALS_DOUBLE*x)+1] = GLOBALS_WHITE;
				 framePointer[(current_pos_y + (GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + current_pos_x + (GLOBALS_DOUBLE*x)] = GLOBALS_WHITE;
				 framePointer[(current_pos_y + (GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + current_pos_x + (GLOBALS_DOUBLE*x)+1] = GLOBALS_WHITE;
			 }
		}
	}
}

static void draw_alien_explosion(unsigned int * framePointer, uint16_t current_pos_x, uint16_t current_pos_y)
{
	uint8_t x, y; // Iterators
	for (y=0; y < ALIENS_EXPLOSION_SIZE_Y; y++){ // Loop through the y coordinates
		for (x=0; x < ALIENS_EXPLOSION_SIZE_X; x++){ // Loop through the x coordinates
			 if(alien_explosion_12x10[y] & (1<<x)){ // If the alien struct is 1 in this location, write a box of 4 white pixels
				 framePointer[(current_pos_y + (GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + current_pos_x + (GLOBALS_DOUBLE*x)] = GLOBALS_WHITE;
				 framePointer[(current_pos_y + (GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + current_pos_x + (GLOBALS_DOUBLE*x)+1] = GLOBALS_WHITE;
				 framePointer[(current_pos_y + (GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + current_pos_x + (GLOBALS_DOUBLE*x)] = GLOBALS_WHITE;
				 framePointer[(current_pos_y + (GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + current_pos_x + (GLOBALS_DOUBLE*x)+1] = GLOBALS_WHITE;
			 }
		}
	}
}

void erase_alien_explosion(unsigned int * framePointer)
{
	if (alien_explosion_x < 0 || alien_explosion_y < 0)
		return;
	uint8_t x, y; // Iterators
	for (y=0; y < ALIENS_EXPLOSION_SIZE_Y; y++){ // Loop through the y coordinates
		for (x=0; x < ALIENS_EXPLOSION_SIZE_X; x++){ // Loop through the x coordinates
			 if(alien_explosion_12x10[y] & (1<<x)){ // If the alien struct is 1 in this location, write a box of 4 black pixels
				 framePointer[(alien_explosion_y + (GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + alien_explosion_x + (GLOBALS_DOUBLE*x)] = GLOBALS_BLACK;
				 framePointer[(alien_explosion_y + (GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + alien_explosion_x + (GLOBALS_DOUBLE*x)+1] = GLOBALS_BLACK;
				 framePointer[(alien_explosion_y + (GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + alien_explosion_x + (GLOBALS_DOUBLE*x)] = GLOBALS_BLACK;
				 framePointer[(alien_explosion_y + (GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + alien_explosion_x + (GLOBALS_DOUBLE*x)+1] = GLOBALS_BLACK;
			 }
		}
	}
	alien_explosion_x = -1; //Reset alien explosion variables
	alien_explosion_y = -1; //Reset alien explosion variables
}

//Helper function to shift alien frame right
static void shift_aliens_right(unsigned int * framePointer)
{
	if (alien_explosion_x >= 0 || alien_explosion_y >= 0) //Check to see if the alien explosion has been erased
		erase_alien_explosion(framePointer);

	if ((alien_frame_pos_x + ALIENS_FRAME_SIZE_X + ALIENS_X_MOVEMENT) > GLOBALS_MAX_X+dead_alien_space_right) //If our frame has reached the right edge
	{
		alien_frame_pos_y += ALIENS_Y_MOVEMENT; //Shift alien frame down
		alien_frame_direction = 1; //Change alien frame direction
	}
	else
		alien_frame_pos_x += ALIENS_X_MOVEMENT; //Shift alien frame right
}

//Helper function to shift alien frame left
static void shift_aliens_left(unsigned int * framePointer)
{
	if (alien_explosion_x >= 0 || alien_explosion_y >= 0) //Check to see if the alien explosion has been erased
		erase_alien_explosion(framePointer);

	if ((alien_frame_pos_x - ALIENS_X_MOVEMENT) < (-1*dead_alien_space_left)) //If our frame has reached the left edge
	{
		alien_frame_pos_y += ALIENS_Y_MOVEMENT; //Shift alien frame down
		alien_frame_direction = 0; //Change alien frame direction
	}
	else
		alien_frame_pos_x -= ALIENS_X_MOVEMENT; //Shift alien frame left
}

//Move the aliens left or right according to their current direction and position
void update_alien_pos(unsigned int * framePointer)
{
	uint16_t original_x_pos = alien_frame_pos_x; //Save original x position for reference
	uint16_t previous_x_pos = alien_frame_pos_x; //Save previous x position for erasing
	uint16_t previous_y_pos = alien_frame_pos_y; //Save previous y position for erasing

	if (alien_frame_direction) //If the alien has been moving left
		shift_aliens_left(framePointer);
	else //If the alien has been moving right
		shift_aliens_right(framePointer);

	uint8_t x, y; //Iterators
	uint16_t current_x_pos = alien_frame_pos_x; //Get current x position after shift
	uint16_t current_y_pos = alien_frame_pos_y; //Get current y position after shift

	for (y=0; y < ALIENS_ROWS; y++){ //Loop through each row of aliens
		previous_x_pos = original_x_pos; //Save previous x position from original
		current_x_pos = alien_frame_pos_x; //Save current x position
		for (x=0; x < ALIENS_COLS; x++){ //Loop through each alien in the rows
			if (aliens_alive[y] & (1<<(ALIENS_COLS-x-1))){ //If the pixel in the alien struct is a 1 at this location
				if (y == 0 && aliens_legs){ //If we're in the first row, and the alien's legs are out
					erase_single_alien(framePointer, alien_top_out_12x8, previous_x_pos, previous_y_pos);
					draw_single_alien(framePointer, alien_top_in_12x8, current_x_pos, current_y_pos);
				}
				else if (y == 0 && !aliens_legs){ //If we're in the first row, and the alien's legs are in
					erase_single_alien(framePointer, alien_top_in_12x8, previous_x_pos, previous_y_pos);
					draw_single_alien(framePointer, alien_top_out_12x8, current_x_pos, current_y_pos);
				}
				else if (y > 0 && y <= GLOBALS_TWO && aliens_legs){ //If we're in the second or third rows, and the alien's legs are out
					erase_single_alien(framePointer, alien_middle_out_12x8, previous_x_pos, previous_y_pos);
					draw_single_alien(framePointer, alien_middle_in_12x8, current_x_pos, current_y_pos);
				}
				else if (y > 0 && y <= GLOBALS_TWO && !aliens_legs){ //If we're in the second or third rows, and the alien's legs are in
					erase_single_alien(framePointer, alien_middle_in_12x8, previous_x_pos, previous_y_pos);
					draw_single_alien(framePointer, alien_middle_out_12x8, current_x_pos, current_y_pos);
				}
				else if (y > GLOBALS_TWO && aliens_legs){ //If we're in the last row, and the alien's legs are out
					erase_single_alien(framePointer, alien_bottom_out_12x8, previous_x_pos, previous_y_pos);
					draw_single_alien(framePointer, alien_bottom_in_12x8, current_x_pos, current_y_pos);
				}
				else if (y > GLOBALS_TWO && !aliens_legs){ //If we're in the last row, and the alien's legs are in
					erase_single_alien(framePointer, alien_bottom_in_12x8, previous_x_pos, previous_y_pos);
					draw_single_alien(framePointer, alien_bottom_out_12x8, current_x_pos, current_y_pos);
				}
			}
			current_x_pos += GLOBALS_TWO*ALIENS_BITS_X + ALIENS_SPACER_X; //Increment the current x position that we're currently at
			previous_x_pos += GLOBALS_TWO*ALIENS_BITS_X + ALIENS_SPACER_X; //Increment the previous x posiiton that we're currently at
		}
		current_y_pos += GLOBALS_TWO*ALIENS_BITS_Y + ALIENS_SPACER_Y; //Increment the current y position that we're currently at
		previous_y_pos += GLOBALS_TWO*ALIENS_BITS_Y + ALIENS_SPACER_Y; //Increment the previous y posiiton that we're currently at
	}
	aliens_legs = (aliens_legs+1)%GLOBALS_TWO; //Toggle alien's leg orientation
}

//Helper function for getting the alien position
static int8_t get_alien_pos(uint16_t bullet_x_pos, uint16_t bullet_y_pos)
{
	//These if statements check if the bullet is within the frame, if not just return
	if (bullet_x_pos > (alien_frame_pos_x + ALIENS_FRAME_SIZE_X - dead_alien_space_right))
		return -1;
	if (bullet_x_pos < (alien_frame_pos_x + dead_alien_space_left))
		return -1;
	if (bullet_y_pos < alien_frame_pos_y)
		return -1;
	if (bullet_y_pos > alien_frame_pos_y + ALIENS_FRAME_SIZE_Y - dead_alien_space_bottom)
		return -1;
	int8_t alien_num = -1;
	uint8_t x, y; //Iterators
	uint16_t previous_x_pos = alien_frame_pos_x; //Save previous x position for erasing
	uint16_t previous_y_pos = alien_frame_pos_y; //Save previous y position for erasing
	uint16_t current_x_pos = alien_frame_pos_x; //Get current x position after shift
	uint16_t current_y_pos = alien_frame_pos_y; //Get current y position after shift

	for (y=0; y < ALIENS_ROWS; y++){ //Loop through each row of aliens
		previous_y_pos = current_y_pos; //Save previous y pos
		current_y_pos += (GLOBALS_TWO*ALIENS_BITS_Y + ALIENS_SPACER_Y);//Increment current y pos
		previous_x_pos = alien_frame_pos_x; //Reset previous x pos
		current_x_pos = alien_frame_pos_x; //Reset current x pos
		for (x=0; x < ALIENS_COLS; x++){ //Loop through each alien in the rows
			previous_x_pos = current_x_pos; //Save previous x pos
			current_x_pos += (GLOBALS_TWO*ALIENS_BITS_X + ALIENS_SPACER_X); //Increment current x pos
			alien_num++; //Increment alien number
			if ((bullet_x_pos >= previous_x_pos)&&(bullet_x_pos <= current_x_pos)&&(bullet_y_pos >= previous_y_pos)&&(bullet_y_pos <= current_y_pos))
			{
				if (aliens_alive[y] & (1<<(ALIENS_COLS-x-1))) //If theres an alien alive at this location
					return alien_num; //return the alien number
				else
					return -1; //alien dead, or not found
			}
		}
	}
	return -1;
}

//Erase the alien at alien_num and mark that alien as dead, takes framePointer and alien number as parameters
uint8_t kill_alien(unsigned int * framePointer, uint16_t bullet_x_pos, uint16_t bullet_y_pos)
{
	int8_t alien_num = get_alien_pos(bullet_x_pos, bullet_y_pos);
	if (alien_num > ALIENS_MAX_NUM || alien_num < 0) // Invalid
		return 0;

	uint16_t row = alien_num/ALIENS_COLS; // Calculate which row the alien is in
	uint16_t col = alien_num%ALIENS_COLS; // Leftovers from the mod will give us which column
	uint16_t alien_x_pos = alien_frame_pos_x + ((GLOBALS_TWO*ALIENS_BITS_X + ALIENS_SPACER_X)*col); //Calculate starting x position of alien to kill
	uint16_t alien_y_pos = alien_frame_pos_y + ((GLOBALS_TWO*ALIENS_BITS_Y + ALIENS_SPACER_Y)*row); //Calculate starting y position of alien to kill

	if (row == 0 && aliens_legs) //If we're in the first row, and the alien's legs are out
		erase_single_alien(framePointer, alien_top_out_12x8, alien_x_pos, alien_y_pos);
	else if (row == 0 && !aliens_legs) //If we're in the first row, and the alien's legs are in
		erase_single_alien(framePointer, alien_top_in_12x8, alien_x_pos, alien_y_pos);
	else if (row > 0 && row <= GLOBALS_TWO && aliens_legs) //If we're in the second or third rows, and the alien's legs are out
		erase_single_alien(framePointer, alien_middle_out_12x8, alien_x_pos, alien_y_pos);
	else if (row > 0 && row <= GLOBALS_TWO && !aliens_legs) //If we're in the second or third rows, and the alien's legs are in
		erase_single_alien(framePointer, alien_middle_in_12x8, alien_x_pos, alien_y_pos);
	else if (row > GLOBALS_TWO && aliens_legs) //If we're in the last row, and the alien's legs are out
		erase_single_alien(framePointer, alien_bottom_out_12x8, alien_x_pos, alien_y_pos);
	else if (row > GLOBALS_TWO && !aliens_legs) //If we're in the last row, and the alien's legs are in
		erase_single_alien(framePointer, alien_bottom_in_12x8, alien_x_pos, alien_y_pos);

	aliens_alive[row] &= ~(1<<(ALIENS_COLS-col-1)); // This zeroes out the bit corresponding to that alien

	//DRAW ALIEN EXPLOSION
	alien_explosion_x = alien_x_pos; //Set x pos of explosion
	alien_explosion_y = alien_y_pos; //Set y pos of explosion
	draw_alien_explosion(framePointer, alien_x_pos, alien_y_pos); //Draw alien explosion
	uint16_t current_score = stats_get_points_score(); //Get current score
	uint8_t alien_score = 0; //Variable that will keep track of how much to increment depending on row
	switch(row)
	{
		case 0:
			alien_score = ALIENS_TOP_SCORE;
			break;
		case 1:
			alien_score = ALIENS_MIDDLE_SCORE;
			break;
		case 2:
			alien_score = ALIENS_MIDDLE_SCORE;
			break;
		case 3:
			alien_score = ALIENS_BOTTOM_SCORE;
			break;
		case 4:
			alien_score = ALIENS_BOTTOM_SCORE;
			break;
		default:
			break;
	}
	stats_set_points_score(current_score+alien_score); //Update new score
	stats_update_score(framePointer); //Draw new score

	//OR all rows to see if a column has been killed off
	uint16_t all_rows = ~(	aliens_alive[ALIENS_FIRST] | aliens_alive[ALIENS_SECOND] |
							aliens_alive[ALIENS_THIRD] | aliens_alive[ALIENS_FOURTH] | aliens_alive[ALIENS_FIFTH]);
	uint16_t dead_pos = ALIENS_LEFT_COL; //Iterator
	uint16_t dead_rows = 0; //Counter to keep track of total dead columns
	//This loop will check the empty alien rows starting on the left
	int8_t i;
	for(i=0; i < ALIENS_COLS; i++)
	{
		if (all_rows & dead_pos){ //Check if all the aliens at this position are dead
			dead_rows += 1; //Increment the number of dead alien rows
			dead_pos >>= 1; //Shift the position of the column right one
		}
		else
			break;
	}
	dead_alien_space_left = dead_rows*(GLOBALS_TWO*ALIENS_BITS_X + ALIENS_SPACER_X); //Set global dead space

	dead_pos = 1; //Set column position to start from right side
	dead_rows = 0; //Reset number of dead rows
	//This loop will check the empty alien rows starting on the right
	for(i=0; i < ALIENS_COLS; i++)
	{
		if (all_rows & dead_pos){ //Check if all the aliens at this position are dead
			dead_rows += 1; //Increment the number of dead alien rows
			dead_pos <<= 1; //Shift the position of the column left one
		}
		else
			break;
	}
	dead_alien_space_right = dead_rows*(GLOBALS_TWO*ALIENS_BITS_X + ALIENS_SPACER_X); //Set global dead space

	dead_rows = 0; //Reset number of dead rows
	for (i=ALIENS_ROWS-1; i >= 0; i--)
	{
		if (aliens_alive[i] == 0) //If all aliens are dead in this row
			dead_rows++;
		else
			break;
	}
	dead_alien_space_bottom = dead_rows*(GLOBALS_TWO*ALIENS_BITS_Y + ALIENS_SPACER_Y); //Set global dead space
	return 1;
}

//This will shift all alien bullets down the display a frame, takes framePointer as the parameter
uint8_t update_alien_bullets(unsigned int * framePointer)
{
	uint8_t i; //Iterator
	uint16_t new_bullet_type = 0;
	for (i=0; i < ALIENS_MAX_NUM_BULLETS; i++) // Loop through all bullet slots
	{
		if (alien_bullets[i][ALIENS_BULLET_STATUS]) // If there is a bullet slot that's active
		{
			erase_alien_bullet(framePointer, alien_bullets[i][ALIENS_BULLET_X], alien_bullets[i][ALIENS_BULLET_Y], alien_bullets[i][ALIENS_BULLET_TYPE]);
			alien_bullets[i][ALIENS_BULLET_Y] += ALIENS_BULLET_Y_MOVEMENT; //Increment bullet y position

			if(bunker_check_if_erosion_necessary(framePointer, alien_bullets[i][ALIENS_BULLET_X], alien_bullets[i][ALIENS_BULLET_Y])){//bullet erases when it hits bunker
				alien_bullets[i][ALIENS_BULLET_STATUS] = 0;
				continue;
			}
			else if(bunker_check_if_erosion_necessary(framePointer, alien_bullets[i][ALIENS_BULLET_X], alien_bullets[i][ALIENS_BULLET_Y]+(GLOBALS_TWO*ALIENS_BULLET_HEIGHT))){//bullet erases when it hits bunker
				alien_bullets[i][ALIENS_BULLET_STATUS] = 0;
				continue;
			}
			else if(bunker_check_if_erosion_necessary(framePointer, alien_bullets[i][ALIENS_BULLET_X]+(GLOBALS_TWO*ALIENS_BULLET_WIDTH), alien_bullets[i][ALIENS_BULLET_Y])){//bullet erases when it hits bunker
				alien_bullets[i][ALIENS_BULLET_STATUS] = 0;
				continue;
			}
			else if(bunker_check_if_erosion_necessary(framePointer, alien_bullets[i][ALIENS_BULLET_X]+(GLOBALS_TWO*ALIENS_BULLET_WIDTH), alien_bullets[i][ALIENS_BULLET_Y]+(GLOBALS_TWO*ALIENS_BULLET_HEIGHT))){//bullet erases when it hits bunker
				alien_bullets[i][ALIENS_BULLET_STATUS] = 0;
				continue;
			}

			if(tank_check_if_hit(framePointer, alien_bullets[i][ALIENS_BULLET_X], alien_bullets[i][ALIENS_BULLET_Y])){//bullet erases when it hits tank
				alien_bullets[i][ALIENS_BULLET_STATUS] = 0;
				return 1;//return a 1 to main.c to know to pause game while you kill the tank
			}
			else if(tank_check_if_hit(framePointer, alien_bullets[i][ALIENS_BULLET_X], alien_bullets[i][ALIENS_BULLET_Y]+(GLOBALS_TWO*ALIENS_BULLET_HEIGHT))){//bullet erases when it hits tank
				alien_bullets[i][ALIENS_BULLET_STATUS] = 0;
				return 1;//return a 1 to main.c to know to pause game while you kill the tank
			}
			else if(tank_check_if_hit(framePointer, alien_bullets[i][ALIENS_BULLET_X]+(GLOBALS_TWO*ALIENS_BULLET_WIDTH), alien_bullets[i][ALIENS_BULLET_Y])){//bullet erases when it hits tank
				alien_bullets[i][ALIENS_BULLET_STATUS] = 0;
				return 1;//return a 1 to main.c to know to pause game while you kill the tank
			}
			else if(tank_check_if_hit(framePointer, alien_bullets[i][ALIENS_BULLET_X]+(GLOBALS_TWO*ALIENS_BULLET_WIDTH), alien_bullets[i][ALIENS_BULLET_Y]+(GLOBALS_TWO*ALIENS_BULLET_HEIGHT))){//bullet erases when it hits tank
				alien_bullets[i][ALIENS_BULLET_STATUS] = 0;
				return 1;//return a 1 to main.c to know to pause game while you kill the tank
			}


			if ((alien_bullets[i][ALIENS_BULLET_Y] + GLOBALS_TWO*ALIENS_BULLET_HEIGHT) >= GLOBALS_MAX_Y - ALIENS_BULLET_DISAPPEAR)//bullet erases when it gets to bottom green line not bottom of screen
			{
				alien_bullets[i][ALIENS_BULLET_STATUS] = 0; //Reset bullet status
				continue;
			}

			switch(alien_bullets[i][ALIENS_BULLET_TYPE]) //Switch through bullet types to switch to next bullet sprite
			{
				case 0:
					new_bullet_type = ALIENS_BULLET1;
					break;
				case 1:
					new_bullet_type = ALIENS_BULLET0;
					break;
				case 2:
					new_bullet_type = ALIENS_BULLET3;
					break;
				case 3:
					new_bullet_type = ALIENS_BULLET2;
					break;
				default:
					continue;
			}
			alien_bullets[i][ALIENS_BULLET_TYPE] = new_bullet_type; //Set new bullet type
			draw_alien_bullet(framePointer, alien_bullets[i][ALIENS_BULLET_X], alien_bullets[i][ALIENS_BULLET_Y], new_bullet_type); //draw bullet
		}
	}
	return 0;
}

//This will select a random alive alien from the bottom row and shoot a bullet from it
void fire_alien_bullet(unsigned int * framePointer)
{
	int8_t bullet = ALIENS_BULLET_NOT_FOUND; //Set to negative so I can check if a valid index has been found
	int8_t i;
	for (i=0; i < ALIENS_MAX_NUM_BULLETS; i++) // Loop through all bullet slots
	{
		if (!alien_bullets[i][ALIENS_BULLET_STATUS]) // If there is a bullet slot that's available
		{
			bullet = i; // Save the bullet slot
			break;
		}
	}
	if (bullet == ALIENS_BULLET_NOT_FOUND) // No available bullets to shoot
		return;

	uint8_t j, index; //Iterator, counter
	uint16_t space=0; //This will keep track of pixels to spawn bullet
	for (j=0; j < ALIENS_COLS; j++)
	{
		index = rand() % ALIENS_COLS;// Get random index of an alien (0-10)
		for (i=ALIENS_MAX_NUM_BULLETS-1; i >= 0; i--){ //Loop through the possible bullets
			if (aliens_alive[i] & (1<<(ALIENS_COLS-index-1))){ //Check if alien is alive at this location
				alien_bullets[bullet][ALIENS_BULLET_STATUS] = 1; //Set bullet to fired
				alien_bullets[bullet][ALIENS_BULLET_X] = alien_frame_pos_x + ((GLOBALS_TWO*ALIENS_BITS_X + ALIENS_SPACER_X)*index + ALIENS_BITS_X); // x position of bullet
				alien_bullets[bullet][ALIENS_BULLET_TYPE] = rand() % ALIENS_MAX_NUM_BULLETS; //Assign bullet a random type

				switch(i) //Switch through the rows
				{
					case 0:
						space = GLOBALS_TWO*ALIENS_BITS_Y;
						break;
					case ALIENS_MAX_NUM_BULLETS-1:
						space = (GLOBALS_TWO*ALIENS_BITS_Y + ALIENS_SPACER_Y)*(i+2);
						break;
					default:
						space = (GLOBALS_TWO*ALIENS_BITS_Y + ALIENS_SPACER_Y)*(i+1);
						break;
				}
				alien_bullets[bullet][ALIENS_BULLET_Y] = alien_frame_pos_y + space; // y position of bullet
				//draw bullet
				draw_alien_bullet(framePointer, alien_bullets[bullet][ALIENS_BULLET_X], alien_bullets[bullet][ALIENS_BULLET_Y], alien_bullets[bullet][ALIENS_BULLET_TYPE]);
				return;
			}
		}
		index = j; //if it doesnt find a good random number, start from the left
		for (i=ALIENS_MAX_NUM_BULLETS-1; i >= 0; i--){ //Loop through the possible bullets
			if (aliens_alive[i] & (1<<(ALIENS_COLS-index-1))){ //Check if alien is alive at this location
				alien_bullets[bullet][ALIENS_BULLET_STATUS] = 1; //Set bullet to fired
				alien_bullets[bullet][ALIENS_BULLET_X] = alien_frame_pos_x + ((GLOBALS_TWO*ALIENS_BITS_X + ALIENS_SPACER_X)*index + ALIENS_BITS_X); // x position of bullet
				alien_bullets[bullet][ALIENS_BULLET_TYPE] = rand() % ALIENS_MAX_NUM_BULLETS; //Assign bullet a random type

				switch(i) //Switch through the rows
				{
					case 0:
						space = GLOBALS_TWO*ALIENS_BITS_Y;
						break;
					case ALIENS_MAX_NUM_BULLETS-1:
						space = (GLOBALS_TWO*ALIENS_BITS_Y + ALIENS_SPACER_Y)*(i+2);
						break;
					default:
						space = (GLOBALS_TWO*ALIENS_BITS_Y + ALIENS_SPACER_Y)*(i+1);
						break;
				}
				alien_bullets[bullet][ALIENS_BULLET_Y] = alien_frame_pos_y + space; // y position of bullet
				//draw bullet
				draw_alien_bullet(framePointer, alien_bullets[bullet][ALIENS_BULLET_X], alien_bullets[bullet][ALIENS_BULLET_Y], alien_bullets[bullet][ALIENS_BULLET_TYPE]);
				return;
			}
		}
	}
}

