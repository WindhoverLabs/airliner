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

#include "Quaternion_test.hpp"
#include <math/Quaternion.hpp>
#include <math/Vector3F.hpp>
#include <math/Matrix3F3.hpp>

#include "utassert.h"
#include <float.h>


void Test_Quaternion_Constructor(void)
{
	math::Quaternion quat(1.0f, 2.0f, 3.0f, 4.0f);

	/* Verify results */
	UtAssert_True(quat[0] == 1.0f, "quat[0] == 1.0f");
	UtAssert_True(quat[1] == 2.0f, "quat[1] == 2.0f");
	UtAssert_True(quat[2] == 3.0f, "quat[2] == 3.0f");
	UtAssert_True(quat[3] == 4.0f, "quat[3] == 4.0f");
}


void Test_Quaternion_Matrix3F3Constructor(void)
{
	/* This is actual flight data collected from PX4 code which also
	 * implemented the algorithm defined in
	 * http://www.cs.ucr.edu/~vbz/resources/quatut.pdf
	 */
	math::Matrix3F3 matrixA(
		{0.028104f, 0.998996f, 0.034875f},
		{-0.999343f, 0.028878f, -0.021891f},
		{-0.022876f, -0.034237f, 0.999152f}
	);
	math::Matrix3F3 matrixB(
		{0.028108f, 0.998996f, 0.034883f},
		{-0.999343f, 0.028882f, -0.021877f},
		{-0.022863f, -0.034245f, 0.999152f}
	);
	math::Matrix3F3 matrixC(
		{0.028141f, 0.998994f, 0.034930f},
		{-0.999342f, 0.028916f, -0.021892f},
		{-0.022880f, -0.034291f, 0.999150f}
	);
	math::Quaternion quatA(matrixA);
	math::Quaternion quatB(matrixB);
	math::Quaternion quatC(matrixC);

	/* Verify results */
	UtAssert_True(fabs(quatA[0] - 0.716961f) < 0.00001, "[0]:  Quaternion A constructed from rotation matrix.");
	UtAssert_True(fabs(quatA[1] - -0.004305f) < 0.00001, "[1]:  Quaternion A constructed from rotation matrix.");
	UtAssert_True(fabs(quatA[2] - 0.020137f) < 0.00001, "[2]:  Quaternion A constructed from rotation matrix.");
	UtAssert_True(fabs(quatA[3] - -0.696809f) < 0.00001, "[3]:  Quaternion A constructed from rotation matrix.");

	UtAssert_True(fabs(quatB[0] - 0.716963f) < 0.00001, "[0]:  Quaternion B constructed from rotation matrix.");
	UtAssert_True(fabs(quatB[1] - -0.004312f) < 0.00001, "[1]:  Quaternion B constructed from rotation matrix.");
	UtAssert_True(fabs(quatB[2] - 0.020135f) < 0.00001, "[2]:  Quaternion B constructed from rotation matrix.");
	UtAssert_True(fabs(quatB[3] - -0.696807f) < 0.00001, "[3]:  Quaternion B constructed from rotation matrix.");

	UtAssert_True(fabs(quatC[0] - 0.716974f) < 0.00001, "[0]:  Quaternion C constructed from rotation matrix.");
	UtAssert_True(fabs(quatC[1] - -0.004323f) < 0.00001, "[1]:  Quaternion C constructed from rotation matrix.");
	UtAssert_True(fabs(quatC[2] - 0.020158f) < 0.00001, "[2]:  Quaternion C constructed from rotation matrix.");
	UtAssert_True(fabs(quatC[3] - -0.696795f) < 0.00001, "[3]:  Quaternion C constructed from rotation matrix.");
}


void Test_Quaternion_IndexOutOfBounds(void)
{
	math::Quaternion quat(1.0f, 2.0f, 3.0f, 4.0f);

	/* Verify results */
	UtAssert_True(quat[0] == 1.0f, "quat[0] == 1.0f");
	UtAssert_True(quat[1] == 2.0f, "quat[1] == 2.0f");
	UtAssert_True(quat[2] == 3.0f, "quat[2] == 3.0f");
	UtAssert_True(quat[3] == 4.0f, "quat[3] == 4.0f");
	UtAssert_True(isnan(quat[4]), "quat[4] == nan");
}


