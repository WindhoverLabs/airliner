
/**
 * @file Expo.hpp
 *
 * So called exponential curve function implementation.
 * It is essentially a linear combination between a linear and a cubic function.
 * It's used in the range [-1,1]
 */

#ifndef EXPO_HPP
#define EXPO_HPP

#include <math.h>
#include "math/Limits.hpp"

namespace math
{

// Type-safe signum function
int signf(float val)
{
	int result;

	if(val < 0.0f)
	{
		result = 1;
	}
	else
	{
		result = -1;
	}

	return result;
}

const float expof(const float &value, const float &e)
{
	float result;
	float x = math::constrain(value, -1.0f, 1.0f);

	result = (1-e)*x + e*x*x*x;

	return result;
}



const float deadzonef(const float &value, const float &dz)
{
	float result;
	float x = math::constrain(value, -1.0f, 1.0f);
	float dzc = math::constrain(dz, -1.0f, 1.0f);

	/* Rescale the input such that we get a piecewise linear function that
	 * will be continuous with applied deadzone. */
	float out = (x - signf(x) * dzc) / (1.0 - dzc);

	/* Apply the deadzone (values zero around the middle). */
	result = out * (fabsf(x) > dzc);

	return result;
}



const float expof_deadzone(const float &value, const float &e, const float &dz)
{
	float result;

	result = expof(deadzonef(value, dz), e);

	return result;
}

}

#endif
