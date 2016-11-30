#include <xparameters.h>
#include "platform.h"
#include "xac97_l.h"
#include "sounds.h"
#include "saucer.h"
#include "bullet.h"

#include "game_sounds/soundexplosion.h"
#include "game_sounds/soundinvade1.h"
#include "game_sounds/soundinvade2.h"
#include "game_sounds/soundinvade3.h"
#include "game_sounds/soundinvade4.h"
#include "game_sounds/soundinvadekill.h"
#include "game_sounds/soundshoot.h"
#include "game_sounds/soundufohigh.h"
#include "game_sounds/soundufolow.h"

extern uint16_t soundexplosion_sample_rate; //This sample rate is the same for all sounds

extern uint8_t soundexplosion[]; //Array of samples for explosion
extern uint16_t soundexplosion_num_samples; //Number of samples for explosion

extern uint8_t soundinvade1[]; //Array of samples for alien march1
extern uint16_t soundinvade1_num_samples; //Number of samples for alien march1

extern uint8_t soundinvade2[];//Array of samples for alien march2
extern uint16_t soundinvade2_num_samples; //Number of samples for alien march2

extern uint8_t soundinvade3[];//Array of samples for alien march3
extern uint16_t soundinvade3_num_samples; //Number of samples for alien march3

extern uint8_t soundinvade4[];//Array of samples for alien march4
extern uint16_t soundinvade4_num_samples; //Number of samples for alien march4

extern uint8_t soundinvadekill[]; //Array of samples for ufo kill
extern uint16_t soundinvadekill_num_samples; //Number of samples for ufo kill

extern uint8_t soundshoot[]; //Array of samples for tank shoot
extern uint16_t soundshoot_num_samples; //Number of samples for tank shoot

extern uint8_t soundufohigh[]; //Array of samples for ufo high
extern uint16_t soundufohigh_num_samples; //Number of samples for ufo high

extern uint8_t soundufolow[]; //Array of samples for ufo low
extern uint16_t soundufolow_num_samples; //Number of samples for ufo low

uint16_t tank_hit_sound_counter = 0; //counter that keeps track of tank hit sound position
uint16_t alien_march_sound_counter1 = 0; //counter that keeps track of alien march1 sound position
uint16_t alien_march_sound_counter2 = 0; //counter that keeps track of alien march2 sound position
uint16_t alien_march_sound_counter3 = 0; //counter that keeps track of alien march3 sound position
uint16_t alien_march_sound_counter4 = 0; //counter that keeps track of alien march4 sound position
uint16_t alien_hit_sound_counter = 0; //counter that keeps track of alien hit sound position
uint16_t tank_shoot_sound_counter = 0; //counter that keeps track of tank shoot sound position
uint16_t ufo_high_sound_counter = 0; //counter that keeps track of tank sound position
uint16_t ufo_low_sound_counter = 0; //counter that keeps track of tank sound position


//This function allows other files to start a sound by setting it's counter
void sound_set_sample_size(uint8_t current_sound)
{
	switch(current_sound)
	{
		case SOUND_ALIEN_MARCH1://this is will start the first marching sound playing
			alien_march_sound_counter1 = soundinvade1_num_samples;//set the counter to the number of samples in the array
			break;
		case SOUND_ALIEN_MARCH2://this is will start the second marching sound playing
			alien_march_sound_counter2 = soundinvade2_num_samples;//set the counter to the number of samples in the array
			break;
		case SOUND_ALIEN_MARCH3://this is will start the third marching sound playing
			alien_march_sound_counter3 = soundinvade3_num_samples;//set the counter to the number of samples in the array
			break;
		case SOUND_ALIEN_MARCH4://this is will start the fourth marching sound playing
			alien_march_sound_counter4 = soundinvade4_num_samples;//set the counter to the number of samples in the array
			break;
		case SOUND_UFO_HIGH://this is will start the ufo explosion sound playing
			ufo_high_sound_counter = soundufohigh_num_samples;//set the counter to the number of samples in the array
			break;
		case SOUND_UFO_LOW://this is will start the ufo flying sound playing
			ufo_low_sound_counter = soundufolow_num_samples;//set the counter to the number of samples in the array
			break;
		case SOUND_ALIEN_HIT://this is will start the alien explosion sound playing
			alien_hit_sound_counter = soundinvadekill_num_samples;//set the counter to the number of samples in the array
			break;
		case SOUND_TANK_HIT://this is will start the tank explosion sound playing
			tank_hit_sound_counter = soundexplosion_num_samples;//set the counter to the number of samples in the array
			break;
		case SOUND_TANK_SHOOT://this is will start the tank shoot sound playing
			tank_shoot_sound_counter = soundshoot_num_samples;//set the counter to the number of samples in the array
			break;
		default:
			return;
	}
}

//initialize the sound settings
void sound_init()
{
    XAC97_HardReset(XPAR_AXI_AC97_0_BASEADDR); //Hard reset

    XAC97_AwaitCodecReady(XPAR_AXI_AC97_0_BASEADDR); //Wait for codec to be ready

    XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_ExtendedAudioStat, 1); //Enable VRA Mode

    XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_PCM_DAC_Rate, soundexplosion_sample_rate); //Writer frequency

    // Play Volume Settings
    XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_MasterVol, AC97_VOL_MAX);
    XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_AuxOutVol, AC97_VOL_MAX);
    XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_MasterVolMono, AC97_VOL_MAX);
    XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_PCBeepVol, AC97_VOL_MAX);
    XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_PCMOutVol, AC97_VOL_MAX);
    XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_LineInVol, AC97_VOL_MAX);
    XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_MicVol, AC97_VOL_MAX);

    XAC97_ClearFifos(XPAR_AXI_AC97_0_BASEADDR); // Clear FIFOs
}

