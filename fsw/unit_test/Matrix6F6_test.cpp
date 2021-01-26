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

#include "Matrix6F6_test.hpp"
#include <math/Matrix6F6.hpp>

#include "utassert.h"
#include <float.h>


void Test_Matrix6F6_Constructor(void)
{
	math::Matrix6F6 matrix(
			{1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f},
			{7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f},
			{13.0f, 14.0f, 15.0f, 16.0f, 17.0f, 18.0f},
			{19.0f, 20.0f, 21.0f, 22.0f, 23.0f, 24.0f},
			{25.0f, 26.0f, 27.0f, 28.0f, 29.0f, 30.0f},
			{31.0f, 32.0f, 33.0f, 34.0f, 35.0f, 36.0f}
	);

	/* Verify results */
	UtAssert_True(matrix[0][0] == 1.0f, "matrix[0][0] == 1.0f");
	UtAssert_True(matrix[0][1] == 2.0f, "matrix[0][1] == 2.0f");
	UtAssert_True(matrix[0][2] == 3.0f, "matrix[0][2] == 3.0f");
	UtAssert_True(matrix[0][3] == 4.0f, "matrix[0][3] == 4.0f");
	UtAssert_True(matrix[0][4] == 5.0f, "matrix[1][4] == 5.0f");
	UtAssert_True(matrix[0][5] == 6.0f, "matrix[1][5] == 6.0f");
    
	UtAssert_True(matrix[1][0] == 7.0f, "matrix[1][0] == 7.0f");
	UtAssert_True(matrix[1][1] == 8.0f, "matrix[1][1] == 8.0f");
	UtAssert_True(matrix[1][2] == 9.0f, "matrix[1][2] == 9.0f");
	UtAssert_True(matrix[1][3] == 10.0f, "matrix[1][3] == 10.0f");
	UtAssert_True(matrix[1][4] == 11.0f, "matrix[1][4] == 11.0f");
	UtAssert_True(matrix[1][5] == 12.0f, "matrix[1][5] == 12.0f");

	UtAssert_True(matrix[2][0] == 13.0f, "matrix[2][0] == 13.0f");
	UtAssert_True(matrix[2][1] == 14.0f, "matrix[2][1] == 14.0f");
	UtAssert_True(matrix[2][2] == 15.0f, "matrix[2][2] == 15.0f");
	UtAssert_True(matrix[2][3] == 16.0f, "matrix[2][3] == 16.0f");
	UtAssert_True(matrix[2][4] == 17.0f, "matrix[2][4] == 17.0f");
	UtAssert_True(matrix[2][5] == 18.0f, "matrix[2][5] == 18.0f");

	UtAssert_True(matrix[3][0] == 19.0f, "matrix[3][0] == 19.0f");
	UtAssert_True(matrix[3][1] == 20.0f, "matrix[3][1] == 20.0f");
	UtAssert_True(matrix[3][2] == 21.0f, "matrix[3][2] == 21.0f");
	UtAssert_True(matrix[3][3] == 22.0f, "matrix[3][3] == 22.0f");
	UtAssert_True(matrix[3][4] == 23.0f, "matrix[3][4] == 23.0f");
	UtAssert_True(matrix[3][5] == 24.0f, "matrix[3][5] == 24.0f");

	UtAssert_True(matrix[4][0] == 25.0f, "matrix[4][0] == 25.0f");
	UtAssert_True(matrix[4][1] == 26.0f, "matrix[4][1] == 26.0f");
	UtAssert_True(matrix[4][2] == 27.0f, "matrix[4][2] == 27.0f");
	UtAssert_True(matrix[4][3] == 28.0f, "matrix[4][3] == 28.0f");
	UtAssert_True(matrix[4][4] == 29.0f, "matrix[4][4] == 29.0f");
	UtAssert_True(matrix[4][5] == 30.0f, "matrix[4][5] == 30.0f");

	UtAssert_True(matrix[5][0] == 31.0f, "matrix[5][0] == 31.0f");
	UtAssert_True(matrix[5][1] == 32.0f, "matrix[5][1] == 32.0f");
	UtAssert_True(matrix[5][2] == 33.0f, "matrix[5][2] == 33.0f");
	UtAssert_True(matrix[5][3] == 34.0f, "matrix[5][3] == 34.0f");
	UtAssert_True(matrix[5][4] == 35.0f, "matrix[5][4] == 35.0f");
	UtAssert_True(matrix[5][5] == 36.0f, "matrix[5][5] == 36.0f");
}


