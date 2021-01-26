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

#include "Vector6F_test.hpp"
#include <math/Vector6F.hpp>

#include "utassert.h"
#include <float.h>


void Test_Vector6F_Constructor(void)
{
	math::Vector6F vector(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f);

	/* Verify results */
	UtAssert_True(vector[0] == 1.0f, "vector[0] == 1.0f");
	UtAssert_True(vector[1] == 2.0f, "vector[1] == 2.0f");
	UtAssert_True(vector[2] == 3.0f, "vector[2] == 3.0f");
	UtAssert_True(vector[3] == 4.0f, "vector[3] == 4.0f");
	UtAssert_True(vector[4] == 5.0f, "vector[4] == 5.0f");
	UtAssert_True(vector[5] == 6.0f, "vector[5] == 6.0f");
}


void Test_Vector6F_IndexOutOfBounds(void)
{
	math::Vector6F vector(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f);

	/* Verify results */
	UtAssert_True(vector[0] == 1.0f, "vector[0] == 1.0f");
	UtAssert_True(vector[1] == 2.0f, "vector[1] == 2.0f");
	UtAssert_True(vector[2] == 3.0f, "vector[2] == 3.0f");
	UtAssert_True(vector[3] == 4.0f, "vector[3] == 4.0f");
	UtAssert_True(vector[4] == 5.0f, "vector[4] == 5.0f");
	UtAssert_True(vector[5] == 6.0f, "vector[5] == 6.0f");
	UtAssert_True(isnan(vector[6]), "vector[6] == nan");
}


void Test_Vector6F_Length(void)
{
	math::Vector6F vector(1.5f, 2.5f, 3.5f, 4.5f, 5.5f, 6.5f);
	float expectedLength = sqrtf((1.5f)*(1.5f) + (2.5f)*(2.5f) + (3.5f)*(3.5f) +
                                 (4.5f)*(4.5f) + (5.5f)*(5.5f) + (6.5f)*(6.5f)
    );
    float actualLength = vector.Length();

	/* Verify results */
    UtAssert_True(fabs(expectedLength - actualLength) <= FLT_EPSILON, "vector.Length() == expectedLength");
}


void Test_Vector6F_Zero(void)
{
	math::Vector6F vector(1.5f, 2.5f, 3.5f, 4.5f, 5.5f, 6.5f);
	vector.Zero();

	/* Verify results */
    UtAssert_True(vector[0] == 0.0f, "vector[0] == 0.0f");
    UtAssert_True(vector[1] == 0.0f, "vector[1] == 0.0f");
    UtAssert_True(vector[2] == 0.0f, "vector[2] == 0.0f");
    UtAssert_True(vector[3] == 0.0f, "vector[3] == 0.0f");
    UtAssert_True(vector[4] == 0.0f, "vector[4] == 0.0f");
    UtAssert_True(vector[5] == 0.0f, "vector[5] == 0.0f");
}



void Test_Vector6F_ArrayOperator(void)
{
	math::Vector6F vector(1.5f, 2.5f, 3.5f, 4.5f, 5.5f, 6.5f);
	float nanResult = vector[6];

    UtAssert_True(vector[0] == 1.5f, "vector[0] == 1.5f");
    UtAssert_True(vector[1] == 2.5f, "vector[1] == 2.5f");
    UtAssert_True(vector[2] == 3.5f, "vector[2] == 3.5f");
    UtAssert_True(vector[3] == 4.5f, "vector[3] == 4.5f");
    UtAssert_True(vector[4] == 5.5f, "vector[4] == 5.5f");
    UtAssert_True(vector[5] == 6.5f, "vector[5] == 6.5f");
    UtAssert_True(isnan(vector[6]), "vector[6] == NAN");

    UtAssert_True(1.5f == vector[0], "1.5f == vector[0]");
    UtAssert_True(2.5f == vector[1], "2.5f == vector[1]");
    UtAssert_True(3.5f == vector[2], "3.5f == vector[2]");
    UtAssert_True(4.5f == vector[3], "4.5f == vector[3]");
    UtAssert_True(5.5f == vector[4], "5.5f == vector[4]");
    UtAssert_True(6.5f == vector[5], "6.5f == vector[5]");
    UtAssert_True(isnan(nanResult), "NAN == vector[6]");

}


