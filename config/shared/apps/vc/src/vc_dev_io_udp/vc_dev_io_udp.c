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
/************************************************************************
** Pragmas
*************************************************************************/

/************************************************************************
** Includes
*************************************************************************/
#include "vc_dev_io_udp.h"
#include "cfe.h"
#include "px4lib.h"
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#include <arpa/inet.h>

/************************************************************************
** Local Defines
*************************************************************************/

/************************************************************************
** Local Structure Declarations
*************************************************************************/

/************************************************************************
** External Global Variables
*************************************************************************/

/************************************************************************
** Global Variables
*************************************************************************/

/************************************************************************
** Local Variables
*************************************************************************/

/************************************************************************
** Local Function Definitions
*************************************************************************/

VC_AppCustomDevice_t VC_AppCustomDevice;


int32 VC_Devices_InitData(void)
{
    int32 iStatus = CFE_SUCCESS;
    
    /* Set all struct zero values */
    bzero(&VC_AppCustomDevice, sizeof(VC_AppCustomDevice));
    
    /* Set all non-zero values for channel zero */
    VC_AppCustomDevice.ContinueFlag          = TRUE;
    VC_AppCustomDevice.Priority              = VC_STREAMING_TASK_PRIORITY;
    VC_AppCustomDevice.StreamingTask         = VC_Stream_Task;

    VC_AppCustomDevice.Channel[0].Status     = VC_DEVICE_UNINITIALIZED;
    VC_AppCustomDevice.Channel[0].Mode       = VC_DEVICE_ENABLED;
    VC_AppCustomDevice.Channel[0].Socket     = 0;
    VC_AppCustomDevice.Channel[0].Port       = VC_GST_GAZEBO_PORT;

    VC_AppCustomDevice.Channel[1].Status     = VC_DEVICE_UNINITIALIZED;
    VC_AppCustomDevice.Channel[1].Mode       = VC_DEVICE_ENABLED;
    VC_AppCustomDevice.Channel[1].Socket     = 0;
    VC_AppCustomDevice.Channel[1].Port       = VC_OPTICAL_FLOW_PORT;

    /* Initialize output messages*/
    CFE_SB_InitMsg(&OpticalFlowFrameMsg, FLOW_FRAME_MID,
                sizeof(PX4_OpticalFlowFrameMsg_t), TRUE);
    return (iStatus);
}


boolean VC_Devices_Start(void)
{
    boolean returnBool = TRUE;
    int32 returnCode = 0;
    
    /* Set loop flag to continue forever */
    VC_AppCustomDevice.ContinueFlag = TRUE;
    
    /* Create the streaming task */
    returnCode = CFE_ES_CreateChildTask(
        &VC_AppCustomDevice.ChildTaskID,
        VC_STREAMING_TASK_NAME,
        VC_AppCustomDevice.StreamingTask,
        0,
        CFE_ES_DEFAULT_STACK_SIZE,
        VC_AppCustomDevice.Priority,
        VC_STREAMING_TASK_FLAGS);

    if(returnCode != CFE_SUCCESS)
    {
        VC_AppCustomDevice.ContinueFlag = FALSE;
        returnBool = FALSE;
    }
    return (returnBool);
}


boolean VC_Devices_Stop(void)
{
    boolean returnBool = TRUE;
    int32   returnCode = CFE_SUCCESS;
    /* Delete the child task */
    returnCode = CFE_ES_DeleteChildTask(VC_AppCustomDevice.ChildTaskID);
    
    /* Set streaming task loop flag to stop */
    VC_AppCustomDevice.ContinueFlag = FALSE;
    
    /* Set app state to initialized */
    VC_AppData.AppState = VC_INITIALIZED;
    
    if (returnCode != CFE_SUCCESS)
    {
        returnBool = FALSE;
    }
    return (returnBool);
}


boolean VC_Devices_Init(void)
{
    boolean returnBool = TRUE;
    if(-1 == VC_Init_CustomDevices())
    {
        returnBool = FALSE;
    }
    return (returnBool);
}


boolean VC_Devices_Uninit(void)
{
    boolean returnBool = TRUE;
    if(-1 == VC_CleanupDevices())
    {
        returnBool = FALSE;
    }
    return (returnBool);
}


void VC_Devices_Critical_Cleanup(void)
{
    uint8 i = 0;

    for(i=0; i < VC_MAX_DEVICES; i++)
    {
        if(VC_AppCustomDevice.Channel[i].Socket != 0)
        {
            close(VC_AppCustomDevice.Channel[i].Socket);
        }
    }
    return;
}


