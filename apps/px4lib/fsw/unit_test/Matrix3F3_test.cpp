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

#include "Matrix3F3_test.hpp"
#include <math/Matrix3F3.hpp>

#include "utassert.h"
#include <float.h>


void Test_Matrix3F3_Constructor(void)
{
	math::Matrix3F3 matrix(
			{1.0f, 2.0f, 3.0f},
			{4.0f, 5.0f, 6.0f},
			{7.0f, 8.0f, 9.0f}
	);

	/* Verify results */
	UtAssert_True(matrix[0][0] == 1.0f, "matrix[0][0] == 1.0f");
	UtAssert_True(matrix[0][1] == 2.0f, "matrix[0][1] == 2.0f");
	UtAssert_True(matrix[0][2] == 3.0f, "matrix[0][2] == 3.0f");
	UtAssert_True(matrix[1][0] == 4.0f, "matrix[1][0] == 4.0f");
	UtAssert_True(matrix[1][1] == 5.0f, "matrix[1][1] == 5.0f");
	UtAssert_True(matrix[1][2] == 6.0f, "matrix[1][2] == 6.0f");
	UtAssert_True(matrix[2][0] == 7.0f, "matrix[2][0] == 7.0f");
	UtAssert_True(matrix[2][1] == 8.0f, "matrix[2][1] == 8.0f");
	UtAssert_True(matrix[2][2] == 9.0f, "matrix[2][2] == 9.0f");
}


void Test_Matrix3F3_IndexOutOfBounds(void)
{
	math::Matrix3F3 matrix(
			{1.0f, 2.0f, 3.0f},
			{4.0f, 5.0f, 6.0f},
			{7.0f, 8.0f, 9.0f}
	);

	/* Verify results */
	UtAssert_True(isnan(matrix[2][3]), "matrix[2][3] == NAN");
	UtAssert_True(isnan(matrix[3][0]), "matrix[3][0] == NAN");
}


void Test_Matrix3F3_ArrayOperator(void)
{
	math::Matrix3F3 matrix(
			{1.0f, 2.0f, 3.0f},
			{4.0f, 5.0f, 6.0f},
			{7.0f, 8.0f, 9.0f}
	);
	float nanResultA = matrix[7][0];
	float nanResultB = matrix[2][7];

    UtAssert_True(isnan(matrix[7][0]), "matrix[7][0] == NAN");
    UtAssert_True(isnan(matrix[2][7]), "matrix[2][7] == NAN");

    UtAssert_True(isnan(nanResultA), "NAN == matrix[7][0]");
    UtAssert_True(isnan(nanResultB), "NAN == matrix[2][7]");
}



