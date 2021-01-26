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

#include "Matrix10F6_test.hpp"
#include <math/Matrix10F6.hpp>
#include <math/Matrix10F10.hpp>


#include "utassert.h"
#include <float.h>


void Test_Matrix10F6_Constructor(void)
{
    math::Matrix10F6 matrix(
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f},
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f},
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f},
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f},
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f},
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f},
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f},
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f},
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f},
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f}
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

    UtAssert_True(matrix[0][3] == 4.0f, "matrix[0][3] == 4.0f");
    UtAssert_True(matrix[1][3] == 4.0f, "matrix[1][3] == 4.0f");
    UtAssert_True(matrix[2][3] == 4.0f, "matrix[2][3] == 4.0f");
    UtAssert_True(matrix[3][3] == 4.0f, "matrix[3][3] == 4.0f");
    UtAssert_True(matrix[4][3] == 4.0f, "matrix[4][3] == 4.0f");
    UtAssert_True(matrix[5][3] == 4.0f, "matrix[5][3] == 4.0f");
    UtAssert_True(matrix[6][3] == 4.0f, "matrix[6][3] == 4.0f");
    UtAssert_True(matrix[7][3] == 4.0f, "matrix[7][3] == 4.0f");
    UtAssert_True(matrix[8][3] == 4.0f, "matrix[8][3] == 4.0f");
    UtAssert_True(matrix[9][3] == 4.0f, "matrix[9][3] == 4.0f");

    UtAssert_True(matrix[0][4] == 5.0f, "matrix[0][4] == 5.0f");
    UtAssert_True(matrix[1][4] == 5.0f, "matrix[1][4] == 5.0f");
    UtAssert_True(matrix[2][4] == 5.0f, "matrix[2][4] == 5.0f");
    UtAssert_True(matrix[3][4] == 5.0f, "matrix[3][4] == 5.0f");
    UtAssert_True(matrix[4][4] == 5.0f, "matrix[4][4] == 5.0f");
    UtAssert_True(matrix[5][4] == 5.0f, "matrix[5][4] == 5.0f");
    UtAssert_True(matrix[6][4] == 5.0f, "matrix[6][4] == 5.0f");
    UtAssert_True(matrix[7][4] == 5.0f, "matrix[7][4] == 5.0f");
    UtAssert_True(matrix[8][4] == 5.0f, "matrix[8][4] == 5.0f");
    UtAssert_True(matrix[9][4] == 5.0f, "matrix[9][4] == 5.0f");

    UtAssert_True(matrix[0][5] == 6.0f, "matrix[0][5] == 6.0f");
    UtAssert_True(matrix[1][5] == 6.0f, "matrix[1][5] == 6.0f");
    UtAssert_True(matrix[2][5] == 6.0f, "matrix[2][5] == 6.0f");
    UtAssert_True(matrix[3][5] == 6.0f, "matrix[3][5] == 6.0f");
    UtAssert_True(matrix[4][5] == 6.0f, "matrix[4][5] == 6.0f");
    UtAssert_True(matrix[5][5] == 6.0f, "matrix[5][5] == 6.0f");
    UtAssert_True(matrix[6][5] == 6.0f, "matrix[6][5] == 6.0f");
    UtAssert_True(matrix[7][5] == 6.0f, "matrix[7][5] == 6.0f");
    UtAssert_True(matrix[8][5] == 6.0f, "matrix[8][5] == 6.0f");
    UtAssert_True(matrix[9][5] == 6.0f, "matrix[9][5] == 6.0f");
}


void Test_Matrix10F6_OutOfBounds(void)
{
    math::Matrix10F6 matrix(
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f},
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f},
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f},
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f},
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f},
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f},
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f},
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f},
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f},
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f}
    );

    /* Verify results */
    UtAssert_True(!isfinite(matrix[0][6]), "matrix[0][6] == NAN");
    UtAssert_True(!isfinite(matrix[10][0]), "matrix[10][0] == NAN");
}


