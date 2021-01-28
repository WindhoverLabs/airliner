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
** Includes
*************************************************************************/
#include "cfe.h"
#include "amc_app.h"
#include "amc_custom_bebop.hpp"
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

/************************************************************************
** Local Defines
*************************************************************************/
AMC_AppCustomData_t AMC_AppCustomData;

/************************************************************************
** Local Structure Declarations
*************************************************************************/
typedef enum {

/** \brief <tt> 'AMC - ' </tt>
**  \event <tt> 'AMC - ' </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when a device resource encounters an 
**  error.
**
*/
    AMC_DEVICE_ERR_EID = AMC_EVT_CNT,

/** \brief <tt> 'AMC - ' </tt>
**  \event <tt> 'AMC - ' </tt>
**  
**  \par Type: Info
**
**  \par Cause:
**
**  This event message is issued when a device successfully complete a
**  self test.
**
*/
    AMC_DEVICE_INF_EID,
/** \brief Number of custom events 
**
**  \par Limits:
**       int32
*/
    AMC_CUSTOM_EVT_CNT
} AMC_CustomEventIds_t;


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* AMC::InitDevice function.                                       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 AMC::InitDevice(void)
{
    int32 returnValue = CFE_SUCCESS;
    boolean returnBool = FALSE;
    AMC_BLDC_Info_t info = {0};

    AMC_AppCustomData.DeviceFd = open(AMC_BLDC_I2C_DEVICE_PATH, O_RDWR);
    if (AMC_AppCustomData.DeviceFd < 0) 
    {
        (void) CFE_EVS_SendEvent(AMC_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "AMC Device open errno: %i", errno);
        returnValue = -1;
        goto end_of_function;
    }

    returnBool = AMC_Clear_Errors();
    if (FALSE == returnBool)
    {
        returnValue = -1;
        goto end_of_function;
    }

    returnBool = AMC_Custom_Get_Info(&info);
    if(TRUE == returnBool)
    {
        (void) CFE_EVS_SendEvent(AMC_DEVICE_INF_EID, CFE_EVS_INFORMATION,
            "AMC Bebop BLDC controller initialized version %hhu.%hhu \
            type %hhu motors %hhu, flights %hu, last flight time %hu, \
            total flight time %u, last error %hhu", info.version_major, 
            info.version_minor, info.type, info.n_motors_controlled,
            info.n_flights, info.last_flight_time, (unsigned int)info.total_flight_time,
            info.last_error);
    }
    else
    {
        returnValue = -1;
        goto end_of_function;
    }

    returnBool = AMC_Custom_Play_Sound();
    if(FALSE == returnBool)
    {
        returnValue = -1;
        goto end_of_function;
    }
    
    returnBool = AMC_Custom_Get_Observation(&AMC_AppCustomData.ObservationMsg.observation);
    if(TRUE == returnBool)
    {
        (void) CFE_EVS_SendEvent(AMC_DEVICE_INF_EID, CFE_EVS_INFORMATION,
            "AMC BLDC info mV %hu, status %hhu, error %hhu, faults %hhu, temperature C %hhu", 
            AMC_AppCustomData.ObservationMsg.observation.battery_voltage_mv, 
            AMC_AppCustomData.ObservationMsg.observation.status, 
            AMC_AppCustomData.ObservationMsg.observation.error,
            AMC_AppCustomData.ObservationMsg.observation.motors_in_fault, 
            AMC_AppCustomData.ObservationMsg.observation.temperatur_c);
    }
    else
    {
        returnValue = -1;
        goto end_of_function;
    }

    CFE_SB_InitMsg(&AMC_AppCustomData.ObservationMsg,
            AMC_OUT_DATA_MID, sizeof(AMC_AppCustomData.ObservationMsg), TRUE);

    AMC_AppCustomData.Status = AMC_CUSTOM_INITIALIZED;

end_of_function:
    return (returnValue);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* AMC::DeInitDevice function.                                     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
//int32 AMC::DeInitDevice(void)
//{
    //int32 returnCode = CFE_SUCCESS;
    //returnCode = close(AMC_AppCustomData.DeviceFd);
    //return (returnCode);
//}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* AMC::SetMotorOutputs function.                                  */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void AMC::SetMotorOutputs(const uint16 *PWM)
{
    uint32 i = 0;
    static uint32 count = 0;
    float motor_speeds[4];

    /* If disarmed stop motors. */
    if(PWM[0] <= AMC_PWM_DISARMED)
    {
        /* If motors aren't already stopped... */
        if (AMC_AppCustomData.Status == AMC_CUSTOM_MOTORS_STARTED)
        {
            (void) AMC_Stop_Motors();
        }
    }

    /* If armed start motors. */
    if(PWM[0] > AMC_PWM_DISARMED)
    {
        for (i = 0; i < 4; ++i)
        {
            motor_speeds[i] = AMC_Scale_To_Dimensionless(PWM[i]);
        }

        (void) AMC_Set_ESC_Speeds(motor_speeds);
        /* If motors aren't already started... */
        if (AMC_AppCustomData.Status == AMC_CUSTOM_MOTORS_STOPPED || AMC_AppCustomData.Status == AMC_CUSTOM_INITIALIZED)
        {
            (void) AMC_Start_Motors();
        }
    }

    /* Publish at 10 Hz. */
    if(25 >= count)
    {
        (void) AMC_Custom_Get_Observation(&AMC_AppCustomData.ObservationMsg.observation);
        CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&AMC_AppCustomData.ObservationMsg);
        CFE_SB_SendMsg((CFE_SB_Msg_t*)&AMC_AppCustomData.ObservationMsg);
        count = 0;
    }
    count++;
}


