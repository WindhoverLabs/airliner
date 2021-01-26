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

#include "Matrix2F10_test.hpp"
#include <math/Matrix2F10.hpp>


#include "utassert.h"
#include <float.h>

int rows = 2;
int cols = 10;


void Test_Matrix2F10_Constructor(void)
{
    math::Matrix2F10 matrix(
        {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f},
        {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f}
    );

    /* Verify results */
    UtAssert_True(matrix[0][0] == 1.0f, "matrix[0][0] == 1.0f");
    UtAssert_True(matrix[0][1] == 2.0f, "matrix[1][0] == 2.0f");
    UtAssert_True(matrix[0][2] == 3.0f, "matrix[2][0] == 3.0f");
    UtAssert_True(matrix[0][3] == 4.0f, "matrix[3][0] == 4.0f");
    UtAssert_True(matrix[0][4] == 5.0f, "matrix[4][0] == 5.0f");
    UtAssert_True(matrix[0][5] == 6.0f, "matrix[5][0] == 6.0f");
    UtAssert_True(matrix[0][6] == 7.0f, "matrix[6][0] == 7.0f");
    UtAssert_True(matrix[0][7] == 8.0f, "matrix[7][0] == 8.0f");
    UtAssert_True(matrix[0][8] == 9.0f, "matrix[8][0] == 9.0f");
    UtAssert_True(matrix[0][9] == 10.0f, "matrix[9][0] == 10.0f");
    
    UtAssert_True(matrix[1][0] == 1.0f, "matrix[0][1] == 1.0f");
    UtAssert_True(matrix[1][1] == 2.0f, "matrix[1][1] == 2.0f");
    UtAssert_True(matrix[1][2] == 3.0f, "matrix[2][1] == 3.0f");
    UtAssert_True(matrix[1][3] == 4.0f, "matrix[3][1] == 4.0f");
    UtAssert_True(matrix[1][4] == 5.0f, "matrix[4][1] == 5.0f");
    UtAssert_True(matrix[1][5] == 6.0f, "matrix[5][1] == 6.0f");
    UtAssert_True(matrix[1][6] == 7.0f, "matrix[6][1] == 7.0f");
    UtAssert_True(matrix[1][7] == 8.0f, "matrix[7][1] == 8.0f");
    UtAssert_True(matrix[1][8] == 9.0f, "matrix[8][1] == 9.0f");
    UtAssert_True(matrix[1][9] == 10.0f, "matrix[9][1] == 10.0f");
    
}


void Test_Matrix2F10_OutOfBounds(void)
{
    math::Matrix2F10 matrix(
        {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f},
        {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f}
    );


    /* Verify results */
    UtAssert_True(!isfinite(matrix[0][10]), "matrix[0][10] == NAN");
    UtAssert_True(!isfinite(matrix[2][0]), "matrix[2][0] == NAN");
}

void Test_Matrix2F10_Transpose(void)
{
    math::Matrix2F10 matrix(
        {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f},
        {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f}
    );

    math::Matrix10F2 matrixT(
        {1.0f, 1.0f},
        {2.0f, 2.0f},
        {3.0f, 3.0f},
        {4.0f, 4.0f},
        {5.0f, 5.0f},
        {6.0f, 6.0f},
        {7.0f, 7.0f},
        {8.0f, 8.0f},
        {9.0f, 9.0f},
        {10.0f, 10.0f}
    );

    math::Matrix10F2 result;
    result.Zero();

    result = matrix.Transpose();

    /* Verify results */
    for(int i = 0; i < cols; ++i)
    {
        for(int j = 0; j < rows; ++j)
        {
            UtAssert_True(result[i][j] == matrixT[i][j], "matrix transpose correct");
        }
    }
}


void Test_Matrix2F10_Zero(void)
{
    math::Matrix2F10 matrix(
        {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f},
        {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f}
    );

    matrix.Zero();

    /* Verify results */
    for(int i = 0; i < rows; ++i)
    {
        for(int j = 0; j < cols; ++j)
        {
            UtAssert_True(0.0 == matrix[i][j], "matrix idx == 0.0");
        }
    }
}

void Test_Matrix2F10_Mult_10F(void)
{
    math::Matrix2F10 matrixA(
        {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f},
        {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f}
    );

    math::Vector10F vectorB(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f);

    math::Vector2F result;
    result.Zero();

    result = matrixA * vectorB;

    UtAssert_True(result[0] == 385.0, "result[0] == 385.0");
    UtAssert_True(result[1] == 385.0, "result[1] == 385.0");
}


/* Auto generated with GNU Octave, version 4.0.0 */
void Test_Matrix2F10_Mult_10F_Random(void)
{
    int i, j = 0;

    math::Matrix2F10 A (
        {0.040082, 0.956354, -0.140479, -0.295712, 0.500550, -0.045940, -0.210247, 0.497643, 0.537541, -0.112452},
        {0.337030, 0.620136, -0.049539, -0.553251, -0.819240, 0.817782, 0.272904, -0.642462, 0.071825, -0.923481}
    );

    math::Vector10F B (
        {-0.376146,0.142280,-0.355838,-0.648908,0.046917,0.378122,0.699633,-0.868630,-0.612864,0.840203}
    );

    math::Vector2F expected (
        {-0.634301,0.537946}
    );

    math::Vector2F result;
    result.Zero();

    result = A * B;

    for(i = 0; i < 2; i++)
    {
        UtAssert_True(fabs(result[i] - expected[i]) < 0.00001f, "fabs(result[i] - expected[i]) < 0.00001f");

    }
}


/* Auto generated with GNU Octave, version 4.0.0 */
void Test_Matrix2F10_Mult_10F2_Random(void)
{
    int i, j = 0;

    math::Matrix2F10 A (
        {0.896450, 0.826114, -0.548676, 0.299170, -0.737607, -0.324979, -0.294995, 0.713616, -0.220724, 0.502837},
        {0.236862, -0.963378, 0.900362, 0.027987, 0.811111, -0.913326, -0.402787, -0.010654, 0.942687, 0.983234}
    );

    math::Matrix10F2 B (
        {0.494147, -0.631239},
        {-0.384315, 0.937477},
        {-0.627682, -0.873189},
        {-0.741107, -0.258663},
        {0.292748, -0.560963},
        {0.174713, -0.683653},
        {0.104133, 0.270202},
        {0.514955, -0.020963},
        {-0.037407, 0.936168},
        {-0.796898, 0.454313}
    );

    math::Matrix2F2 expected (
        {-0.080235, 1.173388},
        {-0.886947, -0.456092}
    );

    math::Matrix2F2 result;
    result.Zero();

    result = A * B;

    for(i = 0; i < 2; i++)
    {
        for(j = 0; j < 2; j++)
        {
            UtAssert_True(fabs(result[i][j] - expected[i][j]) < 0.00001f, "fabs(result[i][j] - expected[i][j]) < 0.00001f");

        }
    }
}
