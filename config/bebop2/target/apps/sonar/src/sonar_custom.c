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
#include "sonar_bebop.h"
#include "sonar_events.h"
#include "sonar_perfids.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/************************************************************************
** Local Defines
*************************************************************************/

/************************************************************************
** Local Structure Declarations
*************************************************************************/
typedef enum 
{
/** \brief <tt> 'SONAR - ' </tt>
**  \event <tt> 'SONAR - ' </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when a device resource encounters an 
**  error.
**
*/
    SONAR_DEVICE_ERR_EID = SONAR_EVT_CNT,

/** \brief Number of custom events 
**
**  \par Limits:
**       int32
*/
    SONAR_CUSTOM_EVT_CNT
} SONAR_CustomEventIds_t;

/************************************************************************
** External Global Variables
*************************************************************************/

/************************************************************************
** Global Variables
*************************************************************************/
SONAR_AppCustomData_t SONAR_AppCustomData;
struct spi_ioc_transfer SONAR_SPI_Xfer[2];

/************************************************************************
** Local Variables
*************************************************************************/

/************************************************************************
** Local Function Definitions
*************************************************************************/
int32 SONAR_Ioctl(int fh, int request, void *arg)
{
    int32 returnCode = 0;
    uint32 i = 0;

    for (i=0; i < SONAR_MAX_RETRY_ATTEMPTS; i++)
    {
        returnCode = ioctl(fh, request, arg);
            
        if (-1 == returnCode && EINTR == errno)
        {
            usleep(SONAR_MAX_RETRY_SLEEP_USEC);
        }
        else
        {
            break;
        }
    }

    return (returnCode);
}


void SONAR_Custom_InitData(void)
{
    /* Set all struct zero values */
    bzero(&SONAR_AppCustomData, sizeof(SONAR_AppCustomData));
    /* Set the device path. */
    snprintf(SONAR_AppCustomData.ADCDevicePath, sizeof(SONAR_AppCustomData.ADCDevicePath), SONAR_ADC_DEVICE_PATH);
    /* Generate the pulse to send via SPI emitter. */
    memset(SONAR_AppCustomData.SendPulse, 0xF0, (SONAR_PULSE_LEN / 2));
    
    /* Keep CS activated */
    SONAR_SPI_Xfer[0].cs_change     = 0; 
    SONAR_SPI_Xfer[0].delay_usecs   = 0; 
    SONAR_SPI_Xfer[0].speed_hz      = SONAR_SPI_DEVICE_SPEED; 
    SONAR_SPI_Xfer[0].bits_per_word = 8;

    return;
}


