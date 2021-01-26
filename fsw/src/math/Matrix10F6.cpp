#include "math/Matrix10F6.hpp"
#include <math.h>

using namespace math;


Matrix10F6::Matrix10F6(Vector6F m0, Vector6F m1, Vector6F m2, 
                       Vector6F m3, Vector6F m4, Vector6F m5, 
                       Vector6F m6, Vector6F m7, Vector6F m8, 
                       Vector6F m9) :
	data{m0, m1, m2, m3, m4, m5, m6, m7, m8, m9},
	nan{NAN,NAN,NAN,NAN,NAN,NAN}
{
};


Matrix10F6::Matrix10F6() :
	data{
		{0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
		{0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
		{0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
		{0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
		{0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
		{0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
		{0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
		{0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
		{0.0, 0.0, 0.0, 0.0, 0.0, 0.0}
	},
	nan{NAN,NAN,NAN,NAN,NAN,NAN}
{
};


Matrix10F6::~Matrix10F6()
{
};


Vector6F& Matrix10F6::operator [] (uint32 i)
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


Vector6F Matrix10F6::operator [] (uint32 i) const
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


// overload * operator to provide a matrix product
Matrix10F6 Matrix10F6::operator*(const Matrix6F6 &matIn)
{
    Matrix10F6 matOut;
    float value = 0.0f;

    /* Rows in matrix 1 */
    int rows1 = 10;
    /* Columns in matrix 1 */
    int cols1 = 6;
    /* Columns in matrix 2 */
    int cols2 = 6
;
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


// overload * operator to provide a matrix vector product
Vector10F Matrix10F6::operator*(const Vector6F &vecIn)
{
    Vector10F vecOut;
    float value = 0.0f;

    /* Rows in matrix 1 */
    int rows1 = 10;
    /* Columns in matrix 1 */
    int cols1 = 6;

    // TODO: verify correct
    for(int i = 0; i < rows1; i++)
    {
        for(int j = 0; j < cols1; j++)
        {
            value += data[i][j] * vecIn[j];
        }
        vecOut[i] = value;
        value = 0.0f;
    }

    return vecOut;
}


// overload * operator to provide a matrix product
Matrix10F10 Matrix10F6::operator*(const Matrix6F10 &matIn)
{
    Matrix10F10 matOut;
    float value = 0.0f;

    /* Rows in matrix 1 */
    int rows1 = 10;
    /* Columns in matrix 1 */
    int cols1 = 6;
    /* Columns in matrix 2 */
    int cols2 = 10
;
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


void Matrix10F6::Zero(void)
{
	for(int i = 0; i < ROWS; i++)
	{
    	for(int j = 0; j < COLS; j++)
		{
			data[i][j] = 0.0f;
		}
	}
}