void Test_Matrix3F3_Transpose(void)
{
	math::Matrix3F3 matrixA(
			{1.0f, 2.0f, 3.0f},
			{4.0f, 5.0f, 6.0f},
			{7.0f, 8.0f, 9.0f}
	);
	/* This is actual flight data collected from PX4 code. */
	math::Matrix3F3 matrixB(
			{-0.006345f, -0.999761f, -0.020913f},
			{0.999639f, -0.005796f, -0.026238f},
			{0.026111f, -0.021072f, 0.999437f}
	);
	math::Matrix3F3 matrixC(
			{-0.006324f, -0.999762f, -0.020890f},
			{0.999638f, -0.005774f, -0.026283f},
			{0.026156f, -0.021049f, 0.999436f}
	);
	math::Matrix3F3 matrixD(
			{-0.006324f, -0.999762f, -0.020890f},
			{0.999638f, -0.005774f, -0.026283f},
			{0.026156f, -0.021049f, 0.999436f}
	);
	math::Matrix3F3 matrixAT = matrixA.Transpose();
	math::Matrix3F3 matrixBT = matrixB.Transpose();
	math::Matrix3F3 matrixCT = matrixC.Transpose();
	math::Matrix3F3 matrixDT = matrixD.Transpose();

    UtAssert_True(matrixA[0][0] == matrixAT[0][0], "[0][0]:  matrixA.Transpose()");
    UtAssert_True(matrixA[0][1] == matrixAT[1][0], "[0][1]:  matrixA.Transpose()");
    UtAssert_True(matrixA[0][2] == matrixAT[2][0], "[0][2]:  matrixA.Transpose()");
    UtAssert_True(matrixA[1][0] == matrixAT[0][1], "[1][0]:  matrixA.Transpose()");
    UtAssert_True(matrixA[1][1] == matrixAT[1][1], "[1][1]:  matrixA.Transpose()");
    UtAssert_True(matrixA[1][2] == matrixAT[2][1], "[1][2]:  matrixA.Transpose()");
    UtAssert_True(matrixA[2][0] == matrixAT[0][2], "[2][0]:  matrixA.Transpose()");
    UtAssert_True(matrixA[2][1] == matrixAT[1][2], "[2][1]:  matrixA.Transpose()");
    UtAssert_True(matrixA[2][2] == matrixAT[2][2], "[2][2]:  matrixA.Transpose()");

    UtAssert_True(matrixB[0][0] == matrixBT[0][0], "[0][0]:  matrixB.Transpose()");
    UtAssert_True(matrixB[0][1] == matrixBT[1][0], "[0][1]:  matrixB.Transpose()");
    UtAssert_True(matrixB[0][2] == matrixBT[2][0], "[0][2]:  matrixB.Transpose()");
    UtAssert_True(matrixB[1][0] == matrixBT[0][1], "[1][0]:  matrixB.Transpose()");
    UtAssert_True(matrixB[1][1] == matrixBT[1][1], "[1][1]:  matrixB.Transpose()");
    UtAssert_True(matrixB[1][2] == matrixBT[2][1], "[1][2]:  matrixB.Transpose()");
    UtAssert_True(matrixB[2][0] == matrixBT[0][2], "[2][0]:  matrixB.Transpose()");
    UtAssert_True(matrixB[2][1] == matrixBT[1][2], "[2][1]:  matrixB.Transpose()");
    UtAssert_True(matrixB[2][2] == matrixBT[2][2], "[2][2]:  matrixB.Transpose()");

    UtAssert_True(matrixC[0][0] == matrixCT[0][0], "[0][0]:  matrixC.Transpose()");
    UtAssert_True(matrixC[0][1] == matrixCT[1][0], "[0][1]:  matrixC.Transpose()");
    UtAssert_True(matrixC[0][2] == matrixCT[2][0], "[0][2]:  matrixC.Transpose()");
    UtAssert_True(matrixC[1][0] == matrixCT[0][1], "[1][0]:  matrixC.Transpose()");
    UtAssert_True(matrixC[1][1] == matrixCT[1][1], "[1][1]:  matrixC.Transpose()");
    UtAssert_True(matrixC[1][2] == matrixCT[2][1], "[1][2]:  matrixC.Transpose()");
    UtAssert_True(matrixC[2][0] == matrixCT[0][2], "[2][0]:  matrixC.Transpose()");
    UtAssert_True(matrixC[2][1] == matrixCT[1][2], "[2][1]:  matrixC.Transpose()");
    UtAssert_True(matrixC[2][2] == matrixCT[2][2], "[2][2]:  matrixC.Transpose()");

    UtAssert_True(matrixD[0][0] == matrixDT[0][0], "[0][0]:  matrixD.Transpose()");
    UtAssert_True(matrixD[0][1] == matrixDT[1][0], "[0][1]:  matrixD.Transpose()");
    UtAssert_True(matrixD[0][2] == matrixDT[2][0], "[0][2]:  matrixD.Transpose()");
    UtAssert_True(matrixD[1][0] == matrixDT[0][1], "[1][0]:  matrixD.Transpose()");
    UtAssert_True(matrixD[1][1] == matrixDT[1][1], "[1][1]:  matrixD.Transpose()");
    UtAssert_True(matrixD[1][2] == matrixDT[2][1], "[1][2]:  matrixD.Transpose()");
    UtAssert_True(matrixD[2][0] == matrixDT[0][2], "[2][0]:  matrixD.Transpose()");
    UtAssert_True(matrixD[2][1] == matrixDT[1][2], "[2][1]:  matrixD.Transpose()");
    UtAssert_True(matrixD[2][2] == matrixDT[2][2], "[2][2]:  matrixD.Transpose()");
}



