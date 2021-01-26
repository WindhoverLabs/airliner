#include <math/Matrix1F2.hpp>
#include <math.h>

using namespace math;


Matrix1F2::Matrix1F2(Vector2F m0) :
	data{m0},
	nan{NAN,NAN}
{
};


Matrix1F2::Matrix1F2() :
	data{
		{0.0, 0.0}
    },
    nan{NAN,NAN}
{
};


Matrix1F2::~Matrix1F2()
{
};


Vector2F& Matrix1F2::operator [] (uint32 i)
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


Vector2F Matrix1F2::operator [] (uint32 i) const
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

void Matrix1F2::Zero(void)
{
	data[0][0] = 0.0f;
	data[0][1] = 0.0f;
}

// overload * operator to provide a matrix vector product
float Matrix1F2::operator*(const Vector2F &vecIn)
{
    float valOut;

    valOut = data[0][0]*vecIn[0] + data[0][1]*vecIn[1];

    return valOut;
}

