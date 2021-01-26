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

#include "Matrix6F1_test.hpp"
#include <math/Matrix6F1.hpp>

#include "utassert.h"
#include <float.h>


void Test_Matrix6F1_Constructor(void)
{
	math::Matrix6F1 matrix(
			{1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f}
	);

	/* Verify results */
	UtAssert_True(matrix[0][0] == 1.0f, "matrix[0][0] == 1.0f");
	UtAssert_True(matrix[1][0] == 2.0f, "matrix[1][0] == 2.0f");
	UtAssert_True(matrix[2][0] == 3.0f, "matrix[2][0] == 3.0f");
	UtAssert_True(matrix[3][0] == 4.0f, "matrix[3][0] == 4.0f");
	UtAssert_True(matrix[4][0] == 5.0f, "matrix[4][0] == 5.0f");
	UtAssert_True(matrix[5][0] == 6.0f, "matrix[5][0] == 6.0f");
}


void Test_Matrix6F1_IndexOutOfBounds(void)
{
	math::Matrix6F1 matrix(
			{1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f}
	);

	/* Verify results */
	UtAssert_True(isnan(matrix[6][0]), "matrix[6][0] == NAN");
	UtAssert_True(isnan(matrix[0][1]), "matrix[0][1] == NAN");
}


void Test_Matrix6F1_ArrayOperator(void)
{
	math::Matrix6F1 matrix(
			{1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f}
	);

	float nanResultA = matrix[6][0];
	float nanResultB = matrix[0][1];

    UtAssert_True(isnan(matrix[6][0]), "matrix[6][0] == NAN");
    UtAssert_True(isnan(matrix[0][1]), "matrix[0][1] == NAN");

    UtAssert_True(isnan(nanResultA), "NAN == matrix[6][0]");
    UtAssert_True(isnan(nanResultB), "NAN == matrix[0][1]");
}


void Test_Matrix6F1_Zero(void)
{
    int i = 0;
	math::Matrix6F1 matrixA(
			{1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f}
	);

    matrixA.Zero();

	/* Verify results */
    for(i=0; i<6; i++)
    {
        UtAssert_True(matrixA[i][0] == 0.0f, "matrix[i][0] == 0.0f");
    }


}