void Test_Matrix3F3_Identity(void)
{
	math::Matrix3F3 identity = math::Matrix3F3::Identity();

    UtAssert_True(identity[0][0] == 1.0f, "[0][0]:  matrix.Identity()");
    UtAssert_True(identity[0][1] == 0.0f, "[0][1]:  matrix.Identity()");
    UtAssert_True(identity[0][2] == 0.0f, "[0][2]:  matrix.Identity()");

    UtAssert_True(identity[1][0] == 0.0f, "[1][0]:  matrix.Identity()");
    UtAssert_True(identity[1][1] == 1.0f, "[1][1]:  matrix.Identity()");
    UtAssert_True(identity[1][2] == 0.0f, "[1][2]:  matrix.Identity()");

    UtAssert_True(identity[2][0] == 0.0f, "[2][0]:  matrix.Identity()");
    UtAssert_True(identity[2][1] == 0.0f, "[2][1]:  matrix.Identity()");
    UtAssert_True(identity[2][2] == 1.0f, "[2][2]:  matrix.Identity()");
}



void Test_Matrix3F3_VectorMultiply(void)
{
	math::Matrix3F3 matrix(
			{1.0f, 2.0f, 3.0f},
			{4.0f, 5.0f, 6.0f},
			{7.0f, 8.0f, 9.0f}
	);
	math::Vector3F vector({10.0f, 11.0f, 12.0f});
	math::Vector3F result = matrix * vector;

	UtAssert_True(result[0] == 68.0,  "[0]:  Matrix3F3 * Vector3F");
	UtAssert_True(result[1] == 167.0, "[1]:  Matrix3F3 * Vector3F");
	UtAssert_True(result[2] == 266.0, "[2]:  Matrix3F3 * Vector3F");
}



void Test_Matrix3F3_MatrixMultiply(void)
{
	math::Matrix3F3 matrixA(
			{1.0f, 2.0f, 3.0f},
			{4.0f, 5.0f, 6.0f},
			{7.0f, 8.0f, 9.0f}
	);
	math::Matrix3F3 matrixB(
			{10.0f, 11.0f, 12.0f},
			{13.0f, 14.0f, 15.0f},
			{16.0f, 17.0f, 18.0f}
	);
	math::Matrix3F3 result = matrixA * matrixB;

	UtAssert_True(result[0][0] == 84.0f,  "[0][0]:  Matrix3F3 * Matrix3F3");
	UtAssert_True(result[0][1] == 90.0f,  "[0][1]:  Matrix3F3 * Matrix3F3");
	UtAssert_True(result[0][2] == 96.0f,  "[0][2]:  Matrix3F3 * Matrix3F3");
	UtAssert_True(result[1][0] == 201.0f, "[1][0]:  Matrix3F3 * Matrix3F3");
	UtAssert_True(result[1][1] == 216.0f, "[1][1]:  Matrix3F3 * Matrix3F3");
	UtAssert_True(result[1][2] == 231.0f, "[1][2]:  Matrix3F3 * Matrix3F3");
	UtAssert_True(result[2][0] == 318.0f, "[2][0]:  Matrix3F3 * Matrix3F3");
	UtAssert_True(result[2][1] == 342.0f, "[2][1]:  Matrix3F3 * Matrix3F3");
	UtAssert_True(result[2][2] == 366.0f, "[2][2]:  Matrix3F3 * Matrix3F3");
}