void Test_Vector6F_AdditionOperator(void)
{
	/* vectorActual = vectorA + vectorB */
	math::Vector6F vectorA(1.5f, 2.5f, 3.5f, 4.5f, 5.5f, 6.5f);
	math::Vector6F vectorB(10.5f, 20.5f, 30.5f, 40.5f, 50.5f, 60.5f);
	math::Vector6F vectorActual = vectorA + vectorB;

	/* Verify results */
	UtAssert_True(vectorActual[0] == 12.0, "vectorActual[0] == vectorA[0] + vectorB[0]");
	UtAssert_True(vectorActual[1] == 23.0, "vectorActual[1] == vectorA[1] + vectorB[1]");
	UtAssert_True(vectorActual[2] == 34.0, "vectorActual[2] == vectorA[2] + vectorB[2]");
	UtAssert_True(vectorActual[3] == 45.0, "vectorActual[3] == vectorA[3] + vectorB[3]");
	UtAssert_True(vectorActual[4] == 56.0, "vectorActual[4] == vectorA[4] + vectorB[4]");
	UtAssert_True(vectorActual[5] == 67.0, "vectorActual[5] == vectorA[5] + vectorB[5]");
}


void Test_Vector6F_SubtractionOperator(void)
{
	/* vectorActual = vectorA - vectorB */
	math::Vector6F vectorA(1.5f, 2.5f, 3.5f, 4.5f, 5.5f, 6.5f);
	math::Vector6F vectorB(10.5f, 20.5f, 30.5f, 40.5f, 50.5f, 60.5f);
	math::Vector6F vectorActual = vectorA - vectorB;

	/* Verify results */
	UtAssert_True(vectorActual[0] == -9.0, "vectorActual[0] == vectorA[0] - vectorB[0]");
	UtAssert_True(vectorActual[1] == -18.0, "vectorActual[1] == vectorA[1] - vectorB[1]");
	UtAssert_True(vectorActual[2] == -27.0, "vectorActual[2] == vectorA[2] - vectorB[2]");
	UtAssert_True(vectorActual[3] == -36.0, "vectorActual[3] == vectorA[3] - vectorB[3]");
	UtAssert_True(vectorActual[4] == -45.0, "vectorActual[4] == vectorA[4] - vectorB[4]");
	UtAssert_True(vectorActual[5] == -54.0, "vectorActual[5] == vectorA[5] - vectorB[5]");
}


void Test_Vector6F_ScalarMultiplyOperator(void)
{
	/* actual = vectorA * scalar */
	math::Vector6F vectorA(1.5f, 2.5f, 3.5f, 4.5f, 5.5f, 6.5f);
	float scalar = 10.5f;
	math::Vector6F vectorActual = vectorA * scalar;
	math::Vector6F vectorExpected;

	/* Verify results */
	vectorExpected[0] = 15.75;
	vectorExpected[1] = 26.25;
	vectorExpected[2] = 36.75;
	vectorExpected[3] = 47.25;
	vectorExpected[4] = 57.75;
	vectorExpected[5] = 68.25;


	UtAssert_True(vectorExpected[0] == vectorActual[0], "[0] vectorExpected == vectorA * scalar");
	UtAssert_True(vectorExpected[1] == vectorActual[1], "[1] vectorExpected == vectorA * scalar");
	UtAssert_True(vectorExpected[2] == vectorActual[2], "[2] vectorExpected == vectorA * scalar");
	UtAssert_True(vectorExpected[3] == vectorActual[3], "[3] vectorExpected == vectorA * scalar");
	UtAssert_True(vectorExpected[4] == vectorActual[4], "[4] vectorExpected == vectorA * scalar");
	UtAssert_True(vectorExpected[5] == vectorActual[5], "[5] vectorExpected == vectorA * scalar");
}


