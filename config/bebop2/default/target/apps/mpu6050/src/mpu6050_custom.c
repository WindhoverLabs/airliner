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
#include "mpu6050_i2c.h"
#include "mpu6050_events.h"
#include "mpu6050_perfids.h"
#include "px4lib.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <stdint.h>

/************************************************************************
** Local Defines
*************************************************************************/
#define MPU6050_ENABLE_FIFO_QUEUE

/************************************************************************
** Local Structure Declarations
*************************************************************************/
typedef enum {

/** \brief <tt> 'MPU6050 - ' </tt>
**  \event <tt> 'MPU6050 - ' </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when a device resource encounters an 
**  error.
**
*/
    MPU6050_DEVICE_ERR_EID = MPU6050_EVT_CNT,

/** \brief <tt> 'MPU6050 - ' </tt>
**  \event <tt> 'MPU6050 - ' </tt>
**  
**  \par Type: Info
**
**  \par Cause:
**
**  This event message is issued when a device successfully complete a
**  self test.
**
*/
    MPU6050_DEVICE_INF_EID,
/** \brief Number of custom events 
**
**  \par Limits:
**       int32
*/
    MPU6050_CUSTOM_EVT_CNT
} MPU6050_CustomEventIds_t;


/************************************************************************
** External Global Variables
*************************************************************************/

/************************************************************************
** Global Variables
*************************************************************************/
MPU6050_AppCustomData_t MPU6050_AppCustomData;

/************************************************************************
** Local Variables
*************************************************************************/

/************************************************************************
** Local Function Definitions
*************************************************************************/
int32 MPU6050_Ioctl(int fh, int request, void *arg)
{
    int32 returnCode = 0;
    uint32 i = 0;

    for (i=0; i < MPU6050_MAX_RETRY_ATTEMPTS; i++)
    {
        returnCode = ioctl(fh, request, arg);
            
        if (-1 == returnCode && EINTR == errno)
        {
            usleep(MPU6050_MAX_RETRY_SLEEP_USEC);
        }
        else
        {
            break;
        }
    }

    return returnCode;
}


void MPU6050_Custom_InitData(void)
{
    /* Set all struct zero values */
    bzero(&MPU6050_AppCustomData, sizeof(MPU6050_AppCustomData));
    MPU6050_AppCustomData.TempInitialized = FALSE;
}


