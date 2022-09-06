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
#include "ms5611_spi.h"
#include "ms5611_events.h"
#include "ms5611_perfids.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
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
/** \brief <tt> 'MS5611 - ' </tt>
**  \event <tt> 'MS5611 - ' </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when a device resource encounters an 
**  error.
**
*/
    MS5611_DEVICE_ERR_EID = MS5611_EVT_CNT,

/** \brief Number of custom events 
**
**  \par Limits:
**       int32
*/
    MS5611_CUSTOM_EVT_CNT
} MS5611_CustomEventIds_t;


/************************************************************************
** External Global Variables
*************************************************************************/

/************************************************************************
** Global Variables
*************************************************************************/
MS5611_AppCustomData_t MS5611_AppCustomData;
struct spi_ioc_transfer MS5611_SPI_Xfer[2];

/************************************************************************
** Local Variables
*************************************************************************/

/************************************************************************
** Local Function Definitions
*************************************************************************/
int32 MS5611_Ioctl(int fh, int request, void *arg)
{
    int32 returnCode = 0;
    uint32 i = 0;

    for (i=0; i < MS5611_MAX_RETRY_ATTEMPTS; i++)
    {
        returnCode = ioctl(fh, request, arg);
            
        if (-1 == returnCode && EINTR == errno)
        {
            usleep(MS5611_MAX_RETRY_SLEEP_USEC);
        }
        else
        {
            break;
        }
    }

    return (returnCode);
}


void MS5611_Custom_InitData(void)
{
    /* Set all struct zero values */
    bzero(&MS5611_AppCustomData, sizeof(MS5611_AppCustomData));
}