void Test_Quaternion_ArrayOperator(void)
{
	math::Quaternion quat(1.5f, 2.5f, 3.5f, 4.5f);
	float nanResult = quat[4];

    UtAssert_True(quat[0] == 1.5f, "quat[0] == 1.5f");
    UtAssert_True(quat[1] == 2.5f, "quat[1] == 2.5f");
    UtAssert_True(quat[2] == 3.5f, "quat[2] == 3.5f");
    UtAssert_True(quat[3] == 4.5f, "quat[3] == 4.5f");
    UtAssert_True(isnan(quat[4]), "quat[4] == NAN");

    UtAssert_True(1.5f == quat[0], "1.5f == quat[0]");
    UtAssert_True(2.5f == quat[1], "2.5f == quat[1]");
    UtAssert_True(3.5f == quat[2], "3.5f == quat[2]");
    UtAssert_True(4.5f == quat[3], "4.5f == quat[3]");
    UtAssert_True(isnan(nanResult), "NAN == quat[4]");
}


void Test_Quaternion_MultiplyOperator(void)
{
    math::Quaternion quatA(1.5f, 2.5f, 3.5f, 4.5f);
    math::Quaternion quatB(1.5f, 2.5f, 3.5f, 4.5f);
    math::Quaternion quat(0.0f, 0.0f, 0.0f, 0.0f);
    math::Quaternion expected(-36.5f, 7.5f, 10.5f, 13.5f);
    float nanResult = quat[4];

    quat = quatA * quatB;
    
    UtAssert_True(expected[0] == quat[0], "-36.5f == quat[0]");
    UtAssert_True(expected[1] == quat[1], "7.5f == quat[1]");
    UtAssert_True(expected[2] == quat[2], "10.5f == quat[2]");
    UtAssert_True(expected[3] == quat[3], "13.5f == quat[3]");
    UtAssert_True(isnan(nanResult), "NAN == quat[4]");
}


void Test_Quaternion_DivideOperator(void)
{
    math::Quaternion quatA(1.5f, 2.5f, 3.5f, 4.5f);
    math::Quaternion quatB(2.5f, 3.5f, 4.5f, 5.5f);
    math::Quaternion quat(0.0f, 0.0f, 0.0f, 0.0f);
    math::Quaternion quatR(0.0f, 0.0f, 0.0f, 0.0f);
    math::Quaternion expected(1.0f, 0.0f, 0.0f, 0.0f);
    math::Quaternion expectedR(0.7681159377f, 0.0289855078f, 0.0000000000f, 0.0579710156f);
    
    float nanResult = quat[4];

    quat = quatA / quatA;

    quatR = quatA / quatB;

    UtAssert_True(expected[0] == quat[0], "1.0f == quat[0]");
    UtAssert_True(expected[1] == quat[1], "0.0f == quat[1]");
    UtAssert_True(expected[2] == quat[2], "0.0f == quat[2]");
    UtAssert_True(expected[3] == quat[3], "0.0f == quat[3]");
    UtAssert_True(expectedR[0] == quatR[0], "0.7681159377f == quat[0]");
    UtAssert_True(expectedR[1] == quatR[1], "0.0289855078f == quat[1]");
    UtAssert_True(expectedR[2] == quatR[2], "0.0000000000f == quat[2]");
    UtAssert_True(expectedR[3] == quatR[3], "0.0579710156f == quat[3]");
    UtAssert_True(isnan(nanResult), "NAN == quat[4]");
}


void Test_Quaternion_Conjugate(void)
{
    math::Quaternion quatA(1.5f, 2.5f, 3.5f, 4.5f);
    math::Vector3F vectorOut(0.0f, 0.0f, 0.0f);
    math::Vector3F vectorIn(2.5f, 3.5f, 4.5f);
    math::Vector3F expected(102.5f, 143.5f, 184.5f);
    
    vectorOut = quatA.Conjugate(vectorIn);
    UtAssert_True(expected[0] == vectorOut[0], "102.5f == vectorOut[0]");
    UtAssert_True(expected[1] == vectorOut[1], "143.5f == vectorOut[1]");
    UtAssert_True(expected[2] == vectorOut[2], "184.5f == vectorOut[2]");
}


