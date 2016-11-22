<<<<<<< HEAD
#include "bunker.h"
#include "globals.h"
#include "bullet.h"

#define BUNKER_STATE 4 //Number of states for bunker erosion
#define BUNKER_BLOCKS 10 //Number of blocks in the bunker
#define BUNKER_Y_BITS 18 //Number of bits in y
#define BUNKER_X_BITS 32 //Number of bits in x
#define BUNKER_2_SEPERATION 2
#define BUNKER_3_SEPERATION 3
#define BUNKER_BLOCK_BITS 6 //Number of bits in each block
#define BUNKER_BLOCK0 0
#define BUNKER_BLOCK1 1
#define BUNKER_BLOCK2 2
#define BUNKER_BLOCK3 3
#define BUNKER_BLOCK4 4
#define BUNKER_BLOCK5 5
#define BUNKER_BLOCK6 6
#define BUNKER_BLOCK7 7
#define BUNKER_BLOCK8 8
#define BUNKER_BLOCK9 9
#define BUNKER_NON_BLOCK 10
#define BUNKER_NUM0 0
#define BUNKER_NUM1 1
#define BUNKER_NUM2 2
#define BUNKER_NUM3 3

// Packs each horizontal line of the figures into a single 24 bit word.
#define packword24(b23,b22,b21,b20,b19,b18,b17,b16,b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0) \
((b23 << 23) | (b22 << 22) | (b21 << 21) | (b20 << 20) | (b19 << 19) | (b18 << 18) | (b17 << 17) | (b16 << 16) |  \
 (b15 << 15) | (b14 << 14) | (b13 << 13) | (b12 << 12) | (b11 << 11) | (b10 << 10) | (b9  << 9 ) | (b8  << 8 ) | \
 (b7  << 7 ) | (b6  << 6 ) | (b5  << 5 ) | (b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ) )

// Packs each horizontal line of the figures into a single 6 bit word.
#define packword6(b5,b4,b3,b2,b1,b0) \
((b5  << 5 ) | (b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ) )