boolean MPU6050_Custom_Init()
{
    boolean returnBool = TRUE;
    int32 returnCode   = 0;

    MPU6050_AppCustomData.DeviceFd = open(MPU6050_I2C_DEVICE_PATH, O_RDWR);
    if (MPU6050_AppCustomData.DeviceFd < 0) 
    {
        (void) CFE_EVS_SendEvent(MPU6050_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "MPU6050 Device open errno: %i", errno);
        returnBool = FALSE;
        goto end_of_function;
    }

    returnCode = MPU6050_ResetDevice();
    if (-1 == returnCode)
    {
        CFE_EVS_SendEvent(MPU6050_DEVICE_ERR_EID, CFE_EVS_ERROR,
                          "Reset device failed.");
        returnBool = FALSE;
        goto end_of_function;
    }

    /* TODO Add Gyroscope Self-Test */
    /* TODO Add Accelerometer Self-Test */

    /* Set clock source. */
    returnBool = MPU6050_WriteReg(MPU6050_REG_PWR_MGMT_1, MPU6050_MPU_CLK_SEL_PLLGYROX);
    if(FALSE == returnBool)
    {
        CFE_EVS_SendEvent(MPU6050_DEVICE_ERR_EID, CFE_EVS_ERROR,
                "Set clock source failed.");
        goto end_of_function;
    }
    usleep(1000);

    /* Clear any standby modes. */
    returnBool = MPU6050_WriteReg(MPU6050_REG_PWR_MGMT_2, 0x00);
    if(FALSE == returnBool)
    {
        CFE_EVS_SendEvent(MPU6050_DEVICE_ERR_EID, CFE_EVS_ERROR,
                "Enable Acc and Gyro failed");
        goto end_of_function;
    }
    usleep(1000);

#ifdef MPU6050_ENABLE_FIFO_QUEUE
    /* Enable FIFO queue. */
    returnBool = MPU6050_WriteReg(MPU6050_REG_USER_CTRL, 
            (MPU6050_BIT_USER_CTRL_FIFO_RST | MPU6050_BIT_USER_CTRL_FIFO_EN));
    if(FALSE == returnBool)
    {
        CFE_EVS_SendEvent(MPU6050_DEVICE_ERR_EID, CFE_EVS_ERROR,
                "Enable FIFO queue failed.");
        goto end_of_function;
    }
    usleep(1000);

    returnBool = MPU6050_WriteReg(MPU6050_REG_FIFO_EN,
            (MPU6050_BIT_FIFO_ENABLE_TEMP_OUT | 
            MPU6050_BIT_FIFO_ENABLE_GYRO_XOUT |
            MPU6050_BIT_FIFO_ENABLE_GYRO_YOUT |
            MPU6050_BIT_FIFO_ENABLE_GYRO_ZOUT |
            MPU6050_BIT_FIFO_ENABLE_ACCEL));

    if(FALSE == returnBool)
    {
        CFE_EVS_SendEvent(MPU6050_DEVICE_ERR_EID, CFE_EVS_ERROR,
                "Enable FIFO queue bits failed.");
        goto end_of_function;
    }

    usleep(1000);
#endif

    /* Set DLPF.  */
    returnBool = MPU6050_WriteReg(MPU6050_REG_CONFIG, MPU6050_DEFAULT_LOWPASS_FILTER);
    if(FALSE == returnBool)
    {
        CFE_EVS_SendEvent(MPU6050_DEVICE_ERR_EID, CFE_EVS_ERROR,
                "Set DLPF failed. ");
        goto end_of_function;
    }
    usleep(1000);

    /* Set sample divider.  
     * sample rate = Gyroscope_Output_Rate / (1 + sample_divider)
     * If DLPF is disabled (0 or 7) Gyroscope_Output_Rate = 8 kHz
     * otherwise Gyroscope_Output_Rate = 1kHz.
     * 250Hz = 1kHz / (1 + 3) */
    returnBool = MPU6050_WriteReg(MPU6050_REG_SMPLRT_DIV, 0x00);
    if(FALSE == returnBool)
    {
        CFE_EVS_SendEvent(MPU6050_DEVICE_ERR_EID, CFE_EVS_ERROR,
                "Set accel resolution failed. ");
        goto end_of_function;
    }
    usleep(1000);

    /* Set gyro resolution. */
    returnBool = MPU6050_WriteReg(MPU6050_REG_GYRO_CONFIG, MPU6050_BITS_FS_2000DPS);
    if(FALSE == returnBool)
    {
        CFE_EVS_SendEvent(MPU6050_DEVICE_ERR_EID, CFE_EVS_ERROR,
                "Set gyro resolution failed. ");
        goto end_of_function;
    }
    usleep(1000);

    /* Set accel resolution. */
    returnBool = MPU6050_WriteReg(MPU6050_REG_ACCEL_CONFIG, MPU6050_BITS_FS_16G);
    if(FALSE == returnBool)
    {
        CFE_EVS_SendEvent(MPU6050_DEVICE_ERR_EID, CFE_EVS_ERROR,
                "Set accel resolution failed. ");
        goto end_of_function;
    }

    usleep(1000);

    MPU6050_AppCustomData.Status = MPU6050_CUSTOM_INITIALIZED;

end_of_function:
    return returnBool;
}


