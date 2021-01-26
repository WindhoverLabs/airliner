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

#include "Vector4F_test.hpp"
#include <math/Vector4F.hpp>

#include "utassert.h"
#include <float.h>


void Test_Vector4F_Constructor(void)
{
	math::Vector4F vector(1.0f, 2.0f, 3.0f, 4.0f);

	/* Verify results */
	UtAssert_True(vector[0] == 1.0f, "vector[0] == 1.0f");
	UtAssert_True(vector[1] == 2.0f, "vector[1] == 2.0f");
	UtAssert_True(vector[2] == 3.0f, "vector[2] == 3.0f");
	UtAssert_True(vector[3] == 4.0f, "vector[3] == 4.0f");
}


void Test_Vector4F_IndexOutOfBounds(void)
{
	math::Vector4F vector(1.0f, 2.0f, 3.0f, 4.0f);

	/* Verify results */
	UtAssert_True(vector[0] == 1.0f, "vector[0] == 1.0f");
	UtAssert_True(vector[1] == 2.0f, "vector[1] == 2.0f");
	UtAssert_True(vector[2] == 3.0f, "vector[2] == 3.0f");
	UtAssert_True(vector[3] == 4.0f, "vector[3] == 4.0f");
	UtAssert_True(isnan(vector[4]), "vector[4] == nan");
}


void Test_Vector4F_Length(void)
{
	math::Vector4F vector(1.5f, 2.5f, 3.5f, 4.5f);
	float expectedLength = vector.Length();
    float expectedResult = sqrtf((1.5f)*(1.5f) + (2.5f)*(2.5f) + (3.5f)*(3.5f) + (4.5f)*(4.5f));
    float actualLength = 6.4031242f;

    /* Verify results */
    UtAssert_True(fabs(expectedLength - actualLength) <= FLT_EPSILON, "vector.Length() == expectedLength");
    UtAssert_True(fabs(vector.Length() - expectedResult) <= FLT_EPSILON, "vector.Length() == expectedResult");
}


void Test_Vector4F_LengthSquared(void)
{
	math::Vector4F vector(1.5f, 2.5f, 3.5f, 4.5f);
	float expectedLength = vector.LengthSquared();
    float expectedResult = (1.5f)*(1.5f) + (2.5f)*(2.5f) + (3.5f)*(3.5f) + (4.5f)*(4.5f);
    float actualLength = 41.0f;

    /* Verify results */
    UtAssert_True(fabs(expectedLength - actualLength) <= FLT_EPSILON, "vector.Length() == expectedLength");
    UtAssert_True(vector.LengthSquared() == expectedResult, "vector.Length() == expectedResult");
}


void Test_Vector4F_AdditionOperator(void)
{
	/* vectorActual = vectorA + vectorB */
	math::Vector4F vectorA(1.5f, 2.5f, 3.5f, 4.5f);
	math::Vector4F vectorB(10.5f, 20.5f, 30.5f, 40.5f);
	math::Vector4F vectorActual = vectorA + vectorB;

	/* Verify results */
	UtAssert_True(vectorActual[0] == 12.0, "vectorActual[0] == vectorA[0] + vectorB[0]");
	UtAssert_True(vectorActual[1] == 23.0, "vectorActual[1] == vectorA[1] + vectorB[1]");
	UtAssert_True(vectorActual[2] == 34.0, "vectorActual[2] == vectorA[2] + vectorB[2]");
	UtAssert_True(vectorActual[3] == 45.0, "vectorActual[3] == vectorA[3] + vectorB[3]");
}


void Test_Vector3F_SubtractionOperator(void)
{
	/* vectorActual = vectorA - vectorB */
	math::Vector4F vectorA(1.5f, 2.5f, 3.5f, 4.5f);
	math::Vector4F vectorB(10.5f, 20.5f, 30.5f, 40.5f);
	math::Vector4F vectorActual = vectorA - vectorB;

	/* Verify results */
	UtAssert_True(vectorActual[0] == -9.0, "vectorActual[0] == vectorA[0] - vectorB[0]");
	UtAssert_True(vectorActual[1] == -18.0, "vectorActual[1] == vectorA[1] - vectorB[1]");
	UtAssert_True(vectorActual[2] == -27.0, "vectorActual[2] == vectorA[2] - vectorB[2]");
	UtAssert_True(vectorActual[3] == -36.0, "vectorActual[3] == vectorA[3] - vectorB[3]");
}

