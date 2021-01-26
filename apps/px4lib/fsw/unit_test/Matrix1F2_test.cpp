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

#include "Matrix1F2_test.hpp"
#include <math/Matrix1F2.hpp>

#include "utassert.h"
#include <float.h>


void Test_Matrix1F2_Constructor(void)
{
    math::Matrix1F2 matrix({1.0f, 2.0f});

    /* Verify results */
    UtAssert_True(matrix[0][0] == 1.0f, "matrix[0][0] == 1.0f");
    UtAssert_True(matrix[0][1] == 2.0f, "matrix[0][1] == 2.0f");
}


void Test_Matrix1F2_IndexOutOfBounds(void)
{
    math::Matrix1F2 matrix({1.0f, 2.0f});

    /* Verify results */
    UtAssert_True(isnan(matrix[0][2]), "matrix[0][3] == NAN");
    UtAssert_True(isnan(matrix[1][0]), "matrix[1][0] == NAN");
}


void Test_Matrix1F2_Zero(void)
{
    math::Matrix1F2 matrixA({1.0f, 2.0f});
    math::Matrix1F2 matrixB;
    
    matrixB[0][0] = 2.0f;

    matrixA.Zero();
    matrixB.Zero();

    /* Verify results */
    UtAssert_True(matrixA[0][0] == 0.0f, "matrixA[0][0] == 0.0f");
    UtAssert_True(matrixA[0][1] == 0.0f, "matrixA[0][1] == 0.0f");
    UtAssert_True(matrixB[0][0] == 0.0f, "matrixB[0][0] == 0.0f");
    UtAssert_True(matrixB[0][1] == 0.0f, "matrixB[0][1] == 0.0f");
}


void Test_Matrix1F2_DotProduct(void)
{
    math::Matrix1F2 matrixA({1.0f, 2.0f});
    math::Vector2F vectorB({1.0f, 2.0f});
    float result;
    
    result = matrixA * vectorB;

    /* Verify results */
    UtAssert_True(result == 5.0f, "result == 5.0f");
}


/* Auto generated with GNU Octave, version 4.0.0 */
void Test_Matrix1F2_Mult_2F_Random(void)
{
    int i, j = 0;

    math::Matrix1F2 A (
        {-0.257305, 0.673107}
    );

    math::Vector2F B (
        {0.283640,-0.465289}
    );

    float expected = -0.386171;
    float result = A * B;

    for(i = 0; i < 1; i++)
    {
        UtAssert_True(fabs(result - expected) < 0.00001f, "fabs(result - expected) < 0.00001f");

    }
}

