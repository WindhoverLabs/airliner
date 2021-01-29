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
#include "rcin_sbus.h"
#include "rcin_events.h"
#include "perf_ids.h"
#include "px4lib.h"
#include "rcin_platform_cfg.h"

/************************************************************************
** Local Defines
*************************************************************************/

/************************************************************************
** Local Structure Declarations
*************************************************************************/

typedef enum {

/** \brief <tt> 'RCIN - ' </tt>
**  \event <tt> 'RCIN - ' </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when a device resource encounters an 
**  error.
**
*/
    RCIN_DEVICE_ERR_EID = RCIN_EVT_CNT,
/** \brief <tt> 'RCIN - ' </tt>
**  \event <tt> 'RCIN - ' </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the RC input stream is out of 
**  sync.
**
*/
    RCIN_SYNC_ERR_EID,

/** \brief Number of custom events 
**
**  \par Limits:
**       int32
*/
    RCIN_CUSTOM_EVT_CNT

} RCIN_CustomEventIds_t;

/************************************************************************
** External Global Variables
*************************************************************************/

/************************************************************************
** Global Variables
*************************************************************************/
RCIN_AppCustomData_t RCIN_AppCustomData;

/************************************************************************
** Local Variables
*************************************************************************/

/************************************************************************
** Local Function Definitions
*************************************************************************/
int32 RCIN_Ioctl(int fh, int request, void *arg)
{
    int32 returnCode = 0;
    uint32 i         = 0;

    for (i=0; i < RCIN_MAX_RETRY_ATTEMPTS; i++)
    {
        returnCode = ioctl(fh, request, arg);
            
        if (-1 == returnCode && EINTR == errno)
        {
            usleep(RCIN_MAX_RETRY_SLEEP_USEC);
        }
        else
        {
            break;
        }
    }

    return (returnCode);
}


int32 RCIN_Read(int fd, void *buf, size_t count)
{
    int32 returnSize = 0;
    uint32 i         = 0;
    
    for (i=0; i < RCIN_MAX_RETRY_ATTEMPTS; i++)
    {
        returnSize = read(fd, buf, count);
        /* Interrupted */
        if (-1 == returnSize && EINTR == errno)
        {
            (void) CFE_EVS_SendEvent(RCIN_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "RCIN device read EINTR");
            usleep(RCIN_MAX_RETRY_SLEEP_USEC);
        }
        else
        {
            break;
        }
    }

    return (returnSize);
}


void RCIN_Custom_InitData(void)
{
    /* Set all struct zero values */
    bzero(&RCIN_AppCustomData, sizeof(RCIN_AppCustomData));
    strncpy(RCIN_AppCustomData.DevName, RCIN_DEVICE_PATH, RCIN_MAX_DEVICE_PATH);
    /* Set all non-zero values */
    RCIN_AppCustomData.ContinueFlag          = TRUE;
    RCIN_AppCustomData.Priority              = RCIN_STREAMING_TASK_PRIORITY;
    RCIN_AppCustomData.StreamingTask         = RCIN_Stream_Task;
    /* Initialize for failsafe and RcLost */
    RCIN_AppCustomData.Measure.RcFailsafe    = 1;
    RCIN_AppCustomData.Measure.RcLost        = 1;
    return;
}


