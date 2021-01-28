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
#include "rgbled_driver.h"
#include "rgbled_events.h"
#include "rgbled_perfids.h"
#include "rgbled_platform_cfg.h"
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>

/************************************************************************
** Local Defines
*************************************************************************/

/************************************************************************
** Local Structure Declarations
*************************************************************************/
typedef enum {

/** \brief <tt> 'RGBLED - ' </tt>
**  \event <tt> 'RGBLED - ' </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when a device resource encounters an 
**  error.
**
*/
    RGBLED_DEVICE_ERR_EID = RGBLED_EVT_CNT,

/** \brief <tt> 'RGBLED - ' </tt>
**  \event <tt> 'RGBLED - ' </tt>
**  
**  \par Type: Info
**
**  \par Cause:
**
**  This event message is issued when a device successfully complete a
**  self test.
**
*/
    RGBLED_DEVICE_INF_EID,
/** \brief Number of custom events 
**
**  \par Limits:
**       int32
*/
    RGBLED_CUSTOM_EVT_CNT
} RGBLED_CustomEventIds_t;


/************************************************************************
** External Global Variables
*************************************************************************/

/************************************************************************
** Global Variables
*************************************************************************/
RGBLED_AppCustomData_t RGBLED_AppCustomData;

/************************************************************************
** Local Variables
*************************************************************************/

/************************************************************************
** Local Function Definitions
*************************************************************************/
int32 RGBLED_Ioctl(int fh, int request, void *arg)
{
    int32 returnCode = 0;
    uint32 i = 0;

    for (i=0; i < RGBLED_MAX_RETRY_ATTEMPTS; i++)
    {
        returnCode = ioctl(fh, request, arg);
            
        if (-1 == returnCode && EINTR == errno)
        {
            usleep(RGBLED_MAX_RETRY_SLEEP_USEC);
        }
        else
        {
            break;
        }
    }

    return (returnCode);
}


void RGBLED_Custom_InitData(void)
{
    /* Set all struct zero values */
    bzero(&RGBLED_AppCustomData, sizeof(RGBLED_AppCustomData));

    strncpy(RGBLED_AppCustomData.DevName, RGBLED_DEVICE_PATH, RGBLED_MAX_DEVICE_PATH);
    RGBLED_AppCustomData.SelfTestTask = RGBLED_Custom_SelfTest_Task;
    return;
}


boolean RGBLED_Custom_Init(void)
{
    boolean returnBool = TRUE;
    
    RGBLED_AppCustomData.DeviceFd = open(RGBLED_DEVICE_PATH, O_RDWR);

    if (RGBLED_AppCustomData.DeviceFd < 0) 
    {
        (void) CFE_EVS_SendEvent(RGBLED_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "RGBLED Device open errno: %i", errno);
        returnBool = FALSE;
        goto end_of_function;
    }
    else
    {
        RGBLED_AppCustomData.Status = RGBLED_CUSTOM_INITIALIZED;
    }
    
    returnBool = RGBLED_Custom_GetSettings();
    if (FALSE == returnBool)
    {
        (void) CFE_EVS_SendEvent(RGBLED_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "RGBLED Device custom enable failed to get initial settings");
        goto end_of_function;
    }
    
    returnBool = RGBLED_Custom_Enable();
    if (FALSE == returnBool)
    {
        (void) CFE_EVS_SendEvent(RGBLED_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "RGBLED Device custom enable failed errno: %i", errno);
        goto end_of_function;
    }
    else
    {
        RGBLED_AppCustomData.Status = RGBLED_CUSTOM_ENABLED;
    }

end_of_function:
    return (returnBool);
}