void Test_Quaternion_ConjugateInversed(void)
{
    math::Quaternion quatA(1.5f, 2.5f, 3.5f, 4.5f);
    math::Vector3F vectorOut(0.0f, 0.0f, 0.0f);
    math::Vector3F vectorIn(2.5f, 3.5f, 4.5f);
    math::Vector3F expected(102.5f, 143.5f, 184.5f);
    
    vectorOut = quatA.ConjugateInversed(vectorIn);
    UtAssert_True(expected[0] == vectorOut[0], "102.5f == vectorOut[0]");
    UtAssert_True(expected[1] == vectorOut[1], "143.5f == vectorOut[1]");
    UtAssert_True(expected[2] == vectorOut[2], "184.5f == vectorOut[2]");
}


void Test_Quaternion_FromDCM(void)
{
    math::Vector3F row1(1.5f, 2.5f, 3.5f);
    math::Vector3F row2(4.5f, 5.5f, 6.5f);
    math::Vector3F row3(7.5f, 8.5f, 9.5f);
    math::Matrix3F3 input(row1, row2, row3);
    math::Quaternion output(0.0f, 0.0f, 0.0f, 0.0f);
    math::Quaternion expected(2.0916500092f, 0.2390457243f, -0.4780914485f, 0.2390457243f);
    
    output.FromDCM(input);

    UtAssert_True(fabs(expected[0] - output[0]) <= 0.00001f, "2.0916500092f == output[0]");
    UtAssert_True(fabs(expected[1] - output[1]) <= 0.00001f, "0.2390457243f == output[1]");
    UtAssert_True(fabs(expected[2] - output[2]) <= 0.00001f, "-0.4780914485f == output[2]");
    UtAssert_True(fabs(expected[3] - output[3]) <= 0.00001f, "0.2390457243f == output[3]");
}


void Test_Quaternion_FromYaw(void)
{
    float input = 35.5;
    math::Quaternion output(0.0f, 0.0f, 0.0f, 0.0f);
    math::Quaternion expected(0.4539918303, 0.0f, 0.0f, -0.8910058141f);
    
    output.FromYaw(input);

    UtAssert_True(fabs(expected[0] - output[0]) <= 0.00001f, "0.4539918303f == output[0]");
    UtAssert_True(fabs(expected[1] - output[1]) <= 0.00001f, "0.0f == output[1]");
    UtAssert_True(fabs(expected[2] - output[2]) <= 0.00001f, "0.0f == output[2]");
    UtAssert_True(fabs(expected[3] - output[3]) <= 0.00001f, "-0.8910058141f == output[3]");
}


void Test_Quaternion_ToEuler(void)
{
    math::Vector3F output(0.0f, 0.0f, 0.0f);
    math::Vector3F expected(2.3162157536f, 0.0f, 2.6905229092f);
    math::Quaternion existing(1.5f, 2.5f, 3.5f, 4.5f);
    
    output = existing.ToEuler();

    UtAssert_True(fabs(expected[0] - output[0]) <= 0.00001f, "2.3162157536f == output[0]");
    UtAssert_True(isnan(output[1]), "nan == output[1]");
    UtAssert_True(fabs(expected[2] - output[2]) <= 0.00001f, "2.6905229092f == output[2]");
}


void Test_Quaternion_Derivative(void)
{
    math::Quaternion existing(1.5f, 2.5f, 3.5f, 4.5f);
    math::Quaternion output(0.0f, 0.0f, 0.0f, 0.0f);
    math::Vector3F input(5.5f, 6.5f, 7.5f);
    math::Quaternion expected(-35.125f, 2.625f, 7.875f, 4.125f);
    
    output = existing.Derivative(input);

    UtAssert_True(fabs(expected[0] - output[0]) <= 0.00001f, "-35.125f == output[0]");
    UtAssert_True(fabs(expected[1] - output[1]) <= 0.00001f, "2.625ff == output[1]");
    UtAssert_True(fabs(expected[2] - output[2]) <= 0.00001f, "7.875f == output[2]");
    UtAssert_True(fabs(expected[3] - output[3]) <= 0.00001f, "4.125f == output[3]");
}


