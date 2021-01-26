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

#include "Matrix1F10_test.hpp"
#include <math/Matrix1F10.hpp>
#include <math/Matrix10F1.hpp>
#include <math/Matrix10F10.hpp>

#include "utassert.h"
#include <float.h>


void Test_Matrix1F10_Constructor(void)
{
    math::Matrix1F10 matrix(
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f}
    );

    /* Verify results */
    UtAssert_True(matrix[0][0] == 1.0f, "matrix[0][0] == 1.0f");
    UtAssert_True(matrix[0][1] == 2.0f, "matrix[0][1] == 2.0f");
    UtAssert_True(matrix[0][2] == 3.0f, "matrix[0][2] == 3.0f");
    UtAssert_True(matrix[0][3] == 4.0f, "matrix[0][3] == 4.0f");
    UtAssert_True(matrix[0][4] == 5.0f, "matrix[0][4] == 5.0f");
    UtAssert_True(matrix[0][5] == 6.0f, "matrix[0][5] == 6.0f");
    UtAssert_True(matrix[0][6] == 7.0f, "matrix[0][6] == 7.0f");
    UtAssert_True(matrix[0][7] == 8.0f, "matrix[0][7] == 8.0f");
    UtAssert_True(matrix[0][8] == 9.0f, "matrix[0][8] == 9.0f");
    UtAssert_True(matrix[0][9] == 10.0f, "matrix[0][8] == 9.0f");
}


void Test_Matrix1F10_IndexOutOfBounds(void)
{
    math::Matrix1F10 matrix({1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 
                             8.0f, 9.0f, 10.0f});

    /* Verify results */
    UtAssert_True(isnan(matrix[0][10]), "matrix[0][10] == NAN");
    UtAssert_True(isnan(matrix[1][0]), "matrix[1][0] == NAN");
}


void Test_Matrix1F10_Mult_10F10(void)
{
    math::Matrix1F10 matrixA(
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f}
    );
    math::Matrix10F10 matrixB(
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f},
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f},
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f},
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f},
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f},
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f},
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f},
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f},
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f},
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f}
    );
    
    math::Matrix1F10 result;
    result.Zero();
    result = matrixA * matrixB;

    /* Verify results */
    UtAssert_True(result[0][0] == 55.0f, "result[0][0] == 55.0f");
    UtAssert_True(result[0][1] == 110.0f, "result[0][1] == 110.0f");
    UtAssert_True(result[0][2] == 165.0f, "result[0][2] == 165.0f");
    UtAssert_True(result[0][3] == 220.0f, "result[0][3] == 220.0f");
    UtAssert_True(result[0][4] == 275.0f, "result[0][4] == 275.0f");
    UtAssert_True(result[0][5] == 330.0f, "result[0][5] == 330.0f");
    UtAssert_True(result[0][6] == 385.0f, "result[0][6] == 385.0f");
    UtAssert_True(result[0][7] == 440.0f, "result[0][7] == 440.0f");
    UtAssert_True(result[0][8] == 495.0f, "result[0][8] == 495.0f");
    UtAssert_True(result[0][9] == 550.0f, "result[0][9] == 550.0f");
}


void Test_Matrix1F10_Mult_10F(void)
{
    math::Matrix1F10 matrixA(
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f}
    );
    math::Vector10F vectorB(
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f}
    );
    
    math::Vector1F result;
    result.Zero();
    result = matrixA * vectorB;

    /* Verify results */
    UtAssert_True(result[0] == 385.0f, "result[0] == 385.0f");
}


/* Auto generated with GNU Octave, version 4.0.0 */
void Test_Matrix1F10_Mult_10F_Random(void)
{
    int i, j = 0;

    math::Matrix1F10 A (
        {-0.445110, 0.160485, -0.051881, 0.756216, 0.341298, 0.334414, 0.082746, 0.614596, 0.886050, -0.116662}
    );

    math::Vector10F B (
        {0.645302,0.300880,0.812591,-0.633887,0.784374,-0.134481,-0.335494,0.109816,0.392515,-0.479122}
    );

    math::Vector1F expected (
        {-0.094309}
    );

    math::Vector1F result;
    result.Zero();

    result = A * B;

    for(i = 0; i < 1; i++)
    {
        UtAssert_True(fabs(result[i] - expected[i]) < 0.00001f, "fabs(result[i] - expected[i]) < 0.00001f");

    }
}


void Test_Matrix1F10_Mult_10F1(void)
{
    math::Matrix1F10 matrixA(
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f}
    );
    math::Matrix10F1 matrixB(
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f}
    );
    
    math::Matrix1F1 result;
    result.Zero();
    result = matrixA * matrixB;

    /* Verify results */
    UtAssert_True(result[0][0] == 385.0f, "result[0][0] == 385.0f");
}


