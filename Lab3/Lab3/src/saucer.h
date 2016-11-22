<<<<<<< HEAD
/*
 * saucer.h
 *
 *  Created on: Oct 5, 2016
 *      Author: superman
 */

#ifndef SAUCER_H_
#define SAUCER_H_

static uint32_t saucer_get_sprite_bits(uint8_t);
int16_t saucer_get_saucer_x_position();
static void saucer_set_saucer_x_position(int16_t);
int8_t saucer_get_exists_bool();
static void saucer_set_exists_bool(int8_t set_bool);
void saucer_spawn_randomly(unsigned int *);
void saucer_draw(unsigned int *, uint8_t, uint8_t);
static void saucer_erase(unsigned int *);
void saucer_shift_left(unsigned int *);
void saucer_shift_right(unsigned int *);

int8_t saucer_check_if_saucer_was_hit(unsigned int *, uint16_t, uint16_t);
void saucer_draw_mystery_score(unsigned int *);


#endif /* SAUCER_H_ */
=======
/*
 * saucer.h
 *
 *  Created on: Oct 5, 2016
 *      Author: superman
 */

#ifndef SAUCER_H_
#define SAUCER_H_

static uint32_t saucer_get_sprite_bits(uint8_t);
int16_t saucer_get_saucer_x_position();
static void saucer_set_saucer_x_position(int16_t);
int8_t saucer_get_exists_bool();
static void saucer_set_exists_bool(int8_t set_bool);
void saucer_spawn_randomly(unsigned int *);
void saucer_draw(unsigned int *, uint8_t, uint8_t);
static void saucer_erase(unsigned int *);
void saucer_shift_left(unsigned int *);
void saucer_shift_right(unsigned int *);

int8_t saucer_check_if_saucer_was_hit(unsigned int *, uint16_t, uint16_t);
void saucer_draw_mystery_score(unsigned int *);


#endif /* SAUCER_H_ */
>>>>>>> 46133ae2d6a022a717e89ee90e8959352f12048c