void Test_Matrix3F3_ScalarMultiply(void)
{
	math::Matrix3F3 matrix(
			{1.0f, 2.0f, 3.0f},
			{4.0f, 5.0f, 6.0f},
			{7.0f, 8.0f, 9.0f}
	);
    float scalar = 10.0f;
	math::Matrix3F3 result = matrix * scalar;

	UtAssert_True(result[0][0] == 10.0f, "[0][0]:  Matrix3F3 * Matrix3F3");
	UtAssert_True(result[0][1] == 20.0f, "[0][1]:  Matrix3F3 * Matrix3F3");
	UtAssert_True(result[0][2] == 30.0f, "[0][2]:  Matrix3F3 * Matrix3F3");
	UtAssert_True(result[1][0] == 40.0f, "[1][0]:  Matrix3F3 * Matrix3F3");
	UtAssert_True(result[1][1] == 50.0f, "[1][1]:  Matrix3F3 * Matrix3F3");
	UtAssert_True(result[1][2] == 60.0f, "[1][2]:  Matrix3F3 * Matrix3F3");
	UtAssert_True(result[2][0] == 70.0f, "[2][0]:  Matrix3F3 * Matrix3F3");
	UtAssert_True(result[2][1] == 80.0f, "[2][1]:  Matrix3F3 * Matrix3F3");
	UtAssert_True(result[2][2] == 90.0f, "[2][2]:  Matrix3F3 * Matrix3F3");
}



void Test_Matrix3F3_MatrixAddition(void)
{
	math::Matrix3F3 matrixA(
		{1.0f, 2.0f, 3.0f},
		{4.0f, 5.0f, 6.0f},
		{7.0f, 8.0f, 9.0f}
	);
	math::Matrix3F3 matrixB(
		{10.0f, 11.0f, 12.0f},
		{13.0f, 14.0f, 15.0f},
		{16.0f, 17.0f, 18.0f}
	);
	math::Matrix3F3 result = matrixA + matrixB;

	UtAssert_True(result[0][0] == 11.0f, "[0][0]:  Matrix3F3 + Matrix3F3");
	UtAssert_True(result[0][1] == 13.0f, "[0][1]:  Matrix3F3 + Matrix3F3");
	UtAssert_True(result[0][2] == 15.0f, "[0][2]:  Matrix3F3 + Matrix3F3");
	UtAssert_True(result[1][0] == 17.0f, "[1][0]:  Matrix3F3 + Matrix3F3");
	UtAssert_True(result[1][1] == 19.0f, "[1][1]:  Matrix3F3 + Matrix3F3");
	UtAssert_True(result[1][2] == 21.0f, "[1][2]:  Matrix3F3 + Matrix3F3");
	UtAssert_True(result[2][0] == 23.0f, "[2][0]:  Matrix3F3 + Matrix3F3");
	UtAssert_True(result[2][1] == 25.0f, "[2][1]:  Matrix3F3 + Matrix3F3");
	UtAssert_True(result[2][2] == 27.0f, "[2][2]:  Matrix3F3 + Matrix3F3");
}


