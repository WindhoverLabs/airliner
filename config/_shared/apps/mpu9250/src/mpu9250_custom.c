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
#include "mpu9250_spi.h"
#include "mpu9250_events.h"
#include "mpu9250_perfids.h"
#include "px4lib.h"


#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
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

/** \brief <tt> 'MPU9250 - ' </tt>
**  \event <tt> 'MPU9250 - ' </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when a device resource encounters an 
**  error.
**
*/
    MPU9250_DEVICE_ERR_EID = MPU9250_EVT_CNT,

/** \brief <tt> 'MPU9250 - ' </tt>
**  \event <tt> 'MPU9250 - ' </tt>
**  
**  \par Type: Info
**
**  \par Cause:
**
**  This event message is issued when a device successfully complete a
**  self test.
**
*/
    MPU9250_DEVICE_INF_EID,
/** \brief Number of custom events 
**
**  \par Limits:
**       int32
*/
    MPU9250_CUSTOM_EVT_CNT
} MPU9250_CustomEventIds_t;


/************************************************************************
** External Global Variables
*************************************************************************/

/************************************************************************
** Global Variables
*************************************************************************/
MPU9250_AppCustomData_t MPU9250_AppCustomData;
struct spi_ioc_transfer MPU9250_SPI_Xfer[2];

/************************************************************************
** Local Variables
*************************************************************************/

/************************************************************************
** Local Function Definitions
*************************************************************************/
int32 MPU9250_Ioctl(int fh, int request, void *arg)
{
    int32 returnCode = 0;
    uint32 i = 0;

    for (i=0; i < MPU9250_MAX_RETRY_ATTEMPTS; i++)
    {
        returnCode = ioctl(fh, request, arg);
            
        if (-1 == returnCode && EINTR == errno)
        {
            usleep(MPU9250_MAX_RETRY_SLEEP_USEC);
        }
        else
        {
            break;
        }
    }

    return returnCode;
}


void MPU9250_Custom_InitData(void)
{
    /* Set all struct zero values */
    bzero(&MPU9250_AppCustomData, sizeof(MPU9250_AppCustomData));
}


