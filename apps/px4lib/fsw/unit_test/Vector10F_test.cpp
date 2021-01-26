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

#include "Vector10F_test.hpp"
#include <math/Vector10F.hpp>

#include "utassert.h"
#include <float.h>


void Test_Vector10F_Constructor(void)
{
	math::Vector10F vector(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f,
                           7.0f, 8.0f, 9.0f, 10.0f);

	/* Verify results */
	UtAssert_True(vector[0] == 1.0f, "vector[0] == 1.0f");
	UtAssert_True(vector[1] == 2.0f, "vector[1] == 2.0f");
	UtAssert_True(vector[2] == 3.0f, "vector[2] == 3.0f");
	UtAssert_True(vector[3] == 4.0f, "vector[3] == 4.0f");
	UtAssert_True(vector[4] == 5.0f, "vector[4] == 5.0f");
	UtAssert_True(vector[5] == 6.0f, "vector[5] == 6.0f");
	UtAssert_True(vector[6] == 7.0f, "vector[3] == 7.0f");
	UtAssert_True(vector[7] == 8.0f, "vector[4] == 8.0f");
	UtAssert_True(vector[8] == 9.0f, "vector[5] == 9.0f");
	UtAssert_True(vector[9] == 10.0f, "vector[5] == 10.0f");
}


void Test_Vector10F_IndexOutOfBounds(void)
{
	math::Vector10F vector(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f,
                           7.0f, 8.0f, 9.0f, 10.0f);
	/* Verify results */
	UtAssert_True(vector[0] == 1.0f, "vector[0] == 1.0f");
	UtAssert_True(vector[1] == 2.0f, "vector[1] == 2.0f");
	UtAssert_True(vector[2] == 3.0f, "vector[2] == 3.0f");
	UtAssert_True(vector[3] == 4.0f, "vector[3] == 4.0f");
	UtAssert_True(vector[4] == 5.0f, "vector[4] == 5.0f");
	UtAssert_True(vector[5] == 6.0f, "vector[5] == 6.0f");
	UtAssert_True(vector[6] == 7.0f, "vector[3] == 7.0f");
	UtAssert_True(vector[7] == 8.0f, "vector[4] == 8.0f");
	UtAssert_True(vector[8] == 9.0f, "vector[5] == 9.0f");
	UtAssert_True(vector[9] == 10.0f, "vector[5] == 10.0f");
	UtAssert_True(isnan(vector[10]), "vector[10] == nan");
}


void Test_Vector10F_Length(void)
{
	math::Vector10F vector(1.5f, 2.5f, 3.5f, 4.5f, 5.5f, 6.5f,
                           7.5f, 8.5f, 9.5f, 10.5f);

	/* The static declaration below is to fix a unit test issue that shows itself
	 * when you build the tests with maximum performance optimization.  Without
	 * the static declaration, the compiler can sometimes optimize the code such
	 * that the last calculated value, "actualLength", is not actually stored in
	 * memory.  Since both variables are floats, the compile leaves the value in
	 * the FPU registers so the "expectedLength - actualLength" computation is
	 * performed without having to reload the registers.  When values are copied
	 * back to RAM, only the 32-bit value is copied back to RAM.  When the value
	 * is used directly from the FPU register without copying it back to RAM, its
	 * actually using a 64-bit value.  With enough computations, this increased
	 * precision can result in divergence greater than FLT_EPSILON.
	 *
	 * Technically, this is a more correct answer.  That is another conversation.
	 * For now, we want to limit calculations to 32-bits only.
	 */
	static float expectedLength = sqrtf((1.5f)*(1.5f) + (2.5f)*(2.5f) + (3.5f)*(3.5f) +
                                 (4.5f)*(4.5f) + (5.5f)*(5.5f) + (6.5f)*(6.5f) +
                                 (7.5f)*(7.5f) + (8.5f)*(8.5f) + (9.5f)*(9.5f) +
                                 (10.5f)*(10.5f)
    );
    static float actualLength = vector.Length();

	/* Verify results */
    UtAssert_True(fabs(expectedLength - actualLength) <= FLT_EPSILON, "vector.Length() == expectedLength");
}


void Test_Vector10F_Zero(void)
{
	math::Vector10F vector(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f,
                           7.0f, 8.0f, 9.0f, 10.0f);
	vector.Zero();

	/* Verify results */
    UtAssert_True(vector[0] == 0.0f, "vector[0] == 0.0f");
    UtAssert_True(vector[1] == 0.0f, "vector[1] == 0.0f");
    UtAssert_True(vector[2] == 0.0f, "vector[2] == 0.0f");
    UtAssert_True(vector[3] == 0.0f, "vector[3] == 0.0f");
    UtAssert_True(vector[4] == 0.0f, "vector[4] == 0.0f");
    UtAssert_True(vector[5] == 0.0f, "vector[5] == 0.0f");
    UtAssert_True(vector[6] == 0.0f, "vector[6] == 0.0f");
    UtAssert_True(vector[7] == 0.0f, "vector[7] == 0.0f");
    UtAssert_True(vector[8] == 0.0f, "vector[8] == 0.0f");
    UtAssert_True(vector[9] == 0.0f, "vector[9] == 0.0f");
}


