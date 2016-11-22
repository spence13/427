<<<<<<< HEAD
#include "bullet.h"
#include "tank.h"
#include "globals.h"
#include "bunker.h"
#include "saucer.h"
#include "aliens.h"

#define BULLET_BITS 6

static uint8_t is_fired = 0;//use as bool to know if tank bullet is fired
static uint16_t bullet_x_position = 0;//x pos of tank bullet
static uint16_t bullet_y_position = 0;//y pos of tank bullet

//return variable to know if a bullet is already fired
uint8_t bullet_get_is_fired()
{
	return is_fired;
}

//draw the bullet with framepointer
void bullet_draw(unsigned int * framePointer)
{
	is_fired = 1;//set bool equal to 1 so you know a bullet is fired

	bullet_x_position = tank_get_tank_x_position() + GLOBALS_TANK_PIX_WIDTH/GLOBALS_DOUBLE - 1;//x pos is set by tanks position
	bullet_y_position = GLOBALS_TANK_Y_POS-GLOBALS_BULLET_PIX_HEIGHT;//original y pos is set at top of tank
	uint8_t i;
	for(i=0; i<BULLET_BITS; i++){//bullet will 6 pixels tall
		framePointer[(bullet_y_position+i)*GLOBALS_MAX_X + bullet_x_position] = GLOBALS_WHITE;//draw a white straight bullet
	}
}

//erase the bullet
void bullet_erase(unsigned int * framePointer)
{
	uint8_t i;
	for(i=0; i<BULLET_BITS; i++){//erase all 6 pixels
		framePointer[(bullet_y_position+i)*GLOBALS_MAX_X + bullet_x_position] = GLOBALS_BLACK;//draw black where bullet was drawn
	}
}

//update the bullet to move up the screen
uint8_t bullet_update_pos(unsigned int * framePointer)
{
	bullet_erase(framePointer);//call the erase function on bullet
	bullet_y_position = bullet_y_position - GLOBALS_BULLET_PIX_HEIGHT;//calculate new y position of bullet
	//check if bullet has intersected a bunker
	if(bunker_check_if_erosion_necessary(framePointer, bullet_x_position, bullet_y_position)){
		is_fired = 0;//set boolean to zero cuz erosion happened and bullet is now gone
		bullet_y_position = GLOBALS_TANK_Y_POS-GLOBALS_BULLET_PIX_HEIGHT;//set y pos of bullet to tank height for next bullet
		return 0;
	}
	//check if bullet has intersected an alien
	else if (kill_alien(framePointer, bullet_x_position, bullet_y_position)){ //check if bullet has hit alien, if not bullet is still on screen
		is_fired = 0;//set boolean to zero cuz erosion happened and bullet is now gone
		bullet_y_position = GLOBALS_TANK_Y_POS-GLOBALS_BULLET_PIX_HEIGHT;//set y pos of bullet to tank height for next bullet
		return 1;
	}
	//check if bullet has intersected the saucer
	else if(saucer_check_if_saucer_was_hit(framePointer, bullet_x_position, bullet_y_position)){
		is_fired = 0;//set boolean to zero cuz erosion happened and bullet is now gone
		bullet_y_position = GLOBALS_TANK_Y_POS-GLOBALS_BULLET_PIX_HEIGHT;//set y pos of bullet to tank height for next bullet
		return 2;
	}
	//if bullet has moved above the screen
	else if(bullet_y_position <= GLOBALS_MIN_Y_BULLET){
		is_fired = 0;//set boolean to zero cuz bullet is now gone
		bullet_y_position = GLOBALS_TANK_Y_POS-GLOBALS_BULLET_PIX_HEIGHT;//set y pos of bullet to tank height for next bullet
		return 0;
	}

	uint8_t i;
	for(i=0; i<BULLET_BITS; i++)//draw all 6 pixels of the bullet
		framePointer[(bullet_y_position+i)*GLOBALS_MAX_X + bullet_x_position] = GLOBALS_WHITE;//redraw it a little higher
	return 0;
}
=======
#include "bullet.h"
#include "tank.h"
#include "globals.h"
#include "bunker.h"
#include "saucer.h"
#include "aliens.h"

