/*Includes*/
#include <deque>
#include <valarray>
#include <numeric>
#include <cmath>
#include <climits>
#include <src/stdlib.h>
#include "src/utils/pin.h"
#include "src/utils/timer.h"
#include "src/music/wave_creation.h"
#include "src/music/note_generation.h"

#define PINSEL1 (*(volatile int*)0x4002C004)
#define PINMODE1 (*(volatile int*)0x4002C044)
#define PINMODE0 (*(volatile int*)0x4002C040)
#define PINMODE3 (*(volatile int*)0x4002C04C)

//Used for setting the Digital-to-Analog subsystem values.
#define DACR (*(volatile unsigned int *)0x4008C000)

/*Main function*/
/**
 * LPC1769 Final Project: Audio Training Game
 * Generates audio signals on a pentatonic scale and awaits the proper octave-higher input responses.
 */
int main()
{

	//Uses PS/2 Keyboard Input to begin program
	//Also keeps a counter that is used for the random number generation seed
	bool program_start = false;
	for (int random_number_generation_seed = 0; random_number_generation_seed++; !program_start)
	{
		//Reads in input from keyboard.
		//If a key is being pressed (that is, the interpreted keycode is not 0), then the program will start
		if (keyInput() > 0)
		{
			program_start = true;
		}
		random_number_generation_seed %= INT_MAX;
	}

	//Seed the random number generation LCG
	srand(0);

	//The number of seconds to wait between rounds of the game
	int ticks_to_wait = seconds_to_ticks(0.001);
	//The number of seconds
	int ticks_to_wait_interior = seconds_to_ticks(0.01);

	PINSEL1 |= 1 << 21;
	PINSEL1 &= ~(1 << 20);

	PINMODE1 |= 0b11 << 23;

	//Configure response buttons
	pin a3_in = pin(0, 6);
	pin c4_in = pin(0, 1);
	pin d4_in = pin(0, 17);
	pin e4_in = pin(0, 16);
	pin g4_in = pin(0, 24);
	pin a4_in = pin(1, 31);

	//Set input buttons to have no pull-up or pull-down resistors
	PINMODE0 |= (1 << 13);
	PINMODE0 |= (1 << 3);
	PINMODE1 |= (1 << 3);
	PINMODE1 |= (1 << 1);
	PINMODE1 |= (1 << 17);
	PINMODE3 |= (1 << 31);

	//Configure response buttons to have pull-down resistors
 	PINMODE1 |= (0b11 << 18);
	PINMODE1 |= (0b11 << 14);
	PINMODE0 |= (0b11 << 30);
	PINMODE1 |= (0b11 << 4);
	PINMODE0 |= (0b11 << 0);
	PINMODE0 |= (0b11 << 14);

	//Play OU bells starting music sequence
	play_a4(200);
	play_g4(100);
	play_e4(100);
	play_d4(200);
	play_g4(200);
	play_e4(150);
	play_c4(150);
	play_d4(150);
	play_g4(300);

	wait_ticks(seconds_to_ticks(2));

	play_e4(350);
	play_c4(350);
	play_d4(350);
	play_g4(350);
	wait_ticks(seconds_to_ticks(1));
	play_g4(350);
	play_d4(350);
	play_e4(350);
	play_c4(350);

	wait_ticks(seconds_to_ticks(2));

	//Generate random note and receive response
	while (true)
	{
		//Choose a random note to play and play it
		int note_to_play = rand() % 6;
		switch (note_to_play)
		{
		case 0:
			play_a3();
			break;
		case 1:
			play_c4();
			break;
		case 2:
			play_d4();
			break;
		case 3:
			play_e4();
			break;
		case 4:
			play_g4();
			break;
		case 5:
			play_a4();
			break;
		default:
			break;
		}

		//Stall until player selects the right note
		bool wrong_entry = true;
		while (wrong_entry)
		{
			switch (note_to_play)
			{
			case 0:
				if (a3_in)
				{
					wrong_entry = false;
				}
				break;
			case 1:
				if (c4_in)
				{
					wrong_entry = false;
				}
				break;
			case 2:
				if (d4_in)
				{
					wrong_entry = false;
				}
				break;
			case 3:
				if (e4_in)
				{
					wrong_entry = false;
				}
				break;
			case 4:
				if (g4_in)
				{
					wrong_entry = false;
				}
				break;
			case 5:
				if (a4_in)
				{
					wrong_entry = false;
				}
				break;
			default:
				break;
			}

			pin(0, 25) = a3_in;
			pin(0, 23) = c4_in;
			pin(0, 15) = d4_in;
			pin(0, 18) = e4_in;
			pin(0, 0) = g4_in;
			pin(0, 7) = a4_in;

			wait_ticks(ticks_to_wait_interior);
		}

		wait_ticks(ticks_to_wait);

		//The player has gotten it right. Play the "success" jingle and start again.
		play_d4(100);
		play_g4(100);

		wait_ticks(seconds_to_ticks(1));
	}

	return 0;
}