void Test_Matrix10F6_Mult_6F(void)
{
    math::Matrix10F6 matrixA(
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f},
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f},
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f},
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f},
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f},
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f},
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f},
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f},
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f},
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f}
    );

    math::Vector6F vectorB(
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f}
    );
    
    math::Vector10F result;
    
    result = matrixA * vectorB;

    /* Verify results */
    UtAssert_True(result[0] == 91.0f, "result[0] == 91.0f");
    UtAssert_True(result[1] == 91.0f, "result[1] == 91.0f");
    UtAssert_True(result[2] == 91.0f, "result[2] == 91.0f");
    UtAssert_True(result[3] == 91.0f, "result[3] == 91.0f");
    UtAssert_True(result[4] == 91.0f, "result[4] == 91.0f");
    UtAssert_True(result[5] == 91.0f, "result[5] == 91.0f");
    UtAssert_True(result[6] == 91.0f, "result[6] == 91.0f");
    UtAssert_True(result[7] == 91.0f, "result[7] == 91.0f");
    UtAssert_True(result[8] == 91.0f, "result[8] == 91.0f");
    UtAssert_True(result[9] == 91.0f, "result[9] == 91.0f");
}


/* Auto generated with GNU Octave, version 4.0.0 */
void Test_Matrix10F6_Mult_6F_Random(void)
{
    int i, j = 0;

    math::Matrix10F6 A (
        {0.807287, 0.419844, 0.659787, -0.613013, 0.580066, -0.121667},
        {-0.024354, 0.844951, -0.846564, 0.523159, -0.461940, -0.209270},
        {0.732011, -0.982809, -0.951160, -0.931787, -0.086699, -0.492228},
        {0.229060, 0.181308, 0.760085, 0.482016, -0.370160, 0.250648},
        {0.376988, 0.940099, 0.969682, -0.062760, -0.265268, 0.798259},
        {0.438542, -0.736570, -0.722367, 0.785847, 0.522473, 0.132046},
        {-0.550162, -0.670657, -0.413729, -0.647885, -0.930779, -0.368044},
        {-0.571656, 0.971052, -0.273582, 0.913818, 0.419619, -0.003115},
        {0.217193, 0.334840, 0.326746, -0.118148, 0.932827, 0.600662},
        {-0.650198, 0.330728, 0.118228, -0.979947, 0.327954, -0.734835}
    );

    math::Vector6F B (
        {-0.544272,0.394184,0.163907,0.547141,0.129751,0.921195}
    );

    math::Vector10F expected (
        {-0.537964,0.241089,-1.916228,0.517979,0.990920,-0.028031,-0.847030,1.200631,0.677051,-0.666913}
    );

    math::Vector10F result;
    result.Zero();

    result = A * B;

    for(i = 0; i < 10; i++)
    {
        UtAssert_True(fabs(result[i] - expected[i]) < 0.00001f, "fabs(result[i] - expected[i]) < 0.00001f");

    }
}