boolean RCIN_Custom_Init(void)
{
    boolean returnBool = TRUE;
    int32 returnCode   = 0;
    int baud           = RCIN_SERIAL_INPUT_SPEED;
    struct serial_struct serials;

    /* Open */
    RCIN_AppCustomData.DeviceFd = open(RCIN_DEVICE_PATH, O_RDWR | O_NONBLOCK | O_CLOEXEC);
    if (RCIN_AppCustomData.DeviceFd < 0) 
    {
        (void) CFE_EVS_SendEvent(RCIN_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "RCIN custom device open errno: %i", errno);
        returnBool = FALSE;
        goto end_of_function;
    }
    else
    {
        RCIN_AppCustomData.Status = RCIN_CUSTOM_INITIALIZED;
    }

    /* Create mutex for shared data */
    returnCode = OS_MutSemCreate(&RCIN_AppCustomData.Mutex, RCIN_MUTEX_NAME, 0);
    if (returnCode != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(RCIN_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "RCIN mutex create failed in custom init");
        returnBool = FALSE;
        goto end_of_function;
    }

    /* Get configuration */
    returnCode = tcgetattr(RCIN_AppCustomData.DeviceFd, 
            &RCIN_AppCustomData.TerminalConfig);
    if (0 != returnCode)
    {
        (void) CFE_EVS_SendEvent(RCIN_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "RCIN custom get terminal config failed errno: %i", errno);
        goto end_of_function;
    }
    
    tcflush(RCIN_AppCustomData.DeviceFd, TCIFLUSH);
    bzero(&RCIN_AppCustomData.TerminalConfig, 
            sizeof(RCIN_AppCustomData.TerminalConfig));
    RCIN_AppCustomData.TerminalConfig.c_cflag |= (CLOCAL | CREAD);
    RCIN_AppCustomData.TerminalConfig.c_cflag &= ~CSIZE;
    RCIN_AppCustomData.TerminalConfig.c_cflag |= CS8;
    RCIN_AppCustomData.TerminalConfig.c_cflag |= PARENB;
    RCIN_AppCustomData.TerminalConfig.c_cflag &= ~PARODD;
    RCIN_AppCustomData.TerminalConfig.c_iflag |= INPCK;
    RCIN_AppCustomData.TerminalConfig.c_cflag |= CSTOPB;
    RCIN_AppCustomData.TerminalConfig.c_cc[VTIME] = 0;
    RCIN_AppCustomData.TerminalConfig.c_cc[VMIN] = 0;

    cfsetispeed(&RCIN_AppCustomData.TerminalConfig, B38400);
    cfsetospeed(&RCIN_AppCustomData.TerminalConfig, B38400);
    
    tcflush(RCIN_AppCustomData.DeviceFd, TCIFLUSH);
    
    returnCode = tcsetattr(RCIN_AppCustomData.DeviceFd, TCSANOW, 
            &RCIN_AppCustomData.TerminalConfig);
    if (0 != returnCode)
    {
        (void) CFE_EVS_SendEvent(RCIN_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "RCIN custom set terminal config failed errno: %i", errno);
        goto end_of_function;
    }
    
    returnCode = RCIN_Ioctl(RCIN_AppCustomData.DeviceFd, TIOCGSERIAL, 
            &serials);
    if (0 > returnCode)
    {
        (void) CFE_EVS_SendEvent(RCIN_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "RCIN custom get serial line info errno: %i", errno);
        goto end_of_function;
    }

    serials.flags = ASYNC_SPD_CUST;
    serials.custom_divisor = serials.baud_base / baud;

    returnCode = RCIN_Ioctl(RCIN_AppCustomData.DeviceFd, TIOCSSERIAL, 
            &serials);
    if (0 > returnCode)
    {
        (void) CFE_EVS_SendEvent(RCIN_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "RCIN custom set serial line info errno: %i", errno);
        goto end_of_function;
    }

    RCIN_Ioctl(RCIN_AppCustomData.DeviceFd, TIOCGSERIAL, &serials);

    tcflush(RCIN_AppCustomData.DeviceFd, TCIFLUSH);

    /* Create streaming task */
    /* Set loop flag to continue forever */
    RCIN_AppCustomData.ContinueFlag = TRUE;
    
        /* Create the streaming task */
    returnCode = CFE_ES_CreateChildTask(
        &RCIN_AppCustomData.ChildTaskID,
        RCIN_STREAMING_TASK_NAME,
        RCIN_AppCustomData.StreamingTask,
        0,
        CFE_ES_DEFAULT_STACK_SIZE,
        RCIN_AppCustomData.Priority,
        RCIN_STREAMING_TASK_FLAGS);
        
    if(CFE_SUCCESS != returnCode)
    {
        RCIN_AppCustomData.ContinueFlag = FALSE;
        returnBool = FALSE;
        CFE_EVS_SendEvent(RCIN_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "RCIN create streaming task failed");
        goto end_of_function;
    }
    else
    {
        RCIN_AppCustomData.Status = RCIN_CUSTOM_ENABLED;
    }

end_of_function:
    return (returnBool);
}


