#include "math/Vector1F.hpp"
#include <math.h>

using namespace math;

Vector1F::Vector1F(float A):
    data{A},
	nan(NAN)
{
};


Vector1F::Vector1F()
{
};


Vector1F::~Vector1F()
{
};


float& Vector1F::operator [] (uint32 i)
{
	if(i < SIZE)
	{
		return data[i];
	}

	return nan;
};


float Vector1F::operator [] (uint32 i) const
{
	if(i < SIZE)
	{
		return data[i];
	}

	return NAN;
};


float Vector1F::Length() const
{
    return sqrtf(data[0]*data[0]);
}


void Vector1F::Zero()
{
    data[0] = 0.0f;
}


// overload + operator to provide a vector addition
Vector1F Vector1F::operator+(const Vector1F &vecIn)
{
    Vector1F vecOut;
    vecOut[0] = data[0] + vecIn[0];
    return vecOut;
}


// overload + operator to provide a vector addition
//Vector1F Vector1F::operator+=(const Vector1F &vecIn)
//{
    //Vector1F vecOut;
    //vecOut[0] = data[0] + vecIn[0];
    //return vecOut;
//}


// overload - operator to provide a vector subtraction
Vector1F Vector1F::operator-(const Vector1F &vecIn)
{
    Vector1F vecOut;
    vecOut[0] = data[0] - vecIn[0];
    return vecOut;
}


// overload * operator to provide a vector scalar product
Vector1F Vector1F::operator*(const float scalar)
{
    Vector1F vecOut;
    vecOut[0] = data[0] * scalar;
    return vecOut;
}


// overload / operator to provide a vector scalar division
Vector1F Vector1F::operator/(const float scalar)
{
    Vector1F vecOut;
    vecOut[0] = data[0] / scalar;
    return vecOut;
}


// overload dot product operator to provide a vector scalar product
float Vector1F::operator*(const Vector1F &vecIn)
{
	float res = 0.0f;

	res = data[0] * vecIn[0];

	return res;
}


Vector1F Vector1F::operator-() const
{
	Vector1F res;

	res[0] = -data[0];

    return res;
}


Vector1F Vector1F::pow(const float scalar) const
{
	Vector1F res;

	res[0] = ::pow(data[0], scalar);

    return res;
}


Vector1F Vector1F::EMult(const Vector1F &vecIn)
{
	Vector1F res;

	res[0] = data[0] * vecIn[0];

	return res;
}


const Vector1F Vector1F::EDivide(const Vector1F &vecIn) const
{
	Vector1F res;

	res[0] = data[0] / vecIn[0];

	return res;
}


void Vector1F::Constrain(uint32 i, float min, float max)
{
	if(data[i] < min)
	{
		data[i] = min;
	}
	else if(data[i] > max)
	{
		data[i] = max;
	}
}


Vector1F Vector1F::Normalized(void)
{
	Vector1F res;
	float mag = Length();

	res[0] = data[0] / mag;

	return res;
}


void Vector1F::Normalize(void)
{
	float mag = Length();

	data[0] = data[0] / mag;
}

