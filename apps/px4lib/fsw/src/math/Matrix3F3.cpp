#include <math/Matrix3F3.hpp>
#include <math.h>

using namespace math;


const Matrix3F3::RotLookup_t Matrix3F3::RotLookup[] = {
	{  0,   0,   0 },
	{  0,   0,  45 },
	{  0,   0,  90 },
	{  0,   0, 135 },
	{  0,   0, 180 },
	{  0,   0, 225 },
	{  0,   0, 270 },
	{  0,   0, 315 },
	{180,   0,   0 },
	{180,   0,  45 },
	{180,   0,  90 },
	{180,   0, 135 },
	{  0, 180,   0 },
	{180,   0, 225 },
	{180,   0, 270 },
	{180,   0, 315 },
	{ 90,   0,   0 },
	{ 90,   0,  45 },
	{ 90,   0,  90 },
	{ 90,   0, 135 },
	{270,   0,   0 },
	{270,   0,  45 },
	{270,   0,  90 },
	{270,   0, 135 },
	{  0,  90,   0 },
	{  0, 270,   0 },
	{270,   0, 270 },
	{180, 270,   0 },
	{  0,  90, 180 },
	{ 90,  90,   0 },
	{ 90,  68, 293 },
	{270,  90,   0 },
	{  0,   9, 180 },
};

Matrix3F3::Matrix3F3(Vector3F m0, Vector3F m1, Vector3F m2) :
	nan{NAN,NAN,NAN},
	data{m0, m1, m2}
{
};


Matrix3F3::Matrix3F3() :
    nan{NAN,NAN,NAN},
	data{
		{0.0, 0.0, 0.0},
		{0.0, 0.0, 0.0},
		{0.0, 0.0, 0.0}
    }
{
};


Matrix3F3::~Matrix3F3()
{
};


Vector3F& Matrix3F3::operator [] (uint32 i)
{
	if(i >= 3)
	{
		return nan;
	}
	else
	{
		return data[i];
	}
};


Vector3F Matrix3F3::operator [] (uint32 i) const
{
	if(i >= 3)
	{
		return nan;
	}
	else
	{
		return data[i];
	}
};


Matrix3F3 Matrix3F3::Transpose(void)
{
	Matrix3F3 res;

	res[0][0] = data[0][0];
	res[0][1] = data[1][0];
	res[0][2] = data[2][0];
	res[1][0] = data[0][1];
	res[1][1] = data[1][1];
	res[1][2] = data[2][1];
	res[2][0] = data[0][2];
	res[2][1] = data[1][2];
	res[2][2] = data[2][2];

	return res;
}


Matrix3F3 Matrix3F3::Identity() {
    Matrix3F3 matOut(
    		{1.0f, 0.0f, 0.0f},
    		{0.0f, 1.0f, 0.0f},
    		{0.0f, 0.0f, 1.0f}
			);

    return matOut;
}


void Matrix3F3::Zero(void)
{
	data[0][0] = 0.0f;
	data[0][1] = 0.0f;
	data[0][2] = 0.0f;
	data[1][0] = 0.0f;
	data[1][1] = 0.0f;
	data[1][2] = 0.0f;
	data[2][0] = 0.0f;
	data[2][1] = 0.0f;
	data[2][2] = 0.0f;
}


// overload * operator to provide a matrix product
Matrix3F3 Matrix3F3::operator*(const Matrix3F3 &matIn)
{
    Matrix3F3 matOut;

    matOut[0][0] = data[0][0]*matIn[0][0] + data[0][1]*matIn[1][0] + data[0][2]*matIn[2][0];
    matOut[0][1] = data[0][0]*matIn[0][1] + data[0][1]*matIn[1][1] + data[0][2]*matIn[2][1];
    matOut[0][2] = data[0][0]*matIn[0][2] + data[0][1]*matIn[1][2] + data[0][2]*matIn[2][2];

    matOut[1][0] = data[1][0]*matIn[0][0] + data[1][1]*matIn[1][0] + data[1][2]*matIn[2][0];
    matOut[1][1] = data[1][0]*matIn[0][1] + data[1][1]*matIn[1][1] + data[1][2]*matIn[2][1];
    matOut[1][2] = data[1][0]*matIn[0][2] + data[1][1]*matIn[1][2] + data[1][2]*matIn[2][2];

    matOut[2][0] = data[2][0]*matIn[0][0] + data[2][1]*matIn[1][0] + data[2][2]*matIn[2][0];
    matOut[2][1] = data[2][0]*matIn[0][1] + data[2][1]*matIn[1][1] + data[2][2]*matIn[2][1];
    matOut[2][2] = data[2][0]*matIn[0][2] + data[2][1]*matIn[1][2] + data[2][2]*matIn[2][2];

    return matOut;
}


