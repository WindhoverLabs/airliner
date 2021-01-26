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

#include "Matrix10F2_test.hpp"
#include <math/Matrix10F2.hpp>


#include "utassert.h"
#include <float.h>


void Test_Matrix10F2_Constructor(void)
{
    math::Matrix10F2 matrix(
            {1.0f, 2.0f},
            {1.0f, 2.0f},
            {1.0f, 2.0f},
            {1.0f, 2.0f},
            {1.0f, 2.0f},
            {1.0f, 2.0f},
            {1.0f, 2.0f},
            {1.0f, 2.0f},
            {1.0f, 2.0f},
            {1.0f, 2.0f}
    );

    /* Verify results */
    UtAssert_True(matrix[0][0] == 1.0f, "matrix[0][0] == 1.0f");
    UtAssert_True(matrix[1][0] == 1.0f, "matrix[1][0] == 1.0f");
    UtAssert_True(matrix[2][0] == 1.0f, "matrix[2][0] == 1.0f");
    UtAssert_True(matrix[3][0] == 1.0f, "matrix[3][0] == 1.0f");
    UtAssert_True(matrix[4][0] == 1.0f, "matrix[4][0] == 1.0f");
    UtAssert_True(matrix[5][0] == 1.0f, "matrix[5][0] == 1.0f");
    UtAssert_True(matrix[6][0] == 1.0f, "matrix[6][0] == 1.0f");
    UtAssert_True(matrix[7][0] == 1.0f, "matrix[7][0] == 1.0f");
    UtAssert_True(matrix[8][0] == 1.0f, "matrix[8][0] == 1.0f");
    UtAssert_True(matrix[9][0] == 1.0f, "matrix[9][0] == 1.0f");

    UtAssert_True(matrix[0][1] == 2.0f, "matrix[0][1] == 2.0f");
    UtAssert_True(matrix[1][1] == 2.0f, "matrix[1][1] == 2.0f");
    UtAssert_True(matrix[2][1] == 2.0f, "matrix[2][1] == 2.0f");
    UtAssert_True(matrix[3][1] == 2.0f, "matrix[3][1] == 2.0f");
    UtAssert_True(matrix[4][1] == 2.0f, "matrix[4][1] == 2.0f");
    UtAssert_True(matrix[5][1] == 2.0f, "matrix[5][1] == 2.0f");
    UtAssert_True(matrix[6][1] == 2.0f, "matrix[6][1] == 2.0f");
    UtAssert_True(matrix[7][1] == 2.0f, "matrix[7][1] == 2.0f");
    UtAssert_True(matrix[8][1] == 2.0f, "matrix[8][1] == 2.0f");
    UtAssert_True(matrix[9][1] == 2.0f, "matrix[9][1] == 2.0f");

}


void Test_Matrix10F2_OutOfBounds(void)
{
    math::Matrix10F2 matrix(
            {1.0f, 2.0f},
            {1.0f, 2.0f},
            {1.0f, 2.0f},
            {1.0f, 2.0f},
            {1.0f, 2.0f},
            {1.0f, 2.0f},
            {1.0f, 2.0f},
            {1.0f, 2.0f},
            {1.0f, 2.0f},
            {1.0f, 2.0f}
    );

    /* Verify results */
    UtAssert_True(!isfinite(matrix[0][3]), "matrix[0][6] == NAN");
    UtAssert_True(!isfinite(matrix[10][0]), "matrix[10][0] == NAN");
}


void Test_Matrix10F2_Identity(void)
{
    math::Matrix10F2 matrix(
        {1.0f, 0.0f},
        {0.0f, 1.0f},
        {0.0f, 0.0f},
        {0.0f, 0.0f},
        {0.0f, 0.0f},
        {0.0f, 0.0f},
        {0.0f, 0.0f},
        {0.0f, 0.0f},
        {0.0f, 0.0f},
        {0.0f, 0.0f}
    );

    /* Verify results */
    for(int i = 0; i < 10; ++i)
    {
        for(int j = 0; j < 2; ++j)
        {
            if(i == j)
            {
                UtAssert_True(1.0 == matrix[i][j], "matrix diagnol == 1.0");
            }
            else
            {
                UtAssert_True(0.0 == matrix[i][j], "matrix non diag == 0.0");
            }
        }
    }
}


