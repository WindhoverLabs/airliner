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
#include "ak8963_i2c.h"
#include "ak8963_events.h"
#include "ak8963_perfids.h"
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

/************************************************************************
** Local Defines
*************************************************************************/

/************************************************************************
** Local Structure Declarations
*************************************************************************/
typedef enum {

/** \brief <tt> 'AK8963 - ' </tt>
**  \event <tt> 'AK8963 - ' </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when a device resource encounters an 
**  error.
**
*/
    AK8963_DEVICE_ERR_EID = AK8963_EVT_CNT,

/** \brief <tt> 'AK8963 - ' </tt>
**  \event <tt> 'AK8963 - ' </tt>
**  
**  \par Type: Info
**
**  \par Cause:
**
**  This event message is issued when a device successfully complete a
**  self test.
**
*/
    AK8963_DEVICE_INF_EID,
/** \brief Number of custom events 
**
**  \par Limits:
**       int32
*/
    AK8963_CUSTOM_EVT_CNT
} AK8963_CustomEventIds_t;


/************************************************************************
** External Global Variables
*************************************************************************/

/************************************************************************
** Global Variables
*************************************************************************/
AK8963_AppCustomData_t AK8963_AppCustomData;

/************************************************************************
** Local Variables
*************************************************************************/

/************************************************************************
** Local Function Definitions
*************************************************************************/
int32 AK8963_Ioctl(int fh, int request, void *arg)
{
    int32 returnCode = 0;
    uint32 i = 0;

    for (i = 0; i < AK8963_MAX_RETRY_ATTEMPTS; ++i)
    {
        returnCode = ioctl(fh, request, arg);
            
        if (-1 == returnCode && EINTR == errno)
        {
            usleep(AK8963_MAX_RETRY_SLEEP_USEC);
        }
        else
        {
            break;
        }
    }

    return returnCode;
}


void AK8963_Custom_InitData(void)
{
    /* Set all struct zero values */
    bzero(&AK8963_AppCustomData, sizeof(AK8963_AppCustomData));
}


boolean AK8963_Custom_Init()
{
    boolean returnBool = TRUE;

    AK8963_AppCustomData.DeviceFd = open(AK8963_I2C_DEVICE_PATH, O_RDWR);
    if (AK8963_AppCustomData.DeviceFd < 0) 
    {
        (void) CFE_EVS_SendEvent(AK8963_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "AK8963 Device open errno: %i", errno);
        returnBool = FALSE;
        goto end_of_function;
    }

    AK8963_AppCustomData.Status = AK8963_CUSTOM_INITIALIZED;

end_of_function:
    return (returnBool);
}


boolean AK8963_Custom_Uninit(void)
{
    boolean returnBool = TRUE;
    int returnCode = 0;

    returnCode = close(AK8963_AppCustomData.DeviceFd);
    if (-1 == returnCode) 
    {
        (void) CFE_EVS_SendEvent(AK8963_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "AK8963 Device close errno: %i", errno);
        returnBool = FALSE;
    }
    else
    {
        AK8963_AppCustomData.Status = AK8963_CUSTOM_UNINITIALIZED;
    }
    return (returnBool);
}


boolean AK8963_ResetDevice(void)
{
    boolean returnBool = TRUE;
    
    returnBool = AK8963_WriteReg(AK8963_REG_CNTL2, AK8963_BITS_CNTL2_SOFT_RESET);
    if (FALSE == returnBool) 
    {            
        (void) CFE_EVS_SendEvent(AK8963_DEVICE_ERR_EID, CFE_EVS_ERROR,
                        "AK8963 ioctl returned %i", errno);
    }
    else
    {
        usleep(1000);
    }
    return (returnBool);
}


