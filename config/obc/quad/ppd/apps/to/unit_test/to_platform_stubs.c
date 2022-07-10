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

#include <arpa/inet.h>
#include <errno.h>

#include "to_platform_stubs.h"


int enable_wrappers = 0;

int __real_socket(int domain, int type, int protocol);
int __real_bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
int __real_sendto(int sockfd, const void *buf, size_t len, int flags,
               const struct sockaddr *dest_addr, socklen_t addrlen);

TO_Platform_Stubs_Returns_t TO_Platform_Stubs_Returns = {0};


int __wrap_socket(int domain, int type, int protocol)
{
    int returnCode = 0;
    if(enable_wrappers == 0)
    {
        returnCode = __real_socket(domain, type, protocol);
    }
    else
    {
        if (TO_Platform_Stubs_Returns.TO_Wrap_Socket_Errno)
        {
            errno = TO_Platform_Stubs_Returns.TO_Wrap_Socket_Errno_Value;
        }
        returnCode = TO_Platform_Stubs_Returns.TO_Wrap_Socket_Return;
    }
    return returnCode;
}


int __wrap_bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
    int returnCode = 0;
    if(enable_wrappers == 0)
    {
        returnCode = __real_bind(sockfd, addr, addrlen);
    }
    else
    {
        if (TO_Platform_Stubs_Returns.TO_Wrap_Bind_Errno)
        {
            errno = TO_Platform_Stubs_Returns.TO_Wrap_Bind_Errno_Value;
        }
        returnCode = TO_Platform_Stubs_Returns.TO_Wrap_Bind_Return;
    }
    return returnCode;
}


int __wrap_sendto(int sockfd, const void *buf, size_t len, int flags,
               const struct sockaddr *dest_addr, socklen_t addrlen)
{
    int returnCode = 0;
    if(enable_wrappers == 0)
    {
        returnCode = __real_sendto(sockfd, buf, len, flags, dest_addr, addrlen);
    }
    else
    {
        if (TO_Platform_Stubs_Returns.TO_Wrap_SendTo_Errno)
        {
            errno = TO_Platform_Stubs_Returns.TO_Wrap_SendTo_Errno_Value;
        }
        returnCode = TO_Platform_Stubs_Returns.TO_Wrap_SendTo_Return;
    }
    return returnCode;
}