// overload * operator to provide a matrix vector product
Vector3F Matrix3F3::operator*(const Vector3F &vecIn)
{
    Vector3F vecOut;
    vecOut[0] = data[0][0]*vecIn[0] + data[0][1]*vecIn[1] + data[0][2]*vecIn[2];
    vecOut[1] = data[1][0]*vecIn[0] + data[1][1]*vecIn[1] + data[1][2]*vecIn[2];
    vecOut[2] = data[2][0]*vecIn[0] + data[2][1]*vecIn[1] + data[2][2]*vecIn[2];
    return vecOut;
}


Matrix3F3 Matrix3F3::operator*(const float &scalar)
{
    Matrix3F3 matOut;

    matOut[0][0] = scalar*data[0][0];
    matOut[1][0] = scalar*data[1][0];
    matOut[2][0] = scalar*data[2][0];

    matOut[0][1] = scalar*data[0][1];
    matOut[1][1] = scalar*data[1][1];
    matOut[2][1] = scalar*data[2][1];

    matOut[0][2] = scalar*data[0][2];
    matOut[1][2] = scalar*data[1][2];
    matOut[2][2] = scalar*data[2][2];

    return matOut;
}


Matrix3F3 Matrix3F3::operator+(const Matrix3F3 &matIn) const
{
    Matrix3F3 matOut;

    matOut[0][0] = data[0][0] + matIn[0][0];
    matOut[1][0] = data[1][0] + matIn[1][0];
    matOut[2][0] = data[2][0] + matIn[2][0];

    matOut[0][1] = data[0][1] + matIn[0][1];
    matOut[1][1] = data[1][1] + matIn[1][1];
    matOut[2][1] = data[2][1] + matIn[2][1];

    matOut[0][2] = data[0][2] + matIn[0][2];
    matOut[1][2] = data[1][2] + matIn[1][2];
    matOut[2][2] = data[2][2] + matIn[2][2];

    return matOut;
}


Matrix3F3 Matrix3F3::RotationMatrix(Matrix3F3::Rotation_t boardRotation)
{
	Matrix3F3 matrix;

	float roll  = M_DEG_TO_RAD_F * (float)Matrix3F3::RotLookup[boardRotation].roll;
	float pitch = M_DEG_TO_RAD_F * (float)Matrix3F3::RotLookup[boardRotation].pitch;
	float yaw   = M_DEG_TO_RAD_F * (float)Matrix3F3::RotLookup[boardRotation].yaw;

	return Matrix3F3::FromEuler(roll, pitch, yaw);
}



Matrix3F3 Matrix3F3::FromEuler(float roll, float pitch, float yaw)
{
	Matrix3F3 matrix;

	float cp = cosf(pitch);
	float sp = sinf(pitch);
	float cr = cosf(roll);
	float sr = sinf(roll);
	float cy = cosf(yaw);
	float sy = sinf(yaw);

	matrix[0][0] = cp * cy;
	matrix[0][1] = (sr * sp * cy) - (cr * sy);
	matrix[0][2] = (cr * sp * cy) + (sr * sy);
	matrix[1][0] = cp * sy;
	matrix[1][1] = (sr * sp * sy) + (cr * cy);
	matrix[1][2] = (cr * sp * sy) - (sr * cy);
	matrix[2][0] = -sp;
	matrix[2][1] = sr * cp;
	matrix[2][2] = cr * cp;

	return matrix;
}



Vector3F Matrix3F3::ToEuler(void) const
{
	Vector3F euler;
	euler[1] = asinf(-data[2][0]);

	if (fabsf(euler[1] - M_PI_2) < 1.0e-3f) {
		euler[0] = 0.0f;
		euler[2] = atan2f(data[1][2] - data[0][1], data[0][2] + data[1][1]) + euler[0];

	} else if (fabsf(euler[1] + M_PI_2) < 1.0e-3f) {
		euler[0] = 0.0f;
		euler[2] = atan2f(data[1][2] - data[0][1], data[0][2] + data[1][1]) - euler[0];

	} else {
		euler[0] = atan2f(data[2][1], data[2][2]);
		euler[2] = atan2f(data[1][0], data[0][0]);
	}

	return euler;
}

/**
 * inverse based on LU factorization with partial pivotting
 */
Matrix3F3 Matrix3F3::Inversed(void)
{
    Matrix3F3 L(
            {1.0f, 0.0f, 0.0f},
            {0.0f, 1.0f, 0.0f},
            {0.0f, 0.0f, 1.0f}
    );
    Matrix3F3 P(
            {1.0f, 0.0f, 0.0f},
            {0.0f, 1.0f, 0.0f},
            {0.0f, 0.0f, 1.0f}
    );
    Matrix3F3 U;

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
        Matrix3F3 Zero;
        Zero.Zero();
        return Zero;
    }
}


void Matrix3F3::SwapRows(uint8 a, uint8 b)
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


void Matrix3F3::SwapCols(uint8 a, uint8 b)
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