boolean RGBLED_Custom_Enable(void)
{
    boolean returnBool = FALSE; 
    uint8 settings     = 0;

    /* Enable output */
    settings |= RGBLED_I2C_SET_ENABLE;
    settings |= RGBLED_I2C_SET_NOT_POWERSAVE;

    uint8 data[2] = { RGBLED_I2C_SUB_ADDR_SETTINGS, settings };

    returnBool = RGBLED_Custom_Send(data, sizeof(data));
    if(TRUE == returnBool)
    {
        RGBLED_AppCustomData.Settings.Enabled = TRUE;
        RGBLED_AppCustomData.Settings.NotPowerSave = TRUE;
    }

    returnBool = RGBLED_Custom_Validate();
    if(FALSE == returnBool)
    {
        (void) CFE_EVS_SendEvent(RGBLED_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "RGBLED device settings validation failed in custom enable");
    }
    return (returnBool);
}


boolean RGBLED_Custom_Disable(void)
{
    boolean returnBool = FALSE;
    uint8 settings     = 0;
    
    uint8 data[2] = { RGBLED_I2C_SUB_ADDR_SETTINGS, settings };
    
    returnBool = RGBLED_Custom_Send(data, sizeof(data));
    if(TRUE == returnBool)
    {
        RGBLED_AppCustomData.Settings.Enabled = FALSE;
        RGBLED_AppCustomData.Settings.NotPowerSave = FALSE;
    }

    returnBool = RGBLED_Custom_Validate();
    if(FALSE == returnBool)
    {
        (void) CFE_EVS_SendEvent(RGBLED_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "RGBLED device settings validation failed in custom disable");
    }
    return (returnBool);
}


boolean RGBLED_Custom_GetSettings(void)
{
    boolean returnBool   = TRUE;
    boolean enabled      = FALSE;
    boolean notPowerSave = FALSE;
    uint8 settings       = 0;
    uint8 redPWM         = 0;
    uint8 greenPWM       = 0;
    uint8 bluePWM        = 0;

    /* The TCA62724FMG LED driver returns 2-bytes */
    uint8 Result[2] = {0, 0};

    returnBool = RGBLED_Custom_Receive(Result, sizeof(Result));

    if(FALSE == returnBool)
    {
        /* If receive was unsuccessful return FALSE */
        goto end_of_function;
    }

    /* Settings are bits 7 - 4 of returned byte 0 */
    settings    = Result[0] & 0xf0;
    /* Blue LED duty cycle is bits 3 - 0 of returned byte 0 */
    bluePWM     = Result[0] & 0x0f;
    /* Green LED duty cycle is bits 7 - 4 of returned byte 1 */ 
    greenPWM    = (Result[1] & 0xf0) >> 4;
    /* Red LED duty cycle is bits 3 - 0 of returned byte 1 */
    redPWM      = Result[1] & 0x0f;

    /* If settings bit 5 is set (1) output is enabled */
    /* if settings bit 4 is set (1) powersave mode is not set */
    if(48 == settings)
    {
        enabled = TRUE;
        notPowerSave = TRUE;
    }
    else if(32 == settings)
    {
        enabled = TRUE;
    }
    else if(16 == settings)
    {
        notPowerSave = TRUE;
    }

    RGBLED_AppCustomData.Settings.Enabled        = enabled;
    RGBLED_AppCustomData.Settings.NotPowerSave   = notPowerSave;
    RGBLED_AppCustomData.Settings.RedDutyCycle   = redPWM;
    RGBLED_AppCustomData.Settings.GreenDutyCycle = greenPWM;
    RGBLED_AppCustomData.Settings.BlueDutyCycle  = bluePWM;

end_of_function:
    return (returnBool);
}


