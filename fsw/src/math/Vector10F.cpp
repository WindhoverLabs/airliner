#include "math/Vector10F.hpp"
#include <math.h>

using namespace math;

Vector10F::Vector10F(float A, float B, float C, float D, float E, float F, float G, float H, float I, float J):
    data{A,B,C,D,E,F,G,H,I,J},
	nan(NAN)
{
};


Vector10F::Vector10F()
{
};


Vector10F::~Vector10F()
{
};


float& Vector10F::operator [] (uint32 i)
{
	if(i < SIZE)
	{
		return data[i];
	}

	return nan;
};


float Vector10F::operator [] (uint32 i) const
{
	if(i < SIZE)
	{
		return data[i];
	}

	return NAN;
};


float Vector10F::Length() const
{
    return sqrtf(data[0]*data[0] + data[1]*data[1] + data[2]*data[2] + \
    		data[3]*data[3] + data[4]*data[4] + data[5]*data[5] + \
			data[6]*data[6] + data[7]*data[7] + data[8]*data[8] + \
			data[9]*data[9]);
}


void Vector10F::Zero()
{
    data[0] = 0.0f;
    data[1] = 0.0f;
    data[2] = 0.0f;
    data[3] = 0.0f;
    data[4] = 0.0f;
    data[5] = 0.0f;
    data[6] = 0.0f;
    data[7] = 0.0f;
    data[8] = 0.0f;
    data[9] = 0.0f;
}


// overload + operator to provide a vector addition
Vector10F Vector10F::operator+(const Vector10F &vecIn)
{
    Vector10F vecOut;
    vecOut[0] = data[0] + vecIn[0];
    vecOut[1] = data[1] + vecIn[1];
    vecOut[2] = data[2] + vecIn[2];
    vecOut[3] = data[3] + vecIn[3];
    vecOut[4] = data[4] + vecIn[4];
    vecOut[5] = data[5] + vecIn[5];
    vecOut[6] = data[6] + vecIn[6];
    vecOut[7] = data[7] + vecIn[7];
    vecOut[8] = data[8] + vecIn[8];
    vecOut[9] = data[9] + vecIn[9];
    return vecOut;
}


// overload + operator to provide a vector addition
Vector10F Vector10F::operator+(const Vector10F &vecIn) const
{
    Vector10F vecOut;
    vecOut[0] = data[0] + vecIn[0];
    vecOut[1] = data[1] + vecIn[1];
    vecOut[2] = data[2] + vecIn[2];
    vecOut[3] = data[3] + vecIn[3];
    vecOut[4] = data[4] + vecIn[4];
    vecOut[5] = data[5] + vecIn[5];
    vecOut[6] = data[6] + vecIn[6];
    vecOut[7] = data[7] + vecIn[7];
    vecOut[8] = data[8] + vecIn[8];
    vecOut[9] = data[9] + vecIn[9];
    return vecOut;
}


// overload + operator to provide a vector addition
void Vector10F::operator+=(const Vector10F &vecIn)
{
    data[0] += vecIn[0];
    data[1] += vecIn[1];
    data[2] += vecIn[2];
    data[3] += vecIn[3];
    data[4] += vecIn[4];
    data[5] += vecIn[5];
    data[6] += vecIn[6];
    data[7] += vecIn[7];
    data[8] += vecIn[8];
    data[9] += vecIn[9];
}


// overload - operator to provide a vector subtraction
Vector10F Vector10F::operator-(const Vector10F &vecIn)
{
    Vector10F vecOut;
    vecOut[0] = data[0] - vecIn[0];
    vecOut[1] = data[1] - vecIn[1];
    vecOut[2] = data[2] - vecIn[2];
    vecOut[3] = data[3] - vecIn[3];
    vecOut[4] = data[4] - vecIn[4];
    vecOut[5] = data[5] - vecIn[5];
    vecOut[6] = data[6] - vecIn[6];
    vecOut[7] = data[7] - vecIn[7];
    vecOut[8] = data[8] - vecIn[8];
    vecOut[9] = data[9] - vecIn[9];
    return vecOut;
}



// TODO: implement this
// overload % operator to provide a vector cross product
//Vector10F Vector10F::operator%(const Vector10F &vecIn)
//{
//    Vector10F vecOut;
//
//    vecOut[0] = data[1]*vecIn[2] - data[2]*vecIn[1];
//    vecOut[1] = data[2]*vecIn[0] - data[0]*vecIn[2];
//    vecOut[2] = data[0]*vecIn[1] - data[1]*vecIn[0];
//
//    return vecOut;
//}