void Test_Vector6F_ScalarDivOperator(void)
{
	/* actual = vectorA / scalar */
	math::Vector6F vectorA(1.5f, 2.5f, 3.5f, 4.5f, 5.5f, 6.5f);
	float scalar = 10.5f;
	math::Vector6F vectorActual = vectorA / scalar;
	math::Vector6F vectorExpected;

	/* Verify results */
	vectorExpected[0] = 0.142857f;
	vectorExpected[1] = 0.238095f;
	vectorExpected[2] = 0.333333f;
	vectorExpected[3] = 0.428571f;
	vectorExpected[4] = 0.523809f;
	vectorExpected[5] = 0.619047f;

	UtAssert_True(fabs(vectorActual[0] - vectorExpected[0]) <= 0.00001, "[0] vectorExpected == vectorA / scalar");
	UtAssert_True(fabs(vectorActual[1] - vectorExpected[1]) <= 0.00001, "[1] vectorExpected == vectorA / scalar");
	UtAssert_True(fabs(vectorActual[2] - vectorExpected[2]) <= 0.00001, "[2] vectorExpected == vectorA / scalar");
	UtAssert_True(fabs(vectorActual[3] - vectorExpected[3]) <= 0.00001, "[3] vectorExpected == vectorA / scalar");
	UtAssert_True(fabs(vectorActual[4] - vectorExpected[4]) <= 0.00001, "[4] vectorExpected == vectorA / scalar");
	UtAssert_True(fabs(vectorActual[5] - vectorExpected[5]) <= 0.00001, "[5] vectorExpected == vectorA / scalar");

}


void Test_Vector6F_DotProductOperator(void)
{
	/* actual = vectorA * vectorB */
	math::Vector6F vectorA(1.5f, 2.5f, 3.5f, 4.5f, 5.5f, 6.5f);
	math::Vector6F vectorB(10.5f, 20.5f, 30.5f, 40.5f, 50.5f, 60.5f);

	float actualAB = vectorA * vectorB;

	float expectedAB = 1027.0f;
    
	/* Verify results */
	UtAssert_True(actualAB == expectedAB, "expected == vectorA dot vectorB");
}


void Test_Vector6F_EMult(void)
{
	/* actual = vectorA / scalar */
	math::Vector6F vectorA(1.5f, 2.5f, 3.5f, 4.5f, 5.5f, 6.5f);
	math::Vector6F vectorB(10.5f, 20.5f, 30.5f, 40.5f, 50.5f, 60.5f);
	math::Vector6F vectorActual = vectorA.EMult(vectorB);
	math::Vector6F vectorExpected;

	/* Verify results */
	vectorExpected[0] = 15.75f;
	vectorExpected[1] = 51.25f;
	vectorExpected[2] = 106.75f;
	vectorExpected[3] = 182.25f;
	vectorExpected[4] = 277.75f;
	vectorExpected[5] = 393.25f;

	UtAssert_True(fabs(vectorActual[0] - vectorExpected[0]) <= 0.00001, "[0] vectorExpected == vectorA.EMult(vectorB)");
	UtAssert_True(fabs(vectorActual[1] - vectorExpected[1]) <= 0.00001, "[1] vectorExpected == vectorA.EMult(vectorB)");
	UtAssert_True(fabs(vectorActual[2] - vectorExpected[2]) <= 0.00001, "[2] vectorExpected == vectorA.EMult(vectorB)");
	UtAssert_True(fabs(vectorActual[3] - vectorExpected[3]) <= 0.00001, "[3] vectorExpected == vectorA.EMult(vectorB)");
	UtAssert_True(fabs(vectorActual[4] - vectorExpected[4]) <= 0.00001, "[4] vectorExpected == vectorA.EMult(vectorB)");
	UtAssert_True(fabs(vectorActual[5] - vectorExpected[5]) <= 0.00001, "[5] vectorExpected == vectorA.EMult(vectorB)");

}


