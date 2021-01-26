#include "math/Vector6F.hpp"
#include <math.h>

using namespace math;

Vector6F::Vector6F(float A, float B, float C, float D, float E, float F):
    data{A,B,C,D,E,F},
	nan(NAN)
{
};


Vector6F::Vector6F()
{
};


Vector6F::~Vector6F()
{
};


float& Vector6F::operator [] (uint32 i)
{
	if(i < SIZE)
	{
		return data[i];
	}

	return nan;
};


float Vector6F::operator [] (uint32 i) const
{
	if(i < SIZE)
	{
		return data[i];
	}

	return NAN;
};


float Vector6F::Length() const
{
    return sqrtf(data[0]*data[0] + data[1]*data[1] + data[2]*data[2] + \
    		data[3]*data[3] + data[4]*data[4] + data[5]*data[5]);
}


void Vector6F::Zero()
{
    data[0] = 0.0f;
    data[1] = 0.0f;
    data[2] = 0.0f;
    data[3] = 0.0f;
    data[4] = 0.0f;
    data[5] = 0.0f;
}


// overload + operator to provide a vector addition
Vector6F Vector6F::operator+(const Vector6F &vecIn)
{
    Vector6F vecOut;
    vecOut[0] = data[0] + vecIn[0];
    vecOut[1] = data[1] + vecIn[1];
    vecOut[2] = data[2] + vecIn[2];
    vecOut[3] = data[3] + vecIn[3];
    vecOut[4] = data[4] + vecIn[4];
    vecOut[5] = data[5] + vecIn[5];
    return vecOut;
}


// overload + operator to provide a vector addition
//Vector6F Vector6F::operator+=(const Vector6F &vecIn)
//{
    //Vector6F vecOut;
    //vecOut[0] = data[0] + vecIn[0];
    //vecOut[1] = data[1] + vecIn[1];
    //vecOut[2] = data[2] + vecIn[2];
    //vecOut[3] = data[3] + vecIn[3];
    //vecOut[4] = data[4] + vecIn[4];
    //vecOut[5] = data[5] + vecIn[5];
    //return vecOut;
//}


// overload - operator to provide a vector subtraction
Vector6F Vector6F::operator-(const Vector6F &vecIn)
{
    Vector6F vecOut;
    vecOut[0] = data[0] - vecIn[0];
    vecOut[1] = data[1] - vecIn[1];
    vecOut[2] = data[2] - vecIn[2];
    vecOut[3] = data[3] - vecIn[3];
    vecOut[4] = data[4] - vecIn[4];
    vecOut[5] = data[5] - vecIn[5];
    return vecOut;
}



// TODO: implement this
// overload % operator to provide a vector cross product
//Vector6F Vector6F::operator%(const Vector6F &vecIn)
//{
//    Vector6F vecOut;
//
//    vecOut[0] = data[1]*vecIn[2] - data[2]*vecIn[1];
//    vecOut[1] = data[2]*vecIn[0] - data[0]*vecIn[2];
//    vecOut[2] = data[0]*vecIn[1] - data[1]*vecIn[0];
//
//    return vecOut;
//}


// overload * operator to provide a vector scalar product
Vector6F Vector6F::operator*(const float scalar)
{
    Vector6F vecOut;
    vecOut[0] = data[0] * scalar;
    vecOut[1] = data[1] * scalar;
    vecOut[2] = data[2] * scalar;
    vecOut[3] = data[3] * scalar;
    vecOut[4] = data[4] * scalar;
    vecOut[5] = data[5] * scalar;
    return vecOut;
}


// overload / operator to provide a vector scalar division
Vector6F Vector6F::operator/(const float scalar)
{
    Vector6F vecOut;
    vecOut[0] = data[0] / scalar;
    vecOut[1] = data[1] / scalar;
    vecOut[2] = data[2] / scalar;
    vecOut[3] = data[3] / scalar;
    vecOut[4] = data[4] / scalar;
    vecOut[5] = data[5] / scalar;
    return vecOut;
}


// overload dot product operator to provide a vector scalar product
float Vector6F::operator*(const Vector6F &vecIn)
{
	float res = 0.0f;

	res = data[0] * vecIn[0];
	res += data[1] * vecIn[1];
	res += data[2] * vecIn[2];
	res += data[3] * vecIn[3];
	res += data[4] * vecIn[4];
	res += data[5] * vecIn[5];

	return res;
}


Vector6F Vector6F::operator-() const
{
	Vector6F res;

	res[0] = -data[0];
	res[1] = -data[1];
	res[2] = -data[2];
	res[3] = -data[3];
	res[4] = -data[4];
	res[5] = -data[5];

    return res;
}


Vector6F Vector6F::pow(const float scalar) const
{
	Vector6F res;

	res[0] = ::pow(data[0], scalar);
	res[1] = ::pow(data[1], scalar);
	res[2] = ::pow(data[2], scalar);
	res[3] = ::pow(data[3], scalar);
	res[4] = ::pow(data[4], scalar);
	res[5] = ::pow(data[5], scalar);

    return res;
}


Vector6F Vector6F::EMult(const Vector6F &vecIn)
{
	Vector6F res;

	res[0] = data[0] * vecIn[0];
	res[1] = data[1] * vecIn[1];
	res[2] = data[2] * vecIn[2];
	res[3] = data[3] * vecIn[3];
	res[4] = data[4] * vecIn[4];
	res[5] = data[5] * vecIn[5];

	return res;
}


const Vector6F Vector6F::EDivide(const Vector6F &vecIn) const
{
	Vector6F res;

	res[0] = data[0] / vecIn[0];
	res[1] = data[1] / vecIn[1];
	res[2] = data[2] / vecIn[2];
	res[3] = data[3] / vecIn[3];
	res[4] = data[4] / vecIn[4];
	res[5] = data[5] / vecIn[5];

	return res;
}


void Vector6F::Constrain(uint32 i, float min, float max)
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


Vector6F Vector6F::Normalized(void)
{
	Vector6F res;
	float mag = Length();

	res[0] = data[0] / mag;
	res[1] = data[1] / mag;
	res[2] = data[2] / mag;
	res[3] = data[3] / mag;
	res[4] = data[4] / mag;
	res[5] = data[5] / mag;

	return res;
}


void Vector6F::Normalize(void)
{
	float mag = Length();

	data[0] = data[0] / mag;
	data[1] = data[1] / mag;
	data[2] = data[2] / mag;
	data[3] = data[3] / mag;
	data[4] = data[4] / mag;
	data[5] = data[5] / mag;
}