void Test_Matrix3F3_FromEuler(void)
{
	math::Matrix3F3 matrixA = math::Matrix3F3::FromEuler(M_PI / 4.0, 0.0f, 0.0f);
	math::Matrix3F3 matrixB = math::Matrix3F3::FromEuler(0.0f, -M_PI / 4.0, 0.0f);
	math::Matrix3F3 matrixC = math::Matrix3F3::FromEuler(0.0f, 0.0f, -M_PI / 4.0);
	math::Matrix3F3 matrixD = math::Matrix3F3::FromEuler(M_PI / 4.0, 0.0f, -M_PI / 4.0);

	UtAssert_True(fabs(matrixA[0][0] - 1.0f) < 0.00001, "[0][0]:  Matrix::FromEuler(M_PI / 4.0, 0.0, 0.0)");
	UtAssert_True(fabs(matrixA[0][1] - 0.0f) < 0.00001, "[0][1]:  Matrix::FromEuler(M_PI / 4.0, 0.0, 0.0)");
	UtAssert_True(fabs(matrixA[0][2] - 0.0f) < 0.00001, "[0][2]:  Matrix::FromEuler(M_PI / 4.0, 0.0, 0.0)");

	UtAssert_True(fabs(matrixA[1][0] - 0.0f) < 0.00001, "[1][0]:  Matrix::FromEuler(M_PI / 4.0, 0.0, 0.0)");
	UtAssert_True(fabs(matrixA[1][1] - 0.7071068f) < 0.00001, "[1][1]:  Matrix::FromEuler(M_PI / 4.0, 0.0, 0.0)");
	UtAssert_True(fabs(matrixA[1][2] - -0.7071068f) < 0.00001, "[1][2]:  Matrix::FromEuler(M_PI / 4.0, 0.0, 0.0)");

	UtAssert_True(fabs(matrixA[2][0] - 0.0f) < 0.00001, "[2][0]:  Matrix::FromEuler(M_PI / 4.0, 0.0, 0.0)");
	UtAssert_True(fabs(matrixA[2][1] - 0.7071068f) < 0.00001, "[2][1]:  Matrix::FromEuler(M_PI / 4.0, 0.0, 0.0)");
	UtAssert_True(fabs(matrixA[2][2] - 0.7071068f) < 0.00001, "[2][2]:  Matrix::FromEuler(M_PI / 4.0, 0.0, 0.0)");

	UtAssert_True(fabs(matrixB[0][0] - 0.7071068f) < 0.00001, "[0][0]:  Matrix::FromEuler(0.0f, -M_PI / 4.0, 0.0f)");
	UtAssert_True(fabs(matrixB[0][1] - 0.0f) < 0.00001, "[0][1]:  Matrix::FromEuler(0.0f, -M_PI / 4.0, 0.0f)");
	UtAssert_True(fabs(matrixB[0][2] - -0.7071068f) < 0.00001, "[0][2]:  Matrix::FromEuler(0.0f, -M_PI / 4.0, 0.0f)");

	UtAssert_True(fabs(matrixB[1][0] - 0.0f) < 0.00001, "[1][0]:  Matrix::FromEuler(0.0f, -M_PI / 4.0, 0.0f)");
	UtAssert_True(fabs(matrixB[1][1] - 1.0) < 0.00001, "[1][1]:  Matrix::FromEuler(0.0f, -M_PI / 4.0, 0.0f)");
	UtAssert_True(fabs(matrixB[1][2] - 0.0f) < 0.00001, "[1][2]:  Matrix::FromEuler(0.0f, -M_PI / 4.0, 0.0f)");

	UtAssert_True(fabs(matrixB[2][0] - 0.7071068f) < 0.00001, "[2][0]:  Matrix::FromEuler(0.0f, -M_PI / 4.0, 0.0f)");
	UtAssert_True(fabs(matrixB[2][1] - 0.0f) < 0.00001, "[2][1]:  Matrix::FromEuler(0.0f, -M_PI / 4.0, 0.0f)");
	UtAssert_True(fabs(matrixB[2][2] - 0.7071068f) < 0.00001, "[2][2]:  Matrix::FromEuler(0.0f, -M_PI / 4.0, 0.0f)");

	UtAssert_True(fabs(matrixC[0][0] - 0.7071068f) < 0.00001, "[0][0]:  Matrix::FromEuler(0.0f, 0.0f, -M_PI / 4.0)");
	UtAssert_True(fabs(matrixC[0][1] - 0.7071068f) < 0.00001, "[0][1]:  Matrix::FromEuler(0.0f, 0.0f, -M_PI / 4.0)");
	UtAssert_True(fabs(matrixC[0][2] - 0.0f) < 0.00001, "[0][2]:  Matrix::FromEuler(0.0f, 0.0f, -M_PI / 4.0)");

	UtAssert_True(fabs(matrixC[1][0] - -0.7071068f) < 0.00001, "[1][0]:  Matrix::FromEuler(0.0f, 0.0f, -M_PI / 4.0)");
	UtAssert_True(fabs(matrixC[1][1] - 0.7071068f) < 0.00001, "[1][1]:  Matrix::FromEuler(0.0f, 0.0f, -M_PI / 4.0)");
	UtAssert_True(fabs(matrixC[1][2] - 0.0f) < 0.00001, "[1][2]:  Matrix::FromEuler(0.0f, 0.0f, -M_PI / 4.0)");

	UtAssert_True(fabs(matrixC[2][0] - 0.0f) < 0.00001, "[2][0]:  Matrix::FromEuler(0.0f, 0.0f, -M_PI / 4.0)");
	UtAssert_True(fabs(matrixC[2][1] - 0.0f) < 0.00001, "[2][1]:  Matrix::FromEuler(0.0f, 0.0f, -M_PI / 4.0)");
	UtAssert_True(fabs(matrixC[2][2] - 1.0f) < 0.00001, "[2][2]:  Matrix::FromEuler(0.0f, 0.0f, -M_PI / 4.0)");

	UtAssert_True(fabs(matrixD[0][0] - 0.7071068f) < 0.00001, "[0][0]:  Matrix::FromEuler(M_PI / 4.0, 0.0f, -M_PI / 4.0)");
	UtAssert_True(fabs(matrixD[0][1] - 0.5f) < 0.00001, "[0][1]:  Matrix::FromEuler(M_PI / 4.0, 0.0f, -M_PI / 4.0)");
	UtAssert_True(fabs(matrixD[0][2] - -0.5f) < 0.00001, "[0][2]:  Matrix::FromEuler(M_PI / 4.0, 0.0f, -M_PI / 4.0)");

	UtAssert_True(fabs(matrixD[1][0] - -0.7071068f) < 0.00001, "[1][0]:  Matrix::FromEuler(M_PI / 4.0, 0.0f, -M_PI / 4.0)");
	UtAssert_True(fabs(matrixD[1][1] - 0.5f) < 0.00001, "[1][1]:  Matrix::FromEuler(M_PI / 4.0, 0.0f, -M_PI / 4.0)");
	UtAssert_True(fabs(matrixD[1][2] - -0.5f) < 0.00001, "[1][2]:  Matrix::FromEuler(M_PI / 4.0, 0.0f, -M_PI / 4.0)");

	UtAssert_True(fabs(matrixD[2][0] - 0.0f) < 0.00001, "[2][0]:  Matrix::FromEuler(M_PI / 4.0, 0.0f, -M_PI / 4.0)");
	UtAssert_True(fabs(matrixD[2][1] - 0.7071068f) < 0.00001, "[2][1]:  Matrix::FromEuler(M_PI / 4.0, 0.0f, -M_PI / 4.0)");
	UtAssert_True(fabs(matrixD[2][2] - 0.7071068f) < 0.00001, "[2][2]:  Matrix::FromEuler(M_PI / 4.0, 0.0f, -M_PI / 4.0)");
}


