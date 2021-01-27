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
#include "vc_dev_io_v4l.h"
#include "cfe.h"
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <sys/ioctl.h>
#include <string.h>
#include <linux/videodev2.h>

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

/**
 * Custom device layer app data declaration
 */
VC_AppCustomDevice_t VC_AppCustomDevice;


int32 VC_CustomDevice_InitData()
{
    int32 iStatus = CFE_SUCCESS;
    
    /* Set all struct zero values */
    bzero(&VC_AppCustomDevice, sizeof(VC_AppCustomDevice));
    
    /* Set all non-zero values for channel zero */
    VC_AppCustomDevice.ContinueFlag             = TRUE;
    VC_AppCustomDevice.Priority                 = VC_STREAMING_TASK_PRIORITY;
    VC_AppCustomDevice.TaskFlags                = VC_STREAMING_TASK_FLAGS;
    VC_AppCustomDevice.StreamingTask            = VC_Stream_Task;
    VC_AppCustomDevice.Channel[0].Status        = VC_DEVICE_UNINITIALIZED;
    VC_AppCustomDevice.Channel[0].Mode          = VC_DEVICE_ENABLED;
    VC_AppCustomDevice.Channel[0].BufferType    = VC_V4L_BUFFER_TYPE;
    VC_AppCustomDevice.Channel[0].Buffer_Size   = VC_MAX_BUFFER_SIZE;
    VC_AppCustomDevice.Channel[0].FrameWidth    = VC_FRAME_WIDTH;
    VC_AppCustomDevice.Channel[0].FrameHeight   = VC_FRAME_HEIGHT;
    VC_AppCustomDevice.Channel[0].VideoFormat   = VC_V4L_VIDEO_FORMAT;
    VC_AppCustomDevice.Channel[0].FieldOrder    = VC_V4L_VIDEO_FIELD_ORDER;
    VC_AppCustomDevice.Channel[0].BufferRequest = VC_V4L_BUFFER_REQUEST;
    VC_AppCustomDevice.Channel[0].MemoryType    = VC_V4L_MEMORY_TYPE;
    
    strncpy(VC_AppCustomDevice.Channel[0].DevName, VC_DEVICE_PATH, VC_MAX_DEVICE_PATH); 

    /* Initialize and patch MT9V117 device */
    MT9V117_Init_Device();

    return (iStatus);
}


int32 VC_Ioctl(int fh, int request, void *arg)
{
    int32 returnCode = 0;
    uint32 i         = 0;

    for (i=0; i < VC_MAX_RETRY_ATTEMPTS; i++)
    {
        returnCode = ioctl(fh, request, arg);
            
        if (-1 == returnCode && EINTR == errno)
        {
            usleep(VC_MAX_RETRY_SLEEP_USEC);
        }
        else
        {
            break;
        }
    }

    return (returnCode);
}


