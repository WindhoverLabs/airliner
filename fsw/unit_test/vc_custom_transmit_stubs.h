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

#ifndef VC_CUSTOM_TRANSMIT_STUBS_H
#define VC_CUSTOM_TRANSMIT_STUBS_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    boolean VC_Transmit_Init_Return;
    boolean VC_Transmit_Uninit_Return;
    boolean VC_Address_Verification_Return;
    boolean VC_Update_Destination_Return;
    int32 VC_SendData_Return;
    uint8   VC_Transmit_Init_Return_Values;
    uint8   VC_Transmit_Uninit_Return_Values;
    uint8   VC_Address_Verification_Return_Values;
    uint8   VC_Update_Destination_Return_Values;
    uint8   VC_SendData_Return_Values;
    uint8   VC_Transmit_Init_CallCount;
    int32   VC_Transmit_InitData_Return;
} VC_Transmit_Returns_t;


extern VC_Transmit_Returns_t VC_Transmit_Test_Returns;


#ifdef __cplusplus
}
#endif

#endif /* VC_CUSTOM_TRANSMIT_STUBS_H */
