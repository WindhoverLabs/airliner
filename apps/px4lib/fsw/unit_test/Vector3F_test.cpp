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

#include "Vector3F_test.hpp"
#include <math/Vector3F.hpp>

#include "utassert.h"
#include <float.h>


void Test_Vector3F_Constructor(void)
{
	math::Vector3F vector(1.0f, 2.0f, 3.0f);

	/* Verify results */
	UtAssert_True(vector[0] == 1.0f, "vector[0] == 1.0f");
	UtAssert_True(vector[1] == 2.0f, "vector[1] == 2.0f");
	UtAssert_True(vector[2] == 3.0f, "vector[2] == 3.0f");
}


void Test_Vector3F_IndexOutOfBounds(void)
{
	math::Vector3F vector(1.0f, 2.0f, 3.0f);

	/* Verify results */
	UtAssert_True(vector[0] == 1.0f, "vector[0] == 1.0f");
	UtAssert_True(vector[1] == 2.0f, "vector[1] == 2.0f");
	UtAssert_True(vector[2] == 3.0f, "vector[2] == 3.0f");
	UtAssert_True(isnan(vector[3]), "vector[3] == nan");
}


void Test_Vector3F_Length(void)
{
	math::Vector3F vector(1.5f, 2.5f, 3.5f);
	float expectedLength = sqrtf((1.5f)*(1.5f) + (2.5f)*(2.5f) + (3.5f)*(3.5f));
    float actualLength = 4.555217f;

	/* Verify results */
    UtAssert_True(fabs(expectedLength - actualLength) <= FLT_EPSILON, "vector.Length() == expectedLength");
}


void Test_Vector3F_Zero(void)
{
	math::Vector3F vector(1.5f, 2.5f, 3.5f);
	vector.Zero();

	/* Verify results */
    UtAssert_True(vector[0] == 0.0f, "vector[0] == 0.0f");
    UtAssert_True(vector[1] == 0.0f, "vector[1] == 0.0f");
    UtAssert_True(vector[2] == 0.0f, "vector[2] == 0.0f");
}



void Test_Vector3F_ArrayOperator(void)
{
	math::Vector3F vector(1.5f, 2.5f, 3.5f);
	float nanResult = vector[3];

    UtAssert_True(vector[0] == 1.5f, "vector[0] == 1.5f");
    UtAssert_True(vector[1] == 2.5f, "vector[1] == 2.5f");
    UtAssert_True(vector[2] == 3.5f, "vector[2] == 3.5f");
    UtAssert_True(isnan(vector[3]), "vector[3] == NAN");

    UtAssert_True(1.5f == vector[0], "1.5f == vector[0]");
    UtAssert_True(2.5f == vector[1], "2.5f == vector[1]");
    UtAssert_True(3.5f == vector[2], "3.5f == vector[2]");
    UtAssert_True(isnan(nanResult), "NAN == vector[3]");

}


void Test_Vector3F_AdditionOperator(void)
{
	/* vectorActual = vectorA + vectorB */
	math::Vector3F vectorA(1.5f, 2.5f, 3.5f);
	math::Vector3F vectorB(10.5f, 20.5f, 30.5f);
	math::Vector3F vectorActual = vectorA + vectorB;

	/* Verify results */
	UtAssert_True(vectorActual[0] == 12.0, "vectorActual[0] == vectorA[0] + vectorB[0]");
	UtAssert_True(vectorActual[1] == 23.0, "vectorActual[1] == vectorA[1] + vectorB[1]");
	UtAssert_True(vectorActual[2] == 34.0, "vectorActual[2] == vectorA[2] + vectorB[2]");
}


void Test_Vector3F_SubtractionOperator(void)
{
	/* vectorActual = vectorA - vectorB */
	math::Vector3F vectorA(1.5f, 2.5f, 3.5f);
	math::Vector3F vectorB(10.5f, 20.5f, 30.5f);
	math::Vector3F vectorActual = vectorA - vectorB;

	/* Verify results */
	UtAssert_True(vectorActual[0] == -9.0, "vectorActual[0] == vectorA[0] - vectorB[0]");
	UtAssert_True(vectorActual[1] == -18.0, "vectorActual[1] == vectorA[1] - vectorB[1]");
	UtAssert_True(vectorActual[2] == -27.0, "vectorActual[2] == vectorA[2] - vectorB[2]");
}