void Test_Vector4F_ScalarMultiplyOperator(void)
{
	/* actual = vectorA * scalar */
	math::Vector4F vectorA(1.5f, 2.5f, 3.5f, 4.5f);
	float scalar = 10.5f;
	math::Vector4F vectorActual = vectorA * scalar;
	math::Vector4F vectorExpected(15.75f, 26.25f, 36.75, 47.25f);

	/* Verify results */
	UtAssert_True(vectorExpected[0] == vectorActual[0], "[0] vectorExpected == vectorA * scalar");
	UtAssert_True(vectorExpected[1] == vectorActual[1], "[1] vectorExpected == vectorA * scalar");
	UtAssert_True(vectorExpected[2] == vectorActual[2], "[2] vectorExpected == vectorA * scalar");
	UtAssert_True(vectorExpected[3] == vectorActual[3], "[3] vectorExpected == vectorA * scalar");
}


void Test_Vector4F_ScalarDivOperator(void)
{
	/* actual = vectorA / scalar */
	math::Vector4F vectorA(1.5f, 2.5f, 3.5f, 4.5f);
	float scalar = 10.5f;
	math::Vector4F vectorActual = vectorA / scalar;
	math::Vector4F vectorExpected(0.142857f, 0.238095f, 0.333333f, 0.428571f);

	/* Verify results */
	UtAssert_True(fabs(vectorActual[0] - vectorExpected[0]) <= 0.00001, "[0] vectorExpected == vectorA / scalar");
	UtAssert_True(fabs(vectorActual[1] - vectorExpected[1]) <= 0.00001, "[1] vectorExpected == vectorA / scalar");
	UtAssert_True(fabs(vectorActual[2] - vectorExpected[2]) <= 0.00001, "[2] vectorExpected == vectorA / scalar");
	UtAssert_True(fabs(vectorActual[3] - vectorExpected[3]) <= 0.00001, "[3] vectorExpected == vectorA / scalar");
}


void Test_Vector4F_DotProductOperator(void)
{
	/* actual = vectorA * vectorB */
	math::Vector4F vectorA(1.5f, 2.5f, 3.5f, 4.5);
	math::Vector4F vectorB(10.5f, 20.5f, 30.5f, 40.5f);
    
	/* This is actual flight data collected from PX4 code */
	math::Vector4F vectorRzA(-0.019784f, -0.024276f, 0.999510f, 0.0f);
	math::Vector4F vectorRzB(-0.019808f, -0.024276f, 0.999509f, 0.0f);
	math::Vector4F vectorRzC(-0.019828f, -0.024274f, 0.999509f, 0.0f);
	math::Vector4F vectorRspzA(-0.019997f, -0.025087f, 0.999485f, 0.0f);
	math::Vector4F vectorRspzB(-0.019883f, -0.024780f, 0.999495f, 0.0f);
	math::Vector4F vectorRspzC(-0.019977f, -0.024787f, 0.999493f, 0.0f);

	float actualAB = vectorA * vectorB;
    
	float actualeRzcosA = vectorRzA * vectorRspzA;
	float actualeRzcosB = vectorRzB * vectorRspzB;
	float actualeRzcosC = vectorRzC * vectorRspzC;

	float expectedAB = 356.0f;
    
	float expectedeRzcosA = 1.000000;
	float expectedeRzcosB = 1.000000;
	float expectedeRzcosC = 1.000000;

	/* Verify results */
	UtAssert_True(fabs(actualeRzcosA - expectedeRzcosA) <= 0.00001, "fabs(actualeRzcosA - expectedeRzcosA) <= FLT_EPSILON");
	UtAssert_True(fabs(actualeRzcosB - expectedeRzcosB) <= 0.00001, "fabs(actualeRzcosB - expectedeRzcosB) <= FLT_EPSILON");
	UtAssert_True(fabs(actualeRzcosC - expectedeRzcosC) <= 0.00001, "fabs(actualeRzcosC - expectedeRzcosC) <= FLT_EPSILON");
}


void Test_Vector4F_EqualityOperator(void)
{
    math::Vector4F vectorA(0.0f, 0.0f, 0.0f, 0.0f);
    math::Vector4F vectorB(0.0f, 0.0f, 0.0f, 0.0f);
    math::Vector4F vectorC(1.0f, 2.0f, 3.0f, 4.0f);
    
    vectorA = vectorB = vectorC;
    
	/* Verify results */
	UtAssert_True(vectorA[0] == vectorB[0], "vectorA[0] == vectorB[0]");
	UtAssert_True(vectorA[1] == vectorB[1], "vectorA[1] == vectorB[1]");
	UtAssert_True(vectorA[2] == vectorB[2], "vectorA[2] == vectorB[2]");
	UtAssert_True(vectorA[3] == vectorB[3], "vectorA[3] == vectorB[3]");
	UtAssert_True(vectorA[0] == vectorC[0], "vectorA[0] == vectorC[0]");
	UtAssert_True(vectorA[1] == vectorC[1], "vectorA[1] == vectorC[1]");
	UtAssert_True(vectorA[2] == vectorC[2], "vectorA[2] == vectorC[2]");
	UtAssert_True(vectorA[3] == vectorC[3], "vectorA[3] == vectorC[3]");
}


