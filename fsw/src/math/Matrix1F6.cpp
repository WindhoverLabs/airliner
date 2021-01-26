#include <math/Matrix1F6.hpp>
#include <math.h>

using namespace math;

Matrix1F6::Matrix1F6(Vector6F m0) :
	data{m0},
	nan{NAN, NAN, NAN, NAN, NAN, NAN}
{
};


Matrix1F6::Matrix1F6() :
    data{
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0}
    },
    nan{NAN, NAN, NAN, NAN, NAN, NAN}
{
};


Matrix1F6::~Matrix1F6()
{
};


Vector6F& Matrix1F6::operator [] (uint32 i)
{
    if(i >= COLS)
    {
        return nan;
    }
    else
    {
        return data[i];
    }
};


Vector6F Matrix1F6::operator [] (uint32 i) const
{
    if(i >= COLS)
    {
        return nan;
    }
    else
    {
        return data[i];
    }
};


void Matrix1F6::Zero(void)
{
    int i = 0;

    for(i = 0; i < COLS; i++)
    {
        data[0][i] = 0.0f;
    }
};


// overload * operator to provide a matrix product
Matrix1F1 Matrix1F6::operator*(const Matrix6F1 &matIn)
{
    Matrix1F1 matOut;
    matOut.Zero();

    int k = 0;
    /* Columns in matrix 1 */
    int cols1 = COLS;

    for(k = 0; k < cols1; k++)
    {
        matOut[0][0] += data[0][k] * matIn[k][0];
    }

    return matOut;
};



