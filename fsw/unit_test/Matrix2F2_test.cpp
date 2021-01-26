/****************************************************************************
*
*   Copyright (c) 2017 Windhover Labs, L.L.C. All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
*
* 1. Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in
*    the documentation and/or other materials provided with the
*    distribution.
* 3. Neither the name Windhover Labs nor the names of its 
*    contributors may be used to endorse or promote products derived 
*    from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
* COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
* OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
* AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
* ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*
*****************************************************************************/

#include "Matrix2F2_test.hpp"
#include <math/Matrix2F2.hpp>

#include "utassert.h"
#include <float.h>


void Test_Matrix2F2_Constructor(void)
{
	math::Matrix2F2 matrix(
			{1.0f, 2.0f},
			{3.0f, 4.0f}
	);

	/* Verify results */
	UtAssert_True(matrix[0][0] == 1.0f, "matrix[0][0] == 1.0f");
	UtAssert_True(matrix[0][1] == 2.0f, "matrix[0][1] == 2.0f");
	UtAssert_True(matrix[1][0] == 3.0f, "matrix[1][0] == 4.0f");
	UtAssert_True(matrix[1][1] == 4.0f, "matrix[1][1] == 5.0f");
}


void Test_Matrix2F2_IndexOutOfBounds(void)
{
	math::Matrix2F2 matrix(
			{1.0f, 2.0f},
			{3.0f, 4.0f}
	);

	/* Verify results */
	UtAssert_True(isnan(matrix[2][3]), "matrix[2][3] == NAN");
	UtAssert_True(isnan(matrix[3][0]), "matrix[3][0] == NAN");
}


void Test_Matrix2F2_ArrayOperator(void)
{
	math::Matrix2F2 matrix(
			{1.0f, 2.0f},
			{3.0f, 4.0f}
	);
	
	float nanResultA = matrix[7][0];
	float nanResultB = matrix[2][7];

    UtAssert_True(isnan(matrix[7][0]), "matrix[7][0] == NAN");
    UtAssert_True(isnan(matrix[2][7]), "matrix[2][7] == NAN");

    UtAssert_True(isnan(nanResultA), "NAN == matrix[7][0]");
    UtAssert_True(isnan(nanResultB), "NAN == matrix[2][7]");
}



void Test_Matrix2F2_Transpose(void)
{
	math::Matrix2F2 matrixA(
			{1.0f, 2.0f},
			{3.0f, 4.0f}
	);
	
	/* This is actual flight data collected from PX4 code. */
	math::Matrix2F2 matrixB(
			{-0.006345f, -0.999761f},
			{0.999639f, -0.005796f}
	);
	math::Matrix2F2 matrixC(
			{-0.006324f, -0.999762f},
			{0.999638f, -0.005774f}
	);
	math::Matrix2F2 matrixD(
			{-0.006324f, -0.999762f},
			{0.999638f, -0.005774f}
	);
	
	math::Matrix2F2 matrixAT = matrixA.Transpose();
	math::Matrix2F2 matrixBT = matrixB.Transpose();
	math::Matrix2F2 matrixCT = matrixC.Transpose();
	math::Matrix2F2 matrixDT = matrixD.Transpose();

    UtAssert_True(matrixA[0][0] == matrixAT[0][0], "[0][0]:  matrixA.Transpose()");
    UtAssert_True(matrixA[0][1] == matrixAT[1][0], "[0][1]:  matrixA.Transpose()");
    UtAssert_True(matrixA[1][0] == matrixAT[0][1], "[1][0]:  matrixA.Transpose()");
    UtAssert_True(matrixA[1][1] == matrixAT[1][1], "[1][1]:  matrixA.Transpose()");

    UtAssert_True(matrixB[0][0] == matrixBT[0][0], "[0][0]:  matrixB.Transpose()");
    UtAssert_True(matrixB[0][1] == matrixBT[1][0], "[0][1]:  matrixB.Transpose()");
    UtAssert_True(matrixB[1][0] == matrixBT[0][1], "[1][0]:  matrixB.Transpose()");
    UtAssert_True(matrixB[1][1] == matrixBT[1][1], "[1][1]:  matrixB.Transpose()");

    UtAssert_True(matrixC[0][0] == matrixCT[0][0], "[0][0]:  matrixC.Transpose()");
    UtAssert_True(matrixC[0][1] == matrixCT[1][0], "[0][1]:  matrixC.Transpose()");
    UtAssert_True(matrixC[1][0] == matrixCT[0][1], "[1][0]:  matrixC.Transpose()");
    UtAssert_True(matrixC[1][1] == matrixCT[1][1], "[1][1]:  matrixC.Transpose()");

    UtAssert_True(matrixD[0][0] == matrixDT[0][0], "[0][0]:  matrixD.Transpose()");
    UtAssert_True(matrixD[0][1] == matrixDT[1][0], "[0][1]:  matrixD.Transpose()");
    UtAssert_True(matrixD[1][0] == matrixDT[0][1], "[1][0]:  matrixD.Transpose()");
    UtAssert_True(matrixD[1][1] == matrixDT[1][1], "[1][1]:  matrixD.Transpose()");
}



void Test_Matrix2F2_Identity(void)
{
	math::Matrix2F2 identity = math::Matrix2F2::Identity();

    UtAssert_True(identity[0][0] == 1.0f, "[0][0]:  matrix.Identity()");
    UtAssert_True(identity[0][1] == 0.0f, "[0][1]:  matrix.Identity()");

    UtAssert_True(identity[1][0] == 0.0f, "[1][0]:  matrix.Identity()");
    UtAssert_True(identity[1][1] == 1.0f, "[1][1]:  matrix.Identity()");
}



