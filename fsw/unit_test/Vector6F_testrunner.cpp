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

#include "Vector6F_test.hpp"
#include "Vector6F_test_utils.hpp"

int main(void)
{   
	UtTest_Add(Test_Vector6F_Constructor, Vector6F_Test_Setup, Vector6F_Test_TearDown,
	         "Test_Vector6F_Constructor");
	UtTest_Add(Test_Vector6F_IndexOutOfBounds, Vector6F_Test_Setup, Vector6F_Test_TearDown,
	         "Test_Vector6F_IndexOutOfBounds");
	UtTest_Add(Test_Vector6F_Length, Vector6F_Test_Setup, Vector6F_Test_TearDown,
			 "Test_Vector6F_Length");
	UtTest_Add(Test_Vector6F_Zero, Vector6F_Test_Setup, Vector6F_Test_TearDown,
			 "Test_Vector6F_Zero");
	UtTest_Add(Test_Vector6F_ArrayOperator, Vector6F_Test_Setup, Vector6F_Test_TearDown,
			 "Test_Vector6F_ArrayOperator");
	UtTest_Add(Test_Vector6F_AdditionOperator, Vector6F_Test_Setup, Vector6F_Test_TearDown,
	         "Test_Vector6F_AdditionOperator");
	UtTest_Add(Test_Vector6F_SubtractionOperator, Vector6F_Test_Setup, Vector6F_Test_TearDown,
	         "Test_Vector6F_SubtractionOperator");
	UtTest_Add(Test_Vector6F_ScalarMultiplyOperator, Vector6F_Test_Setup, Vector6F_Test_TearDown,
	         "Test_Vector6F_ScalarMultiplyOperator");
	UtTest_Add(Test_Vector6F_ScalarDivOperator, Vector6F_Test_Setup, Vector6F_Test_TearDown,
	         "Test_Vector6F_ScalarDivOperator");
	UtTest_Add(Test_Vector6F_DotProductOperator, Vector6F_Test_Setup, Vector6F_Test_TearDown,
	         "Test_Vector6F_DotProductOperator");
	UtTest_Add(Test_Vector6F_EMult, Vector6F_Test_Setup, Vector6F_Test_TearDown,
	         "Test_Vector6F_EMult");
	UtTest_Add(Test_Vector6F_EDivide, Vector6F_Test_Setup, Vector6F_Test_TearDown,
	         "Test_Vector6F_EDivide");
	UtTest_Add(Test_Vector6F_Constrain, Vector6F_Test_Setup, Vector6F_Test_TearDown,
	         "Test_Vector6F_Constrain");
	UtTest_Add(Test_Vector6F_Pow, Vector6F_Test_Setup, Vector6F_Test_TearDown,
	         "Test_Vector6F_Pow");
	UtTest_Add(Test_Vector6F_Normalized, Vector6F_Test_Setup, Vector6F_Test_TearDown,
	         "Test_Vector6F_Normalized");
	UtTest_Add(Test_Vector6F_Normalize, Vector6F_Test_Setup, Vector6F_Test_TearDown,
	         "Test_Vector6F_Normalize");
	UtTest_Add(Test_Vector6F_Negate, Vector6F_Test_Setup, Vector6F_Test_TearDown,
	         "Test_Vector6F_Negate");
	UtTest_Add(Test_Vector6F_Add_6F_Random, Vector6F_Test_Setup, Vector6F_Test_TearDown,
	         "Test_Vector6F_Add_6F_Random");
	UtTest_Add(Test_Vector6F_Sub_6F_Random, Vector6F_Test_Setup, Vector6F_Test_TearDown,
	         "Test_Vector6F_Sub_6F_Random");

    return(UtTest_Run());

}

