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

#include "Matrix6F10_test.hpp"
#include <math/Matrix6F10.hpp>
#include <math/Vector10F.hpp>
#include <math/Vector6F.hpp>

#include "utassert.h"
#include <float.h>


void Test_Matrix6F10_Constructor(void)
{
	math::Matrix6F10 matrix(
			{1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
			{2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f},
			{3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f},
			{4.0f, 4.0f, 4.0f, 4.0f, 4.0f, 4.0f, 4.0f, 4.0f, 4.0f, 4.0f},
			{5.0f, 5.0f, 5.0f, 5.0f, 5.0f, 5.0f, 5.0f, 5.0f, 5.0f, 5.0f},
            {6.0f, 6.0f, 6.0f, 6.0f, 6.0f, 6.0f, 6.0f, 6.0f, 6.0f, 6.0f}
	);

	/* Verify results */
	UtAssert_True(matrix[0][0] == 1.0f, "matrix[0][0] == 1.0f");
	UtAssert_True(matrix[1][0] == 2.0f, "matrix[1][0] == 2.0f");
	UtAssert_True(matrix[2][0] == 3.0f, "matrix[2][0] == 3.0f");
	UtAssert_True(matrix[3][0] == 4.0f, "matrix[3][0] == 4.0f");
	UtAssert_True(matrix[4][0] == 5.0f, "matrix[4][0] == 5.0f");
	UtAssert_True(matrix[5][0] == 6.0f, "matrix[5][0] == 6.0f");

	UtAssert_True(matrix[0][1] == 1.0f, "matrix[0][1] == 1.0f");
	UtAssert_True(matrix[1][1] == 2.0f, "matrix[1][1] == 2.0f");
	UtAssert_True(matrix[2][1] == 3.0f, "matrix[2][1] == 3.0f");
	UtAssert_True(matrix[3][1] == 4.0f, "matrix[3][1] == 4.0f");
	UtAssert_True(matrix[4][1] == 5.0f, "matrix[4][1] == 5.0f");
	UtAssert_True(matrix[5][1] == 6.0f, "matrix[5][1] == 6.0f");
    
	UtAssert_True(matrix[0][2] == 1.0f, "matrix[0][0] == 1.0f");
	UtAssert_True(matrix[1][2] == 2.0f, "matrix[1][1] == 2.0f");
	UtAssert_True(matrix[2][2] == 3.0f, "matrix[2][2] == 3.0f");
	UtAssert_True(matrix[3][2] == 4.0f, "matrix[3][3] == 4.0f");
	UtAssert_True(matrix[4][2] == 5.0f, "matrix[4][4] == 5.0f");
	UtAssert_True(matrix[5][2] == 6.0f, "matrix[5][5] == 6.0f");

	UtAssert_True(matrix[0][3] == 1.0f, "matrix[0][3] == 1.0f");
	UtAssert_True(matrix[1][3] == 2.0f, "matrix[1][3] == 2.0f");
	UtAssert_True(matrix[2][3] == 3.0f, "matrix[2][3] == 3.0f");
	UtAssert_True(matrix[3][3] == 4.0f, "matrix[3][3] == 4.0f");
	UtAssert_True(matrix[4][3] == 5.0f, "matrix[4][3] == 5.0f");
	UtAssert_True(matrix[5][3] == 6.0f, "matrix[5][3] == 6.0f");
    
	UtAssert_True(matrix[0][4] == 1.0f, "matrix[0][4] == 1.0f");
	UtAssert_True(matrix[1][4] == 2.0f, "matrix[1][4] == 2.0f");
	UtAssert_True(matrix[2][4] == 3.0f, "matrix[2][4] == 3.0f");
	UtAssert_True(matrix[3][4] == 4.0f, "matrix[3][4] == 4.0f");
	UtAssert_True(matrix[4][4] == 5.0f, "matrix[4][4] == 5.0f");
	UtAssert_True(matrix[5][4] == 6.0f, "matrix[5][4] == 6.0f");
    
	UtAssert_True(matrix[0][5] == 1.0f, "matrix[0][5] == 1.0f");
	UtAssert_True(matrix[1][5] == 2.0f, "matrix[1][5] == 2.0f");
	UtAssert_True(matrix[2][5] == 3.0f, "matrix[2][5] == 3.0f");
	UtAssert_True(matrix[3][5] == 4.0f, "matrix[3][5] == 4.0f");
	UtAssert_True(matrix[4][5] == 5.0f, "matrix[4][5] == 5.0f");
	UtAssert_True(matrix[5][5] == 6.0f, "matrix[5][5] == 6.0f");

	UtAssert_True(matrix[0][6] == 1.0f, "matrix[0][6] == 1.0f");
	UtAssert_True(matrix[1][6] == 2.0f, "matrix[1][6] == 2.0f");
	UtAssert_True(matrix[2][6] == 3.0f, "matrix[2][6] == 3.0f");
	UtAssert_True(matrix[3][6] == 4.0f, "matrix[3][6] == 4.0f");
	UtAssert_True(matrix[4][6] == 5.0f, "matrix[4][6] == 5.0f");
	UtAssert_True(matrix[5][6] == 6.0f, "matrix[5][6] == 6.0f");

	UtAssert_True(matrix[0][7] == 1.0f, "matrix[0][7] == 1.0f");
	UtAssert_True(matrix[1][7] == 2.0f, "matrix[1][7] == 2.0f");
	UtAssert_True(matrix[2][7] == 3.0f, "matrix[2][7] == 3.0f");
	UtAssert_True(matrix[3][7] == 4.0f, "matrix[3][7] == 4.0f");
	UtAssert_True(matrix[4][7] == 5.0f, "matrix[4][7] == 5.0f");
	UtAssert_True(matrix[5][7] == 6.0f, "matrix[5][7] == 6.0f");

	UtAssert_True(matrix[0][8] == 1.0f, "matrix[0][8] == 1.0f");
	UtAssert_True(matrix[1][8] == 2.0f, "matrix[1][8] == 2.0f");
	UtAssert_True(matrix[2][8] == 3.0f, "matrix[2][8] == 3.0f");
	UtAssert_True(matrix[3][8] == 4.0f, "matrix[3][8] == 4.0f");
	UtAssert_True(matrix[4][8] == 5.0f, "matrix[4][8] == 5.0f");
	UtAssert_True(matrix[5][8] == 6.0f, "matrix[5][8] == 6.0f");

	UtAssert_True(matrix[0][9] == 1.0f, "matrix[0][9] == 1.0f");
	UtAssert_True(matrix[1][9] == 2.0f, "matrix[1][9] == 2.0f");
	UtAssert_True(matrix[2][9] == 3.0f, "matrix[2][9] == 3.0f");
	UtAssert_True(matrix[3][9] == 4.0f, "matrix[3][9] == 4.0f");
	UtAssert_True(matrix[4][9] == 5.0f, "matrix[4][9] == 5.0f");
	UtAssert_True(matrix[5][9] == 6.0f, "matrix[5][9] == 6.0f");

}


void Test_Matrix6F10_IndexOutOfBounds(void)
{
    /* Note row column reversed in constructor */
	math::Matrix6F10 matrix(
			{1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
			{2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f},
			{3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f},
			{4.0f, 4.0f, 4.0f, 4.0f, 4.0f, 4.0f, 4.0f, 4.0f, 4.0f, 4.0f},
			{5.0f, 5.0f, 5.0f, 5.0f, 5.0f, 5.0f, 5.0f, 5.0f, 5.0f, 5.0f},
            {6.0f, 6.0f, 6.0f, 6.0f, 6.0f, 6.0f, 6.0f, 6.0f, 6.0f, 6.0f}
	);

	/* Verify results */
	UtAssert_True(isnan(matrix[6][0]), "matrix[6][0] == NAN");
	UtAssert_True(isnan(matrix[0][10]), "matrix[0][10] == NAN");
}


void Test_Matrix6F10_Mult_10F(void)
{
	math::Matrix6F10 matrixA(
			{1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
			{2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f},
			{3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f},
			{4.0f, 4.0f, 4.0f, 4.0f, 4.0f, 4.0f, 4.0f, 4.0f, 4.0f, 4.0f},
			{5.0f, 5.0f, 5.0f, 5.0f, 5.0f, 5.0f, 5.0f, 5.0f, 5.0f, 5.0f},
            {6.0f, 6.0f, 6.0f, 6.0f, 6.0f, 6.0f, 6.0f, 6.0f, 6.0f, 6.0f}
	);
    
    math::Vector10F vectorB(
            {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f}
    );
    
    math::Vector6F result;
    result.Zero();
    
    result = matrixA * vectorB;

    UtAssert_True(result[0] == 55.0f, "result[0] == 55.0f");
	UtAssert_True(result[1] == 110.0f, "result[1] == 110.0f");
	UtAssert_True(result[2] == 165.0f, "result[2] == 165.0f");
	UtAssert_True(result[3] == 220.0f, "result[3] == 220.0f");
	UtAssert_True(result[4] == 275.0f, "result[4] == 275.0f");
	UtAssert_True(result[5] == 330.0f, "result[5] == 330.0f");
}


/* Auto generated with GNU Octave, version 4.0.0 */
void Test_Matrix6F10_Mult_10F_Random(void)
{
    int i, j = 0;

    math::Matrix6F10 A (
        {-0.840984, 0.973113, 0.231097, -0.592365, -0.399408, -0.983450, -0.848306, 0.731534, 0.125883, 0.951308},
        {-0.281925, 0.205710, 0.361599, 0.186294, 0.988897, 0.161644, -0.359990, 0.959895, 0.780259, 0.058650},
        {-0.658721, 0.668677, -0.634063, 0.890036, 0.291497, 0.919433, 0.477044, 0.130790, 0.643379, -0.367587},
        {0.733821, 0.839267, 0.091494, -0.994008, 0.285287, 0.192680, -0.231401, -0.390360, 0.854749, 0.871344},
        {0.421438, 0.078334, -0.210299, -0.583971, -0.316985, -0.360131, -0.082486, 0.514749, 0.465883, 0.362029},
        {-0.485036, 0.191404, -0.073368, -0.197783, 0.471243, 0.964207, -0.668750, 0.493518, -0.523701, 0.974728}
    );

    math::Vector10F B (
        {-0.609325,0.426572,0.540668,-0.164500,-0.220360,0.214177,0.690519,0.135890,-0.254668,0.081291}
    );

    math::Vector6F expected (
        {0.586218,-0.070978,0.483524,-0.257422,-0.324522,0.290613}
    );

    math::Vector6F result;
    result.Zero();

    result = A * B;

    for(i = 0; i < 6; i++)
    {
        UtAssert_True(fabs(result[i] - expected[i]) < 0.00001f, "fabs(result[i] - expected[i]) < 0.00001f");

    }
}


void Test_Matrix6F10_Mult_10F6(void)
{
	math::Matrix6F10 matrixA(
			{1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
			{2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f},
			{3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f},
			{4.0f, 4.0f, 4.0f, 4.0f, 4.0f, 4.0f, 4.0f, 4.0f, 4.0f, 4.0f},
			{5.0f, 5.0f, 5.0f, 5.0f, 5.0f, 5.0f, 5.0f, 5.0f, 5.0f, 5.0f},
            {6.0f, 6.0f, 6.0f, 6.0f, 6.0f, 6.0f, 6.0f, 6.0f, 6.0f, 6.0f}
	);
    
    math::Matrix10F6 matrixB(
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
    
    math::Matrix6F6 result;
    result.Zero();
    
    result = matrixA * matrixB;

    UtAssert_True(result[0][0] == 10.0f, "result[0][0] == 10.0f");
	UtAssert_True(result[1][0] == 20.0f, "result[1][0] == 20.0f");
	UtAssert_True(result[2][0] == 30.0f, "result[2][0] == 30.0f");
	UtAssert_True(result[3][0] == 40.0f, "result[3][0] == 40.0f");
	UtAssert_True(result[4][0] == 50.0f, "result[4][0] == 50.0f");
	UtAssert_True(result[5][0] == 60.0f, "result[5][0] == 60.0f");

    UtAssert_True(result[0][1] == 20.0f, "result[0][1] == 20.0f");
	UtAssert_True(result[1][1] == 40.0f, "result[1][1] == 40.0f");
	UtAssert_True(result[2][1] == 60.0f, "result[2][1] == 60.0f");
	UtAssert_True(result[3][1] == 80.0f, "result[3][1] == 80.0f");
	UtAssert_True(result[4][1] == 100.0f, "result[4][1] == 100.0f");
	UtAssert_True(result[5][1] == 120.0f, "result[5][1] == 120.0f");

    UtAssert_True(result[0][2] == 30.0f, "result[0][2] == 30.0f");
	UtAssert_True(result[1][2] == 60.0f, "result[1][2] == 60.0f");
	UtAssert_True(result[2][2] == 90.0f, "result[2][2] == 90.0f");
	UtAssert_True(result[3][2] == 120.0f, "result[3][2] == 120.0f");
	UtAssert_True(result[4][2] == 150.0f, "result[4][2] == 150.0f");
	UtAssert_True(result[5][2] == 180.0f, "result[5][2] == 180.0f");

    UtAssert_True(result[0][3] == 40.0f, "result[0][3] == 40.0f");
	UtAssert_True(result[1][3] == 80.0f, "result[1][3] == 80.0f");
	UtAssert_True(result[2][3] == 120.0f, "result[2][3] == 120.0f");
	UtAssert_True(result[3][3] == 160.0f, "result[3][3] == 160.0f");
	UtAssert_True(result[4][3] == 200.0f, "result[4][3] == 200.0f");
	UtAssert_True(result[5][3] == 240.0f, "result[5][3] == 240.0f");

    UtAssert_True(result[0][4] == 50.0f, "result[0][4] == 50.0f");
	UtAssert_True(result[1][4] == 100.0f, "result[1][4] == 100.0f");
	UtAssert_True(result[2][4] == 150.0f, "result[2][4] == 150.0f");
	UtAssert_True(result[3][4] == 200.0f, "result[3][4] == 200.0f");
	UtAssert_True(result[4][4] == 250.0f, "result[4][4] == 250.0f");
	UtAssert_True(result[5][4] == 300.0f, "result[5][4] == 300.0f");

    UtAssert_True(result[0][5] == 60.0f, "result[0][5] == 60.0f");
	UtAssert_True(result[1][5] == 120.0f, "result[1][5] == 120.0f");
	UtAssert_True(result[2][5] == 180.0f, "result[2][5] == 180.0f");
	UtAssert_True(result[3][5] == 240.0f, "result[3][5] == 240.0f");
	UtAssert_True(result[4][5] == 300.0f, "result[4][5] == 300.0f");
	UtAssert_True(result[5][5] == 360.0f, "result[5][5] == 360.0f");
}


/* Auto generated with GNU Octave, version 4.0.0 */
void Test_Matrix6F10_Mult_10F6_Random(void)
{
    int i, j = 0;

    math::Matrix6F10 A (
        {0.639221, -0.332555, 0.524345, 0.904231, -0.812395, -0.471591, 0.759046, -0.771042, 0.836051, 0.174207},
        {0.895421, 0.079103, 0.016970, -0.921275, 0.181456, 0.634541, 0.086779, 0.429518, -0.683895, -0.584413},
        {-0.148456, -0.481747, -0.099922, 0.429846, -0.689183, 0.205039, 0.822007, 0.414243, -0.216377, 0.399581},
        {-0.768467, 0.414903, -0.104755, 0.115413, -0.406569, 0.355307, -0.904909, 0.886455, -0.871238, 0.356206},
        {0.350564, 0.552931, -0.681354, -0.110625, 0.536784, -0.230404, -0.222697, 0.141824, -0.555939, -0.795179},
        {0.636405, 0.498039, -0.834764, 0.968031, 0.757824, -0.805743, 0.063850, -0.923719, 0.404551, -0.213530}
    );

    math::Matrix10F6 B (
        {-0.381144, -0.271826, 0.172603, -0.865579, -0.708032, 0.038134},
        {-0.045749, 0.820523, 0.590842, 0.552114, 0.928299, 0.157700},
        {0.218084, 0.022665, 0.213973, -0.445533, -0.503422, -0.747757},
        {0.795077, 0.340141, -0.065983, 0.506077, -0.026816, -0.581408},
        {0.391534, -0.805676, -0.982172, -0.794822, -0.311222, -0.657830},
        {-0.442861, 0.571892, -0.942911, -0.853084, 0.631334, 0.230508},
        {-0.258927, -0.930408, -0.539865, -0.264151, -0.780827, -0.052265},
        {-0.892862, -0.265056, -0.716832, 0.469330, 0.975240, 0.092298},
        {0.142047, -0.062837, -0.023449, -0.820682, 0.083207, 0.950383},
        {-0.597209, 0.046159, -0.554035, -0.901614, 0.046011, 0.635339}
    );

    math::Matrix6F6 expected (
        {1.002250, -0.288693, 1.235760, -0.870466, -2.361462, 0.274247},
        {-1.437752, -0.453366, -0.525747, -0.623885, 0.067069, -0.389688},
        {-0.814121, -0.381027, -0.833464, 0.292029, -0.196858, 0.287297},
        {-0.867368, 1.795132, -0.177062, 2.068288, 2.844725, -0.075876},
        {0.243698, -0.091075, 0.410901, 1.317908, 0.527884, -0.740685},
        {1.969018, -0.375195, 0.913545, 0.081079, -1.265397, -0.359840}
    );

    math::Matrix6F6 result;
    result.Zero();

    result = A * B;

    for(i = 0; i < 6; i++)
    {
        for(j = 0; j < 6; j++)
        {
            UtAssert_True(fabs(result[i][j] - expected[i][j]) < 0.00001f, "fabs(result[i][j] - expected[i][j]) < 0.00001f");

        }
    }
}


void Test_Matrix6F10_Mult_10F10(void)
{
	math::Matrix6F10 matrixA(
			{1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
			{2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f},
			{3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f},
			{4.0f, 4.0f, 4.0f, 4.0f, 4.0f, 4.0f, 4.0f, 4.0f, 4.0f, 4.0f},
			{5.0f, 5.0f, 5.0f, 5.0f, 5.0f, 5.0f, 5.0f, 5.0f, 5.0f, 5.0f},
            {6.0f, 6.0f, 6.0f, 6.0f, 6.0f, 6.0f, 6.0f, 6.0f, 6.0f, 6.0f}
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
    
    math::Matrix6F10 result;
    result.Zero();
    
    result = matrixA * matrixB;

    UtAssert_True(result[0][0] == 10.0f, "result[0][0] == 10.0f");
	UtAssert_True(result[1][0] == 20.0f, "result[1][0] == 20.0f");
	UtAssert_True(result[2][0] == 30.0f, "result[2][0] == 30.0f");
	UtAssert_True(result[3][0] == 40.0f, "result[3][0] == 40.0f");
	UtAssert_True(result[4][0] == 50.0f, "result[4][0] == 50.0f");
	UtAssert_True(result[5][0] == 60.0f, "result[5][0] == 60.0f");

    UtAssert_True(result[0][1] == 20.0f, "result[0][1] == 20.0f");
	UtAssert_True(result[1][1] == 40.0f, "result[1][1] == 40.0f");
	UtAssert_True(result[2][1] == 60.0f, "result[2][1] == 60.0f");
	UtAssert_True(result[3][1] == 80.0f, "result[3][1] == 80.0f");
	UtAssert_True(result[4][1] == 100.0f, "result[4][1] == 100.0f");
	UtAssert_True(result[5][1] == 120.0f, "result[5][1] == 120.0f");

    UtAssert_True(result[0][2] == 30.0f, "result[0][2] == 30.0f");
	UtAssert_True(result[1][2] == 60.0f, "result[1][2] == 60.0f");
	UtAssert_True(result[2][2] == 90.0f, "result[2][2] == 90.0f");
	UtAssert_True(result[3][2] == 120.0f, "result[3][2] == 120.0f");
	UtAssert_True(result[4][2] == 150.0f, "result[4][2] == 150.0f");
	UtAssert_True(result[5][2] == 180.0f, "result[5][2] == 180.0f");

    UtAssert_True(result[0][3] == 40.0f, "result[0][3] == 40.0f");
	UtAssert_True(result[1][3] == 80.0f, "result[1][3] == 80.0f");
	UtAssert_True(result[2][3] == 120.0f, "result[2][3] == 120.0f");
	UtAssert_True(result[3][3] == 160.0f, "result[3][3] == 160.0f");
	UtAssert_True(result[4][3] == 200.0f, "result[4][3] == 200.0f");
	UtAssert_True(result[5][3] == 240.0f, "result[5][3] == 240.0f");

    UtAssert_True(result[0][4] == 50.0f, "result[0][4] == 50.0f");
	UtAssert_True(result[1][4] == 100.0f, "result[1][4] == 100.0f");
	UtAssert_True(result[2][4] == 150.0f, "result[2][4] == 150.0f");
	UtAssert_True(result[3][4] == 200.0f, "result[3][4] == 200.0f");
	UtAssert_True(result[4][4] == 250.0f, "result[4][4] == 250.0f");
	UtAssert_True(result[5][4] == 300.0f, "result[5][4] == 300.0f");

    UtAssert_True(result[0][5] == 60.0f, "result[0][5] == 60.0f");
	UtAssert_True(result[1][5] == 120.0f, "result[1][5] == 120.0f");
	UtAssert_True(result[2][5] == 180.0f, "result[2][5] == 180.0f");
	UtAssert_True(result[3][5] == 240.0f, "result[3][5] == 240.0f");
	UtAssert_True(result[4][5] == 300.0f, "result[4][5] == 300.0f");
	UtAssert_True(result[5][5] == 360.0f, "result[5][5] == 360.0f");

    UtAssert_True(result[0][6] == 70.0f, "result[0][6] == 70.0f");
	UtAssert_True(result[1][6] == 140.0f, "result[1][6] == 140.0f");
	UtAssert_True(result[2][6] == 210.0f, "result[2][6] == 210.0f");
	UtAssert_True(result[3][6] == 280.0f, "result[3][6] == 280.0f");
	UtAssert_True(result[4][6] == 350.0f, "result[4][6] == 350.0f");
	UtAssert_True(result[5][6] == 420.0f, "result[5][6] == 420.0f");

    UtAssert_True(result[0][7] == 80.0f, "result[0][7] == 80.0f");
	UtAssert_True(result[1][7] == 160.0f, "result[1][7] == 160.0f");
	UtAssert_True(result[2][7] == 240.0f, "result[2][7] == 240.0f");
	UtAssert_True(result[3][7] == 320.0f, "result[3][7] == 320.0f");
	UtAssert_True(result[4][7] == 400.0f, "result[4][7] == 400.0f");
	UtAssert_True(result[5][7] == 480.0f, "result[5][7] == 480.0f");

    UtAssert_True(result[0][8] == 90.0f, "result[0][8] == 90.0f");
	UtAssert_True(result[1][8] == 180.0f, "result[1][8] == 180.0f");
	UtAssert_True(result[2][8] == 270.0f, "result[2][8] == 270.0f");
	UtAssert_True(result[3][8] == 360.0f, "result[3][8] == 360.0f");
	UtAssert_True(result[4][8] == 450.0f, "result[4][8] == 450.0f");
	UtAssert_True(result[5][8] == 540.0f, "result[5][8] == 540.0f");

    UtAssert_True(result[0][9] == 100.0f, "result[0][9] == 100.0f");
	UtAssert_True(result[1][9] == 200.0f, "result[1][9] == 200.0f");
	UtAssert_True(result[2][9] == 300.0f, "result[2][9] == 300.0f");
	UtAssert_True(result[3][9] == 400.0f, "result[3][9] == 400.0f");
	UtAssert_True(result[4][9] == 500.0f, "result[4][9] == 500.0f");
	UtAssert_True(result[5][9] == 600.0f, "result[5][9] == 600.0f");
}


/* Auto generated with GNU Octave, version 4.0.0 */
void Test_Matrix6F10_Mult_10F10_Random(void)
{
    int i, j = 0;

    math::Matrix6F10 A (
        {-0.985106, 0.149251, -0.525228, -0.746840, 0.540400, 0.604984, 0.917581, -0.905147, -0.757948, -0.778677},
        {0.460761, 0.998735, 0.834680, 0.931717, 0.570759, 0.571421, -0.274457, -0.823448, 0.246636, 0.461387},
        {-0.601506, 0.125795, -0.850045, -0.098929, 0.779013, -0.565675, 0.716154, 0.415574, -0.366667, -0.254160},
        {-0.018703, -0.196943, 0.857215, -0.508156, 0.693110, 0.094308, -0.949009, 0.748351, -0.722448, -0.277619},
        {0.646784, 0.531659, 0.448633, 0.843197, -0.302866, -0.276374, 0.903036, 0.865270, 0.780063, -0.541709},
        {0.985624, 0.401786, 0.553626, -0.618882, 0.654318, 0.210892, -0.161548, -0.274362, 0.709248, 0.125635}
    );

    math::Matrix10F10 B (
        {0.056608, 0.274871, -0.046819, -0.813066, 0.986111, 0.335951, 0.917015, 0.137310, 0.226083, 0.663870},
        {0.593007, -0.609591, -0.141287, 0.670066, -0.469759, -0.782826, -0.731671, 0.382947, -0.271135, 0.521052},
        {-0.650611, -0.842965, -0.599648, 0.841959, -0.984556, -0.219122, -0.329756, 0.598969, 0.956279, -0.359166},
        {-0.737220, -0.272613, 0.176633, -0.409351, -0.418883, 0.570437, 0.736666, 0.332157, 0.523382, 0.096012},
        {0.854599, 0.225818, 0.916677, -0.951957, 0.817437, 0.936081, 0.853756, 0.565133, -0.447002, -0.025980},
        {-0.182945, -0.838851, 0.128101, 0.079334, -0.173034, -0.575390, -0.893010, 0.552649, -0.302132, 0.258694},
        {0.915012, -0.063288, -0.194073, -0.354909, -0.837862, 0.279630, 0.977521, -0.647274, 0.943854, 0.029454},
        {0.329734, 0.214980, 0.407659, -0.971684, 0.295265, -0.391678, -0.028650, -0.534762, 0.014944, -0.557668},
        {-0.505960, 0.222287, 0.620794, -0.050822, 0.111303, 0.340469, 0.081970, 0.513180, 0.860390, 0.128740},
        {0.858641, 0.314791, 0.794351, -0.930781, -0.635014, -0.903403, 0.457153, 0.968926, 0.857688, 0.391553}
    );

    math::Matrix6F10 expected (
        {1.532219, -0.767135, -0.855200, 1.615179, -0.500477, 0.455547, -0.963639, -1.254362, -2.048132, -0.187484},
        {-0.479633, -1.749813, 0.334972, 0.573503, -1.137903, -0.160033, 0.066353, 3.085119, 1.028011, 1.412477},
        {2.195363, 1.034442, 0.745192, -1.291089, 0.604036, 1.026239, 1.275943, -1.569013, -1.063377, -0.561923},
        {-0.220302, -0.418792, -0.107414, 0.065436, 1.105191, -0.288928, -1.157788, 0.284830, -1.479607, -1.112309},
        {-0.518100, -0.459179, -0.307086, -0.567371, -0.676926, 0.727870, 1.339802, -0.654490, 2.163097, -0.005021},
        {0.421451, -0.152681, 0.542207, -0.247892, 1.049234, 0.223663, 0.307201, 1.638550, 0.524613, 0.931717}
    );

    math::Matrix6F10 result;
    result.Zero();

    result = A * B;

    for(i = 0; i < 6; i++)
    {
        for(j = 0; j < 10; j++)
        {
            UtAssert_True(fabs(result[i][j] - expected[i][j]) < 0.00001f, "fabs(result[i][j] - expected[i][j]) < 0.00001f");

        }
    }
}