int32 VC_ConfigureDevice(uint8 DeviceID)
{
    int32 returnCode = 0;
    struct v4l2_format              Format = {};
    struct v4l2_capability          Capabilities = {};
    struct v4l2_requestbuffers      Request = {};

    bzero(&Format, sizeof(Format));
    Format.type                = VC_AppCustomDevice.Channel[DeviceID].BufferType;
    Format.fmt.pix.width       = VC_AppCustomDevice.Channel[DeviceID].FrameWidth;
    Format.fmt.pix.height      = VC_AppCustomDevice.Channel[DeviceID].FrameHeight;
    Format.fmt.pix.pixelformat = VC_AppCustomDevice.Channel[DeviceID].VideoFormat;
    Format.fmt.pix.field       = VC_AppCustomDevice.Channel[DeviceID].FieldOrder;
    Format.fmt.pix.colorspace  = V4L2_COLORSPACE_REC709;

    bzero(&Request, sizeof(Request));
    Request.count              = VC_AppCustomDevice.Channel[DeviceID].BufferRequest;
    Request.type               = VC_AppCustomDevice.Channel[DeviceID].BufferType;
    Request.memory             = VC_AppCustomDevice.Channel[DeviceID].MemoryType;

    if (-1 == VC_Ioctl(VC_AppCustomDevice.Channel[DeviceID].DeviceFd, VIDIOC_QUERYCAP, &Capabilities)) 
    {            
        (void) CFE_EVS_SendEvent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR,
                        "VC VIDIOC_QUERYCAP returned %i on %s channel %u", errno,
                        VC_AppCustomDevice.Channel[DeviceID].DevName, (unsigned int)DeviceID);
        returnCode = -1;
        goto end_of_function;
    }
    
    if (!(Capabilities.capabilities & VC_AppCustomDevice.Channel[DeviceID].BufferType)) 
    {
        (void) CFE_EVS_SendEvent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR,
                        "VC Capabilities %u on %s channel %u not found", 
                        (unsigned int)VC_AppCustomDevice.Channel[DeviceID].BufferType,
                        VC_AppCustomDevice.Channel[DeviceID].DevName, (unsigned int)DeviceID);
        returnCode = -1;
        goto end_of_function;
    }
    
    if (!(Capabilities.capabilities & V4L2_CAP_STREAMING)) 
    {
        (void) CFE_EVS_SendEvent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR,
                        "VC Capabilities %u on %s channel %u not found", 
                        V4L2_CAP_STREAMING,
                        VC_AppCustomDevice.Channel[DeviceID].DevName, (unsigned int)DeviceID);
        returnCode = -1;
        goto end_of_function;
    }
    
    if (-1 == VC_Ioctl(VC_AppCustomDevice.Channel[DeviceID].DeviceFd, VIDIOC_S_FMT, &Format)) 
    {
        (void) CFE_EVS_SendEvent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR,
                        "VIDIOC_S_FMT returned %i on %s channel %u", errno,
                        VC_AppCustomDevice.Channel[DeviceID].DevName, (unsigned int)DeviceID);
        returnCode = -1;
        goto end_of_function;
    }
    
    if (Format.fmt.pix.pixelformat != VC_AppCustomDevice.Channel[DeviceID].VideoFormat)
    {
        (void) CFE_EVS_SendEvent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR,
                        "VC device %s channel %u didn't accept format %u.",
                        VC_AppCustomDevice.Channel[DeviceID].DevName,
                        (unsigned int)DeviceID,
                        (unsigned int)VC_AppCustomDevice.Channel[DeviceID].VideoFormat);
        returnCode = -1;
        goto end_of_function;
    }
    
    if ((Format.fmt.pix.width != VC_AppCustomDevice.Channel[DeviceID].FrameWidth) || 
    (Format.fmt.pix.height != VC_AppCustomDevice.Channel[DeviceID].FrameHeight))
    {
         (void) CFE_EVS_SendEvent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR,
                        "VC device %s channel %u didn't accept resolution instead %d:%d.",
                        VC_AppCustomDevice.Channel[DeviceID].DevName, (unsigned int)DeviceID, 
                        Format.fmt.pix.width, Format.fmt.pix.height);
        returnCode = -1;
        goto end_of_function;
    }
    
    if (Format.fmt.pix.sizeimage > VC_AppCustomDevice.Channel[DeviceID].Buffer_Size)
    {
         (void) CFE_EVS_SendEvent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR,
                        "VC device %s channel %u size image %u > buffer size %lu.",
                        VC_AppCustomDevice.Channel[DeviceID].DevName, (unsigned int)DeviceID, 
                        Format.fmt.pix.sizeimage, VC_AppCustomDevice.Channel[DeviceID].Buffer_Size);
        returnCode = -1;
        goto end_of_function;
    }
    
    if (-1 == VC_Ioctl(VC_AppCustomDevice.Channel[DeviceID].DeviceFd, VIDIOC_REQBUFS, &Request))
    {
        (void) CFE_EVS_SendEvent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR,
                        "VC VIDIOC_REQBUFS returned %i on %s channel %u.", errno,
                        VC_AppCustomDevice.Channel[DeviceID].DevName, (unsigned int)DeviceID);
        returnCode = -1;
        goto end_of_function;
    }
    
    if (Request.count != VC_AppCustomDevice.Channel[DeviceID].BufferRequest)
    {
        (void) CFE_EVS_SendEvent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR,
                        "VC VIDIOC_REQBUFS did not comply. %u buffers on %s channel %u.", Request.count, 
                        VC_AppCustomDevice.Channel[DeviceID].DevName, (unsigned int)DeviceID);
        returnCode = -1;
        goto end_of_function;
    }

end_of_function:
    return (returnCode);
}


