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
#include "hmc5883_i2c.h"
#include "hmc5883_events.h"
#include "hmc5883_perfids.h"
#include "px4lib.h"
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <time.h>

/************************************************************************
** Local Defines
*************************************************************************/

/************************************************************************
** Local Structure Declarations
*************************************************************************/
typedef enum {

/** \brief <tt> 'HMC5883 - ' </tt>
**  \event <tt> 'HMC5883 - ' </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when a device resource encounters an 
**  error.
**
*/
    HMC5883_DEVICE_ERR_EID = HMC5883_EVT_CNT,

/** \brief Number of custom events 
**
**  \par Limits:
**       int32
*/
    HMC5883_CUSTOM_EVT_CNT
} HMC5883_CustomEventIds_t;


/************************************************************************
** External Global Variables
*************************************************************************/

/************************************************************************
** Global Variables
*************************************************************************/
HMC5883_AppCustomData_t HMC5883_AppCustomData;

/************************************************************************
** Local Variables
*************************************************************************/

/************************************************************************
** Local Function Definitions
*************************************************************************/
int32 HMC5883_Ioctl(int fh, int request, void *arg)
{
    int32 returnCode = 0;
    uint32 i         = 0;

    for (i=0; i < HMC5883_MAX_RETRY_ATTEMPTS; i++)
    {
        returnCode = ioctl(fh, request, arg);
            
        if (-1 == returnCode && EINTR == errno)
        {
            usleep(HMC5883_MAX_RETRY_SLEEP_USEC);
        }
        else
        {
            break;
        }
    }

    return (returnCode);
}


void HMC5883_Custom_InitData(void)
{
    /* Set all struct zero values */
    bzero(&HMC5883_AppCustomData, sizeof(HMC5883_AppCustomData));
    return;
}


boolean HMC5883_Custom_Init()
{
    boolean returnBool = TRUE;

    HMC5883_AppCustomData.DeviceFd = open(HMC5883_I2C_DEVICE_PATH, O_RDWR);
    
    if (HMC5883_AppCustomData.DeviceFd < 0) 
    {
        (void) CFE_EVS_SendEvent(HMC5883_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "HMC5883 Device open errno: %i", errno);
        returnBool = FALSE;
        goto end_of_function;
    }
    else
    {
        HMC5883_AppCustomData.Status = HMC5883_CUSTOM_INITIALIZED;
    }

end_of_function:
    return (returnBool);
}


boolean HMC5883_Custom_Set_Range(uint8 Range)
{
    boolean returnBool = FALSE;

    returnBool = HMC5883_Custom_Send(HMC5883_I2C_REG_CONFIG_B, 
            Range);

    return (returnBool);
}


boolean HMC5883_Custom_Get_Range(uint8 *Range)
{
    boolean returnBool = FALSE;

    returnBool = HMC5883_Custom_Receive(HMC5883_I2C_REG_CONFIG_B, 
            Range, 1);

    return (returnBool);
}


boolean HMC5883_Custom_Check_Range(uint8 Range)
{
    boolean returnBool = FALSE;
    
    uint8 Result[1] = { 0 };
    
    returnBool = HMC5883_Custom_Receive(HMC5883_I2C_REG_CONFIG_B, 
            Result, sizeof(Result));
    /* If the gain setting is corrupt, reset the value */
    if (Range != Result[0])
    {
        returnBool = HMC5883_Custom_Set_Range(Range);
    }

    return (returnBool);
}


boolean HMC5883_Custom_Set_Config(uint8 Config)
{
    boolean returnBool = FALSE;
    
    returnBool = HMC5883_Custom_Send(HMC5883_I2C_REG_CONFIG_A, 
            Config);

    return (returnBool);
}


boolean HMC5883_Custom_Get_Config(uint8 *Config)
{
    boolean returnBool = FALSE;

    returnBool = HMC5883_Custom_Receive(HMC5883_I2C_REG_CONFIG_A, 
            Config, 1);

    return (returnBool);
}


boolean HMC5883_Custom_Check_Config(uint8 Config)
{
    boolean returnBool = FALSE;
    
    uint8 Result[1] = { 0 };
    
    returnBool = HMC5883_Custom_Receive(HMC5883_I2C_REG_CONFIG_A, 
            Result, sizeof(Result));
    if (Config != Result[0])
    {
        returnBool = HMC5883_Custom_Set_Config(Config);
    }

    return (returnBool);
}


