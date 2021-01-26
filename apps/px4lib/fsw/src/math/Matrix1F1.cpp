#include <math/Matrix1F1.hpp>
#include <math.h>

using namespace math;

Matrix1F1::Matrix1F1(Vector1F m0) :
    data{m0},
    nan(NAN)
{
};


Matrix1F1::Matrix1F1() :
    data{0.0},
    nan(NAN)
{
};


Matrix1F1::~Matrix1F1()
{
};


Vector1F& Matrix1F1::operator [] (uint32 i)
{
    if(i >= SIZE)
    {
        return nan;
    }
    else
    {
        return data[0];
    }
};


Vector1F Matrix1F1::operator [] (uint32 i) const
{
    if(i >= SIZE)
    {
        return nan;
    }
    else
    {
        return data[0];
    }
};


Matrix1F1 Matrix1F1::operator+(const Matrix1F1 &matIn) const
{
    Matrix1F1 matOut;
    matOut[0][0] = data[0][0] + matIn[0][0];
    return matOut;
}


void Matrix1F1::Zero()
{
    data[0] = 0.0f;
};