int32 VC_Start_StreamingDevice(uint8 DeviceID)
{
    int32 returnCode = 0;
    uint32 i         = 0;
    enum v4l2_buf_type    Type;
    struct v4l2_buffer    Buffer;
    
    Type = VC_AppCustomDevice.Channel[DeviceID].BufferType;
    
    for (i=0; i < VC_AppCustomDevice.Channel[DeviceID].BufferRequest; i++)
	{


		VC_AppCustomDevice.Channel[DeviceID].Buffer_Ptrs[i].ptr = (void*)&VC_AppCustomDevice.Channel[DeviceID].Buffers[i][0];
				//(struct Buffer *) malloc(Buffer.length);


		if (VC_AppCustomDevice.Channel[DeviceID].Buffer_Ptrs[i].ptr == NULL) {
			(void) CFE_EVS_SendEvent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR,
						"VC Unable to allocate buffers %i on %s channel %u", errno,
						VC_AppCustomDevice.Channel[DeviceID].DevName, (unsigned int)DeviceID);
			returnCode = -1;
			goto end_of_function;
		}

		bzero(&Buffer, sizeof(Buffer));
		Buffer.type            = VC_AppCustomDevice.Channel[DeviceID].BufferType;
		Buffer.memory          = VC_AppCustomDevice.Channel[DeviceID].MemoryType;
		Buffer.index           = i;

		if (-1 == VC_Ioctl(VC_AppCustomDevice.Channel[DeviceID].DeviceFd, VIDIOC_QBUF, &Buffer))
		{
			(void) CFE_EVS_SendEvent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR,
						"VC VIDIOC_QBUF returned %i on %s channel %u", errno,
						VC_AppCustomDevice.Channel[DeviceID].DevName, (unsigned int)DeviceID);
			returnCode = -1;
			goto end_of_function;
		}

		VC_AppCustomDevice.Channel[DeviceID].Buffer_Ptrs[i].length =  Buffer.length;
		VC_AppCustomDevice.Channel[DeviceID].Buffer_Ptrs[i].ptr = mmap(VC_AppCustomDevice.Channel[DeviceID].Buffer_Ptrs[i].ptr,
											Buffer.length,
		                                  PROT_READ | PROT_WRITE,
		                                  MAP_SHARED, VC_AppCustomDevice.Channel[DeviceID].DeviceFd,
		                                  Buffer.m.offset);

		if (VC_AppCustomDevice.Channel[DeviceID].Buffer_Ptrs[i].ptr == MAP_FAILED) {
			(void) CFE_EVS_SendEvent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR,
						"OUT_OF_MEMORY returned %i on %s channel %u", errno,
						VC_AppCustomDevice.Channel[DeviceID].DevName, (unsigned int)DeviceID);
			returnCode = -1;
			goto end_of_function;
		}
	}

    if (-1 == VC_Ioctl(VC_AppCustomDevice.Channel[DeviceID].DeviceFd, VIDIOC_STREAMON, &Type))
    {
        (void) CFE_EVS_SendEvent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR,
                    "VC VIDIOC_STREAMON returned %i on %s channel %u", errno,
                    VC_AppCustomDevice.Channel[DeviceID].DevName, (unsigned int)DeviceID);
        returnCode = -1;
        goto end_of_function;
    }
    else
    {
        (void) CFE_EVS_SendEvent(VC_DEV_INF_EID, CFE_EVS_INFORMATION,
                    "VC VIDIOC_STREAMON success on %s channel %u", 
                    VC_AppCustomDevice.Channel[DeviceID].DevName, (unsigned int)DeviceID);
    }

end_of_function:

    return (returnCode);
}


int32 VC_Stop_StreamingDevice(uint8 DeviceID)
{
    int32 returnCode = 0;
    uint32 i         = 0;
    enum v4l2_buf_type Type;
    
    Type = VC_AppCustomDevice.Channel[DeviceID].BufferType;
    
    if (-1 == VC_Ioctl(VC_AppCustomDevice.Channel[DeviceID].DeviceFd, VIDIOC_STREAMOFF, &Type))
    {
        returnCode = -1;
    }

    return (returnCode);
}


