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

#include "Matrix3F3_test.hpp"
#include "Matrix3F3_test_utils.hpp"

int main(void)
{   
	UtTest_Add(Test_Matrix3F3_Constructor, Matrix3F3_Test_Setup, Matrix3F3_Test_TearDown,
	         "Test_Matrix3F3_Constructor");
	UtTest_Add(Test_Matrix3F3_IndexOutOfBounds, Matrix3F3_Test_Setup, Matrix3F3_Test_TearDown,
	         "Test_Matrix3F3_IndexOutOfBounds");
	UtTest_Add(Test_Matrix3F3_ArrayOperator, Matrix3F3_Test_Setup, Matrix3F3_Test_TearDown,
			 "Test_Matrix3F3_ArrayOperator");
	UtTest_Add(Test_Matrix3F3_Transpose, Matrix3F3_Test_Setup, Matrix3F3_Test_TearDown,
			 "Test_Matrix3F3_Transpose");
	UtTest_Add(Test_Matrix3F3_Identity, Matrix3F3_Test_Setup, Matrix3F3_Test_TearDown,
			 "Test_Matrix3F3_Identity");
	UtTest_Add(Test_Matrix3F3_VectorMultiply, Matrix3F3_Test_Setup, Matrix3F3_Test_TearDown,
			 "Test_Matrix3F3_VectorMultiply");
	UtTest_Add(Test_Matrix3F3_MatrixMultiply, Matrix3F3_Test_Setup, Matrix3F3_Test_TearDown,
		     "Test_Matrix3F3_MatrixMultiply");
	UtTest_Add(Test_Matrix3F3_ScalarMultiply, Matrix3F3_Test_Setup, Matrix3F3_Test_TearDown,
		     "Test_Matrix3F3_ScalarMultiply");
	UtTest_Add(Test_Matrix3F3_MatrixAddition, Matrix3F3_Test_Setup, Matrix3F3_Test_TearDown,
		     "Test_Matrix3F3_MatrixAddition");
	UtTest_Add(Test_Matrix3F3_FromEuler, Matrix3F3_Test_Setup, Matrix3F3_Test_TearDown,
		     "Test_Matrix3F3_FromEuler");
	UtTest_Add(Test_Matrix3F3_RotationMatrix, Matrix3F3_Test_Setup, Matrix3F3_Test_TearDown,
		     "Test_Matrix3F3_RotationMatrix");
	UtTest_Add(Test_Matrix3F3_Zero, Matrix3F3_Test_Setup, Matrix3F3_Test_TearDown,
		     "Test_Matrix3F3_Zero");
	UtTest_Add(Test_Matrix3F3_Inverse, Matrix3F3_Test_Setup, Matrix3F3_Test_TearDown,
	         "Test_Matrix3F3_Inverse");
	UtTest_Add(Test_Matrix3F3_Determinant, Matrix3F3_Test_Setup, Matrix3F3_Test_TearDown,
	         "Test_Matrix3F3_Determinant");
	UtTest_Add(Test_Matrix3F3_Inverse2, Matrix3F3_Test_Setup, Matrix3F3_Test_TearDown,
	         "Test_Matrix3F3_Inverse2");
	UtTest_Add(Test_Matrix3F3_Mult_3F_Random, Matrix3F3_Test_Setup, Matrix3F3_Test_TearDown,
	         "Test_Matrix3F3_Mult_3F_Random");
	UtTest_Add(Test_Matrix3F3_Mult_3F3_Random, Matrix3F3_Test_Setup, Matrix3F3_Test_TearDown,
	         "Test_Matrix3F3_Mult_3F3_Random");

    return(UtTest_Run());

}

