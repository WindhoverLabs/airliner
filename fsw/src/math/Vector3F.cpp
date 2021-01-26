#include <math/Vector3F.hpp>
#include <math/Matrix1F3.hpp>
#include <math.h>

using namespace math;

Vector3F::Vector3F(float X, float Y, float Z) :
    data{X,Y,Z},
	nan(NAN)
{
};


Vector3F::Vector3F() :
	data{0.0f, 0.0f, 0.0f}
{
};


Vector3F::~Vector3F()
{
};


float& Vector3F::operator [] (uint32 i)
{
	if(i < SIZE)
	{
		return data[i];
	}

	return nan;
};


float Vector3F::operator [] (uint32 i) const
{
	if(i < SIZE)
	{
		return data[i];
	}

	return NAN;
};


float Vector3F::Length() const
{
    return sqrtf(data[0]*data[0] + data[1]*data[1] + data[2]*data[2]);
}


void Vector3F::Zero()
{
    data[0] = 0.0f;
    data[1] = 0.0f;
    data[2] = 0.0f;
}

Matrix1F3 Vector3F::Transpose(void)
{
    Matrix1F3 matOut;
    matOut.Zero();

    matOut[0][0] = data[0];
    matOut[0][1] = data[1];
    matOut[0][2] = data[2];

    return matOut; 
}


// overload + operator to provide a vector addition
Vector3F Vector3F::operator+(const Vector3F &vecIn)
{
    Vector3F vecOut;
    vecOut[0] = data[0] + vecIn[0];
    vecOut[1] = data[1] + vecIn[1];
    vecOut[2] = data[2] + vecIn[2];
    return vecOut;
}


// overload + operator to provide a vector addition
Vector3F Vector3F::operator+(const Vector3F &vecIn) const
{
    Vector3F vecOut;
    vecOut[0] = data[0] + vecIn[0];
    vecOut[1] = data[1] + vecIn[1];
    vecOut[2] = data[2] + vecIn[2];
    return vecOut;
}


// overload - operator to provide a vector subtraction
Vector3F Vector3F::operator-(const Vector3F &vecIn)
{
    Vector3F vecOut;
    vecOut[0] = data[0] - vecIn[0];
    vecOut[1] = data[1] - vecIn[1];
    vecOut[2] = data[2] - vecIn[2];
    return vecOut;
}


// overload - operator to provide a vector subtraction
Vector3F Vector3F::operator-(const Vector3F &vecIn) const
{
    Vector3F vecOut;
    vecOut[0] = data[0] - vecIn[0];
    vecOut[1] = data[1] - vecIn[1];
    vecOut[2] = data[2] - vecIn[2];
    return vecOut;
}


// overload % operator to provide a vector cross product
Vector3F Vector3F::operator%(const Vector3F &vecIn)
{
    Vector3F vecOut;
    vecOut[0] = data[1]*vecIn[2] - data[2]*vecIn[1];
    vecOut[1] = data[2]*vecIn[0] - data[0]*vecIn[2];
    vecOut[2] = data[0]*vecIn[1] - data[1]*vecIn[0];
    return vecOut;
}


// overload * operator to provide a vector scalar product
Vector3F Vector3F::operator*(const float scalar)
{
    Vector3F vecOut;
    vecOut[0] = data[0] * scalar;
    vecOut[1] = data[1] * scalar;
    vecOut[2] = data[2] * scalar;
    return vecOut;
}


// overload / operator to provide a vector scalar division
Vector3F Vector3F::operator/(const float scalar)
{
    Vector3F vecOut;
    vecOut[0] = data[0] / scalar;
    vecOut[1] = data[1] / scalar;
    vecOut[2] = data[2] / scalar;
    return vecOut;
}


// overload dot product operator to provide a vector scalar product
float Vector3F::operator*(const Vector3F &vecIn)
{
	float res = 0.0f;

	res = data[0] * vecIn[0];
	res += data[1] * vecIn[1];
	res += data[2] * vecIn[2];

	return res;
}


Vector3F Vector3F::operator-() const
{
	Vector3F res;

	res[0] = -data[0];
	res[1] = -data[1];
	res[2] = -data[2];

    return res;
}


Vector3F Vector3F::EMult(const Vector3F &vecIn)
{
	Vector3F res;

	res[0] = data[0] * vecIn[0];
	res[1] = data[1] * vecIn[1];
	res[2] = data[2] * vecIn[2];

	return res;
}



const Vector3F Vector3F::EDivide(const Vector3F &vecIn) const
{
	Vector3F res;

	res[0] = data[0] / vecIn[0];
	res[1] = data[1] / vecIn[1];
	res[2] = data[2] / vecIn[2];

	return res;
}



void Vector3F::Constrain(uint32 i, float min, float max)
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



Vector3F Vector3F::Normalized(void)
{
	Vector3F res;
	float mag = Length();

	res[0] = data[0] / mag;
	res[1] = data[1] / mag;
	res[2] = data[2] / mag;

	return res;
}



void Vector3F::Normalize(void)
{
	float mag = Length();

	data[0] = data[0] / mag;
	data[1] = data[1] / mag;
	data[2] = data[2] / mag;
}