void Test_Matrix6F6_IndexOutOfBounds(void)
{
	math::Matrix6F6 matrix(
			{1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f},
			{7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f},
			{13.0f, 14.0f, 15.0f, 16.0f, 17.0f, 18.0f},
			{19.0f, 20.0f, 21.0f, 22.0f, 23.0f, 24.0f},
			{25.0f, 26.0f, 27.0f, 28.0f, 29.0f, 30.0f},
			{31.0f, 32.0f, 33.0f, 34.0f, 35.0f, 36.0f}
	);

	/* Verify results */
	UtAssert_True(isnan(matrix[6][0]), "matrix[6][0] == NAN");
	UtAssert_True(isnan(matrix[0][6]), "matrix[0][6] == NAN");
}


void Test_Matrix6F6_ArrayOperator(void)
{
	math::Matrix6F6 matrix(
			{1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f},
			{7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f},
			{13.0f, 14.0f, 15.0f, 16.0f, 17.0f, 18.0f},
			{19.0f, 20.0f, 21.0f, 22.0f, 23.0f, 24.0f},
			{25.0f, 26.0f, 27.0f, 28.0f, 29.0f, 30.0f},
			{31.0f, 32.0f, 33.0f, 34.0f, 35.0f, 36.0f}
	);

	float nanResultA = matrix[6][0];
	float nanResultB = matrix[0][6];

    UtAssert_True(isnan(matrix[6][0]), "matrix[6][0] == NAN");
    UtAssert_True(isnan(matrix[0][6]), "matrix[0][6] == NAN");

    UtAssert_True(isnan(nanResultA), "NAN == matrix[6][0]");
    UtAssert_True(isnan(nanResultB), "NAN == matrix[0][6]");
}


void Test_Matrix6F6_Determinant(void)
{
	//math::Matrix6F6 matrix(
			//{2.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
			//{1.0f, 2.0f, 1.0f, 1.0f, 1.0f, 1.0f},
			//{1.0f, 1.0f, 2.0f, 1.0f, 1.0f, 1.0f},
			//{1.0f, 1.0f, 1.0f, 2.0f, 1.0f, 1.0f},
			//{1.0f, 1.0f, 1.0f, 1.0f, 2.0f, 1.0f},
			//{1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 2.0f}
	//);

	//float result = 0;

    //result = matrix.Determinant();
    
    ////printf("result %f\n", result);

    //UtAssert_True(result == 7, "result == 7");
}


void Test_Matrix6F6_Mult_6F(void)
{
    int i = 0;
	math::Matrix6F6 matrix(
			{1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f},
			{1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f},
			{1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f},
			{1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f},
			{1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f},
			{1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f}
	);
    
    math::Vector6F vector({1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f});

    math::Matrix6F1 result;

    result = matrix * vector;

    for(i=0; i<6; i++)
    {
        UtAssert_True(result[i][0] == 91.0f, "result[i][0] == 91.0f");
    }
}