boolean HMC5883_Custom_Measure(int16 *X, int16 *Y, int16 *Z)
{
    boolean returnBool = FALSE;
    
    uint8 Message = HMC5883_BITS_MODE_SINGLE_MODE;

    /* to store x, z, and y raw values*/
    uint8 Result[6] = { 0 };

    /* Null pointer check */
    if(0 == X || 0 == Y || 0 == Z)
    {
        (void) CFE_EVS_SendEvent(HMC5883_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "HMC5883 Read_Gyro Null Pointer");
        returnBool = FALSE;
        goto end_of_function;
    }

    /* Issue a measure command */
    returnBool = HMC5883_Custom_Send(HMC5883_I2C_REG_MODE, 
            Message);
    if(FALSE == returnBool)
    {
        /* If send was unsuccessful return FALSE */
        goto end_of_function;
    }
    /* wait for it to complete */
    usleep(HMC5883_CONVERSION_INTERVAL_US);
    
    /* Read the x, z, and y values from the device */
    returnBool = HMC5883_Custom_Receive(HMC5883_I2C_REG_DATA_OUT_X_MSB, 
            Result, sizeof(Result));
    if(FALSE == returnBool)
    {
        /* If receive was unsuccessful return FALSE */
        goto end_of_function;
    }

    *X = (((int16)Result[0]) << 8) + Result[1];
    *Z = (((int16)Result[2]) << 8) + Result[3];
    *Y = (((int16)Result[4]) << 8) + Result[5];

end_of_function:

    return (returnBool);
}


boolean HMC5883_Custom_Measure_Temp(int16 *Temp)
{
    boolean returnBool = FALSE;
    
    /* to store temperature raw values*/
    uint8 Result[2] = { 0 };
    
    /* Null pointer check */
    if(0 == Temp)
    {
        (void) CFE_EVS_SendEvent(HMC5883_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "HMC5883 Read Temp Null Pointer");
        returnBool = FALSE;
        goto end_of_function;
    }
    /* Read the device temperature value */
    returnBool = HMC5883_Custom_Receive(HMC5983_REG_TEMP_OUT_MSB, 
            Result, sizeof(Result));
    if(FALSE == returnBool)
    {
        /* If receive was unsuccessful return FALSE */
        goto end_of_function;
    }

    *Temp = (((int16)Result[0]) << 8) + Result[1];

end_of_function:

    return (returnBool);
}


boolean HMC5883_Custom_ValidateID(void)
{
    boolean returnBool = FALSE;
    
    uint8 Result[1] = { 0 };
    
    returnBool = HMC5883_Custom_Receive(HMC5883_REG_ID_A, &Result[0], 1);
    if(FALSE == returnBool)
    {
        /* If receive was unsuccessful return FALSE */
        goto end_of_function;
    }
    if (HMC5883_ID_A != Result[0])
    {
        /* If the ID doesn't match return FALSE */
        returnBool = FALSE;
        goto end_of_function;
    }
    returnBool = HMC5883_Custom_Receive(HMC5883_REG_ID_B, &Result[0], 1);
    if(FALSE == returnBool)
    {
        /* If receive was unsuccessful return FALSE */
        goto end_of_function;
    }
    if (HMC5883_ID_B != Result[0])
    {
        /* If the ID doesn't match return FALSE */
        returnBool = FALSE;
        goto end_of_function;
    }
    returnBool = HMC5883_Custom_Receive(HMC5883_REG_ID_C, &Result[0], 1);
    if(FALSE == returnBool)
    {
        /* If receive was unsuccessful return FALSE */
        goto end_of_function;
    }
    if (HMC5883_ID_C != Result[0])
    {
        /* If the ID doesn't match return FALSE */
        returnBool = FALSE;
        goto end_of_function;
    }

end_of_function:

    return (returnBool);
}


boolean HMC5883_Custom_Send(uint8 Reg, uint8 Data)
{
    int returnCode      = 0;
    boolean returnBool  = FALSE;
    struct i2c_msg Messages[1];
    struct i2c_rdwr_ioctl_data Packets;
    uint8 buf[2];

    buf[0] = Reg;
    buf[1] = Data;

    Messages[0].addr  = HMC5883_I2C_SLAVE_ADDRESS;
    Messages[0].flags = 0;
    Messages[0].buf   = &buf[0];
    Messages[0].len   = 2;

    Packets.msgs  = Messages;
    Packets.nmsgs = 1;

    CFE_ES_PerfLogEntry(HMC5883_SEND_PERF_ID);
    returnCode = HMC5883_Ioctl(HMC5883_AppCustomData.DeviceFd, I2C_RDWR, &Packets);
    CFE_ES_PerfLogExit(HMC5883_SEND_PERF_ID);
    
    if (-1 == returnCode) 
    {            
        (void) CFE_EVS_SendEvent(HMC5883_DEVICE_ERR_EID, CFE_EVS_ERROR,
                        "HMC5883 ioctl returned errno %i", errno);
    }
    else
    {
        returnBool = TRUE;
    }

    return (returnBool);
}


