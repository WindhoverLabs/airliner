#include "math/Matrix10F2.hpp"
#include <math.h>

using namespace math;


Matrix10F2::Matrix10F2(Vector2F m0, Vector2F m1, Vector2F m2, Vector2F m3, Vector2F m4, Vector2F m5, Vector2F m6, Vector2F m7, Vector2F m8, Vector2F m9) :
	data{m0, m1, m2, m3, m4, m5, m6, m7, m8, m9},
	nan{NAN,NAN}
{
};


Matrix10F2::Matrix10F2() :
	data{
		{0.0, 0.0},
		{0.0, 0.0},
		{0.0, 0.0},
		{0.0, 0.0},
		{0.0, 0.0},
		{0.0, 0.0},
		{0.0, 0.0},
		{0.0, 0.0},
		{0.0, 0.0},
		{0.0, 0.0},
	},
    nan{NAN,NAN}
{
};


Matrix10F2::~Matrix10F2()
{
};


Vector2F& Matrix10F2::operator [] (uint32 i)
{
	if(i >= 10)
	{
		return nan;
	}
	else
	{
		return data[i];
	}
};


Vector2F Matrix10F2::operator [] (uint32 i) const
{
	if(i >= 10)
	{
		return nan;
	}
	else
	{
		return data[i];
	}
};


Matrix2F10 Matrix10F2::Transpose(void)
{
	Matrix2F10 res;

	for(int i = 0; i < ROWS; i++)
	{
		for(int j = 0; j < COLS; j++)
		{
			res[j][i] = data[i][j];
		}
	}

	return res;
}


void Matrix10F2::Zero(void)
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
Matrix10F2 Matrix10F2::operator*(const Matrix2F2 &matIn)
{
    Matrix10F2 matOut;
    matOut.Zero();

    for(int i = 0; i < ROWS; i++)
    {
    	for(int j = 0; j < COLS; j++)
		{

    		for(int k = 0; k < 2; k++)
            {
    			matOut[i][j] += data[i][k] * matIn[k][j];
			}
		}
    }

    return matOut;
}

// overload * operator to provide a matrix product
Matrix10F10 Matrix10F2::operator*(const Matrix2F10 &matIn)
{
    Matrix10F10 matOut;
    matOut.Zero();

    for(int i = 0; i < ROWS; i++)
    {
    	for(int k = 0; k < 10; k++)
		{

    		for(int j = 0; j < 2; j++)
			{
    			matOut[i][k] += data[i][j] * matIn[j][k];
			}
		}
    }

    return matOut;
}


// overload * operator to provide a matrix vector product
Vector10F Matrix10F2::operator*(const Vector2F &vecIn)
{
    Vector10F vecOut;
    vecOut.Zero();

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


Matrix10F2 Matrix10F2::operator*(const float &scalar)
{
    Matrix10F2 matOut;

    // TODO: verify correct
    for(int i = 0; i < ROWS; i++)
    {
    	for(int j = 0; j < COLS; j++)
		{
    		matOut[i][j] = scalar*data[i][j];
		}
    }

    return matOut;
}


Matrix10F2 Matrix10F2::operator+(const Matrix10F2 &matIn) const
{
    Matrix10F2 matOut;

    // TODO: verify correct
	for(int i = 0; i < ROWS; i++)
	{
		for(int j = 0; j < COLS; j++)
		{
			matOut[i][j] = data[i][j] + matIn[i][j];
		}
	}

    return matOut;
}

