#include <math/Matrix10F1.hpp>
#include <math.h>

using namespace math;


Matrix10F1::Matrix10F1(
        Vector1F m0, Vector1F m1, Vector1F m2, Vector1F m3, Vector1F m4, 
        Vector1F m5, Vector1F m6, Vector1F m7, Vector1F m8, Vector1F m9) :
	nan{NAN, NAN, NAN, NAN, NAN, NAN, NAN, NAN, NAN, NAN},
	data{m0, m1, m2, m3, m4, m5, m6, m7, m8, m9}
{
};


Matrix10F1::Matrix10F1() :
    nan{NAN, NAN, NAN, NAN, NAN, NAN, NAN, NAN, NAN, NAN},
    data{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}
{
};


Matrix10F1::~Matrix10F1()
{
};


Matrix1F10 Matrix10F1::Transpose(void)
{
    Matrix1F10 matOut;
    matOut[0][0] = data[0][0];
    matOut[0][1] = data[1][0];
    matOut[0][2] = data[2][0];
    matOut[0][3] = data[3][0];
    matOut[0][4] = data[4][0];
    matOut[0][5] = data[5][0];
    matOut[0][6] = data[6][0];
    matOut[0][7] = data[7][0];
    matOut[0][8] = data[8][0];
    matOut[0][9] = data[9][0];

    return matOut;
};


Vector1F& Matrix10F1::operator [] (uint32 i)
{
    if(i >= 10)
    {
        return nan[0];
    }
    else
    {
        return data[i];
    }
};


Vector1F Matrix10F1::operator [] (uint32 i) const
{
    if(i >= 10)
    {
        return nan[0];
    }
    else
    {
        return data[i];
    }
};


Matrix10F1 Matrix10F1::operator*(const Matrix1F1 & matIn)
{
    Matrix10F1 matOut;
    int i = 0;
    /* Rows in matrix 1 */
    int rows1 = 10;

    for(i = 0; i < rows1; i++)
    {
        matOut[i][0] = data[i][0] * matIn[0][0];
    }

    return matOut;
}


Matrix10F1 Matrix10F1::operator*(const Vector1F & vecIn)
{
    Matrix10F1 matOut;
    int i = 0;
    /* Rows in matrix 1 */
    int rows1 = 10;

    for(i = 0; i < rows1; i++)
    {
        matOut[i][0] = data[i][0] * vecIn[0];
    }

    return matOut;
}


Matrix10F10 Matrix10F1::operator*(const Matrix1F10 & matIn)
{
    Matrix10F10 matOut;
    int i, j, k = 0;
    /* Rows in matrix 1 */
    int rows1 = 10;
    /* Columns in matrix 1 */
    int cols1 = 1;
    /* Columns in matrix 2 */
    int cols2 = 10;

    for(i = 0; i < rows1; i++)
    {
        for(j = 0; j < cols2; j++)
        {
            for(k = 0; k < cols1; k++)
            {
                matOut[i][j] += data[i][k] * matIn[k][j];
            }
        }
    }

    return matOut;
}


Vector10F Matrix10F1::ToVector(void)
{
    math::Vector10F vecOut(
            data[0][0], data[1][0], data[2][0], data[3][0], data[4][0],
            data[5][0], data[6][0], data[7][0], data[8][0], data[9][0]
    );

    return vecOut;
}


void Matrix10F1::Zero(void)
{
    data[0][0] = 0.0f;
    data[1][0] = 0.0f;
    data[2][0] = 0.0f;
    data[3][0] = 0.0f;
    data[4][0] = 0.0f;
    data[5][0] = 0.0f;
    data[6][0] = 0.0f;
    data[7][0] = 0.0f;
    data[8][0] = 0.0f;
    data[9][0] = 0.0f;
}
