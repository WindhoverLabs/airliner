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

#ifndef VC_PLATFORM_STUBS_H
#define VC_PLATFORM_STUBS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <linux/videodev2.h>


typedef struct
{
    int     VC_Wrap_Ioctl_Return;
    int     VC_Wrap_Ioctl_Return1;
    int     VC_Wrap_Ioctl_Return2;
    int     VC_Wrap_Ioctl_Errno;
    int     VC_Wrap_Ioctl_Errno_Value;
    int     VC_Wrap_Ioctl_Struct;
    int     VC_Wrap_Open_Return;
    int     VC_Wrap_Open_Errno;
    int     VC_Wrap_Open_Errno_Value;
    int     VC_Wrap_Close_Return;
    int     VC_Wrap_Select_Return;
    int     VC_Wrap_Select_Errno;
    int     VC_Wrap_Select_Errno_Value;
    int     VC_Wrap_Socket_Return;
    int     VC_Wrap_Socket_Errno;
    int     VC_Wrap_Socket_Errno_Value;
    int     VC_Wrap_Bind_Return;
    int     VC_Wrap_Bind_Errno;
    int     VC_Wrap_Bind_Errno_Value;
    int     VC_Wrap_SendTo_Return;
    int     VC_Wrap_SendTo_Errno;
    int     VC_Wrap_SendTo_Errno_Value;
    int     VC_Wrap_Ioctl_CallCount;
    int     VC_Wrap_Recv_Return;
    int     VC_Wrap_Recv_Return1;
    int     VC_Wrap_Recv_Errno;
    int     VC_Wrap_Recv_Errno_Value;
    int     VC_Wrap_Recv_CallCount;
} VC_Platform_Stubs_Returns_t;


extern VC_Platform_Stubs_Returns_t VC_Platform_Stubs_Returns;


#ifdef __cplusplus
}
#endif

#endif /* VC_PLATFORM_STUBS_H */