int32 VC_Send_Buffer(uint8 DeviceID)
{
    int32 returnCode  = 0;
    uint32 i          = 0;
    boolean checkFlag = FALSE;
    
    struct v4l2_buffer Buffer;
    
    bzero(&Buffer, sizeof(Buffer));
    Buffer.type            = VC_AppCustomDevice.Channel[DeviceID].BufferType;
    Buffer.memory          = VC_AppCustomDevice.Channel[DeviceID].MemoryType;

    /* Dequeue the ready buffer */
    if (-1 == VC_Ioctl(VC_AppCustomDevice.Channel[DeviceID].DeviceFd, VIDIOC_DQBUF, &Buffer))
    {
            /* VIDIOC_DQBUF failed */
            (void) CFE_EVS_SendEvent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR,
                    "VC VIDIOC_DQBUF returned %i on %s channel %u", errno,
                    VC_AppCustomDevice.Channel[DeviceID].DevName, (unsigned int)DeviceID);
            returnCode = -1;
            goto end_of_function;
    }
    /* The dequeued buffer is larger than max transmit packet size */
    if (Buffer.bytesused > VC_MAX_PACKET_SIZE)
    {
        /* The buffer is too large so skip sending */

        (void) CFE_EVS_SendEvent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR,
                "VC Packet on %s channel %u is too large",
                VC_AppCustomDevice.Channel[DeviceID].DevName, (unsigned int)DeviceID);
            //returnCode = -1;
            goto queue_next_buffer;
    }
    /* Send data, for now map device id to senddata channel */
    if (-1 == VC_SendData(DeviceID, (void*)VC_AppCustomDevice.Channel[DeviceID].Buffer_Ptrs[Buffer.index].ptr, Buffer.bytesused))
    {
        (void) CFE_EVS_SendEvent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR,
                "VC send data failed on %s channel %u",
                VC_AppCustomDevice.Channel[DeviceID].DevName, (unsigned int)DeviceID);
        returnCode = -1;
        goto end_of_function;
    }
    
queue_next_buffer:
    /* Queue the next buffer */
    if (-1 == VC_Ioctl(VC_AppCustomDevice.Channel[DeviceID].DeviceFd, VIDIOC_QBUF, &Buffer))
    {
        (void) CFE_EVS_SendEvent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR,
                "VC VIDIOC_QBUF returned %i on %s channel %u", errno,
                VC_AppCustomDevice.Channel[DeviceID].DevName, (unsigned int)DeviceID);
        returnCode = -1;
    }
    
end_of_function:
    return (returnCode);
}