boolean MPU6050_Custom_Uninit(void)
{
    boolean returnBool = TRUE;
    int returnCode = 0;

    (void) MPU6050_ResetDevice();

    returnCode = close(MPU6050_AppCustomData.DeviceFd);
    if (-1 == returnCode) 
    {
        returnBool = FALSE;
    }
    else
    {
        MPU6050_AppCustomData.Status = MPU6050_CUSTOM_UNINITIALIZED;
    }
    return returnBool;
}


int32 MPU6050_ResetDevice(void)
{
    int32 ret = 0;
    boolean returnBool = TRUE;

    returnBool = MPU6050_WriteReg(MPU6050_REG_PWR_MGMT_1, MPU6050_BIT_H_RESET);
    if (FALSE == returnBool) 
    {            
        CFE_EVS_SendEvent(MPU6050_DEVICE_ERR_EID, CFE_EVS_ERROR,
                        "MPU6050 ioctl returned %i", errno);
        ret = -1;
    }
    else
    {
        usleep(100000);
    }
    return ret;
}


boolean MPU6050_WriteReg(uint8 Addr, uint8 Data)
{
    int returnCode      = 0;
    boolean returnBool  = FALSE;
    struct i2c_msg Messages[1];
    struct i2c_rdwr_ioctl_data Packets;
    uint8 buf[2];

    buf[0] = Addr;
    buf[1] = Data;

    Messages[0].addr  = MPU6050_I2C_SLAVE_ADDRESS;
    Messages[0].flags = 0;
    Messages[0].buf   = &buf[0];
    Messages[0].len   = 2;

    Packets.msgs  = Messages;
    Packets.nmsgs = 1;

    CFE_ES_PerfLogEntry(MPU6050_SEND_PERF_ID);
    returnCode = MPU6050_Ioctl(MPU6050_AppCustomData.DeviceFd, I2C_RDWR, &Packets);
    CFE_ES_PerfLogExit(MPU6050_SEND_PERF_ID);
    
    if (-1 == returnCode) 
    {            
        (void) CFE_EVS_SendEvent(MPU6050_DEVICE_ERR_EID, CFE_EVS_ERROR,
                        "MPU6050 ioctl returned errno %i", errno);
    }
    else
    {
        returnBool = TRUE;
    }

    return (returnBool);
}


boolean MPU6050_ReadReg(uint8 Reg, void *Buffer, size_t Length)
{
    int returnCode       = 0;
    boolean returnBool   = FALSE;
    uint8 cmd            = Reg;

    struct i2c_rdwr_ioctl_data Packets;
    struct i2c_msg Messages[2];

    /* send */
    Messages[0].addr  = MPU6050_I2C_SLAVE_ADDRESS;
    Messages[0].flags = 0;
    Messages[0].buf   = &cmd;
    Messages[0].len   = 1;

    /* receive */
    Messages[1].addr  = MPU6050_I2C_SLAVE_ADDRESS;
    Messages[1].flags = MPU6050_I2C_M_READ;
    Messages[1].buf   = Buffer;
    Messages[1].len   = Length;

    Packets.msgs  = Messages;
    Packets.nmsgs = 2;

    CFE_ES_PerfLogEntry(MPU6050_RECEIVE_PERF_ID);
    returnCode = MPU6050_Ioctl(MPU6050_AppCustomData.DeviceFd, I2C_RDWR, &Packets);
    CFE_ES_PerfLogExit(MPU6050_RECEIVE_PERF_ID);

    if (-1 == returnCode) 
    {            
        (void) CFE_EVS_SendEvent(MPU6050_DEVICE_ERR_EID, CFE_EVS_ERROR,
                        "MPU6050 ioctl returned errno %i", errno);
    }
    else
    {
        returnBool = TRUE;
    }

    return (returnBool);
}


