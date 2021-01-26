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

#include "Vector1F_test.hpp"
#include <math/Vector1F.hpp>

#include "utassert.h"
#include <float.h>


void Test_Vector1F_Constructor(void)
{
    math::Vector1F vector(1.0f);

    /* Verify results */
    UtAssert_True(vector[0] == 1.0f, "vector[0] == 1.0f");
}


void Test_Vector1F_IndexOutOfBounds(void)
{
	math::Vector1F vector(1.0f);

	/* Verify results */
	UtAssert_True(vector[0] == 1.0f, "vector[0] == 1.0f");
	UtAssert_True(isnan(vector[1]), "vector[1] == nan");
}


void Test_Vector1F_Length(void)
{
	math::Vector1F vector(1.5f);
	float expectedLength = sqrtf((1.5f)*(1.5f));
    float actualLength = vector.Length();

	/* Verify results */
    UtAssert_True(fabs(expectedLength - actualLength) <= FLT_EPSILON, "vector.Length() == expectedLength");
}


void Test_Vector1F_Zero(void)
{
	math::Vector1F vectorA(1.5f);
    math::Vector1F vectorB;
    vectorB[0] = 2.5f;
    vectorA.Zero();
    vectorB.Zero();

	/* Verify results */
    UtAssert_True(vectorA[0] == 0.0f, "vectorA[0] == 0.0f");
    UtAssert_True(vectorB[0] == 0.0f, "vectorB[0] == 0.0f");
}


void Test_Vector1F_ArrayOperator(void)
{
	math::Vector1F vector(1.5f);
	float nanResult = vector[1];

    UtAssert_True(vector[0] == 1.5f, "vector[0] == 1.5f");
    UtAssert_True(isnan(vector[1]), "vector[1] == NAN");

    UtAssert_True(1.5f == vector[0], "1.5f == vector[0]");
    UtAssert_True(isnan(nanResult), "NAN == vector[1]");
}


void Test_Vector1F_AdditionOperator(void)
{
	/* vectorActual = vectorA + vectorB */
	math::Vector1F vectorA(1.5f);
	math::Vector1F vectorB(10.5f);
	math::Vector1F vectorActual = vectorA + vectorB;

	/* Verify results */
	UtAssert_True(vectorA[0] == 1.5f, "vectorA[0] == 1.5f");
	UtAssert_True(vectorB[0] == 10.5f, "vectorB[0] == 10.5f");
	UtAssert_True(vectorActual[0] == 12.0f, "vectorActual[0] == 12.0f");
}


void Test_Vector1F_SubtractionOperator(void)
{
	/* vectorActual = vectorA - vectorB */
	math::Vector1F vectorA(1.5f);
	math::Vector1F vectorB(10.5f);
	math::Vector1F vectorActual = vectorA - vectorB;

	/* Verify results */
	UtAssert_True(vectorA[0] == 1.5f, "vectorA[0] == 1.5f");
	UtAssert_True(vectorB[0] == 10.5f, "vectorB[0] == 10.5f");
	UtAssert_True(vectorActual[0] == -9.0f, "vectorActual[0] == -9.0f");
}


void Test_Vector1F_ScalarMultiplyOperator(void)
{
	/* actual = vectorA * scalar */
	math::Vector1F vectorA(1.5f);
	float scalar = 10.5f;
	math::Vector1F vectorActual = vectorA * scalar;
	math::Vector1F vectorExpected;

	/* Verify results */
	vectorExpected[0] = 15.75;

	UtAssert_True(vectorExpected[0] == vectorActual[0], "[0] vectorExpected == vectorA * scalar");
}


void Test_Vector1F_ScalarDivOperator(void)
{
	/* actual = vectorA / scalar */
	math::Vector1F vectorA(1.5f);
	float scalar = 10.5f;
	math::Vector1F vectorActual = vectorA / scalar;
	math::Vector1F vectorExpected;

	/* Verify results */
	vectorExpected[0] = 0.142857f;

	UtAssert_True(fabs(vectorActual[0] - vectorExpected[0]) <= 0.00001, "[0] vectorExpected == vectorA / scalar");
}


void Test_Vector1F_DotProductOperator(void)
{
	/* actual = vectorA * vectorB */
	math::Vector1F vectorA(1.5f);
	math::Vector1F vectorB(10.5f);

	float actualAB = vectorA * vectorB;

	float expectedAB = 15.75;

	/* Verify results */
	UtAssert_True(actualAB == expectedAB, "expected == vectorA dot vectorB");
}