// overload * operator to provide a vector scalar product
Vector10F Vector10F::operator*(const float scalar)
{
    Vector10F vecOut;
    vecOut[0] = data[0] * scalar;
    vecOut[1] = data[1] * scalar;
    vecOut[2] = data[2] * scalar;
    vecOut[3] = data[3] * scalar;
    vecOut[4] = data[4] * scalar;
    vecOut[5] = data[5] * scalar;
    vecOut[6] = data[6] * scalar;
    vecOut[7] = data[7] * scalar;
    vecOut[8] = data[8] * scalar;
    vecOut[9] = data[9] * scalar;
    return vecOut;
}


// overload / operator to provide a vector scalar division
Vector10F Vector10F::operator/(const float scalar)
{
    Vector10F vecOut;
    vecOut[0] = data[0] / scalar;
    vecOut[1] = data[1] / scalar;
    vecOut[2] = data[2] / scalar;
    vecOut[3] = data[3] / scalar;
    vecOut[4] = data[4] / scalar;
    vecOut[5] = data[5] / scalar;
    vecOut[6] = data[6] / scalar;
    vecOut[7] = data[7] / scalar;
    vecOut[8] = data[8] / scalar;
    vecOut[9] = data[9] / scalar;
    return vecOut;
}


// overload dot product operator to provide a vector scalar product
//float Vector10F::operator*(const Vector10F &vecIn)
//{
	//float res = 0.0f;

	//res = data[0] * vecIn[0];
	//res += data[1] * vecIn[1];
	//res += data[2] * vecIn[2];
	//res += data[3] * vecIn[3];
	//res += data[4] * vecIn[4];
	//res += data[5] * vecIn[5];
	//res += data[6] * vecIn[6];
	//res += data[7] * vecIn[7];
	//res += data[8] * vecIn[8];
	//res += data[9] * vecIn[9];

	//return res;
//}


Vector10F Vector10F::operator-() const
{
	Vector10F res;

	res[0] = -data[0];
	res[1] = -data[1];
	res[2] = -data[2];
	res[3] = -data[3];
	res[4] = -data[4];
	res[5] = -data[5];
	res[6] = -data[6];
	res[7] = -data[7];
	res[8] = -data[8];
	res[9] = -data[9];

    return res;
}


Vector10F Vector10F::EMult(const Vector10F &vecIn)
{
	Vector10F res;

	res[0] = data[0] * vecIn[0];
	res[1] = data[1] * vecIn[1];
	res[2] = data[2] * vecIn[2];
	res[3] = data[3] * vecIn[3];
	res[4] = data[4] * vecIn[4];
	res[5] = data[5] * vecIn[5];
	res[6] = data[6] * vecIn[6];
	res[7] = data[7] * vecIn[7];
	res[8] = data[8] * vecIn[8];
	res[9] = data[9] * vecIn[9];

	return res;
}

const Vector10F Vector10F::EDivide(const Vector10F &vecIn) const
{
	Vector10F res;

	res[0] = data[0] / vecIn[0];
	res[1] = data[1] / vecIn[1];
	res[2] = data[2] / vecIn[2];
	res[3] = data[3] / vecIn[3];
	res[4] = data[4] / vecIn[4];
	res[5] = data[5] / vecIn[5];
	res[6] = data[6] / vecIn[6];
	res[7] = data[7] / vecIn[7];
	res[8] = data[8] / vecIn[8];
	res[9] = data[9] / vecIn[9];

	return res;
}

void Vector10F::Constrain(uint32 i, float min, float max)
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

Vector10F Vector10F::Normalized(void)
{
	Vector10F res;
	float mag = Length();

	res[0] = data[0] / mag;
	res[1] = data[1] / mag;
	res[2] = data[2] / mag;
	res[3] = data[3] / mag;
	res[4] = data[4] / mag;
	res[5] = data[5] / mag;
	res[6] = data[6] / mag;
	res[7] = data[7] / mag;
	res[8] = data[8] / mag;
	res[9] = data[9] / mag;

	return res;
}

void Vector10F::Normalize(void)
{
	float mag = Length();

	data[0] = data[0] / mag;
	data[1] = data[1] / mag;
	data[2] = data[2] / mag;
	data[3] = data[3] / mag;
	data[4] = data[4] / mag;
	data[5] = data[5] / mag;
	data[6] = data[6] / mag;
	data[7] = data[7] / mag;
	data[8] = data[8] / mag;
	data[9] = data[9] / mag;
}