void Test_Vector6F_EDivide(void)
{
	/* actual = vectorA / scalar */
	math::Vector6F vectorA(1.5f, 2.5f, 3.5f, 4.5f, 5.5f, 6.5f);
	math::Vector6F vectorB(10.5f, 20.5f, 30.5f, 40.5f, 50.5f, 60.5f);
	math::Vector6F vectorActual = vectorA.EDivide(vectorB);
	math::Vector6F vectorExpected;

	/* Verify results */
	vectorExpected[0] = 0.142857f;
	vectorExpected[1] = 0.121951f;
	vectorExpected[2] = 0.114754f;
	vectorExpected[3] = 0.111111f;
	vectorExpected[4] = 0.108910f;
	vectorExpected[5] = 0.107438f;

	UtAssert_True(fabs(vectorActual[0] - vectorExpected[0]) <= 0.00001, "[0] vectorExpected == vectorA.EMult(vectorB)");
	UtAssert_True(fabs(vectorActual[1] - vectorExpected[1]) <= 0.00001, "[1] vectorExpected == vectorA.EMult(vectorB)");
	UtAssert_True(fabs(vectorActual[2] - vectorExpected[2]) <= 0.00001, "[2] vectorExpected == vectorA.EMult(vectorB)");
	UtAssert_True(fabs(vectorActual[3] - vectorExpected[3]) <= 0.00001, "[3] vectorExpected == vectorA.EMult(vectorB)");
	UtAssert_True(fabs(vectorActual[4] - vectorExpected[4]) <= 0.00001, "[4] vectorExpected == vectorA.EMult(vectorB)");
	UtAssert_True(fabs(vectorActual[5] - vectorExpected[5]) <= 0.00001, "[5] vectorExpected == vectorA.EMult(vectorB)");

}


void Test_Vector6F_Constrain(void)
{
	/* actual = vectorA / scalar */
	math::Vector6F vectorA(1.5f, 2.5f, 3.5f, 4.5f, 5.5f, 6.5f);
	math::Vector6F vectorB(-1.5f, -2.5f, -3.5f, -4.5f, -5.5f, -6.5f);
	math::Vector6F vectorAExpected;
	math::Vector6F vectorBExpected;

	vectorA.Constrain(0, 0.5, 1.0);
	vectorA.Constrain(1, 1.5, 2.7);
	vectorA.Constrain(2, 2.5, 3.0);
	vectorA.Constrain(3, 3.5, 4.0);
	vectorA.Constrain(4, 4.5, 5.7);
	vectorA.Constrain(5, 5.5, 6.2);

	vectorB.Constrain(0, -1.0, -0.5);
	vectorB.Constrain(1, -2.7, -1.5);
	vectorB.Constrain(2, -3.0, -2.5);
	vectorB.Constrain(3, -4.0, -3.5);
	vectorB.Constrain(4, -5.7, -4.5);
	vectorB.Constrain(5, -6.2, -5.5);

	/* Verify results */
	vectorAExpected[0] = 1.0f;
	vectorAExpected[1] = 2.5f;
	vectorAExpected[2] = 3.0f;
	vectorAExpected[3] = 4.0f;
	vectorAExpected[4] = 5.5f;
	vectorAExpected[5] = 6.2f;

	vectorBExpected[0] = -1.0f;
	vectorBExpected[1] = -2.5f;
	vectorBExpected[2] = -3.0f;
	vectorBExpected[3] = -4.0f;
	vectorBExpected[4] = -5.5f;
	vectorBExpected[5] = -6.2f;

	UtAssert_True(fabs(vectorA[0] - vectorAExpected[0]) <= 0.00001, "[0] vectorAExpected == vectorA.Constrain(0, min, max)");
	UtAssert_True(fabs(vectorA[1] - vectorAExpected[1]) <= 0.00001, "[1] vectorAExpected == vectorA.Constrain(1, min, max)");
	UtAssert_True(fabs(vectorA[2] - vectorAExpected[2]) <= 0.00001, "[2] vectorAExpected == vectorA.Constrain(2, min, max)");
	UtAssert_True(fabs(vectorA[3] - vectorAExpected[3]) <= 0.00001, "[3] vectorAExpected == vectorA.Constrain(0, min, max)");
	UtAssert_True(fabs(vectorA[4] - vectorAExpected[4]) <= 0.00001, "[4] vectorAExpected == vectorA.Constrain(1, min, max)");
	UtAssert_True(fabs(vectorA[5] - vectorAExpected[5]) <= 0.00001, "[5] vectorAExpected == vectorA.Constrain(2, min, max)");

	UtAssert_True(fabs(vectorB[0] - vectorBExpected[0]) <= 0.00001, "[0] vectorBExpected == vectorB.Constrain(0, min, max)");
	UtAssert_True(fabs(vectorB[1] - vectorBExpected[1]) <= 0.00001, "[1] vectorBExpected == vectorB.Constrain(1, min, max)");
	UtAssert_True(fabs(vectorB[2] - vectorBExpected[2]) <= 0.00001, "[2] vectorBExpected == vectorB.Constrain(2, min, max)");
	UtAssert_True(fabs(vectorB[3] - vectorBExpected[3]) <= 0.00001, "[3] vectorBExpected == vectorB.Constrain(0, min, max)");
	UtAssert_True(fabs(vectorB[4] - vectorBExpected[4]) <= 0.00001, "[4] vectorBExpected == vectorB.Constrain(1, min, max)");
	UtAssert_True(fabs(vectorB[5] - vectorBExpected[5]) <= 0.00001, "[5] vectorBExpected == vectorB.Constrain(2, min, max)");
}


