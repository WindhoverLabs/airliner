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
#include "cfe.h"
#include "ms5607_i2c.h"
#include "ms5607_events.h"
#include "ms5607_perfids.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

/************************************************************************
** Local Defines
*************************************************************************/

/************************************************************************
** Local Structure Declarations
*************************************************************************/
typedef enum 
{
/** \brief <tt> 'MS5607 - ' </tt>
**  \event <tt> 'MS5607 - ' </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when a device resource encounters an 
**  error.
**
*/
    MS5607_DEVICE_ERR_EID = MS5607_EVT_CNT,

/** \brief Number of custom events 
**
**  \par Limits:
**       int32
*/
    MS5607_CUSTOM_EVT_CNT
} MS5607_CustomEventIds_t;


/************************************************************************
** External Global Variables
*************************************************************************/

/************************************************************************
** Global Variables
*************************************************************************/
MS5607_AppCustomData_t MS5607_AppCustomData;


/************************************************************************
** Local Variables
*************************************************************************/

/************************************************************************
** Local Function Definitions
*************************************************************************/
int32 MS5607_Ioctl(int fh, int request, void *arg)
{
    int32 returnCode = 0;
    uint32 i         = 0;

    for (i=0; i < MS5607_MAX_RETRY_ATTEMPTS; i++)
    {
        returnCode = ioctl(fh, request, arg);
            
        if (-1 == returnCode && EINTR == errno)
        {
            usleep(MS5607_MAX_RETRY_SLEEP_USEC);
        }
        else
        {
            break;
        }
    }

    return (returnCode);
}


void MS5607_Custom_InitData(void)
{
    /* Set all struct zero values */
    bzero(&MS5607_AppCustomData, sizeof(MS5607_AppCustomData));
    return;
}


boolean MS5607_Custom_Init()
{
    boolean returnBool = TRUE;

    MS5607_AppCustomData.DeviceFd = open(MS5607_I2C_DEVICE_PATH, O_RDWR);
    if (MS5607_AppCustomData.DeviceFd < 0) 
    {
        (void) CFE_EVS_SendEvent(MS5607_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "MS5607 Device open errno: %i", errno);
        returnBool = FALSE;
        goto end_of_function;
    }
    else
    {
        MS5607_AppCustomData.Status = MS5607_CUSTOM_INITIALIZED;
    }

end_of_function:
    return (returnBool);
}


boolean MS5607_Custom_Uninit(void)
{
    boolean returnBool = TRUE;
    int returnCode     = 0;

    returnCode = close(MS5607_AppCustomData.DeviceFd);
    if (-1 == returnCode) 
    {
        (void) CFE_EVS_SendEvent(MS5607_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "MS5607 Device close errno: %i", errno);
        returnBool = FALSE;
    }
    else
    {
        MS5607_AppCustomData.Status = MS5607_CUSTOM_UNINITIALIZED;
    }
    return (returnBool);
}


void MS5607_Critical_Cleanup(void)
{
    close(MS5607_AppCustomData.DeviceFd);
}


int32 MS5607_ResetDevice(void)
{
    boolean returnBool = TRUE;
    uint8 reg          = 0;
    int32 ret          = 0;

    reg = MS5607_I2C_CMD_RESET;

    returnBool = MS5607_Custom_Send(reg);
    if (FALSE == returnBool) 
    {            
        ret = -1;
        (void) CFE_EVS_SendEvent(MS5607_DEVICE_ERR_EID, CFE_EVS_ERROR,
                        "MS5607 send failed in resetDevice");
    }
    else
    {
        usleep(MS5607_RESET_INTERVAL);
    }
    return (ret);
}


boolean MS5607_ReadPROM(uint8 Addr, uint16 *returnVal)
{
    boolean returnBool = TRUE;
    uint8  rxBuf[2]    = {0, 0};
    uint8  reg         = 0;
    
    /* Null pointer check */
    if(0 == returnVal)
    {
        CFE_EVS_SendEvent(MS5607_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "MS5607 ReadProm Null Pointer");
        returnBool = FALSE;
        goto end_of_function;
    }

    reg = MS5607_I2C_CMD_PROM_READ_MASK + (Addr << MS5607_I2C_CMD_PROM_ADDR_SHIFT);

    returnBool = MS5607_Custom_Receive(reg, &rxBuf[0], 2);
    if (FALSE == returnBool) 
    {            
        (void) CFE_EVS_SendEvent(MS5607_DEVICE_ERR_EID, CFE_EVS_ERROR,
                        "MS5607 receive failed in readPROM");
        goto end_of_function;
    }

    *returnVal = (rxBuf[0] << 8) + rxBuf[1];
    
end_of_function:
    return (returnBool);
}


boolean MS5607_D1Conversion(void)
{
    boolean returnBool = TRUE;
    uint8 reg          = MS5607_I2C_CMD_CONVERT_D1;

    returnBool = MS5607_Custom_Send(reg);
    if (FALSE == returnBool) 
    {            
        (void) CFE_EVS_SendEvent(MS5607_DEVICE_ERR_EID, CFE_EVS_ERROR,
                        "MS5607 send failed in D1Conversion");
    }
    else
    {
        usleep(MS5607_CONVERSION_INTERVAL);
    }
    return (returnBool);
}