boolean AK8963_WriteReg(uint8 Addr, uint8 Data)
{
    int returnCode      = 0;
    boolean returnBool  = FALSE;
    struct i2c_msg Messages[1];
    struct i2c_rdwr_ioctl_data Packets;
    uint8 buf[2];

    buf[0] = Addr;
    buf[1] = Data;

    Messages[0].addr  = AK8963_I2C_SLAVE_ADDRESS;
    Messages[0].flags = 0;
    Messages[0].buf   = &buf[0];
    Messages[0].len   = 2;

    Packets.msgs  = Messages;
    Packets.nmsgs = 1;

    CFE_ES_PerfLogEntry(AK8963_SEND_PERF_ID);
    returnCode = AK8963_Ioctl(AK8963_AppCustomData.DeviceFd, I2C_RDWR, &Packets);
    CFE_ES_PerfLogExit(AK8963_SEND_PERF_ID);
    
    if (-1 == returnCode) 
    {            
        (void) CFE_EVS_SendEvent(AK8963_DEVICE_ERR_EID, CFE_EVS_ERROR,
                        "AK8963 ioctl returned errno %i", errno);
    }
    else
    {
        returnBool = TRUE;
    }

    return (returnBool);
}


boolean AK8963_ReadReg(uint8 Reg, void *Buffer, size_t Length)
{
    int returnCode       = 0;
    boolean returnBool   = FALSE;
    uint8 cmd            = Reg;

    struct i2c_rdwr_ioctl_data Packets;
    struct i2c_msg Messages[2];

    /* send */
    Messages[0].addr  = AK8963_I2C_SLAVE_ADDRESS;
    Messages[0].flags = 0;
    Messages[0].buf   = &cmd;
    Messages[0].len   = 1;

    /* receive */
    Messages[1].addr  = AK8963_I2C_SLAVE_ADDRESS;
    Messages[1].flags = AK8963_I2C_M_READ;
    Messages[1].buf   = Buffer;
    Messages[1].len   = Length;

    Packets.msgs  = Messages;
    Packets.nmsgs = 2;

    CFE_ES_PerfLogEntry(AK8963_RECEIVE_PERF_ID);
    returnCode = AK8963_Ioctl(AK8963_AppCustomData.DeviceFd, I2C_RDWR, &Packets);
    CFE_ES_PerfLogExit(AK8963_RECEIVE_PERF_ID);

    if (-1 == returnCode) 
    {            
        (void) CFE_EVS_SendEvent(AK8963_DEVICE_ERR_EID, CFE_EVS_ERROR,
                        "AK8963 ioctl returned errno %i", errno);
    }
    else
    {
        returnBool = TRUE;
    }

    return (returnBool);
}


boolean AK8963_Read_Mag(int16 *X, int16 *Y, int16 *Z)
{
    uint8 bits   = 0;
    boolean returnBool = TRUE;
    AK8963_Sample_t sample = {0};

    /* Null pointer check */
    if(0 == X || 0 == Y || 0 == Z)
    {
        CFE_EVS_SendEvent(AK8963_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "AK8963 Read_Mag Null Pointer");
        returnBool = FALSE;
        goto end_of_function;
    }

    /* Read status */
    returnBool = AK8963_ReadReg(AK8963_REG_ST1, &bits, 1);
    if(FALSE == returnBool)
    {
        goto end_of_function;
    }

    /* If the data ready bit is set... */
    if (bits & AK8963_BITS_ST1_DRDY)
    {
        returnBool = AK8963_ReadReg(AK8963_REG_HXL, &sample, sizeof(sample));
        if(FALSE == returnBool)
        {
            goto end_of_function;
        }
    }
    else
    {
        /* Data not ready skipping measurement. */
        returnBool = FALSE;
        goto end_of_function;
    }
    
    /* Check for overflow... */
    if (sample.st2 & AK8963_BITS_ST2_HOFL)
    {
        returnBool = FALSE;
        goto end_of_function;
    }

    /* Set output */
    *X = sample.val[0];
    *Y = sample.val[1];
    *Z = sample.val[2];

end_of_function:

    return (returnBool);
}


boolean AK8963_Read_MagDeviceID(uint8 *Value)
{
    boolean returnBool = TRUE;
    /* Null pointer check */
    if(0 == Value)
    {
        CFE_EVS_SendEvent(AK8963_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "AK8963 Read_MagDeviceID Null Pointer");
        returnBool = FALSE;
        goto end_of_function;
    }

    returnBool = AK8963_ReadReg(AK8963_REG_WIA, Value, 1);

end_of_function:
    if (FALSE == returnBool)
    {
        CFE_EVS_SendEvent(AK8963_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "AK8963 read error in MagDeviceID");
    }
    return (returnBool);
}