boolean MPU6050_SetAccScale(uint8 Scale, float *AccDivider)
{
    boolean returnBool = TRUE;
    uint8 value = 0;

    /* Null pointer check */
    if(0 == AccDivider)
    {
        CFE_EVS_SendEvent(MPU6050_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "MPU6050 SetAccScale Null Pointer");
        returnBool = FALSE;
        goto end_of_function;
    }

    switch (Scale)
    {
        case 2:
            *AccDivider = 16384;
            value = MPU6050_BITS_FS_2G;
            break;

        case 4:
            *AccDivider = 8192;
            value = MPU6050_BITS_FS_4G;
            break;

        case 8:
            *AccDivider = 4096;
            value = MPU6050_BITS_FS_8G;
            break;

        case 16:
            *AccDivider = 2048;
            value = MPU6050_BITS_FS_16G;
            break;

        default:
            returnBool = FALSE;
            goto end_of_function;
    }

    returnBool = MPU6050_WriteReg(MPU6050_REG_ACCEL_CONFIG, value);
    if (FALSE == returnBool)
    {
        goto end_of_function;
    }

end_of_function:
    return returnBool;
}


boolean MPU6050_SetGyroScale(uint32 Scale, float *GyroDivider)
{
    boolean returnBool = TRUE;
    uint8 value = 0;

    /* Null pointer check */
    if(0 == GyroDivider)
    {
        CFE_EVS_SendEvent(MPU6050_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "MPU6050 SetGyroScale Null Pointer");
        returnBool = FALSE;
        goto end_of_function;
    }

    switch (Scale)
    {
        case 250:
            *GyroDivider = 131;
            value = MPU6050_BITS_FS_250DPS;
            break;

        case 500:
            *GyroDivider = 65.5;
            value = MPU6050_BITS_FS_500DPS;
            break;

        case 1000:
            *GyroDivider = 32.8;
            value = MPU6050_BITS_FS_1000DPS;
            break;

        case 2000:
            *GyroDivider = 16.4;
            value = MPU6050_BITS_FS_2000DPS;
            break;

        default:
            returnBool = FALSE;
            goto end_of_function;
    }

    returnBool = MPU6050_WriteReg(MPU6050_REG_GYRO_CONFIG, value);
    if (FALSE == returnBool)
    {
        goto end_of_function;
    }

end_of_function:
    return returnBool;
}


