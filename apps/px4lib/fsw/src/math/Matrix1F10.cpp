#include <math/Matrix1F10.hpp>
#include <math.h>

using namespace math;

Matrix1F10::Matrix1F10(Vector10F m0) :
	data{m0},
	nan{NAN, NAN, NAN, NAN, NAN, NAN, NAN, NAN, NAN, NAN}
{
};


Matrix1F10::Matrix1F10() :
    data{
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}
    },
    nan{NAN, NAN, NAN, NAN, NAN, NAN, NAN, NAN, NAN, NAN}
{
};


Matrix1F10::~Matrix1F10()
{
};


Vector10F& Matrix1F10::operator [] (uint32 i)
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


Vector10F Matrix1F10::operator [] (uint32 i) const
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


void Matrix1F10::Zero(void)
{
    int i = 0;

    for(i = 0; i < COLS; i++)
    {
        data[0][i] = 0.0f;
    }
};


Matrix10F1 Matrix1F10::Transpose(void)
{
    Matrix10F1 matOut;

    matOut[0][0] = data[0][0];
    matOut[1][0] = data[0][1];
    matOut[2][0] = data[0][2];
    matOut[3][0] = data[0][3];
    matOut[4][0] = data[0][4];
    matOut[5][0] = data[0][5];
    matOut[6][0] = data[0][6];
    matOut[7][0] = data[0][7];
    matOut[8][0] = data[0][8];
    matOut[9][0] = data[0][9];
    
    return matOut;
};


// overload * operator to provide a matrix product
Matrix1F10 Matrix1F10::operator*(const Matrix10F10 &matIn)
{
    Matrix1F10 matOut;
    matOut.Zero();

    int i, j, k = 0;
    /* Rows in matrix 1 */
    int rows1 = 1;
    /* Columns in matrix 1 */
    int cols1 = 10;
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
};


// overload * operator to provide a matrix product
Matrix1F1 Matrix1F10::operator*(const Matrix10F1 &matIn)
{
    Matrix1F1 matOut;
    matOut.Zero();

    int i, j, k = 0;
    /* Rows in matrix 1 */
    int rows1 = 1;
    /* Columns in matrix 1 */
    int cols1 = 10;
    /* Columns in matrix 2 */
    int cols2 = 1;

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


// overload * operator to provide a matrix vector product
Vector1F Matrix1F10::operator*(const Vector10F &vecIn)
{
    Vector1F vecOut;
    vecOut[0] = 0.0;

    int k = 0;

    for(k = 0; k < COLS; k++)
    {
                vecOut[0] += data[0][k] * vecIn[k];
    }

    return vecOut;
}