void Test_Matrix10F2_Zero(void)
{
    math::Matrix10F2 matrix(
        {1.0f, 2.0f},
        {1.0f, 2.0f},
        {1.0f, 2.0f},
        {1.0f, 2.0f},
        {1.0f, 2.0f},
        {1.0f, 2.0f},
        {1.0f, 2.0f},
        {1.0f, 2.0f},
        {1.0f, 2.0f},
        {1.0f, 2.0f}
    );

    matrix.Zero();

    /* Verify results */
    for(int i = 0; i < 10; ++i)
    {
        for(int j = 0; j < 2; ++j)
        {
            UtAssert_True(0.0 == matrix[i][j], "matrix idx == 0.0");
        }
    }
}


void Test_Matrix10F2_Mult_2F(void)
{
    math::Matrix10F2 matrixA(
        {1.0f, 2.0f},
        {1.0f, 2.0f},
        {1.0f, 2.0f},
        {1.0f, 2.0f},
        {1.0f, 2.0f},
        {1.0f, 2.0f},
        {1.0f, 2.0f},
        {1.0f, 2.0f},
        {1.0f, 2.0f},
        {1.0f, 2.0f}
    );

    math::Vector2F vectorB(1.0f, 2.0f);

    math::Vector10F result;
    result.Zero();

    result = matrixA * vectorB;

    UtAssert_True(result[0] == 5.0f, "result[0] == 5.0f");
    UtAssert_True(result[1] == 5.0f, "result[1] == 5.0f");
    UtAssert_True(result[2] == 5.0f, "result[2] == 5.0f");
    UtAssert_True(result[3] == 5.0f, "result[3] == 5.0f");
    UtAssert_True(result[4] == 5.0f, "result[4] == 5.0f");
    UtAssert_True(result[5] == 5.0f, "result[5] == 5.0f");
    UtAssert_True(result[6] == 5.0f, "result[6] == 5.0f");
    UtAssert_True(result[7] == 5.0f, "result[7] == 5.0f");
    UtAssert_True(result[8] == 5.0f, "result[8] == 5.0f");
    UtAssert_True(result[9] == 5.0f, "result[9] == 5.0f");
}


/* Auto generated with GNU Octave, version 4.0.0 */
void Test_Matrix10F2_Mult_2F_Random(void)
{
    int i, j = 0;

    math::Matrix10F2 A (
        {-0.177535, -0.122088},
        {-0.197694, 0.046381},
        {-0.356339, 0.315887},
        {-0.927795, -0.077766},
        {-0.684193, 0.481219},
        {-0.946582, -0.597486},
        {-0.144562, -0.373043},
        {-0.999740, 0.368892},
        {-0.385848, -0.183620},
        {0.436720, -0.729276}
    );

    math::Vector2F B (
        {0.194859,-0.054030}
    );

    math::Vector10F expected (
        {-0.027998,-0.041029,-0.086503,-0.176588,-0.159322,-0.152168,-0.008014,-0.214740,-0.065265,0.124502}
    );

    math::Vector10F result;
    result.Zero();

    result = A * B;

    for(i = 0; i < 10; i++)
    {
        UtAssert_True(fabs(result[i] - expected[i]) < 0.00001f, "fabs(result[i] - expected[i]) < 0.00001f");

    }
}


void Test_Matrix10F2_Mult_2F2(void)
{
    math::Matrix10F2 matrixA(
        {1.0f, 2.0f},
        {1.0f, 2.0f},
        {1.0f, 2.0f},
        {1.0f, 2.0f},
        {1.0f, 2.0f},
        {1.0f, 2.0f},
        {1.0f, 2.0f},
        {1.0f, 2.0f},
        {1.0f, 2.0f},
        {1.0f, 2.0f}
    );

    math::Matrix2F2 matrixB(
        {1.0f, 2.0f},
        {1.0f, 2.0f}
    );

    math::Matrix10F2 result;
    result.Zero();

    result = matrixA * matrixB;

    /* Verify results */
    for(int i = 0; i < 10; ++i)
    {
        UtAssert_True(result[i][0] == 3.0f,  "result[i][0] == 3.0f");
        UtAssert_True(result[i][1] == 6.0f, "result[i][1] == 6.0f");
    }
}