int32 VC_CleanupDevices(void)
{
    uint32 i = 0;
    int32 returnCode = 0;
    
    for(i=0; i < VC_MAX_DEVICES; i++)
    {
        if(VC_AppCustomDevice.Channel[i].Mode == VC_DEVICE_ENABLED)
        {
            if(-1 == close(VC_AppCustomDevice.Channel[i].Socket))
            {
                /* Close failed */
                returnCode = -1;
            }
            else
            {
                VC_AppCustomDevice.Channel[i].Mode = VC_DEVICE_DISABLED;
            }
        }
    }
    return (returnCode);
}


int32 VC_Init_CustomDevices(void)
{
    int32 returnCode = 0;
    int reuseaddr    = 1;
    struct sockaddr_in address;
    uint32 i         = 0;

    for (i=0; i < VC_MAX_DEVICES; i++)
    {
        /* if a device is enabled... */
        if(VC_AppCustomDevice.Channel[i].Mode == VC_DEVICE_ENABLED)
        {
            /* Create socket */
            VC_AppCustomDevice.Channel[i].Socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
            /* if socket creation failed */
            
            if (VC_AppCustomDevice.Channel[i].Socket < 0)
            {
                (void) CFE_EVS_SendEvent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR, "Socket errno: %i on channel %u", errno, (unsigned int)i);
                returnCode = -1;
                goto end_of_function;
            }
            /* socket creation success try to bind */
            setsockopt(VC_AppCustomDevice.Channel[i].Socket, SOL_SOCKET, 
                    SO_REUSEADDR, &reuseaddr, sizeof(reuseaddr));
            bzero((char *) &address, sizeof(address));
            address.sin_family      = AF_INET;
            address.sin_addr.s_addr = htonl (INADDR_ANY);
            address.sin_port        = htons(VC_AppCustomDevice.Channel[i].Port);
            /* if bind failed... */
            if((bind(VC_AppCustomDevice.Channel[i].Socket , (struct sockaddr *) &address, sizeof(address)) < 0))
            {
                (void) CFE_EVS_SendEvent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR,"Bind errno: %i on channel %u", errno, (unsigned int)i);
                returnCode = -1;
                goto end_of_function;
            }
            /* socket create and bind success */
            VC_AppCustomDevice.Channel[i].Status = VC_DEVICE_INITIALIZED;
            CFE_EVS_SendEvent(VC_DEV_INF_EID, CFE_EVS_INFORMATION,
                    "VC Device initialized channel %u", (unsigned int)i);
        }
    }
    
end_of_function:

    return (returnCode);
}


void VC_Stream_Task(void)
{
    int32 returnCode      = 0;
    static int32 timeouts = 0;
    uint32 i              = 0;
    uint32 j              = 0;
    uint32 maxFd          = 0;
    static uint32 retryAttempts = 0;
    fd_set fds;
    
    struct timeval timeValue;
    uint32 iStatus = -1;
    
    iStatus = CFE_ES_RegisterChildTask();
    
    if (iStatus == CFE_SUCCESS)
    {
        while (VC_AppCustomDevice.ContinueFlag == TRUE)
        {
            maxFd = 0;
            returnCode = 0;

            /* Initialize the set */
            FD_ZERO(&fds);
        
            /* Add enabled and streaming devices to the fd set */
            for (i=0; i < VC_MAX_DEVICES; i++)
            {
                if(VC_AppCustomDevice.Channel[i].Mode == VC_DEVICE_ENABLED 
                && VC_AppCustomDevice.Channel[i].Status == VC_DEVICE_INITIALIZED)
                {
                    FD_SET(VC_AppCustomDevice.Channel[i].Socket, &fds);
                    /* Get the greatest fd value for select() */
                    if (VC_AppCustomDevice.Channel[i].Socket > maxFd)
                    {
                        /* maxFd is needed for select */
                        maxFd = VC_AppCustomDevice.Channel[i].Socket; 
                    }
                }
            }
            /* If maxFd is > 0 a fd was added the set so call select */
            if (maxFd > 0)
            {
                CFE_ES_PerfLogEntry(VC_DEVICE_GET_PERF_ID);
                /* Wait for a socket fd to become ready */
                /* timeout set to null = block indefinitely */
                returnCode = select(maxFd + 1, &fds, 0, 0, 0);
                CFE_ES_PerfLogExit(VC_DEVICE_GET_PERF_ID);
            }
            else
            {
                /* No fd's were added to the set so error out */
                (void) CFE_EVS_SendEvent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR,
                        "VC can't start streaming no devices are streaming enabled");
                returnCode = -1;
                goto end_of_function;
            }
            
            /* select() wasn't successful */
            if (-1 == returnCode)
            {
                /* select was interrupted, try again */
                if (EINTR == errno)
                {
                    if (retryAttempts == VC_MAX_RETRY_ATTEMPTS)
                    {
                        goto end_of_function;
                    }
                    retryAttempts++;
                    usleep(VC_MAX_RETRY_SLEEP_USEC);
                    (void) CFE_EVS_SendEvent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR,
                        "VC select was interrupted");
                    continue;
                }
                else
                {
                    /* select returned an error other than EINTR */
                    (void) CFE_EVS_SendEvent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR,
                        "VC start streaming failed select() returned %i", errno);
                    goto end_of_function;
                }
            }
            /* select() returned and a socket is ready */
            if(returnCode > 0)
            {   
                /* Determine which socket is ready */    
                for (i=0; i < VC_MAX_DEVICES; i++)
                {
                    if(VC_AppCustomDevice.Channel[i].Mode == VC_DEVICE_ENABLED 
                    && VC_AppCustomDevice.Channel[i].Status == VC_DEVICE_INITIALIZED)
                    {
                        if(FD_ISSET(VC_AppCustomDevice.Channel[i].Socket, &fds))
                        {
                            /* Call send buffer with the socket that is ready */
                            VC_Send_Buffer(i);
                        }
                    }
                }
            }    
        } /* end while loop */
    } /* end if status == success */