int32 AMC_Ioctl(int fh, int request, void *arg)
{
    int32 returnCode = 0;
    uint32 i = 0;

    for (i = 0; i < AMC_MAX_RETRY_ATTEMPTS; ++i)
    {
        returnCode = ioctl(fh, request, arg);
            
        if (-1 == returnCode && EINTR == errno)
        {
            usleep(AMC_MAX_RETRY_SLEEP_USEC);
        }
        else
        {
            break;
        }
    }

    return returnCode;
}


boolean AMC_WriteReg(uint8 Addr, uint8 Data)
{
    int returnCode      = 0;
    boolean returnBool  = FALSE;
    struct i2c_msg Messages[1];
    struct i2c_rdwr_ioctl_data Packets;
    uint8 buf[2];

    buf[0] = Addr;
    buf[1] = Data;

    Messages[0].addr  = AMC_BLDC_I2C_SLAVE_ADDRESS;
    Messages[0].flags = 0;
    Messages[0].buf   = &buf[0];
    Messages[0].len   = 2;

    Packets.msgs  = Messages;
    Packets.nmsgs = 1;

    CFE_ES_PerfLogEntry(AMC_SEND_PERF_ID);
    returnCode = AMC_Ioctl(AMC_AppCustomData.DeviceFd, I2C_RDWR, &Packets);
    CFE_ES_PerfLogExit(AMC_SEND_PERF_ID);
    
    if (-1 == returnCode) 
    {            
        (void) CFE_EVS_SendEvent(AMC_DEVICE_ERR_EID, CFE_EVS_ERROR,
                        "AMC write register failed. ");
    }
    else
    {
        returnBool = TRUE;
    }

    return (returnBool);
}