/* Auto generated with GNU Octave, version 4.0.0 */
void Test_Matrix6F6_Mult_6F_Random(void)
{
    int i, j = 0;

    math::Matrix6F6 A (
        {-0.326027, -0.570398, -0.778894, 0.736858, -0.345213, -0.634708},
        {0.222236, 0.811451, 0.418128, -0.086160, -0.646867, 0.068963},
        {0.536402, 0.049314, 0.208847, 0.778238, 0.535976, -0.354460},
        {0.274623, -0.250909, -0.669645, 0.627170, -0.455019, -0.774644},
        {-0.464610, -0.308188, 0.213210, 0.831566, -0.950975, -0.887808},
        {0.085582, -0.728881, 0.942790, 0.229487, -0.286778, 0.888910}
    );

    math::Vector6F B (
        {-0.288138,0.209131,-0.921779,0.327033,-0.518111,0.981905}
    );

    math::Vector6F expected (
        {0.489234,0.094930,-0.707986,0.165890,-0.234197,0.050323}
    );

    math::Matrix6F1 result;
    result.Zero();

    result = A * B;

    for(i = 0; i < 6; i++)
    {
        UtAssert_True(fabs(result[i][0] - expected[i]) < 0.00001f, "fabs(result[i] - expected[i]) < 0.00001f");

    }
}