void VC_Stream_Task(void)
{
    int32 returnCode = 0;
    static int32 timeouts = 0;

    uint32 i = 0;
    uint32 j = 0;
    uint32 maxFd = 0;
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
            
            /* Select modifies the timeout value with time left until 
             * the timeout would expire so timeValue needs to be set
             * every loop iteration
             */
            timeValue.tv_sec = VC_BUFFER_FILL_TIMEOUT_SEC;
            timeValue.tv_usec = VC_BUFFER_FILL_TIMEOUT_USEC;

            /* Initialize the set */
            FD_ZERO(&fds);
        
            /* Add enabled and streaming devices to the fd set */
            for (i=0; i < VC_MAX_DEVICES; i++)
            {
                if(VC_AppCustomDevice.Channel[i].Mode == VC_DEVICE_ENABLED
                && VC_AppCustomDevice.Channel[i].Status == VC_DEVICE_STREAMING)
                {
                    FD_SET(VC_AppCustomDevice.Channel[i].DeviceFd, &fds);

                    /* Get the greatest fd value for select() */
                    if (VC_AppCustomDevice.Channel[i].DeviceFd > maxFd)
                    {
                        /* maxFd is needed for select */
                        maxFd = VC_AppCustomDevice.Channel[i].DeviceFd;
                    }
                }
            }
            /* If maxFd is > 0 a fd was added the set so call select */
            if (maxFd > 0)
            {
                CFE_ES_PerfLogEntry(VC_DEVICE_GET_PERF_ID);
                /* Wait for a queued buffer to be filled by the device */
                returnCode = select(maxFd + 1, &fds, 0, 0, &timeValue);
                usleep(1000);
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
            /* select timed out */
            if (0 == returnCode)
            {
                if (timeouts == VC_BUFFER_TIMEOUTS_ALLOWED)
                {
                    returnCode = -1;
                    goto end_of_function;
                }
                timeouts++;
                usleep(VC_MAX_RETRY_SLEEP_USEC);
                CFE_EVS_SendEvent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR,
                        "VC select timed out");
                continue;
            }
            /* select() returned and a buffer is ready to be dequeued */
            if(returnCode > 0)
            {
                /* Determine which device is ready */
                for (i=0; i < VC_MAX_DEVICES; i++)
                {
                    if(VC_AppCustomDevice.Channel[i].Mode == VC_DEVICE_ENABLED
                    && VC_AppCustomDevice.Channel[i].Status == VC_DEVICE_STREAMING)
                    {
                        if(FD_ISSET(VC_AppCustomDevice.Channel[i].DeviceFd, &fds))
                        {
                            /* Call send buffer with the device that is ready */
                            returnCode = VC_Send_Buffer(i);
                            if(returnCode != 0)
                            {
                                (void) CFE_EVS_SendEvent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR,
                                    "VC Send_Buffer failed, a device may have been disconnected, disabling device %u", (unsigned int)i);
                                VC_DisableDevice(i);
                            }
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


int32 VC_Start_Streaming(void)
{
    uint32 i         = 0;
    int32 returnCode = 0;
    
    for(i=0; i < VC_MAX_DEVICES; i++)
    {
        if((VC_AppCustomDevice.Channel[i].Mode == VC_DEVICE_ENABLED) &&
        (VC_AppCustomDevice.Channel[i].Status == VC_DEVICE_INITIALIZED))
        {
            if(-1 == VC_Start_StreamingDevice(i))
            {
                /* Start streaming failed */
                returnCode = -1;
            }
            else
            {
                /* Start streaming was successful set status to streaming */
                VC_AppCustomDevice.Channel[i].Status = VC_DEVICE_STREAMING;
            }
        }
    }
    return (returnCode);
}


int32 VC_Stop_Streaming(void)
{
    uint32 i         = 0;
    int32 returnCode = 0;
    
    for(i=0; i < VC_MAX_DEVICES; i++)
    {
        if((VC_AppCustomDevice.Channel[i].Mode == VC_DEVICE_ENABLED) &&
        (VC_AppCustomDevice.Channel[i].Status == VC_DEVICE_STREAMING))
        {
            if(-1 == VC_Stop_StreamingDevice(i))
            {
                /* Stop streaming failed */
                returnCode = -1;
            }
            else
            {
                /* Stop streaming was successful set status to initialized */
                VC_AppCustomDevice.Channel[i].Status = VC_DEVICE_INITIALIZED;
            }
        }
    }
    return (returnCode);
}


int32 VC_InitDevice(uint8 DeviceID, const char *DeviceName)
{
    int32 returnCode = 0;
    uint32 i         = 0;

    if((VC_AppCustomDevice.Channel[DeviceID].Mode == VC_DEVICE_ENABLED) &&
        (VC_AppCustomDevice.Channel[DeviceID].DeviceFd != 0))
    {
        (void) CFE_EVS_SendEvent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR,
                        "VC Device %s for channel %u already initialized.", DeviceName, (unsigned int)i);
        returnCode = -1;
        goto end_of_function;
    }

    if(DeviceName == 0)
    {
        (void) CFE_EVS_SendEvent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR,
                        "VC Device name for channel %u is null.", (unsigned int)i);
        returnCode = -1;
        goto end_of_function;
    }

    if(DeviceID >= VC_MAX_DEVICES)
    {
        (void) CFE_EVS_SendEvent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR,
                        "VC DeviceID (%u) invalid.", (unsigned int)DeviceID);
        returnCode = -1;
        goto end_of_function;
    }
    VC_AppCustomDevice.Channel[DeviceID].DeviceFd = open(DeviceName, O_RDWR | O_NONBLOCK);
    if(VC_AppCustomDevice.Channel[DeviceID].DeviceFd < 0)
    {
        (void) CFE_EVS_SendEvent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR,
                        "VC Device open errno: %i on channel %u", errno, (unsigned int)i);
        returnCode = -1;
        goto end_of_function;
    }

end_of_function:
    return (returnCode);
}