void Test_Matrix3F3_RotationMatrix(void)
{
	math::Matrix3F3 matrix = math::Matrix3F3::RotationMatrix(math::Matrix3F3::ROTATION_ROLL_90_YAW_45);

	UtAssert_True(fabs(matrix[0][0] - 0.7071068f) < 0.00001,  "[0][0]:  math::Matrix3F3::RotationMatrix(ROTATION_ROLL_90_YAW_45)");
	UtAssert_True(fabs(matrix[0][1] - 0.0f) < 0.00001,        "[0][1]:  math::Matrix3F3::RotationMatrix(ROTATION_ROLL_90_YAW_45)");
	UtAssert_True(fabs(matrix[0][2] - 0.7071068f) < 0.00001,  "[0][2]:  math::Matrix3F3::RotationMatrix(ROTATION_ROLL_90_YAW_45)");

	UtAssert_True(fabs(matrix[1][0] - 0.7071068f) < 0.00001,  "[1][0]:  math::Matrix3F3::RotationMatrix(ROTATION_ROLL_90_YAW_45)");
	UtAssert_True(fabs(matrix[1][1] - 0.0f) < 0.00001,        "[1][1]:  math::Matrix3F3::RotationMatrix(ROTATION_ROLL_90_YAW_45)");
	UtAssert_True(fabs(matrix[1][2] - -0.7071068f) < 0.00001, "[1][2]:  math::Matrix3F3::RotationMatrix(ROTATION_ROLL_90_YAW_45)");

	UtAssert_True(fabs(matrix[2][0] - 0.0f) < 0.00001,        "[2][0]:  math::Matrix3F3::RotationMatrix(ROTATION_ROLL_90_YAW_45)");
	UtAssert_True(fabs(matrix[2][1] - 1.0f) < 0.00001,        "[2][1]:  math::Matrix3F3::RotationMatrix(ROTATION_ROLL_90_YAW_45)");
	UtAssert_True(fabs(matrix[2][2] - 0.0f) < 0.00001,        "[2][2]:  math::Matrix3F3::RotationMatrix(ROTATION_ROLL_90_YAW_45)");
}


