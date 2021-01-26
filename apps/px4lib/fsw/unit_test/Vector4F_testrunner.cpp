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

#include "Vector4F_test.hpp"
#include "Vector4F_test_utils.hpp"

int main(void)
{   
	UtTest_Add(Test_Vector4F_Constructor, Vector4F_Test_Setup, Vector4F_Test_TearDown,
	         "Test_Vector4F_Constructor");
	UtTest_Add(Test_Vector4F_IndexOutOfBounds, Vector4F_Test_Setup, Vector4F_Test_TearDown,
	         "Test_Vector4F_IndexOutOfBounds");
	UtTest_Add(Test_Vector4F_Length, Vector4F_Test_Setup, Vector4F_Test_TearDown,
	         "Test_Vector4F_Length");
	UtTest_Add(Test_Vector4F_LengthSquared, Vector4F_Test_Setup, Vector4F_Test_TearDown,
	         "Test_Vector4F_LengthSqaured");
	UtTest_Add(Test_Vector4F_Zero, Vector4F_Test_Setup, Vector4F_Test_TearDown,
	         "Test_Vector4F_Zero");
	UtTest_Add(Test_Vector4F_AdditionOperator, Vector4F_Test_Setup, Vector4F_Test_TearDown,
	         "Test_Vector4F_AdditionOperator");
	UtTest_Add(Test_Vector3F_SubtractionOperator, Vector4F_Test_Setup, Vector4F_Test_TearDown,
	         "Test_Vector3F_SubtractionOperator");
	UtTest_Add(Test_Vector4F_ScalarMultiplyOperator, Vector4F_Test_Setup, Vector4F_Test_TearDown,
	         "Test_Vector4F_ScalarMultiplyOperator");
	UtTest_Add(Test_Vector4F_ScalarDivOperator, Vector4F_Test_Setup, Vector4F_Test_TearDown,
	         "Test_Vector4F_ScalarDivOperator");
	UtTest_Add(Test_Vector4F_DotProductOperator, Vector4F_Test_Setup, Vector4F_Test_TearDown,
	         "Test_Vector4F_DotProductOperator");
	UtTest_Add(Test_Vector4F_EqualityOperator, Vector4F_Test_Setup, Vector4F_Test_TearDown,
	         "Test_Vector4F_EqualityOperator");
	UtTest_Add(Test_Vector4F_ArrayOperator, Vector4F_Test_Setup, Vector4F_Test_TearDown,
			 "Test_Vector4F_ArrayOperator");
	UtTest_Add(Test_Vector4F_NegateOperator, Vector4F_Test_Setup, Vector4F_Test_TearDown,
	         "Test_Vector4F_NegateOperator");
	UtTest_Add(Test_Vector4F_Add_4F_Random, Vector4F_Test_Setup, Vector4F_Test_TearDown,
	         "Test_Vector4F_Add_4F_Random");
	UtTest_Add(Test_Vector4F_Sub_4F_Random, Vector4F_Test_Setup, Vector4F_Test_TearDown,
	         "Test_Vector4F_Sub_4F_Random");

    return(UtTest_Run());

}