boolean MPU9250_Custom_Init()
{
    boolean returnBool = TRUE;
    int ret = 0;
    int i = 0;
    int8 mode = MPU9250_SPI_DEVICE_MODE;
    int8 bits = MPU9250_SPI_DEVICE_BITS;
    uint32 speed = MPU9250_SPI_DEVICE_SPEED;

    MPU9250_AppCustomData.DeviceFd = open(MPU9250_SPI_DEVICE_PATH, O_RDWR);
    if (MPU9250_AppCustomData.DeviceFd < 0) 
    {
        CFE_EVS_SendEvent(MPU9250_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "MPU9250 Device open errno: %i", errno);
        returnBool = FALSE;
        goto end_of_function;
    }

    ret = MPU9250_Ioctl(MPU9250_AppCustomData.DeviceFd, SPI_IOC_WR_MODE, &mode);
    if (-1 == ret)
    {
        CFE_EVS_SendEvent(MPU9250_DEVICE_ERR_EID, CFE_EVS_ERROR,
                          "Can't set SPI mode.");
        returnBool = FALSE;
        goto end_of_function;
    }

    ret = MPU9250_Ioctl(MPU9250_AppCustomData.DeviceFd, SPI_IOC_RD_MODE, &mode);
    if (-1 == ret)
    {
        CFE_EVS_SendEvent(MPU9250_DEVICE_ERR_EID, CFE_EVS_ERROR,
                          "Can't get SPI mode.");
        returnBool = FALSE;
        goto end_of_function;
    }

    ret = MPU9250_Ioctl(MPU9250_AppCustomData.DeviceFd, SPI_IOC_WR_BITS_PER_WORD, &bits);
    if (-1 == ret)
    {
        CFE_EVS_SendEvent(MPU9250_DEVICE_ERR_EID, CFE_EVS_ERROR,
                          "Can't set bits per word.");
        returnBool = FALSE;
        goto end_of_function;
    }
    
    ret = MPU9250_Ioctl(MPU9250_AppCustomData.DeviceFd, SPI_IOC_RD_BITS_PER_WORD, &bits);
    if (-1 == ret)
    {
        CFE_EVS_SendEvent(MPU9250_DEVICE_ERR_EID, CFE_EVS_ERROR,
                          "Can't get bits per word.");
        returnBool = FALSE;
        goto end_of_function;
    }
    
    ret = MPU9250_Ioctl(MPU9250_AppCustomData.DeviceFd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
    if (-1 == ret)
    {
        CFE_EVS_SendEvent(MPU9250_DEVICE_ERR_EID, CFE_EVS_ERROR,
                          "Can't set max speed.");
        returnBool = FALSE;
        goto end_of_function;
    }

    ret = MPU9250_Ioctl(MPU9250_AppCustomData.DeviceFd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
    if (-1 == ret)
    {
        CFE_EVS_SendEvent(MPU9250_DEVICE_ERR_EID, CFE_EVS_ERROR,
                          "Can't get max speed.");
        returnBool = FALSE;
        goto end_of_function;
    }

    /* Keep CS activated */
    MPU9250_SPI_Xfer[0].cs_change = 0; 
    MPU9250_SPI_Xfer[0].delay_usecs = 0; 
    MPU9250_SPI_Xfer[0].speed_hz = speed; 
    MPU9250_SPI_Xfer[0].bits_per_word = 8;

    ret = MPU9250_ResetDevice();
    if (-1 == ret)
    {
        CFE_EVS_SendEvent(MPU9250_DEVICE_ERR_EID, CFE_EVS_ERROR,
                          "Reset device failed.");
        returnBool = FALSE;
        goto end_of_function;
    }

    /* TODO Add Gyroscope Self-Test */
    /* TODO Add Accelerometer Self-Test */

    uint8 MPU_Init_Data[MPU_InitRegNum][2] = {
        /* Reset Device */
        {0x80, MPU9250_REG_PWR_MGMT_1},
        /* Clock Source */
        {0x01, MPU9250_REG_PWR_MGMT_1},
        /* Enable Acc & Gyro */
        {0x00, MPU9250_REG_PWR_MGMT_2},
        /* Use DLPF set Gyroscope bandwidth 250Hz, temperature bandwidth 4000Hz */
        {MPU9250_DEFAULT_LOWPASS_FILTER, MPU9250_REG_CONFIG},
        /* +-2000dps */
        {0x18, MPU9250_REG_GYRO_CONFIG},
        /* +-4G */
        {0x08, MPU9250_REG_ACCEL_CONFIG},
        /* Set Acc Data Rates, Enable Acc LPF , Bandwidth 184Hz */
        {0x09, MPU9250_REG_ACCEL_CONFIG2},
        /* LATCH_INT_EN INT pin level held unitl interrupt status is 
         * cleared. IN_ANYRD_2CLEAR interrupt status is cleared if
         * any read operation is performed. */
        {0x30, MPU9250_REG_INT_PIN_CFG},
        /* I2C Speed 348 kHz */
        //{0x40, MPUREG_I2C_MST_CTRL},
        /* Enable AUX */
        //{0x20, MPUREG_USER_CTRL},
        /* I2C Master mode */
        {0x20, MPU9250_REG_USER_CTRL},
        /* I2C 400KHz */
        {MPU9250_I2C_MST_P_NSR | MPU9250_I2C_MST_WAIT_FOR_ES |
                MPU9250_I2C_MST_CLOCK_400HZ, MPU9250_REG_I2C_MST_CTRL},
        //{0x0D, MPU9250_REG_I2C_MST_CTRL},
        /* Set the I2C slave addres of AK8963 and set for write. */
        {MPU9250_AK8963_I2C_ADDR, MPU9250_REG_I2C_SLV0_ADDR},
        //{0x09, MPUREG_I2C_SLV4_CTRL},
        /* Enable I2C delay */
        //{0x81, MPUREG_I2C_MST_DELAY_CTRL},
        /* I2C slave 0 register address from where to begin data transfer */
        {MPU9250_AK8963_CNTL2, MPU9250_REG_I2C_SLV0_REG},
        /* Reset AK8963 */
        {0x01, MPU9250_REG_I2C_SLV0_DO},
        /* Enable I2C and set 1 byte */
        {0x81, MPU9250_REG_I2C_SLV0_CTRL},
        /* I2C slave 0 register address from where to begin data transfer */
        {MPU9250_AK8963_CNTL1, MPU9250_REG_I2C_SLV0_REG},
        /* Fused ROM read mode */
        //{MPU9250_AK8963_FUSE_MODE | MPU9250_AK8963_16BIT_ADC,  MPU9250_REG_I2C_SLV0_DO},
        /* Register value to continuous measurement mode 1 in 16bit */
        //{0x12, MPU9250_REG_I2C_SLV0_DO},
        /* Register value to continuous measurement mode 2 in 16bit */
        {0x16, MPU9250_REG_I2C_SLV0_DO},
        /* Enable I2C and set 1 byte */
        {0x81, MPU9250_REG_I2C_SLV0_CTRL},
        /* Set the I2C slave address of AK8963 and set for read. */
        {MPU9250_AK8963_I2C_ADDR | 0x80, MPU9250_REG_I2C_SLV0_ADDR},
        /* I2C slave 0 register address from where to begin data transfer */
        {MPU9250_AK8963_WIA, MPU9250_REG_I2C_SLV0_REG},
        //{MPU9250_AK8963_ASAX, MPU9250_REG_I2C_SLV0_REG},
        /* Enable I2C and set 1 byte */
        {0x8f, MPU9250_REG_I2C_SLV0_CTRL}
        //{0x83, MPU9250_REG_I2C_SLV0_CTRL}
    };

    for(i = 0; i < MPU_InitRegNum; i++) 
    {
        returnBool = MPU9250_WriteReg(MPU_Init_Data[i][1], MPU_Init_Data[i][0]);
        if(FALSE == returnBool)
        {
            goto end_of_function;
        }
        usleep(100000);
    }
    
    //returnBool = MPU9250_Custom_Read_MagAdj();
    //if(FALSE == returnBool)
    //{
        //goto end_of_function;
    //}

    //uint8 MPU_Init_Data2[MPU_InitRegNum2][2] = {
        ///* Set the I2C slave address of AK8963 and set for write. */
        //{MPU9250_AK8963_I2C_ADDR, MPU9250_REG_I2C_SLV0_ADDR},
        ///* I2C slave 0 register address from where to begin data transfer */
        //{MPU9250_AK8963_CNTL2, MPU9250_REG_I2C_SLV0_REG},
        ///* Reset AK8963 */
        //{0x01, MPU9250_REG_I2C_SLV0_DO},
        ///* Enable I2C and set 1 byte */
        //{0x81, MPU9250_REG_I2C_SLV0_CTRL},
        ///* I2C slave 0 register address from where to begin data transfer */
        //{MPU9250_AK8963_CNTL1, MPU9250_REG_I2C_SLV0_REG},
        ///* Register value to continuous measurement mode 2 in 16bit */
        //{0x16, MPU9250_REG_I2C_SLV0_DO},
        ///* Enable I2C and set 1 byte */
        //{0x81, MPU9250_REG_I2C_SLV0_CTRL},
        ///* Set the I2C slave address of AK8963 and set for read. */
        //{MPU9250_AK8963_I2C_ADDR | 0x80, MPU9250_REG_I2C_SLV0_ADDR},
        ///* I2C slave 0 register address from where to begin data transfer */
        //{MPU9250_AK8963_WIA, MPU9250_REG_I2C_SLV0_REG},
        ///* Enable I2C and set 1 byte, copy the max 15 reg from mag */
        //{0x8f, MPU9250_REG_I2C_SLV0_CTRL}
    //};

    //for(i = 0; i < MPU_InitRegNum2; i++) {
        //returnBool = MPU9250_WriteReg(MPU_Init_Data2[i][1], MPU_Init_Data2[i][0]);
        //if(FALSE == returnBool)
        //{
            //goto end_of_function;
        //}
        //usleep(100000);
    //}

    MPU9250_AppCustomData.Status = MPU9250_CUSTOM_INITIALIZED;

end_of_function:
    return returnBool;
}


boolean MPU9250_Custom_Uninit(void)
{
    boolean returnBool = TRUE;
    int returnCode = 0;

    returnCode = close(MPU9250_AppCustomData.DeviceFd);
    if (-1 == returnCode) 
    {
        CFE_EVS_SendEvent(MPU9250_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "MPU9250 Device close errno: %i", errno);
        returnBool = FALSE;
    }
    else
    {
        MPU9250_AppCustomData.Status = MPU9250_CUSTOM_UNINITIALIZED;
    }
    return returnBool;
}


int32 MPU9250_ResetDevice(void)
{
    int32 ret = 0;
    boolean returnBool = TRUE;
    
    returnBool = MPU9250_WriteReg(MPU9250_BIT_H_RESET, MPU9250_REG_PWR_MGMT_1);
    if (FALSE == returnBool) 
    {            
        CFE_EVS_SendEvent(MPU9250_DEVICE_ERR_EID, CFE_EVS_ERROR,
                        "MPU9250 ioctl returned %i", errno);
        ret = -1;
    }
    else
    {
        usleep(100000);
    }
    return ret;
}


boolean MPU9250_WriteReg(uint8 Addr, uint8 Data)
{
    int ret;
    unsigned char   buf[32];
    boolean returnBool = TRUE;

    memset(buf, 0, sizeof(buf));
    buf[0] = Addr;
    buf[1] = Data;

    MPU9250_SPI_Xfer[0].tx_buf = (unsigned long)buf;
    MPU9250_SPI_Xfer[0].rx_buf = (unsigned long)buf;
    MPU9250_SPI_Xfer[0].len = 2;

    ret = ioctl(MPU9250_AppCustomData.DeviceFd, SPI_IOC_MESSAGE(1), MPU9250_SPI_Xfer);
    if (-1 == ret) 
    {            
        CFE_EVS_SendEvent(MPU9250_DEVICE_ERR_EID, CFE_EVS_ERROR,
                        "MPU9250 ioctl returned %i", errno);
        returnBool = FALSE;
    }
    return returnBool;
}


boolean MPU9250_ReadReg(uint8 Addr, uint8 *returnVal)
{
    int ret = 0;
    unsigned char   buf[32];
    boolean returnBool = TRUE;

    /* Null pointer check */
    if(0 == returnVal)
    {
        CFE_EVS_SendEvent(MPU9250_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "MPU9250 ReadReg Null Pointer");
        returnBool = FALSE;
        goto end_of_function;
    }

    memset(buf, 0, sizeof(buf));
    buf[0] = Addr | 0x80;
    buf[1] = 0;

    MPU9250_SPI_Xfer[0].tx_buf = (unsigned long)buf;
    MPU9250_SPI_Xfer[0].rx_buf = (unsigned long)buf;
    MPU9250_SPI_Xfer[0].len = 2;

    ret = MPU9250_Ioctl(MPU9250_AppCustomData.DeviceFd, SPI_IOC_MESSAGE(1), MPU9250_SPI_Xfer);
    if (-1 == ret) 
    {            
        CFE_EVS_SendEvent(MPU9250_DEVICE_ERR_EID, CFE_EVS_ERROR,
                        "MPU9250 ioctl returned %i", errno);
    }

    *returnVal = (uint8) buf[1];

end_of_function:
    return returnBool;
}


boolean MPU9250_SetAccScale(uint8 Scale, float *AccDivider)
{
    boolean returnBool = TRUE;
    uint8 value = 0;

    /* Null pointer check */
    if(0 == AccDivider)
    {
        CFE_EVS_SendEvent(MPU9250_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "MPU9250 SetAccScale Null Pointer");
        returnBool = FALSE;
        goto end_of_function;
    }

    switch (Scale)
    {
        case 2:
            *AccDivider = 16384;
            value = MPU9250_BITS_FS_2G;
            break;

        case 4:
            *AccDivider = 8192;
            value = MPU9250_BITS_FS_4G;
            break;

        case 8:
            *AccDivider = 4096;
            value = MPU9250_BITS_FS_8G;
            break;

        case 16:
            *AccDivider = 2048;
            value = MPU9250_BITS_FS_16G;
            break;

        default:
            returnBool = FALSE;
            goto end_of_function;
    }

    returnBool = MPU9250_WriteReg(MPU9250_REG_ACCEL_CONFIG, value);
    if (FALSE == returnBool)
    {
        goto end_of_function;
    }

end_of_function:
    return returnBool;
}


boolean MPU9250_SetGyroScale(uint32 Scale, float *GyroDivider)
{
    boolean returnBool = TRUE;
    uint8 value = 0;

    /* Null pointer check */
    if(0 == GyroDivider)
    {
        CFE_EVS_SendEvent(MPU9250_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "MPU9250 SetGyroScale Null Pointer");
        returnBool = FALSE;
        goto end_of_function;
    }

    switch (Scale)
    {
        case 250:
            *GyroDivider = 131;
            value = MPU9250_BITS_FS_250DPS;
            break;

        case 500:
            *GyroDivider = 65.5;
            value = MPU9250_BITS_FS_500DPS;
            break;

        case 1000:
            *GyroDivider = 32.8;
            value = MPU9250_BITS_FS_1000DPS;
            break;

        case 2000:
            *GyroDivider = 16.4;
            value = MPU9250_BITS_FS_2000DPS;
            break;

        default:
            returnBool = FALSE;
            goto end_of_function;
    }

    returnBool = MPU9250_WriteReg(MPU9250_REG_GYRO_CONFIG, value);
    if (FALSE == returnBool)
    {
        goto end_of_function;
    }

end_of_function:
    return returnBool;
}


boolean MPU9250_Read_Gyro(int16 *X, int16 *Y, int16 *Z)
{
    uint8 hValue = 0;
    uint8 lValue = 0;
    boolean returnBool = TRUE;

    /* Null pointer check */
    if(0 == X || 0 == Y || 0 == Z)
    {
        CFE_EVS_SendEvent(MPU9250_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "MPU9250 Read_Gyro Null Pointer");
        returnBool = FALSE;
        goto end_of_function;
    }

    returnBool = MPU9250_ReadReg(MPU9250_REG_GYRO_XOUT_H, &hValue);
    if(FALSE == returnBool)
    {
        returnBool = FALSE;
        goto end_of_function;
    }
    returnBool = MPU9250_ReadReg(MPU9250_REG_GYRO_XOUT_L, &lValue);
    if(FALSE == returnBool)
    {
        returnBool = FALSE;
        goto end_of_function;
    }

    *X = (hValue << 8) | lValue;

    returnBool = MPU9250_ReadReg(MPU9250_REG_GYRO_YOUT_H, &hValue);
    if(FALSE == returnBool)
    {
        returnBool = FALSE;
        goto end_of_function;
    }
    returnBool = MPU9250_ReadReg(MPU9250_REG_GYRO_YOUT_L, &lValue);
    if(FALSE == returnBool)
    {
        returnBool = FALSE;
        goto end_of_function;
    }

    *Y = (hValue << 8) | lValue;

    returnBool = MPU9250_ReadReg(MPU9250_REG_GYRO_ZOUT_H, &hValue);
    if(FALSE == returnBool)
    {
        returnBool = FALSE;
        goto end_of_function;
    }
    returnBool = MPU9250_ReadReg(MPU9250_REG_GYRO_ZOUT_L, &lValue);
    if(FALSE == returnBool)
    {
        returnBool = FALSE;
        goto end_of_function;
    }

    *Z = (hValue << 8) | lValue;

end_of_function:
    if (FALSE == returnBool)
    {
        CFE_EVS_SendEvent(MPU9250_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "MPU9250 read error in Read_Gyro");
    }
    return returnBool;
}


boolean MPU9250_Read_Accel(int16 *X, int16 *Y, int16 *Z)
{
    uint8 hValue = 0;
    uint8 lValue = 0;
    boolean returnBool = TRUE;

    /* Null pointer check */
    if(0 == X || 0 == Y || 0 == Z)
    {
        CFE_EVS_SendEvent(MPU9250_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "MPU9250 Read_Accel Null Pointer");
        returnBool = FALSE;
        goto end_of_function;
    }

    returnBool = MPU9250_ReadReg(MPU9250_REG_ACCEL_XOUT_H, &hValue);
    if(FALSE == returnBool)
    {
        returnBool = FALSE;
        goto end_of_function;
    }
    returnBool = MPU9250_ReadReg(MPU9250_REG_ACCEL_XOUT_L, &lValue);
    if(FALSE == returnBool)
    {
        returnBool = FALSE;
        goto end_of_function;
    }

    *X = (hValue << 8) | lValue;

    returnBool = MPU9250_ReadReg(MPU9250_REG_ACCEL_YOUT_H, &hValue);
    if(FALSE == returnBool)
    {
        returnBool = FALSE;
        goto end_of_function;
    }
    returnBool = MPU9250_ReadReg(MPU9250_REG_ACCEL_YOUT_L, &lValue);
    if(FALSE == returnBool)
    {
        returnBool = FALSE;
        goto end_of_function;
    }

    *Y = (hValue << 8) | lValue;

    returnBool = MPU9250_ReadReg(MPU9250_REG_ACCEL_ZOUT_H, &hValue);
    if(FALSE == returnBool)
    {
        returnBool = FALSE;
        goto end_of_function;
    }
    returnBool = MPU9250_ReadReg(MPU9250_REG_ACCEL_ZOUT_L, &lValue);
    if(FALSE == returnBool)
    {
        returnBool = FALSE;
        goto end_of_function;
    }

    *Z = (hValue << 8) | lValue;

end_of_function:
    if (FALSE == returnBool)
    {
        CFE_EVS_SendEvent(MPU9250_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "MPU9250 read error in Read_Accel");
    }
    return returnBool;
};


//boolean MPU9250_Read_Mag(int16 *X, int16 *Y, int16 *Z)
//{
    //uint8 hValue = 0;
    //uint8 lValue = 0;
    //uint8 st2    = 0;
    //boolean returnBool = TRUE;

    ///* Null pointer check */
    //if(0 == X || 0 == Y || 0 == Z)
    //{
        //CFE_EVS_SendEvent(MPU9250_DEVICE_ERR_EID, CFE_EVS_ERROR,
            //"MPU9250 Read_Mag Null Pointer");
        //returnBool = FALSE;
        //goto end_of_function;
    //}

    //returnBool = MPU9250_ReadReg(MPU9250_REG_EXT_SENS_DATA_00 + MPU9250_AK8963_HXH, &hValue);
    //if(FALSE == returnBool)
    //{
        //returnBool = FALSE;
        //goto end_of_function;
    //}
    //returnBool = MPU9250_ReadReg(MPU9250_REG_EXT_SENS_DATA_00 + MPU9250_AK8963_HXL, &lValue);
    //if(FALSE == returnBool)
    //{
        //returnBool = FALSE;
        //goto end_of_function;
    //}

    //*X = (hValue << 8) | lValue;

    //returnBool = MPU9250_ReadReg(MPU9250_REG_EXT_SENS_DATA_00 + MPU9250_AK8963_HYH, &hValue);
    //if(FALSE == returnBool)
    //{
        //returnBool = FALSE;
        //goto end_of_function;
    //}
    //returnBool = MPU9250_ReadReg(MPU9250_REG_EXT_SENS_DATA_00 + MPU9250_AK8963_HYL, &lValue);
    //if(FALSE == returnBool)
    //{
        //returnBool = FALSE;
        //goto end_of_function;
    //}

    //*Y = (hValue << 8) | lValue;

    //returnBool = MPU9250_ReadReg(MPU9250_REG_EXT_SENS_DATA_00 + MPU9250_AK8963_HZH, &hValue);
    //if(FALSE == returnBool)
    //{
        //returnBool = FALSE;
        //goto end_of_function;
    //}
    //returnBool = MPU9250_ReadReg(MPU9250_REG_EXT_SENS_DATA_00 + MPU9250_AK8963_HZL, &lValue);
    //if(FALSE == returnBool)
    //{
        //returnBool = FALSE;
        //goto end_of_function;
    //}
    //*Z = (hValue << 8) | lValue;
    
    //returnBool = MPU9250_ReadReg(MPU9250_REG_EXT_SENS_DATA_00 + MPU9250_AK8963_ST2, &st2);
    //if(st2 & MPU9250_ST2_HOFL_MASK)
    //{
        //CFE_EVS_SendEvent(MPU9250_DEVICE_ERR_EID, CFE_EVS_ERROR,
            //"Mag sensor data overflow");
        //returnBool = FALSE;
        //goto end_of_function;
    //}

//end_of_function:
    //if (FALSE == returnBool)
    //{
        //CFE_EVS_SendEvent(MPU9250_DEVICE_ERR_EID, CFE_EVS_ERROR,
            //"MPU9250 read error in Read_Mag");
    //}
    //return returnBool;
//}


boolean MPU9250_Read_Temp(uint16 *Temp)
{
    uint8 hValue = 0;
    uint8 lValue = 0;
    boolean returnBool = TRUE;

    /* Null pointer check */
    if(0 == Temp)
    {
        CFE_EVS_SendEvent(MPU9250_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "MPU9250 Read_Temp Null Pointer");
        returnBool = FALSE;
        goto end_of_function;
    }

    returnBool = MPU9250_ReadReg(MPU9250_REG_TEMP_OUT_H, &hValue);
    if(FALSE == returnBool)
    {
        returnBool = FALSE;
        goto end_of_function;
    }
    returnBool = MPU9250_ReadReg(MPU9250_REG_TEMP_OUT_L, &lValue);
    if(FALSE == returnBool)
    {
        returnBool = FALSE;
        goto end_of_function;
    }
    *Temp = (hValue << 8) | lValue;

end_of_function:
    if (FALSE == returnBool)
    {
        CFE_EVS_SendEvent(MPU9250_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "MPU9250 read error in Read_Temp");
    }
    return returnBool;
}


boolean MPU9250_Read_WhoAmI(uint8 *Value)
{
    boolean returnBool = TRUE;
    /* Null pointer check */
    if(0 == Value)
    {
        CFE_EVS_SendEvent(MPU9250_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "MPU9250 Read_WhoAmI Null Pointer");
        returnBool = FALSE;
        goto end_of_function;
    }

    returnBool = MPU9250_ReadReg(MPU9250_REG_WHOAMI, Value);

end_of_function:
    if (FALSE == returnBool)
    {
        CFE_EVS_SendEvent(MPU9250_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "MPU9250 read error in WhoAmI");
    }
    return returnBool;
}


//boolean MPU9250_Read_MagDeviceID(uint8 *Value)
//{
    //boolean returnBool = TRUE;
    ///* Null pointer check */
    //if(0 == Value)
    //{
        //CFE_EVS_SendEvent(MPU9250_DEVICE_ERR_EID, CFE_EVS_ERROR,
            //"MPU9250 Read_MagDeviceID Null Pointer");
        //returnBool = FALSE;
        //goto end_of_function;
    //}

    //returnBool = MPU9250_ReadReg(MPU9250_REG_EXT_SENS_DATA_00 + MPU9250_AK8963_WIA, Value);

//end_of_function:
    //if (FALSE == returnBool)
    //{
        //CFE_EVS_SendEvent(MPU9250_DEVICE_ERR_EID, CFE_EVS_ERROR,
            //"MPU9250 read error in MagDeviceID");
    //}
    //return returnBool;
//}


//boolean MPU9250_Read_MagInfo(uint8 *Value)
//{
    //boolean returnBool = TRUE;
    ///* Null pointer check */
    //if(0 == Value)
    //{
        //CFE_EVS_SendEvent(MPU9250_DEVICE_ERR_EID, CFE_EVS_ERROR,
            //"MPU9250 Read_MagInfo Null Pointer");
        //returnBool = FALSE;
        //goto end_of_function;
    //}
    //*Value = MPU9250_ReadReg(MPU9250_REG_EXT_SENS_DATA_00 + MPU9250_AK8963_INFO, Value);

//end_of_function:
    //if (FALSE == returnBool)
    //{
        //CFE_EVS_SendEvent(MPU9250_DEVICE_ERR_EID, CFE_EVS_ERROR,
            //"MPU9250 read error in MagInfo");
    //}
    //return returnBool;
//}


boolean MPU9250_Perform_AccelSelfTest(void)
{
    return TRUE;
}


boolean MPU9250_Perform_GyroSelfTest(void)
{
    return TRUE;
}


//boolean MPU9250_Start_MagSelfTest(void)
//{
    //return TRUE;
//}


//boolean MPU9250_Stop_MagSelfTest(void)
//{
    //return TRUE;
//}


/* TODO */
boolean MPU9250_Read_ImuStatus(boolean *WOM, boolean *FifoOvflw, boolean *Fsync, boolean *DataReady)
{
    uint8 value = 0;
    boolean returnBool = TRUE;
    
    /* Null pointer check */
    if(0 == WOM || 0 == FifoOvflw || 0 == Fsync|| 0 == DataReady)
    {
        CFE_EVS_SendEvent(MPU9250_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "MPU9250 Read_ImuStatus Null Pointer");
        returnBool = FALSE;
        goto end_of_function;
    }
    
    returnBool = MPU9250_ReadReg(MPU9250_REG_INT_STATUS, &value);
    if(FALSE == returnBool)
    {
        CFE_EVS_SendEvent(MPU9250_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "MPU9250 read error in ImuStatus");
        returnBool = FALSE;
        goto end_of_function;
    }

    *WOM        = value & MPU9250_ST_INT_WOM_MASK;
    *FifoOvflw  = value & MPU9250_ST_INT_FIFO_OFL_MASK;
    *Fsync      = value & MPU9250_ST_INT_FSYNC_MASK;
    *DataReady  = value & MPU9250_ST_INT_RDY_MASK;

end_of_function:
    return returnBool;
}


//boolean MPU9250_Read_MagStatus(boolean *Overrun, boolean *DataReady, boolean *Overflow, boolean *Output16Bit)
//{
    //uint8 value = 0;
    //boolean returnBool = TRUE;
    
    ///* Null pointer check */
    //if(0 == Overrun || 0 == DataReady || 0 == Overflow || 0 == Output16Bit)
    //{
        //CFE_EVS_SendEvent(MPU9250_DEVICE_ERR_EID, CFE_EVS_ERROR,
            //"MPU9250 Read_MagStatus Null Pointer");
        //returnBool = FALSE;
        //goto end_of_function;
    //}

    //returnBool = MPU9250_ReadReg(MPU9250_REG_EXT_SENS_DATA_00 + MPU9250_AK8963_ST1, &value);
    //if(FALSE == returnBool)
    //{
        //returnBool = FALSE;
        //goto end_of_function;
    //}
    //*Overrun = value & MPU9250_ST1_DOR_MASK;
    //*DataReady = value & MPU9250_ST1_DRDY_MASK;

    //returnBool = MPU9250_ReadReg(MPU9250_REG_EXT_SENS_DATA_00 + MPU9250_AK8963_ST2, &value);
    //if(FALSE == returnBool)
    //{
        //returnBool = FALSE;
        //goto end_of_function;
    //}
    //*Overflow = value & MPU9250_ST2_HOFL_MASK;
    //*Output16Bit = value & MPU9250_ST2_BITM_MASK;

//end_of_function:
    //if (FALSE == returnBool)
    //{
        //CFE_EVS_SendEvent(MPU9250_DEVICE_ERR_EID, CFE_EVS_ERROR,
            //"MPU9250 read error in MagStatus");
    //}
    //return returnBool;
//}


//boolean MPU9250_Custom_Read_MagAdj(void)
//{
    //boolean returnBool = TRUE;
    //uint8 validateX = 0;
    //uint8 validateY = 0;
    //uint8 validateZ = 0;

    //returnBool = MPU9250_ReadReg(MPU9250_REG_EXT_SENS_DATA_00, &validateX);
    //if(FALSE == returnBool)
    //{
        //returnBool = FALSE;
        //goto end_of_function;
    //}
    ///* TODO validate X*/
    //MPU9250_AppCustomData.MagAdjX = validateX;
    //returnBool = MPU9250_ReadReg(MPU9250_REG_EXT_SENS_DATA_01, &validateY);
    //if(FALSE == returnBool)
    //{
        //returnBool = FALSE;
        //goto end_of_function;
    //}
    ///* TODO validate Y*/
    //MPU9250_AppCustomData.MagAdjY = validateY;
    //returnBool = MPU9250_ReadReg(MPU9250_REG_EXT_SENS_DATA_02, &validateZ);
    //if(FALSE == returnBool)
    //{
        //returnBool = FALSE;
        //goto end_of_function;
    //}
    ///* TODO validate Z*/
    //MPU9250_AppCustomData.MagAdjZ = validateZ;

//end_of_function:
    //if (FALSE == returnBool)
    //{
        //CFE_EVS_SendEvent(MPU9250_DEVICE_ERR_EID, CFE_EVS_ERROR,
            //"MPU9250 read or write error in Custom MagAdj");
    //}
    //return returnBool;
//}


//boolean MPU9250_Read_MagAdj(uint8 *X, uint8 *Y, uint8 *Z)
//{
    //boolean returnBool = TRUE;

    ///* Null pointer check */
    //if(0 == X || 0 == Y || 0 == Z)
    //{
        //CFE_EVS_SendEvent(MPU9250_DEVICE_ERR_EID, CFE_EVS_ERROR,
            //"MPU9250 Read_MagAdj Null Pointer");
        //returnBool = FALSE;
        //goto end_of_function;
    //}

    //*X = MPU9250_AppCustomData.MagAdjX;
    //*Y = MPU9250_AppCustomData.MagAdjY;
    //*Z = MPU9250_AppCustomData.MagAdjZ;

//end_of_function:

    //return returnBool;
//}


boolean MPU9250_Apply_Platform_Rotation(float *X, float *Y, float *Z)
{
    boolean returnBool = TRUE;
    float temp;

    /* Null pointer check */
    if(0 == X || 0 == Y || 0 == Z)
    {
        CFE_EVS_SendEvent(MPU9250_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "MPU9250 Apply_Platform_Rotation Null Pointer");
        returnBool = FALSE;
        goto end_of_function;
    }
    /* ROTATION_ROLL_180_YAW_90 */
    temp = *X; 
    *X = *Y; 
    *Y = temp; 
    *Z = -*Z;

end_of_function:

    return returnBool;
}


boolean MPU9250_Custom_Max_Events_Not_Reached(int32 ind)
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


int32 MPU9250_Custom_Init_EventFilters(int32 ind, CFE_EVS_BinFilter_t *EventTbl)
{
    int32 customEventCount = ind;
    
    /* Null check */
    if(0 == EventTbl)
    {
        customEventCount = -1;
        goto end_of_function;
    }

    if(TRUE == MPU9250_Custom_Max_Events_Not_Reached(customEventCount))
    {
        EventTbl[  customEventCount].EventID = MPU9250_DEVICE_ERR_EID;
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


void MPU9250_Get_Rotation(uint8 *Rotation)
{
    
    /* Null pointer check */
    if(0 == Rotation)
    {
        CFE_EVS_SendEvent(MPU9250_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "MPU9250 Get_Rotation Null Pointer");
        goto end_of_function;
    }
    
    /* TODO move to a table */
    *Rotation = ROTATION_ROLL_180_YAW_90;

end_of_function:
;
}