void Test_Vector10F_ArrayOperator(void)
{
	math::Vector10F vector(1.5f, 2.5f, 3.5f, 4.5f, 5.5f, 6.5f,
                           7.5f, 8.5f, 9.5f, 10.5f);
	float nanResult = vector[10];

    UtAssert_True(vector[0] == 1.5f, "vector[0] == 1.5f");
    UtAssert_True(vector[1] == 2.5f, "vector[1] == 2.5f");
    UtAssert_True(vector[2] == 3.5f, "vector[2] == 3.5f");
    UtAssert_True(vector[3] == 4.5f, "vector[3] == 4.5f");
    UtAssert_True(vector[4] == 5.5f, "vector[4] == 5.5f");
    UtAssert_True(vector[5] == 6.5f, "vector[5] == 6.5f");
    UtAssert_True(vector[6] == 7.5f, "vector[6] == 7.5f");
    UtAssert_True(vector[7] == 8.5f, "vector[7] == 8.5f");
    UtAssert_True(vector[8] == 9.5f, "vector[8] == 9.5f");
    UtAssert_True(vector[9] == 10.5f, "vector[9] == 10.5f");
    UtAssert_True(isnan(vector[10]), "vector[10] == NAN");

    UtAssert_True(1.5f == vector[0], "1.5f == vector[0]");
    UtAssert_True(2.5f == vector[1], "2.5f == vector[1]");
    UtAssert_True(3.5f == vector[2], "3.5f == vector[2]");
    UtAssert_True(4.5f == vector[3], "4.5f == vector[3]");
    UtAssert_True(5.5f == vector[4], "5.5f == vector[4]");
    UtAssert_True(6.5f == vector[5], "6.5f == vector[5]");
    UtAssert_True(7.5f == vector[6], "7.5f == vector[6]");
    UtAssert_True(8.5f == vector[7], "8.5f == vector[7]");
    UtAssert_True(9.5f == vector[8], "9.5f == vector[8]");
    UtAssert_True(10.5f == vector[9], "10.5f == vector[9]");
    UtAssert_True(isnan(nanResult), "NAN == vector[10]");

}


void Test_Vector10F_AdditionOperator(void)
{
	/* vectorActual = vectorA + vectorB */
	math::Vector10F vectorA(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f,
                           7.0f, 8.0f, 9.0f, 10.0f);
	math::Vector10F vectorB(10.0f, 20.0f, 30.0f, 40.0f, 50.0f, 60.0f,
                           70.0f, 80.0f, 90.0f, 100.0f);
	math::Vector10F vectorActual = vectorA + vectorB;

	/* Verify results */
	UtAssert_True(vectorActual[0] == 11.0, "vectorActual[0] == vectorA[0] + vectorB[0]");
	UtAssert_True(vectorActual[1] == 22.0, "vectorActual[1] == vectorA[1] + vectorB[1]");
	UtAssert_True(vectorActual[2] == 33.0, "vectorActual[2] == vectorA[2] + vectorB[2]");
	UtAssert_True(vectorActual[3] == 44.0, "vectorActual[3] == vectorA[3] + vectorB[3]");
	UtAssert_True(vectorActual[4] == 55.0, "vectorActual[4] == vectorA[4] + vectorB[4]");
	UtAssert_True(vectorActual[5] == 66.0, "vectorActual[5] == vectorA[5] + vectorB[5]");
	UtAssert_True(vectorActual[6] == 77.0, "vectorActual[6] == vectorA[6] + vectorB[6]");
	UtAssert_True(vectorActual[7] == 88.0, "vectorActual[7] == vectorA[7] + vectorB[7]");
	UtAssert_True(vectorActual[8] == 99.0, "vectorActual[8] == vectorA[8] + vectorB[8]");
	UtAssert_True(vectorActual[9] == 110.0, "vectorActual[9] == vectorA[9] + vectorB[9]");

}