void Test_Vector3F_CrossProductOperator(void)
{
	/* vectorActual = vectorA % vectorB */
	math::Vector3F vectorA(1.5f, 2.5f, 3.5f);
	math::Vector3F vectorB(10.5f, 20.5f, 30.5f);
	/* This is actual flight data collected from PX4 code */
	math::Vector3F vectorRzA(-0.019039f, -0.024143f, 0.999527f);
	math::Vector3F vectorRzB(-0.019038f, -0.024110f, 0.999528f);
	math::Vector3F vectorRzC(-0.019047f, -0.024071f, 0.999529f);
	math::Vector3F vectorRspzA(-0.019387f, -0.022884f, 0.999550f);
	math::Vector3F vectorRspzB(-0.019367f, -0.023088f, 0.999546f);
	math::Vector3F vectorRspzC(-0.019475f, -0.022664f, 0.999553f);
	math::Vector3F vectorRspxA(0.999726f, -0.023411f, -0.000000f);
	math::Vector3F vectorRspxB(0.999726f, -0.023411f, -0.000000f);
	math::Vector3F vectorRspxC(0.999726f, -0.023411f, -0.000000f);
	math::Vector3F vectorRrpxA(0.028136f, 0.999604f, 0.000000f);
	math::Vector3F vectorRrpxB(0.028086f, 0.999606f, 0.000000f);
	math::Vector3F vectorRrpxC(0.028102f, 0.999605f, 0.000000f);

	math::Vector3F vectorABActual = vectorA % vectorB;
	math::Vector3F vectorRPAActual = vectorRzA % vectorRspzA;
	math::Vector3F vectorRPBActual = vectorRzB % vectorRspzB;
	math::Vector3F vectorRPCActual = vectorRzC % vectorRspzC;
	math::Vector3F vectorRRRAActual = vectorRrpxA % vectorRspxA;
	math::Vector3F vectorRRRBActual = vectorRrpxB % vectorRspxB;
	math::Vector3F vectorRRRCActual = vectorRrpxC % vectorRspxC;

	math::Vector3F vectorABExpected;
	math::Vector3F vectorRPAExpected;
	math::Vector3F vectorRPBExpected;
	math::Vector3F vectorRPCExpected;
	math::Vector3F vectorRRRAExpected;
	math::Vector3F vectorRRRBExpected;
	math::Vector3F vectorRRRCExpected;

	/* Verify results */
	vectorABExpected[0] = 4.5f;
	vectorABExpected[1] = -9.0f;
	vectorABExpected[2] = 4.5f;
	vectorRPAExpected[0] = -0.001259f;
	vectorRPAExpected[1] = -0.000347f;
	vectorRPAExpected[2] = -0.000032f;
	vectorRPBExpected[0] = -0.001022f;
	vectorRPBExpected[1] = -0.000329f;
	vectorRPBExpected[2] = -0.000027f;
	vectorRPCExpected[0] = -0.001407f;
	vectorRPCExpected[1] = -0.000427f;
	vectorRPCExpected[2] = -0.000037f;
	vectorRRRAExpected[0] = 0.000000f;
	vectorRRRAExpected[1] = 0.000000f;
	vectorRRRAExpected[2] = -0.999989f;
	vectorRRRBExpected[0] = 0.000000f;
	vectorRRRBExpected[1] = 0.000000f;
	vectorRRRBExpected[2] = -0.999989f;
	vectorRRRCExpected[0] = 0.000000f;
	vectorRRRCExpected[1] = 0.000000f;
	vectorRRRCExpected[2] = -0.999989f;

	UtAssert_True(fabs(vectorABExpected[0]-vectorABActual[0]) < 0.00001, "[0] vectorExpected == vectorA % vectorB");
	UtAssert_True(fabs(vectorABExpected[1]-vectorABActual[1]) < 0.00001, "[1] vectorExpected == vectorA % vectorB");
	UtAssert_True(fabs(vectorABExpected[2]-vectorABActual[2]) < 0.00001, "[2] vectorExpected == vectorA % vectorB");
	UtAssert_True(fabs(vectorRPAExpected[0]-vectorRPAActual[0]) < 0.00001, "[0] vectorRPAExpected == vectorRzA % vectorRspzA");
	UtAssert_True(fabs(vectorRPAExpected[1]-vectorRPAActual[1]) < 0.00001, "[1] vectorRPAExpected == vectorRzA % vectorRspzA");
	UtAssert_True(fabs(vectorRPAExpected[2]-vectorRPAActual[2]) < 0.00001, "[2] vectorRPAExpected == vectorRzA % vectorRspzA");
	UtAssert_True(fabs(vectorRPBExpected[0]-vectorRPBActual[0]) < 0.00001, "[0] vectorRPBExpected == vectorRzB % vectorRspzB");
	UtAssert_True(fabs(vectorRPBExpected[1]-vectorRPBActual[1]) < 0.00001, "[1] vectorRPBExpected == vectorRzB % vectorRspzB");
	UtAssert_True(fabs(vectorRPBExpected[2]-vectorRPBActual[2]) < 0.00001, "[2] vectorRPBExpected == vectorRzB % vectorRspzB");
	UtAssert_True(fabs(vectorRPCExpected[0]-vectorRPCActual[0]) < 0.00001, "[0] vectorRPCExpected == vectorRzC % vectorRspzC");
	UtAssert_True(fabs(vectorRPCExpected[1]-vectorRPCActual[1]) < 0.00001, "[1] vectorRPCExpected == vectorRzC % vectorRspzC");
	UtAssert_True(fabs(vectorRPCExpected[2]-vectorRPCActual[2]) < 0.00001, "[2] vectorRPCExpected == vectorRzC % vectorRspzC");
	UtAssert_True(fabs(vectorRRRAExpected[0]-vectorRRRAActual[0]) < 0.00001, "[2] vectorRRRAExpected == vectorRrpxA % vectorRspxA");
	UtAssert_True(fabs(vectorRRRBExpected[0]-vectorRRRBActual[0]) < 0.00001, "[2] vectorRRRBExpected == vectorRrpxB % vectorRspxB");
	UtAssert_True(fabs(vectorRRRCExpected[0]-vectorRRRCActual[0]) < 0.00001, "[2] vectorRRRCExpected == vectorRrpxC % vectorRspxC");
}


