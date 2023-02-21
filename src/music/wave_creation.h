/*
 * wave_creation.h
 *
 *  Created on: May 2, 2019
 *      Author: DEMcKnight
 */
#include <valarray>

#ifndef WAVE_CREATION_H_
#define WAVE_CREATION_H_

/**
 * Truncates a valarray of doubles to a valarray of ints
 * @param untruncated_array The untruncated doubles array
 * @return An integer array of the truncated values of the provided double array
 */
std::valarray<int> truncated_array(std::valarray<double> untruncated_array)
{
	std::valarray<int> truncated_array = std::valarray<int>(
			untruncated_array.size());
	for (int index = 0; index < untruncated_array.size(); index++)
	{
		truncated_array[index] = (int) untruncated_array[index];
	}

	return truncated_array;
}

/**
 * Generates a valarray with values ranging from start through end, with the given step size.
 * @param start The lowest value of the generated valarray.
 * @param end The upper bound on the highest value of the generated valarray.
 * @param step The spacing between members of the generated valarray
 * @return a valarray with a range of values specified by the parameters
 */
std::valarray<double> generate_range(double start, double end, double step)
{
	std::valarray<double> return_array((end - start) / step + 1);
	for (int i = 0; i < return_array.size(); i++)
	{
		return_array[i] = start + i * step;
	}
	return return_array;
}

/**
 * Renormalizes a provided wave from 0 to 1 (inclusive)
 * @param initial_wave The wave to normalize
 * @return The provided wave, renormalized from zero to one (inclusive)
 */
std::valarray<double> normalize_from_zero_to_one(
		std::valarray<double> initial_wave)
{
	//Normalize above/including 0
	std::valarray<double> nonnegative_wave = initial_wave - initial_wave.min();

	//Normalize from 0 to 1 (inclusive)
	std::valarray<double> normalized_wave = nonnegative_wave
			/ (nonnegative_wave.max() - nonnegative_wave.min());

	return normalized_wave;
}

#endif /* WAVE_CREATION_H_ */