void RCIN_Stream_Task(void)
{
    int32 returnCode     = 0;
    int32 timeouts       = 0;
    uint32 maxFd         = 0;
    uint32 retryAttempts = 0;
    uint32 iStatus       = -1;
    fd_set fds;
    struct timeval timeValue;

    iStatus = CFE_ES_RegisterChildTask();
    
    if (iStatus == CFE_SUCCESS)
    {
        /* Task should continue forever until cleanup (uninit) */
        while (RCIN_AppCustomData.ContinueFlag == TRUE)
        {
            maxFd = 0;
            returnCode = 0;
            
            /* Select modifies the timeout value with time left until 
             * the timeout would expire so timeValue needs to be set
             * every loop iteration
             */
            timeValue.tv_sec = RCIN_BUFFER_FILL_TIMEOUT_SEC;
            timeValue.tv_usec = RCIN_BUFFER_FILL_TIMEOUT_USEC;

            /* Initialize the set */
            FD_ZERO(&fds);

            FD_SET(RCIN_AppCustomData.DeviceFd, &fds);
            
            /* Get the greatest fd value for select() */
            maxFd = RCIN_AppCustomData.DeviceFd; 

            CFE_ES_PerfLogEntry(RCIN_DEVICE_GET_PERF_ID);
            /* Wait for RC data */
            returnCode = select(maxFd + 1, &fds, 0, 0, &timeValue);
            CFE_ES_PerfLogExit(RCIN_DEVICE_GET_PERF_ID);

            /* select() wasn't successful */
            if (-1 == returnCode)
            {
                /* select was interrupted, try again */
                if (EINTR == errno)
                {
                    RCIN_Custom_RC_Lost(TRUE);
                    (void) CFE_EVS_SendEvent(RCIN_DEVICE_ERR_EID, CFE_EVS_ERROR,
                        "RCIN select was interrupted");
                    usleep(RCIN_MAX_RETRY_SLEEP_USEC);
                    continue;
                }
                else
                {
                    RCIN_Custom_RC_Lost(TRUE);
                    /* select returned an error other than EINTR */
                    (void) CFE_EVS_SendEvent(RCIN_DEVICE_ERR_EID, CFE_EVS_ERROR,
                        "RCIN stream failed select() returned %i", errno);
                    usleep(RCIN_MAX_RETRY_SLEEP_USEC);
                    continue;
                }
            }
            /* select timed out */
            if (0 == returnCode)
            {
                RCIN_Custom_RC_Lost(TRUE);
                continue;
            } 
            /* select() returned and data is ready to be read */
            if(returnCode > 0)
            {
                RCIN_Custom_RC_Lost(FALSE);
                RCIN_Custom_Read();
                continue;
            }
        } /* end while loop */
    } /* end if status == success */

    /* Take shared data mutex */
    OS_MutSemTake(RCIN_AppCustomData.Mutex);
    /* Set failsafe and RcLost */
    RCIN_AppCustomData.Measure.RcFailsafe    = 1;
    RCIN_AppCustomData.Measure.RcLost        = 1;
    /* Give shared data mutex */
    OS_MutSemGive(RCIN_AppCustomData.Mutex);

    /* Streaming task is exiting so set app flag to initialized */
    RCIN_AppCustomData.Status = RCIN_CUSTOM_ENABLED;
    CFE_EVS_SendEvent(RCIN_DEVICE_ERR_EID, CFE_EVS_ERROR,
        "RCIN receive task exited with return code %li task status (0x%08lX)",
        returnCode, iStatus);

    /* The child task was successfully registered so exit from it */
    if (iStatus == CFE_SUCCESS)
    {
        CFE_ES_ExitChildTask();
    }
}