void Test_Vector10F_SubtractionOperator(void)
{
	/* vectorActual = vectorA - vectorB */
	math::Vector10F vectorA(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f,
                           7.0f, 8.0f, 9.0f, 10.0f);
	math::Vector10F vectorB(10.0f, 20.0f, 30.0f, 40.0f, 50.0f, 60.0f,
                           70.0f, 80.0f, 90.0f, 100.0f);
	math::Vector10F vectorActual = vectorA - vectorB;

	/* Verify results */
	UtAssert_True(vectorActual[0] == -9.0, "vectorActual[0] == vectorA[0] - vectorB[0]");
	UtAssert_True(vectorActual[1] == -18.0, "vectorActual[1] == vectorA[1] - vectorB[1]");
	UtAssert_True(vectorActual[2] == -27.0, "vectorActual[2] == vectorA[2] - vectorB[2]");
	UtAssert_True(vectorActual[3] == -36.0, "vectorActual[3] == vectorA[3] - vectorB[3]");
	UtAssert_True(vectorActual[4] == -45.0, "vectorActual[4] == vectorA[4] - vectorB[4]");
	UtAssert_True(vectorActual[5] == -54.0, "vectorActual[5] == vectorA[5] - vectorB[5]");
	UtAssert_True(vectorActual[6] == -63.0, "vectorActual[6] == vectorA[6] - vectorB[6]");
	UtAssert_True(vectorActual[7] == -72.0, "vectorActual[7] == vectorA[7] - vectorB[7]");
	UtAssert_True(vectorActual[8] == -81.0, "vectorActual[8] == vectorA[8] - vectorB[8]");
	UtAssert_True(vectorActual[9] == -90.0, "vectorActual[9] == vectorA[9] - vectorB[9]");
}


void Test_Vector10F_ScalarMultiplyOperator(void)
{
	/* actual = vectorA * scalar */
	math::Vector10F vectorA(1.5f, 2.5f, 3.5f, 4.5f, 5.5f, 6.5f, 7.5f, 8.5f, 9.5f, 10.5f);
	float scalar = 10.5f;
	math::Vector10F vectorActual = vectorA * scalar;
	math::Vector10F vectorExpected;

	/* Verify results */
	vectorExpected[0] = 15.75;
	vectorExpected[1] = 26.25;
	vectorExpected[2] = 36.75;
	vectorExpected[3] = 47.25;
	vectorExpected[4] = 57.75;
	vectorExpected[5] = 68.25;
	vectorExpected[6] = 78.75;
	vectorExpected[7] = 89.25;
	vectorExpected[8] = 99.75;
	vectorExpected[9] = 110.25;

	UtAssert_True(vectorExpected[0] == vectorActual[0], "[0] vectorExpected == vectorA * scalar");
	UtAssert_True(vectorExpected[1] == vectorActual[1], "[1] vectorExpected == vectorA * scalar");
	UtAssert_True(vectorExpected[2] == vectorActual[2], "[2] vectorExpected == vectorA * scalar");
	UtAssert_True(vectorExpected[3] == vectorActual[3], "[3] vectorExpected == vectorA * scalar");
	UtAssert_True(vectorExpected[4] == vectorActual[4], "[4] vectorExpected == vectorA * scalar");
	UtAssert_True(vectorExpected[5] == vectorActual[5], "[5] vectorExpected == vectorA * scalar");
	UtAssert_True(vectorExpected[6] == vectorActual[6], "[6] vectorExpected == vectorA * scalar");
	UtAssert_True(vectorExpected[7] == vectorActual[7], "[7] vectorExpected == vectorA * scalar");
	UtAssert_True(vectorExpected[8] == vectorActual[8], "[8] vectorExpected == vectorA * scalar");
	UtAssert_True(vectorExpected[9] == vectorActual[9], "[9] vectorExpected == vectorA * scalar");

}


void Test_Vector10F_ScalarDivOperator(void)
{
	/* actual = vectorA / scalar */
	math::Vector10F vectorA(1.5f, 2.5f, 3.5f, 4.5f, 5.5f, 6.5f, 7.5f, 8.5f, 9.5f, 10.5f);
	float scalar = 10.5f;
	math::Vector10F vectorActual = vectorA / scalar;
	math::Vector10F vectorExpected;

	/* Verify results */
	vectorExpected[0] = 0.142857f;
	vectorExpected[1] = 0.238095f;
	vectorExpected[2] = 0.333333f;
	vectorExpected[3] = 0.428571f;
	vectorExpected[4] = 0.523809f;
	vectorExpected[5] = 0.619047f;
	vectorExpected[6] = 0.714285f;
	vectorExpected[7] = 0.809523f;
	vectorExpected[8] = 0.904761f;
	vectorExpected[9] = 1.0;

	UtAssert_True(fabs(vectorActual[0] - vectorExpected[0]) <= 0.00001, "[0] vectorExpected == vectorA / scalar");
	UtAssert_True(fabs(vectorActual[1] - vectorExpected[1]) <= 0.00001, "[1] vectorExpected == vectorA / scalar");
	UtAssert_True(fabs(vectorActual[2] - vectorExpected[2]) <= 0.00001, "[2] vectorExpected == vectorA / scalar");
	UtAssert_True(fabs(vectorActual[3] - vectorExpected[3]) <= 0.00001, "[3] vectorExpected == vectorA / scalar");
	UtAssert_True(fabs(vectorActual[4] - vectorExpected[4]) <= 0.00001, "[4] vectorExpected == vectorA / scalar");
	UtAssert_True(fabs(vectorActual[5] - vectorExpected[5]) <= 0.00001, "[5] vectorExpected == vectorA / scalar");
	UtAssert_True(fabs(vectorActual[6] - vectorExpected[6]) <= 0.00001, "[6] vectorExpected == vectorA / scalar");
	UtAssert_True(fabs(vectorActual[7] - vectorExpected[7]) <= 0.00001, "[7] vectorExpected == vectorA / scalar");
	UtAssert_True(fabs(vectorActual[8] - vectorExpected[8]) <= 0.00001, "[8] vectorExpected == vectorA / scalar");
	UtAssert_True(fabs(vectorActual[9] - vectorExpected[9]) <= 0.00001, "[9] vectorExpected == vectorA / scalar");

}


