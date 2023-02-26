/*
 * note_generation.h
 *
 *  Created on: May 2, 2019
 *      Author: David "Dawn" Estes McKnight
 */

#ifndef NOTE_GENERATION_H_
#define NOTE_GENERATION_H_
#define DACR (*(volatile unsigned int *)0x4008C000)

#include "wave_creation.h"

/**
 * Generates a ready-for-output-on-the-LM386 audio wave with the
 * given number of samples
 * @param num_samples The number of samples to take to generate the specified wave
 * @return
 */
std::valarray<int> generate_audio_wave(int num_samples)
{
	std::valarray<double> raw_sin_values = sin(
			generate_range(0, 2 * M_PI, 2 * M_PI / (double) num_samples));
	std::valarray<double> normalized_sin_values = normalize_from_zero_to_one(
			raw_sin_values);
	normalized_sin_values *= 1023;
	std::valarray<int> output_wave = truncated_array(normalized_sin_values);
	return output_wave;
}

/**
 * Plays the provided wave with the provided waittime between each sample output
 * @param wave The wave to play
 * @param waittime The wait time between each output
 * @param num_notes The number of notes to play
 */
void play_wave_with_wait(std::valarray<int> wave, double waittime,
		int num_notes)
{
	int ticks_to_wait = seconds_to_ticks(waittime);

	int notecount = 0;
	int count = 0;
	while (notecount < num_notes)
	{
		DACR = (int) (wave[count]) << 6;
		count++;
		if (count == wave.size())
		{
			count = 0;
			notecount++;
		}

		wait_ticks(ticks_to_wait);
	}
}

const std::valarray<int> a_220_sin_values = generate_audio_wave(20);
/**
 * Method for playing num_notes A3 waves
 * @param num_notes the number of A3 waves to produce
 */
void play_a3(int num_notes = 250)
{
	play_wave_with_wait(a_220_sin_values, 0.0001915, num_notes);
}

const std::valarray<int> c_261_63_sin_values = generate_audio_wave(18);
/**
 * Method for playing num_notes C4 waves
 * @param num_notes the number of C4 waves to produce
 */
void play_c4(int num_notes = 250)
{
	play_wave_with_wait(c_261_63_sin_values, 0.000174, num_notes);
}

const std::valarray<int> d_293_66_sin_values = generate_audio_wave(18);
/**
 * Method for playing num_notes D4 waves
 * @param num_notes the number of D4 waves to produce
 */
void play_d4(int num_notes = 250)
{
	play_wave_with_wait(d_293_66_sin_values, 0.000153, num_notes);
}

const std::valarray<int> e_329_63_sin_values = generate_audio_wave(17);
/**
 * Method for playing num_notes E4 waves
 * @param num_notes the number of E4 waves to produce
 */
void play_e4(int num_notes = 250)
{
	play_wave_with_wait(e_329_63_sin_values, 0.000142, num_notes);
}

const std::valarray<int> g_392_sin_values = generate_audio_wave(18);
/**
 * Method for playing num_notes G4 waves
 * @param num_notes the number of G4 waves to produce
 */
void play_g4(int num_notes = 250)
{
	play_wave_with_wait(g_392_sin_values, 0.000108, num_notes);
}

const std::valarray<int> a_440_sin_values = generate_audio_wave(13);
/**
 * Method for playing num_notes A4 waves
 * @param num_notes the number of A4 waves to produce
 */
void play_a4(int num_notes = 250)
{
	play_wave_with_wait(a_440_sin_values, 0.000135, num_notes);
}

#endif /* NOTE_GENERATION_H_ */