boolean RCIN_Custom_PWM_Translate(uint8 *data, int size)
{
    boolean returnBool = TRUE;

    /* Null pointer check */
    if(0 == data)
    {
        (void) CFE_EVS_SendEvent(RCIN_DEVICE_ERR_EID, CFE_EVS_ERROR,
                    "RCIN_Custom_PWM_Translate null pointer");
        returnBool = FALSE;
        goto end_of_function;
    }

    /* Size check */
    if(RCIN_SERIAL_READ_SIZE != size)
    {
        (void) CFE_EVS_SendEvent(RCIN_DEVICE_ERR_EID, CFE_EVS_ERROR,
                    "RCIN_Custom_PWM_Translate size error");
        returnBool = FALSE;
        goto end_of_function;
    }

    /* Take shared data mutex */
    OS_MutSemTake(RCIN_AppCustomData.Mutex);

    RCIN_AppCustomData.Measure.Values[0] = (uint16)(((data[1] | data[2] << 8) 
            & 0x07FF) * RCIN_SBUS_SCALE_FACTOR + .5f) + RCIN_SBUS_SCALE_OFFSET;
    RCIN_AppCustomData.Measure.Values[1] = (uint16)(((data[2] >> 3 | data[3] << 5)
            & 0x07FF) * RCIN_SBUS_SCALE_FACTOR + .5f) + RCIN_SBUS_SCALE_OFFSET;
    RCIN_AppCustomData.Measure.Values[2] = (uint16)(((data[3] >> 6 | data[4] << 2
            | data[5] << 10) & 0x07FF) * RCIN_SBUS_SCALE_FACTOR + .5f)
            + RCIN_SBUS_SCALE_OFFSET;
    RCIN_AppCustomData.Measure.Values[3] = (uint16)(((data[5] >> 1 | data[6] << 7)
            & 0x07FF) * RCIN_SBUS_SCALE_FACTOR + .5f) + RCIN_SBUS_SCALE_OFFSET;
    RCIN_AppCustomData.Measure.Values[4] = (uint16)(((data[6] >> 4 | data[7] << 4)
            & 0x07FF) * RCIN_SBUS_SCALE_FACTOR + .5f) + RCIN_SBUS_SCALE_OFFSET;
    RCIN_AppCustomData.Measure.Values[5] = (uint16)(((data[7] >> 7 | data[8] << 1
            | data[9] << 9) & 0x07FF) * RCIN_SBUS_SCALE_FACTOR + .5f)
            + RCIN_SBUS_SCALE_OFFSET;
    RCIN_AppCustomData.Measure.Values[6] = (uint16)(((data[9] >> 2 | data[10] << 6)
            & 0x07FF) * RCIN_SBUS_SCALE_FACTOR + .5f) + RCIN_SBUS_SCALE_OFFSET;
    RCIN_AppCustomData.Measure.Values[7] = (uint16)(((data[10] >> 5 | data[11] << 3)
            & 0x07FF) * RCIN_SBUS_SCALE_FACTOR + .5f) + RCIN_SBUS_SCALE_OFFSET; // & the other 8 + 2 channels if you need them
    RCIN_AppCustomData.Measure.Values[8] = (uint16)(((data[12] | data[13] << 8)
            & 0x07FF) * RCIN_SBUS_SCALE_FACTOR + .5f) + RCIN_SBUS_SCALE_OFFSET;
    RCIN_AppCustomData.Measure.Values[9] = (uint16)(((data[13] >> 3 | data[14] << 5)
            & 0x07FF) * RCIN_SBUS_SCALE_FACTOR + .5f) + RCIN_SBUS_SCALE_OFFSET;
    RCIN_AppCustomData.Measure.Values[10] = (uint16)(((data[14] >> 6 | data[15] << 2
            | data[16] << 10) & 0x07FF) * RCIN_SBUS_SCALE_FACTOR + .5f)
            + RCIN_SBUS_SCALE_OFFSET;
    RCIN_AppCustomData.Measure.Values[11] = (uint16)(((data[16] >> 1 | data[17] << 7)
            & 0x07FF) * RCIN_SBUS_SCALE_FACTOR + .5f) + RCIN_SBUS_SCALE_OFFSET;
    RCIN_AppCustomData.Measure.Values[12] = (uint16)(((data[17] >> 4 | data[18] << 4)
            & 0x07FF) * RCIN_SBUS_SCALE_FACTOR + .5f) + RCIN_SBUS_SCALE_OFFSET;
    RCIN_AppCustomData.Measure.Values[13] = (uint16)(((data[18] >> 7 | data[19] << 1
            | data[20] << 9) & 0x07FF) * RCIN_SBUS_SCALE_FACTOR + .5f)
            + RCIN_SBUS_SCALE_OFFSET;
    RCIN_AppCustomData.Measure.Values[14] = (uint16)(((data[20] >> 2 | data[21] << 6)
            & 0x07FF) * RCIN_SBUS_SCALE_FACTOR + .5f) + RCIN_SBUS_SCALE_OFFSET;
    RCIN_AppCustomData.Measure.Values[15] = (uint16)(((data[21] >> 5 | data[22] << 3)
            & 0x07FF) * RCIN_SBUS_SCALE_FACTOR + .5f) + RCIN_SBUS_SCALE_OFFSET;

    /* Timestamp */
    RCIN_AppCustomData.Measure.Timestamp = PX4LIB_GetPX4TimeUs();

    /* Channel count */
    RCIN_AppCustomData.Measure.ChannelCount = RCIN_SBUS_CHANNEL_COUNT;
    RCIN_AppCustomData.Measure.RSSI = 100;
    RCIN_AppCustomData.Measure.RcLostFrameCount = 0;
    RCIN_AppCustomData.Measure.RcTotalFrameCount = 1;
    RCIN_AppCustomData.Measure.RcPpmFrameLength = 100;
    RCIN_AppCustomData.Measure.RcFailsafe = (data[23] & (1 << 3)) ? TRUE : FALSE;
    RCIN_AppCustomData.Measure.RcLost = (data[23] & (1 << 2)) ? TRUE : FALSE;
    RCIN_AppCustomData.Measure.InputSource = PX4_RC_INPUT_SOURCE_PX4FMU_SBUS;

    /* Give shared data mutex */
    OS_MutSemGive(RCIN_AppCustomData.Mutex);

end_of_function:
    return (returnBool);
}