boolean HMC5883_Custom_Receive(uint8 Reg, uint8 *Buffer, size_t Length)
{
    int returnCode       = 0;
    boolean returnBool   = FALSE;
    uint8 cmd            = Reg;

    struct i2c_rdwr_ioctl_data Packets;
    struct i2c_msg Messages[2];

    /* send */
    Messages[0].addr  = HMC5883_I2C_SLAVE_ADDRESS;
    Messages[0].flags = 0;
    Messages[0].buf   = &cmd;
    Messages[0].len   = 1;

    /* receive */
    Messages[1].addr  = HMC5883_I2C_SLAVE_ADDRESS;
    Messages[1].flags = HMC5883_I2C_M_READ;
    Messages[1].buf   = Buffer;
    Messages[1].len   = Length;

    Packets.msgs  = Messages;
    Packets.nmsgs = 2;

    CFE_ES_PerfLogEntry(HMC5883_RECEIVE_PERF_ID);
    returnCode = HMC5883_Ioctl(HMC5883_AppCustomData.DeviceFd, I2C_RDWR, &Packets);
    CFE_ES_PerfLogExit(HMC5883_RECEIVE_PERF_ID);

    if (-1 == returnCode) 
    {            
        (void) CFE_EVS_SendEvent(HMC5883_DEVICE_ERR_EID, CFE_EVS_ERROR,
                        "HMC5883 ioctl returned errno %i", errno);
    }
    else
    {
        returnBool = TRUE;
    }

    return (returnBool);
}



boolean HMC5883_Custom_Uninit(void)
{
    boolean returnBool = TRUE;
    int returnCode     = 0;

    returnCode = close(HMC5883_AppCustomData.DeviceFd);
    if (-1 == returnCode) 
    {
        (void) CFE_EVS_SendEvent(HMC5883_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "HMC5883 Device close errno: %i", errno);
        returnBool = FALSE;
    }
    else
    {
        HMC5883_AppCustomData.Status = HMC5883_CUSTOM_UNINITIALIZED;
    }
    return (returnBool);
}


boolean HMC5883_Custom_Max_Events_Not_Reached(int32 ind)
{
    boolean returnBool = FALSE;

    if ((ind < CFE_EVS_MAX_EVENT_FILTERS) && (ind > 0))
    {
        returnBool = TRUE;
    }

    return (returnBool);
}


int32 HMC5883_Custom_Init_EventFilters(int32 ind, CFE_EVS_BinFilter_t *EventTbl)
{
    int32 customEventCount = ind;
    
    /* Null check */
    if(0 == EventTbl)
    {
        customEventCount = -1;
        goto end_of_function;
    }

    if(TRUE == HMC5883_Custom_Max_Events_Not_Reached(customEventCount))
    {
        EventTbl[  customEventCount].EventID = HMC5883_DEVICE_ERR_EID;
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


boolean HMC5883_Apply_Platform_Rotation(float *X, float *Y, float *Z)
{
    boolean returnBool = TRUE;
    int16 temp         = 0;

    /* Null pointer check */
    if(0 == X || 0 == Y || 0 == Z)
    {
        (void) CFE_EVS_SendEvent(HMC5883_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "HMC5883 Apply_Platform_Rotation Null Pointer");
        returnBool = FALSE;
        goto end_of_function;
    }
    
    /* The standard external mag by 3DR has x pointing to the
     * right, y pointing backwards, and z down, therefore switch x
     * and y and invert y. */
    temp = *X;
    *X = -*Y;
    *Y = temp;

end_of_function:

    return (returnBool);
}


void HMC5883_Get_Rotation(uint8 *Rotation)
{
    /* Null pointer check */
    if(0 == Rotation)
    {
        goto end_of_function;
    }

    *Rotation = ROTATION_YAW_90;

end_of_function:
    return;
}
