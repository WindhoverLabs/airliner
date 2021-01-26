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

#include "Matrix10F1_test.hpp"
#include <math/Matrix10F1.hpp>
#include <math/Matrix1F10.hpp>
#include <math/Matrix1F1.hpp>
#include <math/Vector1F.hpp>

#include "utassert.h"
#include <float.h>


void Test_Matrix10F1_Constructor(void)
{
    math::Matrix10F1 matrix(
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f}
    );

    /* Verify results */
    UtAssert_True(matrix[0][0] == 1.0f, "matrix[0][0] == 1.0f");
    UtAssert_True(matrix[1][0] == 2.0f, "matrix[1][0] == 2.0f");
    UtAssert_True(matrix[2][0] == 3.0f, "matrix[2][0] == 3.0f");
    UtAssert_True(matrix[3][0] == 4.0f, "matrix[3][0] == 4.0f");
    UtAssert_True(matrix[4][0] == 5.0f, "matrix[4][0] == 5.0f");
    UtAssert_True(matrix[5][0] == 6.0f, "matrix[5][0] == 6.0f");
    UtAssert_True(matrix[6][0] == 7.0f, "matrix[6][0] == 7.0f");
    UtAssert_True(matrix[7][0] == 8.0f, "matrix[7][0] == 8.0f");
    UtAssert_True(matrix[8][0] == 9.0f, "matrix[8][0] == 9.0f");
    UtAssert_True(matrix[9][0] == 10.0f, "matrix[9][0] == 9.0f");
}


void Test_Matrix10F1_Transpose(void)
{
    math::Matrix10F1 matrixA(
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f}
    );
    
    math::Matrix1F10 result;
    
    result = matrixA.Transpose();
    
    /* Verify results */

    UtAssert_True(result[0][0] == 1.0f, "matrixA[0][0] == 1.0f");
    UtAssert_True(result[0][1] == 2.0f, "matrixA[0][1] == 2.0f");
    UtAssert_True(result[0][2] == 3.0f, "matrixA[0][2] == 3.0f");
    UtAssert_True(result[0][3] == 4.0f, "matrixA[0][3] == 4.0f");
    UtAssert_True(result[0][4] == 5.0f, "matrixA[0][4] == 5.0f");
    UtAssert_True(result[0][5] == 6.0f, "matrixA[0][5] == 6.0f");
    UtAssert_True(result[0][6] == 7.0f, "matrixA[0][6] == 7.0f");
    UtAssert_True(result[0][7] == 8.0f, "matrixA[0][7] == 8.0f");
    UtAssert_True(result[0][8] == 9.0f, "matrixA[0][8] == 9.0f");
    UtAssert_True(result[0][9] == 10.0f, "result[0][9] == 10.0f");
    UtAssert_True(matrixA[0][0] == 1.0f, "result[0][0] == 1.0f");
    UtAssert_True(matrixA[1][0] == 2.0f, "result[1][0] == 2.0f");
    UtAssert_True(matrixA[2][0] == 3.0f, "result[2][0] == 3.0f");
    UtAssert_True(matrixA[3][0] == 4.0f, "result[3][0] == 4.0f");
    UtAssert_True(matrixA[4][0] == 5.0f, "result[4][0] == 5.0f");
    UtAssert_True(matrixA[5][0] == 6.0f, "result[5][0] == 6.0f");
    UtAssert_True(matrixA[6][0] == 7.0f, "result[6][0] == 7.0f");
    UtAssert_True(matrixA[7][0] == 8.0f, "result[7][0] == 8.0f");
    UtAssert_True(matrixA[8][0] == 9.0f, "result[8][0] == 9.0f");
    UtAssert_True(matrixA[9][0] == 10.0f, "result[9][0] == 10.0f");
}