boolean MS5607_D2Conversion(void)
{
    boolean returnBool = TRUE;
    uint8 reg          = MS5607_I2C_CMD_CONVERT_D2;

    returnBool = MS5607_Custom_Send(reg);
    if (FALSE == returnBool) 
    {            
        (void) CFE_EVS_SendEvent(MS5607_DEVICE_ERR_EID, CFE_EVS_ERROR,
                        "MS5607 send failed in D2Conversion");
    }
    else
    {
        usleep(MS5607_CONVERSION_INTERVAL);
    }
    return (returnBool);
}


boolean MS5607_ReadADCResult(uint32 *returnVal)
{
    boolean returnBool = TRUE;
    uint8 reg          = MS5607_I2C_CMD_ADC_READ;

    uint8  rxBuf[3] = {0, 0, 0};

    /* Null pointer check */
    if(0 == returnVal)
    {
        (void) CFE_EVS_SendEvent(MS5607_DEVICE_ERR_EID, CFE_EVS_ERROR,
                "MS5607 ReadProm Null Pointer");
        returnBool = FALSE;
        goto end_of_function;
    }

    returnBool = MS5607_Custom_Receive(reg, &rxBuf[0], 3);
    if (FALSE == returnBool) 
    {            
        (void) CFE_EVS_SendEvent(MS5607_DEVICE_ERR_EID, CFE_EVS_ERROR,
                        "MS5607 receive failed in ReadADCResult");
        goto end_of_function;
    }
    *returnVal = (rxBuf[0] << 16) + (rxBuf[1] << 8) + rxBuf[2];

end_of_function:

    return (returnBool);
}


boolean MS5607_Custom_Send(uint8 Reg)
{
    int returnCode      = 0;
    boolean returnBool  = FALSE;
    struct i2c_msg Messages[1];
    struct i2c_rdwr_ioctl_data Packets;
    uint8 buf[1];

    buf[0] = Reg;

    Messages[0].addr  = MS5607_I2C_SLAVE_ADDRESS;
    Messages[0].flags = 0;
    Messages[0].buf   = &buf[0];
    Messages[0].len   = 1;

    Packets.msgs  = Messages;
    Packets.nmsgs = 1;

    CFE_ES_PerfLogEntry(MS5607_SEND_PERF_ID);
    returnCode = MS5607_Ioctl(MS5607_AppCustomData.DeviceFd, I2C_RDWR, &Packets);
    CFE_ES_PerfLogExit(MS5607_SEND_PERF_ID);
    
    if (-1 == returnCode) 
    {            
        (void) CFE_EVS_SendEvent(MS5607_DEVICE_ERR_EID, CFE_EVS_ERROR,
                        "MS5607 ioctl returned errno %i", errno);
    }
    else
    {
        returnBool = TRUE;
    }

    return (returnBool);
}


boolean MS5607_Custom_Receive(uint8 Reg, uint8 *Buffer, size_t Length)
{
    int returnCode       = 0;
    boolean returnBool   = FALSE;
    uint8 cmd            = Reg;

    struct i2c_rdwr_ioctl_data Packets;
    struct i2c_msg Messages[2];

    /* send */
    Messages[0].addr  = MS5607_I2C_SLAVE_ADDRESS;
    Messages[0].flags = 0;
    Messages[0].buf   = &cmd;
    Messages[0].len   = 1;

    /* receive */
    Messages[1].addr  = MS5607_I2C_SLAVE_ADDRESS;
    Messages[1].flags = MS5607_I2C_M_READ;
    Messages[1].buf   = Buffer;
    Messages[1].len   = Length;

    Packets.msgs  = Messages;
    Packets.nmsgs = 2;

    CFE_ES_PerfLogEntry(MS5607_RECEIVE_PERF_ID);
    returnCode = MS5607_Ioctl(MS5607_AppCustomData.DeviceFd, I2C_RDWR, &Packets);
    CFE_ES_PerfLogExit(MS5607_RECEIVE_PERF_ID);

    if (-1 == returnCode) 
    {            
        (void) CFE_EVS_SendEvent(MS5607_DEVICE_ERR_EID, CFE_EVS_ERROR,
                        "MS5607 ioctl returned errno %i", errno);
    }
    else
    {
        returnBool = TRUE;
    }

    return (returnBool);
}


boolean MS5607_Custom_Max_Events_Not_Reached(int32 ind)
{
    boolean returnBool = FALSE;

    if ((ind < CFE_EVS_MAX_EVENT_FILTERS) && (ind > 0))
    {
        returnBool = TRUE;
    }

    return (returnBool);
}


int32 MS5607_Custom_Init_EventFilters(int32 ind, CFE_EVS_BinFilter_t *EventTbl)
{
    int32 customEventCount = ind;
    
    /* Null check */
    if(0 == EventTbl)
    {
        customEventCount = -1;
        goto end_of_function;
    }

    if(TRUE == MS5607_Custom_Max_Events_Not_Reached(customEventCount))
    {
        EventTbl[  customEventCount].EventID = MS5607_DEVICE_ERR_EID;
        EventTbl[customEventCount++].Mask    = CFE_EVS_FIRST_16_STOP;
    }
    else
    {
        customEventCount = -1;
        goto end_of_function;
    }
    
end_of_function:

    return (customEventCount);
}

