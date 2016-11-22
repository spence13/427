<<<<<<< HEAD
#ifndef STATS_H_
#define STATS_H_

#include <stdint.h>

uint16_t stats_get_points_score();
void stats_set_points_score(uint16_t);

uint16_t stats_get_prev_points_score();
void stats_set_prev_points_score(uint16_t);

void stats_draw_score_word(unsigned int *);
void stats_draw_score_number(unsigned int *, int16_t, uint8_t);
void stats_erase_score_number(unsigned int *, int16_t, uint8_t);

void stats_draw_lives_word(unsigned int *);
void stats_draw_lives_tank(unsigned int *, uint8_t);
void stats_erase_lives_tank(unsigned int *, uint8_t);

void stats_calculate_score_num(uint8_t);
int16_t stats_calculate_score_digit_pos(uint8_t, int16_t);
void stats_top_level_erase_score(unsigned int *, uint8_t, int8_t);
void stats_top_level_draw_score(unsigned int *, uint8_t,int8_t);
void stats_update_score(unsigned int *);


#endif /* STATS_H_ */
=======
#ifndef STATS_H_
#define STATS_H_

#include <stdint.h>

uint16_t stats_get_points_score();
void stats_set_points_score(uint16_t);

uint16_t stats_get_prev_points_score();
void stats_set_prev_points_score(uint16_t);

void stats_draw_score_word(unsigned int *);
void stats_draw_score_number(unsigned int *, int16_t, uint8_t);
void stats_erase_score_number(unsigned int *, int16_t, uint8_t);

void stats_draw_lives_word(unsigned int *);
void stats_draw_lives_tank(unsigned int *, uint8_t);
void stats_erase_lives_tank(unsigned int *, uint8_t);

void stats_calculate_score_num(uint8_t);
int16_t stats_calculate_score_digit_pos(uint8_t, int16_t);
void stats_top_level_erase_score(unsigned int *, uint8_t, int8_t);
void stats_top_level_draw_score(unsigned int *, uint8_t,int8_t);
void stats_update_score(unsigned int *);


#endif /* STATS_H_ */
>>>>>>> 46133ae2d6a022a717e89ee90e8959352f12048c