boolean RGBLED_Custom_Validate(void)
{
    boolean returnBool   = FALSE;
    boolean enabled      = FALSE;
    boolean notPowerSave = FALSE;
    uint8 settings       = 0;
    uint8 redPWM         = 0;
    uint8 greenPWM       = 0;
    uint8 bluePWM        = 0;

    /* The TCA62724FMG LED driver returns 2-bytes */
    uint8 Result[2] = {0, 0};

    returnBool = RGBLED_Custom_Receive(Result, sizeof(Result));

    if(FALSE == returnBool)
    {
        /* If receive was unsuccessful return FALSE */
        goto end_of_function;
    }
    
    /* Settings are bits 7 - 4 of returned byte 0 */
    settings    = Result[0] & 0xf0;
    /* Blue LED duty cycle is bits 3 - 0 of returned byte 0 */
    bluePWM     = Result[0] & 0x0f;
    /* Green LED duty cycle is bits 7 - 4 of returned byte 1 */ 
    greenPWM    = (Result[1] & 0xf0) >> 4;
    /* Red LED duty cycle is bits 3 - 0 of returned byte 1 */
    redPWM      = Result[1] & 0x0f;

    /* If settings bit 5 is set (1) output is enabled */
    /* if settings bit 4 is set (1) powersave mode is not set */
    if(48 == settings)
    {
        enabled = TRUE;
        notPowerSave = TRUE;
    }
    else if(32 == settings)
    {
        enabled = TRUE;
    }
    else if(16 == settings)
    {
        notPowerSave = TRUE;
    }

    if( RGBLED_AppCustomData.Settings.Enabled        == enabled      &&
        RGBLED_AppCustomData.Settings.NotPowerSave   == notPowerSave &&
        RGBLED_AppCustomData.Settings.RedDutyCycle   == redPWM       &&
        RGBLED_AppCustomData.Settings.GreenDutyCycle == greenPWM     &&
        RGBLED_AppCustomData.Settings.BlueDutyCycle  == bluePWM )
    {
        returnBool = TRUE;
    }
    else
    {
        returnBool = FALSE;
    }

end_of_function:
    return (returnBool);
}


boolean RGBLED_Custom_Send(uint8 *Buffer, size_t Length)
{
    int returnCode       = 0;
    boolean returnBool   = FALSE;
    struct i2c_msg Messages[1];
    struct i2c_rdwr_ioctl_data Packets;

    Messages[0].addr  = RGBLED_I2C_ADDRESS;
    Messages[0].flags = 0;
    Messages[0].buf   = Buffer;
    Messages[0].len   = Length;

    Packets.msgs  = Messages;
    Packets.nmsgs = 1;

    CFE_ES_PerfLogEntry(RGBLED_SEND_PERF_ID);
    returnCode = RGBLED_Ioctl(RGBLED_AppCustomData.DeviceFd, I2C_RDWR, &Packets);
    CFE_ES_PerfLogExit(RGBLED_SEND_PERF_ID);
    
    if (-1 == returnCode) 
    {            
        (void) CFE_EVS_SendEvent(RGBLED_DEVICE_ERR_EID, CFE_EVS_ERROR,
                        "RGBLED ioctl returned %i", errno);
        returnBool = FALSE;
    }
    else
    {
        returnBool = TRUE;
    }

    return (returnBool);
}


boolean RGBLED_Custom_Receive(uint8 *Buffer, size_t Length)
{
    int returnCode     = 0;
    boolean returnBool = FALSE;
    struct i2c_msg messages[1];
    struct i2c_rdwr_ioctl_data Packets;

    messages[0].addr  = RGBLED_I2C_ADDRESS;
    messages[0].flags = RGBLED_I2C_M_READ;
    messages[0].buf   = Buffer;
    messages[0].len   = Length;

    Packets.msgs  = messages;
    Packets.nmsgs = 1;

    CFE_ES_PerfLogEntry(RGBLED_RECEIVE_PERF_ID);
    returnCode = RGBLED_Ioctl(RGBLED_AppCustomData.DeviceFd, I2C_RDWR, &Packets);
    CFE_ES_PerfLogExit(RGBLED_RECEIVE_PERF_ID);

    if (-1 == returnCode) 
    {            
        (void) CFE_EVS_SendEvent(RGBLED_DEVICE_ERR_EID, CFE_EVS_ERROR,
                        "RGBLED ioctl returned %i", errno);
        returnBool = FALSE;
    }
    else
    {
        returnBool = TRUE;
    }

    return (returnBool);
}