void Test_Vector4F_Zero(void)
{
	math::Vector4F vector(1.5f, 2.5f, 3.5f, 4.5f);
    math::Vector4F copy(0.0f, 0.0f, 0.0f, 0.0f);
    
    copy = vector;
    vector.Zero();

    /* Verify results */
    UtAssert_True(1.5f == copy[0],   "1.5f == copy[0]");
    UtAssert_True(2.5f == copy[1],   "2.5f == copy[1]");
    UtAssert_True(3.5f == copy[2],   "3.5f == copy[2]");
    UtAssert_True(4.5f == copy[3],   "4.5f == copy[3]");
    UtAssert_True(0.0f == vector[0], "0.0f == vector[0]");
    UtAssert_True(0.0f == vector[1], "0.0f == vector[1]");
    UtAssert_True(0.0f == vector[2], "0.0f == vector[2]");
    UtAssert_True(0.0f == vector[3], "0.0f == vector[3]");
}



void Test_Vector4F_ArrayOperator(void)
{
	math::Vector4F vector(1.5f, 2.5f, 3.5f, 4.5f);
	float nanResult = vector[4];

    UtAssert_True(vector[0] == 1.5f, "vector[0] == 1.5f");
    UtAssert_True(vector[1] == 2.5f, "vector[1] == 2.5f");
    UtAssert_True(vector[2] == 3.5f, "vector[2] == 3.5f");
    UtAssert_True(vector[3] == 4.5f, "vector[3] == 4.5f");
    UtAssert_True(isnan(vector[4]), "vector[4] == NAN");

    UtAssert_True(1.5f == vector[0], "1.5f == vector[0]");
    UtAssert_True(2.5f == vector[1], "2.5f == vector[1]");
    UtAssert_True(3.5f == vector[2], "3.5f == vector[2]");
    UtAssert_True(4.5f == vector[3], "4.5f == vector[3]");
    UtAssert_True(isnan(nanResult), "NAN == vector[4]");
}


void Test_Vector4F_NegateOperator(void)
{
	/* vectorActual = vectorA - vectorB */
	math::Vector4F vectorA(1.5f, -2.5f, -3.5f, 4.5f);
	math::Vector4F vectorActual = -vectorA;

	/* Verify results */
	UtAssert_True(vectorActual[0] == -1.5, "vectorActual[0] == vectorA[0] - vectorB[0]");
	UtAssert_True(vectorActual[1] == 2.5, "vectorActual[1] == vectorA[1] - vectorB[1]");
	UtAssert_True(vectorActual[2] == 3.5, "vectorActual[2] == vectorA[2] - vectorB[2]");
	UtAssert_True(vectorActual[3] == -4.5, "vectorActual[3] == vectorA[3] - vectorB[3]");
}


/* Auto generated with GNU Octave, version 4.0.0 */
void Test_Vector4F_Add_4F_Random(void)
{
    int i, j = 0;

    math::Vector4F A (
        {-0.244349,-0.942404,-0.994364,-0.911892}
    );

    math::Vector4F B (
        {0.938821,-0.411616,0.447763,0.157745}
    );

    math::Vector4F expected (
        {0.694471,-1.354020,-0.546601,-0.754147}
    );

    math::Vector4F result;
    result.Zero();

    result = A + B;

    for(i = 0; i < 4; i++)
    {
        UtAssert_True(fabs(result[i] - expected[i]) < 0.00001f, "fabs(result[i] - expected[i]) < 0.00001f");

    }
}


/* Auto generated with GNU Octave, version 4.0.0 */
void Test_Vector4F_Sub_4F_Random(void)
{
    int i, j = 0;

    math::Vector4F A (
        {-0.235606,-0.062519,0.850069,-0.347899}
    );

    math::Vector4F B (
        {-0.741309,0.910904,0.565218,-0.748560}
    );

    math::Vector4F expected (
        {0.505703,-0.973423,0.284851,0.400661}
    );

    math::Vector4F result;
    result.Zero();

    result = A - B;

    for(i = 0; i < 4; i++)
    {
        UtAssert_True(fabs(result[i] - expected[i]) < 0.00001f, "fabs(result[i] - expected[i]) < 0.00001f");

    }
}