void Test_Vector3F_ScalarMultiplyOperator(void)
{
	/* actual = vectorA * scalar */
	math::Vector3F vectorA(1.5f, 2.5f, 3.5f);
	float scalar = 10.5f;
	math::Vector3F vectorActual = vectorA * scalar;
	math::Vector3F vectorExpected;

	/* Verify results */
	vectorExpected[0] = 15.75;
	vectorExpected[1] = 26.25;
	vectorExpected[2] = 36.75;

	UtAssert_True(vectorExpected[0] == vectorActual[0], "[0] vectorExpected == vectorA * scalar");
	UtAssert_True(vectorExpected[1] == vectorActual[1], "[1] vectorExpected == vectorA * scalar");
	UtAssert_True(vectorExpected[2] == vectorActual[2], "[2] vectorExpected == vectorA * scalar");
}


void Test_Vector3F_ScalarDivOperator(void)
{
	/* actual = vectorA / scalar */
	math::Vector3F vectorA(1.5f, 2.5f, 3.5f);
	float scalar = 10.5f;
	math::Vector3F vectorActual = vectorA / scalar;
	math::Vector3F vectorExpected;

	/* Verify results */
	vectorExpected[0] = 0.142857f;
	vectorExpected[1] = 0.238095f;
	vectorExpected[2] = 0.333333f;

	UtAssert_True(fabs(vectorActual[0] - vectorExpected[0]) <= 0.00001, "[0] vectorExpected == vectorA / scalar");
	UtAssert_True(fabs(vectorActual[1] - vectorExpected[1]) <= 0.00001, "[1] vectorExpected == vectorA / scalar");
	UtAssert_True(fabs(vectorActual[2] - vectorExpected[2]) <= 0.00001, "[2] vectorExpected == vectorA / scalar");
}


void Test_Vector3F_DotProductOperator(void)
{
	/* actual = vectorA * vectorB */
	math::Vector3F vectorA(1.5f, 2.5f, 3.5f);
	math::Vector3F vectorB(10.5f, 20.5f, 30.5f);
	/* This is actual flight data collected from PX4 code */
	math::Vector3F vectorRzA(-0.019784f, -0.024276f, 0.999510f);
	math::Vector3F vectorRzB(-0.019808f, -0.024276f, 0.999509f);
	math::Vector3F vectorRzC(-0.019828f, -0.024274f, 0.999509f);
	math::Vector3F vectorRspzA(-0.019997f, -0.025087f, 0.999485f);
	math::Vector3F vectorRspzB(-0.019883f, -0.024780f, 0.999495f);
	math::Vector3F vectorRspzC(-0.019977f, -0.024787f, 0.999493f);

	float actualAB = vectorA * vectorB;
	float actualeRzcosA = vectorRzA * vectorRspzA;
	float actualeRzcosB = vectorRzB * vectorRspzB;
	float actualeRzcosC = vectorRzC * vectorRspzC;

	float expectedAB = 173.75;
	float expectedeRzcosA = 1.000000;
	float expectedeRzcosB = 1.000000;
	float expectedeRzcosC = 1.000000;
    
	/* Verify results */
	UtAssert_True(fabs(actualeRzcosA - expectedeRzcosA) <= 0.00001f, "expected == vectorRzA dot vectorRspzA");
	UtAssert_True(fabs(actualeRzcosB - expectedeRzcosB) <= 0.00001f, "expected == vectorRzB dot vectorRspzB");
	UtAssert_True(fabs(actualeRzcosC - expectedeRzcosC) <= 0.00001f, "expected == vectorRzC dot vectorRspzC");
}


