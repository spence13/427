<<<<<<< HEAD
#ifndef TANK_H_
#define TANK_H_
#include <stdint.h>

void tank_shift_left(unsigned int *); //This function will shift the tank left a frame, takes the framePointer
void tank_shift_right(unsigned int *); //This function will shift the tank right a frame, takes the framePointer
void tank_draw_bottom_line(unsigned int *); //This function will draw the green line beneath the tank, takes the framePointer
uint16_t tank_get_tank_x_position(); //Getter for tank position
uint8_t tank_get_lives_remaining(); //Getter for lives remaining for the tank
uint16_t tank_get_tank_bits(uint16_t); //Getter for tank bits
uint8_t tank_check_if_hit(unsigned int *, uint16_t, uint16_t); //Checks if the tank was hit
void tank_dying1_draw(unsigned int *); //Draw tank dying animation frame 1
void tank_dying2_draw(unsigned int *); //Draw tank dying animation frame 2
void tank_dying1_erase(unsigned int *); //Erase tank dying animation 1
void tank_dying2_erase(unsigned int *); //Erase tank dying animation 2
void tank_draw(unsigned int *); //Function to draw tank

#endif /* TANK_H_ */
=======
#ifndef TANK_H_
#define TANK_H_
#include <stdint.h>

void tank_shift_left(unsigned int *); //This function will shift the tank left a frame, takes the framePointer
void tank_shift_right(unsigned int *); //This function will shift the tank right a frame, takes the framePointer
void tank_draw_bottom_line(unsigned int *); //This function will draw the green line beneath the tank, takes the framePointer
uint16_t tank_get_tank_x_position(); //Getter for tank position
uint8_t tank_get_lives_remaining(); //Getter for lives remaining for the tank
uint16_t tank_get_tank_bits(uint16_t); //Getter for tank bits
uint8_t tank_check_if_hit(unsigned int *, uint16_t, uint16_t); //Checks if the tank was hit
void tank_dying1_draw(unsigned int *); //Draw tank dying animation frame 1
void tank_dying2_draw(unsigned int *); //Draw tank dying animation frame 2
void tank_dying1_erase(unsigned int *); //Erase tank dying animation 1
void tank_dying2_erase(unsigned int *); //Erase tank dying animation 2
void tank_draw(unsigned int *); //Function to draw tank

#endif /* TANK_H_ */
>>>>>>> 46133ae2d6a022a717e89ee90e8959352f12048c