void Test_Quaternion_Imaginary(void)
{
//	/* quatActual = quatA - quatB */
//	math::Quaternion quatA(1.5f, 2.5f, 3.5f, 4.5f);
//	math::Quaternion quatActual = -quatA;
//
//	/* Verify results */
//	UtAssert_True(quatActual[0] == -9.0, "quatActual[0] == quatA[0] - quatB[0]");
//	UtAssert_True(quatActual[1] == -18.0, "quatActual[1] == quatA[1] - quatB[1]");
//	UtAssert_True(quatActual[2] == -27.0, "quatActual[2] == quatA[2] - quatB[2]");
//	UtAssert_True(quatActual[3] == -27.0, "quatActual[3] == quatA[3] - quatB[3]");
}


void Test_Quaternion_RotationMatrix(void)
{
	/* This is actual flight data collected from PX4 code which also
	 * implemented the algorithm defined in
	 * http://www.cs.ucr.edu/~vbz/resources/quatut.pdf
	 */
	math::Quaternion quatA(0.701320f, 0.002123f, -0.016784f, 0.712646f);
	math::Quaternion quatB(0.701320f, 0.002050f, -0.016780f, 0.712646f);
	math::Quaternion quatC(0.701320f, 0.002035f, -0.016731f, 0.712647f);

	math::Matrix3F3 rotA = quatA.RotationMatrix();
	math::Matrix3F3 rotB = quatB.RotationMatrix();
	math::Matrix3F3 rotC = quatC.RotationMatrix();

	/* Verify results */
	UtAssert_True(fabs(rotA[0][0] - -0.016292f) < 0.00001f, "[0][0]:  Rotation matrix A from Quaternion A.");
	UtAssert_True(fabs(rotA[0][1] - -0.999657f) < 0.00001f, "[0][1]:  Rotation matrix A from Quaternion A.");
	UtAssert_True(fabs(rotA[0][2] - -0.020516f) < 0.00001f, "[0][2]:  Rotation matrix A from Quaternion A.");
	UtAssert_True(fabs(rotA[1][0] -  0.999515f) < 0.00001f, "[1][0]:  Rotation matrix A from Quaternion A.");
	UtAssert_True(fabs(rotA[1][1] - -0.015737f) < 0.00001f, "[1][1]:  Rotation matrix A from Quaternion A.");
	UtAssert_True(fabs(rotA[1][2] - -0.026900f) < 0.00001f, "[1][2]:  Rotation matrix A from Quaternion A.");
	UtAssert_True(fabs(rotA[2][0] -  0.026568f) < 0.00001f, "[2][0]:  Rotation matrix A from Quaternion A.");
	UtAssert_True(fabs(rotA[2][1] - -0.020944f) < 0.00001f, "[2][1]:  Rotation matrix A from Quaternion A.");
	UtAssert_True(fabs(rotA[2][2] -  0.999428f) < 0.00001f, "[2][2]:  Rotation matrix A from Quaternion A.");

	UtAssert_True(fabs(rotB[0][0] - -0.016292f) < 0.00001f, "[0][0]:  Rotation matrix B from Quaternion B.");
	UtAssert_True(fabs(rotB[0][1] - -0.999655f) < 0.00001f, "[0][1]:  Rotation matrix B from Quaternion B.");
	UtAssert_True(fabs(rotB[0][2] - -0.020614f) < 0.00001f, "[0][2]:  Rotation matrix B from Quaternion B.");
	UtAssert_True(fabs(rotB[1][0] -  0.999517f) < 0.00001f, "[1][0]:  Rotation matrix B from Quaternion B.");
	UtAssert_True(fabs(rotB[1][1] - -0.015737f) < 0.00001f, "[1][1]:  Rotation matrix B from Quaternion B.");
	UtAssert_True(fabs(rotB[1][2] - -0.026792f) < 0.00001f, "[1][2]:  Rotation matrix B from Quaternion B.");
	UtAssert_True(fabs(rotB[2][0] -  0.026458f) < 0.00001f, "[2][0]:  Rotation matrix B from Quaternion B.");
	UtAssert_True(fabs(rotB[2][1] - -0.021041f) < 0.00001f, "[2][1]:  Rotation matrix B from Quaternion B.");
	UtAssert_True(fabs(rotB[2][2] -  0.999428f) < 0.00001f, "[2][2]:  Rotation matrix B from Quaternion B.");

	UtAssert_True(fabs(rotC[0][0] - -0.016292f) < 0.00001f, "[0][0]:  Rotation matrix C from Quaternion C.");
	UtAssert_True(fabs(rotC[0][1] - -0.999655f) < 0.00001f, "[0][1]:  Rotation matrix C from Quaternion C.");
	UtAssert_True(fabs(rotC[0][2] - -0.020567f) < 0.00001f, "[0][2]:  Rotation matrix C from Quaternion C.");
	UtAssert_True(fabs(rotC[1][0] -  0.999519f) < 0.00001f, "[1][0]:  Rotation matrix C from Quaternion C.");
	UtAssert_True(fabs(rotC[1][1] - -0.015740f) < 0.00001f, "[1][1]:  Rotation matrix C from Quaternion C.");
	UtAssert_True(fabs(rotC[1][2] - -0.026701f) < 0.00001f, "[1][2]:  Rotation matrix C from Quaternion C.");
	UtAssert_True(fabs(rotC[2][0] -  0.026368f) < 0.00001f, "[2][0]:  Rotation matrix C from Quaternion C.");
	UtAssert_True(fabs(rotC[2][1] - -0.020992f) < 0.00001f, "[2][1]:  Rotation matrix C from Quaternion C.");
	UtAssert_True(fabs(rotC[2][2] -  0.999431f) < 0.00001f, "[2][2]:  Rotation matrix C from Quaternion C.");
}