#define BULLET_BITS 6

static uint8_t is_fired = 0;//use as bool to know if tank bullet is fired
static uint16_t bullet_x_position = 0;//x pos of tank bullet
static uint16_t bullet_y_position = 0;//y pos of tank bullet

//return variable to know if a bullet is already fired
uint8_t bullet_get_is_fired()
{
	return is_fired;
}

//draw the bullet with framepointer
void bullet_draw(unsigned int * framePointer)
{
	is_fired = 1;//set bool equal to 1 so you know a bullet is fired

	bullet_x_position = tank_get_tank_x_position() + GLOBALS_TANK_PIX_WIDTH/GLOBALS_DOUBLE - 1;//x pos is set by tanks position
	bullet_y_position = GLOBALS_TANK_Y_POS-GLOBALS_BULLET_PIX_HEIGHT;//original y pos is set at top of tank
	uint8_t i;
	for(i=0; i<BULLET_BITS; i++){//bullet will 6 pixels tall
		framePointer[(bullet_y_position+i)*GLOBALS_MAX_X + bullet_x_position] = GLOBALS_WHITE;//draw a white straight bullet
	}
}

//erase the bullet
void bullet_erase(unsigned int * framePointer)
{
	uint8_t i;
	for(i=0; i<BULLET_BITS; i++){//erase all 6 pixels
		framePointer[(bullet_y_position+i)*GLOBALS_MAX_X + bullet_x_position] = GLOBALS_BLACK;//draw black where bullet was drawn
	}
}

//update the bullet to move up the screen
uint8_t bullet_update_pos(unsigned int * framePointer)
{
	bullet_erase(framePointer);//call the erase function on bullet
	bullet_y_position = bullet_y_position - GLOBALS_BULLET_PIX_HEIGHT;//calculate new y position of bullet
	//check if bullet has intersected a bunker
	if(bunker_check_if_erosion_necessary(framePointer, bullet_x_position, bullet_y_position)){
		is_fired = 0;//set boolean to zero cuz erosion happened and bullet is now gone
		bullet_y_position = GLOBALS_TANK_Y_POS-GLOBALS_BULLET_PIX_HEIGHT;//set y pos of bullet to tank height for next bullet
		return 0;
	}
	//check if bullet has intersected an alien
	else if (kill_alien(framePointer, bullet_x_position, bullet_y_position)){ //check if bullet has hit alien, if not bullet is still on screen
		is_fired = 0;//set boolean to zero cuz erosion happened and bullet is now gone
		bullet_y_position = GLOBALS_TANK_Y_POS-GLOBALS_BULLET_PIX_HEIGHT;//set y pos of bullet to tank height for next bullet
		return 1;
	}
	//check if bullet has intersected the saucer
	else if(saucer_check_if_saucer_was_hit(framePointer, bullet_x_position, bullet_y_position)){
		is_fired = 0;//set boolean to zero cuz erosion happened and bullet is now gone
		bullet_y_position = GLOBALS_TANK_Y_POS-GLOBALS_BULLET_PIX_HEIGHT;//set y pos of bullet to tank height for next bullet
		return 2;
	}
	//if bullet has moved above the screen
	else if(bullet_y_position <= GLOBALS_MIN_Y_BULLET){
		is_fired = 0;//set boolean to zero cuz bullet is now gone
		bullet_y_position = GLOBALS_TANK_Y_POS-GLOBALS_BULLET_PIX_HEIGHT;//set y pos of bullet to tank height for next bullet
		return 0;
	}

	uint8_t i;
	for(i=0; i<BULLET_BITS; i++)//draw all 6 pixels of the bullet
		framePointer[(bullet_y_position+i)*GLOBALS_MAX_X + bullet_x_position] = GLOBALS_WHITE;//redraw it a little higher
	return 0;
}
>>>>>>> 46133ae2d6a022a717e89ee90e8959352f12048c