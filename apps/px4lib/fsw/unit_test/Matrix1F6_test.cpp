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

#include "Matrix1F6_test.hpp"
#include <math/Matrix1F6.hpp>

#include "utassert.h"
#include <float.h>


void Test_Matrix1F6_Constructor(void)
{
    math::Matrix1F6 matrix({1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f});

    /* Verify results */
    UtAssert_True(matrix[0][0] == 1.0f, "matrix[0][0] == 1.0f");
    UtAssert_True(matrix[0][1] == 2.0f, "matrix[0][1] == 2.0f");
    UtAssert_True(matrix[0][2] == 3.0f, "matrix[0][2] == 3.0f");
    UtAssert_True(matrix[0][3] == 4.0f, "matrix[0][3] == 4.0f");
    UtAssert_True(matrix[0][4] == 5.0f, "matrix[0][4] == 5.0f");
    UtAssert_True(matrix[0][5] == 6.0f, "matrix[0][5] == 6.0f");
}


void Test_Matrix1F6_IndexOutOfBounds(void)
{
    math::Matrix1F6 matrix({1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f});

    /* Verify results */
    float result = matrix[0][6];
    UtAssert_True(isnan(result), "matrix[0][6] == NAN");

    result = matrix[1][0];
    UtAssert_True(isnan(result), "matrix[1][0] == NAN");
}


void Test_Matrix1F6_Zero(void)
{
    math::Matrix1F6 matrixA({1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f});
    math::Matrix1F6 matrixB;
    
    matrixB[0][0] = 2.0f;

    matrixA.Zero();
    matrixB.Zero();

    /* Verify results */
    UtAssert_True(matrixA[0][0] == 0.0f, "matrixA[0][0] == 0.0f");
    UtAssert_True(matrixA[0][1] == 0.0f, "matrixA[0][1] == 0.0f");
    UtAssert_True(matrixA[0][2] == 0.0f, "matrixA[0][2] == 0.0f");
    UtAssert_True(matrixA[0][3] == 0.0f, "matrixA[0][3] == 0.0f");
    UtAssert_True(matrixA[0][4] == 0.0f, "matrixA[0][4] == 0.0f");
    UtAssert_True(matrixA[0][5] == 0.0f, "matrixA[0][5] == 0.0f");
    UtAssert_True(matrixB[0][0] == 0.0f, "matrixB[0][0] == 0.0f");
    UtAssert_True(matrixB[0][1] == 0.0f, "matrixB[0][1] == 0.0f");
    UtAssert_True(matrixB[0][2] == 0.0f, "matrixB[0][2] == 0.0f");
    UtAssert_True(matrixB[0][3] == 0.0f, "matrixB[0][3] == 0.0f");
    UtAssert_True(matrixB[0][4] == 0.0f, "matrixB[0][4] == 0.0f");
    UtAssert_True(matrixB[0][5] == 0.0f, "matrixB[0][5] == 0.0f");
}


void Test_Matrix1F6_Mult_6F1(void)
{
    math::Matrix1F6 matrixA({1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f});
    math::Matrix6F1 matrixB(
        {1.0f},
        {2.0f}, 
        {3.0f}, 
        {4.0f}, 
        {5.0f}, 
        {6.0f}
    );
    math::Matrix1F1 result;

    result = matrixA * matrixB;

    /* Verify results */
    UtAssert_True(result[0][0] == 91.0f, "result[0][0] == 91.0f");
}


/* Auto generated with GNU Octave, version 4.0.0 */
void Test_Matrix1F6_Mult_6F1_Random(void)
{
    int i, j = 0;

    math::Matrix1F6 A (
        {0.622881, -0.048872, -0.457310, 0.469925, 0.240354, -0.844799}
    );

    math::Matrix6F1 B (
        {0.133304},
        {0.706889},
        {0.036132},
        {-0.427166},
        {-0.230887},
        {-0.474628}
    );

    math::Matrix1F1 expected (
        0.176696
    );

    math::Matrix1F1 result;
    result.Zero();

    result = A * B;

    for(i = 0; i < 1; i++)
    {
        for(j = 0; j < 1; j++)
        {
            UtAssert_True(fabs(result[i][j] - expected[i][j]) < 0.00001f, "fabs(result[i][j] - expected[i][j]) < 0.00001f");

        }
    }
}

