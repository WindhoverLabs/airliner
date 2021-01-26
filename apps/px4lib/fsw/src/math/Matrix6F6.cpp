#include <math/Matrix6F6.hpp>
#include <math.h>

using namespace math;

Matrix6F6::Matrix6F6(Vector6F m0, Vector6F m1, Vector6F m2, Vector6F m3,
                       Vector6F m4, Vector6F m5) :
	data{m0, m1, m2, m3, m4, m5},
	nan{
        {NAN,NAN,NAN,NAN,NAN,NAN},
        {NAN,NAN,NAN,NAN,NAN,NAN},
        {NAN,NAN,NAN,NAN,NAN,NAN},
        {NAN,NAN,NAN,NAN,NAN,NAN},
        {NAN,NAN,NAN,NAN,NAN,NAN},
        {NAN,NAN,NAN,NAN,NAN,NAN}
    }
{
};


Matrix6F6::Matrix6F6() :
	data{
		{0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
		{0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
		{0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
		{0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0}
	},
	nan{
        {NAN,NAN,NAN,NAN,NAN,NAN},
        {NAN,NAN,NAN,NAN,NAN,NAN},
        {NAN,NAN,NAN,NAN,NAN,NAN},
        {NAN,NAN,NAN,NAN,NAN,NAN},
        {NAN,NAN,NAN,NAN,NAN,NAN},
        {NAN,NAN,NAN,NAN,NAN,NAN}
    }
{
};


Matrix6F6::~Matrix6F6()
{
};


void Matrix6F6::Zero(void)
{
	for(int i = 0; i < SIZE; i++)
	{
		for(int j = 0; j < SIZE; j++)
		{
			data[i][j] = 0.0f;
		}
	}
}


Vector6F& Matrix6F6::operator [] (uint32 i)
{
	if(i >= SIZE)
	{
		return nan[0];
	}
	else
	{
		return data[i];
	}
};


Vector6F Matrix6F6::operator [] (uint32 i) const
{
	if(i >= SIZE)
	{
		return nan[0];
	}
	else
	{
		return data[i];
	}
};


Matrix6F6 Matrix6F6::operator+(const Matrix6F6 &matIn) const
{
    Matrix6F6 matOut;

    matOut[0][0] = data[0][0] + matIn[0][0];
    matOut[1][0] = data[1][0] + matIn[1][0];
    matOut[2][0] = data[2][0] + matIn[2][0];
    matOut[3][0] = data[3][0] + matIn[3][0];
    matOut[4][0] = data[4][0] + matIn[4][0];
    matOut[5][0] = data[5][0] + matIn[5][0];

    matOut[0][1] = data[0][1] + matIn[0][1];
    matOut[1][1] = data[1][1] + matIn[1][1];
    matOut[2][1] = data[2][1] + matIn[2][1];
    matOut[3][1] = data[3][1] + matIn[3][1];
    matOut[4][1] = data[4][1] + matIn[4][1];
    matOut[5][1] = data[5][1] + matIn[5][1];

    matOut[0][2] = data[0][2] + matIn[0][2];
    matOut[1][2] = data[1][2] + matIn[1][2];
    matOut[2][2] = data[2][2] + matIn[2][2];
    matOut[3][2] = data[3][2] + matIn[3][2];
    matOut[4][2] = data[4][2] + matIn[4][2];
    matOut[5][2] = data[5][2] + matIn[5][2];

    matOut[0][3] = data[0][3] + matIn[0][3];
    matOut[1][3] = data[1][3] + matIn[1][3];
    matOut[2][3] = data[2][3] + matIn[2][3];
    matOut[3][3] = data[3][3] + matIn[3][3];
    matOut[4][3] = data[4][3] + matIn[4][3];
    matOut[5][3] = data[5][3] + matIn[5][3];

    matOut[0][4] = data[0][4] + matIn[0][4];
    matOut[1][4] = data[1][4] + matIn[1][4];
    matOut[2][4] = data[2][4] + matIn[2][4];
    matOut[3][4] = data[3][4] + matIn[3][4];
    matOut[4][4] = data[4][4] + matIn[4][4];
    matOut[5][4] = data[5][4] + matIn[5][4];

    matOut[0][5] = data[0][5] + matIn[0][5];
    matOut[1][5] = data[1][5] + matIn[1][5];
    matOut[2][5] = data[2][5] + matIn[2][5];
    matOut[3][5] = data[3][5] + matIn[3][5];
    matOut[4][5] = data[4][5] + matIn[4][5];
    matOut[5][5] = data[5][5] + matIn[5][5];

    return matOut;
}


Matrix6F1 Matrix6F6::operator*(const Vector6F &vecIn)
{
    math::Matrix6F1 matOut;

    matOut[0][0] = data[0][0]*vecIn[0] + data[0][1]*vecIn[1] + data[0][2]*vecIn[2] + data[0][3]*vecIn[3] + data[0][4]*vecIn[4] + data[0][5]*vecIn[5];
    matOut[1][0] = data[1][0]*vecIn[0] + data[1][1]*vecIn[1] + data[1][2]*vecIn[2] + data[1][3]*vecIn[3] + data[1][4]*vecIn[4] + data[1][5]*vecIn[5];
    matOut[2][0] = data[2][0]*vecIn[0] + data[2][1]*vecIn[1] + data[2][2]*vecIn[2] + data[2][3]*vecIn[3] + data[2][4]*vecIn[4] + data[2][5]*vecIn[5];
    matOut[3][0] = data[3][0]*vecIn[0] + data[3][1]*vecIn[1] + data[3][2]*vecIn[2] + data[3][3]*vecIn[3] + data[3][4]*vecIn[4] + data[3][5]*vecIn[5];
    matOut[4][0] = data[4][0]*vecIn[0] + data[4][1]*vecIn[1] + data[4][2]*vecIn[2] + data[4][3]*vecIn[3] + data[4][4]*vecIn[4] + data[4][5]*vecIn[5];
    matOut[5][0] = data[5][0]*vecIn[0] + data[5][1]*vecIn[1] + data[5][2]*vecIn[2] + data[5][3]*vecIn[3] + data[5][4]*vecIn[4] + data[5][5]*vecIn[5];

    return matOut;
}


Matrix6F6 Matrix6F6::Identity(void)
{
    Matrix6F6 identity(
            {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
            {0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f},
            {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f}
    );
    
    return identity;
}


//void Matrix6F6::getCofactor(const Matrix6F6 &mat, Matrix6F6 &temp, int p, int q, int n)
//{
    //int i = 0, j = 0;

    //// Looping for each element of the matrix
    //for (int row = 0; row < n; row++)
    //{
        //for (int col = 0; col < n; col++)
        //{
            ////  Copying into temporary matrix only those element
            ////  which are not in given row and column
            //if (row != p && col != q)
            //{
                //temp[i][j++] = mat[row][col];
                //// Row is filled, so increase row index and
                //// reset col index
                //if (j == n - 1)
                //{
                    //j = 0;
                    //i++;
                //}
            //}
        //}
    //}
//}



//float Matrix6F6::DeterminantRecursive(const Matrix6F6 &mat, int n)
//{
    //if (n < 1)
    //{
        //return NAN;
    //}

    //float D = 0; // Initialize result
 
    ////  Base case : if matrix contains single element
    //if (n == 1)
    //{
        //return mat[0][0];
    //}

    //Matrix6F6 temp; // To store cofactors
    //temp.Zero();
 
    //float sign = 1;  // To store sign multiplier
 
     //// Iterate for each element of first row
    //for (int f = 0; f < n; f++)
    //{
        //// Getting Cofactor of mat[0][f]
        //getCofactor(mat, temp, 0, f, n);
        //D += sign * mat[0][f] * DeterminantRecursive(temp, n - 1);
 
        //// terms are to be added with alternate sign
        //sign = -sign;
    //}
 
    //return D;
//}


//float Matrix6F6::Determinant(void)
//{
    //return DeterminantRecursive(*this, SIZE);
//}


//Matrix6F6 Matrix6F6::Inversed(void)
//{
    //Matrix6F6 matOut;
    //matOut.Zero();
    //Matrix6F6 temp;
    //temp.Zero();
    //int i,j = 0;
    //float determinant = 0;

    //determinant = Determinant();

    //if (0 == determinant || !isfinite(determinant))
    //{
        //return Matrix6F6(nan[0], nan[1], nan[2], nan[3], nan[4], nan[5]);
    //}

    //for(j = 0; j < SIZE; j++)
    //{
        //for(i = 0; i < SIZE; i++)
        //{
            //getCofactor(*this, temp, j, i, SIZE);
            //matOut[i][j] = DeterminantRecursive(temp, SIZE - 1) / determinant;
            //if (1 == (i+j)%2)
            //{
                //matOut[i][j] = -matOut[i][j];
            //}

        //}
    //}
    
    //return matOut;
//}


/**
 * inverse based on LU factorization with partial pivotting
 */
Matrix6F6 Matrix6F6::Inversed(void)
{
    Matrix6F6 L(
            {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
            {0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f},
            {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f}
    );
    Matrix6F6 P(
            {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
            {0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f},
            {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f}
    );
    Matrix6F6 U;

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
        Matrix6F6 Zero;
        Zero.Zero();
        return Zero;
    }
}


void Matrix6F6::SwapRows(uint8 a, uint8 b)
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


void Matrix6F6::SwapCols(uint8 a, uint8 b)
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