void Test_Matrix10F1_Mult_1F(void)
{
    math::Matrix10F1 matrixA(
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f}
    );
    math::Vector1F vectorB(10.0f);
    
    math::Matrix10F1 result;
    result.Zero();

    result = matrixA * vectorB;

    UtAssert_True(result[0][0] == 10.0f, "result[0][0] == 10.0f");
    UtAssert_True(result[1][0] == 20.0f, "result[1][0] == 20.0f");
    UtAssert_True(result[2][0] == 30.0f, "result[2][0] == 30.0f");
    UtAssert_True(result[3][0] == 40.0f, "result[3][0] == 40.0f");
    UtAssert_True(result[4][0] == 50.0f, "result[4][0] == 50.0f");
    UtAssert_True(result[5][0] == 60.0f, "result[5][0] == 60.0f");
    UtAssert_True(result[6][0] == 70.0f, "result[6][0] == 70.0f");
    UtAssert_True(result[7][0] == 80.0f, "result[7][0] == 80.0f");
    UtAssert_True(result[8][0] == 90.0f, "result[8][0] == 90.0f");
    UtAssert_True(result[9][0] == 100.0f, "result[9][0] == 100.0f");
}


void Test_Matrix10F1_Mult_1F1(void)
{
    math::Matrix10F1 matrixA(
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f}
    );
    
    math::Matrix1F1 matrixB(10.0f);
    
    math::Matrix10F1 result;
    result.Zero();
    
    result = matrixA * matrixB;
    
    UtAssert_True(result[0][0] == 10.0f, "result[0][0] == 10.0f");
    UtAssert_True(result[1][0] == 20.0f, "result[1][0] == 20.0f");
    UtAssert_True(result[2][0] == 30.0f, "result[2][0] == 30.0f");
    UtAssert_True(result[3][0] == 40.0f, "result[3][0] == 40.0f");
    UtAssert_True(result[4][0] == 50.0f, "result[4][0] == 50.0f");
    UtAssert_True(result[5][0] == 60.0f, "result[5][0] == 60.0f");
    UtAssert_True(result[6][0] == 70.0f, "result[6][0] == 70.0f");
    UtAssert_True(result[7][0] == 80.0f, "result[7][0] == 80.0f");
    UtAssert_True(result[8][0] == 90.0f, "result[8][0] == 90.0f");
    UtAssert_True(result[9][0] == 100.0f, "result[9][0] == 100.0f");
}


/* Auto generated with GNU Octave, version 4.0.0 */
void Test_Matrix10F1_Mult_1F_Random(void)
{
    int i, j = 0;

    math::Matrix10F1 A (
        {-0.763635},
        {0.949220},
        {0.012934},
        {0.189517},
        {-0.650445},
        {0.667856},
        {0.183265},
        {-0.206743},
        {-0.035127},
        {-0.426673}
    );

    math::Vector1F B (
        0.560754
    );

    math::Matrix10F1 expected (
        {-0.428212},
        {0.532279},
        {0.007253},
        {0.106272},
        {-0.364740},
        {0.374503},
        {0.102766},
        {-0.115932},
        {-0.019698},
        {-0.239259}
    );

    math::Matrix10F1 result;
    result.Zero();

    result = A * B;

    for(i = 0; i < 10; i++)
    {
        UtAssert_True(fabs(result[i][0] - expected[i][0]) < 0.00001f, "fabs(result[i] - expected[i]) < 0.00001f");

    }
}


/* Auto generated with GNU Octave, version 4.0.0 */
void Test_Matrix10F1_Mult_1F1_Random(void)
{
    int i, j = 0;

    math::Matrix10F1 A (
        {-0.229563},
        {-0.875810},
        {-0.024407},
        {-0.986076},
        {-0.324781},
        {-0.708992},
        {-0.655302},
        {-0.992401},
        {0.062881},
        {-0.570661}
    );

    math::Matrix1F1 B (
        0.629858
    );

    math::Matrix10F1 expected (
        {-0.144592},
        {-0.551636},
        {-0.015373},
        {-0.621088},
        {-0.204566},
        {-0.446564},
        {-0.412747},
        {-0.625071},
        {0.039606},
        {-0.359435}
    );

    math::Matrix10F1 result;
    result.Zero();

    result = A * B;

    for(i = 0; i < 10; i++)
    {
        for(j = 0; j < 1; j++)
        {
            UtAssert_True(fabs(result[i][j] - expected[i][j]) < 0.00001f, "fabs(result[i][j] - expected[i][j]) < 0.00001f");

        }
    }
}