//change the volume during play
void sound_change_vol(int16_t master_volume)
{
    // Play Volume Settings
    XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_MasterVol, master_volume);//pass in master volume set from push buttons
    XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_AuxOutVol, master_volume);
    XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_MasterVolMono, master_volume);
    XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_PCBeepVol, master_volume);
    XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_PCMOutVol, master_volume);
}

//This function plays all sounds that are active. There are some sounds that have higher priority than others
void sound_play_sound()
{
	uint8_t i;
	for (i=0; i < SOUND_NUM_SAMPLES; i++)//play 128 iterations of the arrays
	{
		if (tank_shoot_sound_counter > 0)//tank shooting a bullet
		{
			XAC97_mSetInFifoData(XPAR_AXI_AC97_0_BASEADDR, soundshoot[tank_shoot_sound_counter]);//write the tank shooting sound
			tank_shoot_sound_counter--;//decrement the counter to iterate through the array and play all samples
		}
		if (tank_hit_sound_counter > 0)//tank exploding sound
		{
			XAC97_mSetInFifoData(XPAR_AXI_AC97_0_BASEADDR, soundexplosion[tank_hit_sound_counter]);//write the tank exploding sound
			tank_hit_sound_counter--;//decrement the counter to iterate through the array and play all samples
		}
		if (ufo_high_sound_counter > 0)//ufo exploding sound
		{
			XAC97_mSetInFifoData(XPAR_AXI_AC97_0_BASEADDR, soundufohigh[ufo_high_sound_counter]);//write the ufo exploding sound
			ufo_high_sound_counter--;//decrement the counter to iterate through the array and play all samples
		}
		if ((saucer_get_exists_bool()==1) && (ufo_low_sound_counter > 0))//ufo flying sound that loops the entire flight sequence
		{
				if ((tank_hit_sound_counter == 0) && (tank_shoot_sound_counter==0))//if tank is exploding or tank is shooting
				{
					XAC97_mSetInFifoData(XPAR_AXI_AC97_0_BASEADDR, soundufolow[ufo_low_sound_counter]);//only play sound if not shooting and not exploding the tank
				}
				ufo_low_sound_counter--;//decrement the counter even if not playing the sound
				if (ufo_low_sound_counter == 0)//if youve reached the end of the array
				{
					sound_set_sample_size(SOUND_UFO_LOW);//reset the counter and keep looping the sound
				}
		}
		if ((saucer_get_exists_bool() == 0) && (ufo_low_sound_counter > 0))//if the ufo no longer exists but the counter never reached zero
		{
			XAC97_mSetInFifoData(XPAR_AXI_AC97_0_BASEADDR, SOUND_PLAY_NOTHING); // write an intermediate values
			ufo_low_sound_counter = 0;//set counter to zero cuz the ufo is off of the screen
		}

		if (alien_march_sound_counter1 > 0)//first alien marching sound
		{
			if((saucer_get_exists_bool() == 0) && (tank_hit_sound_counter == 0) && (tank_shoot_sound_counter==0))//if ufo doesnt exist and tank isnt exploding and tank isnt shooting
			{
				XAC97_mSetInFifoData(XPAR_AXI_AC97_0_BASEADDR, soundinvade1[alien_march_sound_counter1]);//write the sound only if above conditions apply - low precedence
			}
			alien_march_sound_counter1--;//decrement the counter even if not playing the sound
		}
		if (alien_march_sound_counter2 > 0)//second alien marching sound
		{
			if((saucer_get_exists_bool() == 0) && (tank_hit_sound_counter == 0) && (tank_shoot_sound_counter==0))//if ufo doesnt exist and tank isnt exploding and tank isnt shooting
			{
				XAC97_mSetInFifoData(XPAR_AXI_AC97_0_BASEADDR, soundinvade2[alien_march_sound_counter2]);//write the sound only if above conditions apply - low precedence
			}
			alien_march_sound_counter2--;//decrement the counter even if not playing the sound
		}
		if (alien_march_sound_counter3 > 0)//third alien marching sound
		{
			if((saucer_get_exists_bool() == 0) && (tank_hit_sound_counter == 0) && (tank_shoot_sound_counter==0))//if ufo doesnt exist and tank isnt exploding and tank isnt shooting
			{
				XAC97_mSetInFifoData(XPAR_AXI_AC97_0_BASEADDR, soundinvade3[alien_march_sound_counter3]);//write the sound only if above conditions apply - low precedence
			}
			alien_march_sound_counter3--;//decrement the counter even if not playing the sound
		}
		if (alien_march_sound_counter4 > 0)//fourth alien marching sound
		{
			if((saucer_get_exists_bool() == 0) && (tank_hit_sound_counter == 0) && (tank_shoot_sound_counter==0))//if ufo doesnt exist and tank isnt exploding and tank isnt shooting
			{
				XAC97_mSetInFifoData(XPAR_AXI_AC97_0_BASEADDR, soundinvade4[alien_march_sound_counter4]);//write the sound only if above conditions apply - low precedence
			}
			alien_march_sound_counter4--;//decrement the counter even if not playing the sound
		}

		//if no other sounds are playing than fill the buffer with value of 128 so no sound plays
		if ((alien_march_sound_counter1==0) && (alien_march_sound_counter2==0) && (alien_march_sound_counter3==0) && (alien_march_sound_counter4==0)
				&& (ufo_high_sound_counter==0) && (ufo_low_sound_counter==0) && (alien_hit_sound_counter==0) && (tank_hit_sound_counter==0) && (tank_shoot_sound_counter==0))
		{
			XAC97_mSetInFifoData(XPAR_AXI_AC97_0_BASEADDR, SOUND_PLAY_NOTHING); // write intermediate value if no sound is playing
		}
	}
}

