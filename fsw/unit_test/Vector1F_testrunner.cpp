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

#include "Vector1F_test.hpp"
#include "Vector1F_test_utils.hpp"

int main(void)
{   
	UtTest_Add(Test_Vector1F_Constructor, Vector1F_Test_Setup, Vector1F_Test_TearDown,
	         "Test_Vector1F_Constructor");
	UtTest_Add(Test_Vector1F_IndexOutOfBounds, Vector1F_Test_Setup, Vector1F_Test_TearDown,
	         "Test_Vector1F_IndexOutOfBounds");
	UtTest_Add(Test_Vector1F_Length, Vector1F_Test_Setup, Vector1F_Test_TearDown,
			 "Test_Vector1F_Length");
	UtTest_Add(Test_Vector1F_Zero, Vector1F_Test_Setup, Vector1F_Test_TearDown,
			 "Test_Vector1F_Zero");
	UtTest_Add(Test_Vector1F_ArrayOperator, Vector1F_Test_Setup, Vector1F_Test_TearDown,
			 "Test_Vector1F_ArrayOperator");
	UtTest_Add(Test_Vector1F_AdditionOperator, Vector1F_Test_Setup, Vector1F_Test_TearDown,
	         "Test_Vector1F_AdditionOperator");
	UtTest_Add(Test_Vector1F_SubtractionOperator, Vector1F_Test_Setup, Vector1F_Test_TearDown,
	         "Test_Vector1F_SubtractionOperator");
	UtTest_Add(Test_Vector1F_ScalarMultiplyOperator, Vector1F_Test_Setup, Vector1F_Test_TearDown,
	         "Test_Vector1F_ScalarMultiplyOperator");
	UtTest_Add(Test_Vector1F_ScalarDivOperator, Vector1F_Test_Setup, Vector1F_Test_TearDown,
	         "Test_Vector1F_ScalarDivOperator");
	UtTest_Add(Test_Vector1F_DotProductOperator, Vector1F_Test_Setup, Vector1F_Test_TearDown,
	         "Test_Vector1F_DotProductOperator");
	UtTest_Add(Test_Vector1F_Negate, Vector1F_Test_Setup, Vector1F_Test_TearDown,
	         "Test_Vector1F_Negate");
	UtTest_Add(Test_Vector1F_Pow, Vector1F_Test_Setup, Vector1F_Test_TearDown,
	         "Test_Vector1F_Pow");
	UtTest_Add(Test_Vector1F_EMult, Vector1F_Test_Setup, Vector1F_Test_TearDown,
	         "Test_Vector1F_EMult");
	UtTest_Add(Test_Vector1F_EDivide, Vector1F_Test_Setup, Vector1F_Test_TearDown,
	         "Test_Vector1F_EDivide");
	UtTest_Add(Test_Vector1F_Constrain, Vector1F_Test_Setup, Vector1F_Test_TearDown,
	         "Test_Vector1F_Constrain");
	UtTest_Add(Test_Vector1F_Normalized, Vector1F_Test_Setup, Vector1F_Test_TearDown,
	         "Test_Vector1F_Normalized");
	UtTest_Add(Test_Vector1F_Normalize, Vector1F_Test_Setup, Vector1F_Test_TearDown,
	         "Test_Vector1F_Normalize");
	UtTest_Add(Test_Vector1F_Add_1F_Random, Vector1F_Test_Setup, Vector1F_Test_TearDown,
	         "Test_Vector1F_Add_1F_Random");
	UtTest_Add(Test_Vector1F_Sub_1F_Random, Vector1F_Test_Setup, Vector1F_Test_TearDown,
	         "Test_Vector1F_Sub_1F_Random");


    return(UtTest_Run());

}