boolean MPU6050_Measure(MPU6050_SampleQueue_t *SampleQueue)
{
    uint8 intStatus = 0;
    boolean returnBool = TRUE;
    uint16 fifoByteCount = 0;
    uint16 index = 0;

    /* Null pointer check */
    if(0 == SampleQueue)
    {
        CFE_EVS_SendEvent(MPU6050_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "MPU6050 Measure Null Pointer");
        returnBool = FALSE;
        goto end_of_function;
    }

    /* Check data ready interrupt bit. */
    returnBool = MPU6050_ReadReg(MPU6050_REG_INT_STATUS, &intStatus, 1);
    if(FALSE == returnBool)
    {
        CFE_EVS_SendEvent(MPU6050_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "MPU6050 failed to read MPU6050_REG_INT_STATUS");
        returnBool = FALSE;
        goto end_of_function;
    }

#ifdef MPU6050_ENABLE_FIFO_QUEUE
    /* Check for overflow. */
    if(intStatus & MPU6050_BIT_INT_STATUS_FIFO_OVRFLW)
    {
        /* Reset fifo Queue. */
        MPU6050_ResetFifo();
        /* Fifo should have overflowed on the first read after SCH
         * startup sync. */
        if(MPU6050_AppCustomData.firstMeasureFlag != FALSE)
        {
            CFE_EVS_SendEvent(MPU6050_DEVICE_ERR_EID, CFE_EVS_ERROR,
                    "MPU6050 FIFO overflow.");
        }
        returnBool = FALSE;
        goto end_of_function;
    }
    /* Get the number of bytes in the FIFO queue. */
    fifoByteCount = MPU6050_GetFifoCount();

    if(fifoByteCount > 0)
    {
        /* Read the fifo queue. */
        returnBool = MPU6050_ReadReg(MPU6050_REG_FIFO_R_W, &MPU6050_AppCustomData.samples[0], fifoByteCount);
        if(FALSE == returnBool)
        {
            CFE_EVS_SendEvent(MPU6050_DEVICE_ERR_EID, CFE_EVS_ERROR,
                "MPU6050 FIFO read failed.");
            goto end_of_function;
        }
        /* Calculate filtered fifo samples per cycle. This value should
         * stay close to 4. */
        MPU6050_AppCustomData.FifoSamplesPerCycle = 
                (0.95f * MPU6050_AppCustomData.FifoSamplesPerCycle) + 
                (0.05f * (fifoByteCount / sizeof(MPU6050_Sample_t)));
    }
    else
    {
        /* Data not ready skipping measurement. */
        CFE_EVS_SendEvent(MPU6050_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "MPU6050 FIFO empty.");
        returnBool = FALSE;
        goto end_of_function;
    }

    /* Calculate the number of samples in the fifo queue. */
    SampleQueue->SampleCount = fifoByteCount / sizeof(MPU6050_Sample_t);
    /* Calculate the sampling interval in micro seconds. */
    SampleQueue->SampleIntervalUs = round(1000 / MPU6050_AppCustomData.FifoSamplesPerCycle);

    for(index = 0; index < SampleQueue->SampleCount; ++index)
    {
        //returnBool = MPU6050_sampleChecks(&sample[index]);
        //if(FALSE == returnBool)
        //{
            //goto end_of_function;
        //}
        SampleQueue->Samples[index].AX   = MPU6050_Swap16(MPU6050_AppCustomData.samples[index].val[0]);
        SampleQueue->Samples[index].AY   = MPU6050_Swap16(MPU6050_AppCustomData.samples[index].val[1]);
        SampleQueue->Samples[index].AZ   = MPU6050_Swap16(MPU6050_AppCustomData.samples[index].val[2]);
        SampleQueue->Samples[index].Temp = MPU6050_Swap16(MPU6050_AppCustomData.samples[index].val[3]);
        SampleQueue->Samples[index].GX   = MPU6050_Swap16(MPU6050_AppCustomData.samples[index].val[4]);
        SampleQueue->Samples[index].GY   = MPU6050_Swap16(MPU6050_AppCustomData.samples[index].val[5]);
        SampleQueue->Samples[index].GZ   = MPU6050_Swap16(MPU6050_AppCustomData.samples[index].val[6]);
    }

#else
    /* Check for DATA_RDY_INT set... */
    if(intStatus & MPU6050_BIT_INT_STATUS_DATA)
    {
        returnBool = MPU6050_ReadReg(MPU6050_REG_ACCEL_XOUT_H, &MPU6050_AppCustomData.samples[0], sizeof(MPU6050_Sample_t));
        if(FALSE == returnBool)
        {
            goto end_of_function;
        }

        SampleQueue->Samples[0].AX   = MPU6050_Swap16(MPU6050_AppCustomData.samples[0].val[0]);
        SampleQueue->Samples[0].AY   = MPU6050_Swap16(MPU6050_AppCustomData.samples[0].val[1]);
        SampleQueue->Samples[0].AZ   = MPU6050_Swap16(MPU6050_AppCustomData.samples[0].val[2]);
        SampleQueue->Samples[0].Temp = MPU6050_Swap16(MPU6050_AppCustomData.samples[0].val[3]);
        SampleQueue->Samples[0].GX   = MPU6050_Swap16(MPU6050_AppCustomData.samples[0].val[4]);
        SampleQueue->Samples[0].GY   = MPU6050_Swap16(MPU6050_AppCustomData.samples[0].val[5]);
        SampleQueue->Samples[0].GZ   = MPU6050_Swap16(MPU6050_AppCustomData.samples[0].val[6]);
    }
    else
    {
        /* Data not ready skipping measurement. */
        returnBool = FALSE;
        goto end_of_function;
    }
#endif


end_of_function:
    /* First read should have overflowed. */
    if (FALSE == returnBool && MPU6050_AppCustomData.firstMeasureFlag != FALSE)
    {
        CFE_EVS_SendEvent(MPU6050_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "MPU6050 read error in Measure");
    }
    MPU6050_AppCustomData.firstMeasureFlag = TRUE;
    return returnBool;
}