// Shape of the entire bunker.
static const uint32_t bunker_24x18[] =
{
	packword24(0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
	packword24(0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0),
	packword24(0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0),
	packword24(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
	packword24(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
	packword24(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
	packword24(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
	packword24(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
	packword24(1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1),
	packword24(1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1),
	packword24(1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1),
	packword24(1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1),
	packword24(1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1),
	packword24(1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1),
	packword24(1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1),
	packword24(1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1),
	packword24(1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1),
	packword24(1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1)
};

// These are the blocks that comprise the bunker and each time a bullet
// strikes one of these blocks, you erode the block as you sequence through
// these patterns.
static const uint8_t bunkerDamage0_6x6[] =
{
	packword6(0,1,1,0,0,0),
	packword6(0,0,0,0,0,1),
	packword6(1,1,0,1,0,0),
	packword6(1,0,0,0,0,0),
	packword6(0,0,1,1,0,0),
	packword6(0,0,0,0,1,0)
};

static const uint8_t bunkerDamage1_6x6[] =
{
	packword6(1,1,1,0,1,0),
	packword6(1,0,1,0,0,1),
	packword6(1,1,0,1,1,1),
	packword6(1,0,0,0,0,0),
	packword6(0,1,1,1,0,1),
	packword6(0,1,1,0,1,0)
};

static const uint8_t bunkerDamage2_6x6[] =
{
	packword6(1,1,1,1,1,1),
	packword6(1,0,1,1,0,1),
	packword6(1,1,0,1,1,1),
	packword6(1,1,0,1,1,0),
	packword6(0,1,1,1,0,1),
	packword6(1,1,1,1,1,1)
};

static const uint8_t bunkerDamage3_6x6[] =
{
	packword6(1,1,1,1,1,1),
	packword6(1,1,1,1,1,1),
	packword6(1,1,1,1,1,1),
	packword6(1,1,1,1,1,1),
	packword6(1,1,1,1,1,1),
	packword6(1,1,1,1,1,1)
};

//static uint8_t bunker_erosion_block_state[BUNKER_STATE] = {0,0,0,0}; //This will keep track of what state of erosion each bunker is in
static uint16_t bunker_erode_y_pos = 0;//keeps track of which section of the bunker to erode
static uint16_t bunker_erode_x_pos = 0;//keeps track of which section of the bunker to erode
static uint8_t bunker_damage_to_use[BUNKER_BLOCK_BITS] = {0,0,0,0,0,0};//keeps track of which bunker damage level to implement
//2x2 matrix to keep track of erosion state for each section of each bunker. 4 bunkers, 10 sections for each bunker
static uint16_t erosion_state[BUNKER_STATE][BUNKER_BLOCKS] = {
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
};

//function to return bunker bits
static uint32_t bunker_get_bunker_bits(uint8_t row)
{
	return bunker_24x18[row];
}

//get the correct erosion state to use depending on how much youre eroded already
static uint8_t bunker_get_damage_to_use_bits(uint8_t row)
{
	return bunker_damage_to_use[row];
}

//function to draw the bunkers
void bunker_draw(unsigned int * framePointer)
{
    uint16_t bunker_x_pos;//tracks position of frame buffer on bunker
    //loop through and draw entire bunker
    for(bunker_x_pos = GLOBALS_LEFT_BUNKER_START_PIXEL; bunker_x_pos < GLOBALS_MAX_X; bunker_x_pos=bunker_x_pos + GLOBALS_BUNKER_PIX_SEPARATION){
        uint8_t x, y; //Iterators
        for(y=0; y < BUNKER_Y_BITS; y++){//all 18 y coordinate pixels
    		for(x=0; x < BUNKER_X_BITS; x++){//all 32 x coordinate pixels
    			 if(bunker_get_bunker_bits(y) & (1<<x)){//draw if the pixel is a 1
    				 framePointer[(GLOBALS_BUNKER_Y_POS+(GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + bunker_x_pos + (GLOBALS_DOUBLE*x)] = GLOBALS_GREEN;//draw green at frame pointer
    				 framePointer[(GLOBALS_BUNKER_Y_POS+(GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + bunker_x_pos + (GLOBALS_DOUBLE*x)] = GLOBALS_GREEN;//draw green at frame pointer
    				 framePointer[(GLOBALS_BUNKER_Y_POS+(GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + bunker_x_pos + (GLOBALS_DOUBLE*x)+1] = GLOBALS_GREEN;//draw green at frame pointer
    				 framePointer[(GLOBALS_BUNKER_Y_POS+(GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + bunker_x_pos + (GLOBALS_DOUBLE*x)+1] = GLOBALS_GREEN;//draw green at frame pointer
    			 }
    		}
        }
    }
}

//calculate which section of bunker to erode
void bunker_get_erosion_block(uint8_t erosion_block)
{
	//there are 10 possibilities of erosion sections
	switch(erosion_block)
	{
		case 0:// top row	far left
			bunker_erode_y_pos = GLOBALS_ERODE_BLOCK_Y_POS_1;//save global var as preset constant for y pos
			bunker_erode_x_pos = GLOBALS_ERODE_BLOCK_X_POS_1;//save global var as preset constant for x pos
			break;
		case 1:// top row	left
			bunker_erode_y_pos = GLOBALS_ERODE_BLOCK_Y_POS_1;//save global var as preset constant for y pos
			bunker_erode_x_pos = GLOBALS_ERODE_BLOCK_X_POS_2;//save global var as preset constant for x pos
			break;
		case 2:// top row right
			bunker_erode_y_pos = GLOBALS_ERODE_BLOCK_Y_POS_1;//save global var as preset constant for y pos
			bunker_erode_x_pos = GLOBALS_ERODE_BLOCK_X_POS_3;//save global var as preset constant for x pos
			break;
		case 3:// top row	far right
			bunker_erode_y_pos = GLOBALS_ERODE_BLOCK_Y_POS_1;//save global var as preset constant for y pos
			bunker_erode_x_pos = GLOBALS_ERODE_BLOCK_X_POS_4;//save global var as preset constant for x pos
			break;
		case 4://middle row		far left
			bunker_erode_y_pos = GLOBALS_ERODE_BLOCK_Y_POS_2;//save global var as preset constant for y pos
			bunker_erode_x_pos = GLOBALS_ERODE_BLOCK_X_POS_1;//save global var as preset constant for x pos
			break;
		case 5://middle row		left
			bunker_erode_y_pos = GLOBALS_ERODE_BLOCK_Y_POS_2;//save global var as preset constant for y pos
			bunker_erode_x_pos = GLOBALS_ERODE_BLOCK_X_POS_2;//save global var as preset constant for x pos
			break;
		case 6://middle row	right
			bunker_erode_y_pos = GLOBALS_ERODE_BLOCK_Y_POS_2;//save global var as preset constant for y pos
			bunker_erode_x_pos = GLOBALS_ERODE_BLOCK_X_POS_3;//save global var as preset constant for x pos
			break;
		case 7://middle row		far right
			bunker_erode_y_pos = GLOBALS_ERODE_BLOCK_Y_POS_2;//save global var as preset constant for y pos
			bunker_erode_x_pos = GLOBALS_ERODE_BLOCK_X_POS_4;//save global var as preset constant for x pos
			break;
		case 8://bottom row		left
			bunker_erode_y_pos = GLOBALS_ERODE_BLOCK_Y_POS_3;//save global var as preset constant for y pos
			bunker_erode_x_pos = GLOBALS_ERODE_BLOCK_X_POS_1;//save global var as preset constant for x pos
			break;
		case 9://bottom row 	right
			bunker_erode_y_pos = GLOBALS_ERODE_BLOCK_Y_POS_3;//save global var as preset constant for y pos
			bunker_erode_x_pos = GLOBALS_ERODE_BLOCK_X_POS_4;//save global var as preset constant for x pos
			break;
		default:
			break;
	}
}

//calculate which bunker(1-4) to erode
void bunker_get_erosion_bunker_num(uint8_t bunker_number)
{
	//four possibilites of different bunkers
	switch(bunker_number)
	{
		case 0:
			bunker_erode_x_pos = bunker_erode_x_pos;//change x pos depending on which bunker to erode (far left)
			break;
		case 1:
			bunker_erode_x_pos = bunker_erode_x_pos + GLOBALS_BUNKER_PIX_SEPARATION;//change x pos depending on which bunker to erode (left)
			break;
		case 2:
			bunker_erode_x_pos = bunker_erode_x_pos + GLOBALS_BUNKER_PIX_SEPARATION*BUNKER_2_SEPERATION;//change x pos depending on which bunker to erode (right)
			break;
		case 3:
			bunker_erode_x_pos = bunker_erode_x_pos + GLOBALS_BUNKER_PIX_SEPARATION*BUNKER_3_SEPERATION;//change x pos depending on which bunker to erode (far right)
			break;
		default:
			break;
	}
}

//calculate what the next state of erosion needs to be
void bunker_get_erosion_state(uint8_t erosion_state)
{
	uint8_t i; //Iterator
	//four possibilites for which erosion state
	switch(erosion_state)
	{
		case 0://first erosion
		{
			for(i=0; i<BUNKER_BLOCK_BITS; i++)
				bunker_damage_to_use[i] = bunkerDamage0_6x6[i];//save bits of correct erosion state
			break;
		}
		case 1://second erosion
		{
			for(i=0; i<BUNKER_BLOCK_BITS; i++)
				bunker_damage_to_use[i] = bunkerDamage1_6x6[i];//save bits of correct erosion state
			break;
		}
		case 2://third erosion state
		{
			for(i=0; i<BUNKER_BLOCK_BITS; i++)
				bunker_damage_to_use[i] = bunkerDamage2_6x6[i];//save bits of correct erosion state
			break;
		}
		case 3://final erosion state
		{
			for(i=0; i<BUNKER_BLOCK_BITS; i++)
				bunker_damage_to_use[i] = bunkerDamage3_6x6[i];//save bits of correct erosion state
			break;
		}
		default:
			break;
	}
}


//draw the appropriate erosion state at correct section and bunker
int8_t bunker_erode(unsigned int * framePointer, uint8_t bunker_number, uint8_t erosion_block)
{
	//uint8_t erosion_block = bunker_erosion_block_state[bunker_number]/BUNKER_STATE;
	//if (erosion_block >= BUNKER_BLOCKS)
	//	return;

	bunker_get_erosion_block(erosion_block);//get correct erosion section
	bunker_get_erosion_bunker_num(bunker_number);//get correct bunker(1-4)
	bunker_get_erosion_state(erosion_state[bunker_number][erosion_block]);//get correct erosion state

    uint8_t x, y; //Iterators
    for(y=0; y < BUNKER_BLOCK_BITS; y++){//go through all 6 y coordinates of erosion block
		for(x=0; x < BUNKER_BLOCK_BITS; x++){//go through all 6 x coordinates of erosion block
			 if(bunker_get_damage_to_use_bits(y) & (1<<x)){//only draw if pixel is a 1
				 framePointer[(bunker_erode_y_pos+(GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + bunker_erode_x_pos + (GLOBALS_DOUBLE*x)] = GLOBALS_BLACK;//draw black on erosion block
				 framePointer[(bunker_erode_y_pos+(GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + bunker_erode_x_pos + (GLOBALS_DOUBLE*x)] = GLOBALS_BLACK;//draw black on erosion block
				 framePointer[(bunker_erode_y_pos+(GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + bunker_erode_x_pos + (GLOBALS_DOUBLE*x)+1] = GLOBALS_BLACK;//draw black on erosion block
				 framePointer[(bunker_erode_y_pos+(GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + bunker_erode_x_pos + (GLOBALS_DOUBLE*x)+1] = GLOBALS_BLACK;//draw black on erosion block
			 }
		}
    }
    erosion_state[bunker_number][erosion_block] = erosion_state[bunker_number][erosion_block] + 1;//increment correct index of 2x2 matrix to get to next erosion state
	//bunker_erosion_block_state[bunker_number] += 1;

    if(erosion_state[bunker_number][erosion_block] <= BUNKER_STATE){//check what erosion state youre at
    	return 1;//return a 1 because the bullet eroded a block
    }
    else{
    	return 0;//return 0 since the bullet already eroded that block completely and needs to be allowed to just pass through
    }
}

//returns which erosion block number (0-9). takes in bullet position.
int16_t bunker_calculate_erosion_block_num(uint16_t bullet_x_pos, uint16_t bullet_y_pos)
{
	int16_t erosion_block = BUNKER_NON_BLOCK;//set to default of 10 since this is not an erosion block

	//checking if intersection will occur on far left column of each bunker
	if(((bullet_x_pos >= GLOBALS_BUNKER_0_LEFT) && (bullet_x_pos < 90+12)) || //far left bunker
	   ((bullet_x_pos >= GLOBALS_BUNKER_1_LEFT) && (bullet_x_pos < 90+138+12)) || //left bunker
	   ((bullet_x_pos >= GLOBALS_BUNKER_2_LEFT) && (bullet_x_pos < 90+138*2+12)) || //right bunker
	   ((bullet_x_pos >= GLOBALS_BUNKER_3_LEFT) && (bullet_x_pos < 90+138*3+12))){ //far right bunker
		if(bullet_y_pos >= GLOBALS_ERODE_BLOCK_Y_POS_1 && bullet_y_pos < GLOBALS_ERODE_BLOCK_Y_POS_2){//top row
			erosion_block = BUNKER_BLOCK0;
		}
		else if(bullet_y_pos >= GLOBALS_ERODE_BLOCK_Y_POS_2 && bullet_y_pos < GLOBALS_ERODE_BLOCK_Y_POS_3){//middle row
			erosion_block = BUNKER_BLOCK4;
		}
		else if(bullet_y_pos >= GLOBALS_ERODE_BLOCK_Y_POS_3 && bullet_y_pos <= GLOBALS_ERODE_BLOCK_Y_POS_4){//bottom row
			erosion_block = BUNKER_BLOCK8;
		}
	}
	//checking if intersection will occur on middle left column of each bunker
	if(((bullet_x_pos >= GLOBALS_BUNKER_0_LEFT+12) && (bullet_x_pos < 90+12*2)) || //far left bunker
	   ((bullet_x_pos >= GLOBALS_BUNKER_1_LEFT+12) && (bullet_x_pos < 90+138+12*2)) || //left bunker
	   ((bullet_x_pos >= GLOBALS_BUNKER_2_LEFT+12) && (bullet_x_pos < 90+138*2+12*2)) || //right bunker
	   ((bullet_x_pos >= GLOBALS_BUNKER_3_LEFT+12) && (bullet_x_pos < 90+138*3+12*2))){ //far right bunker
		if(bullet_y_pos >= GLOBALS_ERODE_BLOCK_Y_POS_1 && bullet_y_pos < GLOBALS_ERODE_BLOCK_Y_POS_2){//top row
			erosion_block = BUNKER_BLOCK1;
		}
		else if(bullet_y_pos >= GLOBALS_ERODE_BLOCK_Y_POS_2 && bullet_y_pos <= GLOBALS_ERODE_BLOCK_Y_POS_3){//bottom row
			erosion_block = BUNKER_BLOCK5;
		}
	}
	//checking if intersection will occur on middle right column of each bunker
	if(((bullet_x_pos >= GLOBALS_BUNKER_0_LEFT+12*2) && (bullet_x_pos < 90+12*3)) || //far left bunker
	   ((bullet_x_pos >= GLOBALS_BUNKER_1_LEFT+12*2) && (bullet_x_pos < 90+138+12*3)) || //left bunker
	   ((bullet_x_pos >= GLOBALS_BUNKER_2_LEFT+12*2) && (bullet_x_pos < 90+138*2+12*3)) || //right bunker
	   ((bullet_x_pos >= GLOBALS_BUNKER_3_LEFT+12*2) && (bullet_x_pos < 90+138*3+12*3))){ //far right bunker
		if(bullet_y_pos >= GLOBALS_ERODE_BLOCK_Y_POS_1 && bullet_y_pos < GLOBALS_ERODE_BLOCK_Y_POS_2){//top row
			erosion_block = BUNKER_BLOCK2;
		}
		else if(bullet_y_pos >= GLOBALS_ERODE_BLOCK_Y_POS_2 && bullet_y_pos <= GLOBALS_ERODE_BLOCK_Y_POS_3){
			erosion_block = BUNKER_BLOCK6;
		}
	}
	//checking if intersection will occur on far right column of each bunker
	if(((bullet_x_pos >= GLOBALS_BUNKER_0_LEFT+12*3) && (bullet_x_pos <= 90+12*4)) || //far left bunker
	   ((bullet_x_pos >= GLOBALS_BUNKER_1_LEFT+12*3) && (bullet_x_pos < 90+138+12*4)) || //left bunker
	   ((bullet_x_pos >= GLOBALS_BUNKER_2_LEFT+12*3) && (bullet_x_pos < 90+138*2+12*4)) || //right bunker
	   ((bullet_x_pos >= GLOBALS_BUNKER_3_LEFT+12*3) && (bullet_x_pos < 90+138*3+12*4))){ //far right bunker
		if(bullet_y_pos >= GLOBALS_ERODE_BLOCK_Y_POS_1 && bullet_y_pos < GLOBALS_ERODE_BLOCK_Y_POS_2){//top row
			erosion_block = BUNKER_BLOCK3;
		}
		else if(bullet_y_pos >= GLOBALS_ERODE_BLOCK_Y_POS_2 && bullet_y_pos < GLOBALS_ERODE_BLOCK_Y_POS_3){//middle row
			erosion_block = BUNKER_BLOCK7;
		}
		else if(bullet_y_pos >= GLOBALS_ERODE_BLOCK_Y_POS_3 && bullet_y_pos <= GLOBALS_ERODE_BLOCK_Y_POS_4){//bottom row
			erosion_block = BUNKER_BLOCK9;
		}
	}
	return erosion_block;//return the erosion block number
}


//return 1 or 0 depending on whether you eroded a bunker or not
//takes bullet position and calls functions to calculate which bunker if any and
//which erosion block the bullet will intersect. if intersection is true bunker_erode() is called
//which will return 1 or 0 if erosion occurred or not (erosion wouldnt occur if already max eroded)
int8_t bunker_check_if_erosion_necessary(unsigned int * framePointer, uint16_t bullet_x_pos, uint16_t bullet_y_pos)
{
	int16_t erosion_block = BUNKER_NON_BLOCK;//use this to track erosion block
	int8_t bunker_number = BUNKER_NON_BLOCK;//use this to track bunker numer

	//BUNKER 0
	if(bullet_x_pos >= GLOBALS_BUNKER_0_LEFT && bullet_x_pos <= GLOBALS_BUNKER_0_RIGHT){//bullet is between farthest left and right pixel of bunker
		erosion_block = bunker_calculate_erosion_block_num(bullet_x_pos, bullet_y_pos);//calculate which erosion block
		bunker_number = BUNKER_NUM0;//set var to appropriate bunker number
		if(erosion_block != BUNKER_NON_BLOCK){//only call erosion if erosion_block != 10 which is the default
			return bunker_erode(framePointer, bunker_number, erosion_block);//return 1 or 0 depending on whether you eroded a bunker or not
		}
		else{
			return 0;//you did not erode a bunker so allow bullet to continue
		}
	}
	//BUNKER 1
	else if(bullet_x_pos >= GLOBALS_BUNKER_1_LEFT && bullet_x_pos <= GLOBALS_BUNKER_1_RIGHT){//bullet is between farthest left and right pixel of bunker
		erosion_block = bunker_calculate_erosion_block_num(bullet_x_pos, bullet_y_pos);//calculate which erosion block
		bunker_number = BUNKER_NUM1;//set var to appropriate bunker number
		if(erosion_block != BUNKER_NON_BLOCK){//only call erosion if erosion_block != 10 which is the default
			return bunker_erode(framePointer, bunker_number, erosion_block);//return 1 or 0 depending on whether you eroded a bunker or not
		}
		else{
			return 0;//you did not erode a bunker so allow bullet to continue
		}
	}
	//BUNKER 2
	else if(bullet_x_pos >= GLOBALS_BUNKER_2_LEFT && bullet_x_pos <= GLOBALS_BUNKER_2_RIGHT){//bullet is between farthest left and right pixel of bunker
		erosion_block = bunker_calculate_erosion_block_num(bullet_x_pos, bullet_y_pos);//calculate which erosion block
		bunker_number = BUNKER_NUM2;//set var to appropriate bunker number
		if(erosion_block != BUNKER_NON_BLOCK){//only call erosion if erosion_block != 10 which is the default
			return bunker_erode(framePointer, bunker_number, erosion_block);//return 1 or 0 depending on whether you eroded a bunker or not
		}
		else{
			return 0;//you did not erode a bunker so allow bullet to continue
		}
	}
	//BUNKER 3
	else if(bullet_x_pos >= GLOBALS_BUNKER_3_LEFT && bullet_x_pos <= GLOBALS_BUNKER_3_RIGHT){//bullet is between farthest left and right pixel of bunker
		erosion_block = bunker_calculate_erosion_block_num(bullet_x_pos, bullet_y_pos);//calculate which erosion block
		bunker_number = BUNKER_NUM3;//set var to appropriate bunker number
		if(erosion_block != BUNKER_NON_BLOCK){//only call erosion if erosion_block != 10 which is the default
			return bunker_erode(framePointer, bunker_number, erosion_block);//return 1 or 0 depending on whether you eroded a bunker or not
		}
		else{
			return 0;//you did not erode a bunker so allow bullet to continue
		}
	}
	else{
		return 0;//you did not erode a bunker (bullet missed them) so allow bullet to continue
	}
}
=======
#include "bunker.h"
#include "globals.h"
#include "bullet.h"

#define BUNKER_STATE 4 //Number of states for bunker erosion
#define BUNKER_BLOCKS 10 //Number of blocks in the bunker
#define BUNKER_Y_BITS 18 //Number of bits in y
#define BUNKER_X_BITS 32 //Number of bits in x
#define BUNKER_2_SEPERATION 2
#define BUNKER_3_SEPERATION 3
#define BUNKER_BLOCK_BITS 6 //Number of bits in each block
#define BUNKER_BLOCK0 0
#define BUNKER_BLOCK1 1
#define BUNKER_BLOCK2 2
#define BUNKER_BLOCK3 3
#define BUNKER_BLOCK4 4
#define BUNKER_BLOCK5 5
#define BUNKER_BLOCK6 6
#define BUNKER_BLOCK7 7
#define BUNKER_BLOCK8 8
#define BUNKER_BLOCK9 9
#define BUNKER_NON_BLOCK 10
#define BUNKER_NUM0 0
#define BUNKER_NUM1 1
#define BUNKER_NUM2 2
#define BUNKER_NUM3 3

// Packs each horizontal line of the figures into a single 24 bit word.
#define packword24(b23,b22,b21,b20,b19,b18,b17,b16,b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0) \
((b23 << 23) | (b22 << 22) | (b21 << 21) | (b20 << 20) | (b19 << 19) | (b18 << 18) | (b17 << 17) | (b16 << 16) |  \
 (b15 << 15) | (b14 << 14) | (b13 << 13) | (b12 << 12) | (b11 << 11) | (b10 << 10) | (b9  << 9 ) | (b8  << 8 ) | \
 (b7  << 7 ) | (b6  << 6 ) | (b5  << 5 ) | (b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ) )

// Packs each horizontal line of the figures into a single 6 bit word.
#define packword6(b5,b4,b3,b2,b1,b0) \
((b5  << 5 ) | (b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ) )

// Shape of the entire bunker.
static const uint32_t bunker_24x18[] =
{
	packword24(0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
	packword24(0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0),
	packword24(0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0),
	packword24(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
	packword24(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
	packword24(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
	packword24(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
	packword24(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
	packword24(1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1),
	packword24(1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1),
	packword24(1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1),
	packword24(1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1),
	packword24(1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1),
	packword24(1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1),
	packword24(1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1),
	packword24(1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1),
	packword24(1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1),
	packword24(1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1)
};

// These are the blocks that comprise the bunker and each time a bullet
// strikes one of these blocks, you erode the block as you sequence through
// these patterns.
static const uint8_t bunkerDamage0_6x6[] =
{
	packword6(0,1,1,0,0,0),
	packword6(0,0,0,0,0,1),
	packword6(1,1,0,1,0,0),
	packword6(1,0,0,0,0,0),
	packword6(0,0,1,1,0,0),
	packword6(0,0,0,0,1,0)
};

static const uint8_t bunkerDamage1_6x6[] =
{
	packword6(1,1,1,0,1,0),
	packword6(1,0,1,0,0,1),
	packword6(1,1,0,1,1,1),
	packword6(1,0,0,0,0,0),
	packword6(0,1,1,1,0,1),
	packword6(0,1,1,0,1,0)
};

static const uint8_t bunkerDamage2_6x6[] =
{
	packword6(1,1,1,1,1,1),
	packword6(1,0,1,1,0,1),
	packword6(1,1,0,1,1,1),
	packword6(1,1,0,1,1,0),
	packword6(0,1,1,1,0,1),
	packword6(1,1,1,1,1,1)
};

static const uint8_t bunkerDamage3_6x6[] =
{
	packword6(1,1,1,1,1,1),
	packword6(1,1,1,1,1,1),
	packword6(1,1,1,1,1,1),
	packword6(1,1,1,1,1,1),
	packword6(1,1,1,1,1,1),
	packword6(1,1,1,1,1,1)
};

//static uint8_t bunker_erosion_block_state[BUNKER_STATE] = {0,0,0,0}; //This will keep track of what state of erosion each bunker is in
static uint16_t bunker_erode_y_pos = 0;//keeps track of which section of the bunker to erode
static uint16_t bunker_erode_x_pos = 0;//keeps track of which section of the bunker to erode
static uint8_t bunker_damage_to_use[BUNKER_BLOCK_BITS] = {0,0,0,0,0,0};//keeps track of which bunker damage level to implement
//2x2 matrix to keep track of erosion state for each section of each bunker. 4 bunkers, 10 sections for each bunker
static uint16_t erosion_state[BUNKER_STATE][BUNKER_BLOCKS] = {
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
};

//function to return bunker bits
static uint32_t bunker_get_bunker_bits(uint8_t row)
{
	return bunker_24x18[row];
}

//get the correct erosion state to use depending on how much youre eroded already
static uint8_t bunker_get_damage_to_use_bits(uint8_t row)
{
	return bunker_damage_to_use[row];
}

//function to draw the bunkers
void bunker_draw(unsigned int * framePointer)
{
    uint16_t bunker_x_pos;//tracks position of frame buffer on bunker
    //loop through and draw entire bunker
    for(bunker_x_pos = GLOBALS_LEFT_BUNKER_START_PIXEL; bunker_x_pos < GLOBALS_MAX_X; bunker_x_pos=bunker_x_pos + GLOBALS_BUNKER_PIX_SEPARATION){
        uint8_t x, y; //Iterators
        for(y=0; y < BUNKER_Y_BITS; y++){//all 18 y coordinate pixels
    		for(x=0; x < BUNKER_X_BITS; x++){//all 32 x coordinate pixels
    			 if(bunker_get_bunker_bits(y) & (1<<x)){//draw if the pixel is a 1
    				 framePointer[(GLOBALS_BUNKER_Y_POS+(GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + bunker_x_pos + (GLOBALS_DOUBLE*x)] = GLOBALS_GREEN;//draw green at frame pointer
    				 framePointer[(GLOBALS_BUNKER_Y_POS+(GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + bunker_x_pos + (GLOBALS_DOUBLE*x)] = GLOBALS_GREEN;//draw green at frame pointer
    				 framePointer[(GLOBALS_BUNKER_Y_POS+(GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + bunker_x_pos + (GLOBALS_DOUBLE*x)+1] = GLOBALS_GREEN;//draw green at frame pointer
    				 framePointer[(GLOBALS_BUNKER_Y_POS+(GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + bunker_x_pos + (GLOBALS_DOUBLE*x)+1] = GLOBALS_GREEN;//draw green at frame pointer
    			 }
    		}
        }
    }
}

//calculate which section of bunker to erode
void bunker_get_erosion_block(uint8_t erosion_block)
{
	//there are 10 possibilities of erosion sections
	switch(erosion_block)
	{
		case 0:// top row	far left
			bunker_erode_y_pos = GLOBALS_ERODE_BLOCK_Y_POS_1;//save global var as preset constant for y pos
			bunker_erode_x_pos = GLOBALS_ERODE_BLOCK_X_POS_1;//save global var as preset constant for x pos
			break;
		case 1:// top row	left
			bunker_erode_y_pos = GLOBALS_ERODE_BLOCK_Y_POS_1;//save global var as preset constant for y pos
			bunker_erode_x_pos = GLOBALS_ERODE_BLOCK_X_POS_2;//save global var as preset constant for x pos
			break;
		case 2:// top row right
			bunker_erode_y_pos = GLOBALS_ERODE_BLOCK_Y_POS_1;//save global var as preset constant for y pos
			bunker_erode_x_pos = GLOBALS_ERODE_BLOCK_X_POS_3;//save global var as preset constant for x pos
			break;
		case 3:// top row	far right
			bunker_erode_y_pos = GLOBALS_ERODE_BLOCK_Y_POS_1;//save global var as preset constant for y pos
			bunker_erode_x_pos = GLOBALS_ERODE_BLOCK_X_POS_4;//save global var as preset constant for x pos
			break;
		case 4://middle row		far left
			bunker_erode_y_pos = GLOBALS_ERODE_BLOCK_Y_POS_2;//save global var as preset constant for y pos
			bunker_erode_x_pos = GLOBALS_ERODE_BLOCK_X_POS_1;//save global var as preset constant for x pos
			break;
		case 5://middle row		left
			bunker_erode_y_pos = GLOBALS_ERODE_BLOCK_Y_POS_2;//save global var as preset constant for y pos
			bunker_erode_x_pos = GLOBALS_ERODE_BLOCK_X_POS_2;//save global var as preset constant for x pos
			break;
		case 6://middle row	right
			bunker_erode_y_pos = GLOBALS_ERODE_BLOCK_Y_POS_2;//save global var as preset constant for y pos
			bunker_erode_x_pos = GLOBALS_ERODE_BLOCK_X_POS_3;//save global var as preset constant for x pos
			break;
		case 7://middle row		far right
			bunker_erode_y_pos = GLOBALS_ERODE_BLOCK_Y_POS_2;//save global var as preset constant for y pos
			bunker_erode_x_pos = GLOBALS_ERODE_BLOCK_X_POS_4;//save global var as preset constant for x pos
			break;
		case 8://bottom row		left
			bunker_erode_y_pos = GLOBALS_ERODE_BLOCK_Y_POS_3;//save global var as preset constant for y pos
			bunker_erode_x_pos = GLOBALS_ERODE_BLOCK_X_POS_1;//save global var as preset constant for x pos
			break;
		case 9://bottom row 	right
			bunker_erode_y_pos = GLOBALS_ERODE_BLOCK_Y_POS_3;//save global var as preset constant for y pos
			bunker_erode_x_pos = GLOBALS_ERODE_BLOCK_X_POS_4;//save global var as preset constant for x pos
			break;
		default:
			break;
	}
}

//calculate which bunker(1-4) to erode
void bunker_get_erosion_bunker_num(uint8_t bunker_number)
{
	//four possibilites of different bunkers
	switch(bunker_number)
	{
		case 0:
			bunker_erode_x_pos = bunker_erode_x_pos;//change x pos depending on which bunker to erode (far left)
			break;
		case 1:
			bunker_erode_x_pos = bunker_erode_x_pos + GLOBALS_BUNKER_PIX_SEPARATION;//change x pos depending on which bunker to erode (left)
			break;
		case 2:
			bunker_erode_x_pos = bunker_erode_x_pos + GLOBALS_BUNKER_PIX_SEPARATION*BUNKER_2_SEPERATION;//change x pos depending on which bunker to erode (right)
			break;
		case 3:
			bunker_erode_x_pos = bunker_erode_x_pos + GLOBALS_BUNKER_PIX_SEPARATION*BUNKER_3_SEPERATION;//change x pos depending on which bunker to erode (far right)
			break;
		default:
			break;
	}
}

//calculate what the next state of erosion needs to be
void bunker_get_erosion_state(uint8_t erosion_state)
{
	uint8_t i; //Iterator
	//four possibilites for which erosion state
	switch(erosion_state)
	{
		case 0://first erosion
		{
			for(i=0; i<BUNKER_BLOCK_BITS; i++)
				bunker_damage_to_use[i] = bunkerDamage0_6x6[i];//save bits of correct erosion state
			break;
		}
		case 1://second erosion
		{
			for(i=0; i<BUNKER_BLOCK_BITS; i++)
				bunker_damage_to_use[i] = bunkerDamage1_6x6[i];//save bits of correct erosion state
			break;
		}
		case 2://third erosion state
		{
			for(i=0; i<BUNKER_BLOCK_BITS; i++)
				bunker_damage_to_use[i] = bunkerDamage2_6x6[i];//save bits of correct erosion state
			break;
		}
		case 3://final erosion state
		{
			for(i=0; i<BUNKER_BLOCK_BITS; i++)
				bunker_damage_to_use[i] = bunkerDamage3_6x6[i];//save bits of correct erosion state
			break;
		}
		default:
			break;
	}
}


//draw the appropriate erosion state at correct section and bunker
int8_t bunker_erode(unsigned int * framePointer, uint8_t bunker_number, uint8_t erosion_block)
{
	//uint8_t erosion_block = bunker_erosion_block_state[bunker_number]/BUNKER_STATE;
	//if (erosion_block >= BUNKER_BLOCKS)
	//	return;

	bunker_get_erosion_block(erosion_block);//get correct erosion section
	bunker_get_erosion_bunker_num(bunker_number);//get correct bunker(1-4)
	bunker_get_erosion_state(erosion_state[bunker_number][erosion_block]);//get correct erosion state

    uint8_t x, y; //Iterators
    for(y=0; y < BUNKER_BLOCK_BITS; y++){//go through all 6 y coordinates of erosion block
		for(x=0; x < BUNKER_BLOCK_BITS; x++){//go through all 6 x coordinates of erosion block
			 if(bunker_get_damage_to_use_bits(y) & (1<<x)){//only draw if pixel is a 1
				 framePointer[(bunker_erode_y_pos+(GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + bunker_erode_x_pos + (GLOBALS_DOUBLE*x)] = GLOBALS_BLACK;//draw black on erosion block
				 framePointer[(bunker_erode_y_pos+(GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + bunker_erode_x_pos + (GLOBALS_DOUBLE*x)] = GLOBALS_BLACK;//draw black on erosion block
				 framePointer[(bunker_erode_y_pos+(GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + bunker_erode_x_pos + (GLOBALS_DOUBLE*x)+1] = GLOBALS_BLACK;//draw black on erosion block
				 framePointer[(bunker_erode_y_pos+(GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + bunker_erode_x_pos + (GLOBALS_DOUBLE*x)+1] = GLOBALS_BLACK;//draw black on erosion block
			 }
		}
    }
    erosion_state[bunker_number][erosion_block] = erosion_state[bunker_number][erosion_block] + 1;//increment correct index of 2x2 matrix to get to next erosion state
	//bunker_erosion_block_state[bunker_number] += 1;

    if(erosion_state[bunker_number][erosion_block] <= BUNKER_STATE){//check what erosion state youre at
    	return 1;//return a 1 because the bullet eroded a block
    }
    else{
    	return 0;//return 0 since the bullet already eroded that block completely and needs to be allowed to just pass through
    }
}

//returns which erosion block number (0-9). takes in bullet position.
int16_t bunker_calculate_erosion_block_num(uint16_t bullet_x_pos, uint16_t bullet_y_pos)
{
	int16_t erosion_block = BUNKER_NON_BLOCK;//set to default of 10 since this is not an erosion block

	//checking if intersection will occur on far left column of each bunker
	if(((bullet_x_pos >= GLOBALS_BUNKER_0_LEFT) && (bullet_x_pos < 90+12)) || //far left bunker
	   ((bullet_x_pos >= GLOBALS_BUNKER_1_LEFT) && (bullet_x_pos < 90+138+12)) || //left bunker
	   ((bullet_x_pos >= GLOBALS_BUNKER_2_LEFT) && (bullet_x_pos < 90+138*2+12)) || //right bunker
	   ((bullet_x_pos >= GLOBALS_BUNKER_3_LEFT) && (bullet_x_pos < 90+138*3+12))){ //far right bunker
		if(bullet_y_pos >= GLOBALS_ERODE_BLOCK_Y_POS_1 && bullet_y_pos < GLOBALS_ERODE_BLOCK_Y_POS_2){//top row
			erosion_block = BUNKER_BLOCK0;
		}
		else if(bullet_y_pos >= GLOBALS_ERODE_BLOCK_Y_POS_2 && bullet_y_pos < GLOBALS_ERODE_BLOCK_Y_POS_3){//middle row
			erosion_block = BUNKER_BLOCK4;
		}
		else if(bullet_y_pos >= GLOBALS_ERODE_BLOCK_Y_POS_3 && bullet_y_pos <= GLOBALS_ERODE_BLOCK_Y_POS_4){//bottom row
			erosion_block = BUNKER_BLOCK8;
		}
	}
	//checking if intersection will occur on middle left column of each bunker
	if(((bullet_x_pos >= GLOBALS_BUNKER_0_LEFT+12) && (bullet_x_pos < 90+12*2)) || //far left bunker
	   ((bullet_x_pos >= GLOBALS_BUNKER_1_LEFT+12) && (bullet_x_pos < 90+138+12*2)) || //left bunker
	   ((bullet_x_pos >= GLOBALS_BUNKER_2_LEFT+12) && (bullet_x_pos < 90+138*2+12*2)) || //right bunker
	   ((bullet_x_pos >= GLOBALS_BUNKER_3_LEFT+12) && (bullet_x_pos < 90+138*3+12*2))){ //far right bunker
		if(bullet_y_pos >= GLOBALS_ERODE_BLOCK_Y_POS_1 && bullet_y_pos < GLOBALS_ERODE_BLOCK_Y_POS_2){//top row
			erosion_block = BUNKER_BLOCK1;
		}
		else if(bullet_y_pos >= GLOBALS_ERODE_BLOCK_Y_POS_2 && bullet_y_pos <= GLOBALS_ERODE_BLOCK_Y_POS_3){//bottom row
			erosion_block = BUNKER_BLOCK5;
		}
	}
	//checking if intersection will occur on middle right column of each bunker
	if(((bullet_x_pos >= GLOBALS_BUNKER_0_LEFT+12*2) && (bullet_x_pos < 90+12*3)) || //far left bunker
	   ((bullet_x_pos >= GLOBALS_BUNKER_1_LEFT+12*2) && (bullet_x_pos < 90+138+12*3)) || //left bunker
	   ((bullet_x_pos >= GLOBALS_BUNKER_2_LEFT+12*2) && (bullet_x_pos < 90+138*2+12*3)) || //right bunker
	   ((bullet_x_pos >= GLOBALS_BUNKER_3_LEFT+12*2) && (bullet_x_pos < 90+138*3+12*3))){ //far right bunker
		if(bullet_y_pos >= GLOBALS_ERODE_BLOCK_Y_POS_1 && bullet_y_pos < GLOBALS_ERODE_BLOCK_Y_POS_2){//top row
			erosion_block = BUNKER_BLOCK2;
		}
		else if(bullet_y_pos >= GLOBALS_ERODE_BLOCK_Y_POS_2 && bullet_y_pos <= GLOBALS_ERODE_BLOCK_Y_POS_3){
			erosion_block = BUNKER_BLOCK6;
		}
	}
	//checking if intersection will occur on far right column of each bunker
	if(((bullet_x_pos >= GLOBALS_BUNKER_0_LEFT+12*3) && (bullet_x_pos <= 90+12*4)) || //far left bunker
	   ((bullet_x_pos >= GLOBALS_BUNKER_1_LEFT+12*3) && (bullet_x_pos < 90+138+12*4)) || //left bunker
	   ((bullet_x_pos >= GLOBALS_BUNKER_2_LEFT+12*3) && (bullet_x_pos < 90+138*2+12*4)) || //right bunker
	   ((bullet_x_pos >= GLOBALS_BUNKER_3_LEFT+12*3) && (bullet_x_pos < 90+138*3+12*4))){ //far right bunker
		if(bullet_y_pos >= GLOBALS_ERODE_BLOCK_Y_POS_1 && bullet_y_pos < GLOBALS_ERODE_BLOCK_Y_POS_2){//top row
			erosion_block = BUNKER_BLOCK3;
		}
		else if(bullet_y_pos >= GLOBALS_ERODE_BLOCK_Y_POS_2 && bullet_y_pos < GLOBALS_ERODE_BLOCK_Y_POS_3){//middle row
			erosion_block = BUNKER_BLOCK7;
		}
		else if(bullet_y_pos >= GLOBALS_ERODE_BLOCK_Y_POS_3 && bullet_y_pos <= GLOBALS_ERODE_BLOCK_Y_POS_4){//bottom row
			erosion_block = BUNKER_BLOCK9;
		}
	}
	return erosion_block;//return the erosion block number
}


//return 1 or 0 depending on whether you eroded a bunker or not
//takes bullet position and calls functions to calculate which bunker if any and
//which erosion block the bullet will intersect. if intersection is true bunker_erode() is called
//which will return 1 or 0 if erosion occurred or not (erosion wouldnt occur if already max eroded)
int8_t bunker_check_if_erosion_necessary(unsigned int * framePointer, uint16_t bullet_x_pos, uint16_t bullet_y_pos)
{
	int16_t erosion_block = BUNKER_NON_BLOCK;//use this to track erosion block
	int8_t bunker_number = BUNKER_NON_BLOCK;//use this to track bunker numer

	//BUNKER 0
	if(bullet_x_pos >= GLOBALS_BUNKER_0_LEFT && bullet_x_pos <= GLOBALS_BUNKER_0_RIGHT){//bullet is between farthest left and right pixel of bunker
		erosion_block = bunker_calculate_erosion_block_num(bullet_x_pos, bullet_y_pos);//calculate which erosion block
		bunker_number = BUNKER_NUM0;//set var to appropriate bunker number
		if(erosion_block != BUNKER_NON_BLOCK){//only call erosion if erosion_block != 10 which is the default
			return bunker_erode(framePointer, bunker_number, erosion_block);//return 1 or 0 depending on whether you eroded a bunker or not
		}
		else{
			return 0;//you did not erode a bunker so allow bullet to continue
		}
	}
	//BUNKER 1
	else if(bullet_x_pos >= GLOBALS_BUNKER_1_LEFT && bullet_x_pos <= GLOBALS_BUNKER_1_RIGHT){//bullet is between farthest left and right pixel of bunker
		erosion_block = bunker_calculate_erosion_block_num(bullet_x_pos, bullet_y_pos);//calculate which erosion block
		bunker_number = BUNKER_NUM1;//set var to appropriate bunker number
		if(erosion_block != BUNKER_NON_BLOCK){//only call erosion if erosion_block != 10 which is the default
			return bunker_erode(framePointer, bunker_number, erosion_block);//return 1 or 0 depending on whether you eroded a bunker or not
		}
		else{
			return 0;//you did not erode a bunker so allow bullet to continue
		}
	}
	//BUNKER 2
	else if(bullet_x_pos >= GLOBALS_BUNKER_2_LEFT && bullet_x_pos <= GLOBALS_BUNKER_2_RIGHT){//bullet is between farthest left and right pixel of bunker
		erosion_block = bunker_calculate_erosion_block_num(bullet_x_pos, bullet_y_pos);//calculate which erosion block
		bunker_number = BUNKER_NUM2;//set var to appropriate bunker number
		if(erosion_block != BUNKER_NON_BLOCK){//only call erosion if erosion_block != 10 which is the default
			return bunker_erode(framePointer, bunker_number, erosion_block);//return 1 or 0 depending on whether you eroded a bunker or not
		}
		else{
			return 0;//you did not erode a bunker so allow bullet to continue
		}
	}
	//BUNKER 3
	else if(bullet_x_pos >= GLOBALS_BUNKER_3_LEFT && bullet_x_pos <= GLOBALS_BUNKER_3_RIGHT){//bullet is between farthest left and right pixel of bunker
		erosion_block = bunker_calculate_erosion_block_num(bullet_x_pos, bullet_y_pos);//calculate which erosion block
		bunker_number = BUNKER_NUM3;//set var to appropriate bunker number
		if(erosion_block != BUNKER_NON_BLOCK){//only call erosion if erosion_block != 10 which is the default
			return bunker_erode(framePointer, bunker_number, erosion_block);//return 1 or 0 depending on whether you eroded a bunker or not
		}
		else{
			return 0;//you did not erode a bunker so allow bullet to continue
		}
	}
	else{
		return 0;//you did not erode a bunker (bullet missed them) so allow bullet to continue
	}
}
>>>>>>> 46133ae2d6a022a717e89ee90e8959352f12048c