/* Auto generated with GNU Octave, version 4.0.0 */
void Test_Matrix1F10_Mult_10F10_Random(void)
{
    int i, j = 0;

    math::Matrix1F10 A (
        {-0.789882, 0.615770, 0.293589, 0.563221, -0.856736, 0.560314, -0.386229, -0.331931, -0.550695, 0.165850}
    );

    math::Matrix10F10 B (
        {-0.940722, 0.285908, -0.384376, -0.439918, 0.379773, -0.136546, 0.931149, -0.075142, 0.398986, 0.257576},
        {0.492886, -0.653212, -0.737293, 0.247847, 0.923242, -0.042942, 0.846836, -0.919133, -0.914923, 0.381127},
        {-0.381411, -0.920341, -0.354914, 0.120560, 0.884172, 0.891754, -0.197110, -0.463492, 0.004614, -0.895016},
        {0.815192, 0.083992, -0.090301, -0.160981, 0.539549, -0.405318, 0.633601, -0.814641, -0.683726, 0.013523},
        {0.012939, -0.813884, 0.564346, 0.631373, 0.540073, -0.016430, 0.128309, -0.137069, 0.556163, -0.413168},
        {0.441949, -0.952325, -0.980379, 0.291373, -0.828776, -0.174679, 0.325265, 0.804448, -0.168207, -0.488106},
        {0.624457, 0.313402, -0.912214, -0.459275, 0.536376, 0.615558, -0.266281, -0.134562, -0.972769, 0.948070},
        {0.284442, 0.660300, -0.787400, -0.821518, -0.132664, -0.818274, 0.760943, 0.576408, 0.211016, 0.060354},
        {0.154653, 0.288381, -0.325083, 0.039116, -0.619889, 0.397935, -0.031105, -0.750324, 0.401914, 0.741275},
        {0.110200, -0.889584, 0.858389, -0.969074, 0.742728, -0.514076, -0.773430, -0.175795, 0.601552, 0.115409}
    );

    math::Matrix1F10 expected (
        {1.227777, -1.333841, -0.403195, 0.334978, 0.206344, -0.239397, -0.103608, -0.288656, -1.648897, -0.918716}
    );

    math::Matrix1F10 result;
    result.Zero();

    result = A * B;

    for(i = 0; i < 1; i++)
    {
        for(j = 0; j < 10; j++)
        {
            UtAssert_True(fabs(result[i][j] - expected[i][j]) < 0.00001f, "fabs(result[i][j] - expected[i][j]) < 0.00001f");

        }
    }
}


/* Auto generated with GNU Octave, version 4.0.0 */
void Test_Matrix1F10_Mult_10F1_Random(void)
{
    int i, j = 0;

    math::Matrix1F10 A (
        {-0.621030, -0.579804, 0.028122, -0.254560, 0.445756, 0.992524, 0.284264, -0.660373, -0.307362, 0.442113}
    );

    math::Matrix10F1 B (
        {0.247405},
        {0.737895},
        {0.717530},
        {0.920544},
        {0.193895},
        {-0.914625},
        {0.906238},
        {0.059210},
        {-0.817918},
        {-0.689624}
    );

    math::Matrix1F1 expected (
        -1.451978
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


void Test_Matrix1F10_Transpose(void)
{
    math::Matrix1F10 matrixA(
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f}
    );

    math::Matrix10F1 result;

    result = matrixA.Transpose();

    /* Verify results */
    UtAssert_True(result[0][0] == 1.0f, "result[0][0] == 1.0f");
    UtAssert_True(result[1][0] == 2.0f, "result[1][0] == 2.0f");
    UtAssert_True(result[2][0] == 3.0f, "result[2][0] == 3.0f");
    UtAssert_True(result[3][0] == 4.0f, "result[3][0] == 4.0f");
    UtAssert_True(result[4][0] == 5.0f, "result[4][0] == 5.0f");
    UtAssert_True(result[5][0] == 6.0f, "result[5][0] == 6.0f");
    UtAssert_True(result[6][0] == 7.0f, "result[6][0] == 7.0f");
    UtAssert_True(result[7][0] == 8.0f, "result[7][0] == 8.0f");
    UtAssert_True(result[8][0] == 9.0f, "result[8][0] == 9.0f");
    UtAssert_True(result[9][0] == 10.0f, "result[9][0] == 10.0f");
    UtAssert_True(matrixA[0][0] == 1.0f, "matrixA[0][0] == 1.0f");
    UtAssert_True(matrixA[0][1] == 2.0f, "matrixA[0][1] == 2.0f");
    UtAssert_True(matrixA[0][2] == 3.0f, "matrixA[0][2] == 3.0f");
    UtAssert_True(matrixA[0][3] == 4.0f, "matrixA[0][3] == 4.0f");
    UtAssert_True(matrixA[0][4] == 5.0f, "matrixA[0][4] == 5.0f");
    UtAssert_True(matrixA[0][5] == 6.0f, "matrixA[0][5] == 6.0f");
    UtAssert_True(matrixA[0][6] == 7.0f, "matrixA[0][6] == 7.0f");
    UtAssert_True(matrixA[0][7] == 8.0f, "matrixA[0][7] == 8.0f");
    UtAssert_True(matrixA[0][8] == 9.0f, "matrixA[0][8] == 9.0f");
    UtAssert_True(matrixA[0][9] == 10.0f, "result[0][9] == 10.0f");
}

