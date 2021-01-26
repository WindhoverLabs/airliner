#include <math/Matrix2F2.hpp>
#include <math.h>

using namespace math;

Matrix2F2::Matrix2F2(Vector2F m0, Vector2F m1) :
	data{m0, m1},
	nan{NAN,NAN}
{
};


Matrix2F2::Matrix2F2() :
	data{
		{0.0, 0.0},
		{0.0, 0.0}
    },
    nan{NAN,NAN}
{
};


Matrix2F2::~Matrix2F2()
{
};


Vector2F& Matrix2F2::operator [] (uint32 i)
{
	if(i >= SIZE)
	{
		return nan;
	}
	else
	{
		return data[i];
	}
};


Vector2F Matrix2F2::operator [] (uint32 i) const
{
	if(i >= SIZE)
	{
		return nan;
	}
	else
	{
		return data[i];
	}
};


Matrix2F2 Matrix2F2::Transpose(void)
{
	Matrix2F2 res;

	res[0][0] = data[0][0];
	res[0][1] = data[1][0];
	res[1][0] = data[0][1];
	res[1][1] = data[1][1];

	return res;
}


Matrix2F2 Matrix2F2::Identity() {
    Matrix2F2 matOut(
    		{1.0f, 0.0f},
    		{0.0f, 1.0f}
    		);

    return matOut;
}


void Matrix2F2::Zero(void)
{
	data[0][0] = 0.0f;
	data[0][1] = 0.0f;
	data[1][0] = 0.0f;
	data[1][1] = 0.0f;
}


// overload * operator to provide a matrix product
Matrix2F2 Matrix2F2::operator*(const Matrix2F2 &matIn)
{
    Matrix2F2 matOut;

    matOut[0][0] = data[0][0]*matIn[0][0] + data[0][1]*matIn[1][0];
    matOut[0][1] = data[0][0]*matIn[0][1] + data[0][1]*matIn[1][1];

    matOut[1][0] = data[1][0]*matIn[0][0] + data[1][1]*matIn[1][0];
    matOut[1][1] = data[1][0]*matIn[0][1] + data[1][1]*matIn[1][1];

    return matOut;
}


// overload * operator to provide a matrix vector product
Vector2F Matrix2F2::operator*(const Vector2F &vecIn)
{
    Vector2F vecOut;
    vecOut[0] = data[0][0]*vecIn[0] + data[0][1]*vecIn[1];
    vecOut[1] = data[1][0]*vecIn[0] + data[1][1]*vecIn[1];
    return vecOut;
}


Matrix2F2 Matrix2F2::operator*(const float &scalar)
{
    Matrix2F2 matOut;

    matOut[0][0] = scalar*data[0][0];
    matOut[1][0] = scalar*data[1][0];

    matOut[0][1] = scalar*data[0][1];
    matOut[1][1] = scalar*data[1][1];

    return matOut;
}


Matrix2F2 Matrix2F2::operator+(const Matrix2F2 &matIn) const
{
    Matrix2F2 matOut;

    matOut[0][0] = data[0][0] + matIn[0][0];
    matOut[1][0] = data[1][0] + matIn[1][0];

    matOut[0][1] = data[0][1] + matIn[0][1];
    matOut[1][1] = data[1][1] + matIn[1][1];

    return matOut;
}

/**
 * inverse based on LU factorization with partial pivotting
 */
Matrix2F2 Matrix2F2::Inversed(void)
{
    Matrix2F2 L(
            {1.0f, 0.0f},
            {0.0f, 1.0f}
    );
    Matrix2F2 P(
            {1.0f, 0.0f},
            {0.0f, 1.0f}
    );
    Matrix2F2 U;

    uint8 n, i, j, k, c = 0;
    boolean success = TRUE;

    U = *this;
    
    for (n = 0; n < SIZE; n++)
    {
        /* if diagonal is zero, swap with row below */
        if(fabs(U[n][n]) < 1e-8f)
        {
            for (i = n + 1; i < SIZE; i++)
            {
                if(fabs(U[i][n]) > 1e-8f)
                {
                    U.SwapRows(i, n);
                    P.SwapRows(i, n);
                    L.SwapRows(i, n);
                    L.SwapCols(i, n);
                    break;
                }
            }
        }
        
        if(fabs(U[n][n]) < 1e-8f)
        {
            /* failsafe, return zero matrix */
            success = FALSE;
            goto end_of_function;
        }
        
        for(i = (n + 1); i < SIZE; i++)
        {
            L[i][n] = U[i][n] / U[n][n];
            /* add i-th row and n-th row */
            /* multiplied by: -a(i,n/a(n,n) */
            for(k = n; k < SIZE; k++)
            {
                U[i][k] = U[i][k] - L[i][n] * U[n][k];
            }
        }
    }

    /* for all columns of Y */
    for(c = 0; c < SIZE; c++)
    {
        /* for all rows of L */
        for(i = 0; i < SIZE; i++)
        {
            /* for all columns of L */
            for(j = 0; j < i; j++)
            {
                /* for all existing y
                 * subtract the component they
                 * contribute to the solution
                 */
                P[i][c] = P[i][c] - L[i][j] * P[j][c];
            }
        }
    }
    
    /* for all columns of X */
    for (c = 0; c < SIZE; c++) 
    {
        /* for all rows of U */
        for (k = 0; k < SIZE; k++) 
        {
            /* have to go in reverse order */
            i = SIZE - 1 - k;

            /* for all columns of U */
            for (j = i + 1; j < SIZE; j++) 
            {
                // for all existing x
                // subtract the component they
                // contribute to the solution
                P[i][c] = P[i][c] - U[i][j] * P[j][c];
            }

            /* divide by the factor
             * on current
             * term to be solved
             *
             * we know that U(i, i) != 0 from above
             */
             P[i][c] = P[i][c] / U[i][i];
        }
    }

    /* check sanity of results */
    for(i = 0; i < SIZE; i++)
    {
        for(j = 0; j < SIZE; j++)
        {
            if(!isfinite(P[i][j]))
            {
                success = FALSE;
                goto end_of_function;
            }
        }
    }

end_of_function:

    if(TRUE == success)
    {
        return P;
    }
    else
    {
        Matrix2F2 Zero;
        Zero.Zero();
        return Zero;
    }
}


void Matrix2F2::SwapRows(uint8 a, uint8 b)
{
    uint8 j = 0;
    float tmp = 0;

    if(a == b)
    {
        goto end_of_function;
    }
    
    for(j = 0; j< SIZE; j++)
    {
        tmp = data[a][j];
        data[a][j] = data[b][j];
        data[b][j] = tmp;
    }
    
end_of_function:
;
}


void Matrix2F2::SwapCols(uint8 a, uint8 b)
{
    uint8 i = 0;
    float tmp = 0;

    if(a == b)
    {
        goto end_of_function;
    }
    
    for(i = 0; i< SIZE; i++)
    {
        tmp = data[i][a];
        data[i][a] = data[i][b];
        data[i][b] = tmp;
    }
    
end_of_function:
;
}