//boolean AK8963_Read_MagInfo(uint8 *Value)
//{
    //boolean returnBool = TRUE;
    ///* Null pointer check */
    //if(0 == Value)
    //{
        //CFE_EVS_SendEvent(AK8963_DEVICE_ERR_EID, CFE_EVS_ERROR,
            //"AK8963 Read_MagInfo Null Pointer");
        //returnBool = FALSE;
        //goto end_of_function;
    //}
    //*Value = AK8963_ReadReg(AK8963_REG_EXT_SENS_DATA_00 + AK8963_AK8963_INFO, Value);

//end_of_function:
    //if (FALSE == returnBool)
    //{
        //CFE_EVS_SendEvent(AK8963_DEVICE_ERR_EID, CFE_EVS_ERROR,
            //"AK8963 read error in MagInfo");
    //}
    //return returnBool;
//}


//boolean AK8963_Read_MagStatus(boolean *Overrun, boolean *DataReady, boolean *Overflow, boolean *Output16Bit)
//{
    //uint8 value = 0;
    //boolean returnBool = TRUE;
    
    ///* Null pointer check */
    //if(0 == Overrun || 0 == DataReady || 0 == Overflow || 0 == Output16Bit)
    //{
        //CFE_EVS_SendEvent(AK8963_DEVICE_ERR_EID, CFE_EVS_ERROR,
            //"AK8963 Read_MagStatus Null Pointer");
        //returnBool = FALSE;
        //goto end_of_function;
    //}

    //returnBool = AK8963_ReadReg(AK8963_REG_EXT_SENS_DATA_00 + AK8963_AK8963_ST1, &value);
    //if(FALSE == returnBool)
    //{
        //returnBool = FALSE;
        //goto end_of_function;
    //}
    //*Overrun = value & AK8963_ST1_DOR_MASK;
    //*DataReady = value & AK8963_ST1_DRDY_MASK;

    //returnBool = AK8963_ReadReg(AK8963_REG_EXT_SENS_DATA_00 + AK8963_AK8963_ST2, &value);
    //if(FALSE == returnBool)
    //{
        //returnBool = FALSE;
        //goto end_of_function;
    //}
    //*Overflow = value & AK8963_ST2_HOFL_MASK;
    //*Output16Bit = value & AK8963_ST2_BITM_MASK;

//end_of_function:
    //if (FALSE == returnBool)
    //{
        //CFE_EVS_SendEvent(AK8963_DEVICE_ERR_EID, CFE_EVS_ERROR,
            //"AK8963 read error in MagStatus");
    //}
    //return returnBool;
//}


