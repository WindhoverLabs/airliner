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

#ifndef VECTOR4F_TEST_H
#define VECTOR4F_TEST_H

#ifdef __cplusplus
extern "C" {
#endif

void Test_Vector4F_Constructor(void);
void Test_Vector4F_IndexOutOfBounds(void);
void Test_Vector4F_Length(void);
void Test_Vector4F_LengthSquared(void);
void Test_Vector4F_Zero(void);
void Test_Vector4F_AdditionOperator(void);
void Test_Vector3F_SubtractionOperator(void);
void Test_Vector4F_ScalarMultiplyOperator(void);
void Test_Vector4F_ScalarDivOperator(void);
void Test_Vector4F_DotProductOperator(void);
void Test_Vector4F_EqualityOperator(void);
void Test_Vector4F_ArrayOperator(void);
void Test_Vector4F_NegateOperator(void);
void Test_Vector4F_Add_4F_Random(void);
void Test_Vector4F_Sub_4F_Random(void);

#ifdef __cplusplus
}
#endif

#endif /* VECTOR4F_TEST_H */
