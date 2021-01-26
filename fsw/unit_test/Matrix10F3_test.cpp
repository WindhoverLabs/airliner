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

#include "Matrix10F3_test.hpp"
#include <math/Matrix10F3.hpp>


#include "utassert.h"
#include <float.h>


void Test_Matrix10F3_Constructor(void)
{
    math::Matrix10F3 matrix(
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

    UtAssert_True(matrix[0][2] == 3.0f, "matrix[0][2] == 3.0f");
    UtAssert_True(matrix[1][2] == 3.0f, "matrix[1][2] == 3.0f");
    UtAssert_True(matrix[2][2] == 3.0f, "matrix[2][2] == 3.0f");
    UtAssert_True(matrix[3][2] == 3.0f, "matrix[3][2] == 3.0f");
    UtAssert_True(matrix[4][2] == 3.0f, "matrix[4][2] == 3.0f");
    UtAssert_True(matrix[5][2] == 3.0f, "matrix[5][2] == 3.0f");
    UtAssert_True(matrix[6][2] == 3.0f, "matrix[6][2] == 3.0f");
    UtAssert_True(matrix[7][2] == 3.0f, "matrix[7][2] == 3.0f");
    UtAssert_True(matrix[8][2] == 3.0f, "matrix[8][2] == 3.0f");
    UtAssert_True(matrix[9][2] == 3.0f, "matrix[9][2] == 3.0f");
}


void Test_Matrix10F3_OutOfBounds(void)
{
    math::Matrix10F3 matrix(
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

    /* Verify results */
    UtAssert_True(!isfinite(matrix[0][3]), "matrix[0][6] == NAN");
    UtAssert_True(!isfinite(matrix[10][0]), "matrix[10][0] == NAN");
}


void Test_Matrix10F3_Identity(void)
{
    math::Matrix10F3 matrix(
        {1.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.0f}
    );

    /* Verify results */
    for(int i = 0; i < 10; ++i)
    {
        for(int j = 0; j < 3; ++j)
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


void Test_Matrix10F3_Zero(void)
{
    math::Matrix10F3 matrix(
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

    matrix.Zero();

    /* Verify results */
    for(int i = 0; i < 10; ++i)
    {
        for(int j = 0; j < 3; ++j)
        {
            UtAssert_True(0.0 == matrix[i][j], "matrix idx == 0.0");
        }
    }
}


void Test_Matrix10F3_Mult_3F(void)
{
    math::Matrix10F3 matrixA(
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

    math::Vector3F vectorB(1.0f, 2.0f, 3.0f);

    math::Vector10F result;
    result.Zero();

    result = matrixA * vectorB;

    UtAssert_True(result[0] == 14.0, "result[0] == 14.0");
    UtAssert_True(result[1] == 14.0, "result[1] == 14.0");
    UtAssert_True(result[2] == 14.0, "result[2] == 14.0");
    UtAssert_True(result[3] == 14.0, "result[3] == 14.0");
    UtAssert_True(result[4] == 14.0, "result[4] == 14.0");
    UtAssert_True(result[5] == 14.0, "result[5] == 14.0");
    UtAssert_True(result[6] == 14.0, "result[6] == 14.0");
    UtAssert_True(result[7] == 14.0, "result[7] == 14.0");
    UtAssert_True(result[8] == 14.0, "result[8] == 14.0");
    UtAssert_True(result[9] == 14.0, "result[9] == 14.0");
}


/* Auto generated with GNU Octave, version 4.0.0 */
void Test_Matrix10F3_Mult_3F_Random(void)
{
    int i, j = 0;

    math::Matrix10F3 A (
        {-0.336815, -0.290997, -0.932649},
        {0.950759, -0.085893, -0.762864},
        {0.893356, 0.128010, -0.803978},
        {0.332622, 0.615603, 0.743372},
        {-0.621030, -0.999335, 0.351932},
        {0.369825, 0.126158, 0.248734},
        {0.296540, 0.981548, -0.281587},
        {-0.793459, -0.233476, 0.502224},
        {-0.346314, -0.627155, 0.723922},
        {0.300714, 0.701482, -0.552074}
    );

    math::Vector3F B (
        {-0.439459,0.840317,0.903057}
    );

    math::Vector10F expected (
        {-0.938748,-1.178906,-1.011062,1.042435,-0.249026,0.168110,0.440206,0.606035,0.278924,-0.041238}
    );

    math::Vector10F result;
    result.Zero();

    result = A * B;

    for(i = 0; i < 10; i++)
    {
        UtAssert_True(fabs(result[i] - expected[i]) < 0.00001f, "fabs(result[i] - expected[i]) < 0.00001f");

    }
}


void Test_Matrix10F3_Mult_3F3(void)
{
    math::Matrix10F3 matrixA(
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

    math::Matrix3F3 matrixB(
        {1.0f, 2.0f, 3.0f},
        {1.0f, 2.0f, 3.0f},
        {1.0f, 2.0f, 3.0f}
    );

    math::Matrix10F3 result;
    result.Zero();

    result = matrixA * matrixB;

    /* Verify results */
    for(int i = 0; i < 10; ++i)
    {
        UtAssert_True(result[i][0] == 6.0f,  "result[i][0] == 6.0f");
        UtAssert_True(result[i][1] == 12.0f, "result[i][1] == 12.0f");
        UtAssert_True(result[i][2] == 18.0f, "result[i][2] == 18.0f");
    }
}


/* Auto generated with GNU Octave, version 4.0.0 */
void Test_Matrix10F3_Mult_3F3_Random(void)
{
    int i, j = 0;

    math::Matrix10F3 A (
        {-0.869777, -0.209249, -0.826720},
        {-0.582387, 0.800718, -0.651797},
        {0.134643, 0.628875, 0.344686},
        {-0.067837, 0.576195, -0.143398},
        {-0.125536, -0.305244, -0.840798},
        {0.389404, 0.699705, -0.699568},
        {-0.066210, -0.829961, 0.158799},
        {-0.478039, -0.045986, 0.545617},
        {0.585030, -0.550624, -0.156732},
        {-0.063856, -0.268209, 0.775134}
    );

    math::Matrix3F3 B (
        {-0.743775, 0.986771, 0.379808},
        {-0.557144, -0.517249, 0.210579},
        {0.311645, -0.930732, -0.937297}
    );

    math::Matrix10F3 expected (
        {0.505857, 0.019418, 0.400470},
        {-0.216079, -0.382205, 0.558346},
        {-0.343098, -0.513234, -0.139507},
        {-0.315257, -0.231510, 0.229976},
        {0.001405, 0.816570, 0.676120},
        {-0.897481, 0.673442, 0.950945},
        {0.561141, 0.216163, -0.348761},
        {0.551213, -0.955753, -0.702652},
        {-0.177199, 1.007976, 0.253154},
        {0.438492, -0.645723, -0.807263}
    );

    math::Matrix10F3 result;
    result.Zero();

    result = A * B;

    for(i = 0; i < 10; i++)
    {
        for(j = 0; j < 3; j++)
        {
            UtAssert_True(fabs(result[i][j] - expected[i][j]) < 0.00001f, "fabs(result[i][j] - expected[i][j]) < 0.00001f");

        }
    }
}


void Test_Matrix10F3_Mult_3F10(void)
{
    math::Matrix10F3 matrixA(
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

    math::Matrix3F10 matrixB(
        {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f},
        {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f},
        {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f}
    );

    math::Matrix10F10 result;
    result.Zero();

    result = matrixA * matrixB;

    /* Verify results */
    for(int i = 0; i < 10; ++i)
    {
        UtAssert_True(result[i][0] == 6.0f,  "result[i][0] == 6.0f");
        UtAssert_True(result[i][1] == 12.0f, "result[i][1] == 12.0f");
        UtAssert_True(result[i][2] == 18.0f, "result[i][2] == 18.0f");
        UtAssert_True(result[i][3] == 24.0f, "result[i][3] == 24.0f");
        UtAssert_True(result[i][4] == 30.0f, "result[i][4] == 30.0f");
        UtAssert_True(result[i][5] == 36.0f, "result[i][5] == 36.0f");
        UtAssert_True(result[i][6] == 42.0f, "result[i][6] == 42.0f");
        UtAssert_True(result[i][7] == 48.0f, "result[i][7] == 48.0f");
        UtAssert_True(result[i][8] == 54.0f, "result[i][8] == 54.0f");
        UtAssert_True(result[i][9] == 60.0f, "result[i][9] == 60.0f");
    }
}


/* Auto generated with GNU Octave, version 4.0.0 */
void Test_Matrix10F3_Mult_3F10_Random(void)
{
    int i, j = 0;

    math::Matrix10F3 A (
        {0.467856, -0.954498, 0.558758},
        {-0.400624, 0.230742, 0.425071},
        {0.600145, 0.616247, -0.002770},
        {0.690160, 0.944331, -0.545031},
        {-0.847509, -0.171568, -0.120314},
        {-0.184383, -0.846017, -0.681928},
        {0.810149, 0.636266, 0.094402},
        {0.594387, 0.784677, 0.809666},
        {0.460963, 0.683411, -0.213605},
        {0.288931, 0.698322, -0.276355}
    );

    math::Matrix3F10 B (
        {0.361859, 0.670540, 0.643815, -0.311457, -0.549651, -0.506709, 0.903606, -0.305362, 0.198874, 0.678113},
        {0.961792, -0.163974, -0.713305, 0.129418, 0.187101, -0.688188, -0.246562, 0.947260, -0.602021, -0.990130},
        {-0.224041, 0.948865, -0.810843, -0.839051, 0.767760, -0.647431, 0.822712, -0.983814, -0.223526, 0.378060}
    );

    math::Matrix10F10 expected (
        {-0.873915, 1.000415, 0.528997, -0.738073, -0.006754, 0.058050, 1.117797, -1.596737, 0.542775, 1.473580},
        {-0.018277, 0.096864, -0.767183, -0.202017, 0.589728, -0.230998, -0.069187, -0.077282, -0.313600, -0.339431},
        {0.810490, 0.298745, -0.050944, -0.104843, -0.216697, -0.726399, 0.388073, 0.403209, -0.251021, -0.204245},
        {1.280100, -0.209227, 0.212674, 0.364567, -0.621115, -0.646717, -0.057607, 1.219987, -0.309423, -0.673058},
        {-0.444736, -0.654318, -0.325703, 0.342709, 0.341362, 0.625406, -0.822496, 0.214645, -0.038367, -0.450319},
        {-0.727633, -0.631969, 1.037696, 0.520110, -0.580501, 1.117147, -0.519044, -0.074204, 0.625080, 0.454825},
        {0.883966, 0.528481, -0.008811, -0.249191, -0.253775, -0.909499, 0.652842, 0.262447, -0.243030, -0.044924},
        {0.788382, 1.038158, -0.833551, -0.762927, 0.441737, -1.365389, 1.009742, -0.234771, -0.535166, -0.067769},
        {0.871959, -0.005650, -0.017506, 0.124101, -0.289499, -0.565594, 0.072290, 0.716755, -0.272008, -0.444836},
        {0.838108, -0.182990, -0.088018, 0.232261, -0.240329, -0.448060, -0.138460, 0.845145, -0.301171, -0.599980}
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


void Test_Matrix10F3_Mult_Scaler(void)
{
    math::Matrix10F3 matrixA(
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
    
    math::Matrix10F3 result;
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

    UtAssert_True(result[0][2] == 6.0f, "result[0][2] == 6.0f");
    UtAssert_True(result[1][2] == 6.0f, "result[1][2] == 6.0f");
    UtAssert_True(result[2][2] == 6.0f, "result[2][2] == 6.0f");
    UtAssert_True(result[3][2] == 6.0f, "result[3][2] == 6.0f");
    UtAssert_True(result[4][2] == 6.0f, "result[4][2] == 6.0f");
    UtAssert_True(result[5][2] == 6.0f, "result[5][2] == 6.0f");
    UtAssert_True(result[6][2] == 6.0f, "result[6][2] == 6.0f");
    UtAssert_True(result[7][2] == 6.0f, "result[7][2] == 6.0f");
    UtAssert_True(result[8][2] == 6.0f, "result[8][2] == 6.0f");
    UtAssert_True(result[9][2] == 6.0f, "result[9][2] == 6.0f");
}


void Test_Matrix10F3_Transpose(void)
{
    math::Matrix10F3 matrix(
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

    math::Matrix3F10 result = matrix.Transpose();

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

    UtAssert_True(result[2][0] == 3.0f, "result[2][0] == 3.0f");
    UtAssert_True(result[2][1] == 3.0f, "result[2][1] == 3.0f");
    UtAssert_True(result[2][2] == 3.0f, "result[2][2] == 3.0f");
    UtAssert_True(result[2][3] == 3.0f, "result[2][3] == 3.0f");
    UtAssert_True(result[2][4] == 3.0f, "result[2][4] == 3.0f");
    UtAssert_True(result[2][5] == 3.0f, "result[2][5] == 3.0f");
    UtAssert_True(result[2][6] == 3.0f, "result[2][6] == 3.0f");
    UtAssert_True(result[2][7] == 3.0f, "result[2][7] == 3.0f");
    UtAssert_True(result[2][8] == 3.0f, "result[2][8] == 3.0f");
    UtAssert_True(result[2][9] == 3.0f, "result[2][9] == 3.0f");

}


void Test_Matrix10F3_Addition(void)
{
    math::Matrix10F3 matrixA(
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
    math::Matrix10F3 result = matrixA + matrixB;

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

    UtAssert_True(result[0][2] == 6.0f, "result[0][2] == 6.0f");
    UtAssert_True(result[1][2] == 6.0f, "result[1][2] == 6.0f");
    UtAssert_True(result[2][2] == 6.0f, "result[2][2] == 6.0f");
    UtAssert_True(result[3][2] == 6.0f, "result[3][2] == 6.0f");
    UtAssert_True(result[4][2] == 6.0f, "result[4][2] == 6.0f");
    UtAssert_True(result[5][2] == 6.0f, "result[5][2] == 6.0f");
    UtAssert_True(result[6][2] == 6.0f, "result[6][2] == 6.0f");
    UtAssert_True(result[7][2] == 6.0f, "result[7][2] == 6.0f");
    UtAssert_True(result[8][2] == 6.0f, "result[8][2] == 6.0f");
    UtAssert_True(result[9][2] == 6.0f, "result[9][2] == 6.0f");

}