boolean SONAR_Custom_Init(void)
{
    boolean returnBool = TRUE;
    int returnVal      = 0;
    int8 mode          = SONAR_SPI_DEVICE_MODE;
    int8 bits          = SONAR_SPI_DEVICE_BITS;
    uint32 speed       = SONAR_SPI_DEVICE_SPEED;

    /* Initialize custom data. */
    SONAR_Custom_InitData();

    /* Initialize ADC (for sampling reflected wave). */
    returnVal = SONAR_ADC_Init();
    if(returnVal < 0)
    {
        (void) CFE_EVS_SendEvent(SONAR_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "Sonar ADC init failed.");
        returnBool = FALSE;
        goto end_of_function;
    }

    /* Open SPI (for generating pulses). */
    SONAR_AppCustomData.DeviceFd = open(SONAR_SPI_DEVICE_PATH, O_RDWR);
    if (SONAR_AppCustomData.DeviceFd < 0) 
    {
        (void) CFE_EVS_SendEvent(SONAR_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "Device open errno: %i", errno);
        returnBool = FALSE;
        goto end_of_function;
    }

    /* Setup for SPI. */
    returnVal = SONAR_Ioctl(SONAR_AppCustomData.DeviceFd, SPI_IOC_WR_MODE, &mode);
    if (-1 == returnVal)
    {
        (void) CFE_EVS_SendEvent(SONAR_DEVICE_ERR_EID, CFE_EVS_ERROR,
                          "Can't set SPI mode.");
        returnBool = FALSE;
        goto end_of_function;
    }

    returnVal = SONAR_Ioctl(SONAR_AppCustomData.DeviceFd, SPI_IOC_RD_MODE, &mode);
    if (-1 == returnVal)
    {
        (void) CFE_EVS_SendEvent(SONAR_DEVICE_ERR_EID, CFE_EVS_ERROR,
                          "Can't get SPI mode.");
        returnBool = FALSE;
        goto end_of_function;
    }

    returnVal = SONAR_Ioctl(SONAR_AppCustomData.DeviceFd, SPI_IOC_WR_BITS_PER_WORD, &bits);
    if (-1 == returnVal)
    {
        (void) CFE_EVS_SendEvent(SONAR_DEVICE_ERR_EID, CFE_EVS_ERROR,
                          "Can't set bits per word.");
        returnBool = FALSE;
        goto end_of_function;
    }
    
    returnVal = SONAR_Ioctl(SONAR_AppCustomData.DeviceFd, SPI_IOC_RD_BITS_PER_WORD, &bits);
    if (-1 == returnVal)
    {
        (void) CFE_EVS_SendEvent(SONAR_DEVICE_ERR_EID, CFE_EVS_ERROR,
                          "Can't get bits per word.");
        returnBool = FALSE;
        goto end_of_function;
    }

    returnVal = SONAR_Ioctl(SONAR_AppCustomData.DeviceFd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
    if (-1 == returnVal)
    {
        (void) CFE_EVS_SendEvent(SONAR_DEVICE_ERR_EID, CFE_EVS_ERROR,
                          "Can't set max speed.");
        returnBool = FALSE;
        goto end_of_function;
    }

    returnVal = SONAR_Ioctl(SONAR_AppCustomData.DeviceFd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
    if (-1 == returnVal)
    {
        (void) CFE_EVS_SendEvent(SONAR_DEVICE_ERR_EID, CFE_EVS_ERROR,
                          "Can't get max speed.");
        returnBool = FALSE;
        goto end_of_function;
    }

    SONAR_AppCustomData.Status = SONAR_CUSTOM_INITIALIZED;

end_of_function:
    return (returnBool);
}


boolean SONAR_Custom_Uninit(void)
{
    boolean returnBool = TRUE;
    int returnCode     = 0;

    returnCode = close(SONAR_AppCustomData.DeviceFd);
    if (-1 == returnCode) 
    {
        (void) CFE_EVS_SendEvent(SONAR_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "SONAR Device close errno: %i.", errno);
        returnBool = FALSE;
        goto end_of_function;
    }

    returnCode = SONAR_ADC_Disable();
    if (returnCode < 0) 
    {
        (void) CFE_EVS_SendEvent(SONAR_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "SONAR ADC Disable failed.");
        returnBool = FALSE;
        goto end_of_function;
    }

    SONAR_AppCustomData.Status = SONAR_CUSTOM_UNINITIALIZED;

end_of_function:

    return (returnBool);
}


void SONAR_Critical_Cleanup(void)
{
    (void) close(SONAR_AppCustomData.DeviceFd);
    (void) SONAR_ADC_Disable();
}


int SONAR_ADC_Init(void)
{
    int returnVal = -1;
    /* Before we setup the device, disable the pin (prevent resource busy error). */
    returnVal = SONAR_ADC_Disable();
    if(returnVal < 0)
    {
        goto end_of_function;
    }

    /* Enable the channel. */
    returnVal = SONAR_ADC_Write(SONAR_ADC_CHANNEL, 1);
    if(returnVal < 0)
    {
        goto end_of_function;
    }

    /* Set buffer length and disable it initially. */
    returnVal = SONAR_ADC_Write("/buffer/length", SONAR_BUFFER_LEN);
    if(returnVal < 0)
    {
        goto end_of_function;
    }

end_of_function:
    return returnVal;
}


int SONAR_ADC_Enable(void)
{
    SONAR_AppCustomData.BufferEnabled = TRUE;

    return SONAR_ADC_Write("buffer/enable", 1);
}


int SONAR_ADC_Disable(void)
{
    SONAR_AppCustomData.BufferEnabled = FALSE;

    return SONAR_ADC_Write("buffer/enable", 0);
}


int SONAR_ADC_Read(uint16 *buffer, uint16 length)
{
    int returnVal = -1;
    FILE *file;
    char path[20];

    if(FALSE == SONAR_AppCustomData.BufferEnabled)
    {
        goto end_of_function;
    }

    if(0 == buffer)
    {
        goto end_of_function;
    }

    snprintf(path, sizeof(path), "/dev/iio:device0");

    file = fopen(path, "r");

    if(0 == file)
    {
        goto end_of_function;
    }

    returnVal = fread(buffer, 2, length, file);
    if(returnVal < 0)
    {
        goto end_of_function;
    }

    returnVal = fclose(file);
    if(returnVal < 0)
    {
        goto end_of_function;
    }

end_of_function:
    return returnVal;
}


int SONAR_ADC_Write(const char *path, int value)
{
    int returnVal = -1;
    char filename[sizeof(SONAR_AppCustomData.ADCDevicePath) + 20] = {0};
    FILE *file = 0;

    snprintf(filename, sizeof(filename), "%s/%s", SONAR_AppCustomData.ADCDevicePath, path);

    file = fopen(filename, "w");
    if(0 == file)
    {
        goto end_of_function;
    }

    fprintf(file, "%d", value);

    returnVal = fclose(file);
    if(returnVal < 0)
    {
        goto end_of_function;
    }

end_of_function:
    return returnVal;
}


int SONAR_Custom_Emit_Pulse(void)
{
    int ret = 0;
    uint8 buffer[SONAR_PULSE_LEN + 1] = {0};
    /* First byte is address (0x0). */
    memcpy((void*) &buffer[1], (void*) &SONAR_AppCustomData.SendPulse[0], SONAR_PULSE_LEN);

    SONAR_SPI_Xfer[0].tx_buf = (unsigned long)&buffer[0];
    SONAR_SPI_Xfer[0].rx_buf = 0;
    SONAR_SPI_Xfer[0].len = SONAR_PULSE_LEN + 1;

    if(SONAR_ADC_Enable() >= 0)
    {
        ret = SONAR_Ioctl(SONAR_AppCustomData.DeviceFd, SPI_IOC_MESSAGE(1), SONAR_SPI_Xfer);
        if (-1 == ret) 
        {            
            (void) CFE_EVS_SendEvent(SONAR_DEVICE_ERR_EID, CFE_EVS_ERROR,
                            "Emit pulse ioctl returned %i", errno);
        }
    }

    return ret;
}


int SONAR_Custom_Read_Reflected_Wave(void)
{
    int ret = 0;

    ret = SONAR_ADC_Read(&SONAR_AppCustomData.ReadBuffer[0], SONAR_BUFFER_LEN);
    if (ret < 0) 
    {            
        (void) CFE_EVS_SendEvent(SONAR_DEVICE_ERR_EID, CFE_EVS_ERROR,
                            "Read reflected wave failed.");
        goto end_of_function;
    }

    ret = SONAR_ADC_Disable();
    if (ret < 0) 
    {            
        (void) CFE_EVS_SendEvent(SONAR_DEVICE_ERR_EID, CFE_EVS_ERROR,
                            "ADC disable failed in read reflected wave.");
        goto end_of_function;
    }

end_of_function:
    return ret;
}


int16 SONAR_Find_End_Of_Send(void)
{
    boolean peak = FALSE;
    uint16 start_index = 0;
    uint16 end_index = 0;
    unsigned int i = 0;
    uint16 value = 0;

    for (i = 0; i < SONAR_BUFFER_LEN; ++i)
    {
        value = SONAR_AppCustomData.ReadBuffer[i] >> 4;

        if (!peak && value > SONAR_REQUEST_THRESHOLD) 
        {
            start_index = i;
            peak = TRUE;
        } else if (peak && value < (SONAR_NOISE_LEVEL_THRESHOLD)) 
        {
            end_index = i;
            break;
        }
    }

    SONAR_AppCustomData.SendLength = end_index - start_index;

    if (SONAR_AppCustomData.SendLength > SONAR_SEND_PULSE_LEN) 
    {
        /* Bellow block distance. */
        //printf("Below block distance.\n");
        return -1;
    } 
    else 
    {
        return end_index;
    }
}


int16 SONAR_Filter_Read_Buffer(void)
{
    memset(SONAR_AppCustomData.FilteredBuffer, 0, SONAR_BUFFER_LEN);
    SONAR_AppCustomData.MaximumSignalVal = 0;
    unsigned int i = 0;

    /* get index where the send pulse ends (end-of-send = eos) */
    int16 eos = SONAR_Find_End_Of_Send(); 

    if (eos < 0) 
    {
        return -1;
    }

    /* Perform a rolling mean filter with size 3
     * Raw values on the Bebop's iio have the format (16 >> 4) 
     */

    /* Set the values at the left edge */
    SONAR_AppCustomData.FilteredBuffer[0] = SONAR_AppCustomData.ReadBuffer[eos] >> 4;
    SONAR_AppCustomData.FilteredBuffer[1] = SONAR_AppCustomData.ReadBuffer[eos + 1] >> 4;

    uint16 running_sum = SONAR_AppCustomData.FilteredBuffer[0] + 
                         SONAR_AppCustomData.FilteredBuffer[1] +
                        (SONAR_AppCustomData.ReadBuffer[eos + 2] >> 4);

    /* Mean filter the read signal, but exclude the pulse recorded during send */
    for (i = 2; i < SONAR_BUFFER_LEN - eos - 1; ++i) 
    {
        SONAR_AppCustomData.FilteredBuffer[i] = running_sum / 3;
        running_sum = (running_sum + (SONAR_AppCustomData.ReadBuffer[eos + i + 1] >> 4)
                    - (SONAR_AppCustomData.ReadBuffer[eos + i - 2] >> 4));

        /* Capture the maximum value in the signal */
        if (SONAR_AppCustomData.FilteredBuffer[i] > SONAR_AppCustomData.MaximumSignalVal) 
        {
            SONAR_AppCustomData.MaximumSignalVal = SONAR_AppCustomData.FilteredBuffer[i];
        }
    }

    if (SONAR_AppCustomData.MaximumSignalVal < SONAR_NOISE_LEVEL_THRESHOLD) 
    {
        /* No peak found. */
        //printf("No peak found.\n");
        return -1;
    }

    return 0;
}


int16 SONAR_Get_Echo_Index(void)
{
    unsigned int i = 0;

    if (SONAR_Filter_Read_Buffer() < 0) 
    {
        return -1;
    }

    /* threshold is 4/5 * m_maximum_signal_value */
    uint16 threshold = SONAR_AppCustomData.MaximumSignalVal - (SONAR_AppCustomData.MaximumSignalVal / 5);
    boolean peak = FALSE;
    boolean max_peak_found = FALSE;
    uint16 start_index = 0;

    /* search the filtered signal for the rising edge of the peak, which also
     * includes the maximum value (strongest reflection)
     */
    for (i = 0; i < SONAR_BUFFER_LEN; ++i) 
    {
        if (!peak && SONAR_AppCustomData.FilteredBuffer[i] > (threshold)) 
        {
            peak = TRUE;
            start_index = i;

        }
        else if (peak && SONAR_AppCustomData.FilteredBuffer[i] < threshold)
        {
            peak = FALSE;

            if (max_peak_found) 
            {
                /* return the index of the rising edge and add the length that we cut
                 * at the beginning of the signal to exclude the send peak
                 */
                return (start_index + SONAR_AppCustomData.SendLength);
            }

        } 
        else if (peak && SONAR_AppCustomData.FilteredBuffer[i] >= SONAR_AppCustomData.MaximumSignalVal)
        {
            max_peak_found = TRUE;
        }
    }

    /* No peak found. */
    return -1;
}


boolean SONAR_Custom_Measure_Distance(float *distance)
{
    int16 echo = 0;
    float height = 0.0f;
    float index = 0.0f;
    boolean returnBool = FALSE;

    if(0 == distance)
    {
        goto end_of_function;
    }

    if(SONAR_Custom_Emit_Pulse() >= 0)
    {

        usleep(SONAR_MEASURE_INTERVAL_US);

        if( SONAR_Custom_Read_Reflected_Wave() >= 0)
        {
            echo = SONAR_Get_Echo_Index();
            if(echo >= 0)
            {
                index = (float) echo;
                *distance = (index * SONAR_SPEED_OF_SOUND) / (2.0f * SONAR_ADC_SAMPLING_FREZ_HZ);
                returnBool = TRUE;
                //printf("Distance %f m\n", *distance);
            }
        }
        else
        {
            (void) CFE_EVS_SendEvent(SONAR_DEVICE_ERR_EID, CFE_EVS_ERROR,
                "Read reflected wave failed in custom measure.");
        }
    }
    else
    {
        (void) CFE_EVS_SendEvent(SONAR_DEVICE_ERR_EID, CFE_EVS_ERROR,
                "Emit pulse failed in custom measure.");
    }

end_of_function:

    return returnBool;
}


boolean SONAR_Custom_Max_Events_Not_Reached(int32 ind)
{
    boolean returnBool = FALSE;

    if ((ind < CFE_EVS_MAX_EVENT_FILTERS) && (ind > 0))
    {
        returnBool = TRUE;
    }

    return (returnBool);
}


int32 SONAR_Custom_Init_EventFilters(int32 ind, CFE_EVS_BinFilter_t *EventTbl)
{
    int32 customEventCount = ind;
    
    /* Null check */
    if(0 == EventTbl)
    {
        customEventCount = -1;
        goto end_of_function;
    }

    if(TRUE == SONAR_Custom_Max_Events_Not_Reached(customEventCount))
    {
        EventTbl[  customEventCount].EventID = SONAR_DEVICE_ERR_EID;
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