boolean AK8963_Read_MagAdj(uint8 *X, uint8 *Y, uint8 *Z)
{
    boolean returnBool = TRUE;
    uint8 MagAdjXYZ[3] = {0};

    /* Null pointer check */
    if(0 == X || 0 == Y || 0 == Z)
    {
        CFE_EVS_SendEvent(AK8963_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "AK8963 Read_MagAdj Null Pointer");
        returnBool = FALSE;
        goto end_of_function;
    }
    
    /* Set power-down mode */
    returnBool = AK8963_WriteReg(AK8963_REG_CNTL1, AK8963_BITS_CNTL1_MODE_POWER_DOWN);
    if(FALSE == returnBool)
    {
        CFE_EVS_SendEvent(AK8963_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "AK8963 set power down mode failed");
        goto end_of_function;
    }

    usleep(10000);

    /* Enable fuse ROM access mode */
    returnBool = AK8963_WriteReg(AK8963_REG_CNTL1, AK8963_BITS_CNTL1_MODE_ROM_ACCESS);
    if(FALSE == returnBool)
    {
        CFE_EVS_SendEvent(AK8963_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "AK8963 set fuse ROM access mode failed");
        goto end_of_function;
    }

    usleep(10000);

    /* Read sensitivity adjustment values */
    returnBool = AK8963_ReadReg(AK8963_REG_ASAX, &MagAdjXYZ[0], sizeof(MagAdjXYZ));
    if(FALSE == returnBool)
    {
        CFE_EVS_SendEvent(AK8963_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "AK8963 read sensitivity adjustment values failed");
        goto end_of_function;
    }

    *X = MagAdjXYZ[0];
    *Y = MagAdjXYZ[1];
    *Z = MagAdjXYZ[2];

    /* Store in custom data for self-test. */
    AK8963_AppCustomData.MagAdjX = (MagAdjXYZ[0] - 128.0f) / 256.0f + 1.0f;
    AK8963_AppCustomData.MagAdjY = (MagAdjXYZ[1] - 128.0f) / 256.0f + 1.0f;
    AK8963_AppCustomData.MagAdjZ = (MagAdjXYZ[2] - 128.0f) / 256.0f + 1.0f;
    
    printf("AK8963_AppCustomData.MagAdjX %f\n", AK8963_AppCustomData.MagAdjX);
    printf("AK8963_AppCustomData.MagAdjY %f\n", AK8963_AppCustomData.MagAdjY);
    printf("AK8963_AppCustomData.MagAdjZ %f\n", AK8963_AppCustomData.MagAdjZ);

    /* Return to power-down mode */
    returnBool = AK8963_WriteReg(AK8963_REG_CNTL1, AK8963_BITS_CNTL1_MODE_POWER_DOWN);
    if(FALSE == returnBool)
    {
        CFE_EVS_SendEvent(AK8963_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "AK8963 return to power-down mode failed");
        goto end_of_function;
    }

    usleep(10000);

end_of_function:

    return returnBool;
}


boolean AK8963_PowerOn(void)
{
    boolean returnBool = FALSE;
    uint8 bits = AK8963_BITS_CNTL1_OUTPUT_16BIT | AK8963_BITS_CNTL1_MODE_CONTINOUS2;

    returnBool = AK8963_WriteReg(AK8963_REG_CNTL1, bits);
    if(FALSE == returnBool)
    {
        CFE_EVS_SendEvent(AK8963_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "AK8963 set read mode failed");
    }
    else
    {
        usleep(1000);
    }

    return (returnBool);
}


boolean AK8963_Apply_Platform_Rotation(float *X, float *Y, float *Z)
{
    boolean returnBool = TRUE;
    float tempX = 0.0f;
    float tempY = 0.0f;
    float tempZ = 0.0f;

    /* Null pointer check */
    if(0 == X || 0 == Y || 0 == Z)
    {
        CFE_EVS_SendEvent(AK8963_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "AK8963 Apply_Platform_Rotation Null Pointer");
        returnBool = FALSE;
        goto end_of_function;
    }
    /* ROTATION_PITCH_9_YAW_180 */
    tempX = *X;
    tempY = *Y;
    tempZ = *Z;

    *X = -0.987688f * tempX +  0.000000f * tempY + -0.156434f * tempZ;
    *Y =  0.000000f * tempX + -1.000000f * tempY +  0.000000f * tempZ;
    *Z = -0.156434f * tempX +  0.000000f * tempY +  0.987688f * tempZ;

end_of_function:

    return returnBool;
}


boolean AK8963_Custom_Max_Events_Not_Reached(int32 ind)
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


