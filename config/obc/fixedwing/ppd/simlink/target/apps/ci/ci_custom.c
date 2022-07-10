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

#include "ci_custom.h"
#include "ci_platform_cfg.h"
#include <fcntl.h>
#include <errno.h>
#include <arpa/inet.h>
#include "ci_events.h"
#include <strings.h>
#include <unistd.h>

#define CI_CUSTOM_RETURN_CODE_NULL_POINTER      (-1)


typedef struct
{
    int  Socket;
    uint16 Port;
} CI_AppCustomData_t;

CI_AppCustomData_t CI_AppCustomData = {0, 5010};


osalbool CI_AddCustomEventFilters(uint32 *count)
{	
    return TRUE;
}


int32 CI_InitCustom(void)
{
    int32 Status = CFE_SUCCESS;
    int reuseaddr = 1;
    struct sockaddr_in address;

    if((CI_AppCustomData.Socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
    {
    	CFE_EVS_SendEvent(CI_INIT_ERR_EID, CFE_EVS_ERROR,
    		   "Socket errno: %i", errno);
    		Status = -1;
    		goto end_of_function;
    }

    setsockopt(CI_AppCustomData.Socket, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(reuseaddr));
    fcntl(CI_AppCustomData.Socket, F_SETFL, fcntl(CI_AppCustomData.Socket, F_GETFL, 0) | O_NONBLOCK);

    bzero((char *) &address, sizeof(address));
    address.sin_family      = AF_INET;
    address.sin_addr.s_addr = htonl (INADDR_ANY);
    address.sin_port        = htons(CI_AppCustomData.Port);

    if ( (bind(CI_AppCustomData.Socket, (struct sockaddr *) &address, sizeof(address)) < 0) )
    {
        CFE_EVS_SendEvent(CI_INIT_ERR_EID, CFE_EVS_ERROR,"Bind socket failed = %d", errno);
        Status = -1;
        goto end_of_function;
    }

    CFE_EVS_SendEvent(CI_ENA_INF_EID, CFE_EVS_INFORMATION,
                      "UDP command input enabled on port %u.",
                      CI_AppCustomData.Port);

end_of_function:
    return Status;

}


void CI_ReadMessage(uint8* Buffer, uint32* outSize)
{
	int size = 0;

	size = recv(CI_AppCustomData.Socket,
					   (char *)Buffer,
					   (size_t)&size, 0);

	if(size < 0)
	{
		*outSize = 0;
	}
	else
	{
		*outSize = size;
	}
}


void CI_CleanupCustom(void)
{
    close(CI_AppCustomData.Socket);
}