void Test_Vector10F_EMult(void)
{
	/* actual = vectorA / scalar */
	math::Vector10F vectorA(1.5f, 2.5f, 3.5f, 4.5f, 5.5f, 6.5f, 7.5f, 8.5f, 9.5f, 10.5f);
	math::Vector10F vectorB(10.5f, 20.5f, 30.5f, 40.5f, 50.5f, 60.5f, 70.5f, 80.5f, 90.5f, 100.5f);
	math::Vector10F vectorActual = vectorA.EMult(vectorB);
	math::Vector10F vectorExpected;

	/* Verify results */
	vectorExpected[0] = 15.75f;
	vectorExpected[1] = 51.25f;
	vectorExpected[2] = 106.75f;
	vectorExpected[3] = 182.25f;
	vectorExpected[4] = 277.75f;
	vectorExpected[5] = 393.25f;
	vectorExpected[6] = 528.75f;
	vectorExpected[7] = 684.25f;
	vectorExpected[8] = 859.75f;
	vectorExpected[9] = 1055.25f;

	UtAssert_True(fabs(vectorActual[0] - vectorExpected[0]) <= 0.00001, "[0] vectorExpected == vectorA.EMult(vectorB)");
	UtAssert_True(fabs(vectorActual[1] - vectorExpected[1]) <= 0.00001, "[1] vectorExpected == vectorA.EMult(vectorB)");
	UtAssert_True(fabs(vectorActual[2] - vectorExpected[2]) <= 0.00001, "[2] vectorExpected == vectorA.EMult(vectorB)");
	UtAssert_True(fabs(vectorActual[3] - vectorExpected[3]) <= 0.00001, "[3] vectorExpected == vectorA.EMult(vectorB)");
	UtAssert_True(fabs(vectorActual[4] - vectorExpected[4]) <= 0.00001, "[4] vectorExpected == vectorA.EMult(vectorB)");
	UtAssert_True(fabs(vectorActual[5] - vectorExpected[5]) <= 0.00001, "[5] vectorExpected == vectorA.EMult(vectorB)");
	UtAssert_True(fabs(vectorActual[6] - vectorExpected[6]) <= 0.00001, "[6] vectorExpected == vectorA.EMult(vectorB)");
	UtAssert_True(fabs(vectorActual[7] - vectorExpected[7]) <= 0.00001, "[7] vectorExpected == vectorA.EMult(vectorB)");
	UtAssert_True(fabs(vectorActual[8] - vectorExpected[8]) <= 0.00001, "[8] vectorExpected == vectorA.EMult(vectorB)");
	UtAssert_True(fabs(vectorActual[9] - vectorExpected[9]) <= 0.00001, "[9] vectorExpected == vectorA.EMult(vectorB)");
}


void Test_Vector10F_EDivide(void)
{
	/* actual = vectorA / scalar */
	math::Vector10F vectorA(1.5f, 2.5f, 3.5f, 4.5f, 5.5f, 6.5f, 7.5f, 8.5f, 9.5f, 10.5f);
	math::Vector10F vectorB(10.5f, 20.5f, 30.5f, 40.5f, 50.5f, 60.5f, 70.5f, 80.5f, 90.5f, 100.5f);
	math::Vector10F vectorActual = vectorA.EDivide(vectorB);
	math::Vector10F vectorExpected;

	/* Verify results */
	vectorExpected[0] = 0.142857f;
	vectorExpected[1] = 0.121951f;
	vectorExpected[2] = 0.114754f;
	vectorExpected[3] = 0.111111f;
	vectorExpected[4] = 0.108910f;
	vectorExpected[5] = 0.107438f;
	vectorExpected[6] = 0.106382f;
	vectorExpected[7] = 0.105590f;
	vectorExpected[8] = 0.104972f;
	vectorExpected[9] = 0.104477f;

	UtAssert_True(fabs(vectorActual[0] - vectorExpected[0]) <= 0.00001, "[0] vectorExpected == vectorA.EMult(vectorB)");
	UtAssert_True(fabs(vectorActual[1] - vectorExpected[1]) <= 0.00001, "[1] vectorExpected == vectorA.EMult(vectorB)");
	UtAssert_True(fabs(vectorActual[2] - vectorExpected[2]) <= 0.00001, "[2] vectorExpected == vectorA.EMult(vectorB)");
	UtAssert_True(fabs(vectorActual[3] - vectorExpected[3]) <= 0.00001, "[3] vectorExpected == vectorA.EMult(vectorB)");
	UtAssert_True(fabs(vectorActual[4] - vectorExpected[4]) <= 0.00001, "[4] vectorExpected == vectorA.EMult(vectorB)");
	UtAssert_True(fabs(vectorActual[5] - vectorExpected[5]) <= 0.00001, "[5] vectorExpected == vectorA.EMult(vectorB)");
	UtAssert_True(fabs(vectorActual[6] - vectorExpected[6]) <= 0.00001, "[6] vectorExpected == vectorA.EMult(vectorB)");
	UtAssert_True(fabs(vectorActual[7] - vectorExpected[7]) <= 0.00001, "[7] vectorExpected == vectorA.EMult(vectorB)");
	UtAssert_True(fabs(vectorActual[8] - vectorExpected[8]) <= 0.00001, "[8] vectorExpected == vectorA.EMult(vectorB)");
	UtAssert_True(fabs(vectorActual[9] - vectorExpected[9]) <= 0.00001, "[9] vectorExpected == vectorA.EMult(vectorB)");
}