void Test_Matrix3F3_Zero(void)
{
    math::Matrix3F3 matrixA(
        {1.0f, 3.0f, 5.0f},
        {7.0f, 9.0f, 11.0f},
        {13.0f, 15.0f, 17.0f}
    );
    
    matrixA.Zero();
    
	UtAssert_True(matrixA[0][0] == 0.0f, "matrixA[0][0] == 0.0f");
	UtAssert_True(matrixA[0][1] == 0.0f, "matrixA[0][0] == 0.0f");
	UtAssert_True(matrixA[0][2] == 0.0f, "matrixA[0][0] == 0.0f");
	UtAssert_True(matrixA[1][0] == 0.0f, "matrixA[0][0] == 0.0f");
	UtAssert_True(matrixA[1][1] == 0.0f, "matrixA[0][0] == 0.0f");
	UtAssert_True(matrixA[1][2] == 0.0f, "matrixA[0][0] == 0.0f");
	UtAssert_True(matrixA[2][0] == 0.0f, "matrixA[0][0] == 0.0f");
	UtAssert_True(matrixA[2][1] == 0.0f, "matrixA[0][0] == 0.0f");
	UtAssert_True(matrixA[2][2] == 0.0f, "matrixA[0][0] == 0.0f");
}

void Test_Matrix3F3_Inverse(void)
{
	math::Matrix3F3 matrixA(
		{2.0f, 1.0f, 1.0f},
		{1.0f, 2.0f, 1.0f},
		{1.0f, 1.0f, 2.0f}
	);

    math::Matrix3F3 matrix = matrixA.Inversed();

	/* Verify results */
	UtAssert_True(fabs(matrix[0][0] - (3.0f / 4.0f)) < 0.00001f, "matrix[0][0] == 3 / 4");
	UtAssert_True(fabs(matrix[0][1] - (- 1.0f / 4.0f)) < 0.00001f, "matrix[0][1] == - 1 / 4");
	UtAssert_True(fabs(matrix[0][2] - (- 1.0f / 4.0f)) < 0.00001f, "matrix[0][2] == - 1 / 4");
    
	UtAssert_True(fabs(matrix[1][0] - (- 1.0f / 4.0f)) < 0.00001f, "matrix[1][0] == - 1 / 4");
	UtAssert_True(fabs(matrix[1][1] - (3.0f / 4.0f)) < 0.00001f, "matrix[1][1] == 3 / 4");
	UtAssert_True(fabs(matrix[1][2] - (- 1.0f / 4.0f)) < 0.00001f, "matrix[1][2] == - 1 / 4");

	UtAssert_True(fabs(matrix[2][0] - (- 1.0f / 4.0f)) < 0.00001f, "matrix[2][0] == - 1 / 4");
	UtAssert_True(fabs(matrix[2][1] - (- 1.0f / 4.0f)) < 0.00001f, "matrix[2][1] == - 1 / 4");
	UtAssert_True(fabs(matrix[2][2] - (3.0f / 4.0f)) < 0.00001f, "matrix[2][2] == 3 / 4");

}

