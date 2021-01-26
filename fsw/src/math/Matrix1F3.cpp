#include <math/Matrix1F3.hpp>
#include <math.h>

using namespace math;


Matrix1F3::Matrix1F3(Vector3F m0) :
	data{m0},
	nan{NAN,NAN,NAN}
{
};


Matrix1F3::Matrix1F3() :
	data{
		{0.0, 0.0, 0.0}
    },
    nan{NAN,NAN,NAN}
{
};


Matrix1F3::~Matrix1F3()
{
};


Vector3F& Matrix1F3::operator [] (uint32 i)
{
	if(i >= 1)
	{
		return nan;
	}
	else
	{
		return data[i];
	}
};


Vector3F Matrix1F3::operator [] (uint32 i) const
{
	if(i >= 1)
	{
		return nan;
	}
	else
	{
		return data[i];
	}
};

void Matrix1F3::Zero(void)
{
	data[0][0] = 0.0f;
	data[0][1] = 0.0f;
	data[0][2] = 0.0f;
}

// overload * operator to provide a matrix vector product
float Matrix1F3::operator*(const Vector3F &vecIn)
{
    float valOut;

    valOut = data[0][0]*vecIn[0] + data[0][1]*vecIn[1] + data[0][2]*vecIn[2];

    return valOut;
}

