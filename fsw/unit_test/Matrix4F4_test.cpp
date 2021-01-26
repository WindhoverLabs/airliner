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

#include "Matrix4F4_test.hpp"
#include <math/Matrix4F4.hpp>

#include "utassert.h"
#include <float.h>


void Test_Matrix4F4_Constructor(void)
{
	math::Matrix4F4 matrix(
			{1.0f, 2.0f, 3.0f, 4.0f},
			{5.0f, 6.0f, 7.0f, 8.0f},
			{9.0f, 10.0f, 11.0f, 12.0f},
			{13.0f, 14.0f, 15.0f, 16.0f}
	);

	/* Verify results */
	UtAssert_True(matrix[0][0] == 1.0f, "matrix[0][0] == 1.0f");
	UtAssert_True(matrix[0][1] == 2.0f, "matrix[0][1] == 2.0f");
	UtAssert_True(matrix[0][2] == 3.0f, "matrix[0][2] == 3.0f");
	UtAssert_True(matrix[0][3] == 4.0f, "matrix[0][3] == 4.0f");
	UtAssert_True(matrix[1][0] == 5.0f, "matrix[1][0] == 5.0f");
	UtAssert_True(matrix[1][1] == 6.0f, "matrix[1][1] == 6.0f");
	UtAssert_True(matrix[1][2] == 7.0f, "matrix[1][2] == 7.0f");
	UtAssert_True(matrix[1][3] == 8.0f, "matrix[1][3] == 8.0f");
	UtAssert_True(matrix[2][0] == 9.0f, "matrix[2][0] == 9.0f");
	UtAssert_True(matrix[2][1] == 10.0f, "matrix[2][1] == 10.0f");
	UtAssert_True(matrix[2][2] == 11.0f, "matrix[2][2] == 11.0f");
	UtAssert_True(matrix[2][3] == 12.0f, "matrix[2][3] == 12.0f");
	UtAssert_True(matrix[3][0] == 13.0f, "matrix[3][0] == 13.0f");
	UtAssert_True(matrix[3][1] == 14.0f, "matrix[3][1] == 14.0f");
	UtAssert_True(matrix[3][2] == 15.0f, "matrix[3][2] == 15.0f");
	UtAssert_True(matrix[3][3] == 16.0f, "matrix[3][3] == 16.0f");
}


void Test_Matrix4F4_IndexOutOfBounds(void)
{
	math::Matrix4F4 matrix(
		{1.0f, 2.0f, 3.0f, 4.0f},
		{5.0f, 6.0f, 7.0f, 8.0f},
		{9.0f, 10.0f, 11.0f, 12.0f},
		{13.0f, 14.0f, 15.0f, 16.0f}
	);

	/* Verify results */
	UtAssert_True(isnan(matrix[3][4]), "matrix[3][4] == NAN");
	UtAssert_True(isnan(matrix[4][0]), "matrix[4][0] == NAN");
}


void Test_Matrix4F4_ArrayOperator(void)
{
	math::Matrix4F4 matrix(
		{1.0f, 2.0f, 3.0f, 4.0f},
		{5.0f, 6.0f, 7.0f, 8.0f},
		{9.0f, 10.0f, 11.0f, 12.0f},
		{13.0f, 14.0f, 15.0f, 16.0f}
	);
	float nanResultA = matrix[4][0];
	float nanResultB = matrix[3][4];

    UtAssert_True(isnan(matrix[4][0]), "matrix[4][0] == NAN");
    UtAssert_True(isnan(matrix[3][4]), "matrix[3][4] == NAN");

    UtAssert_True(isnan(nanResultA), "NAN == matrix[4][0]");
    UtAssert_True(isnan(nanResultB), "NAN == matrix[3][4]");
}


void Test_Matrix4F4_MultiplicationByVector(void)
{
    math::Vector4F vector1(1.5f, 2.5f, 3.5f, 4.5f);
    math::Vector4F vector2(4.5f, 5.5f, 6.5f, 7.5f);
    math::Vector4F vector3(8.5f, 9.5f, 10.5f, 11.5f);
    math::Vector4F vector4(12.5f, 13.5f, 14.5f, 15.5f);
    math::Vector4F vectorx(16.5f, 17.5f, 18.5f, 19.5f);
    math::Vector4F result(0.0f, 0.0f, 0.0f, 0.0f);
    math::Vector4F expected(221.0f, 437.0f, 1013.0f, 0.0f);
    math::Matrix4F4 existing(vector1, vector2, vector3, vector4);
    
    result = existing * vectorx;
    
    UtAssert_True(result[0] == expected[0], "result[0] == 221.0f");
	UtAssert_True(result[0] == expected[0], "result[1] == 437.0f");
	UtAssert_True(result[0] == expected[0], "result[2] == 1013.0f");
	UtAssert_True(result[0] == expected[0], "result[3] == 0.0f");
}


/* Auto generated with GNU Octave, version 4.0.0 */
void Test_Matrix4F4_Mult_4F_Random(void)
{
    int i, j = 0;

    math::Matrix4F4 A (
        {-0.829441, 0.933767, -0.141631, -0.864443},
        {-0.535494, -0.404551, -0.608416, 0.466376},
        {-0.959111, -0.214757, 0.863463, 0.567176},
        {-0.925036, -0.172581, 0.274853, -0.668142}
    );

    math::Vector4F B (
        {-0.832417,-0.999814,0.492776,-0.435221}
    );

    math::Vector4F expected (
        {0.063279,0.347441,1.191744,1.368795}
    );

    math::Vector4F result;
    result.Zero();

    result = A * B;

    for(i = 0; i < 4; i++)
    {
        UtAssert_True(fabs(result[i] - expected[i]) < 0.00001f, "fabs(result[i] - expected[i]) < 0.00001f");

    }
}


void Test_Matrix4F4_NegateOperator(void)
{
//	/* vectorActual = vectorA - vectorB */
//	math::Matrix4F4 vectorA(1.5f, 2.5f, 3.5f, 4.5f);
//	math::Matrix4F4 vectorActual = -vectorA;

//	/* Verify results */
//	UtAssert_True(vectorActual[0] == -9.0, "vectorActual[0] == vectorA[0] - vectorB[0]");
//	UtAssert_True(vectorActual[1] == -18.0, "vectorActual[1] == vectorA[1] - vectorB[1]");
//	UtAssert_True(vectorActual[2] == -27.0, "vectorActual[2] == vectorA[2] - vectorB[2]");
//	UtAssert_True(vectorActual[3] == -27.0, "vectorActual[3] == vectorA[3] - vectorB[3]");
}


