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

#include "Matrix1F1_test.hpp"
#include <math/Matrix1F1.hpp>

#include "utassert.h"
#include <float.h>


void Test_Matrix1F1_Constructor(void)
{
    math::Matrix1F1 matrix(1.0f);

    /* Verify results */
    UtAssert_True(matrix[0][0] == 1.0f, "matrix[0][0] == 1.0f");
}


void Test_Matrix1F1_IndexOutOfBounds(void)
{
    math::Matrix1F1 matrix(1.0f);

    /* Verify results */
    UtAssert_True(isnan(matrix[0][1]), "matrix[0][1] == NAN");
    UtAssert_True(isnan(matrix[1][0]), "matrix[1][0] == NAN");
}


void Test_Matrix1F1_Addition(void)
{
    math::Matrix1F1 matrixA(1.0f);
    math::Matrix1F1 matrixB(1.0f);
    math::Matrix1F1 result;
    
    result = matrixA + matrixB;

    /* Verify results */
    UtAssert_True(result[0][0] == 2.0f, "result[0][0] == 2.0f");
}


void Test_Matrix1F1_Zero(void)
{
    math::Matrix1F1 matrixA(1.0f);
    math::Matrix1F1 matrixB;
    
    matrixB[0][0] = 2.0f;

    matrixA.Zero();
    matrixB.Zero();

    /* Verify results */
    UtAssert_True(matrixA[0][0] == 0.0f, "matrixA[0][0] == 0.0f");
    UtAssert_True(matrixB[0][0] == 0.0f, "matrixB[0][0] == 0.0f");
}