void Test_Vector6F_Pow(void)
{
	/* vectorActual = vectorA + vectorB */
	math::Vector6F vectorA(1.5f, 2.5f, 3.5f, 4.5f, 5.5f, 6.5f);
	math::Vector6F vectorB;
    
	vectorB = vectorA.pow(2.0f);

	/* Verify results */
	UtAssert_True(vectorB[0] == 2.25f, "vectorB[0] == vectorA[0].pow(2.0f)");
	UtAssert_True(vectorB[1] == 6.25f, "vectorB[1] == vectorA[1].pow(2.0f)");
	UtAssert_True(vectorB[2] == 12.25f, "vectorB[2] == vectorA[2].pow(2.0f)");
	UtAssert_True(vectorB[3] == 20.25f, "vectorB[3] == vectorA[3].pow(2.0f)");
	UtAssert_True(vectorB[4] == 30.25f, "vectorB[4] == vectorA[4].pow(2.0f)");
	UtAssert_True(vectorB[5] == 42.25f, "vectorB[5] == vectorA[5].pow(2.0f)");
}


void Test_Vector6F_Normalized(void)
{
	/* vectorActual = vectorA + vectorB */
	math::Vector6F vectorA(1.5f, 2.5f, 3.5f, 4.5f, 5.5f, 6.5f);
	math::Vector6F vectorActual;
    math::Vector6F vectorExpected(0.140797f, 0.234662f, 0.328526f, 0.422391f,
                                  0.516256f, 0.610120f);
    
	vectorActual = vectorA.Normalized();

	/* Verify results */
	UtAssert_True(fabs(vectorActual[0] - vectorExpected[0]) <= 0.00001, "[0] vectorExpected == vectorActual");
	UtAssert_True(fabs(vectorActual[1] - vectorExpected[1]) <= 0.00001, "[1] vectorExpected == vectorActual");
	UtAssert_True(fabs(vectorActual[2] - vectorExpected[2]) <= 0.00001, "[2] vectorExpected == vectorActual");
	UtAssert_True(fabs(vectorActual[3] - vectorExpected[3]) <= 0.00001, "[3] vectorExpected == vectorActual");
	UtAssert_True(fabs(vectorActual[4] - vectorExpected[4]) <= 0.00001, "[4] vectorExpected == vectorActual");
	UtAssert_True(fabs(vectorActual[5] - vectorExpected[5]) <= 0.00001, "[5] vectorExpected == vectorActual");
}


