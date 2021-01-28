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

#include <sys/ioctl.h>
#include <errno.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <sys/time.h> 
#include <sys/types.h> 
#include <unistd.h> 

#include "vc_platform_stubs.h"
#include "vc_platform_cfg.h"

int enable_wrappers = 0;

int __real_open(char *filename, int  access, int  permission);
int __real_close(int fildes);
int __real_ioctl(int fh, int request, void *arg);
int __real_select(int nfds, fd_set *readfds, fd_set *writefds, 
                    fd_set *exceptfds, struct timeval *timeout);
int __real_socket(int domain, int type, int protocol);
int __real_bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
int __real_sendto(int sockfd, const void *buf, size_t len, int flags,
               const struct sockaddr *dest_addr, socklen_t addrlen);
ssize_t __real_recv(int s, void *buf, size_t len, int flags); 


VC_Platform_Stubs_Returns_t VC_Platform_Stubs_Returns = {0};


int __wrap_ioctl(int fh, int request, void *arg)
{
    int returnCode = 0;

    if(enable_wrappers == 0)
    {
        returnCode = __real_ioctl(fh, request, arg);
    }
    else
    {
        if (VC_Platform_Stubs_Returns.VC_Wrap_Ioctl_Errno)
        {
            errno = VC_Platform_Stubs_Returns.VC_Wrap_Ioctl_Errno_Value;
        }
        if (VC_Platform_Stubs_Returns.VC_Wrap_Ioctl_Struct)
        {
            switch(request)
            {
                case VIDIOC_QUERYCAP:
                    if(1 == VC_Platform_Stubs_Returns.VC_Wrap_Ioctl_Struct)
                    {
                        ((struct v4l2_capability *)arg)->capabilities = VC_V4L_BUFFER_TYPE;
                    }
                    if(2 == VC_Platform_Stubs_Returns.VC_Wrap_Ioctl_Struct)
                    {
                        ((struct v4l2_capability *)arg)->capabilities = (VC_V4L_BUFFER_TYPE +
                        V4L2_CAP_STREAMING);
                    }
                    break;
                case VIDIOC_S_FMT:
                    ((struct v4l2_format *)arg)->fmt.pix.pixelformat = VC_V4L_VIDEO_FORMAT;
                    ((struct v4l2_format *)arg)->fmt.pix.width = VC_FRAME_WIDTH;
                    ((struct v4l2_format *)arg)->fmt.pix.height = VC_FRAME_HEIGHT;
                    ((struct v4l2_format *)arg)->fmt.pix.sizeimage = VC_MAX_BUFFER_SIZE;
                    break;
                case VIDIOC_REQBUFS:
                    ((struct v4l2_requestbuffers *)arg)->count = VC_V4L_BUFFER_REQUEST;
                case VIDIOC_DQBUF:
                    if(1 == VC_Platform_Stubs_Returns.VC_Wrap_Ioctl_Struct)
                    {
                        ((struct v4l2_buffer *)arg)->bytesused = VC_MAX_PACKET_SIZE;
                    }
                    if(2 == VC_Platform_Stubs_Returns.VC_Wrap_Ioctl_Struct)
                    {
                        ((struct v4l2_buffer *)arg)->bytesused = (VC_MAX_PACKET_SIZE + 1);
                    }
                default:
                    break;
            }
        }

        if (1 == VC_Platform_Stubs_Returns.VC_Wrap_Ioctl_CallCount)
        {
            returnCode = VC_Platform_Stubs_Returns.VC_Wrap_Ioctl_Return1;
        }
        else if (2 == VC_Platform_Stubs_Returns.VC_Wrap_Ioctl_CallCount)
        {
            returnCode = VC_Platform_Stubs_Returns.VC_Wrap_Ioctl_Return2;
        }
        else
        {
            returnCode = VC_Platform_Stubs_Returns.VC_Wrap_Ioctl_Return;
        }

        VC_Platform_Stubs_Returns.VC_Wrap_Ioctl_CallCount++;
    }
    return returnCode;
}


int __wrap_open(char *filename, int  access, int  permission)
{
    int returnCode = 0;

    if(enable_wrappers == 0)
    {
        returnCode = __real_open(filename, access, permission);
    }
    else
    {
        if (VC_Platform_Stubs_Returns.VC_Wrap_Open_Errno)
        {
            errno = VC_Platform_Stubs_Returns.VC_Wrap_Open_Errno_Value;
        }
        returnCode = VC_Platform_Stubs_Returns.VC_Wrap_Open_Return;
    }
    return returnCode;
}


int __wrap_close(int fildes)
{
    int returnCode = 0;

    if(enable_wrappers == 0)
    {
        returnCode = __real_close(fildes);
    }
    else
    {
        returnCode = VC_Platform_Stubs_Returns.VC_Wrap_Close_Return;
    }
    return returnCode;
}


int __wrap_select(int nfds, fd_set *readfds, fd_set *writefds, 
                    fd_set *exceptfds, struct timeval *timeout)
{
    int returnCode = 0;
    if(enable_wrappers == 0)
    {
        returnCode = __real_select(nfds, readfds, writefds, exceptfds, timeout);
    }
    else
    {
        if (VC_Platform_Stubs_Returns.VC_Wrap_Select_Errno)
        {
            errno = VC_Platform_Stubs_Returns.VC_Wrap_Select_Errno_Value;
        }
        returnCode = VC_Platform_Stubs_Returns.VC_Wrap_Select_Return;
    }
    return returnCode;
}


int __wrap_socket(int domain, int type, int protocol)
{
    int returnCode = 0;
    if(enable_wrappers == 0)
    {
        returnCode = __real_socket(domain, type, protocol);
    }
    else
    {
        if (VC_Platform_Stubs_Returns.VC_Wrap_Socket_Errno)
        {
            errno = VC_Platform_Stubs_Returns.VC_Wrap_Socket_Errno_Value;
        }
        returnCode = VC_Platform_Stubs_Returns.VC_Wrap_Socket_Return;
    }
    return returnCode;
}


ssize_t __wrap_recv(int s, void *buf, size_t len, int flags)
{
    ssize_t returnCode = 0;
    if(enable_wrappers == 0)
    {
        returnCode = __real_recv(s, buf, len, flags);
    }
    else
    {
        if (VC_Platform_Stubs_Returns.VC_Wrap_Recv_Errno)
        {
            errno = VC_Platform_Stubs_Returns.VC_Wrap_Recv_Errno_Value;
        }
        if (1 == VC_Platform_Stubs_Returns.VC_Wrap_Recv_CallCount)
        {
            returnCode = VC_Platform_Stubs_Returns.VC_Wrap_Recv_Return1;
        }
        else 
        {
            returnCode = VC_Platform_Stubs_Returns.VC_Wrap_Recv_Return;
        }

        VC_Platform_Stubs_Returns.VC_Wrap_Recv_CallCount++;
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
        if (VC_Platform_Stubs_Returns.VC_Wrap_Bind_Errno)
        {
            errno = VC_Platform_Stubs_Returns.VC_Wrap_Bind_Errno_Value;
        }
        returnCode = VC_Platform_Stubs_Returns.VC_Wrap_Bind_Return;
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
        if (VC_Platform_Stubs_Returns.VC_Wrap_SendTo_Errno)
        {
            errno = VC_Platform_Stubs_Returns.VC_Wrap_SendTo_Errno_Value;
        }
        returnCode = VC_Platform_Stubs_Returns.VC_Wrap_SendTo_Return;
    }
    return returnCode;
}
