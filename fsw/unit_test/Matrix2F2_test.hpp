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

#ifndef MATRIX2F2_TEST_H
#define MATRIX2F2_TEST_H

#ifdef __cplusplus
extern "C" {
#endif

void Test_Matrix2F2_Constructor(void);
void Test_Matrix2F2_IndexOutOfBounds(void);
void Test_Matrix2F2_ArrayOperator(void);
void Test_Matrix2F2_Transpose(void);
void Test_Matrix2F2_Identity(void);
void Test_Matrix2F2_VectorMultiply(void);
void Test_Matrix2F2_MatrixMultiply(void);
void Test_Matrix2F2_ScalarMultiply(void);
void Test_Matrix2F2_MatrixAddition(void);
void Test_Matrix2F2_FromEuler(void);
void Test_Matrix2F2_RotationMatrix(void);
void Test_Matrix2F2_Zero(void);
void Test_Matrix2F2_Determinant(void);
void Test_Matrix2F2_Inverse(void);
void Test_Matrix2F2_Inverse2(void);
void Test_Matrix2F2_Mult_2F_Random(void);
void Test_Matrix2F2_Mult_2F2_Random(void);


#ifdef __cplusplus
}
#endif

#endif /* MATRIX3F3_TEST_H */
