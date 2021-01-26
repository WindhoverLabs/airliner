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

#include "vc_app.h"
#include "vc_dev_io.h"
#include "vc_custom_device_stubs.h"

VC_Device_Returns_t VC_Device_Test_Returns = { TRUE,
                                               TRUE,
                                               TRUE, 
                                               TRUE,
                                               0b11111111,
                                               0b11111111,
                                               0b11111111,
                                               0b11111111,
                                               0 };

boolean VC_Devices_Start(void)
{
    boolean returnCode = FALSE;
    
    /* Bitshift 1 to the left the value of call count AND with 
     * return values */
    if(VC_Device_Test_Returns.VC_Devices_Start_Return_Values &
        (1<<VC_Device_Test_Returns.VC_Devices_Start_CallCount))
    {
        returnCode = VC_Device_Test_Returns.VC_Devices_Start_Return;
    }
    else
    {
        returnCode = !VC_Device_Test_Returns.VC_Devices_Start_Return;
    }
    VC_Device_Test_Returns.VC_Devices_Start_CallCount++;
    return returnCode;
}


boolean VC_Devices_Stop(void)
{
    return VC_Device_Test_Returns.VC_Devices_Stop_Return;
}


boolean VC_Devices_Init(void)
{
    return VC_Device_Test_Returns.VC_Devices_Init_Return;
}


boolean VC_Devices_Uninit(void)
{
    return VC_Device_Test_Returns.VC_Devices_Uninit_Return;
}


int32 VC_Devices_InitData(void)
{
    return VC_Device_Test_Returns.VC_Devices_InitData_Return;
}


void VC_Devices_Critical_Cleanup(void)
{
    return;
}
