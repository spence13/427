<<<<<<< HEAD
#include "videoController.h"
#include <stdio.h>
#include <stdlib.h>
#include "tank.h"
#include "bullet.h"
#include "bunker.h"
#include "aliens.h"
#include "saucer.h"
#include "stats.h"

#define VIDEO_DIGIT_SIZE 2 //Number of digits needed to kill alien
#define VIDEO_DIGIT_ONE 0 //Index in array for first digit
#define VIDEO_DIGIT_TWO 1 //Index in array for second digit

// Call the appropriate functions when a character is received
void update_display(unsigned int * framePointer)
{
	char input = getchar(); //Get menu option from the user
	char digits[VIDEO_DIGIT_SIZE] = ""; //array that will hold both digits when you kill alien
	switch(input)
	{
		case '1':
			if(!saucer_get_exists_bool()){
				saucer_spawn_randomly(framePointer);
			}
			break;
		case '2':
			digits[VIDEO_DIGIT_ONE] = getchar(); //get first digit
			digits[VIDEO_DIGIT_TWO] = getchar(); //get second digit
			kill_alien(framePointer, atoi(digits));//kills the alien at that number
			break;
		case '3':
			fire_alien_bullet(framePointer); //shoot a random alien bullet
			break;
		case '4':
			if(saucer_get_exists_bool()){
				saucer_shift_left(framePointer);
			}
			tank_shift_left(framePointer);//shift tank to the left
			break;
		case '5':
			if(!bullet_get_is_fired())//only fire if bullet doesnt already exist
				bullet_draw(framePointer);//fire_tank_bullet();
			break;
		case '6':
			if(saucer_get_exists_bool()){
				saucer_shift_right(framePointer);
			}
			tank_shift_right(framePointer);//shift tank to the right
			break;
		case '7':
			bunker_erode(framePointer, getchar()-'0', getchar()-'0');//erode_bunker(framepointer, bunker_number, erosion section);
			break;
		case '8':
			update_alien_pos(framePointer);//move the block of aliens
			break;
		case '9':
			if(bullet_get_is_fired())//only update the bullet if a bullet exists
				bullet_update_pos(framePointer);//update_all_bullets();
			update_alien_bullets(framePointer); //update the alien bullets
			break;
		default:
			break;
	}
}
=======
#include "videoController.h"
#include <stdio.h>
#include <stdlib.h>
#include "tank.h"
#include "bullet.h"
#include "bunker.h"
#include "aliens.h"
#include "saucer.h"
#include "stats.h"

#define VIDEO_DIGIT_SIZE 2 //Number of digits needed to kill alien
#define VIDEO_DIGIT_ONE 0 //Index in array for first digit
#define VIDEO_DIGIT_TWO 1 //Index in array for second digit

// Call the appropriate functions when a character is received
void update_display(unsigned int * framePointer)
{
	char input = getchar(); //Get menu option from the user
	char digits[VIDEO_DIGIT_SIZE] = ""; //array that will hold both digits when you kill alien
	switch(input)
	{
		case '1':
			if(!saucer_get_exists_bool()){
				saucer_spawn_randomly(framePointer);
			}
			break;
		case '2':
			digits[VIDEO_DIGIT_ONE] = getchar(); //get first digit
			digits[VIDEO_DIGIT_TWO] = getchar(); //get second digit
			kill_alien(framePointer, atoi(digits));//kills the alien at that number
			break;
		case '3':
			fire_alien_bullet(framePointer); //shoot a random alien bullet
			break;
		case '4':
			if(saucer_get_exists_bool()){
				saucer_shift_left(framePointer);
			}
			tank_shift_left(framePointer);//shift tank to the left
			break;
		case '5':
			if(!bullet_get_is_fired())//only fire if bullet doesnt already exist
				bullet_draw(framePointer);//fire_tank_bullet();
			break;
		case '6':
			if(saucer_get_exists_bool()){
				saucer_shift_right(framePointer);
			}
			tank_shift_right(framePointer);//shift tank to the right
			break;
		case '7':
			bunker_erode(framePointer, getchar()-'0', getchar()-'0');//erode_bunker(framepointer, bunker_number, erosion section);
			break;
		case '8':
			update_alien_pos(framePointer);//move the block of aliens
			break;
		case '9':
			if(bullet_get_is_fired())//only update the bullet if a bullet exists
				bullet_update_pos(framePointer);//update_all_bullets();
			update_alien_bullets(framePointer); //update the alien bullets
			break;
		default:
			break;
	}
}
>>>>>>> 46133ae2d6a022a717e89ee90e8959352f12048c
