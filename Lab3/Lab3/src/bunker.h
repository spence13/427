<<<<<<< HEAD
#ifndef BUNKER_H_
#define BUNKER_H_

#include <stdint.h>

void bunker_draw(unsigned int *);
void bunker_get_erosion_block(uint8_t);
void bunker_get_erosion_bunker_num(uint8_t);
void bunker_get_erosion_state(uint8_t);
int8_t bunker_erode(unsigned int *, uint8_t, uint8_t);
int16_t bunker_calculate_erosion_block_num(uint16_t, uint16_t);
int8_t bunker_check_if_erosion_necessary(unsigned int *, uint16_t, uint16_t);

#endif /* BUNKER_H_ */
=======
#ifndef BUNKER_H_
#define BUNKER_H_

#include <stdint.h>

void bunker_draw(unsigned int *);
void bunker_get_erosion_block(uint8_t);
void bunker_get_erosion_bunker_num(uint8_t);
void bunker_get_erosion_state(uint8_t);
int8_t bunker_erode(unsigned int *, uint8_t, uint8_t);
int16_t bunker_calculate_erosion_block_num(uint16_t, uint16_t);
int8_t bunker_check_if_erosion_necessary(unsigned int *, uint16_t, uint16_t);

#endif /* BUNKER_H_ */
>>>>>>> 46133ae2d6a022a717e89ee90e8959352f12048c