/* Auto generated with GNU Octave, version 4.0.0 */
void Test_Matrix10F2_Mult_2F2_Random(void)
{
    int i, j = 0;

    math::Matrix10F2 A (
        {0.958232, -0.353940},
        {-0.637760, -0.161997},
        {0.791130, 0.232317},
        {0.038803, 0.427547},
        {0.473376, -0.960793},
        {0.370533, -0.928903},
        {-0.497278, -0.329334},
        {-0.824696, 0.926832},
        {0.436397, -0.064773},
        {-0.342190, -0.343577}
    );

    math::Matrix2F2 B (
        {-0.076646, 0.445702},
        {0.492726, -0.436357}
    );

    math::Matrix10F2 expected (
        {-0.247840, 0.581530},
        {-0.030938, -0.213562},
        {0.053832, 0.251235},
        {0.207690, -0.169269},
        {-0.509690, 0.630233},
        {-0.486094, 0.570480},
        {-0.124157, -0.077931},
        {0.519883, -0.771998},
        {-0.065363, 0.222767},
        {-0.143062, -0.002593}
    );

    math::Matrix10F2 result;
    result.Zero();

    result = A * B;

    for(i = 0; i < 10; i++)
    {
        for(j = 0; j < 2; j++)
        {
            UtAssert_True(fabs(result[i][j] - expected[i][j]) < 0.00001f, "fabs(result[i][j] - expected[i][j]) < 0.00001f");

        }
    }
}


void Test_Matrix10F2_Mult_2F10(void)
{
    math::Matrix10F2 matrixA(
        {1.0f, 2.0f},
        {1.0f, 2.0f},
        {1.0f, 2.0f},
        {1.0f, 2.0f},
        {1.0f, 2.0f},
        {1.0f, 2.0f},
        {1.0f, 2.0f},
        {1.0f, 2.0f},
        {1.0f, 2.0f},
        {1.0f, 2.0f}
    );

    math::Matrix2F10 matrixB(
        {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f},
        {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f}
    );

    math::Matrix10F10 result;
    result.Zero();

    result = matrixA * matrixB;

    /* Verify results */
    for(int i = 0; i < 10; ++i)
    {
        UtAssert_True(result[i][0] == 3.0f,  "result[i][0] == 3.0f");
        UtAssert_True(result[i][1] == 6.0f, "result[i][1] == 6.0f");
        UtAssert_True(result[i][2] == 9.0f, "result[i][2] == 9.0f");
        UtAssert_True(result[i][3] == 12.0f, "result[i][3] == 12.0f");
        UtAssert_True(result[i][4] == 15.0f, "result[i][4] == 15.0f");
        UtAssert_True(result[i][5] == 18.0f, "result[i][5] == 18.0f");
        UtAssert_True(result[i][6] == 21.0f, "result[i][6] == 21.0f");
        UtAssert_True(result[i][7] == 24.0f, "result[i][7] == 24.0f");
        UtAssert_True(result[i][8] == 27.0f, "result[i][8] == 27.0f");
        UtAssert_True(result[i][9] == 30.0f, "result[i][9] == 30.0f");
    }
}