void Test_Matrix6F6_Inverse(void)
{
	math::Matrix6F6 matrixA(
			{2.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
			{1.0f, 2.0f, 1.0f, 1.0f, 1.0f, 1.0f},
			{1.0f, 1.0f, 2.0f, 1.0f, 1.0f, 1.0f},
			{1.0f, 1.0f, 1.0f, 2.0f, 1.0f, 1.0f},
			{1.0f, 1.0f, 1.0f, 1.0f, 2.0f, 1.0f},
			{1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 2.0f}
	);

    math::Matrix6F6 matrix = matrixA.Inversed();

	/* Verify results */
	UtAssert_True(fabs(matrix[0][0] - (6.0f / 7.0f)) < 0.00001f, "matrix[0][0] == 6 / 7");
	UtAssert_True(fabs(matrix[0][1] - (- 1.0f / 7.0f)) < 0.00001f, "matrix[0][1] == - 1 / 7");
	UtAssert_True(fabs(matrix[0][2] - (- 1.0f / 7.0f)) < 0.00001f, "matrix[0][2] == - 1 / 7");
	UtAssert_True(fabs(matrix[0][3] - (- 1.0f / 7.0f)) < 0.00001f, "matrix[0][3] == - 1 / 7");
	UtAssert_True(fabs(matrix[0][4] - (- 1.0f / 7.0f)) < 0.00001f, "matrix[1][4] == - 1 / 7");
	UtAssert_True(fabs(matrix[0][5] - (- 1.0f / 7.0f)) < 0.00001f, "matrix[1][5] == - 1 / 7");
    
	UtAssert_True(fabs(matrix[1][0] - (- 1.0f / 7.0f)) < 0.00001f, "matrix[1][0] == - 1 / 7");
	UtAssert_True(fabs(matrix[1][1] - (6.0f / 7.0f)) < 0.00001f, "matrix[1][1] == 6 / 7");
	UtAssert_True(fabs(matrix[1][2] - (- 1.0f / 7.0f)) < 0.00001f, "matrix[1][2] == - 1 / 7");
	UtAssert_True(fabs(matrix[1][3] - (- 1.0f / 7.0f)) < 0.00001f, "matrix[1][3] == - 1 / 7");
	UtAssert_True(fabs(matrix[1][4] - (- 1.0f / 7.0f)) < 0.00001f, "matrix[1][4] == - 1 / 7");
	UtAssert_True(fabs(matrix[1][5] - (- 1.0f / 7.0f)) < 0.00001f, "matrix[1][5] == - 1 / 7");

	UtAssert_True(fabs(matrix[2][0] - (- 1.0f / 7.0f)) < 0.00001f, "matrix[2][0] == - 1 / 7");
	UtAssert_True(fabs(matrix[2][1] - (- 1.0f / 7.0f)) < 0.00001f, "matrix[2][1] == - 1 / 7f");
	UtAssert_True(fabs(matrix[2][2] - (6.0f / 7.0f)) < 0.00001f, "matrix[2][2] == 6 / 7");
	UtAssert_True(fabs(matrix[2][3] - (- 1.0f / 7.0f)) < 0.00001f, "matrix[2][3] == - 1 / 7");
	UtAssert_True(fabs(matrix[2][4] - (- 1.0f / 7.0f)) < 0.00001f, "matrix[2][4] == - 1 / 7");
	UtAssert_True(fabs(matrix[2][5] - (- 1.0f / 7.0f)) < 0.00001f, "matrix[2][5] == - 1 / 7");

	UtAssert_True(fabs(matrix[3][0] - (- 1.0f / 7.0f)) < 0.00001f, "matrix[3][0] == - 1 / 7");
	UtAssert_True(fabs(matrix[3][1] - (- 1.0f / 7.0f)) < 0.00001f, "matrix[3][1] == - 1 / 7");
	UtAssert_True(fabs(matrix[3][2] - (- 1.0f / 7.0f)) < 0.00001f, "matrix[3][2] == - 1 / 7");
	UtAssert_True(fabs(matrix[3][3] - (6.0f / 7.0f)) < 0.00001f, "matrix[3][3] == 6 / 7");
	UtAssert_True(fabs(matrix[3][4] - (- 1.0f / 7.0f)) < 0.00001f, "matrix[3][4] == - 1 / 7");
	UtAssert_True(fabs(matrix[3][5] - (- 1.0f / 7.0f)) < 0.00001f, "matrix[3][5] == - 1 / 7");

	UtAssert_True(fabs(matrix[4][0] - (- 1.0f / 7.0f)) < 0.00001f, "matrix[4][0] == - 1 / 7");
	UtAssert_True(fabs(matrix[4][1] - (- 1.0f / 7.0f)) < 0.00001f, "matrix[4][1] == - 1 / 7");
	UtAssert_True(fabs(matrix[4][2] - (- 1.0f / 7.0f)) < 0.00001f, "matrix[4][2] == - 1 / 7");
	UtAssert_True(fabs(matrix[4][3] - (- 1.0f / 7.0f)) < 0.00001f, "matrix[4][3] == - 1 / 7");
	UtAssert_True(fabs(matrix[4][4] - (6.0f / 7.0f)) < 0.00001f, "matrix[4][4] == 6 / 7");
	UtAssert_True(fabs(matrix[4][5] - (- 1.0f / 7.0f)) < 0.00001f, "matrix[4][5] == - 1 / 7");

	UtAssert_True(fabs(matrix[5][0] - (- 1.0f / 7.0f)) < 0.00001f, "matrix[5][0] == - 1 / 7");
	UtAssert_True(fabs(matrix[5][1] - (- 1.0f / 7.0f)) < 0.00001f, "matrix[5][1] == - 1 / 7");
	UtAssert_True(fabs(matrix[5][2] - (- 1.0f / 7.0f)) < 0.00001f, "matrix[5][2] == - 1 / 7");
	UtAssert_True(fabs(matrix[5][3] - (- 1.0f / 7.0f)) < 0.00001f, "matrix[5][3] == - 1 / 7");
	UtAssert_True(fabs(matrix[5][4] - (- 1.0f / 7.0f)) < 0.00001f, "matrix[5][4] == - 1 / 7");
	UtAssert_True(fabs(matrix[5][5] - (6.0f / 7.0f)) < 0.00001f, "matrix[5][5] == 6 / 7");
}


void Test_Matrix6F6_Addition(void)
{
	math::Matrix6F6 matrixA(
			{1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f},
			{1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f},
			{1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f},
			{1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f},
			{1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f},
			{1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f}
	);

	math::Matrix6F6 matrixB(
			{1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f},
			{1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f},
			{1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f},
			{1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f},
			{1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f},
			{1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f}
	);
    
    math::Matrix6F6 result;

    result = matrixA + matrixB;

	/* Verify results */
	UtAssert_True(result[0][0] == 2.0f, "result[0][0] == 2.0f");
	UtAssert_True(result[1][0] == 2.0f, "result[1][0] == 2.0f");
	UtAssert_True(result[2][0] == 2.0f, "result[2][0] == 2.0f");
	UtAssert_True(result[3][0] == 2.0f, "result[3][0] == 2.0f");
	UtAssert_True(result[4][0] == 2.0f, "result[4][0] == 2.0f");
	UtAssert_True(result[5][0] == 2.0f, "result[5][0] == 2.0f");

	UtAssert_True(result[0][1] == 4.0f, "result[0][1] == 4.0f");
	UtAssert_True(result[1][1] == 4.0f, "result[1][1] == 4.0f");
	UtAssert_True(result[2][1] == 4.0f, "result[2][1] == 4.0f");
	UtAssert_True(result[3][1] == 4.0f, "result[3][1] == 4.0f");
	UtAssert_True(result[4][1] == 4.0f, "result[4][1] == 4.0f");
	UtAssert_True(result[5][1] == 4.0f, "result[5][1] == 4.0f");

	UtAssert_True(result[0][2] == 6.0f, "result[0][2] == 6.0f");
	UtAssert_True(result[1][2] == 6.0f, "result[1][2] == 6.0f");
	UtAssert_True(result[2][2] == 6.0f, "result[2][2] == 6.0f");
	UtAssert_True(result[3][2] == 6.0f, "result[3][2] == 6.0f");
	UtAssert_True(result[4][2] == 6.0f, "result[4][2] == 6.0f");
	UtAssert_True(result[5][2] == 6.0f, "result[5][2] == 6.0f");

	UtAssert_True(result[0][3] == 8.0f, "result[0][3] == 8.0f");
	UtAssert_True(result[1][3] == 8.0f, "result[1][3] == 8.0f");
	UtAssert_True(result[2][3] == 8.0f, "result[2][3] == 8.0f");
	UtAssert_True(result[3][3] == 8.0f, "result[3][3] == 8.0f");
	UtAssert_True(result[4][3] == 8.0f, "result[4][3] == 8.0f");
	UtAssert_True(result[5][3] == 8.0f, "result[5][3] == 8.0f");

	UtAssert_True(result[0][4] == 10.0f, "result[0][4] == 10.0f");
	UtAssert_True(result[1][4] == 10.0f, "result[1][4] == 10.0f");
	UtAssert_True(result[2][4] == 10.0f, "result[2][4] == 10.0f");
	UtAssert_True(result[3][4] == 10.0f, "result[3][4] == 10.0f");
	UtAssert_True(result[4][4] == 10.0f, "result[4][4] == 10.0f");
	UtAssert_True(result[5][4] == 10.0f, "result[5][4] == 10.0f");

	UtAssert_True(result[0][5] == 12.0f, "result[0][5] == 12.0f");
	UtAssert_True(result[1][5] == 12.0f, "result[1][5] == 12.0f");
	UtAssert_True(result[2][5] == 12.0f, "result[2][5] == 12.0f");
	UtAssert_True(result[3][5] == 12.0f, "result[3][5] == 12.0f");
	UtAssert_True(result[4][5] == 12.0f, "result[4][5] == 12.0f");
	UtAssert_True(result[5][5] == 12.0f, "result[5][5] == 12.0f");
}


void Test_Matrix6F6_Zero(void)
{
    int i, j = 0;
	math::Matrix6F6 matrixA(
			{1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f},
			{1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f},
			{1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f},
			{1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f},
			{1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f},
			{1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f}
	);

    math::Matrix6F6 result;

    matrixA.Zero();

	/* Verify results */
    for(i=0; i<6; i++)
    {
        for(j=0; j<6; j++)
        {
            UtAssert_True(matrixA[i][j] == 0.0f, "matrix[i][j] == 0.0f");
        }
    }
}


void Test_Matrix6F6_SwapRows(void)
{
	math::Matrix6F6 matrix(
			{1.0f,   2.0f,  3.0f,  4.0f,  5.0f, 6.0f},
			{7.0f,   8.0f,  9.0f, 10.0f, 11.0f, 12.0f},
			{13.0f, 14.0f, 15.0f, 16.0f, 17.0f, 18.0f},
			{19.0f, 20.0f, 21.0f, 22.0f, 23.0f, 24.0f},
			{25.0f, 26.0f, 27.0f, 28.0f, 29.0f, 30.0f},
			{31.0f, 32.0f, 33.0f, 34.0f, 35.0f, 36.0f}
	);
    
    matrix.SwapRows(0,1);

    UtAssert_True(matrix[0][0] == 7.0f, "matrix[0][0] == 7.0f");
    UtAssert_True(matrix[0][1] == 8.0f, "matrix[0][1] == 8.0f");
    UtAssert_True(matrix[0][2] == 9.0f, "matrix[0][2] == 9.0f");
    UtAssert_True(matrix[0][3] == 10.0f, "matrix[0][3] == 10.0f");
    UtAssert_True(matrix[0][4] == 11.0f, "matrix[0][4] == 11.0f");
    UtAssert_True(matrix[0][5] == 12.0f, "matrix[0][5] == 12.0f");

    UtAssert_True(matrix[1][0] == 1.0f, "matrix[1][0] == 1.0f");
    UtAssert_True(matrix[1][1] == 2.0f, "matrix[1][1] == 2.0f");
    UtAssert_True(matrix[1][2] == 3.0f, "matrix[1][2] == 3.0f");
    UtAssert_True(matrix[1][3] == 4.0f, "matrix[1][3] == 4.0f");
    UtAssert_True(matrix[1][4] == 5.0f, "matrix[1][4] == 5.0f");
    UtAssert_True(matrix[1][5] == 6.0f, "matrix[1][5] == 6.0f");
}


void Test_Matrix6F6_SwapCols(void)
{
	math::Matrix6F6 matrix(
			{1.0f,   2.0f,  3.0f,  4.0f,  5.0f, 6.0f},
			{7.0f,   8.0f,  9.0f, 10.0f, 11.0f, 12.0f},
			{13.0f, 14.0f, 15.0f, 16.0f, 17.0f, 18.0f},
			{19.0f, 20.0f, 21.0f, 22.0f, 23.0f, 24.0f},
			{25.0f, 26.0f, 27.0f, 28.0f, 29.0f, 30.0f},
			{31.0f, 32.0f, 33.0f, 34.0f, 35.0f, 36.0f}
	);
    
    matrix.SwapCols(0,1);

    UtAssert_True(matrix[0][0] == 2.0f, "matrix[0][0] == 2.0f");
    UtAssert_True(matrix[1][0] == 8.0f, "matrix[1][0] == 8.0f");
    UtAssert_True(matrix[2][0] == 14.0f, "matrix[2][0] == 14.0f");
    UtAssert_True(matrix[3][0] == 20.0f, "matrix[3][0] == 20.0f");
    UtAssert_True(matrix[4][0] == 26.0f, "matrix[4][0] == 26.0f");
    UtAssert_True(matrix[5][0] == 32.0f, "matrix[5][0] == 32.0f");

    UtAssert_True(matrix[0][1] == 1.0f, "matrix[0][1] == 1.0f");
    UtAssert_True(matrix[1][1] == 7.0f, "matrix[1][1] == 7.0f");
    UtAssert_True(matrix[2][1] == 13.0f, "matrix[2][1] == 13.0f");
    UtAssert_True(matrix[3][1] == 19.0f, "matrix[3][1] == 19.0f");
    UtAssert_True(matrix[4][1] == 25.0f, "matrix[4][1] == 25.0f");
    UtAssert_True(matrix[5][1] == 31.0f, "matrix[5][1] == 31.0f");
}


	//math::Matrix6F6 matrixA(
			//{1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f},
			//{7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f},
			//{13.0f, 14.0f, 15.0f, 16.0f, 17.0f, 18.0f},
			//{19.0f, 20.0f, 21.0f, 22.0f, 23.0f, 24.0f},
			//{25.0f, 26.0f, 27.0f, 28.0f, 29.0f, 30.0f},
			//{31.0f, 32.0f, 33.0f, 34.0f, 35.0f, 36.0f}
	//);