void Test_Matrix10F1_Mult_1F10(void)
{
    math::Matrix10F1 matrixA(
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f}
    );
    math::Matrix1F10 matrixB(
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f}
    );
    
    math::Matrix10F10 result;
    result.Zero();
    
    result = matrixA * matrixB;

    UtAssert_True(result[0][0] == 1.0f, "result[0][0] == 1.0f");
    UtAssert_True(result[0][1] == 2.0f, "result[0][0] == 2.0f");
    UtAssert_True(result[0][2] == 3.0f, "result[0][0] == 3.0f");
    UtAssert_True(result[0][3] == 4.0f, "result[0][0] == 4.0f");
    UtAssert_True(result[0][4] == 5.0f, "result[0][0] == 5.0f");
    UtAssert_True(result[0][5] == 6.0f, "result[0][0] == 6.0f");
    UtAssert_True(result[0][6] == 7.0f, "result[0][0] == 7.0f");
    UtAssert_True(result[0][7] == 8.0f, "result[0][0] == 8.0f");
    UtAssert_True(result[0][8] == 9.0f, "result[0][0] == 9.0f");
    UtAssert_True(result[0][9] == 10.0f, "result[0][0] == 10.0f");

    UtAssert_True(result[1][0] == 1.0f * 2.0f, "result[0][0] == 1.0f * 2.0f");
    UtAssert_True(result[1][1] == 2.0f * 2.0f, "result[0][0] == 2.0f * 2.0f");
    UtAssert_True(result[1][2] == 3.0f * 2.0f, "result[0][0] == 3.0f * 2.0f");
    UtAssert_True(result[1][3] == 4.0f * 2.0f, "result[0][0] == 4.0f * 2.0f");
    UtAssert_True(result[1][4] == 5.0f * 2.0f, "result[0][0] == 5.0f * 2.0f");
    UtAssert_True(result[1][5] == 6.0f * 2.0f, "result[0][0] == 6.0f * 2.0f");
    UtAssert_True(result[1][6] == 7.0f * 2.0f, "result[0][0] == 7.0f * 2.0f");
    UtAssert_True(result[1][7] == 8.0f * 2.0f, "result[0][0] == 8.0f * 2.0f");
    UtAssert_True(result[1][8] == 9.0f * 2.0f, "result[0][0] == 9.0f * 2.0f");
    UtAssert_True(result[1][9] == 10.0f * 2.0f, "result[0][0] == 10.0f * 2.0f");

    UtAssert_True(result[2][0] == 1.0f * 3.0f, "result[0][0] == 1.0f * 3.0f");
    UtAssert_True(result[2][1] == 2.0f * 3.0f, "result[0][0] == 2.0f * 3.0f");
    UtAssert_True(result[2][2] == 3.0f * 3.0f, "result[0][0] == 3.0f * 3.0f");
    UtAssert_True(result[2][3] == 4.0f * 3.0f, "result[0][0] == 4.0f * 3.0f");
    UtAssert_True(result[2][4] == 5.0f * 3.0f, "result[0][0] == 5.0f * 3.0f");
    UtAssert_True(result[2][5] == 6.0f * 3.0f, "result[0][0] == 6.0f * 3.0f");
    UtAssert_True(result[2][6] == 7.0f * 3.0f, "result[0][0] == 7.0f * 3.0f");
    UtAssert_True(result[2][7] == 8.0f * 3.0f, "result[0][0] == 8.0f * 3.0f");
    UtAssert_True(result[2][8] == 9.0f * 3.0f, "result[0][0] == 9.0f * 3.0f");
    UtAssert_True(result[2][9] == 10.0f * 3.0f, "result[0][0] == 10.0f * 3.0f");

    UtAssert_True(result[3][0] == 1.0f * 4.0f, "result[0][0] == 1.0f * 4.0f");
    UtAssert_True(result[3][1] == 2.0f * 4.0f, "result[0][0] == 2.0f * 4.0f");
    UtAssert_True(result[3][2] == 3.0f * 4.0f, "result[0][0] == 3.0f * 4.0f");
    UtAssert_True(result[3][3] == 4.0f * 4.0f, "result[0][0] == 4.0f * 4.0f");
    UtAssert_True(result[3][4] == 5.0f * 4.0f, "result[0][0] == 5.0f * 4.0f");
    UtAssert_True(result[3][5] == 6.0f * 4.0f, "result[0][0] == 6.0f * 4.0f");
    UtAssert_True(result[3][6] == 7.0f * 4.0f, "result[0][0] == 7.0f * 4.0f");
    UtAssert_True(result[3][7] == 8.0f * 4.0f, "result[0][0] == 8.0f * 4.0f");
    UtAssert_True(result[3][8] == 9.0f * 4.0f, "result[0][0] == 9.0f * 4.0f");
    UtAssert_True(result[3][9] == 10.0f * 4.0f, "result[0][0] == 10.0f * 4.0f");

    UtAssert_True(result[4][0] == 1.0f * 5.0f, "result[0][0] == 1.0f * 5.0f");
    UtAssert_True(result[4][1] == 2.0f * 5.0f, "result[0][0] == 2.0f * 5.0f");
    UtAssert_True(result[4][2] == 3.0f * 5.0f, "result[0][0] == 3.0f * 5.0f");
    UtAssert_True(result[4][3] == 4.0f * 5.0f, "result[0][0] == 4.0f * 5.0f");
    UtAssert_True(result[4][4] == 5.0f * 5.0f, "result[0][0] == 5.0f * 5.0f");
    UtAssert_True(result[4][5] == 6.0f * 5.0f, "result[0][0] == 6.0f * 5.0f");
    UtAssert_True(result[4][6] == 7.0f * 5.0f, "result[0][0] == 7.0f * 5.0f");
    UtAssert_True(result[4][7] == 8.0f * 5.0f, "result[0][0] == 8.0f * 5.0f");
    UtAssert_True(result[4][8] == 9.0f * 5.0f, "result[0][0] == 9.0f * 5.0f");
    UtAssert_True(result[4][9] == 10.0f * 5.0f, "result[0][0] == 10.0f * 5.0f");

    UtAssert_True(result[5][0] == 1.0f * 6.0f, "result[0][0] == 1.0f * 6.0f");
    UtAssert_True(result[5][1] == 2.0f * 6.0f, "result[0][0] == 2.0f * 6.0f");
    UtAssert_True(result[5][2] == 3.0f * 6.0f, "result[0][0] == 3.0f * 6.0f");
    UtAssert_True(result[5][3] == 4.0f * 6.0f, "result[0][0] == 4.0f * 6.0f");
    UtAssert_True(result[5][4] == 5.0f * 6.0f, "result[0][0] == 5.0f * 6.0f");
    UtAssert_True(result[5][5] == 6.0f * 6.0f, "result[0][0] == 6.0f * 6.0f");
    UtAssert_True(result[5][6] == 7.0f * 6.0f, "result[0][0] == 7.0f * 6.0f");
    UtAssert_True(result[5][7] == 8.0f * 6.0f, "result[0][0] == 8.0f * 6.0f");
    UtAssert_True(result[5][8] == 9.0f * 6.0f, "result[0][0] == 9.0f * 6.0f");
    UtAssert_True(result[5][9] == 10.0f * 6.0f, "result[0][0] == 10.0f * 6.0f");
    
    UtAssert_True(result[6][0] == 1.0f * 7.0f, "result[0][0] == 1.0f * 7.0f");
    UtAssert_True(result[6][1] == 2.0f * 7.0f, "result[0][0] == 2.0f * 7.0f");
    UtAssert_True(result[6][2] == 3.0f * 7.0f, "result[0][0] == 3.0f * 7.0f");
    UtAssert_True(result[6][3] == 4.0f * 7.0f, "result[0][0] == 4.0f * 7.0f");
    UtAssert_True(result[6][4] == 5.0f * 7.0f, "result[0][0] == 5.0f * 7.0f");
    UtAssert_True(result[6][5] == 6.0f * 7.0f, "result[0][0] == 6.0f * 7.0f");
    UtAssert_True(result[6][6] == 7.0f * 7.0f, "result[0][0] == 7.0f * 7.0f");
    UtAssert_True(result[6][7] == 8.0f * 7.0f, "result[0][0] == 8.0f * 7.0f");
    UtAssert_True(result[6][8] == 9.0f * 7.0f, "result[0][0] == 9.0f * 7.0f");
    UtAssert_True(result[6][9] == 10.0f * 7.0f, "result[0][0] == 10.0f * 7.0f");

    UtAssert_True(result[7][0] == 1.0f * 8.0f, "result[0][0] == 1.0f * 8.0f");
    UtAssert_True(result[7][1] == 2.0f * 8.0f, "result[0][0] == 2.0f * 8.0f");
    UtAssert_True(result[7][2] == 3.0f * 8.0f, "result[0][0] == 3.0f * 8.0f");
    UtAssert_True(result[7][3] == 4.0f * 8.0f, "result[0][0] == 4.0f * 8.0f");
    UtAssert_True(result[7][4] == 5.0f * 8.0f, "result[0][0] == 5.0f * 8.0f");
    UtAssert_True(result[7][5] == 6.0f * 8.0f, "result[0][0] == 6.0f * 8.0f");
    UtAssert_True(result[7][6] == 7.0f * 8.0f, "result[0][0] == 7.0f * 8.0f");
    UtAssert_True(result[7][7] == 8.0f * 8.0f, "result[0][0] == 8.0f * 8.0f");
    UtAssert_True(result[7][8] == 9.0f * 8.0f, "result[0][0] == 9.0f * 8.0f");
    UtAssert_True(result[7][9] == 10.0f * 8.0f, "result[0][0] == 10.0f * 8.0f");
    
    UtAssert_True(result[8][0] == 1.0f * 9.0f, "result[0][0] == 1.0f * 9.0f");
    UtAssert_True(result[8][1] == 2.0f * 9.0f, "result[0][0] == 2.0f * 9.0f");
    UtAssert_True(result[8][2] == 3.0f * 9.0f, "result[0][0] == 3.0f * 9.0f");
    UtAssert_True(result[8][3] == 4.0f * 9.0f, "result[0][0] == 4.0f * 9.0f");
    UtAssert_True(result[8][4] == 5.0f * 9.0f, "result[0][0] == 5.0f * 9.0f");
    UtAssert_True(result[8][5] == 6.0f * 9.0f, "result[0][0] == 6.0f * 9.0f");
    UtAssert_True(result[8][6] == 7.0f * 9.0f, "result[0][0] == 7.0f * 9.0f");
    UtAssert_True(result[8][7] == 8.0f * 9.0f, "result[0][0] == 8.0f * 9.0f");
    UtAssert_True(result[8][8] == 9.0f * 9.0f, "result[0][0] == 9.0f * 9.0f");
    UtAssert_True(result[8][9] == 10.0f * 9.0f, "result[0][0] == 10.0f * 9.0f");

    UtAssert_True(result[9][0] == 1.0f * 10.0f, "result[0][0] == 1.0f * 10.0f");
    UtAssert_True(result[9][1] == 2.0f * 10.0f, "result[0][0] == 2.0f * 10.0f");
    UtAssert_True(result[9][2] == 3.0f * 10.0f, "result[0][0] == 3.0f * 10.0f");
    UtAssert_True(result[9][3] == 4.0f * 10.0f, "result[0][0] == 4.0f * 10.0f");
    UtAssert_True(result[9][4] == 5.0f * 10.0f, "result[0][0] == 5.0f * 10.0f");
    UtAssert_True(result[9][5] == 6.0f * 10.0f, "result[0][0] == 6.0f * 10.0f");
    UtAssert_True(result[9][6] == 7.0f * 10.0f, "result[0][0] == 7.0f * 10.0f");
    UtAssert_True(result[9][7] == 8.0f * 10.0f, "result[0][0] == 8.0f * 10.0f");
    UtAssert_True(result[9][8] == 9.0f * 10.0f, "result[0][0] == 9.0f * 10.0f");
    UtAssert_True(result[9][9] == 10.0f * 10.0f, "result[0][0] == 10.0f * 10.0f");
}


