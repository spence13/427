<<<<<<< HEAD
#include "aliens.h"
#include "globals.h"
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
#define ALIENS_Y_MOVEMENT 4 // Number of pixels to move aliens in the y direction
#define ALIENS_MAX_NUM 54 // Total number of aliens
#define ALIENS_START_Y 75 // Number of pixels from the top to start the alien frame
#define ALIENS_MAX_NUM_BULLETS 4 //Maximum number of bullets that the aliens can shoot at a time
#define ALIENS_BULLET_HEIGHT 3 // Number of pixels high the alien bullet is
#define ALIENS_BULLET_DISAPPEAR 45 // Number of pixels from the bottom for bullet to disappear
#define ALIENS_BULLET_STATUS 0 //Index of the array in the struct that keeps track of bullet status
#define ALIENS_BULLET_X 1 //Index of the array in the struct that keeps track of bullet x position
#define ALIENS_BULLET_Y 2 //Index of the array in the struct that keeps track of bullet y position
#define ALIENS_BULLET_NOT_FOUND -1 //Value that represents a bullet being off screen
#define ALIENS_LEFT_COL 0x400 //100 0000 0000
#define ALIENS_FIRST 0 //First row of aliens
#define ALIENS_SECOND 1 //Second row of aliens
#define ALIENS_THIRD 2 //Third row of aliens
#define ALIENS_FOURTH 3 //Fourth row of aliens
#define ALIENS_FIFTH 4 //Fifth row of aliens

// Packs each horizontal line of the figures into a single 12 bit word.
#define packword12(b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0) \
 ((b11 << 11) | (b10 << 10) | (b9  << 9 ) | (b8  << 8 ) | (b7  << 7 ) | (b6  << 6 ) | \
 (b5  << 5 ) | (b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ))

// Packs each horizontal line of the figures into a single 11 bit word.
#define packword11(b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0) \
 ((b10 << 10) | (b9  << 9 ) | (b8  << 8 ) | (b7  << 7 ) | (b6  << 6 ) | \
 (b5  << 5 ) | (b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ))

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


static uint16_t aliens_alive[] = // This global will keep track of which aliens are alive. 1 = alive, 0 = dead
{
	packword11(1,1,1,1,1,1,1,1,1,1,1),
	packword11(1,1,1,1,1,1,1,1,1,1,1),
	packword11(1,1,1,1,1,1,1,1,1,1,1),
	packword11(1,1,1,1,1,1,1,1,1,1,1),
	packword11(1,1,1,1,1,1,1,1,1,1,1)
};

static int16_t dead_alien_space_left = 0;
static int16_t dead_alien_space_right = 0;
static int16_t alien_frame_pos_x = GLOBALS_MAX_X/GLOBALS_TWO - ALIENS_FRAME_SIZE_X/GLOBALS_TWO; // Current x position of alien frame
static int16_t alien_frame_pos_y = ALIENS_START_Y; // Current y position of alien frame
static uint8_t alien_frame_direction = 0; // Right when 0, Left when 1
static uint8_t aliens_legs = 0; // 0 when legs closed, 1 when legs opened
static uint16_t alien_bullets[ALIENS_MAX_NUM_BULLETS][ALIENS_BULLET_HEIGHT] = {
// 4 arrays, one for each bullet. 3 numbers in each array, index 0 is the status (0 off - 1 on), index 1 is the x position, index 2 is the y position
		{0,0,0},
		{0,0,0},
		{0,0,0},
		{0,0,0}
};

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

//Helper function to shift alien frame right
static void shift_aliens_right()
{
	if ((alien_frame_pos_x + ALIENS_FRAME_SIZE_X + ALIENS_X_MOVEMENT) > GLOBALS_MAX_X+dead_alien_space_right) //If our frame has reached the right edge
	{
		alien_frame_pos_y += ALIENS_Y_MOVEMENT; //Shift alien frame down
		alien_frame_direction = 1; //Change alien frame direction
	}
	else
		alien_frame_pos_x += ALIENS_X_MOVEMENT; //Shift alien frame right
}