boolean MPU6050_Read_WhoAmI(uint8 *Value)
{
    boolean returnBool = TRUE;
    /* Null pointer check */
    if(0 == Value)
    {
        CFE_EVS_SendEvent(MPU6050_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "MPU6050 Read_WhoAmI Null Pointer");
        returnBool = FALSE;
        goto end_of_function;
    }

    returnBool = MPU6050_ReadReg(MPU6050_REG_WHOAMI, Value, 1);

end_of_function:
    if (FALSE == returnBool)
    {
        CFE_EVS_SendEvent(MPU6050_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "MPU6050 read error in WhoAmI");
    }
    return returnBool;
}


boolean MPU6050_Perform_AccelSelfTest(void)
{
    return TRUE;
}


boolean MPU6050_Perform_GyroSelfTest(void)
{
    return TRUE;
}


boolean MPU6050_Apply_Platform_Rotation(float *X, float *Y, float *Z)
{
    boolean returnBool = TRUE;
    float temp;

    /* Null pointer check */
    if(0 == X || 0 == Y || 0 == Z)
    {
        CFE_EVS_SendEvent(MPU6050_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "MPU6050 Apply_Platform_Rotation Null Pointer");
        returnBool = FALSE;
        goto end_of_function;
    }
    /* ROTATION_ROLL_180 */
    *Y = -*Y; 
    *Z = -*Z;

end_of_function:

    return returnBool;
}


