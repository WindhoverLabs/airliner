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

#include "Vector2F_test.hpp"
#include <math/Vector2F.hpp>

#include "utassert.h"
#include <float.h>


void Test_Vector2F_Constructor(void)
{
	math::Vector2F vector(1.0f, 2.0f);

	/* Verify results */
	UtAssert_True(vector[0] == 1.0f, "vector[0] == 1.0f");
	UtAssert_True(vector[1] == 2.0f, "vector[1] == 2.0f");
}


void Test_Vector2F_IndexOutOfBounds(void)
{
	math::Vector2F vector(1.0f, 2.0f);

	/* Verify results */
	UtAssert_True(vector[0] == 1.0f, "vector[0] == 1.0f");
	UtAssert_True(vector[1] == 2.0f, "vector[1] == 2.0f");
	UtAssert_True(isnan(vector[2]), "vector[2] == nan");
}


void Test_Vector2F_Length(void)
{
	math::Vector2F vector(1.5f, 2.5f);
	float expectedLength = sqrtf((1.5f)*(1.5f) + (2.5f)*(2.5f));
    float actualLength = vector.Length();;

	/* Verify results */
    UtAssert_True(fabs(expectedLength - actualLength) <= FLT_EPSILON, "vector.Length() == expectedLength");
}


void Test_Vector2F_Zero(void)
{
	math::Vector2F vector(1.5f, 2.5f);
    vector.Zero();


	/* Verify results */
    UtAssert_True(vector[0] == 0.0f, "vector[0] == 0.0f");
    UtAssert_True(vector[1] == 0.0f, "vector[1] == 0.0f");

}


void Test_Vector2F_ArrayOperator(void)
{
	math::Vector2F vector(1.5f, 2.5f);
	float nanResult = vector[2];

    UtAssert_True(vector[0] == 1.5f, "vector[0] == 1.5f");
    UtAssert_True(vector[1] == 2.5f, "vector[1] == 2.5f");
    UtAssert_True(isnan(vector[2]), "vector[2] == NAN");

    UtAssert_True(1.5f == vector[0], "1.5f == vector[0]");
    UtAssert_True(2.5f == vector[1], "2.5f == vector[1]");
    UtAssert_True(isnan(nanResult), "NAN == vector[3]");

}


void Test_Vector2F_AdditionOperator(void)
{
	/* vectorActual = vectorA + vectorB */
	math::Vector2F vectorA(1.5f, 2.5f);
	math::Vector2F vectorB(10.5f, 20.5f);
	math::Vector2F vectorActual = vectorA + vectorB;

	/* Verify results */
	UtAssert_True(vectorActual[0] == 12.0, "vectorActual[0] == vectorA[0] + vectorB[0]");
	UtAssert_True(vectorActual[1] == 23.0, "vectorActual[1] == vectorA[1] + vectorB[1]");
}


void Test_Vector2F_SubtractionOperator(void)
{
	/* vectorActual = vectorA - vectorB */
	math::Vector2F vectorA(1.5f, 2.5f);
	math::Vector2F vectorB(10.5f, 20.5f);
	math::Vector2F vectorActual = vectorA - vectorB;

	/* Verify results */
	UtAssert_True(vectorActual[0] == -9.0, "vectorActual[0] == vectorA[0] - vectorB[0]");
	UtAssert_True(vectorActual[1] == -18.0, "vectorActual[1] == vectorA[1] - vectorB[1]");
}


void Test_Vector2F_ScalarMultiplyOperator(void)
{
	/* actual = vectorA * scalar */
	math::Vector2F vectorA(1.5f, 2.5f);
	float scalar = 10.5f;
	math::Vector2F vectorActual = vectorA * scalar;
	math::Vector2F vectorExpected;

	/* Verify results */
	vectorExpected[0] = 15.75;
	vectorExpected[1] = 26.25;

	UtAssert_True(vectorExpected[0] == vectorActual[0], "[0] vectorExpected == vectorA * scalar");
	UtAssert_True(vectorExpected[1] == vectorActual[1], "[1] vectorExpected == vectorA * scalar");
}


void Test_Vector2F_ScalarDivOperator(void)
{
	/* actual = vectorA / scalar */
	math::Vector2F vectorA(1.5f, 2.5f);
	float scalar = 10.5f;
	math::Vector2F vectorActual = vectorA / scalar;
	math::Vector2F vectorExpected;

	/* Verify results */
	vectorExpected[0] = 0.142857f;
	vectorExpected[1] = 0.238095f;

	UtAssert_True(fabs(vectorActual[0] - vectorExpected[0]) <= 0.00001, "[0] vectorExpected == vectorA / scalar");
	UtAssert_True(fabs(vectorActual[1] - vectorExpected[1]) <= 0.00001, "[1] vectorExpected == vectorA / scalar");
}


void Test_Vector2F_DotProductOperator(void)
{
	/* actual = vectorA * vectorB */
	math::Vector2F vectorA(1.5f, 2.5f);
	math::Vector2F vectorB(10.5f, 20.5f);

	float actualAB = vectorA * vectorB;

	float expectedAB = 67.0f;

	/* Verify results */
	UtAssert_True(actualAB == expectedAB, "expected == vectorA dot vectorB");
}


