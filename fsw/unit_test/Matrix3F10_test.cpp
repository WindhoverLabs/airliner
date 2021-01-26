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

#include "Matrix3F10_test.hpp"
#include <math/Matrix3F10.hpp>


#include "utassert.h"
#include <float.h>

int rows = 3;
int cols = 10;


void Test_Matrix3F10_Constructor(void)
{
    math::Matrix3F10 matrix(
        {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f},
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
    
    UtAssert_True(matrix[2][0] == 1.0f, "matrix[0][1] == 1.0f");
    UtAssert_True(matrix[2][1] == 2.0f, "matrix[1][1] == 2.0f");
    UtAssert_True(matrix[2][2] == 3.0f, "matrix[2][1] == 3.0f");
    UtAssert_True(matrix[2][3] == 4.0f, "matrix[3][1] == 4.0f");
    UtAssert_True(matrix[2][4] == 5.0f, "matrix[4][1] == 5.0f");
    UtAssert_True(matrix[2][5] == 6.0f, "matrix[5][1] == 6.0f");
    UtAssert_True(matrix[2][6] == 7.0f, "matrix[6][1] == 7.0f");
    UtAssert_True(matrix[2][7] == 8.0f, "matrix[7][1] == 8.0f");
    UtAssert_True(matrix[2][8] == 9.0f, "matrix[8][1] == 9.0f");
    UtAssert_True(matrix[2][9] == 10.0f, "matrix[9][1] == 10.0f");
}


void Test_Matrix3F10_OutOfBounds(void)
{
    math::Matrix3F10 matrix(
        {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f},
        {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f},
        {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f}
    );


    /* Verify results */
    UtAssert_True(!isfinite(matrix[0][10]), "matrix[0][10] == NAN");
    UtAssert_True(!isfinite(matrix[4][0]), "matrix[4][0] == NAN");
}

void Test_Matrix3F10_Transpose(void)
{
    math::Matrix3F10 matrix(
        {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f},
        {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f},
        {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f}
    );

    math::Matrix10F3 matrixT(
        {1.0f, 1.0f, 1.0f},
        {2.0f, 2.0f, 2.0f},
        {3.0f, 3.0f, 3.0f},
        {4.0f, 4.0f, 4.0f},
        {5.0f, 5.0f, 5.0f},
        {6.0f, 6.0f, 6.0f},
        {7.0f, 7.0f, 7.0f},
        {8.0f, 8.0f, 8.0f},
        {9.0f, 9.0f, 9.0f},
        {10.0f, 10.0f, 10.0f}
    );

    math::Matrix10F3 result;
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


void Test_Matrix3F10_Zero(void)
{
    math::Matrix3F10 matrix(
        {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f},
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

void Test_Matrix3F10_Mult_10F(void)
{
    math::Matrix3F10 matrixA(
        {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f},
        {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f},
        {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f}
    );

    math::Vector10F vectorB(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f);

    math::Vector3F result;
    result.Zero();

    result = matrixA * vectorB;

    UtAssert_True(result[0] == 385.0, "result[0] == 385.0");
    UtAssert_True(result[1] == 385.0, "result[1] == 385.0");
    UtAssert_True(result[2] == 385.0, "result[2] == 385.0");
}


/* Auto generated with GNU Octave, version 4.0.0 */
void Test_Matrix3F10_Mult_10F_Random(void)
{
    int i, j = 0;

    math::Matrix3F10 A (
        {-0.380259, 0.253242, -0.647132, 0.977245, 0.201284, -0.157566, -0.237407, 0.007855, -0.595675, -0.738428},
        {0.518025, -0.856095, 0.941989, 0.257400, -0.456754, 0.823891, -0.835851, 0.771125, -0.851280, 0.973450},
        {0.327336, 0.450101, 0.774594, 0.526969, 0.776630, -0.487822, -0.414001, -0.943328, -0.511091, -0.504533}
    );

    math::Vector10F B (
        {0.737466,-0.599147,0.171933,0.755590,-0.987586,0.721055,-0.358837,0.026422,0.832785,0.735170}
    );

    math::Vector3F expected (
        {-1.070964,2.623582,-1.288574}
    );

    math::Vector3F result;
    result.Zero();

    result = A * B;

    for(i = 0; i < 3; i++)
    {
        UtAssert_True(fabs(result[i] - expected[i]) < 0.00001f, "fabs(result[i] - expected[i]) < 0.00001f");

    }
}


void Test_Matrix3F10_Mult_10F3(void)
{
    math::Matrix3F10 matrixA(
        {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f},
        {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f},
        {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f}
    );

    math::Matrix10F3 matrixB(
        {1.0f, 2.0f, 3.0f},
        {1.0f, 2.0f, 3.0f},
        {1.0f, 2.0f, 3.0f},
        {1.0f, 2.0f, 3.0f},
        {1.0f, 2.0f, 3.0f},
        {1.0f, 2.0f, 3.0f},
        {1.0f, 2.0f, 3.0f},
        {1.0f, 2.0f, 3.0f},
        {1.0f, 2.0f, 3.0f},
        {1.0f, 2.0f, 3.0f}
    );

    math::Matrix3F3 result;
    result.Zero();

    result = matrixA * matrixB;

    /* Verify results */
    for(int i = 0; i < rows; ++i)
    {
        UtAssert_True(result[i][0] == 55.0f,  "result[i][0] == 55.0f");
        UtAssert_True(result[i][1] == 110.0f, "result[i][1] == 110.0f");
        UtAssert_True(result[i][2] == 165.0f, "result[i][2] == 165.0f");
    }
}


/* Auto generated with GNU Octave, version 4.0.0 */
void Test_Matrix3F10_Mult_10F3_Random(void)
{
    int i, j = 0;

    math::Matrix3F10 A (
        {-0.080514, 0.824841, -0.024022, -0.933492, -0.088014, -0.132499, -0.743131, -0.890025, -0.106517, 0.906010},
        {-0.428114, -0.272991, -0.385843, -0.281124, 0.808031, 0.467739, -0.680103, 0.813567, 0.353887, 0.623899},
        {0.173305, -0.086170, -0.549709, 0.050011, -0.468429, -0.707820, 0.392682, -0.130397, -0.217442, 0.420806}
    );

    math::Matrix10F3 B (
        {0.674206, 0.622307, 0.451231},
        {0.956792, -0.675385, 0.081729},
        {-0.008309, -0.711852, 0.364420},
        {-0.625710, 0.369897, -0.699420},
        {0.995313, -0.907356, -0.097402},
        {0.966556, -0.085779, 0.733219},
        {0.377784, 0.461953, -0.507252},
        {0.443980, 0.633768, 0.873275},
        {-0.333241, 0.629287, 0.901204},
        {0.475428, -0.752854, 0.727990}
    );

    math::Matrix3F3 expected (
        {0.893886, -2.500644, 0.749945},
        {1.168579, -0.730232, 1.933343},
        {-0.779731, 0.706725, -0.840185}
    );

    math::Matrix3F3 result;
    result.Zero();

    result = A * B;

    for(i = 0; i < 3; i++)
    {
        for(j = 0; j < 3; j++)
        {
            UtAssert_True(fabs(result[i][j] - expected[i][j]) < 0.00001f, "fabs(result[i][j] - expected[i][j]) < 0.00001f");

        }
    }
}