void Test_Vector10F_Constrain(void)
{
	/* actual = vectorA / scalar */
	math::Vector10F vectorA(1.5f, 2.5f, 3.5f, 4.5f, 5.5f, 6.5f, 7.5f, 8.5f, 9.5f, 10.5f);
	math::Vector10F vectorB(-1.5f, -2.5f, -3.5f, -4.5f, -5.5f, -6.5f, -7.5f, -8.5f, -9.5f, -10.5f);
	
	math::Vector10F vectorAExpected;
	math::Vector10F vectorBExpected;

	vectorA.Constrain(0, 0.5, 1.0);
	vectorA.Constrain(1, 1.5, 2.7);
	vectorA.Constrain(2, 2.5, 3.0);
	vectorA.Constrain(3, 3.5, 4.0);
	vectorA.Constrain(4, 4.5, 5.7);
	vectorA.Constrain(5, 5.5, 6.2);
	vectorA.Constrain(6, 6.5, 7.5);
	vectorA.Constrain(7, 7.5, 8.5);
	vectorA.Constrain(8, 9.5, 10.5);
	vectorA.Constrain(9, 10.0, 11.0);

	vectorB.Constrain(0, -1.0, -0.5);
	vectorB.Constrain(1, -2.7, -1.5);
	vectorB.Constrain(2, -3.0, -2.5);
	vectorB.Constrain(3, -4.0, -3.5);
	vectorB.Constrain(4, -5.7, -4.5);
	vectorB.Constrain(5, -6.2, -5.5);
	vectorB.Constrain(6, -7.5, -6.5);
	vectorB.Constrain(7, -8.5, -7.5);
	vectorB.Constrain(8, -10.5, -9.5);
	vectorB.Constrain(9, -11.0, -10.5);

	/* Verify results */
	vectorAExpected[0] = 1.0f;
	vectorAExpected[1] = 2.5f;
	vectorAExpected[2] = 3.0f;
	vectorAExpected[3] = 4.0f;
	vectorAExpected[4] = 5.5f;
	vectorAExpected[5] = 6.2f;
	vectorAExpected[6] = 7.5f;
	vectorAExpected[7] = 8.5f;
	vectorAExpected[8] = 9.5f;
	vectorAExpected[9] = 10.5f;

	vectorBExpected[0] = -1.0f;
	vectorBExpected[1] = -2.5f;
	vectorBExpected[2] = -3.0f;
	vectorBExpected[3] = -4.0f;
	vectorBExpected[4] = -5.5f;
	vectorBExpected[5] = -6.2f;
	vectorBExpected[6] = -7.5f;
	vectorBExpected[7] = -8.5f;
	vectorBExpected[8] = -9.5f;
	vectorBExpected[9] = -10.5f;

	UtAssert_True(fabs(vectorA[0] - vectorAExpected[0]) <= 0.00001, "[0] vectorAExpected == vectorA.Constrain(0, min, max)");
	UtAssert_True(fabs(vectorA[1] - vectorAExpected[1]) <= 0.00001, "[1] vectorAExpected == vectorA.Constrain(1, min, max)");
	UtAssert_True(fabs(vectorA[2] - vectorAExpected[2]) <= 0.00001, "[2] vectorAExpected == vectorA.Constrain(2, min, max)");
	UtAssert_True(fabs(vectorA[3] - vectorAExpected[3]) <= 0.00001, "[3] vectorAExpected == vectorA.Constrain(0, min, max)");
	UtAssert_True(fabs(vectorA[4] - vectorAExpected[4]) <= 0.00001, "[4] vectorAExpected == vectorA.Constrain(1, min, max)");
	UtAssert_True(fabs(vectorA[5] - vectorAExpected[5]) <= 0.00001, "[5] vectorAExpected == vectorA.Constrain(2, min, max)");
	UtAssert_True(fabs(vectorA[6] - vectorAExpected[6]) <= 0.00001, "[6] vectorAExpected == vectorA.Constrain(2, min, max)");
	UtAssert_True(fabs(vectorA[7] - vectorAExpected[7]) <= 0.00001, "[7] vectorAExpected == vectorA.Constrain(0, min, max)");
	UtAssert_True(fabs(vectorA[8] - vectorAExpected[8]) <= 0.00001, "[8] vectorAExpected == vectorA.Constrain(1, min, max)");
	UtAssert_True(fabs(vectorA[9] - vectorAExpected[9]) <= 0.00001, "[9] vectorAExpected == vectorA.Constrain(2, min, max)");

	UtAssert_True(fabs(vectorB[0] - vectorBExpected[0]) <= 0.00001, "[0] vectorBExpected == vectorB.Constrain(0, min, max)");
	UtAssert_True(fabs(vectorB[1] - vectorBExpected[1]) <= 0.00001, "[1] vectorBExpected == vectorB.Constrain(1, min, max)");
	UtAssert_True(fabs(vectorB[2] - vectorBExpected[2]) <= 0.00001, "[2] vectorBExpected == vectorB.Constrain(2, min, max)");
	UtAssert_True(fabs(vectorB[3] - vectorBExpected[3]) <= 0.00001, "[3] vectorBExpected == vectorB.Constrain(0, min, max)");
	UtAssert_True(fabs(vectorB[4] - vectorBExpected[4]) <= 0.00001, "[4] vectorBExpected == vectorB.Constrain(1, min, max)");
	UtAssert_True(fabs(vectorB[5] - vectorBExpected[5]) <= 0.00001, "[5] vectorBExpected == vectorB.Constrain(2, min, max)");
	UtAssert_True(fabs(vectorB[6] - vectorBExpected[6]) <= 0.00001, "[6] vectorBExpected == vectorB.Constrain(2, min, max)");
	UtAssert_True(fabs(vectorB[7] - vectorBExpected[7]) <= 0.00001, "[7] vectorBExpected == vectorB.Constrain(0, min, max)");
	UtAssert_True(fabs(vectorB[8] - vectorBExpected[8]) <= 0.00001, "[8] vectorBExpected == vectorB.Constrain(1, min, max)");
	UtAssert_True(fabs(vectorB[9] - vectorBExpected[9]) <= 0.00001, "[9] vectorBExpected == vectorB.Constrain(2, min, max)");

}


