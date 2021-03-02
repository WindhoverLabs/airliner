#include <math/Matrix6F10.hpp>
#include <math.h>

using namespace math;

Matrix6F10::Matrix6F10(Vector10F m0, Vector10F m1, Vector10F m2, Vector10F m3,
                       Vector10F m4, Vector10F m5) :
	nan{NAN,NAN,NAN,NAN,NAN,NAN,NAN,NAN,NAN,NAN},
	data{m0, m1, m2, m3, m4, m5}
{
};


Matrix6F10::Matrix6F10() :
    nan{NAN,NAN,NAN,NAN,NAN,NAN,NAN,NAN,NAN,NAN},
	data{
		{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
		{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
		{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
		{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}
	}
{
};


Matrix6F10::~Matrix6F10()
{
};


void Matrix6F10::Zero(void)
{
	for(int i = 0; i < ROWS; i++)
	{
		for(int j = 0; j < COLS; j++)
		{
			data[i][j] = 0.0f;
		}
	}
}


Vector10F& Matrix6F10::operator [] (uint32 i)
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


Vector10F Matrix6F10::operator [] (uint32 i) const
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


// overload * operator to provide a matrix vector product
Vector6F Matrix6F10::operator*(const Vector10F &vecIn)
{
    Vector6F vecOut;
    vecOut.Zero();

    int i, k = 0;
    /* Rows in matrix 1 */
    int rows1 = 6;
    /* Columns in matrix 1 */
    int cols1 = 10;

    for(i = 0; i < rows1; i++)
    {
        for(k = 0; k < cols1; k++)
        {
            vecOut[i] += data[i][k] * vecIn[k];
        }
    }

    return vecOut;
}



Matrix6F6 Matrix6F10::operator*(const Matrix10F6 &matIn)
{
    Matrix6F6 matOut;
    float value = 0.0f;

    /* Rows in matrix 1 */
    int rows1 = 6;
    /* Columns in matrix 1 */
    int cols1 = 10;
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


Matrix6F10 Matrix6F10::operator*(const Matrix10F10 &matIn)
{
    Matrix6F10 matOut;
    float value = 0.0f;

    /* Rows in matrix 1 */
    int rows1 = 6;
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


Matrix10F6 Matrix6F10::Transpose(void)
{
    int i, j = 0;
    math::Matrix10F6 res;

	for(i = 0; i < COLS; i++)
	{
		for(j = 0; j < ROWS; j++)
		{
			res[i][j] = data[j][i];
		}
	}

	return res;
}