int32 VC_Init_CustomDevices(void)
{
    uint32 i         = 0;
    int32 returnCode = 0;

    for (i=0; i < VC_MAX_DEVICES; i++)
    {
        if(VC_AppCustomDevice.Channel[i].Mode == VC_DEVICE_ENABLED)
        {
            /* If the device is enabled initialize it */
            returnCode = VC_InitDevice(i, VC_AppCustomDevice.Channel[i].DevName);
            if (returnCode)
            {
                /* If the device failed to be initialized set to disabled 
                 * an error event will be generated in VC_InitDevice */
                VC_AppCustomDevice.Channel[i].Mode = VC_DEVICE_DISABLED;
            }
            else
            {
                /* Success, raise info event */
                (void) CFE_EVS_SendEvent(VC_DEV_INF_EID, CFE_EVS_INFORMATION,
                        "VC Device initialized channel %u from %s",
                        (unsigned int)i, VC_AppCustomDevice.Channel[i].DevName);
                        
                /* The device is initialized so now configure it */
                returnCode = VC_ConfigureDevice(i);
                if (returnCode)
                {
                    /* The device failed to be configured so make sure
                     * the status is set to uninitialized 
                     */
                    VC_AppCustomDevice.Channel[i].Status = VC_DEVICE_UNINITIALIZED;
                }
                else
                {
                    /* Success, set status to device initialized and
                     * raise a info event
                     */
                    VC_AppCustomDevice.Channel[i].Status = VC_DEVICE_INITIALIZED;
                    (void) CFE_EVS_SendEvent(VC_DEV_INF_EID, CFE_EVS_INFORMATION,
                            "VC Device configured channel %u from %s",
                            (unsigned int)i, VC_AppCustomDevice.Channel[i].DevName);
                }
            }
        }
    }
    return (returnCode);
}


int32 VC_CleanupDevices(void)
{
    uint32 i         = 0;
    int32 returnCode = 0;
    
    for(i=0; i < VC_MAX_DEVICES; i++)
    {
        if(VC_AppCustomDevice.Channel[i].Mode == VC_DEVICE_ENABLED)
        {
            if(VC_AppCustomDevice.Channel[i].Status == VC_DEVICE_STREAMING)
            {    
                if(-1 == VC_Stop_StreamingDevice(i))
                {
                    /* VC_Stop_Streaming failed */
                    returnCode = -1;
                }
                else
                {
                    /* Success set device status to initialized */
                    VC_AppCustomDevice.Channel[i].Status = VC_DEVICE_INITIALIZED;
                }
            }
            
            if(-1 == VC_DisableDevice(i))
            {
                /* VC_DisableDevice failed */
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


void VC_Devices_Critical_Cleanup(void)
{
    uint8 i = 0;

    for(i=0; i < VC_MAX_DEVICES; i++)
    {
        if(VC_AppCustomDevice.Channel[i].DeviceFd != 0)
        {
            ioctl(VC_AppCustomDevice.Channel[i].DeviceFd, VIDIOC_STREAMOFF, &VC_AppCustomDevice.Channel[i].BufferType);
            close(VC_AppCustomDevice.Channel[i].DeviceFd);
        }
    }
    return;
}


int32 VC_DisableDevice(uint8 DeviceID)
{
    int32 returnCode = 0;

    if(VC_AppCustomDevice.Channel[DeviceID].Mode != VC_DEVICE_ENABLED)
    {
        returnCode = -1;
        goto end_of_function;
    }

    returnCode = close(VC_AppCustomDevice.Channel[DeviceID].DeviceFd);
    if (0 == returnCode)
    {
        VC_AppCustomDevice.Channel[DeviceID].DeviceFd = 0;
    }

end_of_function:
    return (returnCode);
}


/*
 * Start streaming on all enabled and initialized devices
 * True for success, false for failure
 * Note: Creates the streaming task
 */
boolean VC_Devices_Start(void)
{
    int32 returnCode = 0;
    boolean returnBool = TRUE;
    
    /* Set loop flag to continue forever */
    VC_AppCustomDevice.ContinueFlag = TRUE;
    
    /* Start streaming on all devices */
    if(-1 == VC_Start_Streaming())
    {
        VC_AppCustomDevice.ContinueFlag = FALSE;
        returnBool = FALSE;
        goto end_of_function;
    }

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

end_of_function:

    return (returnBool);
}


/*
 * Stop streaming on all enabled and initialized devices
 * True for success, false for failure
 * Note: Deletes the streaming task
 */
boolean VC_Devices_Stop(void)
{
    boolean returnBool = TRUE;

    /* Delete the child task */
    CFE_ES_DeleteChildTask(VC_AppCustomDevice.ChildTaskID);
    
    /* Set streaming task loop flag to stop */
    VC_AppCustomDevice.ContinueFlag = FALSE;

    /* Set app state to initialized */
    VC_AppData.AppState = VC_INITIALIZED;
    
    if(-1 == VC_Stop_Streaming())
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


int32 VC_Devices_InitData()
{
    return VC_CustomDevice_InitData();
}

