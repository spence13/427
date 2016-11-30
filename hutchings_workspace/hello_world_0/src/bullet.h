
#ifndef BULLET_H_
#define BULLET_H_
#include <stdint.h>

uint8_t bullet_get_is_fired(); //Check if bullet is fired
void bullet_set_is_fired(); //Set if bullet is fired
void bullet_draw(unsigned int *); //Draw bullet
void bullet_erase(unsigned int *); //Erase bullet
uint8_t bullet_update_pos(unsigned int *); //Update bullet position

#endif /* BULLET_H_ */
