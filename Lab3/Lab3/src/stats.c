<<<<<<< HEAD
#include <stdint.h>
#include "stats.h"
#include "globals.h"
#include "tank.h"
#include "saucer.h"

#define STATS_ROWS 5 //Number of rows
#define STATS_COLUMNS 28 //Number of columns
#define STATS_NEW_COLUMNS 3 //Number of columns after 1st iteration
#define STATS_LIVES_COLUMNS 24 //Number of columns in lives word
#define STATS_TANK_ROWS 8 //Number of rows in tank
#define STATS_TANK_COLUMNS 15 //Number of columns in tank
#define STATS_MAX_NUM_DIGITS 4
#define STATS_DIGIT_PLACE_OP 10
#define STATS_ONES_PLACE 1
#define STATS_TENS_PLACE 10
#define STATS_HUNDS_PLACE 100
#define STATS_THOUS_PLACE 1000
// Packs each horizontal line of the figures into a single 29 bit word.
#define packword29(b28,b27,b26,b25,b24,b23,b22,b21,b20,b19,b18,b17,b16,b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0) \
((b28 << 28) | (b27 << 27) | (b26 << 26) | (b25 << 25) | (b24 << 24) | \
 (b23 << 23) | (b22 << 22) | (b21 << 21) | (b20 << 20) | (b19 << 19) | (b18 << 18) | (b17 << 17) | (b16 << 16) |  \
 (b15 << 15) | (b14 << 14) | (b13 << 13) | (b12 << 12) | (b11 << 11) | (b10 << 10) | (b9  << 9 ) | (b8  << 8 ) | \
 (b7  << 7 ) | (b6  << 6 ) | (b5  << 5 ) | (b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ) )

// Packs each horizontal line of the figures into a single 24 bit word.
#define packword24(b23,b22,b21,b20,b19,b18,b17,b16,b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0) \
((b23 << 23) | (b22 << 22) | (b21 << 21) | (b20 << 20) | (b19 << 19) | (b18 << 18) | (b17 << 17) | (b16 << 16) |  \
 (b15 << 15) | (b14 << 14) | (b13 << 13) | (b12 << 12) | (b11 << 11) | (b10 << 10) | (b9  << 9 ) | (b8  << 8 ) | \
 (b7  << 7 ) | (b6  << 6 ) | (b5  << 5 ) | (b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ) )

// Packs each horizontal line of the figures into a single 4 bit word.
#define packword4(b3,b2,b1,b0) \
((b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ) )

// SCORE
static const uint32_t stats_score29x5[] =
{
packword29(0,1,1,1,1,0,0,1,1,1,1,0,0,1,1,1,0,0,1,1,1,1,0,0,1,1,1,1,1),
packword29(1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,0),
packword29(0,1,1,1,0,0,1,0,0,0,0,0,1,0,0,0,1,0,1,1,1,1,0,0,1,1,1,1,0),
packword29(0,0,0,0,1,0,1,0,0,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,0),
packword29(1,1,1,1,0,0,0,1,1,1,1,0,0,1,1,1,0,0,1,0,0,0,1,0,1,1,1,1,1)
};

// LIVES
static const uint32_t stats_lives_24x5[] =
{
packword24(1,0,0,0,0,1,0,1,0,0,0,1,0,1,1,1,1,1,0,0,1,1,1,1),
packword24(1,0,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,0,0,1,0,0,0,0),
packword24(1,0,0,0,0,1,0,1,0,0,0,1,0,1,1,1,1,0,0,0,1,1,1,0),
packword24(1,0,0,0,0,1,0,0,1,0,1,0,0,1,0,0,0,0,0,0,0,0,0,1),
packword24(1,1,1,1,0,1,0,0,0,1,0,0,0,1,1,1,1,1,0,1,1,1,1,0)
};


// sprite for number 0
static const uint8_t stats0_4x5[] =
{
packword4(0,1,1,0),
packword4(1,0,0,1),
packword4(1,0,0,1),
packword4(1,0,0,1),
packword4(0,1,1,0)
};

//sprite for number 1
static const uint8_t stats1_4x5[] =
{
packword4(0,0,1,0),
packword4(0,1,1,0),
packword4(1,0,1,0),
packword4(0,0,1,0),
packword4(1,1,1,1)
};

//sprite for number 2
static const uint8_t stats2_4x5[] =
{
packword4(1,1,1,0),
packword4(0,0,0,1),
packword4(0,1,1,0),
packword4(1,0,0,0),
packword4(1,1,1,1)
};

//sprite for number 3
static const uint8_t stats3_4x5[] =
{
packword4(1,1,1,0),
packword4(0,0,0,1),
packword4(0,1,1,0),
packword4(0,0,0,1),
packword4(1,1,1,0)
};

//sprite for number 4
static const uint8_t stats4_4x5[] =
{
packword4(1,0,1,0),
packword4(1,0,1,0),
packword4(1,1,1,1),
packword4(0,0,1,0),
packword4(0,0,1,0)
};

//sprite for number 5
static const uint8_t stats5_4x5[] =
{
packword4(1,1,1,1),
packword4(1,0,0,0),
packword4(1,1,1,0),
packword4(0,0,0,1),
packword4(1,1,1,0)
};

//sprite for number 6
static const uint8_t stats6_4x5[] =
{
packword4(0,1,1,0),
packword4(1,0,0,0),
packword4(1,1,1,0),
packword4(1,0,0,1),
packword4(0,1,1,0)
};

//sprite for number 7
static const uint8_t stats7_4x5[] =
{
packword4(1,1,1,1),
packword4(1,0,0,1),
packword4(0,0,1,0),
packword4(0,1,0,0),
packword4(0,1,0,0)
};

//sprite for number 8
static const uint8_t stats8_4x5[] =
{
packword4(0,1,1,0),
packword4(1,0,0,1),
packword4(0,1,1,0),
packword4(1,0,0,1),
packword4(0,1,1,0)
};

//sprite for number 9
static const uint8_t stats9_4x5[] =
{
packword4(0,1,1,0),
packword4(1,0,0,1),
packword4(0,1,1,1),
packword4(0,0,0,1),
packword4(0,1,1,0)
};

uint16_t points_score = 0;//holds the updated score
uint16_t prev_points_score = 0;//holds previous score

static uint8_t score_num_to_use[STATS_ROWS] = {0,0,0,0,0};//dummy array you can set to equal any of the number sprite bits

//return bits of the sprite digit you want
static uint8_t stats_get_score_num_to_use_bits(uint8_t row)
{
	return score_num_to_use[row];
}

//returns the value of your current score
uint16_t stats_get_points_score()
{
	return points_score;
}

//sets the value of your score possibly from other files
void stats_set_points_score(uint16_t update_score)
{
	points_score = update_score;
}

//returns the value of your current score
uint16_t stats_get_prev_points_score()
{
	return prev_points_score;
}

//sets the value of your score possibly from other files
void stats_set_prev_points_score(uint16_t update_score)
{
	prev_points_score = update_score;
}

//get bits for the word score
static uint32_t stats_get_score_word_bits(uint8_t row)
{
	return stats_score29x5[row];
}