boolean MS5611_Custom_Init()
{
    boolean returnBool = TRUE;
    int ret            = 0;
    int i              = 0;
    int8 mode          = MS5611_SPI_DEVICE_MODE;
    int8 bits          = MS5611_SPI_DEVICE_BITS;
    uint32 speed       = MS5611_SPI_DEVICE_SPEED;

    MS5611_AppCustomData.DeviceFd = open(MS5611_SPI_DEVICE_PATH, O_RDWR);
    if (MS5611_AppCustomData.DeviceFd < 0) 
    {
        (void) CFE_EVS_SendEvent(MS5611_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "MS5611 Device open errno: %i", errno);
        returnBool = FALSE;
        goto end_of_function;
    }

    ret = MS5611_Ioctl(MS5611_AppCustomData.DeviceFd, SPI_IOC_WR_MODE, &mode);
    if (-1 == ret)
    {
        (void) CFE_EVS_SendEvent(MS5611_DEVICE_ERR_EID, CFE_EVS_ERROR,
                          "Can't set SPI mode.");
        returnBool = FALSE;
        goto end_of_function;
    }

    ret = MS5611_Ioctl(MS5611_AppCustomData.DeviceFd, SPI_IOC_RD_MODE, &mode);
    if (-1 == ret)
    {
        (void) CFE_EVS_SendEvent(MS5611_DEVICE_ERR_EID, CFE_EVS_ERROR,
                          "Can't get SPI mode.");
        returnBool = FALSE;
        goto end_of_function;
    }

    ret = MS5611_Ioctl(MS5611_AppCustomData.DeviceFd, SPI_IOC_WR_BITS_PER_WORD, &bits);
    if (-1 == ret)
    {
        (void) CFE_EVS_SendEvent(MS5611_DEVICE_ERR_EID, CFE_EVS_ERROR,
                          "Can't set bits per word.");
        returnBool = FALSE;
        goto end_of_function;
    }
    
    ret = MS5611_Ioctl(MS5611_AppCustomData.DeviceFd, SPI_IOC_RD_BITS_PER_WORD, &bits);
    if (-1 == ret)
    {
        (void) CFE_EVS_SendEvent(MS5611_DEVICE_ERR_EID, CFE_EVS_ERROR,
                          "Can't get bits per word.");
        returnBool = FALSE;
        goto end_of_function;
    }
    
    ret = MS5611_Ioctl(MS5611_AppCustomData.DeviceFd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
    if (-1 == ret)
    {
        (void) CFE_EVS_SendEvent(MS5611_DEVICE_ERR_EID, CFE_EVS_ERROR,
                          "Can't set max speed.");
        returnBool = FALSE;
        goto end_of_function;
    }

    ret = MS5611_Ioctl(MS5611_AppCustomData.DeviceFd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
    if (-1 == ret)
    {
        (void) CFE_EVS_SendEvent(MS5611_DEVICE_ERR_EID, CFE_EVS_ERROR,
                          "Can't get max speed.");
        returnBool = FALSE;
        goto end_of_function;
    }

    /* Keep CS activated */
    MS5611_SPI_Xfer[0].cs_change     = 0; 
    MS5611_SPI_Xfer[0].delay_usecs   = 0; 
    MS5611_SPI_Xfer[0].speed_hz      = speed; 
    MS5611_SPI_Xfer[0].bits_per_word = 8;

    ret = MS5611_ResetDevice();
    if (-1 == ret)
    {
        (void) CFE_EVS_SendEvent(MS5611_DEVICE_ERR_EID, CFE_EVS_ERROR,
                          "Reset device failed.");
        returnBool = FALSE;
        goto end_of_function;
    }

    MS5611_AppCustomData.Status = MS5611_CUSTOM_INITIALIZED;

end_of_function:
    return (returnBool);
}


boolean MS5611_Custom_Uninit(void)
{
    boolean returnBool = TRUE;
    int returnCode     = 0;

    returnCode = close(MS5611_AppCustomData.DeviceFd);
    if (-1 == returnCode) 
    {
        (void) CFE_EVS_SendEvent(MS5611_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "MS5611 Device close errno: %i", errno);
        returnBool = FALSE;
    }
    else
    {
        MS5611_AppCustomData.Status = MS5611_CUSTOM_UNINITIALIZED;
    }
    return (returnBool);
}


void MS5611_Critical_Cleanup(void)
{
    close(MS5611_AppCustomData.DeviceFd);
}


int32 MS5611_ResetDevice(void)
{
    int ret = 0;

    unsigned char   txBuf[2];
    unsigned char   rxBuf[2];

    memset(txBuf, 0, sizeof(txBuf));
    memset(rxBuf, 0, sizeof(rxBuf));
    txBuf[0] = MS5611_SPI_CMD_RESET;

    MS5611_SPI_Xfer[0].tx_buf = (unsigned long)txBuf;
    MS5611_SPI_Xfer[0].rx_buf = (unsigned long)rxBuf;
    MS5611_SPI_Xfer[0].len = 2;

    ret = MS5611_Ioctl(MS5611_AppCustomData.DeviceFd, SPI_IOC_MESSAGE(1), MS5611_SPI_Xfer);
    if (-1 == ret) 
    {            
        (void) CFE_EVS_SendEvent(MS5611_DEVICE_ERR_EID, CFE_EVS_ERROR,
                        "MS5611 ioctl returned %i", errno);
    }
    else
    {
        usleep(100000);
    }
    return (ret);
}


boolean MS5611_ReadPROM(uint8 Addr, uint16 *returnVal)
{
    int ret            = 0;
    uint32 i           = 0;
    boolean returnBool = TRUE;
    
    /* Null pointer check */
    if(0 == returnVal)
    {
        CFE_EVS_SendEvent(MS5611_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "MS5611 ReadProm Null Pointer");
        returnBool = FALSE;
        goto end_of_function;
    }

    unsigned char   txBuf[30];
    unsigned char   rxBuf[30];

    memset(txBuf, 0, sizeof(txBuf));
    memset(rxBuf, 0, sizeof(rxBuf));
    txBuf[0] = MS5611_SPI_CMD_PROM_READ_MASK + (Addr << MS5611_SPI_CMD_PROM_ADDR_SHIFT);

    MS5611_SPI_Xfer[0].tx_buf = (unsigned long)txBuf;
    MS5611_SPI_Xfer[0].rx_buf = (unsigned long)rxBuf;
    MS5611_SPI_Xfer[0].len = 30;

    ret = MS5611_Ioctl(MS5611_AppCustomData.DeviceFd, SPI_IOC_MESSAGE(1), MS5611_SPI_Xfer);
    if (-1 == ret) 
    {            
        (void) CFE_EVS_SendEvent(MS5611_DEVICE_ERR_EID, CFE_EVS_ERROR,
                        "MS5611 ioctl returned %i", errno);
        returnBool = FALSE;
        goto end_of_function;
    }
    *returnVal = (rxBuf[1] << 8) + rxBuf[2];
    
end_of_function:
    return (returnBool);
}


boolean MS5611_D1Conversion(void)
{
    int ret            = 0;
    int32 result       = 0;
    boolean returnBool = TRUE;

    unsigned char   txBuf[1];
    unsigned char   rxBuf[1];

    memset(txBuf, 0, sizeof(txBuf));
    memset(rxBuf, 0, sizeof(rxBuf));
    txBuf[0] = MS5611_SPI_CMD_CONVERT_D1;

    MS5611_SPI_Xfer[0].tx_buf = (unsigned long)txBuf;
    MS5611_SPI_Xfer[0].rx_buf = (unsigned long)rxBuf;
    MS5611_SPI_Xfer[0].len = 1;

    ret = MS5611_Ioctl(MS5611_AppCustomData.DeviceFd, SPI_IOC_MESSAGE(1), MS5611_SPI_Xfer);
    if (-1 == ret) 
    {            
        (void) CFE_EVS_SendEvent(MS5611_DEVICE_ERR_EID, CFE_EVS_ERROR,
                        "MS5611 ioctl returned %i", errno);
        returnBool = FALSE;
    }
    else
    {
        usleep(10000);
    }
    return (returnBool);
}


boolean MS5611_D2Conversion(void)
{
    int ret            = 0;
    int32 result       = 0;
    boolean returnBool = TRUE;

    unsigned char   txBuf[1];
    unsigned char   rxBuf[1];

    memset(txBuf, 0, sizeof(txBuf));
    memset(rxBuf, 0, sizeof(rxBuf));
    txBuf[0] = MS5611_SPI_CMD_CONVERT_D2;

    MS5611_SPI_Xfer[0].tx_buf = (unsigned long)txBuf;
    MS5611_SPI_Xfer[0].rx_buf = (unsigned long)rxBuf;
    MS5611_SPI_Xfer[0].len = 1;

    ret = MS5611_Ioctl(MS5611_AppCustomData.DeviceFd, SPI_IOC_MESSAGE(1), MS5611_SPI_Xfer);
    if (-1 == ret) 
    {            
        (void) CFE_EVS_SendEvent(MS5611_DEVICE_ERR_EID, CFE_EVS_ERROR,
                        "MS5611 ioctl returned %i", errno);
        returnBool = FALSE;
    }
    else
    {
        usleep(10000);
    }
    return (returnBool);
}


boolean MS5611_ReadADCResult(uint32 *returnVal)
{
    int ret            = 0;
    int32 result       = 0;
    uint32 i           = 0;
    boolean returnBool = TRUE;
    
    unsigned char   txBuf[30];
    unsigned char   rxBuf[30];

    /* Null pointer check */
    if(0 == returnVal)
    {
        (void) CFE_EVS_SendEvent(MS5611_DEVICE_ERR_EID, CFE_EVS_ERROR,
                "MS5611 ReadProm Null Pointer");
        returnBool = FALSE;
        goto end_of_function;
    }

    memset(txBuf, 0, sizeof(txBuf));
    memset(rxBuf, 0, sizeof(rxBuf));
    txBuf[0] = MS5611_SPI_CMD_ADC_READ;

    MS5611_SPI_Xfer[0].tx_buf = (unsigned long)txBuf;
    MS5611_SPI_Xfer[0].rx_buf = (unsigned long)rxBuf;
    MS5611_SPI_Xfer[0].len = 30;

    ret = MS5611_Ioctl(MS5611_AppCustomData.DeviceFd, SPI_IOC_MESSAGE(1), MS5611_SPI_Xfer);
    if (-1 == ret) 
    {            
        (void) CFE_EVS_SendEvent(MS5611_DEVICE_ERR_EID, CFE_EVS_ERROR,
                        "MS5611 ioctl returned %i", errno);
        returnBool = FALSE;
        goto end_of_function;
    }
    *returnVal = (rxBuf[1] << 16) + (rxBuf[2] << 8) + rxBuf[3];

end_of_function:

    return (returnBool);
}


boolean MS5611_Custom_Max_Events_Not_Reached(int32 ind)
{
    boolean returnBool = FALSE;

    if ((ind < CFE_EVS_MAX_EVENT_FILTERS) && (ind > 0))
    {
        returnBool = TRUE;
    }

    return (returnBool);
}


int32 MS5611_Custom_Init_EventFilters(int32 ind, CFE_EVS_BinFilter_t *EventTbl)
{
    int32 customEventCount = ind;
    
    /* Null check */
    if(0 == EventTbl)
    {
        customEventCount = -1;
        goto end_of_function;
    }

    if(TRUE == MS5611_Custom_Max_Events_Not_Reached(customEventCount))
    {
        EventTbl[  customEventCount].EventID = MS5611_DEVICE_ERR_EID;
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