void Test_Matrix10F6_Mult_6F6(void)
{
    math::Matrix10F6 matrixA(
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f},
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f},
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f},
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f},
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f},
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f},
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f},
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f},
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f},
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f}
    );

    math::Matrix6F6 matrixB(
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f},
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f},
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f},
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f},
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f},
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f}
    );
    
    math::Matrix10F6 result;
    
    result = matrixA * matrixB;

    /* Verify results */
    UtAssert_True(result[0][0] == 21.0f, "result[0][0] == 21.0f");
    UtAssert_True(result[1][0] == 21.0f, "result[1][0] == 21.0f");
    UtAssert_True(result[2][0] == 21.0f, "result[2][0] == 21.0f");
    UtAssert_True(result[3][0] == 21.0f, "result[3][0] == 21.0f");
    UtAssert_True(result[4][0] == 21.0f, "result[4][0] == 21.0f");
    UtAssert_True(result[5][0] == 21.0f, "result[5][0] == 21.0f");
    UtAssert_True(result[6][0] == 21.0f, "result[6][0] == 21.0f");
    UtAssert_True(result[7][0] == 21.0f, "result[7][0] == 21.0f");
    UtAssert_True(result[8][0] == 21.0f, "result[8][0] == 21.0f");
    UtAssert_True(result[9][0] == 21.0f, "result[9][0] == 21.0f");

    UtAssert_True(result[0][1] == 42.0f, "result[0][1] == 42.0f");
    UtAssert_True(result[1][1] == 42.0f, "result[1][1] == 42.0f");
    UtAssert_True(result[2][1] == 42.0f, "result[2][1] == 42.0f");
    UtAssert_True(result[3][1] == 42.0f, "result[3][1] == 42.0f");
    UtAssert_True(result[4][1] == 42.0f, "result[4][1] == 42.0f");
    UtAssert_True(result[5][1] == 42.0f, "result[5][1] == 42.0f");
    UtAssert_True(result[6][1] == 42.0f, "result[6][1] == 42.0f");
    UtAssert_True(result[7][1] == 42.0f, "result[7][1] == 42.0f");
    UtAssert_True(result[8][1] == 42.0f, "result[8][1] == 42.0f");
    UtAssert_True(result[9][1] == 42.0f, "result[9][1] == 42.0f");

    UtAssert_True(result[0][2] == 63.0f, "result[0][2] == 63.0f");
    UtAssert_True(result[1][2] == 63.0f, "result[1][2] == 63.0f");
    UtAssert_True(result[2][2] == 63.0f, "result[2][2] == 63.0f");
    UtAssert_True(result[3][2] == 63.0f, "result[3][2] == 63.0f");
    UtAssert_True(result[4][2] == 63.0f, "result[4][2] == 63.0f");
    UtAssert_True(result[5][2] == 63.0f, "result[5][2] == 63.0f");
    UtAssert_True(result[6][2] == 63.0f, "result[6][2] == 63.0f");
    UtAssert_True(result[7][2] == 63.0f, "result[7][2] == 63.0f");
    UtAssert_True(result[8][2] == 63.0f, "result[8][2] == 63.0f");
    UtAssert_True(result[9][2] == 63.0f, "result[9][2] == 63.0f");

    UtAssert_True(result[0][3] == 84.0f, "result[0][3] == 84.0f");
    UtAssert_True(result[1][3] == 84.0f, "result[1][3] == 84.0f");
    UtAssert_True(result[2][3] == 84.0f, "result[2][3] == 84.0f");
    UtAssert_True(result[3][3] == 84.0f, "result[3][3] == 84.0f");
    UtAssert_True(result[4][3] == 84.0f, "result[4][3] == 84.0f");
    UtAssert_True(result[5][3] == 84.0f, "result[5][3] == 84.0f");
    UtAssert_True(result[6][3] == 84.0f, "result[6][3] == 84.0f");
    UtAssert_True(result[7][3] == 84.0f, "result[7][3] == 84.0f");
    UtAssert_True(result[8][3] == 84.0f, "result[8][3] == 84.0f");
    UtAssert_True(result[9][3] == 84.0f, "result[9][3] == 84.0f");

    UtAssert_True(result[0][4] == 105.0f, "result[0][4] == 105.0f");
    UtAssert_True(result[1][4] == 105.0f, "result[1][4] == 105.0f");
    UtAssert_True(result[2][4] == 105.0f, "result[2][4] == 105.0f");
    UtAssert_True(result[3][4] == 105.0f, "result[3][4] == 105.0f");
    UtAssert_True(result[4][4] == 105.0f, "result[4][4] == 105.0f");
    UtAssert_True(result[5][4] == 105.0f, "result[5][4] == 105.0f");
    UtAssert_True(result[6][4] == 105.0f, "result[6][4] == 105.0f");
    UtAssert_True(result[7][4] == 105.0f, "result[7][4] == 105.0f");
    UtAssert_True(result[8][4] == 105.0f, "result[8][4] == 105.0f");
    UtAssert_True(result[9][4] == 105.0f, "result[9][4] == 105.0f");

    UtAssert_True(result[0][5] == 126.0f, "result[0][5] == 126.0f");
    UtAssert_True(result[1][5] == 126.0f, "result[1][5] == 126.0f");
    UtAssert_True(result[2][5] == 126.0f, "result[2][5] == 126.0f");
    UtAssert_True(result[3][5] == 126.0f, "result[3][5] == 126.0f");
    UtAssert_True(result[4][5] == 126.0f, "result[4][5] == 126.0f");
    UtAssert_True(result[5][5] == 126.0f, "result[5][5] == 126.0f");
    UtAssert_True(result[6][5] == 126.0f, "result[6][5] == 126.0f");
    UtAssert_True(result[7][5] == 126.0f, "result[7][5] == 126.0f");
    UtAssert_True(result[8][5] == 126.0f, "result[8][5] == 126.0f");
    UtAssert_True(result[9][5] == 126.0f, "result[9][5] == 126.0f");
}