void Test_Vector10F_PlusEquals(void)
{
    
	math::Vector10F vectorA(1.5f, 2.5f, 3.5f, 4.5f, 5.5f, 6.5f, 7.5f, 8.5f, 9.5f, 10.5f);
	math::Vector10F vectorB(10.5f, 20.5f, 30.5f, 40.5f, 50.5f, 60.5f, 70.5f, 80.5f, 90.5f, 100.5f);
	math::Vector10F vectorActual;
	math::Vector10F vectorExpected;
    
    vectorActual = vectorA;
    vectorActual += vectorB;
    
    vectorExpected[0] = 12.0f;
    vectorExpected[1] = 23.0f;
    vectorExpected[2] = 34.0f;
    vectorExpected[3] = 45.0f;
    vectorExpected[4] = 56.0f;
    vectorExpected[5] = 67.0f;
    vectorExpected[6] = 78.0f;
    vectorExpected[7] = 89.0f;
    vectorExpected[8] = 100.0f;
    vectorExpected[9] = 111.0f;

	UtAssert_True(vectorExpected[0] == vectorActual[0], "[0] vectorExpected == vectorActual");
	UtAssert_True(vectorExpected[1] == vectorActual[1], "[1] vectorExpected == vectorActual");
	UtAssert_True(vectorExpected[2] == vectorActual[2], "[2] vectorExpected == vectorActual");
	UtAssert_True(vectorExpected[3] == vectorActual[3], "[3] vectorExpected == vectorActual");
	UtAssert_True(vectorExpected[4] == vectorActual[4], "[4] vectorExpected == vectorActual");
	UtAssert_True(vectorExpected[5] == vectorActual[5], "[5] vectorExpected == vectorActual");
	UtAssert_True(vectorExpected[6] == vectorActual[6], "[6] vectorExpected == vectorActual");
	UtAssert_True(vectorExpected[7] == vectorActual[7], "[7] vectorExpected == vectorActual");
	UtAssert_True(vectorExpected[8] == vectorActual[8], "[8] vectorExpected == vectorActual");
	UtAssert_True(vectorExpected[9] == vectorActual[9], "[9] vectorExpected == vectorActual");
}


