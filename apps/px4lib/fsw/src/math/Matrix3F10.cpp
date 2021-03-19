#include "math/Matrix3F10.hpp"
#include <math.h>

using namespace math;

Matrix3F10::Matrix3F10(Vector10F m0, Vector10F m1, Vector10F m2) :
	nan{NAN,NAN,NAN,NAN,NAN,NAN,NAN,NAN,NAN,NAN},
	data{m0, m1, m2}
{
};


Matrix3F10::Matrix3F10() :
    nan{NAN,NAN,NAN,NAN,NAN,NAN,NAN,NAN,NAN,NAN},
	data{
		{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
		{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
		{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
	}
{
};


Matrix3F10::~Matrix3F10()
{
};


Vector10F& Matrix3F10::operator [] (uint32 i)
{
	if(i >= ROWS)
	{
		return nan;
	}
	else
	{
		return data[i];
	}
};


Vector10F Matrix3F10::operator [] (uint32 i) const
{
	if(i >= ROWS)
	{
		return nan;
	}
	else
	{
		return data[i];
	}
};


Matrix10F3 Matrix3F10::Transpose(void)
{
	Matrix10F3 res;
    res.Zero();

	for(int i = 0; i < ROWS; i++)
	{
		for(int j = 0; j < COLS; j++)
		{
			res[j][i] = data[i][j];
		}
	}

	return res;
}


void Matrix3F10::Zero(void)
{
	for(int i = 0; i < ROWS; i++)
	{
    	for(int j = 0; j < COLS; j++)
		{
			data[i][j] = 0.0f;
		}
	}
}


// overload * operator to provide a matrix product
Matrix3F3 Matrix3F10::operator*(const Matrix10F3 &matIn)
{
    Matrix3F3 matOut;
    matOut.Zero();

    // TODO: verify correct
    for(int i = 0; i < ROWS; i++)
    {
    	for(int k = 0; k < 3; k++)
		{

    		for(int j = 0; j < COLS; j++)
			{
    			matOut[i][k] += data[i][j] * matIn[j][k];
			}
		}
    }

    return matOut;
}


// overload * operator to provide a matrix vector product
Vector3F Matrix3F10::operator*(const Vector10F &vecIn)
{
    Vector3F vecOut;

    // TODO: verify correct
    for(int i = 0; i < ROWS; i++)
    {
    	for(int j = 0; j < COLS; j++)
		{
    		vecOut[i] += data[i][j]*vecIn[j];
		}
    }

    return vecOut;
}