boolean AMC_ReadReg(uint8 Reg, void *Buffer, size_t Length)
{
    int returnCode       = 0;
    boolean returnBool   = FALSE;
    uint8 cmd            = Reg;

    struct i2c_rdwr_ioctl_data Packets;
    struct i2c_msg Messages[2];

    /* send */
    Messages[0].addr  = AMC_BLDC_I2C_SLAVE_ADDRESS;
    Messages[0].flags = 0;
    Messages[0].buf   = &cmd;
    Messages[0].len   = 1;

    /* receive */
    Messages[1].addr  = AMC_BLDC_I2C_SLAVE_ADDRESS;
    Messages[1].flags = AMC_I2C_M_READ;
    Messages[1].buf   = (uint8 *) Buffer;
    Messages[1].len   = Length;

    Packets.msgs  = Messages;
    Packets.nmsgs = 2;

    CFE_ES_PerfLogEntry(AMC_RECEIVE_PERF_ID);
    returnCode = AMC_Ioctl(AMC_AppCustomData.DeviceFd, I2C_RDWR, &Packets);
    CFE_ES_PerfLogExit(AMC_RECEIVE_PERF_ID);

    if (-1 == returnCode) 
    {            
        (void) CFE_EVS_SendEvent(AMC_DEVICE_ERR_EID, CFE_EVS_ERROR,
                        "AMC read register failed. ");
    }
    else
    {
        returnBool = TRUE;
    }

    return (returnBool);
}


boolean AMC_Start_Motors(void)
{
    boolean returnBool = TRUE;
    
    returnBool = AMC_WriteReg(AMC_BLDC_REG_START_BLDC, AMC_BLDC_BITS_RLRL);
    if (FALSE == returnBool) 
    {            
        (void) CFE_EVS_SendEvent(AMC_DEVICE_ERR_EID, CFE_EVS_ERROR,
                        "AMC start motors failed. ");
    }
    else
    {
        AMC_AppCustomData.Status = AMC_CUSTOM_MOTORS_STARTED;
    }

    return (returnBool);
}


boolean AMC_Stop_Motors(void)
{
    int returnCode      = 0;
    boolean returnBool  = FALSE;
    struct i2c_msg Messages[1];
    struct i2c_rdwr_ioctl_data Packets;
    uint8 buf[1];

    buf[0] = AMC_BLDC_REG_STOP_BLDC;

    Messages[0].addr  = AMC_BLDC_I2C_SLAVE_ADDRESS;
    Messages[0].flags = 0;
    Messages[0].buf   = &buf[0];
    Messages[0].len   = 1;

    Packets.msgs  = Messages;
    Packets.nmsgs = 1;

    CFE_ES_PerfLogEntry(AMC_SEND_PERF_ID);
    returnCode = AMC_Ioctl(AMC_AppCustomData.DeviceFd, I2C_RDWR, &Packets);
    CFE_ES_PerfLogExit(AMC_SEND_PERF_ID);
    
    if (-1 == returnCode) 
    {            
        (void) CFE_EVS_SendEvent(AMC_DEVICE_ERR_EID, CFE_EVS_ERROR,
                        "AMC stop motors failed. ");
    }
    else
    {
        returnBool = TRUE;
        AMC_AppCustomData.Status = AMC_CUSTOM_MOTORS_STOPPED;
    }

    return (returnBool);
}


boolean AMC_Clear_Errors(void)
{
    int returnCode      = 0;
    boolean returnBool  = FALSE;
    struct i2c_msg Messages[1];
    struct i2c_rdwr_ioctl_data Packets;
    uint8 buf[1];

    buf[0] = AMC_BLDC_REG_CLEAR_ERROR;

    Messages[0].addr  = AMC_BLDC_I2C_SLAVE_ADDRESS;
    Messages[0].flags = 0;
    Messages[0].buf   = &buf[0];
    Messages[0].len   = 1;

    Packets.msgs  = Messages;
    Packets.nmsgs = 1;

    CFE_ES_PerfLogEntry(AMC_SEND_PERF_ID);
    returnCode = AMC_Ioctl(AMC_AppCustomData.DeviceFd, I2C_RDWR, &Packets);
    CFE_ES_PerfLogExit(AMC_SEND_PERF_ID);
    
    if (-1 == returnCode) 
    {            
        (void) CFE_EVS_SendEvent(AMC_DEVICE_ERR_EID, CFE_EVS_ERROR,
                        "AMC stop motors failed. ");
    }
    else
    {
        returnBool = TRUE;
    }

    return (returnBool);
}