void Test_Vector10F_Normalize(void)
{
    float temp = 0;
	math::Vector10F vectorA(1.5f, 2.5f, 3.5f, 4.5f, 5.5f, 6.5f, 7.5f, 8.5f, 9.5f, 10.5f);
	math::Vector10F vectorAExpected;

    vectorA.Normalize();
    
    temp = sqrtf((1.5 * 1.5) + (2.5 * 2.5) + (3.5 * 3.5) + (4.5 * 4.5) + (5.5 * 5.5) + (6.5 * 6.5) + (7.5 * 7.5) + (8.5 * 8.5) + (9.5 * 9.5) + (10.5 * 10.5));
    
    vectorAExpected[0] = 1.5f / temp;
    vectorAExpected[1] = 2.5f / temp;
    vectorAExpected[2] = 3.5f / temp;
    vectorAExpected[3] = 4.5f / temp;
    vectorAExpected[4] = 5.5f / temp;
    vectorAExpected[5] = 6.5f / temp;
    vectorAExpected[6] = 7.5f / temp;
    vectorAExpected[7] = 8.5f / temp;
    vectorAExpected[8] = 9.5f / temp;
    vectorAExpected[9] = 10.5f / temp;

	UtAssert_True(fabs(vectorA[0] - vectorAExpected[0]) <= 0.00001, "[0] vectorAExpected == vectorA.Constrain(0, min, max)");
	UtAssert_True(fabs(vectorA[1] - vectorAExpected[1]) <= 0.00001, "[1] vectorAExpected == vectorA.Constrain(1, min, max)");
	UtAssert_True(fabs(vectorA[2] - vectorAExpected[2]) <= 0.00001, "[2] vectorAExpected == vectorA.Constrain(2, min, max)");
	UtAssert_True(fabs(vectorA[3] - vectorAExpected[3]) <= 0.00001, "[3] vectorAExpected == vectorA.Constrain(0, min, max)");
	UtAssert_True(fabs(vectorA[4] - vectorAExpected[4]) <= 0.00001, "[4] vectorAExpected == vectorA.Constrain(1, min, max)");
	UtAssert_True(fabs(vectorA[5] - vectorAExpected[5]) <= 0.00001, "[5] vectorAExpected == vectorA.Constrain(2, min, max)");
	UtAssert_True(fabs(vectorA[6] - vectorAExpected[6]) <= 0.00001, "[6] vectorAExpected == vectorA.Constrain(2, min, max)");
	UtAssert_True(fabs(vectorA[7] - vectorAExpected[7]) <= 0.00001, "[7] vectorAExpected == vectorA.Constrain(0, min, max)");
	UtAssert_True(fabs(vectorA[8] - vectorAExpected[8]) <= 0.00001, "[8] vectorAExpected == vectorA.Constrain(1, min, max)");
	UtAssert_True(fabs(vectorA[9] - vectorAExpected[9]) <= 0.00001, "[9] vectorAExpected == vectorA.Constrain(2, min, max)");
}


void Test_Vector10F_Normalized(void)
{
    float temp = 0;
	math::Vector10F vectorA(1.5f, 2.5f, 3.5f, 4.5f, 5.5f, 6.5f, 7.5f, 8.5f, 9.5f, 10.5f);
	math::Vector10F vectorAExpected;

    vectorA = vectorA.Normalized();
    
    temp = sqrtf((1.5 * 1.5) + (2.5 * 2.5) + (3.5 * 3.5) + (4.5 * 4.5) + (5.5 * 5.5) + (6.5 * 6.5) + (7.5 * 7.5) + (8.5 * 8.5) + (9.5 * 9.5) + (10.5 * 10.5));
    
    vectorAExpected[0] = 1.5f / temp;
    vectorAExpected[1] = 2.5f / temp;
    vectorAExpected[2] = 3.5f / temp;
    vectorAExpected[3] = 4.5f / temp;
    vectorAExpected[4] = 5.5f / temp;
    vectorAExpected[5] = 6.5f / temp;
    vectorAExpected[6] = 7.5f / temp;
    vectorAExpected[7] = 8.5f / temp;
    vectorAExpected[8] = 9.5f / temp;
    vectorAExpected[9] = 10.5f / temp;

	UtAssert_True(fabs(vectorA[0] - vectorAExpected[0]) <= 0.00001, "[0] vectorAExpected == vectorA.Constrain(0, min, max)");
	UtAssert_True(fabs(vectorA[1] - vectorAExpected[1]) <= 0.00001, "[1] vectorAExpected == vectorA.Constrain(1, min, max)");
	UtAssert_True(fabs(vectorA[2] - vectorAExpected[2]) <= 0.00001, "[2] vectorAExpected == vectorA.Constrain(2, min, max)");
	UtAssert_True(fabs(vectorA[3] - vectorAExpected[3]) <= 0.00001, "[3] vectorAExpected == vectorA.Constrain(0, min, max)");
	UtAssert_True(fabs(vectorA[4] - vectorAExpected[4]) <= 0.00001, "[4] vectorAExpected == vectorA.Constrain(1, min, max)");
	UtAssert_True(fabs(vectorA[5] - vectorAExpected[5]) <= 0.00001, "[5] vectorAExpected == vectorA.Constrain(2, min, max)");
	UtAssert_True(fabs(vectorA[6] - vectorAExpected[6]) <= 0.00001, "[6] vectorAExpected == vectorA.Constrain(2, min, max)");
	UtAssert_True(fabs(vectorA[7] - vectorAExpected[7]) <= 0.00001, "[7] vectorAExpected == vectorA.Constrain(0, min, max)");
	UtAssert_True(fabs(vectorA[8] - vectorAExpected[8]) <= 0.00001, "[8] vectorAExpected == vectorA.Constrain(1, min, max)");
	UtAssert_True(fabs(vectorA[9] - vectorAExpected[9]) <= 0.00001, "[9] vectorAExpected == vectorA.Constrain(2, min, max)");
}


