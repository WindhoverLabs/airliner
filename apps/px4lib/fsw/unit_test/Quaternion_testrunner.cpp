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

#include "uttest.h"

#include "Quaternion_test.hpp"
#include "Quaternion_test_utils.hpp"

int main(void)
{   
	UtTest_Add(Test_Quaternion_Constructor, Quaternion_Test_Setup, Quaternion_Test_TearDown,
	         "Test_Quaternion_Constructor");
	UtTest_Add(Test_Quaternion_Matrix3F3Constructor, Quaternion_Test_Setup, Quaternion_Test_TearDown,
	         "Test_Quaternion_Matrix3F3Constructor");
	UtTest_Add(Test_Quaternion_RotationMatrix, Quaternion_Test_Setup, Quaternion_Test_TearDown,
	         "Test_Quaternion_RotationMatrix");
	UtTest_Add(Test_Quaternion_IndexOutOfBounds, Quaternion_Test_Setup, Quaternion_Test_TearDown,
	         "Test_Quaternion_IndexOutOfBounds");
	UtTest_Add(Test_Quaternion_ArrayOperator, Quaternion_Test_Setup, Quaternion_Test_TearDown,
			 "Test_Quaternion_ArrayOperator");
	UtTest_Add(Test_Quaternion_MultiplyOperator, Quaternion_Test_Setup, Quaternion_Test_TearDown,
			 "Test_Quaternion_MultiplyOperator");
	UtTest_Add(Test_Quaternion_DivideOperator, Quaternion_Test_Setup, Quaternion_Test_TearDown,
			 "Test_Quaternion_DivideOperator");
	UtTest_Add(Test_Quaternion_Conjugate, Quaternion_Test_Setup, Quaternion_Test_TearDown,
			 "Test_Quaternion_Conjugate");
	UtTest_Add(Test_Quaternion_ConjugateInversed, Quaternion_Test_Setup, Quaternion_Test_TearDown,
			 "Test_Quaternion_ConjugateInversed");
	UtTest_Add(Test_Quaternion_FromDCM, Quaternion_Test_Setup, Quaternion_Test_TearDown,
			 "Test_Quaternion_FromDCM");
	UtTest_Add(Test_Quaternion_FromYaw, Quaternion_Test_Setup, Quaternion_Test_TearDown,
			 "Test_Quaternion_FromYaw");
	UtTest_Add(Test_Quaternion_ToEuler, Quaternion_Test_Setup, Quaternion_Test_TearDown,
			 "Test_Quaternion_ToEuler");
	UtTest_Add(Test_Quaternion_Derivative, Quaternion_Test_Setup, Quaternion_Test_TearDown,
			 "Test_Quaternion_Derivative");
	UtTest_Add(Test_Quaternion_Imaginary, Quaternion_Test_Setup, Quaternion_Test_TearDown,
	         "Test_Quaternion_Imaginary");
	UtTest_Add(Test_Quaternion_EqualityOperator, Quaternion_Test_Setup, Quaternion_Test_TearDown,
	         "Test_Quaternion_EqualityOperator");
	UtTest_Add(Test_Quaternion_ScalerMultiplyOperator, Quaternion_Test_Setup, Quaternion_Test_TearDown,
	         "Test_Quaternion_ScalerMultiplyOperator");
    return(UtTest_Run());

}

