#include <math/Vector2F.hpp>
#include <math/Matrix1F2.hpp>
#include <math.h>

using namespace math;

Vector2F::Vector2F(float X, float Y) :
    data{X,Y},
	nan(NAN)
{
};


Vector2F::Vector2F()
{
};


Vector2F::~Vector2F()
{
};


float& Vector2F::operator [] (uint32 i)
{
	if(i < SIZE)
	{
		return data[i];
	}

	return nan;
};


float Vector2F::operator [] (uint32 i) const
{
	if(i < SIZE)
	{
		return data[i];
	}

	return NAN;
};


float Vector2F::Length() const
{
    return sqrtf(data[0]*data[0] + data[1]*data[1]);
}


void Vector2F::Zero()
{
    data[0] = 0.0f;
    data[1] = 0.0f;
}

Vector2F Vector2F::pow(const float scalar) const
{
	Vector2F res;

	res[0] = ::pow(data[0], scalar);
	res[1] = ::pow(data[1], scalar);

    return res;
}

// overload + operator to provide a vector addition
Vector2F Vector2F::operator+(const Vector2F &vecIn)
{
    Vector2F vecOut;
    vecOut[0] = data[0] + vecIn[0];
    vecOut[1] = data[1] + vecIn[1];
    return vecOut;
}


// overload - operator to provide a vector subtraction
Vector2F Vector2F::operator-(const Vector2F &vecIn)
{
    Vector2F vecOut;
    vecOut[0] = data[0] - vecIn[0];
    vecOut[1] = data[1] - vecIn[1];
    return vecOut;
}


// overload * operator to provide a vector scalar product
Vector2F Vector2F::operator*(const float scalar)
{
    Vector2F vecOut;
    vecOut[0] = data[0] * scalar;
    vecOut[1] = data[1] * scalar;
    return vecOut;
}

// overload * operator to provide a vector scalar product
Vector2F Vector2F::operator*(const float scalar) const
{
    Vector2F vecOut;
    vecOut[0] = data[0] * scalar;
    vecOut[1] = data[1] * scalar;
    return vecOut;
}


// overload / operator to provide a vector scalar division
Vector2F Vector2F::operator/(const float scalar)
{
    Vector2F vecOut;
    vecOut[0] = data[0] / scalar;
    vecOut[1] = data[1] / scalar;
    return vecOut;
}


// overload dot product operator to provide a vector scalar product
float Vector2F::operator*(const Vector2F &vecIn)
{
	float res = 0.0f;

	res = data[0] * vecIn[0];
	res += data[1] * vecIn[1];

	return res;
}

// overload dot product operator to provide a vector scalar product
float Vector2F::operator*(const Vector2F &vecIn) const
{
	float res = 0.0f;

	res = data[0] * vecIn[0];
	res += data[1] * vecIn[1];

	return res;
}


Vector2F Vector2F::operator-() const
{
	Vector2F res;

	res[0] = -data[0];
	res[1] = -data[1];

    return res;
}


Vector2F Vector2F::EMult(const Vector2F &vecIn)
{
	Vector2F res;

	res[0] = data[0] * vecIn[0];
	res[1] = data[1] * vecIn[1];

	return res;
}



void Vector2F::Constrain(uint32 i, float min, float max)
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



Vector2F Vector2F::Normalized(void)
{
	Vector2F res;
	float mag = Length();

	res[0] = data[0] / mag;
	res[1] = data[1] / mag;

	return res;
}

Matrix1F2 Vector2F::Transpose(void)
{
    Matrix1F2 matOut;
    matOut.Zero();

    matOut[0][0] = data[0];
    matOut[0][1] = data[1];

    return matOut; 
}