/* Auto generated with GNU Octave, version 4.0.0 */
void Test_Matrix10F6_Mult_6F6_Random(void)
{
    int i, j = 0;

    math::Matrix10F6 A (
        {0.446850, -0.563391, 0.820279, 0.609366, -0.075542, -0.127568},
        {-0.963317, -0.059497, 0.109892, 0.267407, 0.526809, 0.099597},
        {-0.630767, -0.387657, -0.410373, 0.620630, 0.598152, -0.411291},
        {-0.607321, 0.654330, 0.298242, -0.329365, 0.196409, -0.615003},
        {-0.291814, -0.435198, 0.684590, -0.886537, -0.102154, 0.618577},
        {-0.646956, 0.600941, 0.127197, -0.562579, -0.680662, -0.588874},
        {-0.737436, 0.709735, 0.279948, 0.298700, 0.715120, -0.364580},
        {-0.849586, 0.709204, -0.957620, -0.736755, -0.547152, 0.933565},
        {0.644736, -0.953263, 0.204943, -0.057846, -0.334686, -0.115624},
        {-0.472937, 0.679438, -0.567083, 0.691191, -0.621953, -0.604357}
    );

    math::Matrix6F6 B (
        {0.681531, 0.797187, -0.754516, 0.703742, -0.588108, 0.587410},
        {-0.986639, -0.587254, -0.300791, 0.816348, -0.550762, -0.769738},
        {0.799137, 0.383570, 0.727920, -0.488007, 0.278084, 0.444745},
        {0.531995, 0.496564, -0.529327, 0.725091, 0.483323, -0.566500},
        {-0.034295, -0.074060, 0.810683, -0.073696, 0.692918, -0.220299},
        {-0.434248, -0.339689, 0.143093, -0.574851, 0.477723, 0.548535}
    );

    math::Matrix10F6 expected (
        {1.898089, 1.353229, 0.027357, -0.025013, 0.456839, 0.662423},
        {-0.429067, -0.630916, 1.124508, -0.682308, 1.171722, -0.684100},
        {0.112906, -0.028999, 0.391352, 0.082268, 0.988299, -0.963603},
        {-0.736053, -0.723197, 0.724077, 0.061461, -0.237169, -0.921801},
        {0.040839, -0.357252, 1.324375, -1.885601, 0.397922, 1.332081},
        {-0.952414, -0.848774, 0.061694, -0.046031, -0.939993, -0.640393},
        {-0.686421, -0.678084, 0.916162, 0.297271, 0.586365, -1.381720},
        {-2.822601, -2.103522, -0.189363, -0.582159, -0.446489, -0.420852},
        {1.574625, 1.187731, -0.307799, -0.375292, -0.112267, 1.246711},
        {-0.794378, -0.398962, -1.216873, 1.393000, -0.639376, -1.639060}
    );

    math::Matrix10F6 result;
    result.Zero();

    result = A * B;

    for(i = 0; i < 10; i++)
    {
        for(j = 0; j < 6; j++)
        {
            UtAssert_True(fabs(result[i][j] - expected[i][j]) < 0.00001f, "fabs(result[i][j] - expected[i][j]) < 0.00001f");

        }
    }
}