/* Auto generated with GNU Octave, version 4.0.0 */
void Test_Matrix10F2_Mult_2F10_Random(void)
{
    int i, j = 0;

    math::Matrix10F2 A (
        {0.296138, -0.033921},
        {0.995268, 0.672015},
        {-0.827637, -0.121672},
        {0.603040, 0.058004},
        {0.047949, -0.718685},
        {0.606029, -0.656754},
        {-0.554683, -0.448961},
        {0.791282, -0.916950},
        {-0.963857, 0.781365},
        {-0.763308, -0.299853}
    );

    math::Matrix2F10 B (
        {-0.878910, 0.134400, 0.879692, 0.783023, -0.243432, -0.986075, 0.625898, 0.739394, -0.818416, -0.829002},
        {-0.397717, 0.378846, 0.994759, 0.759892, 0.394848, -0.138801, -0.849503, -0.864000, -0.941999, 0.720487}
    );

    math::Matrix10F10 expected (
        {-0.246787, 0.026950, 0.226767, 0.206106, -0.085483, -0.287306, 0.214168, 0.248270, -0.210410, -0.269938},
        {-1.142023, 0.388355, 1.544023, 1.289977, 0.023064, -1.074686, 0.052058, 0.155275, -1.447581, -0.340902},
        {0.775810, -0.157329, -0.849100, -0.740516, 0.153432, 0.833001, -0.414656, -0.506826, 0.791966, 0.598451},
        {-0.553087, 0.103023, 0.588189, 0.516271, -0.123897, -0.602694, 0.328167, 0.395769, -0.548177, -0.458131},
        {0.243690, -0.265827, -0.672738, -0.508578, -0.295444, 0.052473, 0.640536, 0.656397, 0.637759, -0.557552},
        {-0.271443, -0.167358, -0.120193, -0.024528, -0.406845, -0.506432, 0.937227, 1.015530, 0.122678, -0.975582},
        {0.666076, -0.244637, -0.934559, -0.775492, -0.042244, 0.609276, 0.034219, -0.022227, 0.876883, 0.136363},
        {-0.330779, -0.241035, -0.216060, -0.077192, -0.554680, -0.652990, 1.274214, 1.377314, 0.216169, -1.316625},
        {0.536382, 0.166475, -0.070627, -0.160968, 0.543155, 0.841981, -1.267048, -1.387770, 0.052790, 1.362003},
        {0.790136, -0.216187, -0.969758, -0.825544, 0.067417, 0.794299, -0.223027, -0.305313, 0.907165, 0.416744}
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


void Test_Matrix10F2_Mult_Scaler(void)
{
    math::Matrix10F2 matrixA(
            {1.0f, 2.0f},
            {1.0f, 2.0f},
            {1.0f, 2.0f},
            {1.0f, 2.0f},
            {1.0f, 2.0f},
            {1.0f, 2.0f},
            {1.0f, 2.0f},
            {1.0f, 2.0f},
            {1.0f, 2.0f},
            {1.0f, 2.0f}
    );
    
    math::Matrix10F2 result;
    result = matrixA * 2.0f;

    /* Verify results */
    UtAssert_True(result[0][0] == 2.0f, "result[0][0] == 2.0f");
    UtAssert_True(result[1][0] == 2.0f, "result[1][0] == 2.0f");
    UtAssert_True(result[2][0] == 2.0f, "result[2][0] == 2.0f");
    UtAssert_True(result[3][0] == 2.0f, "result[3][0] == 2.0f");
    UtAssert_True(result[4][0] == 2.0f, "result[4][0] == 2.0f");
    UtAssert_True(result[5][0] == 2.0f, "result[5][0] == 2.0f");
    UtAssert_True(result[6][0] == 2.0f, "result[6][0] == 2.0f");
    UtAssert_True(result[7][0] == 2.0f, "result[7][0] == 2.0f");
    UtAssert_True(result[8][0] == 2.0f, "result[8][0] == 2.0f");
    UtAssert_True(result[9][0] == 2.0f, "result[9][0] == 2.0f");

    UtAssert_True(result[0][1] == 4.0f, "result[0][1] == 4.0f");
    UtAssert_True(result[1][1] == 4.0f, "result[1][1] == 4.0f");
    UtAssert_True(result[2][1] == 4.0f, "result[2][1] == 4.0f");
    UtAssert_True(result[3][1] == 4.0f, "result[3][1] == 4.0f");
    UtAssert_True(result[4][1] == 4.0f, "result[4][1] == 4.0f");
    UtAssert_True(result[5][1] == 4.0f, "result[5][1] == 4.0f");
    UtAssert_True(result[6][1] == 4.0f, "result[6][1] == 4.0f");
    UtAssert_True(result[7][1] == 4.0f, "result[7][1] == 4.0f");
    UtAssert_True(result[8][1] == 4.0f, "result[8][1] == 4.0f");
    UtAssert_True(result[9][1] == 4.0f, "result[9][1] == 4.0f");

}


void Test_Matrix10F2_Transpose(void)
{
    math::Matrix10F2 matrix(
            {1.0f, 2.0f},
            {1.0f, 2.0f},
            {1.0f, 2.0f},
            {1.0f, 2.0f},
            {1.0f, 2.0f},
            {1.0f, 2.0f},
            {1.0f, 2.0f},
            {1.0f, 2.0f},
            {1.0f, 2.0f},
            {1.0f, 2.0f}
    );

    math::Matrix2F10 result = matrix.Transpose();

    /* Verify results */
    UtAssert_True(result[0][0] == 1.0f, "result[0][0] == 1.0f");
    UtAssert_True(result[0][1] == 1.0f, "result[0][1] == 1.0f");
    UtAssert_True(result[0][2] == 1.0f, "result[0][2] == 1.0f");
    UtAssert_True(result[0][3] == 1.0f, "result[0][3] == 1.0f");
    UtAssert_True(result[0][4] == 1.0f, "result[0][4] == 1.0f");
    UtAssert_True(result[0][5] == 1.0f, "result[0][5] == 1.0f");
    UtAssert_True(result[0][6] == 1.0f, "result[0][6] == 1.0f");
    UtAssert_True(result[0][7] == 1.0f, "result[0][7] == 1.0f");
    UtAssert_True(result[0][8] == 1.0f, "result[0][8] == 1.0f");
    UtAssert_True(result[0][9] == 1.0f, "result[0][9] == 1.0f");

    UtAssert_True(result[1][0] == 2.0f, "result[1][0] == 2.0f");
    UtAssert_True(result[1][1] == 2.0f, "result[1][1] == 2.0f");
    UtAssert_True(result[1][2] == 2.0f, "result[1][2] == 2.0f");
    UtAssert_True(result[1][3] == 2.0f, "result[1][3] == 2.0f");
    UtAssert_True(result[1][4] == 2.0f, "result[1][4] == 2.0f");
    UtAssert_True(result[1][5] == 2.0f, "result[1][5] == 2.0f");
    UtAssert_True(result[1][6] == 2.0f, "result[1][6] == 2.0f");
    UtAssert_True(result[1][7] == 2.0f, "result[1][7] == 2.0f");
    UtAssert_True(result[1][8] == 2.0f, "result[1][8] == 2.0f");
    UtAssert_True(result[1][9] == 2.0f, "result[1][9] == 2.0f");


}


void Test_Matrix10F2_Addition(void)
{
    math::Matrix10F2 matrixA(
            {1.0f, 2.0f},
            {1.0f, 2.0f},
            {1.0f, 2.0f},
            {1.0f, 2.0f},
            {1.0f, 2.0f},
            {1.0f, 2.0f},
            {1.0f, 2.0f},
            {1.0f, 2.0f},
            {1.0f, 2.0f},
            {1.0f, 2.0f}
    );

    math::Matrix10F2 matrixB(
            {1.0f, 2.0f},
            {1.0f, 2.0f},
            {1.0f, 2.0f},
            {1.0f, 2.0f},
            {1.0f, 2.0f},
            {1.0f, 2.0f},
            {1.0f, 2.0f},
            {1.0f, 2.0f},
            {1.0f, 2.0f},
            {1.0f, 2.0f}
    );
    math::Matrix10F2 result = matrixA + matrixB;

    /* Verify results */
    UtAssert_True(result[0][0] == 2.0f, "result[0][0] == 2.0f");
    UtAssert_True(result[1][0] == 2.0f, "result[1][0] == 2.0f");
    UtAssert_True(result[2][0] == 2.0f, "result[2][0] == 2.0f");
    UtAssert_True(result[3][0] == 2.0f, "result[3][0] == 2.0f");
    UtAssert_True(result[4][0] == 2.0f, "result[4][0] == 2.0f");
    UtAssert_True(result[5][0] == 2.0f, "result[5][0] == 2.0f");
    UtAssert_True(result[6][0] == 2.0f, "result[6][0] == 2.0f");
    UtAssert_True(result[7][0] == 2.0f, "result[7][0] == 2.0f");
    UtAssert_True(result[8][0] == 2.0f, "result[8][0] == 2.0f");
    UtAssert_True(result[9][0] == 2.0f, "result[9][0] == 2.0f");
    
    UtAssert_True(result[0][1] == 4.0f, "result[0][1] == 4.0f");
    UtAssert_True(result[1][1] == 4.0f, "result[1][1] == 4.0f");
    UtAssert_True(result[2][1] == 4.0f, "result[2][1] == 4.0f");
    UtAssert_True(result[3][1] == 4.0f, "result[3][1] == 4.0f");
    UtAssert_True(result[4][1] == 4.0f, "result[4][1] == 4.0f");
    UtAssert_True(result[5][1] == 4.0f, "result[5][1] == 4.0f");
    UtAssert_True(result[6][1] == 4.0f, "result[6][1] == 4.0f");
    UtAssert_True(result[7][1] == 4.0f, "result[7][1] == 4.0f");
    UtAssert_True(result[8][1] == 4.0f, "result[8][1] == 4.0f");
    UtAssert_True(result[9][1] == 4.0f, "result[9][1] == 4.0f");

}
