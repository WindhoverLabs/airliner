#include "math/Matrix10F3.hpp"
#include <math.h>

using namespace math;


Matrix10F3::Matrix10F3(Vector3F m0, Vector3F m1, Vector3F m2, Vector3F m3, Vector3F m4, Vector3F m5, Vector3F m6, Vector3F m7, Vector3F m8, Vector3F m9) :
	nan{NAN,NAN,NAN},
	data{m0, m1, m2, m3, m4, m5, m6, m7, m8, m9}
{
};


Matrix10F3::Matrix10F3() :
    nan{NAN,NAN,NAN},
	data{
		{0.0, 0.0, 0.0},
		{0.0, 0.0, 0.0},
		{0.0, 0.0, 0.0},
		{0.0, 0.0, 0.0},
		{0.0, 0.0, 0.0},
		{0.0, 0.0, 0.0},
		{0.0, 0.0, 0.0},
		{0.0, 0.0, 0.0},
		{0.0, 0.0, 0.0},
		{0.0, 0.0, 0.0}
	}
{
};


Matrix10F3::~Matrix10F3()
{
};


Vector3F& Matrix10F3::operator [] (uint32 i)
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


Vector3F Matrix10F3::operator [] (uint32 i) const
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


Matrix3F10 Matrix10F3::Transpose(void)
{
	Matrix3F10 res;

	for(int i = 0; i < ROWS; i++)
	{
		for(int j = 0; j < COLS; j++)
		{
			res[j][i] = data[i][j];
		}
	}

	return res;
}


void Matrix10F3::Zero(void)
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
Matrix10F3 Matrix10F3::operator*(const Matrix3F3 &matIn)
{
    Matrix10F3 matOut;
    matOut.Zero();

    // TODO: verify correct
    for(int i = 0; i < ROWS; i++)
    {
    	for(int j = 0; j < COLS; j++)
		{

    		for(int k = 0; k < 3; k++)
            {
    			matOut[i][j] += data[i][k] * matIn[k][j];
			}
		}
    }

    return matOut;
}

// overload * operator to provide a matrix product
Matrix10F10 Matrix10F3::operator*(const Matrix3F10 &matIn)
{
    Matrix10F10 matOut;
    matOut.Zero();

    // TODO: verify correct
    for(int i = 0; i < ROWS; i++)
    {
    	for(int k = 0; k < 10; k++)
		{

    		for(int j = 0; j < 3; j++)
			{
    			matOut[i][k] += data[i][j] * matIn[j][k];
			}
		}
    }

    return matOut;
}


// overload * operator to provide a matrix vector product
Vector10F Matrix10F3::operator*(const Vector3F &vecIn)
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


Matrix10F3 Matrix10F3::operator*(const float &scalar)
{
    Matrix10F3 matOut;

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


Matrix10F3 Matrix10F3::operator+(const Matrix10F3 &matIn) const
{
    Matrix10F3 matOut;

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

