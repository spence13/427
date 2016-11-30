#ifndef SOUNDS_H_
#define SOUNDS_H_

#include <stdint.h>

//These are just enumerations for the different sound types
#define SOUND_ALIEN_MARCH1 0
#define SOUND_ALIEN_MARCH2 1
#define SOUND_ALIEN_MARCH3 2
#define SOUND_ALIEN_MARCH4 3
#define SOUND_UFO_HIGH 4
#define SOUND_UFO_LOW 5
#define SOUND_ALIEN_HIT 6
#define SOUND_TANK_HIT 7
#define SOUND_TANK_SHOOT 8

#define SOUND_PLAY_NOTHING 128
#define SOUND_NUM_SAMPLES 128


void sound_init(); //Initialize sound settings and codec
void sound_set_sample_size(uint8_t); //This function allows other files to start a sound by setting it's counter
void sound_play_sound();//This function plays all sounds that are active. There are some sounds that have higher priority than others
void sound_change_vol(int16_t); //change the volume during play

#endif /* SOUNDS_H_ */
