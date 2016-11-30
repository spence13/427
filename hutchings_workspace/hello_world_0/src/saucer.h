
#ifndef SAUCER_H_
#define SAUCER_H_
#include <stdint.h>

int16_t saucer_get_saucer_x_position(); //Get saucer x position
int8_t saucer_get_exists_bool(); //Check if saucer is in play
void saucer_spawn_randomly(unsigned int *); //Spawn the saucer randomly
void saucer_draw(unsigned int *, uint8_t, uint8_t); //Draw saucer
void saucer_shift_left(unsigned int *); //Shift saucer left
void saucer_shift_right(unsigned int *); //Shift saucer right
void saucer_update_position(unsigned int *); //Update saucer position

int8_t saucer_check_if_saucer_was_hit(unsigned int *, uint16_t, uint16_t); //Check if saucer was hit
void saucer_draw_mystery_score(unsigned int *); //Draw pop up score after saucer kill
void saucer_erase_mystery_score(unsigned int *); //Erase pop up score

#endif /* SAUCER_H_ */