/* Auto generated with GNU Octave, version 4.0.0 */
void Test_Matrix10F1_Mult_1F10_Random(void)
{
    int i, j = 0;

    math::Matrix10F1 A (
        {0.323595},
        {-0.399958},
        {0.449576},
        {-0.984620},
        {-0.845570},
        {-0.305761},
        {0.905133},
        {-0.582586},
        {0.784562},
        {-0.869613}
    );

    math::Matrix1F10 B (
        {-0.401048, -0.928099, -0.848122, -0.886731, 0.457584, 0.010263, 0.074156, -0.284138, 0.313587, -0.310383}
    );

    math::Matrix10F10 expected (
        {-0.129777, -0.300328, -0.274448, -0.286941, 0.148072, 0.003321, 0.023996, -0.091945, 0.101475, -0.100438},
        {0.160402, 0.371200, 0.339213, 0.354655, -0.183014, -0.004105, -0.029659, 0.113643, -0.125422, 0.124140},
        {-0.180302, -0.417250, -0.381295, -0.398653, 0.205718, 0.004614, 0.033339, -0.127741, 0.140981, -0.139541},
        {0.394880, 0.913824, 0.835078, 0.873093, -0.450546, -0.010105, -0.073015, 0.279768, -0.308764, 0.305609},
        {0.339115, 0.784773, 0.717147, 0.749794, -0.386919, -0.008678, -0.062704, 0.240258, -0.265160, 0.262451},
        {0.122625, 0.283776, 0.259322, 0.271127, -0.139911, -0.003138, -0.022674, 0.086878, -0.095883, 0.094903},
        {-0.363002, -0.840053, -0.767664, -0.802610, 0.414174, 0.009289, 0.067121, -0.257182, 0.283838, -0.280938},
        {0.233645, 0.540697, 0.494104, 0.516597, -0.266582, -0.005979, -0.043202, 0.165535, -0.182691, 0.180825},
        {-0.314647, -0.728151, -0.665404, -0.695696, 0.359003, 0.008052, 0.058180, -0.222924, 0.246028, -0.243515},
        {0.348757, 0.807086, 0.737538, 0.771113, -0.397920, -0.008925, -0.064487, 0.247090, -0.272699, 0.269913}
    );

    math::Matrix10F10 result;
    result.Zero();

    result = A * B;

    for(i = 0; i < 10; i++)
    {
        for(j = 0; j < 10; j++)
        {
            UtAssert_True(fabs(result[i][j] - expected[i][j]) < 0.00001f, "fabs(result[i][j] - expected[i][j]) < 0.00001f");

        }
    }
}