void RCIN_Custom_RC_Lost(boolean notReset)
{
    static int errorCount = 0;

    /* Take shared data mutex */
    OS_MutSemTake(RCIN_AppCustomData.Mutex);

    if (FALSE == notReset)
    {
        /* Set status to streaming state */
        RCIN_AppCustomData.Status = RCIN_CUSTOM_STREAMING;
        errorCount = 0;
    }
    else
    {
        /* Set status to not streaming if we're not in RC Lost*/
        if (RCIN_AppCustomData.Status != RCIN_CUSTOM_RC_LOST 
            && RCIN_AppCustomData.Status != RCIN_CUSTOM_NOTSTREAMING)
        {
            RCIN_AppCustomData.Status = RCIN_CUSTOM_NOTSTREAMING;
        }
        errorCount++;
    }

    if(RCIN_MAX_ERROR_COUNT == errorCount)
    {
        /* We're at max error count, set failsafe and lost flags */
        RCIN_AppCustomData.Measure.RcFailsafe = TRUE;
        RCIN_AppCustomData.Measure.RcLost = TRUE;

        if (RCIN_AppCustomData.Status != RCIN_CUSTOM_RC_LOST)
        {
            CFE_EVS_SendEvent(RCIN_DEVICE_ERR_EID, CFE_EVS_ERROR,
                    "RCIN RC LOST, setting failsafe and lost flags");
            RCIN_AppCustomData.Status = RCIN_CUSTOM_RC_LOST;
        }
        errorCount = 0;
    }
    /* Give shared data mutex */
    OS_MutSemGive(RCIN_AppCustomData.Mutex);
    return;
}