void Test_Quaternion_EqualityOperator(void)
{
    math::Quaternion quatA(0.0f, 0.0f, 0.0f, 0.0f);
    math::Quaternion quatB(0.0f, 0.0f, 0.0f, 0.0f);
    math::Quaternion quatC(1.0f, 2.0f, 3.0f, 4.0f);
    
    quatA = quatB = quatC;
    
	/* Verify results */
	UtAssert_True(quatA[0] == quatB[0], "quatA[0] == quatB[0]");
	UtAssert_True(quatA[1] == quatB[1], "quatA[1] == quatB[1]");
	UtAssert_True(quatA[2] == quatB[2], "quatA[2] == quatB[2]");
	UtAssert_True(quatA[3] == quatB[3], "quatA[3] == quatB[3]");
	UtAssert_True(quatA[0] == quatC[0], "quatA[0] == quatC[0]");
	UtAssert_True(quatA[1] == quatC[1], "quatA[1] == quatC[1]");
	UtAssert_True(quatA[2] == quatC[2], "quatA[2] == quatC[2]");
	UtAssert_True(quatA[3] == quatC[3], "quatA[3] == quatC[3]");
}


void Test_Quaternion_ScalerMultiplyOperator(void)
{
	/* actual = vectorA * scalar */
	math::Quaternion vectorA(1.5f, 2.5f, 3.5f, 4.5f);
	float scalar = 10.5f;
	math::Quaternion vectorActual(0.0f, 0.0f, 0.0f, 0.0f);
	math::Quaternion vectorExpected(15.75f, 26.25f, 36.75, 47.25f);
    
    vectorActual = vectorA * scalar;

	/* Verify results */
	UtAssert_True(vectorExpected[0] == vectorActual[0], "[0] vectorExpected == vectorA * scalar");
	UtAssert_True(vectorExpected[1] == vectorActual[1], "[1] vectorExpected == vectorA * scalar");
	UtAssert_True(vectorExpected[2] == vectorActual[2], "[2] vectorExpected == vectorA * scalar");
	UtAssert_True(vectorExpected[3] == vectorActual[3], "[3] vectorExpected == vectorA * scalar");
}