boolean MPU6050_Custom_Max_Events_Not_Reached(int32 ind)
{
    if ((ind < CFE_EVS_MAX_EVENT_FILTERS) && (ind > 0))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


int32 MPU6050_Custom_Init_EventFilters(int32 ind, CFE_EVS_BinFilter_t *EventTbl)
{
    int32 customEventCount = ind;
    
    /* Null check */
    if(0 == EventTbl)
    {
        customEventCount = -1;
        goto end_of_function;
    }

    if(TRUE == MPU6050_Custom_Max_Events_Not_Reached(customEventCount))
    {
        EventTbl[  customEventCount].EventID = MPU6050_DEVICE_ERR_EID;
        EventTbl[customEventCount++].Mask    = CFE_EVS_FIRST_16_STOP;
    }
    else
    {
        customEventCount = -1;
        goto end_of_function;
    }
    
end_of_function:

    return customEventCount;
}


void MPU6050_Get_Rotation(uint8 *Rotation)
{
    
    /* Null pointer check */
    if(0 == Rotation)
    {
        CFE_EVS_SendEvent(MPU6050_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "MPU6050 Get_Rotation Null Pointer");
        goto end_of_function;
    }
    
    /* TODO move to a table */
    *Rotation = ROTATION_ROLL_180;

end_of_function:
;
}


uint16 MPU6050_GetFifoCount(void)
{
    uint16 bytes = 0;
    uint16 completeSamples = 0;
    boolean returnBool = FALSE;

    returnBool = MPU6050_ReadReg(MPU6050_REG_FIFO_COUNTH, &bytes, sizeof(bytes));
    if(FALSE == returnBool)
    {
        CFE_EVS_SendEvent(MPU6050_DEVICE_ERR_EID, CFE_EVS_ERROR,
                "Get FIFO count failed.");
        bytes = 0;
    }

    bytes = MPU6050_Swap16(bytes);
    /* Ensure a complete set of samples from all sensors. */
    completeSamples = bytes / sizeof(MPU6050_Sample_t);
    bytes = completeSamples * sizeof(MPU6050_Sample_t);

    return bytes;
}


void MPU6050_ResetFifo(void)
{
    boolean returnBool = FALSE;
    returnBool = MPU6050_WriteReg(MPU6050_REG_USER_CTRL, 
        MPU6050_BIT_USER_CTRL_FIFO_RST | MPU6050_BIT_USER_CTRL_FIFO_EN);
    if(FALSE == returnBool)
    {
        CFE_EVS_SendEvent(MPU6050_DEVICE_ERR_EID, CFE_EVS_ERROR,
                "Reset FIFO Queue failed.");
    }

    return;
}


boolean MPU6050_sampleChecks(MPU6050_Sample_t *sample)
{
    boolean returnBool = TRUE;

    if(0 == sample)
    {
        CFE_EVS_SendEvent(MPU6050_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "Null pointer in MPU6050_RangeCheck.");
        goto end_of_function;
    }

    const int16 AX    = MPU6050_Swap16(sample->val[0]);
    const int16 AY    = MPU6050_Swap16(sample->val[1]);
    const int16 AZ    = MPU6050_Swap16(sample->val[2]);
    const float TempC = (float)MPU6050_Swap16(sample->val[3]) / 340.0f + 36.53f;
    const int16 GX    = MPU6050_Swap16(sample->val[4]);
    const int16 GY    = MPU6050_Swap16(sample->val[5]);
    const int16 GZ    = MPU6050_Swap16(sample->val[6]);

    /* Check acceleration in the range for min/max. */
    if (AX == INT16_MIN  || AY == INT16_MIN  || AZ == INT16_MIN )
    {
        CFE_EVS_SendEvent(MPU6050_DEVICE_ERR_EID, CFE_EVS_ERROR,
                "Acceleration sample hit a minimum");
    }

    if (AX == INT16_MAX || AY == INT16_MAX  || AZ == INT16_MAX)
    {
        CFE_EVS_SendEvent(MPU6050_DEVICE_ERR_EID, CFE_EVS_ERROR,
                "Acceleration sample hit a maximum");
    }

    /* Check gyro in the range for min/max. */
    if (GX == INT16_MIN  || GY == INT16_MIN  || GZ == INT16_MIN )
    {
        CFE_EVS_SendEvent(MPU6050_DEVICE_ERR_EID, CFE_EVS_ERROR,
                "Gyro sample hit a minimum");
    }

    if (GX == INT16_MAX || GY == INT16_MAX  || GZ == INT16_MAX)
    {
        CFE_EVS_SendEvent(MPU6050_DEVICE_ERR_EID, CFE_EVS_ERROR,
                "Gyro sample hit a maximum");
    }

    if(FALSE == MPU6050_AppCustomData.TempInitialized)
    {
        if(TempC > -40.0f && TempC < 85.0f)
        {
            MPU6050_AppCustomData.TempInitialized = TRUE;
            CFE_EVS_SendEvent(MPU6050_DEVICE_INF_EID, CFE_EVS_INFORMATION,
                    "Temperature initialized %f.", TempC);
        }
        else
        {
            CFE_EVS_SendEvent(MPU6050_DEVICE_ERR_EID, CFE_EVS_ERROR,
                    "Fifo temperature out of range %f init failed.", TempC);
        }
    }
    else
    {
        if (fabsf(TempC - MPU6050_AppCustomData.FifoLastTemp) > 2.0f)
        {
            CFE_EVS_SendEvent(MPU6050_DEVICE_ERR_EID, CFE_EVS_ERROR,
                    "Fifo corruption detected. Reset queue.");
            MPU6050_AppCustomData.TempInitialized = FALSE;
            MPU6050_ResetFifo();
            returnBool = FALSE;
            goto end_of_function;
        }
    }

    MPU6050_AppCustomData.FifoLastTemp = TempC;
    
end_of_function:
    return returnBool;
}


uint16 MPU6050_Swap16(uint16 val) 
{
    return (val >> 8) | (val << 8); 
}
