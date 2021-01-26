#include "math/Matrix2F10.hpp"
#include <math.h>

using namespace math;

Matrix2F10::Matrix2F10(Vector10F m0, Vector10F m1) :
	data{m0, m1},
	nan{NAN,NAN,NAN,NAN,NAN,NAN,NAN,NAN,NAN,NAN}
{
};


Matrix2F10::Matrix2F10() :
	data{
		{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
		{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}
	},
    nan{NAN,NAN,NAN,NAN,NAN,NAN,NAN,NAN,NAN,NAN}
{
};


Matrix2F10::~Matrix2F10()
{
};


Vector10F& Matrix2F10::operator [] (uint32 i)
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


Vector10F Matrix2F10::operator [] (uint32 i) const
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


Matrix10F2 Matrix2F10::Transpose(void)
{
	Matrix10F2 res;
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


void Matrix2F10::Zero(void)
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
Matrix2F2 Matrix2F10::operator*(const Matrix10F2 &matIn)
{
    Matrix2F2 matOut;
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
Vector2F Matrix2F10::operator*(const Vector10F &vecIn)
{
    Vector2F vecOut;
    vecOut.Zero();

    for(int i = 0; i < ROWS; i++)
    {
    	for(int j = 0; j < COLS; j++)
		{
    		vecOut[i] += data[i][j]*vecIn[j];
		}
    }

    return vecOut;
}

Matrix2F10 Matrix2F10::operator*(const Matrix10F10 &matIn)
{
    Matrix2F10 matOut;
    float value = 0.0f;

    /* Rows in matrix 1 */
    int rows1 = 2;
    /* Columns in matrix 1 */
    int cols1 = 10;
    /* Columns in matrix 2 */
    int cols2 = 10;

    // TODO: verify correct
    for(int i = 0; i < rows1; i++)
    {
    	for(int j = 0; j < cols2; j++)
		{
    		for(int k = 0; k < cols1; k++)
			{
    			value += data[i][k] * matIn[k][j];
			}

    		matOut[i][j] = value;
    		value = 0.0f;
		}
    }

    return matOut;
}