void RCIN_Custom_Read(void)
{
    uint32 i            = 0;
    int bytesRead       = 0;
    uint8 sbusTemp[RCIN_SERIAL_READ_SIZE*4] = { 0 };
    static boolean sync = FALSE;

    /* Read */
    bytesRead = RCIN_Read(RCIN_AppCustomData.DeviceFd, &sbusTemp, sizeof(sbusTemp));

    if(bytesRead > 0)
    {
        for(i = 0; i < bytesRead; i++)
        {
            switch(RCIN_AppCustomData.ParserState)
            {
                case RCIN_PARSER_STATE_UNKNOWN:
                {
                    /* If header is found move to next state */
                    if(0x0f == sbusTemp[i] || 0x8f == sbusTemp[i])
                    {
                        RCIN_AppCustomData.sbusData[0] = sbusTemp[i];
                        RCIN_AppCustomData.ParserState = RCIN_PARSER_STATE_WAITING_FOR_DATA1;
                    }
                    break;
                }
                case RCIN_PARSER_STATE_WAITING_FOR_HEADER:
                {
                    /* If header is found move to next state */
                    if(0x0f == sbusTemp[i] || 0x8f == sbusTemp[i])
                    {
                        RCIN_AppCustomData.sbusData[0] = sbusTemp[i];
                        RCIN_AppCustomData.ParserState = RCIN_PARSER_STATE_WAITING_FOR_DATA1;
                    }
                    break;
                }
                case RCIN_PARSER_STATE_WAITING_FOR_DATA1:
                {
                    RCIN_AppCustomData.sbusData[1] = sbusTemp[i];
                    RCIN_AppCustomData.ParserState = RCIN_PARSER_STATE_WAITING_FOR_DATA2;
                    break;
                }
                case RCIN_PARSER_STATE_WAITING_FOR_DATA2:
                {
                    RCIN_AppCustomData.sbusData[2] = sbusTemp[i];
                    RCIN_AppCustomData.ParserState = RCIN_PARSER_STATE_WAITING_FOR_DATA3;
                    break;
                }
                case RCIN_PARSER_STATE_WAITING_FOR_DATA3:
                {
                    RCIN_AppCustomData.sbusData[3] = sbusTemp[i];
                    RCIN_AppCustomData.ParserState = RCIN_PARSER_STATE_WAITING_FOR_DATA4;
                    break;
                }
                case RCIN_PARSER_STATE_WAITING_FOR_DATA4:
                {
                    RCIN_AppCustomData.sbusData[4] = sbusTemp[i];
                    RCIN_AppCustomData.ParserState = RCIN_PARSER_STATE_WAITING_FOR_DATA5;
                    break;
                }
                case RCIN_PARSER_STATE_WAITING_FOR_DATA5:
                {
                    RCIN_AppCustomData.sbusData[5] = sbusTemp[i];
                    RCIN_AppCustomData.ParserState = RCIN_PARSER_STATE_WAITING_FOR_DATA6;
                    break;
                }
                case RCIN_PARSER_STATE_WAITING_FOR_DATA6:
                {
                    RCIN_AppCustomData.sbusData[6] = sbusTemp[i];
                    RCIN_AppCustomData.ParserState = RCIN_PARSER_STATE_WAITING_FOR_DATA7;
                    break;
                }
                case RCIN_PARSER_STATE_WAITING_FOR_DATA7:
                {
                    RCIN_AppCustomData.sbusData[7] = sbusTemp[i];
                    RCIN_AppCustomData.ParserState = RCIN_PARSER_STATE_WAITING_FOR_DATA8;
                    break;
                }
                case RCIN_PARSER_STATE_WAITING_FOR_DATA8:
                {
                    RCIN_AppCustomData.sbusData[8] = sbusTemp[i];
                    RCIN_AppCustomData.ParserState = RCIN_PARSER_STATE_WAITING_FOR_DATA9;
                    break;
                }
                case RCIN_PARSER_STATE_WAITING_FOR_DATA9:
                {
                    RCIN_AppCustomData.sbusData[9] = sbusTemp[i];
                    RCIN_AppCustomData.ParserState = RCIN_PARSER_STATE_WAITING_FOR_DATA10;
                    break;
                }
                case RCIN_PARSER_STATE_WAITING_FOR_DATA10:
                {
                    RCIN_AppCustomData.sbusData[10] = sbusTemp[i];
                    RCIN_AppCustomData.ParserState = RCIN_PARSER_STATE_WAITING_FOR_DATA11;
                    break;
                }
                case RCIN_PARSER_STATE_WAITING_FOR_DATA11:
                {
                    RCIN_AppCustomData.sbusData[11] = sbusTemp[i];
                    RCIN_AppCustomData.ParserState = RCIN_PARSER_STATE_WAITING_FOR_DATA12;
                    break;
                }
                case RCIN_PARSER_STATE_WAITING_FOR_DATA12:
                    {
                        RCIN_AppCustomData.sbusData[12] = sbusTemp[i];
                        RCIN_AppCustomData.ParserState = RCIN_PARSER_STATE_WAITING_FOR_DATA13;
                    }
                    break;
                case RCIN_PARSER_STATE_WAITING_FOR_DATA13:
                {
                    RCIN_AppCustomData.sbusData[13] = sbusTemp[i];
                    RCIN_AppCustomData.ParserState = RCIN_PARSER_STATE_WAITING_FOR_DATA14;
                    break;
                }
                case RCIN_PARSER_STATE_WAITING_FOR_DATA14:
                {
                    RCIN_AppCustomData.sbusData[14] = sbusTemp[i];
                    RCIN_AppCustomData.ParserState = RCIN_PARSER_STATE_WAITING_FOR_DATA15;
                    break;
                }
                case RCIN_PARSER_STATE_WAITING_FOR_DATA15:
                {
                    RCIN_AppCustomData.sbusData[15] = sbusTemp[i];
                    RCIN_AppCustomData.ParserState = RCIN_PARSER_STATE_WAITING_FOR_DATA16;
                    break;
                }
                case RCIN_PARSER_STATE_WAITING_FOR_DATA16:
                {
                    RCIN_AppCustomData.sbusData[16] = sbusTemp[i];
                    RCIN_AppCustomData.ParserState = RCIN_PARSER_STATE_WAITING_FOR_DATA17;
                    break;
                }
                case RCIN_PARSER_STATE_WAITING_FOR_DATA17:
                {
                    RCIN_AppCustomData.sbusData[17] = sbusTemp[i];
                    RCIN_AppCustomData.ParserState = RCIN_PARSER_STATE_WAITING_FOR_DATA18;
                    break;
                }
                case RCIN_PARSER_STATE_WAITING_FOR_DATA18:
                {
                    RCIN_AppCustomData.sbusData[18] = sbusTemp[i];
                    RCIN_AppCustomData.ParserState = RCIN_PARSER_STATE_WAITING_FOR_DATA19;
                    break;
                }
                case RCIN_PARSER_STATE_WAITING_FOR_DATA19:
                {
                    RCIN_AppCustomData.sbusData[19] = sbusTemp[i];
                    RCIN_AppCustomData.ParserState = RCIN_PARSER_STATE_WAITING_FOR_DATA20;
                    break;
                }
                case RCIN_PARSER_STATE_WAITING_FOR_DATA20:
                {
                    RCIN_AppCustomData.sbusData[20] = sbusTemp[i];
                    RCIN_AppCustomData.ParserState = RCIN_PARSER_STATE_WAITING_FOR_DATA21;
                    break;
                }
                case RCIN_PARSER_STATE_WAITING_FOR_DATA21:
                {
                    RCIN_AppCustomData.sbusData[21] = sbusTemp[i];
                    RCIN_AppCustomData.ParserState = RCIN_PARSER_STATE_WAITING_FOR_DATA22;
                    break;
                }
                case RCIN_PARSER_STATE_WAITING_FOR_DATA22:
                {
                    RCIN_AppCustomData.sbusData[22] = sbusTemp[i];
                    RCIN_AppCustomData.ParserState = RCIN_PARSER_STATE_WAITING_FOR_FLAGS;
                    break;
                }
                case RCIN_PARSER_STATE_WAITING_FOR_FLAGS:
                {
                    RCIN_AppCustomData.sbusData[23] = sbusTemp[i];
                    RCIN_AppCustomData.ParserState = RCIN_PARSER_STATE_WAITING_FOR_FOOTER;
                    break;
                }
                case RCIN_PARSER_STATE_WAITING_FOR_FOOTER:
                {
                    if(0x00 == sbusTemp[i])
                    {
                        /* Reset the error counter */
                        RCIN_Custom_RC_Lost(FALSE);
                        RCIN_AppCustomData.sbusData[24] = sbusTemp[i];
                        RCIN_AppCustomData.ParserState = RCIN_PARSER_STATE_WAITING_FOR_HEADER;
                        if (FALSE == sync)
                        {
                            sync = TRUE;
                            (void) CFE_EVS_SendEvent(RCIN_SYNC_ERR_EID, CFE_EVS_ERROR,
                                    "RCIN in sync.");
                        }
                        /* We have a valid packet, translate SBUS data */
                        RCIN_Custom_PWM_Translate(RCIN_AppCustomData.sbusData, sizeof(RCIN_AppCustomData.sbusData));
                    }
                    else
                    {
                        RCIN_Custom_RC_Lost(TRUE);
                        /* The end byte wasn't found so find a header candidate */
                        RCIN_AppCustomData.ParserState = RCIN_PARSER_STATE_WAITING_FOR_HEADER;
                        (void) CFE_EVS_SendEvent(RCIN_SYNC_ERR_EID, CFE_EVS_ERROR,
                            "RCIN out of sync.");
                        sync = FALSE;
                    }
                    break;
                }
                default:
                {
                    RCIN_Custom_RC_Lost(TRUE);
                    /* Unknown parser state */
                    (void) CFE_EVS_SendEvent(RCIN_SYNC_ERR_EID, CFE_EVS_ERROR,
                            "Parser in invalid state.");
                    RCIN_AppCustomData.ParserState = RCIN_PARSER_STATE_UNKNOWN;
                    break;
                }
            }
        }
    }
    /* Read returned an error */
    else
    {
        RCIN_Custom_RC_Lost(TRUE);
        (void) CFE_EVS_SendEvent(RCIN_DEVICE_ERR_EID, CFE_EVS_ERROR,
                "RCIN device read error, errno: %i", errno);
    }
    return;
}