int32 AK8963_Custom_Init_EventFilters(int32 ind, CFE_EVS_BinFilter_t *EventTbl)
{
    int32 customEventCount = ind;
    
    /* Null check */
    if(0 == EventTbl)
    {
        customEventCount = -1;
        goto end_of_function;
    }

    if(TRUE == AK8963_Custom_Max_Events_Not_Reached(customEventCount))
    {
        EventTbl[  customEventCount].EventID = AK8963_DEVICE_ERR_EID;
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


void AK8963_Get_Rotation(uint8 *Rotation)
{
    
    /* Null pointer check */
    if(0 == Rotation)
    {
        CFE_EVS_SendEvent(AK8963_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "AK8963 Get_Rotation Null Pointer");
        goto end_of_function;
    }

    *Rotation = ROTATION_PITCH_9_YAW_180;

end_of_function:
    return;
}


boolean AK8963_RunSelfTest(void)
{
    boolean returnBool = FALSE;
    uint8 bits = 0;
    AK8963_Sample_t sample = {0};
    uint32 i = 0;
    const float Xadj = AK8963_AppCustomData.MagAdjX;
    const float Yadj = AK8963_AppCustomData.MagAdjY;
    const float Zadj = AK8963_AppCustomData.MagAdjZ;
    uint8 MeasureCount = 0;

    /* Set power-down mode */
    returnBool = AK8963_WriteReg(AK8963_REG_CNTL1, AK8963_BITS_CNTL1_MODE_POWER_DOWN);
    if(FALSE == returnBool)
    {
        CFE_EVS_SendEvent(AK8963_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "AK8963 set power-down mode failed");
        goto end_of_function;
    }

    usleep(1000);

    /* Set self-test */
    returnBool = AK8963_WriteReg(AK8963_REG_ASTC, AK8963_BITS_ASTC_SELF_TEST);
    if(FALSE == returnBool)
    {
        CFE_EVS_SendEvent(AK8963_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "AK8963 set self-test failed");
        goto end_of_function;
    }

    usleep(1000);

    bits = AK8963_BITS_CNTL1_MODE_SELF_TEST | AK8963_BITS_CNTL1_OUTPUT_16BIT;

    /* Set self-test mode */
    returnBool = AK8963_WriteReg(AK8963_REG_CNTL1, bits);
    if(FALSE == returnBool)
    {
        CFE_EVS_SendEvent(AK8963_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "AK8963 set self-test mode failed");
        goto end_of_function;
    }

    usleep(1000);

    /* Wait for data ready. */
    for(i = 0; i < AK8963_SELFTEST_READ_STATUS_ATTEMPTS; ++i)
    {
        returnBool = AK8963_ReadReg(AK8963_REG_ST1, &bits, 1);
        if(FALSE == returnBool)
        {
            CFE_EVS_SendEvent(AK8963_DEVICE_ERR_EID, CFE_EVS_ERROR,
                "AK8963 self-test read status failed");
            goto end_of_function;
        }

        if(bits & AK8963_BITS_ST1_DRDY)
        {
            /* Throw away the first 5 measurements to allow the sensor
             * to settle after startup. */
            if(5 == MeasureCount)
            {
                break;
            }
            MeasureCount++;
            (void) AK8963_ReadReg(AK8963_REG_HXL, &sample, sizeof(sample));
        }
        
        usleep(100);
    }

    returnBool = AK8963_ReadReg(AK8963_REG_HXL, &sample, sizeof(sample));
    if(FALSE == returnBool)
    {
        goto end_of_function;
    }

    /* Check if the measurements are in reasonable range (see the data sheet) */
    if (AK8963_Custom_In_Range(sample.val[0] * Xadj, -200.0, 200.0) &&
        AK8963_Custom_In_Range(sample.val[1] * Yadj, -200.0, 200.0) &&
        AK8963_Custom_In_Range(sample.val[2] * Zadj, -3200.0, -800.0)) 
    {
        CFE_EVS_SendEvent(AK8963_DEVICE_INF_EID, CFE_EVS_INFORMATION,
            "AK8963 self-test passed.");
    }
    else
    {
        returnBool = FALSE;
        CFE_EVS_SendEvent(AK8963_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "AK8963 self-test range check failed");
        goto end_of_function;
    }

    bits = AK8963_BITS_ASTC_NORMAL;

    /* Return to normal */
    returnBool = AK8963_WriteReg(AK8963_REG_ASTC, bits);
    if(FALSE == returnBool)
    {
        CFE_EVS_SendEvent(AK8963_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "AK8963 set self-test normal failed");
        goto end_of_function;
    }

    usleep(1000);

    /* Set power-down mode */
    returnBool = AK8963_WriteReg(AK8963_REG_CNTL1, AK8963_BITS_CNTL1_MODE_POWER_DOWN);
    if(FALSE == returnBool)
    {
        CFE_EVS_SendEvent(AK8963_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "AK8963 set power-down mode failed");
        goto end_of_function;
    }

    usleep(1000);

end_of_function:

    return (returnBool);
}


boolean AK8963_Custom_In_Range(float value, float min, float max)
{
    printf("self test %f\n", value);
    return (min <= value) && (value <= max);
}
