#include <math/Matrix4F4.hpp>
#include <math.h>

using namespace math;

Matrix4F4::Matrix4F4(Vector4F m0, Vector4F m1, Vector4F m2, Vector4F m3) :
	data{m0, m1, m2, m3},
	nan{NAN,NAN,NAN,NAN}
{
};


Matrix4F4::Matrix4F4() :
	data{
		{0.0, 0.0, 0.0, 0.0},
		{0.0, 0.0, 0.0, 0.0},
		{0.0, 0.0, 0.0, 0.0},
		{0.0, 0.0, 0.0, 0.0}
	},
	nan{NAN,NAN,NAN,NAN}
{
};


Matrix4F4::~Matrix4F4()
{
};


Vector4F& Matrix4F4::operator [] (uint32 i)
{
	if(i >= 4)
	{
		return nan;
	}
	else
	{
		return data[i];
	}
};


Vector4F Matrix4F4::operator [] (uint32 i) const
{
	if(i >= 4)
	{
		return nan;
	}
	else
	{
		return data[i];
	}
};


/* multiplication by a vector */
Vector4F Matrix4F4::operator*(const Vector4F &v) const
{
    Vector4F res(data[0][0] * v[0] + data[0][1] * v[1] + data[0][2] * v[2] + data[0][3] * v[3],
                 data[1][0] * v[0] + data[1][1] * v[1] + data[1][2] * v[2] + data[1][3] * v[3],
                 data[2][0] * v[0] + data[2][1] * v[1] + data[2][2] * v[2] + data[2][3] * v[3],
                 data[3][0] * v[0] + data[3][1] * v[1] + data[3][2] * v[2] + data[3][3] * v[3]);
    return res;
}




