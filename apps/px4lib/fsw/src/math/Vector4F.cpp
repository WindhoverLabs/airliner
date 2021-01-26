#include <math/Vector4F.hpp>
#include <math.h>

using namespace math;

Vector4F::Vector4F(float m1, float m2, float m3, float m4) :
    data{m1, m2, m3, m4},
	nan(NAN)
{
};


Vector4F::Vector4F()
{
};


Vector4F::~Vector4F()
{
};


float& Vector4F::operator [] (uint32 i)
{
	if(i < SIZE)
	{
		return data[i];
	}

	return nan;
};


float Vector4F::operator [] (uint32 i) const
{
	if(i < SIZE)
	{
		return data[i];
	}

	return NAN;
};


float Vector4F::Length() const
{
    return sqrtf(data[0]*data[0] + data[1]*data[1] + data[2]*data[2] + data[3]*data[3]);
}


float Vector4F::LengthSquared() const
{
    return (data[0]*data[0] + data[1]*data[1] + data[2]*data[2] + data[3]*data[3]);
}


void Vector4F::Zero()
{
    data[0] = 0.0f;
    data[1] = 0.0f;
    data[2] = 0.0f;
    data[3] = 0.0f;
}


// overload + operator to provide a vector addition
const Vector4F Vector4F::operator+(const Vector4F &vecIn) const
{
    Vector4F vecOut;
    vecOut[0] = data[0] + vecIn[0];
    vecOut[1] = data[1] + vecIn[1];
    vecOut[2] = data[2] + vecIn[2];
    vecOut[3] = data[3] + vecIn[3];
    return vecOut;
}


// overload - operator to provide a vector subtraction
const Vector4F Vector4F::operator-(const Vector4F &vecIn) const
{
    Vector4F vecOut;
    vecOut[0] = data[0] - vecIn[0];
    vecOut[1] = data[1] - vecIn[1];
    vecOut[2] = data[2] - vecIn[2];
    vecOut[3] = data[3] - vecIn[3];
    return vecOut;
}


// overload * operator to provide a vector scalar product
const Vector4F Vector4F::operator*(const float scalar) const
{
    Vector4F vecOut;
    vecOut[0] = data[0] * scalar;
    vecOut[1] = data[1] * scalar;
    vecOut[2] = data[2] * scalar;
    vecOut[3] = data[3] * scalar;
    return vecOut;
}


// overload / operator to provide a vector scalar division
const Vector4F Vector4F::operator/(const float scalar) const
{
    Vector4F vecOut;
    vecOut[0] = data[0] / scalar;
    vecOut[1] = data[1] / scalar;
    vecOut[2] = data[2] / scalar;
    vecOut[3] = data[3] / scalar;
    return vecOut;
}


// overload dot product operator to provide a vector scalar product
float Vector4F::operator*(const Vector4F &vecIn) const
{
    float res = 0.0f;

    res = data[0] * vecIn[0];
    res += data[1] * vecIn[1];
    res += data[2] * vecIn[2];
    res += data[3] * vecIn[3];
    return res;
}



const Vector4F Vector4F::operator-() const
{
	Vector4F res;

	res[0] = -data[0];
	res[1] = -data[1];
	res[2] = -data[2];
	res[3] = -data[3];

    return res;
}


const Vector4F& Vector4F::operator =(const Vector4F &v)
{

    data[0] = v[0];
    data[1] = v[1];
    data[2] = v[2];
    data[3] = v[3];

    return *this;
}


void Vector4F::Normalize(void)
{
    float mag = Length();

    data[0] = data[0] / mag;
    data[1] = data[1] / mag;
    data[2] = data[2] / mag;
    data[3] = data[3] / mag;
}


Vector4F Vector4F::Normalized(void) const
{
    Vector4F res;
    float mag = Length();

    res[0] = data[0] / mag;
    res[1] = data[1] / mag;
    res[2] = data[2] / mag;
    res[3] = data[3] / mag;

    return res;
}