end_of_function:

    /* Streaming task is exiting so set app flag to initialized */
    VC_AppData.AppState = VC_INITIALIZED;
    (void) CFE_EVS_SendEvent(VC_DEV_INF_EID, CFE_EVS_INFORMATION,
        "VC streaming task exited with return code %li task status (0x%08lX)",
        returnCode, iStatus);

    /* The child task was successfully created so exit from it */
    if (iStatus == CFE_SUCCESS)
    {
        CFE_ES_ExitChildTask();
    }
}


int32 VC_Send_Buffer(uint8 DeviceID)
{
    int32 returnCode          = 0;
    int size                  = 0;
    unsigned int packetLength = 0;

    if(DeviceID == 0)
    {
        size = recv(VC_AppCustomDevice.Channel[DeviceID].Socket, 
                VC_AppCustomDevice.Channel[DeviceID].Buffer, VC_MAX_PACKET_SIZE, 0);
        /* if we have the preamble header... */
        if (size == VC_MPARTMUX_HEADER_SIZE)
        {
            /* get the size of the payload from the preamble header */
            sscanf(&VC_AppCustomDevice.Channel[DeviceID].Buffer[VC_MPARTMUX_HEADER_LENGTH_START],
                    "%u", &packetLength);
            /* receive the payload */
            size = recv(VC_AppCustomDevice.Channel[DeviceID].Socket,
                    VC_AppCustomDevice.Channel[DeviceID].Buffer, VC_MAX_PACKET_SIZE, 0);
            /* if the size we received is not the size specified in the header
             * something went wrong. Or sscanf didn't find a match so we
             * don't have a size from the preamble etc */

            if (size != packetLength)
            {
                (void) CFE_EVS_SendEvent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR,
                    "VC recv size error on channel %u", (unsigned int)DeviceID);
                returnCode = -1;
                goto end_of_function;
            }
            /* Send data, for now map device id to senddata channel */
            if (-1 == VC_SendData(DeviceID, (void*)VC_AppCustomDevice.Channel[DeviceID].Buffer, packetLength))
            {
                (void) CFE_EVS_SendEvent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR,
                        "VC send data failed on channel %u", (unsigned int)DeviceID);
                returnCode = -1;
                goto end_of_function;
            }
        }
        /* if recv returned an error */
        else if (size == -1)
        {
            /* recv returned an error */
            (void) CFE_EVS_SendEvent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR,
                    "VC recv errno: %i on channel %u", errno, (unsigned int)DeviceID);
            returnCode = -1;
            goto end_of_function;
        }
        else
        {
            /* do nothing we received the 2 byte preamble */
            goto end_of_function;
        }
    }
    else if (DeviceID == 1)
    {
        size = recv(VC_AppCustomDevice.Channel[DeviceID].Socket,
                VC_AppCustomDevice.Channel[DeviceID].Buffer, PX4_OPTICAL_FLOW_FRAME_SIZE, 0);
        if(size == PX4_OPTICAL_FLOW_FRAME_SIZE)
        {
        	// Copy to message
        	uint64 timestamp;
			timestamp = PX4LIB_GetPX4TimeUs();
			OpticalFlowFrameMsg.Timestamp = timestamp;
			uint32 i;
			for (i=0; i<PX4_OPTICAL_FLOW_FRAME_SIZE;i++){
				OpticalFlowFrameMsg.Frame[i] = VC_AppCustomDevice.Channel[DeviceID].Buffer[i];
			}
			// Publish message to software bus
        	CFE_SB_TimeStampMsg((CFE_SB_Msg_t*) &OpticalFlowFrameMsg);
			CFE_SB_SendMsg((CFE_SB_Msg_t*) &OpticalFlowFrameMsg);
        }
    }

end_of_function:
    return (returnCode);
}