void Test_Vector6F_Normalize(void)
{
	/* vectorActual = vectorA + vectorB */
	math::Vector6F vectorA(1.5f, 2.5f, 3.5f, 4.5f, 5.5f, 6.5f);
	math::Vector6F vectorActual;
    math::Vector6F vectorExpected(0.140797f, 0.234662f, 0.328526f, 0.422391f,
                                  0.516256f, 0.610120f);
    
    vectorA.Normalize();
	vectorActual = vectorA;

	/* Verify results */
	UtAssert_True(fabs(vectorActual[0] - vectorExpected[0]) <= 0.00001, "[0] vectorExpected == vectorActual");
	UtAssert_True(fabs(vectorActual[1] - vectorExpected[1]) <= 0.00001, "[1] vectorExpected == vectorActual");
	UtAssert_True(fabs(vectorActual[2] - vectorExpected[2]) <= 0.00001, "[2] vectorExpected == vectorActual");
	UtAssert_True(fabs(vectorActual[3] - vectorExpected[3]) <= 0.00001, "[3] vectorExpected == vectorActual");
	UtAssert_True(fabs(vectorActual[4] - vectorExpected[4]) <= 0.00001, "[4] vectorExpected == vectorActual");
	UtAssert_True(fabs(vectorActual[5] - vectorExpected[5]) <= 0.00001, "[5] vectorExpected == vectorActual");

}

void Test_Vector6F_Negate(void)
{
	/* vectorActual = vectorA + vectorB */
	math::Vector6F vectorA(1.5f, 2.5f, 3.5f, 4.5f, 5.5f, 6.5f);
	math::Vector6F vectorActual;
	math::Vector6F vectorExpected(-1.5f, -2.5f, -3.5f, -4.5f, -5.5f, -6.5f);

	vectorActual = -vectorA;
    vectorA = -vectorA;

	/* Verify results */
	UtAssert_True(vectorActual[0] == vectorExpected[0], "[0] vectorExpected == vectorActual");
	UtAssert_True(vectorActual[1] == vectorExpected[1], "[1] vectorExpected == vectorActual");
	UtAssert_True(vectorActual[2] == vectorExpected[2], "[2] vectorExpected == vectorActual");
	UtAssert_True(vectorActual[3] == vectorExpected[3], "[3] vectorExpected == vectorActual");
	UtAssert_True(vectorActual[4] == vectorExpected[4], "[4] vectorExpected == vectorActual");
	UtAssert_True(vectorActual[5] == vectorExpected[5], "[5] vectorExpected == vectorActual");

	UtAssert_True(vectorA[0] == vectorExpected[0], "[0] vectorExpected == vectorA");
	UtAssert_True(vectorA[1] == vectorExpected[1], "[1] vectorExpected == vectorA");
	UtAssert_True(vectorA[2] == vectorExpected[2], "[2] vectorExpected == vectorA");
	UtAssert_True(vectorA[3] == vectorExpected[3], "[3] vectorExpected == vectorA");
	UtAssert_True(vectorA[4] == vectorExpected[4], "[4] vectorExpected == vectorA");
	UtAssert_True(vectorA[5] == vectorExpected[5], "[5] vectorExpected == vectorA");


}


/* Auto generated with GNU Octave, version 4.0.0 */
void Test_Vector6F_Add_6F_Random(void)
{
    int i, j = 0;

    math::Vector6F A (
        {-0.887414,-0.119126,0.079142,0.499716,0.354095,0.723813}
    );

    math::Vector6F B (
        {-0.286455,0.349328,-0.464778,0.454298,0.534963,-0.156184}
    );

    math::Vector6F expected (
        {-1.173869,0.230201,-0.385635,0.954014,0.889058,0.567630}
    );

    math::Vector6F result;
    result.Zero();

    result = A + B;

    for(i = 0; i < 6; i++)
    {
        UtAssert_True(fabs(result[i] - expected[i]) < 0.00001f, "fabs(result[i] - expected[i]) < 0.00001f");

    }
}


/* Auto generated with GNU Octave, version 4.0.0 */
void Test_Vector6F_Sub_6F_Random(void)
{
    int i, j = 0;

    math::Vector6F A (
        {-0.955791,0.700360,-0.770943,-0.839343,-0.449211,0.917790}
    );

    math::Vector6F B (
        {0.712302,-0.770698,0.392407,0.275895,0.684654,-0.541848}
    );

    math::Vector6F expected (
        {-1.668094,1.471058,-1.163350,-1.115237,-1.133865,1.459638}
    );

    math::Vector6F result;
    result.Zero();

    result = A - B;

    for(i = 0; i < 6; i++)
    {
        UtAssert_True(fabs(result[i] - expected[i]) < 0.00001f, "fabs(result[i] - expected[i]) < 0.00001f");

    }
}