void Test_Matrix10F6_Mult_6F10(void)
{
    math::Matrix10F6 matrixA(
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f},
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f},
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f},
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f},
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f},
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f},
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f},
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f},
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f},
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f}
    );

    math::Matrix6F10 matrixB(
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f},
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f},
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f},
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f},
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f},
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f}
    );
    
    math::Matrix10F10 result;
    
    result = matrixA * matrixB;

    /* Verify results */
    UtAssert_True(result[0][0] == 21.0f, "result[0][0] == 21.0f");
    UtAssert_True(result[1][0] == 21.0f, "result[1][0] == 21.0f");
    UtAssert_True(result[2][0] == 21.0f, "result[2][0] == 21.0f");
    UtAssert_True(result[3][0] == 21.0f, "result[3][0] == 21.0f");
    UtAssert_True(result[4][0] == 21.0f, "result[4][0] == 21.0f");
    UtAssert_True(result[5][0] == 21.0f, "result[5][0] == 21.0f");
    UtAssert_True(result[6][0] == 21.0f, "result[6][0] == 21.0f");
    UtAssert_True(result[7][0] == 21.0f, "result[7][0] == 21.0f");
    UtAssert_True(result[8][0] == 21.0f, "result[8][0] == 21.0f");
    UtAssert_True(result[9][0] == 21.0f, "result[9][0] == 21.0f");

    UtAssert_True(result[0][1] == 42.0f, "result[0][1] == 42.0f");
    UtAssert_True(result[1][1] == 42.0f, "result[1][1] == 42.0f");
    UtAssert_True(result[2][1] == 42.0f, "result[2][1] == 42.0f");
    UtAssert_True(result[3][1] == 42.0f, "result[3][1] == 42.0f");
    UtAssert_True(result[4][1] == 42.0f, "result[4][1] == 42.0f");
    UtAssert_True(result[5][1] == 42.0f, "result[5][1] == 42.0f");
    UtAssert_True(result[6][1] == 42.0f, "result[6][1] == 42.0f");
    UtAssert_True(result[7][1] == 42.0f, "result[7][1] == 42.0f");
    UtAssert_True(result[8][1] == 42.0f, "result[8][1] == 42.0f");
    UtAssert_True(result[9][1] == 42.0f, "result[9][1] == 42.0f");

    UtAssert_True(result[0][2] == 63.0f, "result[0][2] == 63.0f");
    UtAssert_True(result[1][2] == 63.0f, "result[1][2] == 63.0f");
    UtAssert_True(result[2][2] == 63.0f, "result[2][2] == 63.0f");
    UtAssert_True(result[3][2] == 63.0f, "result[3][2] == 63.0f");
    UtAssert_True(result[4][2] == 63.0f, "result[4][2] == 63.0f");
    UtAssert_True(result[5][2] == 63.0f, "result[5][2] == 63.0f");
    UtAssert_True(result[6][2] == 63.0f, "result[6][2] == 63.0f");
    UtAssert_True(result[7][2] == 63.0f, "result[7][2] == 63.0f");
    UtAssert_True(result[8][2] == 63.0f, "result[8][2] == 63.0f");
    UtAssert_True(result[9][2] == 63.0f, "result[9][2] == 63.0f");

    UtAssert_True(result[0][3] == 84.0f, "result[0][3] == 84.0f");
    UtAssert_True(result[1][3] == 84.0f, "result[1][3] == 84.0f");
    UtAssert_True(result[2][3] == 84.0f, "result[2][3] == 84.0f");
    UtAssert_True(result[3][3] == 84.0f, "result[3][3] == 84.0f");
    UtAssert_True(result[4][3] == 84.0f, "result[4][3] == 84.0f");
    UtAssert_True(result[5][3] == 84.0f, "result[5][3] == 84.0f");
    UtAssert_True(result[6][3] == 84.0f, "result[6][3] == 84.0f");
    UtAssert_True(result[7][3] == 84.0f, "result[7][3] == 84.0f");
    UtAssert_True(result[8][3] == 84.0f, "result[8][3] == 84.0f");
    UtAssert_True(result[9][3] == 84.0f, "result[9][3] == 84.0f");

    UtAssert_True(result[0][4] == 105.0f, "result[0][4] == 105.0f");
    UtAssert_True(result[1][4] == 105.0f, "result[1][4] == 105.0f");
    UtAssert_True(result[2][4] == 105.0f, "result[2][4] == 105.0f");
    UtAssert_True(result[3][4] == 105.0f, "result[3][4] == 105.0f");
    UtAssert_True(result[4][4] == 105.0f, "result[4][4] == 105.0f");
    UtAssert_True(result[5][4] == 105.0f, "result[5][4] == 105.0f");
    UtAssert_True(result[6][4] == 105.0f, "result[6][4] == 105.0f");
    UtAssert_True(result[7][4] == 105.0f, "result[7][4] == 105.0f");
    UtAssert_True(result[8][4] == 105.0f, "result[8][4] == 105.0f");
    UtAssert_True(result[9][4] == 105.0f, "result[9][4] == 105.0f");

    UtAssert_True(result[0][5] == 126.0f, "result[0][5] == 126.0f");
    UtAssert_True(result[1][5] == 126.0f, "result[1][5] == 126.0f");
    UtAssert_True(result[2][5] == 126.0f, "result[2][5] == 126.0f");
    UtAssert_True(result[3][5] == 126.0f, "result[3][5] == 126.0f");
    UtAssert_True(result[4][5] == 126.0f, "result[4][5] == 126.0f");
    UtAssert_True(result[5][5] == 126.0f, "result[5][5] == 126.0f");
    UtAssert_True(result[6][5] == 126.0f, "result[6][5] == 126.0f");
    UtAssert_True(result[7][5] == 126.0f, "result[7][5] == 126.0f");
    UtAssert_True(result[8][5] == 126.0f, "result[8][5] == 126.0f");
    UtAssert_True(result[9][5] == 126.0f, "result[9][5] == 126.0f");

    UtAssert_True(result[0][6] == 147.0f, "result[0][6] == 147.0f");
    UtAssert_True(result[1][6] == 147.0f, "result[1][6] == 147.0f");
    UtAssert_True(result[2][6] == 147.0f, "result[2][6] == 147.0f");
    UtAssert_True(result[3][6] == 147.0f, "result[3][6] == 147.0f");
    UtAssert_True(result[4][6] == 147.0f, "result[4][6] == 147.0f");
    UtAssert_True(result[5][6] == 147.0f, "result[5][6] == 147.0f");
    UtAssert_True(result[6][6] == 147.0f, "result[6][6] == 147.0f");
    UtAssert_True(result[7][6] == 147.0f, "result[7][6] == 147.0f");
    UtAssert_True(result[8][6] == 147.0f, "result[8][6] == 147.0f");
    UtAssert_True(result[9][6] == 147.0f, "result[9][6] == 147.0f");

    UtAssert_True(result[0][7] == 168.0f, "result[0][7] == 168.0f");
    UtAssert_True(result[1][7] == 168.0f, "result[1][7] == 168.0f");
    UtAssert_True(result[2][7] == 168.0f, "result[2][7] == 168.0f");
    UtAssert_True(result[3][7] == 168.0f, "result[3][7] == 168.0f");
    UtAssert_True(result[4][7] == 168.0f, "result[4][7] == 168.0f");
    UtAssert_True(result[5][7] == 168.0f, "result[5][7] == 168.0f");
    UtAssert_True(result[6][7] == 168.0f, "result[6][7] == 168.0f");
    UtAssert_True(result[7][7] == 168.0f, "result[7][7] == 168.0f");
    UtAssert_True(result[8][7] == 168.0f, "result[8][7] == 168.0f");
    UtAssert_True(result[9][7] == 168.0f, "result[9][7] == 168.0f");

    UtAssert_True(result[0][8] == 189.0f, "result[0][8] == 189.0f");
    UtAssert_True(result[1][8] == 189.0f, "result[1][8] == 189.0f");
    UtAssert_True(result[2][8] == 189.0f, "result[2][8] == 189.0f");
    UtAssert_True(result[3][8] == 189.0f, "result[3][8] == 189.0f");
    UtAssert_True(result[4][8] == 189.0f, "result[4][8] == 189.0f");
    UtAssert_True(result[5][8] == 189.0f, "result[5][8] == 189.0f");
    UtAssert_True(result[6][8] == 189.0f, "result[6][8] == 189.0f");
    UtAssert_True(result[7][8] == 189.0f, "result[7][8] == 189.0f");
    UtAssert_True(result[8][8] == 189.0f, "result[8][8] == 189.0f");
    UtAssert_True(result[9][8] == 189.0f, "result[9][8] == 189.0f");

    UtAssert_True(result[0][9] == 210.0f, "result[0][9] == 210.0f");
    UtAssert_True(result[1][9] == 210.0f, "result[1][9] == 210.0f");
    UtAssert_True(result[2][9] == 210.0f, "result[2][9] == 210.0f");
    UtAssert_True(result[3][9] == 210.0f, "result[3][9] == 210.0f");
    UtAssert_True(result[4][9] == 210.0f, "result[4][9] == 210.0f");
    UtAssert_True(result[5][9] == 210.0f, "result[5][9] == 210.0f");
    UtAssert_True(result[6][9] == 210.0f, "result[6][9] == 210.0f");
    UtAssert_True(result[7][9] == 210.0f, "result[7][9] == 210.0f");
    UtAssert_True(result[8][9] == 210.0f, "result[8][9] == 210.0f");
    UtAssert_True(result[9][9] == 210.0f, "result[9][9] == 210.0f");
}


