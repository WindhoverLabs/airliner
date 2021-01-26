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

void Test_Quaternion_Constructor(void);
void Test_Quaternion_Matrix3F3Constructor(void);
void Test_Quaternion_IndexOutOfBounds(void);
void Test_Quaternion_ArrayOperator(void);
void Test_Quaternion_MultiplyOperator(void);
void Test_Quaternion_DivideOperator(void);

void Test_Quaternion_Conjugate(void);
void Test_Quaternion_ConjugateInversed(void);
void Test_Quaternion_FromDCM(void);
void Test_Quaternion_FromYaw(void);
void Test_Quaternion_Derivative(void);
void Test_Quaternion_ToEuler(void);
void Test_Quaternion_EqualityOperator(void);
void Test_Quaternion_Imaginary(void);
void Test_Quaternion_RotationMatrix(void);
void Test_Quaternion_ScalerMultiplyOperator(void);


#ifdef __cplusplus
}
#endif

#endif /* VECTOR4F_TEST_H */