void Test_Vector3F_EMult(void)
{
	/* actual = vectorA / scalar */
	math::Vector3F vectorA(1.5f, 2.5f, 3.5f);
	math::Vector3F vectorB(10.5f, 20.5f, 30.5f);
	math::Vector3F vectorActual = vectorA.EMult(vectorB);
	math::Vector3F vectorExpected;

	/* Verify results */
	vectorExpected[0] = 15.75f;
	vectorExpected[1] = 51.25f;
	vectorExpected[2] = 106.75f;

	UtAssert_True(fabs(vectorActual[0] - vectorExpected[0]) <= 0.00001, "[0] vectorExpected == vectorA.EMult(vectorB)");
	UtAssert_True(fabs(vectorActual[1] - vectorExpected[1]) <= 0.00001, "[1] vectorExpected == vectorA.EMult(vectorB)");
	UtAssert_True(fabs(vectorActual[2] - vectorExpected[2]) <= 0.00001, "[2] vectorExpected == vectorA.EMult(vectorB)");
}


void Test_Vector3F_Constrain(void)
{
	/* actual = vectorA / scalar */
	math::Vector3F vectorA(1.5f, 2.5f, 3.5f);
	math::Vector3F vectorB(-1.5f, -2.5f, -3.5f);
	math::Vector3F vectorAExpected;
	math::Vector3F vectorBExpected;

	vectorA.Constrain(0, 0.5, 1.0);
	vectorA.Constrain(1, 1.5, 2.7);
	vectorA.Constrain(2, 2.5, 3.0);

	vectorB.Constrain(0, -1.0, -0.5);
	vectorB.Constrain(1, -2.7, -1.5);
	vectorB.Constrain(2, -3.0, -2.5);

	/* Verify results */
	vectorAExpected[0] = 1.0f;
	vectorAExpected[1] = 2.5f;
	vectorAExpected[2] = 3.0f;

	vectorBExpected[0] = -1.0f;
	vectorBExpected[1] = -2.5f;
	vectorBExpected[2] = -3.0f;

	UtAssert_True(fabs(vectorA[0] - vectorAExpected[0]) <= 0.00001, "[0] vectorAExpected == vectorA.Constrain(0, min, max)");
	UtAssert_True(fabs(vectorA[1] - vectorAExpected[1]) <= 0.00001, "[1] vectorAExpected == vectorA.Constrain(1, min, max)");
	UtAssert_True(fabs(vectorA[2] - vectorAExpected[2]) <= 0.00001, "[2] vectorAExpected == vectorA.Constrain(2, min, max)");

	UtAssert_True(fabs(vectorB[0] - vectorBExpected[0]) <= 0.00001, "[0] vectorBExpected == vectorB.Constrain(0, min, max)");
	UtAssert_True(fabs(vectorB[1] - vectorBExpected[1]) <= 0.00001, "[1] vectorBExpected == vectorB.Constrain(1, min, max)");
	UtAssert_True(fabs(vectorB[2] - vectorBExpected[2]) <= 0.00001, "[2] vectorBExpected == vectorB.Constrain(2, min, max)");
}


/* Auto generated with GNU Octave, version 4.0.0 */
void Test_Vector3F_Add_3F_Random(void)
{
    int i, j = 0;

    math::Vector3F A (
        {-0.985966,-0.370751,0.697822}
    );

    math::Vector3F B (
        {-0.862678,-0.264839,-0.097175}
    );

    math::Vector3F expected (
        {-1.848644,-0.635590,0.600647}
    );

    math::Vector3F result;
    result.Zero();

    result = A + B;

    for(i = 0; i < 3; i++)
    {
        UtAssert_True(fabs(result[i] - expected[i]) < 0.00001f, "fabs(result[i] - expected[i]) < 0.00001f");

    }
}


/* Auto generated with GNU Octave, version 4.0.0 */
void Test_Vector3F_Sub_3F_Random(void)
{
    int i, j = 0;

    math::Vector3F A (
        {0.675745,-0.357198,-0.637360}
    );

    math::Vector3F B (
        {0.972953,0.849521,0.308778}
    );

    math::Vector3F expected (
        {-0.297208,-1.206719,-0.946138}
    );

    math::Vector3F result;
    result.Zero();

    result = A - B;

    for(i = 0; i < 3; i++)
    {
        UtAssert_True(fabs(result[i] - expected[i]) < 0.00001f, "fabs(result[i] - expected[i]) < 0.00001f");

    }
}