//Helper function to shift alien frame left
static void shift_aliens_left()
{
	if ((alien_frame_pos_x - ALIENS_X_MOVEMENT) < (-1*dead_alien_space_left)) //If our frame has reached the left edge
	{
		alien_frame_pos_y += ALIENS_Y_MOVEMENT; //Shift alien frame down
		alien_frame_direction = 0; //Change alien frame direction
	}
	else
		alien_frame_pos_x -= ALIENS_X_MOVEMENT; //Shift alien frame left
}

//Helper function to erase a certain bullet. Takes framePointer, current x_pos, and y_pos of the bullet
static void erase_alien_bullet(unsigned int * framePointer, uint16_t x_pos, uint16_t y_pos)
{
	uint8_t y; //Iterator
	for (y=0; y < ALIENS_BULLET_HEIGHT; y++){ //Loop through the height of the bullet and draw a box of 4 black pixels
		framePointer[(y_pos + (GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + x_pos + (GLOBALS_DOUBLE)] = GLOBALS_BLACK;
		framePointer[(y_pos + (GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + x_pos + (GLOBALS_DOUBLE+1)] = GLOBALS_BLACK;
		framePointer[(y_pos + (GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + x_pos + (GLOBALS_DOUBLE)] = GLOBALS_BLACK;
		framePointer[(y_pos + (GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + x_pos + (GLOBALS_DOUBLE+1)] = GLOBALS_BLACK;
	}
}

//Helper function to draw a certain bullet. Takes framePointer, current x_pos, and y_pos of the bullet
static void draw_alien_bullet(unsigned int * framePointer, uint16_t x_pos, uint16_t y_pos)
{
	uint8_t y; //Iterator
	for (y=0; y < ALIENS_BULLET_HEIGHT; y++){//Loop through the height of the bullet and draw a box of 4 white pixels
		framePointer[(y_pos + (GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + x_pos + (GLOBALS_DOUBLE)] = GLOBALS_WHITE;
		framePointer[(y_pos + (GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + x_pos + (GLOBALS_DOUBLE+1)] = GLOBALS_WHITE;
		framePointer[(y_pos + (GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + x_pos + (GLOBALS_DOUBLE)] = GLOBALS_WHITE;
		framePointer[(y_pos + (GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + x_pos + (GLOBALS_DOUBLE+1)] = GLOBALS_WHITE;
	}
}

static void draw_alien_explosion(unsigned int * framePointer, uint16_t current_pos_x, uint16_t current_pos_y)
{
	uint8_t x, y; // Iterators
	for (y=0; y < 10; y++){ // Loop through the y coordinates
		for (x=0; x < 12; x++){ // Loop through the x coordinates
			 if(alien_explosion_12x10[y] & (1<<x)){ // If the alien struct is 1 in this location, write a box of 4 black pixels
				 framePointer[(current_pos_y + (GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + current_pos_x + (GLOBALS_DOUBLE*x)] = GLOBALS_WHITE;
				 framePointer[(current_pos_y + (GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + current_pos_x + (GLOBALS_DOUBLE*x)+1] = GLOBALS_WHITE;
				 framePointer[(current_pos_y + (GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + current_pos_x + (GLOBALS_DOUBLE*x)] = GLOBALS_WHITE;
				 framePointer[(current_pos_y + (GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + current_pos_x + (GLOBALS_DOUBLE*x)+1] = GLOBALS_WHITE;
			 }
		}
	}
}

static void erase_alien_explosion(unsigned int * framePointer, uint16_t current_pos_x, uint16_t current_pos_y)
{
	uint8_t x, y; // Iterators
	for (y=0; y < 10; y++){ // Loop through the y coordinates
		for (x=0; x < 12; x++){ // Loop through the x coordinates
			 if(alien_explosion_12x10[y] & (1<<x)){ // If the alien struct is 1 in this location, write a box of 4 black pixels
				 framePointer[(current_pos_y + (GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + current_pos_x + (GLOBALS_DOUBLE*x)] = GLOBALS_BLACK;
				 framePointer[(current_pos_y + (GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + current_pos_x + (GLOBALS_DOUBLE*x)+1] = GLOBALS_BLACK;
				 framePointer[(current_pos_y + (GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + current_pos_x + (GLOBALS_DOUBLE*x)] = GLOBALS_BLACK;
				 framePointer[(current_pos_y + (GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + current_pos_x + (GLOBALS_DOUBLE*x)+1] = GLOBALS_BLACK;
			 }
		}
	}
}

//Move the aliens left or right according to their current direction and position
void update_alien_pos(unsigned int * framePointer)
{
	uint16_t original_x_pos = alien_frame_pos_x; //Save original x position for reference
	uint16_t previous_x_pos = alien_frame_pos_x; //Save previous x position for erasing
	uint16_t previous_y_pos = alien_frame_pos_y; //Save previous y position for erasing

	if (alien_frame_direction) //If the alien has been moving left
		shift_aliens_left();
	else //If the alien has been moving right
		shift_aliens_right();

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

//Erase the alien at alien_num and mark that alien as dead, takes framePointer and alien number as parameters
void kill_alien(unsigned int * framePointer, uint8_t alien_num)
{
	if (alien_num > ALIENS_MAX_NUM) // Invalid
		return;

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
	draw_alien_explosion(framePointer, alien_x_pos, alien_y_pos);

	//OR all rows to see if a column has been killed off
	uint16_t all_rows = ~(	aliens_alive[ALIENS_FIRST] | aliens_alive[ALIENS_SECOND] |
							aliens_alive[ALIENS_THIRD] | aliens_alive[ALIENS_FOURTH] | aliens_alive[ALIENS_FIFTH]);
	uint16_t dead_pos = ALIENS_LEFT_COL; //Iterator
	uint16_t dead_rows = 0; //Counter to keep track of total dead columns
	//This loop will check the empty alien rows starting on the left
	while (1)
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
	while (1)
	{
		if (all_rows & dead_pos){ //Check if all the aliens at this position are dead
			dead_rows += 1; //Increment the number of dead alien rows
			dead_pos <<= 1; //Shift the position of the column left one
		}
		else
			break;
	}
	dead_alien_space_right = dead_rows*(GLOBALS_TWO*ALIENS_BITS_X + ALIENS_SPACER_X); //Set global dead space
}

//This will shift all alien bullets down the display a frame, takes framePointer as the parameter
void update_alien_bullets(unsigned int * framePointer)
{
	uint8_t i; //Iterator
	for (i=0; i < ALIENS_MAX_NUM_BULLETS; i++) // Loop through all bullet slots
	{
		if (alien_bullets[i][ALIENS_BULLET_STATUS]) // If there is a bullet slot that's active
		{
			erase_alien_bullet(framePointer, alien_bullets[i][ALIENS_BULLET_X], alien_bullets[i][ALIENS_BULLET_Y]);
			alien_bullets[i][ALIENS_BULLET_Y] += ALIENS_MAX_NUM_BULLETS; //Increment bullet y position
			if (alien_bullets[i][ALIENS_BULLET_Y] >= GLOBALS_MAX_Y - ALIENS_BULLET_DISAPPEAR)//bullet erases when it gets to bottom green line not bottom of screen
			{
				alien_bullets[i][ALIENS_BULLET_STATUS] = 0;
				continue;
			}
			draw_alien_bullet(framePointer, alien_bullets[i][ALIENS_BULLET_X], alien_bullets[i][ALIENS_BULLET_Y]);
		}
	}
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

	while(1)
	{
		uint8_t index = rand() % ALIENS_COLS;// Get random index of an alien (0-10)
		if (aliens_alive[ALIENS_MAX_NUM_BULLETS] & (1<<index)){ // Check the bottom row of the aliens
			alien_bullets[bullet][ALIENS_BULLET_STATUS] = 1;
			alien_bullets[bullet][ALIENS_BULLET_X] = alien_frame_pos_x + ((GLOBALS_TWO*ALIENS_BITS_X + ALIENS_SPACER_X)*index + ALIENS_BITS_X); // x position of bullet
			alien_bullets[bullet][ALIENS_BULLET_Y] = alien_frame_pos_y + ((GLOBALS_TWO*ALIENS_BITS_Y + ALIENS_SPACER_Y)*(ALIENS_MAX_NUM_BULLETS+1)); // y position of bullet
			break;
		}
	}
	draw_alien_bullet(framePointer, alien_bullets[bullet][ALIENS_BULLET_X], alien_bullets[bullet][ALIENS_BULLET_Y]);
}







=======
#include "aliens.h"
#include "globals.h"
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
#define ALIENS_Y_MOVEMENT 4 // Number of pixels to move aliens in the y direction
#define ALIENS_MAX_NUM 54 // Total number of aliens
#define ALIENS_START_Y 75 // Number of pixels from the top to start the alien frame
#define ALIENS_MAX_NUM_BULLETS 4 //Maximum number of bullets that the aliens can shoot at a time
#define ALIENS_BULLET_HEIGHT 3 // Number of pixels high the alien bullet is
#define ALIENS_BULLET_DISAPPEAR 45 // Number of pixels from the bottom for bullet to disappear
#define ALIENS_BULLET_STATUS 0 //Index of the array in the struct that keeps track of bullet status
#define ALIENS_BULLET_X 1 //Index of the array in the struct that keeps track of bullet x position
#define ALIENS_BULLET_Y 2 //Index of the array in the struct that keeps track of bullet y position
#define ALIENS_BULLET_NOT_FOUND -1 //Value that represents a bullet being off screen
#define ALIENS_LEFT_COL 0x400 //100 0000 0000
#define ALIENS_FIRST 0 //First row of aliens
#define ALIENS_SECOND 1 //Second row of aliens
#define ALIENS_THIRD 2 //Third row of aliens
#define ALIENS_FOURTH 3 //Fourth row of aliens
#define ALIENS_FIFTH 4 //Fifth row of aliens

// Packs each horizontal line of the figures into a single 12 bit word.
#define packword12(b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0) \
 ((b11 << 11) | (b10 << 10) | (b9  << 9 ) | (b8  << 8 ) | (b7  << 7 ) | (b6  << 6 ) | \
 (b5  << 5 ) | (b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ))

// Packs each horizontal line of the figures into a single 11 bit word.
#define packword11(b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0) \
 ((b10 << 10) | (b9  << 9 ) | (b8  << 8 ) | (b7  << 7 ) | (b6  << 6 ) | \
 (b5  << 5 ) | (b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ))

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


static uint16_t aliens_alive[] = // This global will keep track of which aliens are alive. 1 = alive, 0 = dead
{
	packword11(1,1,1,1,1,1,1,1,1,1,1),
	packword11(1,1,1,1,1,1,1,1,1,1,1),
	packword11(1,1,1,1,1,1,1,1,1,1,1),
	packword11(1,1,1,1,1,1,1,1,1,1,1),
	packword11(1,1,1,1,1,1,1,1,1,1,1)
};

static int16_t dead_alien_space_left = 0;
static int16_t dead_alien_space_right = 0;
static int16_t alien_frame_pos_x = GLOBALS_MAX_X/GLOBALS_TWO - ALIENS_FRAME_SIZE_X/GLOBALS_TWO; // Current x position of alien frame
static int16_t alien_frame_pos_y = ALIENS_START_Y; // Current y position of alien frame
static uint8_t alien_frame_direction = 0; // Right when 0, Left when 1
static uint8_t aliens_legs = 0; // 0 when legs closed, 1 when legs opened
static uint16_t alien_bullets[ALIENS_MAX_NUM_BULLETS][ALIENS_BULLET_HEIGHT] = {
// 4 arrays, one for each bullet. 3 numbers in each array, index 0 is the status (0 off - 1 on), index 1 is the x position, index 2 is the y position
		{0,0,0},
		{0,0,0},
		{0,0,0},
		{0,0,0}
};

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

//Helper function to shift alien frame right
static void shift_aliens_right()
{
	if ((alien_frame_pos_x + ALIENS_FRAME_SIZE_X + ALIENS_X_MOVEMENT) > GLOBALS_MAX_X+dead_alien_space_right) //If our frame has reached the right edge
	{
		alien_frame_pos_y += ALIENS_Y_MOVEMENT; //Shift alien frame down
		alien_frame_direction = 1; //Change alien frame direction
	}
	else
		alien_frame_pos_x += ALIENS_X_MOVEMENT; //Shift alien frame right
}

//Helper function to shift alien frame left
static void shift_aliens_left()
{
	if ((alien_frame_pos_x - ALIENS_X_MOVEMENT) < (-1*dead_alien_space_left)) //If our frame has reached the left edge
	{
		alien_frame_pos_y += ALIENS_Y_MOVEMENT; //Shift alien frame down
		alien_frame_direction = 0; //Change alien frame direction
	}
	else
		alien_frame_pos_x -= ALIENS_X_MOVEMENT; //Shift alien frame left
}

//Helper function to erase a certain bullet. Takes framePointer, current x_pos, and y_pos of the bullet
static void erase_alien_bullet(unsigned int * framePointer, uint16_t x_pos, uint16_t y_pos)
{
	uint8_t y; //Iterator
	for (y=0; y < ALIENS_BULLET_HEIGHT; y++){ //Loop through the height of the bullet and draw a box of 4 black pixels
		framePointer[(y_pos + (GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + x_pos + (GLOBALS_DOUBLE)] = GLOBALS_BLACK;
		framePointer[(y_pos + (GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + x_pos + (GLOBALS_DOUBLE+1)] = GLOBALS_BLACK;
		framePointer[(y_pos + (GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + x_pos + (GLOBALS_DOUBLE)] = GLOBALS_BLACK;
		framePointer[(y_pos + (GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + x_pos + (GLOBALS_DOUBLE+1)] = GLOBALS_BLACK;
	}
}

//Helper function to draw a certain bullet. Takes framePointer, current x_pos, and y_pos of the bullet
static void draw_alien_bullet(unsigned int * framePointer, uint16_t x_pos, uint16_t y_pos)
{
	uint8_t y; //Iterator
	for (y=0; y < ALIENS_BULLET_HEIGHT; y++){//Loop through the height of the bullet and draw a box of 4 white pixels
		framePointer[(y_pos + (GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + x_pos + (GLOBALS_DOUBLE)] = GLOBALS_WHITE;
		framePointer[(y_pos + (GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + x_pos + (GLOBALS_DOUBLE+1)] = GLOBALS_WHITE;
		framePointer[(y_pos + (GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + x_pos + (GLOBALS_DOUBLE)] = GLOBALS_WHITE;
		framePointer[(y_pos + (GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + x_pos + (GLOBALS_DOUBLE+1)] = GLOBALS_WHITE;
	}
}

static void draw_alien_explosion(unsigned int * framePointer, uint16_t current_pos_x, uint16_t current_pos_y)
{
	uint8_t x, y; // Iterators
	for (y=0; y < 10; y++){ // Loop through the y coordinates
		for (x=0; x < 12; x++){ // Loop through the x coordinates
			 if(alien_explosion_12x10[y] & (1<<x)){ // If the alien struct is 1 in this location, write a box of 4 black pixels
				 framePointer[(current_pos_y + (GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + current_pos_x + (GLOBALS_DOUBLE*x)] = GLOBALS_WHITE;
				 framePointer[(current_pos_y + (GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + current_pos_x + (GLOBALS_DOUBLE*x)+1] = GLOBALS_WHITE;
				 framePointer[(current_pos_y + (GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + current_pos_x + (GLOBALS_DOUBLE*x)] = GLOBALS_WHITE;
				 framePointer[(current_pos_y + (GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + current_pos_x + (GLOBALS_DOUBLE*x)+1] = GLOBALS_WHITE;
			 }
		}
	}
}

static void erase_alien_explosion(unsigned int * framePointer, uint16_t current_pos_x, uint16_t current_pos_y)
{
	uint8_t x, y; // Iterators
	for (y=0; y < 10; y++){ // Loop through the y coordinates
		for (x=0; x < 12; x++){ // Loop through the x coordinates
			 if(alien_explosion_12x10[y] & (1<<x)){ // If the alien struct is 1 in this location, write a box of 4 black pixels
				 framePointer[(current_pos_y + (GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + current_pos_x + (GLOBALS_DOUBLE*x)] = GLOBALS_BLACK;
				 framePointer[(current_pos_y + (GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + current_pos_x + (GLOBALS_DOUBLE*x)+1] = GLOBALS_BLACK;
				 framePointer[(current_pos_y + (GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + current_pos_x + (GLOBALS_DOUBLE*x)] = GLOBALS_BLACK;
				 framePointer[(current_pos_y + (GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + current_pos_x + (GLOBALS_DOUBLE*x)+1] = GLOBALS_BLACK;
			 }
		}
	}
}

//Move the aliens left or right according to their current direction and position
void update_alien_pos(unsigned int * framePointer)
{
	uint16_t original_x_pos = alien_frame_pos_x; //Save original x position for reference
	uint16_t previous_x_pos = alien_frame_pos_x; //Save previous x position for erasing
	uint16_t previous_y_pos = alien_frame_pos_y; //Save previous y position for erasing

	if (alien_frame_direction) //If the alien has been moving left
		shift_aliens_left();
	else //If the alien has been moving right
		shift_aliens_right();

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

//Erase the alien at alien_num and mark that alien as dead, takes framePointer and alien number as parameters
void kill_alien(unsigned int * framePointer, uint8_t alien_num)
{
	if (alien_num > ALIENS_MAX_NUM) // Invalid
		return;

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
	draw_alien_explosion(framePointer, alien_x_pos, alien_y_pos);

	//OR all rows to see if a column has been killed off
	uint16_t all_rows = ~(	aliens_alive[ALIENS_FIRST] | aliens_alive[ALIENS_SECOND] |
							aliens_alive[ALIENS_THIRD] | aliens_alive[ALIENS_FOURTH] | aliens_alive[ALIENS_FIFTH]);
	uint16_t dead_pos = ALIENS_LEFT_COL; //Iterator
	uint16_t dead_rows = 0; //Counter to keep track of total dead columns
	//This loop will check the empty alien rows starting on the left
	while (1)
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
	while (1)
	{
		if (all_rows & dead_pos){ //Check if all the aliens at this position are dead
			dead_rows += 1; //Increment the number of dead alien rows
			dead_pos <<= 1; //Shift the position of the column left one
		}
		else
			break;
	}
	dead_alien_space_right = dead_rows*(GLOBALS_TWO*ALIENS_BITS_X + ALIENS_SPACER_X); //Set global dead space
}

//This will shift all alien bullets down the display a frame, takes framePointer as the parameter
void update_alien_bullets(unsigned int * framePointer)
{
	uint8_t i; //Iterator
	for (i=0; i < ALIENS_MAX_NUM_BULLETS; i++) // Loop through all bullet slots
	{
		if (alien_bullets[i][ALIENS_BULLET_STATUS]) // If there is a bullet slot that's active
		{
			erase_alien_bullet(framePointer, alien_bullets[i][ALIENS_BULLET_X], alien_bullets[i][ALIENS_BULLET_Y]);
			alien_bullets[i][ALIENS_BULLET_Y] += ALIENS_MAX_NUM_BULLETS; //Increment bullet y position
			if (alien_bullets[i][ALIENS_BULLET_Y] >= GLOBALS_MAX_Y - ALIENS_BULLET_DISAPPEAR)//bullet erases when it gets to bottom green line not bottom of screen
			{
				alien_bullets[i][ALIENS_BULLET_STATUS] = 0;
				continue;
			}
			draw_alien_bullet(framePointer, alien_bullets[i][ALIENS_BULLET_X], alien_bullets[i][ALIENS_BULLET_Y]);
		}
	}
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

	while(1)
	{
		uint8_t index = rand() % ALIENS_COLS;// Get random index of an alien (0-10)
		if (aliens_alive[ALIENS_MAX_NUM_BULLETS] & (1<<index)){ // Check the bottom row of the aliens
			alien_bullets[bullet][ALIENS_BULLET_STATUS] = 1;
			alien_bullets[bullet][ALIENS_BULLET_X] = alien_frame_pos_x + ((GLOBALS_TWO*ALIENS_BITS_X + ALIENS_SPACER_X)*index + ALIENS_BITS_X); // x position of bullet
			alien_bullets[bullet][ALIENS_BULLET_Y] = alien_frame_pos_y + ((GLOBALS_TWO*ALIENS_BITS_Y + ALIENS_SPACER_Y)*(ALIENS_MAX_NUM_BULLETS+1)); // y position of bullet
			break;
		}
	}
	draw_alien_bullet(framePointer, alien_bullets[bullet][ALIENS_BULLET_X], alien_bullets[bullet][ALIENS_BULLET_Y]);
}







>>>>>>> 46133ae2d6a022a717e89ee90e8959352f12048c