boolean RCIN_Custom_Measure(PX4_InputRcMsg_t *Measure)
{
    boolean returnBool    = TRUE;
    void *userDataPtr     = 0;
    void *copyDataPtr     = 0;
    uint16 userDataLength = 0;
    uint16 i              = 0;

    /* Null pointer check */
    if (0 == Measure)
    {
        (void) CFE_EVS_SendEvent(RCIN_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "RCIN device read null pointer error");
        returnBool = FALSE;
        goto end_of_function;
    }

    /* Take the shared data mutex */
    OS_MutSemTake(RCIN_AppCustomData.Mutex);

    if(RCIN_CUSTOM_STREAMING != RCIN_AppCustomData.Status)
    {
        returnBool = FALSE;
    }
    
    if(RCIN_AppCustomData.Measure.RcFailsafe == TRUE &&
       RCIN_AppCustomData.Measure.RcLost == TRUE)
    {
        returnBool = FALSE;
    }

    Measure->Timestamp = RCIN_AppCustomData.Measure.Timestamp;
    Measure->ChannelCount = RCIN_AppCustomData.Measure.ChannelCount;
    Measure->RSSI = RCIN_AppCustomData.Measure.RSSI;
    Measure->RcLostFrameCount = RCIN_AppCustomData.Measure.RcLostFrameCount;
    Measure->RcTotalFrameCount = RCIN_AppCustomData.Measure.RcTotalFrameCount;
    Measure->RcPpmFrameLength = RCIN_AppCustomData.Measure.RcPpmFrameLength;
    for(i = 0; i<18; i++)
    {
        Measure->Values[i] = RCIN_AppCustomData.Measure.Values[i];
    }

    Measure->RcFailsafe = RCIN_AppCustomData.Measure.RcFailsafe;
    Measure->RcLost = RCIN_AppCustomData.Measure.RcLost;
    Measure->InputSource = RCIN_AppCustomData.Measure.InputSource;

    /* Give the shared data mutex */
    OS_MutSemGive(RCIN_AppCustomData.Mutex);

end_of_function:
    return (returnBool);
}