uint8 AMC_Calculate_Checksum(uint8 initial, uint8 *data, uint16 packet_size)
{
    uint8 checksum = initial;
    uint32 i       = 0;

    for (i = 0; i < packet_size; ++i) 
    {
        checksum = checksum ^ data[i];
    }

    return (checksum);
}


uint16 AMC_Scale_To_RPM(float scale)
{
    float checked_scale = scale;

    if (checked_scale < 0.0f) 
    {
        checked_scale = 0.0f;
    }

    if (checked_scale > 1.0f) 
    {
        checked_scale = 1.0f;
    }

    // we assume that scale is a dimensionless desired thrust force in the range [0,1]
    // remap scale to the range [scale_min, 1], where scale_min represents the minimum
    // dimensionless force that we can output (because lowest motor speed is constrained).
    // Then calculate back from dimensionless force to desired motor speed
    // Force_norm = w^2 / w_max^2
    // Force_norm: dimensionless force in range [scale_min, 1]
    // w: desired motor speed in rpm
    // w_max: maximum motor speed
    float rpm_min_sq = (AMC_BLDC_LIMITS_RPM_MIN * AMC_BLDC_LIMITS_RPM_MIN);
    float rmp_max_sq = (AMC_BLDC_LIMITS_RPM_MAX * AMC_BLDC_LIMITS_RPM_MAX);
    float scale_min = rpm_min_sq / rmp_max_sq;

    // remap dimensionless force to range [scale_min, 1]
    scale = scale_min + (1.0f - scale_min) * scale;

    // use Force_norm = w^2 / w_max^2 and solve for w
    return sqrtf(scale * rmp_max_sq);
}


float AMC_Scale_To_Dimensionless(uint16 PWM)
{
    float returnVal = 0;
    uint16 PWM_Constrained = 0;

    /* Constrain */
    if(PWM < AMC_PWM_MIN)
    {
        PWM_Constrained = AMC_PWM_MIN;
    }
    else if (PWM > AMC_PWM_MAX)
    {
        PWM_Constrained = AMC_PWM_MAX;
    }
    else
    {
        PWM_Constrained = PWM;
    }

    /* Normalize PWM to dimensionless range 0 to 1 */
    returnVal = (float) (PWM_Constrained - AMC_PWM_MIN) / (AMC_PWM_MAX - AMC_PWM_MIN);
    
    return (returnVal);
}


boolean AMC_Set_ESC_Speeds(const float speeds[4])
{
    AMC_BLDC_ESC_Speeds_t data = {0};
    int returnCode      = 0;
    boolean returnBool  = FALSE;
    struct i2c_msg Messages[1];
    struct i2c_rdwr_ioctl_data Packets;
    uint8 buf[11];
    memset(&data, 0, sizeof(data));

    // Correct endians and scale to MIN-MAX rpm
    // Adjust order of BLDCs from PX4 to Bebop
    data.rpm_front_left = AMC_Swap16(AMC_Scale_To_RPM(speeds[2]));
    data.rpm_front_right = AMC_Swap16(AMC_Scale_To_RPM(speeds[0]));
    data.rpm_back_right = AMC_Swap16(AMC_Scale_To_RPM(speeds[3]));
    data.rpm_back_left = AMC_Swap16(AMC_Scale_To_RPM(speeds[1]));

    AMC_AppCustomData.SpeedSetpoint[0] = AMC_Swap16(data.rpm_front_right);
    AMC_AppCustomData.SpeedSetpoint[1] = AMC_Swap16(data.rpm_front_left);
    AMC_AppCustomData.SpeedSetpoint[2] = AMC_Swap16(data.rpm_back_right);
    AMC_AppCustomData.SpeedSetpoint[3] = AMC_Swap16(data.rpm_back_left);

    data.enable_security = 0x00;

    data.checksum = AMC_Calculate_Checksum(AMC_BLDC_REG_SET_ESC_SPEED, (uint8 *) &data, sizeof(data) - 1);

    buf[0] = AMC_BLDC_REG_SET_ESC_SPEED;
    memcpy(&buf[1], &data, 10);

    Messages[0].addr  = AMC_BLDC_I2C_SLAVE_ADDRESS;
    Messages[0].flags = 0;
    Messages[0].buf   = &buf[0];
    Messages[0].len   = 11;

    Packets.msgs  = Messages;
    Packets.nmsgs = 1;

    CFE_ES_PerfLogEntry(AMC_SEND_PERF_ID);
    returnCode = AMC_Ioctl(AMC_AppCustomData.DeviceFd, I2C_RDWR, &Packets);
    CFE_ES_PerfLogExit(AMC_SEND_PERF_ID);
    
    if (-1 == returnCode) 
    {            
        (void) CFE_EVS_SendEvent(AMC_DEVICE_ERR_EID, CFE_EVS_ERROR,
                        "AMC write register failed. ");
    }
    else
    {
        returnBool = TRUE;
    }

    return (returnBool);
}