void Test_Matrix3F3_Determinant(void)
{
	//math::Matrix3F3 matrix(
		//{2.0f, 1.0f, 1.0f},
		//{1.0f, 2.0f, 1.0f},
		//{1.0f, 1.0f, 2.0f}
	//);

	//float result = 0;

    //result = matrix.Determinant();
    
    ////printf("result %f\n", result);

    //UtAssert_True(result == 4, "result == 4");
}

void Test_Matrix3F3_Inverse2(void)
{
	math::Matrix3F3 matrixA(
        {0.004737, -0.000000, 0.000000},
        {-0.000000, 0.004737, 0.000000}, 
        {0.000000, 0.000000, 0.002761} 
	);

    math::Matrix3F3 matrix = matrixA.Inversed();

    printf("inverse\n");
    for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			printf("%f, ", matrix[i][j]);
		}
		printf("\n");
	}

	/* Verify results */
	UtAssert_True(fabs(matrix[0][0] - (1000000.0f / 4737.0f)) < 0.0001f, "matrix[0][0] == 3 / 4");
	UtAssert_True(matrix[0][1] == 0, "matrix[0][1] == - 1 / 4");
	UtAssert_True(matrix[0][2] == 0, "matrix[0][2] == - 1 / 4");
    
	UtAssert_True(matrix[1][0] == 0, "matrix[1][0] == - 1 / 4");
	UtAssert_True(fabs(matrix[1][1] - (1000000.0f / 4737.0f)) < 0.0001f, "matrix[1][1] == 3 / 4");
	UtAssert_True(matrix[1][2] == 0, "matrix[1][2] == - 1 / 4");

	UtAssert_True(matrix[2][0] == 0, "matrix[2][0] == - 1 / 4");
	UtAssert_True(matrix[2][1] == 0, "matrix[2][1] == - 1 / 4");
	UtAssert_True(fabs(matrix[2][2] - (1000000.0f / 2761.0f)) < 0.0001f, "matrix[2][2] == 3 / 4");

}


/* Auto generated with GNU Octave, version 4.0.0 */
void Test_Matrix3F3_Mult_3F_Random(void)
{
    int i, j = 0;

    math::Matrix3F3 A (
        {-0.638700, -0.785700, -0.650849},
        {0.855696, -0.983750, -0.685714},
        {0.104937, -0.908615, -0.519282}
    );

    math::Vector3F B (
        {-0.379527,0.695147,-0.297576}
    );

    math::Vector3F expected (
        {-0.110096,-0.804559,-0.516922}
    );

    math::Vector3F result;
    result.Zero();

    result = A * B;

    for(i = 0; i < 3; i++)
    {
        UtAssert_True(fabs(result[i] - expected[i]) < 0.00001f, "fabs(result[i] - expected[i]) < 0.00001f");

    }
}


/* Auto generated with GNU Octave, version 4.0.0 */
void Test_Matrix3F3_Mult_3F3_Random(void)
{
    int i, j = 0;

    math::Matrix3F3 A (
        {-0.515712, -0.736414, -0.231187},
        {0.594396, -0.471127, -0.258234},
        {-0.479417, 0.628603, 0.732344}
    );

    math::Matrix3F3 B (
        {-0.804984, 0.134784, -0.553694},
        {0.669973, -0.573392, -0.071690},
        {-0.657323, 0.005375, -0.120588}
    );

    math::Matrix3F3 expected (
        {0.073727, 0.351502, 0.366218},
        {-0.624379, 0.348868, -0.264199},
        {0.325684, -0.421118, 0.132074}
    );

    math::Matrix3F3 result;
    result.Zero();

    result = A * B;

    for(i = 0; i < 3; i++)
    {
        for(j = 0; j < 3; j++)
        {
            UtAssert_True(fabs(result[i][j] - expected[i][j]) < 0.00001f, "fabs(result[i][j] - expected[i][j]) < 0.00001f");

        }
    }
}