void Test_Vector1F_Negate(void)
{
	/* actual = vectorA / scalar */
	math::Vector1F vectorA(1.5f);
	math::Vector1F vectorActual = -vectorA;
	math::Vector1F vectorExpected;

	/* Verify results */
	vectorExpected[0] = -1.5f;

	UtAssert_True(vectorActual[0] == vectorExpected[0], "[0] vectorExpected == vectorA");
}


void Test_Vector1F_Pow(void)
{
	/* actual = vectorA / scalar */
	math::Vector1F vectorA(2.0f);
	math::Vector1F vectorActual = vectorA.pow(2.0f);
	math::Vector1F vectorExpected;

	/* Verify results */
	vectorExpected[0] = 2.0f * 2.0f;

	UtAssert_True(vectorActual[0] == vectorExpected[0], "[0] vectorExpected == vectorA");
}


void Test_Vector1F_EMult(void)
{
	/* actual = vectorA / scalar */
	math::Vector1F vectorA(1.5f);
	math::Vector1F vectorB(10.5f);
	math::Vector1F vectorActual = vectorA.EMult(vectorB);
	math::Vector1F vectorExpected;

	/* Verify results */
	vectorExpected[0] = 15.75f;

	UtAssert_True(fabs(vectorActual[0] - vectorExpected[0]) <= 0.00001, "[0] vectorExpected == vectorA.EMult(vectorB)");
}


void Test_Vector1F_EDivide(void)
{
	/* actual = vectorA / scalar */
	math::Vector1F vectorA(1.5f);
	math::Vector1F vectorB(10.5f);
	math::Vector1F vectorActual = vectorA.EDivide(vectorB);
	math::Vector1F vectorExpected;

	/* Verify results */
	vectorExpected[0] = 1.5f / 10.5f;

	UtAssert_True(fabs(vectorActual[0] - vectorExpected[0]) <= 0.00001, "[0] vectorExpected == vectorA.EMult(vectorB)");
}


void Test_Vector1F_Constrain(void)
{
	/* actual = vectorA / scalar */
	math::Vector1F vectorA(1.5f);
	math::Vector1F vectorB(-1.5f);
	math::Vector1F vectorAExpected;
	math::Vector1F vectorBExpected;

	vectorA.Constrain(0, 0.5, 1.0);
	vectorB.Constrain(0, -1.0, -0.5);

	/* Verify results */
	vectorAExpected[0] = 1.0f;
	vectorBExpected[0] = -1.0f;

	UtAssert_True(fabs(vectorA[0] - vectorAExpected[0]) <= 0.00001, "[0] vectorAExpected == vectorA.Constrain(0, min, max)");
	UtAssert_True(fabs(vectorB[0] - vectorBExpected[0]) <= 0.00001, "[0] vectorBExpected == vectorB.Constrain(0, min, max)");
}


void Test_Vector1F_Normalized(void)
{
	/* actual = vectorA / scalar */
	math::Vector1F vectorA(1.5f);
    
    vectorA = vectorA.Normalized();

	math::Vector1F vectorAExpected;

	/* Verify results */
	vectorAExpected = 1.5 / sqrt(1.5f * 1.5f);

	UtAssert_True(fabs(vectorA[0] - vectorAExpected[0]) <= 0.00001, "[0] vectorAExpected == vectorA.Constrain(0, min, max)");
}


void Test_Vector1F_Normalize(void)
{
	/* actual = vectorA / scalar */
	math::Vector1F vectorA(1.5f);
    
    vectorA.Normalize();

	math::Vector1F vectorAExpected;

	/* Verify results */
	vectorAExpected = 1.5 / sqrt(1.5f * 1.5f);

	UtAssert_True(fabs(vectorA[0] - vectorAExpected[0]) <= 0.00001, "[0] vectorAExpected == vectorA.Constrain(0, min, max)");
}


/* Auto generated with GNU Octave, version 4.0.0 */
void Test_Vector1F_Add_1F_Random(void)
{
    int i = 0;

    math::Vector1F A (
        -0.295051
    );

    math::Vector1F B (
        -0.932045
    );

    math::Vector1F expected (
        -1.227096
    );

    math::Vector1F result;
    result.Zero();

    result = A + B;

    UtAssert_True(fabs(result[i] - expected[i]) < 0.00001f, "fabs(result[i] - expected[i]) < 0.00001f");

}


/* Auto generated with GNU Octave, version 4.0.0 */
void Test_Vector1F_Sub_1F_Random(void)
{
    int i = 0;

    math::Vector1F A (
        0.824133
    );

    math::Vector1F B (
        -0.916948
    );

    math::Vector1F expected (
        1.741082
    );

    math::Vector1F result;
    result.Zero();

    result = A - B;

    UtAssert_True(fabs(result[i] - expected[i]) < 0.00001f, "fabs(result[i] - expected[i]) < 0.00001f");
}