uint16 AMC_Swap16(uint16 val) 
{
    return (val >> 8) | (val << 8); 
}


uint32 AMC_Swap32(uint32 val)
{
    return (val >> 24) | ((val >> 8) & 0x0000FF00) | ((val << 8) & 0x00FF0000) | (val << 24); 
}


boolean AMC_Custom_Max_Events_Not_Reached(int32 ind)
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


int32 AMC_Custom_Init_EventFilters(int32 ind, CFE_EVS_BinFilter_t *EventTbl)
{
    int32 customEventCount = ind;
    
    /* Null check */
    if(0 == EventTbl)
    {
        customEventCount = -1;
        goto end_of_function;
    }

    if(TRUE == AMC_Custom_Max_Events_Not_Reached(customEventCount))
    {
        EventTbl[  customEventCount].EventID = AMC_DEVICE_ERR_EID;
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


boolean AMC_Custom_Play_Sound(void)
{
    boolean returnBool = TRUE;
    
    returnBool = AMC_WriteReg(AMC_BLDC_REG_PLAY_SOUND, AMC_BLDC_SOUND_BOOT);
    if (FALSE == returnBool) 
    {            
        (void) CFE_EVS_SendEvent(AMC_DEVICE_ERR_EID, CFE_EVS_ERROR,
                        "AMC play sound failed. ");
    }

    return (returnBool);
}


boolean AMC_Custom_Get_Info(AMC_BLDC_Info_t *info)
{
    boolean returnBool = FALSE;

    if(0 == info)
    {
        goto end_of_function;
    }

    returnBool = AMC_ReadReg(AMC_BLDC_REG_GET_INFO, (void *) info, sizeof(AMC_BLDC_Info_t));

    if(FALSE == returnBool)
    {
        goto end_of_function;
    }

    /* correct endianness */
    info->n_flights = AMC_Swap16(info->n_flights);
    info->last_flight_time = AMC_Swap16(info->last_flight_time);
    info->total_flight_time = AMC_Swap32(info->total_flight_time);

end_of_function:
    return returnBool;
}


boolean AMC_Custom_Get_Observation(AMC_BLDC_Observation_t *observation)
{
    boolean returnBool = FALSE;

    if(0 == observation)
    {
        goto end_of_function;
    }

    returnBool = AMC_ReadReg(AMC_BLDC_REG_GET_OBS, (void *) observation, sizeof(AMC_BLDC_Observation_t));

    if(FALSE == returnBool)
    {
        goto end_of_function;
    }

    // Endian conversion and remove saturation bit
    observation->rpm_front_left = AMC_Swap16(observation->rpm_front_left) & ~(1 << 15);
    observation->rpm_front_right = AMC_Swap16(observation->rpm_front_right) & ~(1 << 15);
    observation->rpm_back_right = AMC_Swap16(observation->rpm_back_right) & ~(1 << 15);
    observation->rpm_back_left = AMC_Swap16(observation->rpm_back_left) & ~(1 << 15);
    observation->battery_voltage_mv = AMC_Swap16(observation->battery_voltage_mv);

end_of_function:
    return returnBool;
}
