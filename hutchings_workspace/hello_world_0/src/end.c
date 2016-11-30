
#include "end.h"
#include "stats.h"
#include "globals.h"

#define END_WORD_ROWS 5
#define END_WORD_COL 22
#define END_WORD_Y_POS 200
#define END_GAME_X_POS 250
#define END_OVER_X_POS 350

#define packword23(b22,b21,b20,b19,b18,b17,b16,b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0) \
((b22 << 22) | (b21 << 21) | (b20 << 20) | (b19 << 19) | (b18 << 18) | (b17 << 17) | (b16 << 16) |  \
 (b15 << 15) | (b14 << 14) | (b13 << 13) | (b12 << 12) | (b11 << 11) | (b10 << 10) | (b9  << 9 ) | (b8  << 8 ) | \
 (b7  << 7 ) | (b6  << 6 ) | (b5  << 5 ) | (b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ) )

//GAME
static const uint32_t stats_game29x5[] =
{
packword23(1,1,1,1,1,0,0,0,1,0,0,0,1,0,0,0,1,0,1,1,1,1,1),
packword23(1,0,0,0,0,0,0,1,0,1,0,0,1,1,0,1,1,0,1,0,0,0,0),
packword23(1,0,1,1,1,0,0,1,1,1,0,0,1,0,1,0,1,0,1,1,1,1,0),
packword23(1,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,0),
packword23(1,1,1,1,1,0,1,0,0,0,1,0,1,0,0,0,1,0,1,1,1,1,1)
};

//OVER
static const uint32_t stats_over23x5[] =
{
packword23(1,1,1,1,1,0,1,0,0,0,1,0,1,1,1,1,1,0,1,1,1,1,0),
packword23(1,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,0,0,1,0,0,0,1),
packword23(1,0,0,0,1,0,0,1,0,1,0,0,1,1,1,1,0,0,1,1,1,1,0),
packword23(1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1),
packword23(1,1,1,1,1,0,0,0,1,0,0,0,1,1,1,1,1,0,1,0,0,0,1)
};

//get bits for the word game
static uint32_t stats_get_game_word_bits(uint8_t row)
{
	return stats_game29x5[row];
}

//get bits for the word over
static uint32_t stats_get_over_word_bits(uint8_t row)
{
	return stats_over23x5[row];
}

//Draw the GAME word
void end_draw_game_word(unsigned int * framePointer)
{
    int8_t x, y; //Iterators
    for(y=0; y < END_WORD_ROWS; y++){//iterate through all rows
		for(x=END_WORD_COL; x >= 0; x--){//iterate through all col
			 if(stats_get_game_word_bits(y) & (1<<x)){//draw a pixel if bit is a 1
				 framePointer[(END_WORD_Y_POS+(GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + END_GAME_X_POS + (GLOBALS_DOUBLE*(END_WORD_COL-x))] = GLOBALS_WHITE;//draw 4 white pixels per bit
				 framePointer[(END_WORD_Y_POS+(GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + END_GAME_X_POS + (GLOBALS_DOUBLE*(END_WORD_COL-x))] = GLOBALS_WHITE;//draw 4 white pixels per bit
				 framePointer[(END_WORD_Y_POS+(GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + END_GAME_X_POS + (GLOBALS_DOUBLE*(END_WORD_COL-x))+1] = GLOBALS_WHITE;//draw 4 white pixels per bit
				 framePointer[(END_WORD_Y_POS+(GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + END_GAME_X_POS + (GLOBALS_DOUBLE*(END_WORD_COL-x))+1] = GLOBALS_WHITE;//draw 4 white pixels per bit
			 }
		}
    }
}

//Draw the OVER word
void end_draw_over_word(unsigned int * framePointer)
{
    int8_t x, y; //Iterators
    for(y=0; y < END_WORD_ROWS; y++){//iterate through all rows
		for(x=END_WORD_COL; x >= 0; x--){//iterate through all col
			 if(stats_get_over_word_bits(y) & (1<<x)){//draw a pixel if bit is a 1
				 framePointer[(END_WORD_Y_POS+(GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + END_OVER_X_POS + (GLOBALS_DOUBLE*(END_WORD_COL-x))] = GLOBALS_WHITE;//draw 4 white pixels per bit
				 framePointer[(END_WORD_Y_POS+(GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + END_OVER_X_POS + (GLOBALS_DOUBLE*(END_WORD_COL-x))] = GLOBALS_WHITE;//draw 4 white pixels per bit
				 framePointer[(END_WORD_Y_POS+(GLOBALS_DOUBLE*y))*GLOBALS_MAX_X + END_OVER_X_POS + (GLOBALS_DOUBLE*(END_WORD_COL-x))+1] = GLOBALS_WHITE;//draw 4 white pixels per bit
				 framePointer[(END_WORD_Y_POS+(GLOBALS_DOUBLE*y)+1)*GLOBALS_MAX_X + END_OVER_X_POS + (GLOBALS_DOUBLE*(END_WORD_COL-x))+1] = GLOBALS_WHITE;//draw 4 white pixels per bit
			 }
		}
    }
}

//Clear the screen for end of game
void end_clear_screen(unsigned int * framePointer){
    uint16_t row=0, col=0; //Iterators
    for( row=0; row<GLOBALS_MAX_Y; row++){
		for(col=0; col<GLOBALS_MAX_X; col++)
			framePointer[row*GLOBALS_MAX_X + col] = 0; //Clear screen for the first and only time
    }
}