void Test_Vector2F_Negate(void)
{
	/* actual = vectorA * vectorB */
	math::Vector2F vectorA(1.5f, 2.5f);

	math::Vector2F actualA = -vectorA;

    math::Vector2F expectedA;
    expectedA[0] = -1.5f;
    expectedA[1] = -2.5f;

	/* Verify results */
	UtAssert_True(actualA[0] == expectedA[0], "actualA[0] == expectedA[0]");
    UtAssert_True(actualA[1] == expectedA[1], "actualA[1] == expectedA[1]");
}


void Test_Vector2F_EMult(void)
{
	/* actual = vectorA / scalar */
	math::Vector2F vectorA(1.5f, 2.5f);
	math::Vector2F vectorB(10.5f, 20.5f);
	math::Vector2F vectorActual = vectorA.EMult(vectorB);
	math::Vector2F vectorExpected;

	/* Verify results */
	vectorExpected[0] = 15.75f;
	vectorExpected[1] = 51.25f;

	UtAssert_True(fabs(vectorActual[0] - vectorExpected[0]) <= 0.00001, "[0] vectorExpected == vectorA.EMult(vectorB)");
	UtAssert_True(fabs(vectorActual[1] - vectorExpected[1]) <= 0.00001, "[1] vectorExpected == vectorA.EMult(vectorB)");
}


void Test_Vector2F_Constrain(void)
{
	/* actual = vectorA / scalar */
	math::Vector2F vectorA(1.5f, 2.5f);
	math::Vector2F vectorB(-1.5f, -2.5f);
	math::Vector2F vectorAExpected;
	math::Vector2F vectorBExpected;

	vectorA.Constrain(0, 0.5, 1.0);
	vectorA.Constrain(1, 1.5, 2.7);

	vectorB.Constrain(0, -1.0, -0.5);
	vectorB.Constrain(1, -2.7, -1.5);

	/* Verify results */
	vectorAExpected[0] = 1.0f;
	vectorAExpected[1] = 2.5f;

	vectorBExpected[0] = -1.0f;
	vectorBExpected[1] = -2.5f;

	UtAssert_True(fabs(vectorA[0] - vectorAExpected[0]) <= 0.00001, "[0] vectorAExpected == vectorA.Constrain(0, min, max)");
	UtAssert_True(fabs(vectorA[1] - vectorAExpected[1]) <= 0.00001, "[1] vectorAExpected == vectorA.Constrain(1, min, max)");

	UtAssert_True(fabs(vectorB[0] - vectorBExpected[0]) <= 0.00001, "[0] vectorBExpected == vectorB.Constrain(0, min, max)");
	UtAssert_True(fabs(vectorB[1] - vectorBExpected[1]) <= 0.00001, "[1] vectorBExpected == vectorB.Constrain(1, min, max)");
}


void Test_Vector2F_Normalized(void)
{
	/* actual = vectorA / scalar */
	math::Vector2F vectorA(1.5f, 2.5f);
	math::Vector2F vectorAExpected;

    vectorA = vectorA.Normalized();

	/* Verify results */
	vectorAExpected[0] = 1.5 / sqrtf(1.5f * 1.5f + 2.5f * 2.5f);
	vectorAExpected[1] = 2.5f / sqrtf(1.5f * 1.5f + 2.5f * 2.5f);

	UtAssert_True(fabs(vectorA[0] - vectorAExpected[0]) <= 0.00001, "[0] vectorAExpected == vectorA.Constrain(0, min, max)");
	UtAssert_True(fabs(vectorA[1] - vectorAExpected[1]) <= 0.00001, "[1] vectorAExpected == vectorA.Constrain(1, min, max)");
}


/* Auto generated with GNU Octave, version 4.0.0 */
void Test_Vector2F_Add_2F_Random(void)
{
    int i, j = 0;

    math::Vector2F A (
        {0.585336,0.080683}
    );

    math::Vector2F B (
        {0.108697,0.051081}
    );

    math::Vector2F expected (
        {0.694033,0.131764}
    );

    math::Vector2F result;
    result.Zero();

    result = A + B;

    for(i = 0; i < 2; i++)
    {
        UtAssert_True(fabs(result[i] - expected[i]) < 0.00001f, "fabs(result[i] - expected[i]) < 0.00001f");

    }
}


/* Auto generated with GNU Octave, version 4.0.0 */
void Test_Vector2F_Sub_2F_Random(void)
{
    int i, j = 0;

    math::Vector2F A (
        {-0.855277,-0.210917}
    );

    math::Vector2F B (
        {0.635585,0.791613}
    );

    math::Vector2F expected (
        {-1.490862,-1.002530}
    );

    math::Vector2F result;
    result.Zero();

    result = A - B;

    for(i = 0; i < 2; i++)
    {
        UtAssert_True(fabs(result[i] - expected[i]) < 0.00001f, "fabs(result[i] - expected[i]) < 0.00001f");

    }
}