void Test_Vector10F_Negate(void)
{
	math::Vector10F vectorA(1.5f, 2.5f, 3.5f, 4.5f, 5.5f, 6.5f, 7.5f, 8.5f, 9.5f, 10.5f);
	vectorA = -vectorA;
    
	UtAssert_True(vectorA[0] == -1.5f, "vectorA[0] == -1.5f");
	UtAssert_True(vectorA[1] == -2.5f, "vectorA[1] == -2.5f");
	UtAssert_True(vectorA[2] == -3.5f, "vectorA[2] == -3.5f");
	UtAssert_True(vectorA[3] == -4.5f, "vectorA[3] == -4.5f");
	UtAssert_True(vectorA[4] == -5.5f, "vectorA[4] == -5.5f");
	UtAssert_True(vectorA[5] == -6.5f, "vectorA[5] == -6.5f");
	UtAssert_True(vectorA[6] == -7.5f, "vectorA[6] == -7.5f");
	UtAssert_True(vectorA[7] == -8.5f, "vectorA[7] == -8.5f");
	UtAssert_True(vectorA[8] == -9.5f, "vectorA[8] == -9.5f");
	UtAssert_True(vectorA[9] == -10.5f, "vectorA[8] == -10.5f");
}


/* Auto generated with GNU Octave, version 4.0.0 */
void Test_Vector10F_Add_10F_Random(void)
{
    int i, j = 0;

    math::Vector10F A (
        {0.025518,0.404600,0.713228,0.090022,-0.785246,0.763062,-0.603314,-0.427869,0.444792,-0.471371}
    );

    math::Vector10F B (
        {0.328385,0.293871,0.745552,-0.501815,0.882076,-0.813979,0.209484,-0.234234,0.378351,0.521002}
    );

    math::Vector10F expected (
        {0.353903,0.698470,1.458780,-0.411794,0.096830,-0.050917,-0.393830,-0.662104,0.823143,0.049631}
    );

    math::Vector10F result;
    result.Zero();

    result = A + B;

    for(i = 0; i < 10; i++)
    {
        UtAssert_True(fabs(result[i] - expected[i]) < 0.00001f, "fabs(result[i] - expected[i]) < 0.00001f");

    }
}


/* Auto generated with GNU Octave, version 4.0.0 */
void Test_Vector10F_PlusEquals_10F_Random(void)
{
    int i, j = 0;

    math::Vector10F A (
        {0.025518,0.404600,0.713228,0.090022,-0.785246,0.763062,-0.603314,-0.427869,0.444792,-0.471371}
    );

    math::Vector10F B (
        {0.328385,0.293871,0.745552,-0.501815,0.882076,-0.813979,0.209484,-0.234234,0.378351,0.521002}
    );

    math::Vector10F expected (
        {0.353903,0.698470,1.458780,-0.411794,0.096830,-0.050917,-0.393830,-0.662104,0.823143,0.049631}
    );

    math::Vector10F result;
    result.Zero();

    A += B;

    result = A;

    for(i = 0; i < 10; i++)
    {
        UtAssert_True(fabs(result[i] - expected[i]) < 0.00001f, "fabs(result[i] - expected[i]) < 0.00001f");

    }
}


/* Auto generated with GNU Octave, version 4.0.0 */
void Test_Vector10F_Sub_10F_Random(void)
{
    int i, j = 0;

    math::Vector10F A (
        {0.296611,0.072534,-0.648729,-0.556663,0.391361,-0.013357,-0.403058,0.310833,0.714922,-0.855171}
    );

    math::Vector10F B (
        {0.317210,-0.987583,-0.404500,0.493143,-0.402748,-0.216720,0.282142,-0.009202,0.038401,0.422956}
    );

    math::Vector10F expected (
        {-0.020599,1.060117,-0.244229,-1.049806,0.794110,0.203363,-0.685200,0.320035,0.676521,-1.278127}
    );

    math::Vector10F result;
    result.Zero();

    result = A - B;

    for(i = 0; i < 10; i++)
    {
        UtAssert_True(fabs(result[i] - expected[i]) < 0.00001f, "fabs(result[i] - expected[i]) < 0.00001f");

    }
}