boolean RGBLED_Custom_SetColor(uint8 Red, uint8 Green, uint8 Blue)
{
    boolean returnBool = FALSE;
    /* Scale from 0-255 to 0-15 */
    uint8 redPWM    = Red >> 4;
    uint8 greenPWM  = Green >> 4;
    uint8 bluePWM   = Blue >> 4;

    uint8 data[6] = { RGBLED_I2C_SUB_ADDR_PWM0, redPWM,
                      RGBLED_I2C_SUB_ADDR_PWM1, greenPWM,
                      RGBLED_I2C_SUB_ADDR_PWM2, bluePWM };

    returnBool = RGBLED_Custom_Send(&data[0], sizeof(data));
    
    /* Set the current value */
    RGBLED_AppCustomData.Settings.RedDutyCycle   = redPWM;
    RGBLED_AppCustomData.Settings.GreenDutyCycle = greenPWM;
    RGBLED_AppCustomData.Settings.BlueDutyCycle  = bluePWM;
    
    returnBool = RGBLED_Custom_Validate();
    if(FALSE == returnBool)
    {
        (void) CFE_EVS_SendEvent(RGBLED_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "RGBLED device settings validation failed");
    }

    return (returnBool);
}


boolean RGBLED_Custom_SelfTest(void)
{
    boolean returnBool = TRUE;
    int32 returnCode = 0;
    
    /* Create the self-test task */
    returnCode = CFE_ES_CreateChildTask(
        &RGBLED_AppCustomData.ChildTaskID,
        RGBLED_SELFTEST_TASK_NAME,
        RGBLED_AppCustomData.SelfTestTask,
        0,
        CFE_ES_DEFAULT_STACK_SIZE,
        RGBLED_SELFTEST_TASK_PRIORITY,
        RGBLED_SELFTEST_TASK_FLAGS);

    if(returnCode != CFE_SUCCESS)
    {
        return FALSE;
    }
    return (returnBool);
}


void RGBLED_Custom_SelfTest_Task(void)
{
    uint32 iStatus = -1;
    uint8 redPWM;
    uint8 greenPWM;
    uint8 bluePWM;

    /* Save the original settings */
    redPWM   = RGBLED_AppCustomData.Settings.RedDutyCycle;
    greenPWM = RGBLED_AppCustomData.Settings.GreenDutyCycle;
    bluePWM  = RGBLED_AppCustomData.Settings.BlueDutyCycle;

    iStatus = CFE_ES_RegisterChildTask();

    if (iStatus == CFE_SUCCESS)
    {
        RGBLED_Custom_SetColor(255, 0, 0);
        sleep(1);

        RGBLED_Custom_SetColor(0, 255, 0);
        sleep(1);

        RGBLED_Custom_SetColor(0, 0, 255);
        sleep(1);

        RGBLED_Custom_SetColor(255, 255, 255);
        sleep(1);
    
        RGBLED_Custom_SetColor(0, 0, 0);
        sleep(1);
        /* Restore the original settings */
        RGBLED_Custom_SetColor((redPWM << 4), (greenPWM << 4), (bluePWM << 4));
        
    }
    (void) CFE_EVS_SendEvent(RGBLED_DEVICE_INF_EID, CFE_EVS_INFORMATION,
            "RGBLED Device completed self test");
    /* Set app status in none custom code back to previous value */
    RGBLED_SelfTest_Complete();

    /* The child task was successfully created so exit from it */
    if (iStatus == CFE_SUCCESS)
    {
        CFE_ES_ExitChildTask();
    }
}



boolean RGBLED_Custom_Uninit(void)
{
    boolean returnBool = TRUE;
    int returnCode     = 0;

    returnBool = RGBLED_Custom_Disable();
    if(FALSE == returnBool)
    {
        (void) CFE_EVS_SendEvent(RGBLED_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "RGBLED Device disable error");
    }
    else
    {
        RGBLED_AppCustomData.Status == RGBLED_CUSTOM_INITIALIZED;
    }

    returnCode = close(RGBLED_AppCustomData.DeviceFd);
    if (-1 == returnCode) 
    {
        (void) CFE_EVS_SendEvent(RGBLED_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "RGBLED Device close errno: %i", errno);
        returnBool = FALSE;
    }
    else
    {
        RGBLED_AppCustomData.Status = RGBLED_CUSTOM_UNINITIALIZED;
    }
    return (returnBool);
}