void Test_Matrix2F2_ScalarMultiply(void)
{
	math::Matrix2F2 matrix(
			{1.0f, 2.0f},
			{4.0f, 5.0f}
	);
    float scalar = 10.0f;
	math::Matrix2F2 result = matrix * scalar;

	UtAssert_True(result[0][0] == 10.0f, "[0][0]:  Matrix2F2 * Matrix2F2");
	UtAssert_True(result[0][1] == 20.0f, "[0][1]:  Matrix2F2 * Matrix2F2");
	UtAssert_True(result[1][0] == 40.0f, "[1][0]:  Matrix2F2 * Matrix2F2");
	UtAssert_True(result[1][1] == 50.0f, "[1][1]:  Matrix2F2 * Matrix2F2");
}



void Test_Matrix2F2_MatrixAddition(void)
{
	math::Matrix2F2 matrixA(
		{1.0f, 2.0f},
		{3.0f, 4.0f}
	);
	math::Matrix2F2 matrixB(
		{5.0f, 6.0f},
		{7.0f, 8.0f}
	);
	math::Matrix2F2 result = matrixA + matrixB;

	UtAssert_True(result[0][0] == 6.0f, "[0][0]:  Matrix2F2 + Matrix2F2");
	UtAssert_True(result[0][1] == 8.0f, "[0][1]:  Matrix2F2 + Matrix2F2");
	UtAssert_True(result[1][0] == 10.0f, "[1][0]:  Matrix2F2 + Matrix2F2");
	UtAssert_True(result[1][1] == 12.0f, "[1][1]:  Matrix2F2 + Matrix2F2");
}



void Test_Matrix2F2_Zero(void)
{
    math::Matrix2F2 matrixA(
        {1.0f, 3.0f},
        {7.0f, 9.0f}
    );
    
    matrixA.Zero();
    
	UtAssert_True(matrixA[0][0] == 0.0f, "matrixA[0][0] == 0.0f");
	UtAssert_True(matrixA[0][1] == 0.0f, "matrixA[0][0] == 0.0f");
	UtAssert_True(matrixA[1][0] == 0.0f, "matrixA[0][0] == 0.0f");
	UtAssert_True(matrixA[1][1] == 0.0f, "matrixA[0][0] == 0.0f");
}

void Test_Matrix2F2_Inverse(void)
{
	math::Matrix2F2 matrixA(
		{2.0f, 1.0f},
		{1.0f, 2.0f}
	);

    math::Matrix2F2 matrix = matrixA.Inversed();

	/* Verify results */
	UtAssert_True(fabs(matrix[0][0] - (2.0f / 3.0f)) < 0.00001f, "matrix[0][0] == 2 / 3");
	UtAssert_True(fabs(matrix[0][1] - (- 1.0f / 3.0f)) < 0.00001f, "matrix[0][1] == - 1 / 3");
    
	UtAssert_True(fabs(matrix[1][0] - (- 1.0f / 3.0f)) < 0.00001f, "matrix[1][0] == - 1 / 3");
	UtAssert_True(fabs(matrix[1][1] - (2.0f / 3.0f)) < 0.00001f, "matrix[1][1] == 2 / 3");

}

void Test_Matrix2F2_Determinant(void)
{
	//math::Matrix2F2 matrix(
		//{2.0f, 1.0f, 1.0f},
		//{1.0f, 2.0f, 1.0f},
		//{1.0f, 1.0f, 2.0f}
	//);

	//float result = 0;

    //result = matrix.Determinant();
    
    ////printf("result %f\n", result);

    //UtAssert_True(result == 4, "result == 4");
}

/* Auto generated with GNU Octave, version 4.0.0 */
void Test_Matrix2F2_Mult_2F_Random(void)
{
    int i, j = 0;

    math::Matrix2F2 A (
        {-0.648010, -0.151895},
        {-0.133369, 0.865734}
    );

    math::Vector2F B (
        {0.923901,-0.699953}
    );

    math::Vector2F expected (
        {-0.492377,-0.729192}
    );

    math::Vector2F result;
    result.Zero();

    result = A * B;

    for(i = 0; i < 2; i++)
    {
        UtAssert_True(fabs(result[i] - expected[i]) < 0.00001f, "fabs(result[i] - expected[i]) < 0.00001f");

    }
}

/* Auto generated with GNU Octave, version 4.0.0 */
void Test_Matrix2F2_Mult_2F2_Random(void)
{
    int i, j = 0;

    math::Matrix2F2 A (
        {0.909679, 0.650480},
        {0.017760, -0.157760}
    );

    math::Matrix2F2 B (
        {-0.553164, -0.847697},
        {0.609642, -0.856567}
    );

    math::Matrix2F2 expected (
        {-0.106642, -1.328312},
        {-0.106001, 0.120077}
    );

    math::Matrix2F2 result;
    result.Zero();

    result = A * B;

    for(i = 0; i < 2; i++)
    {
        for(j = 0; j < 2; j++)
        {
            UtAssert_True(fabs(result[i][j] - expected[i][j]) < 0.00001f, "fabs(result[i][j] - expected[i][j]) < 0.00001f");

        }
    }
}