boolean RCIN_Custom_Uninit(void)
{
    boolean returnBool = TRUE;
    int returnCode     = 0;

    /* Delete the child task */
    CFE_ES_DeleteChildTask(RCIN_AppCustomData.ChildTaskID);
    /* Set streaming task loop flag to stop */
    RCIN_AppCustomData.ContinueFlag = FALSE;
    /* Set app state to initialized */
    RCIN_AppCustomData.Status = RCIN_CUSTOM_INITIALIZED;
    
    returnCode = close(RCIN_AppCustomData.DeviceFd);
    if (-1 == returnCode) 
    {
        (void) CFE_EVS_SendEvent(RCIN_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "RCIN Device close errno: %i", errno);
        returnBool = FALSE;
    }
    else
    {
        RCIN_AppCustomData.Status = RCIN_CUSTOM_UNINITIALIZED;
    }
    return (returnBool);
}


boolean RCIN_Custom_Max_Events_Not_Reached(int32 ind)
{
    boolean returnBool = FALSE;

    if ((ind < RCIN_MAX_EVENT_FILTERS) && (ind > 0))
    {
        returnBool = TRUE;
    }

    return (returnBool);
}


int32 RCIN_Custom_Init_EventFilters(int32 ind, CFE_EVS_BinFilter_t *EventTbl)
{
    int32 customEventCount = ind;
    
    /* Null check */
    if(0 == EventTbl)
    {
        customEventCount = -1;
        goto end_of_function;
    }

    if(TRUE == RCIN_Custom_Max_Events_Not_Reached(customEventCount + 2))
    {
        EventTbl[  customEventCount].EventID = RCIN_DEVICE_ERR_EID;
        EventTbl[customEventCount++].Mask    = CFE_EVS_FIRST_16_STOP;
        EventTbl[  customEventCount].EventID = RCIN_SYNC_ERR_EID;
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
