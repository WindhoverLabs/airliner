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

#ifndef VC_DEV_IO_V4L_H
#define VC_DEV_IO_V4L_H

#include "cfe.h"
#include "vc_dev_io.h"
#include "../vc_custom_shared.h"
#include "../vc_dev_io_i2c/vc_dev_io_i2c.h"
#include "vc_perfids.h"
#include "vc_app.h"

/************************************************************************
** Structure Declarations
*************************************************************************/

/**
 * Device modes
 */
typedef enum
{
    /*! Device state unused  */
    VC_DEVICE_UNUSED       = 0,
    /*! Device state disabled */
    VC_DEVICE_DISABLED     = 1,
    /*! Device state enabled */
    VC_DEVICE_ENABLED      = 2
} VC_DeviceMode_t;


/**
 * Device status
 */
typedef enum
{
    /*! Device status uninitialized */
    VC_DEVICE_UNINITIALIZED = 0,
    /*! Device status uninitialized */
    VC_DEVICE_INITIALIZED   = 1,
    /*! Device status streaming */
    VC_DEVICE_STREAMING     = 2
} VC_DeviceStatus_t;


/**
 * Buffer pointers for V4L user pointer IO
 */
typedef struct {
    /*! Pointer to user defined buffer */
    void   *ptr;
    size_t length;
}VC_BufferPtr_t;



/**
 * Device handle
 */
typedef struct
{
    /*! Device status */
    VC_DeviceStatus_t   Status;
    /*! Device mode */
    VC_DeviceMode_t     Mode;
    /*! Device resource channel identifier */
    uint8               ChannelID;
    /*! Path to device */
    char                DevName[VC_MAX_DEVICE_PATH];
    /*! Device file descriptor */
    int                 DeviceFd;
    /*! V4L buffer type */
    uint32              BufferType;
    /*! V4L frame width */
    uint32              FrameWidth;
    /*! V4L frame height */
    uint32              FrameHeight;
    /*! V4L video format */
    uint32              VideoFormat;
    /*! V4L field order */
    uint8               FieldOrder;
    /*! V4L buffer request */
    uint8               BufferRequest;
    /*! V4L memory type */
    uint32              MemoryType;
    /*! V4L user defined buffer(s) */
    char                Buffers[VC_V4L_BUFFER_REQUEST][VC_MAX_BUFFER_SIZE];
    /*! V4L buffer size (for all buffers)  */
    uint32              Buffer_Size;
    /*! V4L user defined buffer pointer(s) */
    VC_BufferPtr_t      Buffer_Ptrs[VC_V4L_BUFFER_REQUEST];
} VC_Device_Handle_t;


/**
 * Global data structure for custom device IO layer
 */
typedef struct
{
    /*! Flag to start and stop streaming */
    boolean                         ContinueFlag;
    /*! Streaming task priority */
    uint8                           Priority;
    /*! Streaming child task identifier */
    uint32                          ChildTaskID;
    /*! Streaming task function pointer */
    CFE_ES_ChildTaskMainFuncPtr_t   StreamingTask;
    /*! Device resource(s) channels */
    VC_Device_Handle_t              Channel[VC_MAX_DEVICES];
    /*! Streaming task flags */
    uint32                          TaskFlags;
} VC_AppCustomDevice_t;

/************************************************************************
** Function Prototypes
*************************************************************************/

/**
 * @brief Custom function to initialize custom device data structure.
 * @return CFE_SUCCESS
 */
int32 VC_CustomDevice_InitData(void);


/**
 * @brief ioctl with limited EINTR retry attempts
 * @param fh file descriptor
 * @param request ioctl request code
 * @param arg pointer to a device specific struct
 * @return usually 0 for success and -1 for failure 
 * @note see ioctl man-page for more info
 */
int32 VC_Ioctl(int fh, int request, void *arg);


/**
 * @brief Initialize, configure, and start all enabled devices
 * @return usually 0 for success -1 for failure
 * @note see ioctl man-page for more info
 */
int32 VC_Init_CustomDevices(void); 


/**
 * @brief Initialize an enabled device
 * @param DeviceID the device to initialize
 * @param DeviceName the device name (path) to open
 * @return 0 for success -1 for failure
 */
int32 VC_InitDevice(uint8 DeviceID, const char *DeviceName);


/**
 * Configure device resource
 * @param DeviceID the device ID of the resource to configure
 * @return 0 for success -1 for failure
 */
int32 VC_ConfigureDevice(uint8 DeviceID);


/**
 * @brief Start streaming on all devices
 * @return 0 for success -1 for failure
 */
int32 VC_Start_Streaming(void);


/**
 * @brief start streaming on a particular device
 * @param DeviceID the device ID to start streaming on
 * @return 0 for success -1 for failure
 */
int32 VC_Start_StreamingDevice(uint8 DeviceID);


/**
 * @brief Streaming task, takes ready buffer and sends
 * @note This is a loop to be run in a VC app child task
 */
void VC_Stream_Task(void);


/**
 * @brief dequeue and send a buffer from a "ready" device
 * @param DeviceID the device ID of the "ready" device
 * @return 0 for success -1 for failure
 */
int32 VC_Send_Buffer(uint8 DeviceID);


/**
 * @brief Stop streaming on all devices
 * @return 0 for success -1 for failure
 */
int32 VC_Stop_Streaming(void);


/**
 * @brief stop streaming on a particular device
 * @param DeviceID the device ID to stop streaming on
 * @return 0 for success -1 for failure
 */
int32 VC_Stop_StreamingDevice(uint8 DeviceID);


/**
 * @brief Disable a device
 * @param DeviceID the device to enable
 * @return 0 for success -1 for failure
 */
int32 VC_DisableDevice(uint8 DeviceID);


/**
 * @brief Cleanup (stop & close) all enabled and initialized devices
 * @return 0 for success -1 for failure
 */
int32 VC_CleanupDevices(void);


extern VC_AppCustomDevice_t VC_AppCustomDevice;

#endif