/* Auto generated with GNU Octave, version 4.0.0 */
void Test_Matrix10F6_Mult_6F10_Random(void)
{
    int i, j = 0;

    math::Matrix10F6 A (
        {-0.277028, 0.310363, -0.275584, 0.300425, 0.249210, 0.044852},
        {0.892603, -0.514368, 0.816955, 0.501894, -0.398091, 0.292527},
        {-0.688337, 0.259920, 0.613277, 0.696687, -0.654432, -0.124166},
        {0.728706, 0.925493, -0.162484, 0.878325, -0.410506, -0.686961},
        {0.865323, -0.496739, -0.402914, -0.649127, 0.289909, -0.034628},
        {-0.879554, 0.379973, 0.262784, 0.087249, -0.102489, -0.256590},
        {0.812466, 0.019872, 0.886982, -0.257231, -0.200846, -0.929160},
        {-0.229175, 0.663560, 0.861170, -0.123155, -0.844735, -0.983019},
        {-0.054078, -0.917943, 0.968998, 0.362606, 0.380480, 0.472329},
        {-0.626968, 0.100104, -0.361101, -0.693384, -0.797762, -0.926318}
    );

    math::Matrix6F10 B (
        {0.727600, -0.279516, -0.270794, 0.399746, -0.067369, 0.337085, -0.680571, -0.190303, -0.411068, -0.030754},
        {0.055449, 0.137818, -0.586754, 0.785234, 0.054090, -0.581767, -0.581804, 0.419501, 0.744530, 0.036419},
        {0.134823, 0.750355, -0.059411, -0.147507, -0.791924, 0.951290, -0.245176, -0.313508, 0.913061, -0.718476},
        {-0.487151, 0.003946, 0.525353, 0.886762, -0.746794, -0.653048, 0.477698, -0.758530, 0.206207, -0.602089},
        {0.707987, -0.432386, 0.035823, -0.416456, 0.745128, 0.967731, -0.761619, 0.434606, 0.068696, -0.949132},
        {0.048559, 0.480718, -0.042047, 0.131845, -0.568026, -0.193522, -0.270082, -0.309098, 0.506544, -0.298958}
    );

    math::Matrix10F10 expected (
        {-0.189248, -0.171587, 0.074155, 0.342152, 0.189553, -0.499805, 0.017129, 0.135877, 0.195115, -0.213001},
        {0.218943, 0.607353, 0.248670, 0.481825, -1.572525, 0.607669, -0.044574, -1.285899, 0.220372, -0.644943},
        {-1.212488, 0.914426, 0.345237, 0.712443, -1.362625, -0.864092, 1.031647, -0.726737, 1.072240, -0.171194},
        {-0.192251, -0.347327, -0.255103, 1.901245, -0.441953, -1.285261, -0.076795, -0.331796, 0.046093, 0.194204},
        {1.067536, -0.757220, -0.248103, -0.685635, 0.954366, 0.908551, -0.722658, 0.382343, -1.224909, 0.370804},
        {-0.710988, 0.416709, 0.052569, -0.005772, -0.124072, -0.374060, 0.502137, 0.212984, 0.765372, -0.026462},
        {0.649832, 0.080358, -0.387631, -0.057416, -0.185854, 1.259518, -0.500929, -0.029325, -0.046814, -0.038252},
        {-0.599649, 0.693903, -0.432078, 0.415390, -0.609734, -0.190881, 0.408801, 0.082131, 0.793181, 0.582282},
        {0.156064, 0.679673, 0.679947, -0.659985, -1.068961, 1.477594, 0.089160, -0.934259, 0.563713, -1.448623},
        {-0.771320, -0.185003, -0.221404, -0.523521, 0.783169, -0.753035, 0.983533, 0.740078, -0.664454, 1.733961}
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



void Test_Matrix10F6_Zero(void)
{
    math::Matrix10F6 matrix(
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f},
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f},
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f},
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f},
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f},
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f},
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f},
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f},
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f},
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f}
    );
    
    math::Matrix10F6 result = matrix;
    result.Zero();

    /* Verify results */
    UtAssert_True(result[0][0] == 0.0f, "result[0][0] == 0.0f");
    UtAssert_True(result[1][0] == 0.0f, "result[1][0] == 0.0f");
    UtAssert_True(result[2][0] == 0.0f, "result[2][0] == 0.0f");
    UtAssert_True(result[3][0] == 0.0f, "result[3][0] == 0.0f");
    UtAssert_True(result[4][0] == 0.0f, "result[4][0] == 0.0f");
    UtAssert_True(result[5][0] == 0.0f, "result[5][0] == 0.0f");
    UtAssert_True(result[6][0] == 0.0f, "result[6][0] == 0.0f");
    UtAssert_True(result[7][0] == 0.0f, "result[7][0] == 0.0f");
    UtAssert_True(result[8][0] == 0.0f, "result[8][0] == 0.0f");
    UtAssert_True(result[9][0] == 0.0f, "result[9][0] == 0.0f");

    UtAssert_True(result[0][1] == 0.0f, "result[0][1] == 0.0f");
    UtAssert_True(result[1][1] == 0.0f, "result[1][1] == 0.0f");
    UtAssert_True(result[2][1] == 0.0f, "result[2][1] == 0.0f");
    UtAssert_True(result[3][1] == 0.0f, "result[3][1] == 0.0f");
    UtAssert_True(result[4][1] == 0.0f, "result[4][1] == 0.0f");
    UtAssert_True(result[5][1] == 0.0f, "result[5][1] == 0.0f");
    UtAssert_True(result[6][1] == 0.0f, "result[6][1] == 0.0f");
    UtAssert_True(result[7][1] == 0.0f, "result[7][1] == 0.0f");
    UtAssert_True(result[8][1] == 0.0f, "result[8][1] == 0.0f");
    UtAssert_True(result[9][1] == 0.0f, "result[9][1] == 0.0f");

    UtAssert_True(result[0][2] == 0.0f, "result[0][2] == 0.0f");
    UtAssert_True(result[1][2] == 0.0f, "result[1][2] == 0.0f");
    UtAssert_True(result[2][2] == 0.0f, "result[2][2] == 0.0f");
    UtAssert_True(result[3][2] == 0.0f, "result[3][2] == 0.0f");
    UtAssert_True(result[4][2] == 0.0f, "result[4][2] == 0.0f");
    UtAssert_True(result[5][2] == 0.0f, "result[5][2] == 0.0f");
    UtAssert_True(result[6][2] == 0.0f, "result[6][2] == 0.0f");
    UtAssert_True(result[7][2] == 0.0f, "result[7][2] == 0.0f");
    UtAssert_True(result[8][2] == 0.0f, "result[8][2] == 0.0f");
    UtAssert_True(result[9][2] == 0.0f, "result[9][2] == 0.0f");

    UtAssert_True(result[0][3] == 0.0f, "result[0][3] == 0.0f");
    UtAssert_True(result[1][3] == 0.0f, "result[1][3] == 0.0f");
    UtAssert_True(result[2][3] == 0.0f, "result[2][3] == 0.0f");
    UtAssert_True(result[3][3] == 0.0f, "result[3][3] == 0.0f");
    UtAssert_True(result[4][3] == 0.0f, "result[4][3] == 0.0f");
    UtAssert_True(result[5][3] == 0.0f, "result[5][3] == 0.0f");
    UtAssert_True(result[6][3] == 0.0f, "result[6][3] == 0.0f");
    UtAssert_True(result[7][3] == 0.0f, "result[7][3] == 0.0f");
    UtAssert_True(result[8][3] == 0.0f, "result[8][3] == 0.0f");
    UtAssert_True(result[9][3] == 0.0f, "result[9][3] == 0.0f");

    UtAssert_True(result[0][4] == 0.0f, "result[0][4] == 0.0f");
    UtAssert_True(result[1][4] == 0.0f, "result[1][4] == 0.0f");
    UtAssert_True(result[2][4] == 0.0f, "result[2][4] == 0.0f");
    UtAssert_True(result[3][4] == 0.0f, "result[3][4] == 0.0f");
    UtAssert_True(result[4][4] == 0.0f, "result[4][4] == 0.0f");
    UtAssert_True(result[5][4] == 0.0f, "result[5][4] == 0.0f");
    UtAssert_True(result[6][4] == 0.0f, "result[6][4] == 0.0f");
    UtAssert_True(result[7][4] == 0.0f, "result[7][4] == 0.0f");
    UtAssert_True(result[8][4] == 0.0f, "result[8][4] == 0.0f");
    UtAssert_True(result[9][4] == 0.0f, "result[9][4] == 0.0f");

    UtAssert_True(result[0][5] == 0.0f, "result[0][5] == 0.0f");
    UtAssert_True(result[1][5] == 0.0f, "result[1][5] == 0.0f");
    UtAssert_True(result[2][5] == 0.0f, "result[2][5] == 0.0f");
    UtAssert_True(result[3][5] == 0.0f, "result[3][5] == 0.0f");
    UtAssert_True(result[4][5] == 0.0f, "result[4][5] == 0.0f");
    UtAssert_True(result[5][5] == 0.0f, "result[5][5] == 0.0f");
    UtAssert_True(result[6][5] == 0.0f, "result[6][5] == 0.0f");
    UtAssert_True(result[7][5] == 0.0f, "result[7][5] == 0.0f");
    UtAssert_True(result[8][5] == 0.0f, "result[8][5] == 0.0f");
    UtAssert_True(result[9][5] == 0.0f, "result[9][5] == 0.0f");
}