//get bits for the word lives
static uint32_t stats_get_lives_word_bits(uint8_t row)
{
	return stats_lives_24x5[row];
}


//draws the word "score" as a single sprite
void stats_draw_score_word(unsigned int * framePointer)
{
    int8_t x, y, i; //Iterators
    for(y=0; y < STATS_ROWS; y++){//iterate through all rows
		for(x=STATS_COLUMNS; x >= 0; x--){//iterate through all col
			 if(stats_get_score_word_bits(y) & (1<<x)){//draw a pixel if bit is a 1
				 framePointer[(GLOBALS_SCORE_LIVES_Y_POS+(GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + GLOBALS_SCORE_WORD_X_POS + (GLOBALS_DOUBLE*(STATS_COLUMNS-x))] = GLOBALS_WHITE;//draw 4 white pixels per bit
				 framePointer[(GLOBALS_SCORE_LIVES_Y_POS+(GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + GLOBALS_SCORE_WORD_X_POS + (GLOBALS_DOUBLE*(STATS_COLUMNS-x))] = GLOBALS_WHITE;//draw 4 white pixels per bit
				 framePointer[(GLOBALS_SCORE_LIVES_Y_POS+(GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + GLOBALS_SCORE_WORD_X_POS + (GLOBALS_DOUBLE*(STATS_COLUMNS-x))+1] = GLOBALS_WHITE;//draw 4 white pixels per bit
				 framePointer[(GLOBALS_SCORE_LIVES_Y_POS+(GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + GLOBALS_SCORE_WORD_X_POS + (GLOBALS_DOUBLE*(STATS_COLUMNS-x))+1] = GLOBALS_WHITE;//draw 4 white pixels per bit
			 }
		}
    }
	for(i=0; i<STATS_ROWS; i++){
		score_num_to_use[i] = stats0_4x5[i];//set dummy array equal to bits in stats0_4x5
	}
    stats_draw_score_number(framePointer,GLOBALS_SCORE_NUMBER_X_POS_THOUS, GLOBALS_SCORE_LIVES_Y_POS);//initialize with score of "0"
}

//draw the number value of the player score. pass in x position to correctly position each digit
void stats_draw_score_number(unsigned int * framePointer, int16_t digit_x_pos, uint8_t digit_y_pos)
{
	int8_t x, y; //Iterators
    //loop to draw the number zero initially
    for(y=0; y < STATS_ROWS; y++){//iterate through all rows
		for(x=STATS_NEW_COLUMNS; x >= 0; x--){//iterate through all col
			 if(stats_get_score_num_to_use_bits(y) & (1<<x)){//draw a pixel if bit is a 1
				 framePointer[(digit_y_pos+(GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + digit_x_pos + (GLOBALS_DOUBLE*(STATS_NEW_COLUMNS-x))] = GLOBALS_GREEN;//draw 4 green pixels per bit
				 framePointer[(digit_y_pos+(GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + digit_x_pos + (GLOBALS_DOUBLE*(STATS_NEW_COLUMNS-x))] = GLOBALS_GREEN;//draw 4 green pixels per bit
				 framePointer[(digit_y_pos+(GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + digit_x_pos + (GLOBALS_DOUBLE*(STATS_NEW_COLUMNS-x))+1] = GLOBALS_GREEN;//draw 4 green pixels per bit
				 framePointer[(digit_y_pos+(GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + digit_x_pos + (GLOBALS_DOUBLE*(STATS_NEW_COLUMNS-x))+1] = GLOBALS_GREEN;//draw 4 green pixels per bit
			 }
		}
    }
}

//erase the number value of the player score. pass in x position to correctly position each digit
void stats_erase_score_number(unsigned int * framePointer, int16_t digit_x_pos, uint8_t digit_y_pos)
{
	int8_t x, y; //Iterators
    //loop to draw the number zero initially
    for(y=0; y < STATS_ROWS; y++){//iterate through all rows
		for(x=STATS_NEW_COLUMNS; x >= 0; x--){//iterate through all col
			 if(stats_get_score_num_to_use_bits(y) & (1<<x)){//draw a pixel if bit is a 1
				 framePointer[(digit_y_pos+(GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + digit_x_pos + (GLOBALS_DOUBLE*(STATS_NEW_COLUMNS-x))] = GLOBALS_BLACK;//draw 4 green pixels per bit
				 framePointer[(digit_y_pos+(GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + digit_x_pos + (GLOBALS_DOUBLE*(STATS_NEW_COLUMNS-x))] = GLOBALS_BLACK;//draw 4 green pixels per bit
				 framePointer[(digit_y_pos+(GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + digit_x_pos + (GLOBALS_DOUBLE*(STATS_NEW_COLUMNS-x))+1] = GLOBALS_BLACK;//draw 4 green pixels per bit
				 framePointer[(digit_y_pos+(GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + digit_x_pos + (GLOBALS_DOUBLE*(STATS_NEW_COLUMNS-x))+1] = GLOBALS_BLACK;//draw 4 green pixels per bit
			 }
		}
    }
}

//draws the word "lives" as a single sprite
void stats_draw_lives_word(unsigned int * framePointer)
{
    int8_t x, y; //Iterators
    for(y=0; y < STATS_ROWS; y++){//iterate through all rows
		for(x=STATS_LIVES_COLUMNS-1; x >= 0; x--){//iterate through all col
			 if(stats_get_lives_word_bits(y) & (1<<x)){//draw a pixel if bit is a 1
				 framePointer[(GLOBALS_SCORE_LIVES_Y_POS+(GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + GLOBALS_LIVES_WORD_X_POS + (GLOBALS_DOUBLE*(STATS_LIVES_COLUMNS-x))] = GLOBALS_WHITE;//draw 4 white pixels per bit
				 framePointer[(GLOBALS_SCORE_LIVES_Y_POS+(GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + GLOBALS_LIVES_WORD_X_POS + (GLOBALS_DOUBLE*(STATS_LIVES_COLUMNS-x))] = GLOBALS_WHITE;//draw 4 white pixels per bit
				 framePointer[(GLOBALS_SCORE_LIVES_Y_POS+(GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + GLOBALS_LIVES_WORD_X_POS + (GLOBALS_DOUBLE*(STATS_LIVES_COLUMNS-x))+1] = GLOBALS_WHITE;//draw 4 white pixels per bit
				 framePointer[(GLOBALS_SCORE_LIVES_Y_POS+(GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + GLOBALS_LIVES_WORD_X_POS + (GLOBALS_DOUBLE*(STATS_LIVES_COLUMNS-x))+1] = GLOBALS_WHITE;//draw 4 white pixels per bit
			 }
		}
    }
}

//draws the tank representing a life depending on which one you specify
void stats_draw_lives_tank(unsigned int * framePointer, uint8_t tank_number)
{
	uint16_t lives_tank_x_pos = 0;//variable to be set to correct tank life x_pos to draw
	//switch statement to determine which tank life to draw
	switch(tank_number)
	{
		case 1://left tank life
			lives_tank_x_pos = GLOBALS_LIVES_TANK1_X_POS;//change x pos depending on which tank life to draw (left)
			break;
		case 2://middle tank life
			lives_tank_x_pos = GLOBALS_LIVES_TANK2_X_POS;//change x pos depending on which tank life to draw (middle)
			break;
		case 3://right tank life
			lives_tank_x_pos = GLOBALS_LIVES_TANK3_X_POS;//change x pos depending on tank life to draw (right)
			break;
		default:
			break;
	}

    uint8_t x, y; //Iterators
    for(y=0; y < STATS_TANK_ROWS; y++){//iterate through all rows
		for(x=0; x < STATS_TANK_COLUMNS; x++){//iterate through all col
			 if(tank_get_tank_bits(y) & (1<<x)){//draw a pixel if bit is a one
				 framePointer[(GLOBALS_LIVES_TANK_Y_POS+(GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + lives_tank_x_pos + (GLOBALS_DOUBLE*x)] = GLOBALS_GREEN;//draw 4 green pixels per bit
				 framePointer[(GLOBALS_LIVES_TANK_Y_POS+(GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + lives_tank_x_pos + (GLOBALS_DOUBLE*x)] = GLOBALS_GREEN;//draw 4 green pixels
				 framePointer[(GLOBALS_LIVES_TANK_Y_POS+(GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + lives_tank_x_pos + (GLOBALS_DOUBLE*x)+1] = GLOBALS_GREEN;//draw 4 green pixels
				 framePointer[(GLOBALS_LIVES_TANK_Y_POS+(GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + lives_tank_x_pos + (GLOBALS_DOUBLE*x)+1] = GLOBALS_GREEN;//draw 4 green pixels
			 }
		}
    }
}

//erase the tank life depending on which one you specify
void stats_erase_lives_tank(unsigned int * framePointer, uint8_t tank_number)
{
	uint16_t lives_tank_x_pos = 0;//variable to be set to correct tank life x_pos to draw
	//switch statement to determine which tank life to erase
	switch(tank_number)
	{
		case 1://left tank life
			lives_tank_x_pos = GLOBALS_LIVES_TANK1_X_POS;//change x pos depending on which tank life to erase (left)
			break;
		case 2://middle tank life
			lives_tank_x_pos = GLOBALS_LIVES_TANK2_X_POS;//change x pos depending on which tank life to erase (middle)
			break;
		case 3://right tank life
			lives_tank_x_pos = GLOBALS_LIVES_TANK3_X_POS;//change x pos depending on which tank life to erase (right)
			break;
		default:
			break;
	}

    uint8_t x, y; //Iterators
    for(y=0; y < STATS_TANK_ROWS; y++){//iterate through all rows
		for(x=0; x < STATS_TANK_COLUMNS; x++){//iterate through all col
			 if(tank_get_tank_bits(y) & (1<<x)){//draw a pixel if bit is a 1
				 framePointer[(GLOBALS_LIVES_TANK_Y_POS+(GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + lives_tank_x_pos + (GLOBALS_DOUBLE*x)] = GLOBALS_BLACK;//draw 4 black pixels per bit
				 framePointer[(GLOBALS_LIVES_TANK_Y_POS+(GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + lives_tank_x_pos + (GLOBALS_DOUBLE*x)] = GLOBALS_BLACK;//draw 4 black pixels per bit
				 framePointer[(GLOBALS_LIVES_TANK_Y_POS+(GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + lives_tank_x_pos + (GLOBALS_DOUBLE*x)+1] = GLOBALS_BLACK;//draw 4 black pixels per bit
				 framePointer[(GLOBALS_LIVES_TANK_Y_POS+(GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + lives_tank_x_pos + (GLOBALS_DOUBLE*x)+1] = GLOBALS_BLACK;//draw 4 black pixels per bit
			 }
		}
    }
}


//this function is used to find the appropriate sprite to draw based on the integer value of points_score variable
//its is called once at a time for each digit (ones, tens, hunds, thous) in the integer of points_score variable
void stats_calculate_score_num(uint8_t digit)
{
	uint8_t i; //Iterator
	switch(digit)
	{
		case 0:
			for(i=0; i<STATS_ROWS; i++){//do it for all 5 rows of sprite aka all 5 indices of stats0_4x5 array
				score_num_to_use[i] = stats0_4x5[i];//set dummy array equal to bits in stats0_4x5
			}
			break;
		case 1:
			for(i=0; i<STATS_ROWS; i++){//do it for all 5 rows of sprite aka all 5 indices of stats1_4x5 array
				score_num_to_use[i] = stats1_4x5[i];//set dummy array equal to bits in stats1_4x5
			}
			break;
		case 2:
			for(i=0; i<STATS_ROWS; i++){//do it for all 5 rows of sprite aka all 5 indices of stats2_4x5 array
				score_num_to_use[i] = stats2_4x5[i];//set dummy array equal to bits in stats2_4x5
			}
			break;
		case 3:
			for(i=0; i<STATS_ROWS; i++){//do it for all 5 rows of sprite aka all 5 indices of stats3_4x5 array
				score_num_to_use[i] = stats3_4x5[i];//set dummy array equal to bits in stats3_4x5
			}
			break;
		case 4:
			for(i=0; i<STATS_ROWS; i++){//do it for all 5 rows of sprite aka all 5 indices of stats4_4x5 array
				score_num_to_use[i] = stats4_4x5[i];//set dummy array equal to bits in stats4_4x5
			}
			break;
		case 5:
			for(i=0; i<STATS_ROWS; i++){//do it for all 5 rows of sprite aka all 5 indices of stats5_4x5 array
				score_num_to_use[i] = stats5_4x5[i];//set dummy array equal to bits in stats5_4x5
			}
			break;
		case 6:
			for(i=0; i<STATS_ROWS; i++){//do it for all 5 rows of sprite aka all 5 indices of stats6_4x5 array
				score_num_to_use[i] = stats6_4x5[i];//set dummy array equal to bits in stats6_4x5
			}
			break;
		case 7:
			for(i=0; i<STATS_ROWS; i++){//do it for all 5 rows of sprite aka all 5 indices of stats7_4x5 array
				score_num_to_use[i] = stats7_4x5[i];//set dummy array equal to bits in stats7_4x5
			}
			break;
		case 8:
			for(i=0; i<STATS_ROWS; i++){//do it for all 5 rows of sprite aka all 5 indices of stats8_4x5 array
				score_num_to_use[i] = stats8_4x5[i];//set dummy array equal to bits in stats8_4x5
			}
			break;
		case 9:
			for(i=0; i<STATS_ROWS; i++){//do it for all 5 rows of sprite aka all 5 indices of stats9_4x5 array
				score_num_to_use[i] = stats9_4x5[i];//set dummy array equal to bits in stats9_4x5
			}
			break;
		default:
			break;
	}
}


//this function checks which iteration for the for loop you are in from the stats_update_score()
//the iteration number corresponds to whether you are looking at ones, tens, hunds, thous, place and it
//goes through in that order.
//based on which iteration number (placeholder) the positioning of the digit is calculated
//when you call this from stats_update_score() function you must use a current_score
//that wont change value from iteration to iteration of the for loop in stats_update_score()
int16_t stats_calculate_score_digit_pos(uint8_t iteration, int16_t current_score)
{
	int16_t x_pos = 0;//use this to save the proper x position of the digit you want to draw
	switch(iteration)
	{
		//ones placeholder
		case 0:
			if(current_score >= 0 && current_score < STATS_TENS_PLACE){//current score is 0-9
				x_pos = GLOBALS_SCORE_NUMBER_X_POS_THOUS;//put digit at thousands place
			}
			if(current_score >= STATS_TENS_PLACE && current_score < STATS_HUNDS_PLACE){//current score is 10-99
				x_pos = GLOBALS_SCORE_NUMBER_X_POS_HUNDS;//put digit at hundreds place
			}
			if(current_score >= STATS_HUNDS_PLACE && current_score < STATS_THOUS_PLACE){//current score is 100-999
				x_pos = GLOBALS_SCORE_NUMBER_X_POS_TENS;//put digit at tens place
			}
			if(current_score >= STATS_THOUS_PLACE){//current score is greater than 1000
				x_pos = GLOBALS_SCORE_NUMBER_X_POS_ONES;//put digit at ones place
			}
			break;

		//tens placeholder
		case 1:
			if(current_score < STATS_TENS_PLACE){//current score is less than 10
				x_pos = 700;//set it to 700 as a flag this is a leading zero that you shouldnt draw
			}
			else if(current_score >= STATS_TENS_PLACE && current_score < STATS_HUNDS_PLACE){//current score is 10-99
				x_pos = GLOBALS_SCORE_NUMBER_X_POS_THOUS;//put digit at thousands place
			}
			else if(current_score >= STATS_HUNDS_PLACE && current_score < STATS_THOUS_PLACE){//current score is 100-999
				x_pos = GLOBALS_SCORE_NUMBER_X_POS_HUNDS;//put digit at hundreds place
			}
			else if(current_score >= STATS_THOUS_PLACE){//current score is greater than 1000
				x_pos = GLOBALS_SCORE_NUMBER_X_POS_TENS;//put digit at tens place
			}
			break;

		//hundreds placeholder
		case 2:
			if(current_score < STATS_HUNDS_PLACE){
				x_pos = 700;//set it to 700 as a flag this is a leading zero that you shouldnt draw
			}
			if(current_score >= STATS_HUNDS_PLACE && current_score < STATS_THOUS_PLACE){//current score is 100-999
				x_pos = GLOBALS_SCORE_NUMBER_X_POS_THOUS;//put digit at thousands place
			}
			else if(current_score >= STATS_THOUS_PLACE){//current score is greater than 1000
				x_pos = GLOBALS_SCORE_NUMBER_X_POS_HUNDS;//put digit at hundreds place
			}
			break;

		//thousands placeholder
		case 3:
			if(current_score < STATS_THOUS_PLACE){
				x_pos = 700;//set it to 700 as a flag this is a leading zero that you shouldnt draw
			}
			else if(current_score >= STATS_THOUS_PLACE){//current score is greater than 1000
				x_pos = GLOBALS_SCORE_NUMBER_X_POS_THOUS;//put digit at thousands place
			}
			break;
		default:
			break;
	}
	return x_pos;
}

//this is a top level function which calls all necessary functions to erase the score
//you specify the y position.
//at_saucer is a boolean -> 1=draw score where the saucer is. 0=draw score at regular place
void stats_top_level_erase_score(unsigned int * framePointer, uint8_t y_pos, int8_t at_saucer){
	uint16_t temp_prev_score = prev_points_score;//use as a dummy variable to hold value of prev_points_score
	int8_t i;//to be used in for loops

	//erase the previously drawn score
	for (i=0; i<STATS_MAX_NUM_DIGITS; i++) {//go thru 4 times : ones, tens, hundreds, thousands. and in that order
		uint8_t digit = prev_points_score % STATS_DIGIT_PLACE_OP;//get the remainder of digit aka the value in the smallest placeholder (ones)
		stats_calculate_score_num(digit);//this saves the sprite of the digit to be drawn

		int16_t x_pos = stats_calculate_score_digit_pos(i, temp_prev_score);//calc the x pos of digit. use temp_prev_score because this value doesnt change through loop
		if(x_pos == 700){//if x_pos = 700 it means you have a leading zero you shouldnt draw
			continue;//skip to next iteration cuz you dont want to draw anything
		}
		if(at_saucer == 1){//if you are drawing the score where the saucer is
			x_pos = x_pos - (GLOBALS_SCORE_NUMBER_X_POS_THOUS) + saucer_get_saucer_x_position();//adjust the calculated x_pos to be where the saucer is
			stats_erase_score_number(framePointer, x_pos, y_pos);//draw the score where the saucer is
		}
		else{//draw in regular place
			stats_erase_score_number(framePointer, x_pos, y_pos);//draw the score in the regular place
		}
		prev_points_score = prev_points_score / STATS_DIGIT_PLACE_OP;//divide by ten to move from ones to tens and from tens to hundreds placeholder etc
	}
}

//this is a top level function which calls all necessary functions to draw the score
//you specify the y position.
//at_saucer is a boolean -> 1=draw score where the saucer is. 0=draw score at regular place
void stats_top_level_draw_score(unsigned int * framePointer, uint8_t y_pos, int8_t at_saucer){
	int8_t i;//to be used in for loops

	//draws the updated score
	for (i=0; i<STATS_MAX_NUM_DIGITS; i++) {//go thru 4 times : ones, tens, hundreds, thousands. and in that order
		uint8_t digit = points_score % STATS_DIGIT_PLACE_OP;//get the remainder of digit aka the value in the smallest placeholder (ones)
		stats_calculate_score_num(digit);//this saves the sprite of the digit to be drawn

		int16_t x_pos = stats_calculate_score_digit_pos(i, prev_points_score);//calc the x pos of digit. use prev_points_score because this value doesnt change through loop
		if(x_pos == 700){//if x_pos = 700 it means you have a leading zero you shouldnt draw
			continue;//skip to next iteration cuz you dont want to draw anything
		}
		if(at_saucer == 1){//if you are drawing the score where the saucer is
			x_pos = x_pos - (GLOBALS_SCORE_NUMBER_X_POS_THOUS) + saucer_get_saucer_x_position();//adjust the calculated x_pos to be where the saucer is
			stats_draw_score_number(framePointer, x_pos, y_pos);//draw the score where the saucer is
		}
		else{//draw in regular place
			stats_draw_score_number(framePointer, x_pos, y_pos);//draw the score in the regular place
		}
		points_score = points_score / STATS_DIGIT_PLACE_OP;//divide by ten to move from ones to tens and from tens to hundreds placeholder etc
	}
}

//update the score by erasing the old one and drawing a new one
//functions to calculate digit number and digit positioning are called herein
void stats_update_score(unsigned int * framePointer)
{
	stats_top_level_erase_score(framePointer, GLOBALS_SCORE_LIVES_Y_POS, 0);//erase the old score
	prev_points_score = points_score;//change value of previous score to the new score

	stats_top_level_draw_score(framePointer, GLOBALS_SCORE_LIVES_Y_POS, 0);//draw the new score
	points_score = prev_points_score;//set value of new score back to itself after it got changed in the above function
}






=======
#include <stdint.h>
#include "stats.h"
#include "globals.h"
#include "tank.h"
#include "saucer.h"

#define STATS_ROWS 5 //Number of rows
#define STATS_COLUMNS 28 //Number of columns
#define STATS_NEW_COLUMNS 3 //Number of columns after 1st iteration
#define STATS_LIVES_COLUMNS 24 //Number of columns in lives word
#define STATS_TANK_ROWS 8 //Number of rows in tank
#define STATS_TANK_COLUMNS 15 //Number of columns in tank
#define STATS_MAX_NUM_DIGITS 4
#define STATS_DIGIT_PLACE_OP 10
#define STATS_ONES_PLACE 1
#define STATS_TENS_PLACE 10
#define STATS_HUNDS_PLACE 100
#define STATS_THOUS_PLACE 1000
// Packs each horizontal line of the figures into a single 29 bit word.
#define packword29(b28,b27,b26,b25,b24,b23,b22,b21,b20,b19,b18,b17,b16,b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0) \
((b28 << 28) | (b27 << 27) | (b26 << 26) | (b25 << 25) | (b24 << 24) | \
 (b23 << 23) | (b22 << 22) | (b21 << 21) | (b20 << 20) | (b19 << 19) | (b18 << 18) | (b17 << 17) | (b16 << 16) |  \
 (b15 << 15) | (b14 << 14) | (b13 << 13) | (b12 << 12) | (b11 << 11) | (b10 << 10) | (b9  << 9 ) | (b8  << 8 ) | \
 (b7  << 7 ) | (b6  << 6 ) | (b5  << 5 ) | (b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ) )

// Packs each horizontal line of the figures into a single 24 bit word.
#define packword24(b23,b22,b21,b20,b19,b18,b17,b16,b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0) \
((b23 << 23) | (b22 << 22) | (b21 << 21) | (b20 << 20) | (b19 << 19) | (b18 << 18) | (b17 << 17) | (b16 << 16) |  \
 (b15 << 15) | (b14 << 14) | (b13 << 13) | (b12 << 12) | (b11 << 11) | (b10 << 10) | (b9  << 9 ) | (b8  << 8 ) | \
 (b7  << 7 ) | (b6  << 6 ) | (b5  << 5 ) | (b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ) )

// Packs each horizontal line of the figures into a single 4 bit word.
#define packword4(b3,b2,b1,b0) \
((b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ) )

// SCORE
static const uint32_t stats_score29x5[] =
{
packword29(0,1,1,1,1,0,0,1,1,1,1,0,0,1,1,1,0,0,1,1,1,1,0,0,1,1,1,1,1),
packword29(1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,0),
packword29(0,1,1,1,0,0,1,0,0,0,0,0,1,0,0,0,1,0,1,1,1,1,0,0,1,1,1,1,0),
packword29(0,0,0,0,1,0,1,0,0,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,0),
packword29(1,1,1,1,0,0,0,1,1,1,1,0,0,1,1,1,0,0,1,0,0,0,1,0,1,1,1,1,1)
};

// LIVES
static const uint32_t stats_lives_24x5[] =
{
packword24(1,0,0,0,0,1,0,1,0,0,0,1,0,1,1,1,1,1,0,0,1,1,1,1),
packword24(1,0,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,0,0,1,0,0,0,0),
packword24(1,0,0,0,0,1,0,1,0,0,0,1,0,1,1,1,1,0,0,0,1,1,1,0),
packword24(1,0,0,0,0,1,0,0,1,0,1,0,0,1,0,0,0,0,0,0,0,0,0,1),
packword24(1,1,1,1,0,1,0,0,0,1,0,0,0,1,1,1,1,1,0,1,1,1,1,0)
};


// sprite for number 0
static const uint8_t stats0_4x5[] =
{
packword4(0,1,1,0),
packword4(1,0,0,1),
packword4(1,0,0,1),
packword4(1,0,0,1),
packword4(0,1,1,0)
};

//sprite for number 1
static const uint8_t stats1_4x5[] =
{
packword4(0,0,1,0),
packword4(0,1,1,0),
packword4(1,0,1,0),
packword4(0,0,1,0),
packword4(1,1,1,1)
};

//sprite for number 2
static const uint8_t stats2_4x5[] =
{
packword4(1,1,1,0),
packword4(0,0,0,1),
packword4(0,1,1,0),
packword4(1,0,0,0),
packword4(1,1,1,1)
};

//sprite for number 3
static const uint8_t stats3_4x5[] =
{
packword4(1,1,1,0),
packword4(0,0,0,1),
packword4(0,1,1,0),
packword4(0,0,0,1),
packword4(1,1,1,0)
};

//sprite for number 4
static const uint8_t stats4_4x5[] =
{
packword4(1,0,1,0),
packword4(1,0,1,0),
packword4(1,1,1,1),
packword4(0,0,1,0),
packword4(0,0,1,0)
};

//sprite for number 5
static const uint8_t stats5_4x5[] =
{
packword4(1,1,1,1),
packword4(1,0,0,0),
packword4(1,1,1,0),
packword4(0,0,0,1),
packword4(1,1,1,0)
};

//sprite for number 6
static const uint8_t stats6_4x5[] =
{
packword4(0,1,1,0),
packword4(1,0,0,0),
packword4(1,1,1,0),
packword4(1,0,0,1),
packword4(0,1,1,0)
};

//sprite for number 7
static const uint8_t stats7_4x5[] =
{
packword4(1,1,1,1),
packword4(1,0,0,1),
packword4(0,0,1,0),
packword4(0,1,0,0),
packword4(0,1,0,0)
};

//sprite for number 8
static const uint8_t stats8_4x5[] =
{
packword4(0,1,1,0),
packword4(1,0,0,1),
packword4(0,1,1,0),
packword4(1,0,0,1),
packword4(0,1,1,0)
};

//sprite for number 9
static const uint8_t stats9_4x5[] =
{
packword4(0,1,1,0),
packword4(1,0,0,1),
packword4(0,1,1,1),
packword4(0,0,0,1),
packword4(0,1,1,0)
};

uint16_t points_score = 0;//holds the updated score
uint16_t prev_points_score = 0;//holds previous score

static uint8_t score_num_to_use[STATS_ROWS] = {0,0,0,0,0};//dummy array you can set to equal any of the number sprite bits

//return bits of the sprite digit you want
static uint8_t stats_get_score_num_to_use_bits(uint8_t row)
{
	return score_num_to_use[row];
}

//returns the value of your current score
uint16_t stats_get_points_score()
{
	return points_score;
}

//sets the value of your score possibly from other files
void stats_set_points_score(uint16_t update_score)
{
	points_score = update_score;
}

//returns the value of your current score
uint16_t stats_get_prev_points_score()
{
	return prev_points_score;
}

//sets the value of your score possibly from other files
void stats_set_prev_points_score(uint16_t update_score)
{
	prev_points_score = update_score;
}

//get bits for the word score
static uint32_t stats_get_score_word_bits(uint8_t row)
{
	return stats_score29x5[row];
}

//get bits for the word lives
static uint32_t stats_get_lives_word_bits(uint8_t row)
{
	return stats_lives_24x5[row];
}


//draws the word "score" as a single sprite
void stats_draw_score_word(unsigned int * framePointer)
{
    int8_t x, y, i; //Iterators
    for(y=0; y < STATS_ROWS; y++){//iterate through all rows
		for(x=STATS_COLUMNS; x >= 0; x--){//iterate through all col
			 if(stats_get_score_word_bits(y) & (1<<x)){//draw a pixel if bit is a 1
				 framePointer[(GLOBALS_SCORE_LIVES_Y_POS+(GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + GLOBALS_SCORE_WORD_X_POS + (GLOBALS_DOUBLE*(STATS_COLUMNS-x))] = GLOBALS_WHITE;//draw 4 white pixels per bit
				 framePointer[(GLOBALS_SCORE_LIVES_Y_POS+(GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + GLOBALS_SCORE_WORD_X_POS + (GLOBALS_DOUBLE*(STATS_COLUMNS-x))] = GLOBALS_WHITE;//draw 4 white pixels per bit
				 framePointer[(GLOBALS_SCORE_LIVES_Y_POS+(GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + GLOBALS_SCORE_WORD_X_POS + (GLOBALS_DOUBLE*(STATS_COLUMNS-x))+1] = GLOBALS_WHITE;//draw 4 white pixels per bit
				 framePointer[(GLOBALS_SCORE_LIVES_Y_POS+(GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + GLOBALS_SCORE_WORD_X_POS + (GLOBALS_DOUBLE*(STATS_COLUMNS-x))+1] = GLOBALS_WHITE;//draw 4 white pixels per bit
			 }
		}
    }
	for(i=0; i<STATS_ROWS; i++){
		score_num_to_use[i] = stats0_4x5[i];//set dummy array equal to bits in stats0_4x5
	}
    stats_draw_score_number(framePointer,GLOBALS_SCORE_NUMBER_X_POS_THOUS, GLOBALS_SCORE_LIVES_Y_POS);//initialize with score of "0"
}

//draw the number value of the player score. pass in x position to correctly position each digit
void stats_draw_score_number(unsigned int * framePointer, int16_t digit_x_pos, uint8_t digit_y_pos)
{
	int8_t x, y; //Iterators
    //loop to draw the number zero initially
    for(y=0; y < STATS_ROWS; y++){//iterate through all rows
		for(x=STATS_NEW_COLUMNS; x >= 0; x--){//iterate through all col
			 if(stats_get_score_num_to_use_bits(y) & (1<<x)){//draw a pixel if bit is a 1
				 framePointer[(digit_y_pos+(GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + digit_x_pos + (GLOBALS_DOUBLE*(STATS_NEW_COLUMNS-x))] = GLOBALS_GREEN;//draw 4 green pixels per bit
				 framePointer[(digit_y_pos+(GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + digit_x_pos + (GLOBALS_DOUBLE*(STATS_NEW_COLUMNS-x))] = GLOBALS_GREEN;//draw 4 green pixels per bit
				 framePointer[(digit_y_pos+(GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + digit_x_pos + (GLOBALS_DOUBLE*(STATS_NEW_COLUMNS-x))+1] = GLOBALS_GREEN;//draw 4 green pixels per bit
				 framePointer[(digit_y_pos+(GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + digit_x_pos + (GLOBALS_DOUBLE*(STATS_NEW_COLUMNS-x))+1] = GLOBALS_GREEN;//draw 4 green pixels per bit
			 }
		}
    }
}

//erase the number value of the player score. pass in x position to correctly position each digit
void stats_erase_score_number(unsigned int * framePointer, int16_t digit_x_pos, uint8_t digit_y_pos)
{
	int8_t x, y; //Iterators
    //loop to draw the number zero initially
    for(y=0; y < STATS_ROWS; y++){//iterate through all rows
		for(x=STATS_NEW_COLUMNS; x >= 0; x--){//iterate through all col
			 if(stats_get_score_num_to_use_bits(y) & (1<<x)){//draw a pixel if bit is a 1
				 framePointer[(digit_y_pos+(GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + digit_x_pos + (GLOBALS_DOUBLE*(STATS_NEW_COLUMNS-x))] = GLOBALS_BLACK;//draw 4 green pixels per bit
				 framePointer[(digit_y_pos+(GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + digit_x_pos + (GLOBALS_DOUBLE*(STATS_NEW_COLUMNS-x))] = GLOBALS_BLACK;//draw 4 green pixels per bit
				 framePointer[(digit_y_pos+(GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + digit_x_pos + (GLOBALS_DOUBLE*(STATS_NEW_COLUMNS-x))+1] = GLOBALS_BLACK;//draw 4 green pixels per bit
				 framePointer[(digit_y_pos+(GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + digit_x_pos + (GLOBALS_DOUBLE*(STATS_NEW_COLUMNS-x))+1] = GLOBALS_BLACK;//draw 4 green pixels per bit
			 }
		}
    }
}

//draws the word "lives" as a single sprite
void stats_draw_lives_word(unsigned int * framePointer)
{
    int8_t x, y; //Iterators
    for(y=0; y < STATS_ROWS; y++){//iterate through all rows
		for(x=STATS_LIVES_COLUMNS-1; x >= 0; x--){//iterate through all col
			 if(stats_get_lives_word_bits(y) & (1<<x)){//draw a pixel if bit is a 1
				 framePointer[(GLOBALS_SCORE_LIVES_Y_POS+(GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + GLOBALS_LIVES_WORD_X_POS + (GLOBALS_DOUBLE*(STATS_LIVES_COLUMNS-x))] = GLOBALS_WHITE;//draw 4 white pixels per bit
				 framePointer[(GLOBALS_SCORE_LIVES_Y_POS+(GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + GLOBALS_LIVES_WORD_X_POS + (GLOBALS_DOUBLE*(STATS_LIVES_COLUMNS-x))] = GLOBALS_WHITE;//draw 4 white pixels per bit
				 framePointer[(GLOBALS_SCORE_LIVES_Y_POS+(GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + GLOBALS_LIVES_WORD_X_POS + (GLOBALS_DOUBLE*(STATS_LIVES_COLUMNS-x))+1] = GLOBALS_WHITE;//draw 4 white pixels per bit
				 framePointer[(GLOBALS_SCORE_LIVES_Y_POS+(GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + GLOBALS_LIVES_WORD_X_POS + (GLOBALS_DOUBLE*(STATS_LIVES_COLUMNS-x))+1] = GLOBALS_WHITE;//draw 4 white pixels per bit
			 }
		}
    }
}

//draws the tank representing a life depending on which one you specify
void stats_draw_lives_tank(unsigned int * framePointer, uint8_t tank_number)
{
	uint16_t lives_tank_x_pos = 0;//variable to be set to correct tank life x_pos to draw
	//switch statement to determine which tank life to draw
	switch(tank_number)
	{
		case 1://left tank life
			lives_tank_x_pos = GLOBALS_LIVES_TANK1_X_POS;//change x pos depending on which tank life to draw (left)
			break;
		case 2://middle tank life
			lives_tank_x_pos = GLOBALS_LIVES_TANK2_X_POS;//change x pos depending on which tank life to draw (middle)
			break;
		case 3://right tank life
			lives_tank_x_pos = GLOBALS_LIVES_TANK3_X_POS;//change x pos depending on tank life to draw (right)
			break;
		default:
			break;
	}

    uint8_t x, y; //Iterators
    for(y=0; y < STATS_TANK_ROWS; y++){//iterate through all rows
		for(x=0; x < STATS_TANK_COLUMNS; x++){//iterate through all col
			 if(tank_get_tank_bits(y) & (1<<x)){//draw a pixel if bit is a one
				 framePointer[(GLOBALS_LIVES_TANK_Y_POS+(GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + lives_tank_x_pos + (GLOBALS_DOUBLE*x)] = GLOBALS_GREEN;//draw 4 green pixels per bit
				 framePointer[(GLOBALS_LIVES_TANK_Y_POS+(GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + lives_tank_x_pos + (GLOBALS_DOUBLE*x)] = GLOBALS_GREEN;//draw 4 green pixels
				 framePointer[(GLOBALS_LIVES_TANK_Y_POS+(GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + lives_tank_x_pos + (GLOBALS_DOUBLE*x)+1] = GLOBALS_GREEN;//draw 4 green pixels
				 framePointer[(GLOBALS_LIVES_TANK_Y_POS+(GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + lives_tank_x_pos + (GLOBALS_DOUBLE*x)+1] = GLOBALS_GREEN;//draw 4 green pixels
			 }
		}
    }
}

//erase the tank life depending on which one you specify
void stats_erase_lives_tank(unsigned int * framePointer, uint8_t tank_number)
{
	uint16_t lives_tank_x_pos = 0;//variable to be set to correct tank life x_pos to draw
	//switch statement to determine which tank life to erase
	switch(tank_number)
	{
		case 1://left tank life
			lives_tank_x_pos = GLOBALS_LIVES_TANK1_X_POS;//change x pos depending on which tank life to erase (left)
			break;
		case 2://middle tank life
			lives_tank_x_pos = GLOBALS_LIVES_TANK2_X_POS;//change x pos depending on which tank life to erase (middle)
			break;
		case 3://right tank life
			lives_tank_x_pos = GLOBALS_LIVES_TANK3_X_POS;//change x pos depending on which tank life to erase (right)
			break;
		default:
			break;
	}

    uint8_t x, y; //Iterators
    for(y=0; y < STATS_TANK_ROWS; y++){//iterate through all rows
		for(x=0; x < STATS_TANK_COLUMNS; x++){//iterate through all col
			 if(tank_get_tank_bits(y) & (1<<x)){//draw a pixel if bit is a 1
				 framePointer[(GLOBALS_LIVES_TANK_Y_POS+(GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + lives_tank_x_pos + (GLOBALS_DOUBLE*x)] = GLOBALS_BLACK;//draw 4 black pixels per bit
				 framePointer[(GLOBALS_LIVES_TANK_Y_POS+(GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + lives_tank_x_pos + (GLOBALS_DOUBLE*x)] = GLOBALS_BLACK;//draw 4 black pixels per bit
				 framePointer[(GLOBALS_LIVES_TANK_Y_POS+(GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + lives_tank_x_pos + (GLOBALS_DOUBLE*x)+1] = GLOBALS_BLACK;//draw 4 black pixels per bit
				 framePointer[(GLOBALS_LIVES_TANK_Y_POS+(GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + lives_tank_x_pos + (GLOBALS_DOUBLE*x)+1] = GLOBALS_BLACK;//draw 4 black pixels per bit
			 }
		}
    }
}


//this function is used to find the appropriate sprite to draw based on the integer value of points_score variable
//its is called once at a time for each digit (ones, tens, hunds, thous) in the integer of points_score variable
void stats_calculate_score_num(uint8_t digit)
{
	uint8_t i; //Iterator
	switch(digit)
	{
		case 0:
			for(i=0; i<STATS_ROWS; i++){//do it for all 5 rows of sprite aka all 5 indices of stats0_4x5 array
				score_num_to_use[i] = stats0_4x5[i];//set dummy array equal to bits in stats0_4x5
			}
			break;
		case 1:
			for(i=0; i<STATS_ROWS; i++){//do it for all 5 rows of sprite aka all 5 indices of stats1_4x5 array
				score_num_to_use[i] = stats1_4x5[i];//set dummy array equal to bits in stats1_4x5
			}
			break;
		case 2:
			for(i=0; i<STATS_ROWS; i++){//do it for all 5 rows of sprite aka all 5 indices of stats2_4x5 array
				score_num_to_use[i] = stats2_4x5[i];//set dummy array equal to bits in stats2_4x5
			}
			break;
		case 3:
			for(i=0; i<STATS_ROWS; i++){//do it for all 5 rows of sprite aka all 5 indices of stats3_4x5 array
				score_num_to_use[i] = stats3_4x5[i];//set dummy array equal to bits in stats3_4x5
			}
			break;
		case 4:
			for(i=0; i<STATS_ROWS; i++){//do it for all 5 rows of sprite aka all 5 indices of stats4_4x5 array
				score_num_to_use[i] = stats4_4x5[i];//set dummy array equal to bits in stats4_4x5
			}
			break;
		case 5:
			for(i=0; i<STATS_ROWS; i++){//do it for all 5 rows of sprite aka all 5 indices of stats5_4x5 array
				score_num_to_use[i] = stats5_4x5[i];//set dummy array equal to bits in stats5_4x5
			}
			break;
		case 6:
			for(i=0; i<STATS_ROWS; i++){//do it for all 5 rows of sprite aka all 5 indices of stats6_4x5 array
				score_num_to_use[i] = stats6_4x5[i];//set dummy array equal to bits in stats6_4x5
			}
			break;
		case 7:
			for(i=0; i<STATS_ROWS; i++){//do it for all 5 rows of sprite aka all 5 indices of stats7_4x5 array
				score_num_to_use[i] = stats7_4x5[i];//set dummy array equal to bits in stats7_4x5
			}
			break;
		case 8:
			for(i=0; i<STATS_ROWS; i++){//do it for all 5 rows of sprite aka all 5 indices of stats8_4x5 array
				score_num_to_use[i] = stats8_4x5[i];//set dummy array equal to bits in stats8_4x5
			}
			break;
		case 9:
			for(i=0; i<STATS_ROWS; i++){//do it for all 5 rows of sprite aka all 5 indices of stats9_4x5 array
				score_num_to_use[i] = stats9_4x5[i];//set dummy array equal to bits in stats9_4x5
			}
			break;
		default:
			break;
	}
}


//this function checks which iteration for the for loop you are in from the stats_update_score()
//the iteration number corresponds to whether you are looking at ones, tens, hunds, thous, place and it
//goes through in that order.
//based on which iteration number (placeholder) the positioning of the digit is calculated
//when you call this from stats_update_score() function you must use a current_score
//that wont change value from iteration to iteration of the for loop in stats_update_score()
int16_t stats_calculate_score_digit_pos(uint8_t iteration, int16_t current_score)
{
	int16_t x_pos = 0;//use this to save the proper x position of the digit you want to draw
	switch(iteration)
	{
		//ones placeholder
		case 0:
			if(current_score >= 0 && current_score < STATS_TENS_PLACE){//current score is 0-9
				x_pos = GLOBALS_SCORE_NUMBER_X_POS_THOUS;//put digit at thousands place
			}
			if(current_score >= STATS_TENS_PLACE && current_score < STATS_HUNDS_PLACE){//current score is 10-99
				x_pos = GLOBALS_SCORE_NUMBER_X_POS_HUNDS;//put digit at hundreds place
			}
			if(current_score >= STATS_HUNDS_PLACE && current_score < STATS_THOUS_PLACE){//current score is 100-999
				x_pos = GLOBALS_SCORE_NUMBER_X_POS_TENS;//put digit at tens place
			}
			if(current_score >= STATS_THOUS_PLACE){//current score is greater than 1000
				x_pos = GLOBALS_SCORE_NUMBER_X_POS_ONES;//put digit at ones place
			}
			break;

		//tens placeholder
		case 1:
			if(current_score < STATS_TENS_PLACE){//current score is less than 10
				x_pos = 700;//set it to 700 as a flag this is a leading zero that you shouldnt draw
			}
			else if(current_score >= STATS_TENS_PLACE && current_score < STATS_HUNDS_PLACE){//current score is 10-99
				x_pos = GLOBALS_SCORE_NUMBER_X_POS_THOUS;//put digit at thousands place
			}
			else if(current_score >= STATS_HUNDS_PLACE && current_score < STATS_THOUS_PLACE){//current score is 100-999
				x_pos = GLOBALS_SCORE_NUMBER_X_POS_HUNDS;//put digit at hundreds place
			}
			else if(current_score >= STATS_THOUS_PLACE){//current score is greater than 1000
				x_pos = GLOBALS_SCORE_NUMBER_X_POS_TENS;//put digit at tens place
			}
			break;

		//hundreds placeholder
		case 2:
			if(current_score < STATS_HUNDS_PLACE){
				x_pos = 700;//set it to 700 as a flag this is a leading zero that you shouldnt draw
			}
			if(current_score >= STATS_HUNDS_PLACE && current_score < STATS_THOUS_PLACE){//current score is 100-999
				x_pos = GLOBALS_SCORE_NUMBER_X_POS_THOUS;//put digit at thousands place
			}
			else if(current_score >= STATS_THOUS_PLACE){//current score is greater than 1000
				x_pos = GLOBALS_SCORE_NUMBER_X_POS_HUNDS;//put digit at hundreds place
			}
			break;

		//thousands placeholder
		case 3:
			if(current_score < STATS_THOUS_PLACE){
				x_pos = 700;//set it to 700 as a flag this is a leading zero that you shouldnt draw
			}
			else if(current_score >= STATS_THOUS_PLACE){//current score is greater than 1000
				x_pos = GLOBALS_SCORE_NUMBER_X_POS_THOUS;//put digit at thousands place
			}
			break;
		default:
			break;
	}
	return x_pos;
}

//this is a top level function which calls all necessary functions to erase the score
//you specify the y position.
//at_saucer is a boolean -> 1=draw score where the saucer is. 0=draw score at regular place
void stats_top_level_erase_score(unsigned int * framePointer, uint8_t y_pos, int8_t at_saucer){
	uint16_t temp_prev_score = prev_points_score;//use as a dummy variable to hold value of prev_points_score
	int8_t i;//to be used in for loops

	//erase the previously drawn score
	for (i=0; i<STATS_MAX_NUM_DIGITS; i++) {//go thru 4 times : ones, tens, hundreds, thousands. and in that order
		uint8_t digit = prev_points_score % STATS_DIGIT_PLACE_OP;//get the remainder of digit aka the value in the smallest placeholder (ones)
		stats_calculate_score_num(digit);//this saves the sprite of the digit to be drawn

		int16_t x_pos = stats_calculate_score_digit_pos(i, temp_prev_score);//calc the x pos of digit. use temp_prev_score because this value doesnt change through loop
		if(x_pos == 700){//if x_pos = 700 it means you have a leading zero you shouldnt draw
			continue;//skip to next iteration cuz you dont want to draw anything
		}
		if(at_saucer == 1){//if you are drawing the score where the saucer is
			x_pos = x_pos - (GLOBALS_SCORE_NUMBER_X_POS_THOUS) + saucer_get_saucer_x_position();//adjust the calculated x_pos to be where the saucer is
			stats_erase_score_number(framePointer, x_pos, y_pos);//draw the score where the saucer is
		}
		else{//draw in regular place
			stats_erase_score_number(framePointer, x_pos, y_pos);//draw the score in the regular place
		}
		prev_points_score = prev_points_score / STATS_DIGIT_PLACE_OP;//divide by ten to move from ones to tens and from tens to hundreds placeholder etc
	}
}

//this is a top level function which calls all necessary functions to draw the score
//you specify the y position.
//at_saucer is a boolean -> 1=draw score where the saucer is. 0=draw score at regular place
void stats_top_level_draw_score(unsigned int * framePointer, uint8_t y_pos, int8_t at_saucer){
	int8_t i;//to be used in for loops

	//draws the updated score
	for (i=0; i<STATS_MAX_NUM_DIGITS; i++) {//go thru 4 times : ones, tens, hundreds, thousands. and in that order
		uint8_t digit = points_score % STATS_DIGIT_PLACE_OP;//get the remainder of digit aka the value in the smallest placeholder (ones)
		stats_calculate_score_num(digit);//this saves the sprite of the digit to be drawn

		int16_t x_pos = stats_calculate_score_digit_pos(i, prev_points_score);//calc the x pos of digit. use prev_points_score because this value doesnt change through loop
		if(x_pos == 700){//if x_pos = 700 it means you have a leading zero you shouldnt draw
			continue;//skip to next iteration cuz you dont want to draw anything
		}
		if(at_saucer == 1){//if you are drawing the score where the saucer is
			x_pos = x_pos - (GLOBALS_SCORE_NUMBER_X_POS_THOUS) + saucer_get_saucer_x_position();//adjust the calculated x_pos to be where the saucer is
			stats_draw_score_number(framePointer, x_pos, y_pos);//draw the score where the saucer is
		}
		else{//draw in regular place
			stats_draw_score_number(framePointer, x_pos, y_pos);//draw the score in the regular place
		}
		points_score = points_score / STATS_DIGIT_PLACE_OP;//divide by ten to move from ones to tens and from tens to hundreds placeholder etc
	}
}

//update the score by erasing the old one and drawing a new one
//functions to calculate digit number and digit positioning are called herein
void stats_update_score(unsigned int * framePointer)
{
	stats_top_level_erase_score(framePointer, GLOBALS_SCORE_LIVES_Y_POS, 0);//erase the old score
	prev_points_score = points_score;//change value of previous score to the new score

	stats_top_level_draw_score(framePointer, GLOBALS_SCORE_LIVES_Y_POS, 0);//draw the new score
	points_score = prev_points_score;//set value of new score back to itself after it got changed in the above function
}






>>>>>>> 46133ae2d6a022a717e89ee90e8959352f12048c