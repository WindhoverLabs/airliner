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

#ifndef MPU6050_I2C_H
#define MPU6050_I2C_H
/************************************************************************
** Includes
*************************************************************************/
#include "mpu6050_custom.h"
#include "cfe.h"

/************************************************************************
** Local Defines
*************************************************************************/

/* MPU6050 general device defines. */
/** \brief MPU6050 device ID to verify on I2C. */
#define MPU6050_WHOAMI_ID                   (0x68)
/** \brief MPU6050 I2C slave address. */
#define MPU6050_I2C_SLAVE_ADDRESS           (0x68)
/* MPU6050 register map for Gyroscope and Accelerometer. */
/* Self test registers. */
/** \brief Self test registers gyro and accel X-axis. */
#define MPU6050_REG_SELF_TEST_X             (0x0D)
/** \brief Self test registers gyro and accel Y-axis. */
#define MPU6050_REG_SELF_TEST_Y             (0x0E)
/** \brief Self test registers gyro and accel Z-axis. */
#define MPU6050_REG_SELF_TEST_Z             (0x0F)
/** \brief Self test registers accel XYZ remaining bits. */
#define MPU6050_REG_SELF_TEST_A             (0x10)
/* Divides the internal sample rate to generate the sample rate that
 * controls sensor data output rate. Only effective when Fchoice = 2'b11
 * such that the average filter's output is selected.  */
/** \brief Sample rate divisor. */
#define MPU6050_REG_SMPLRT_DIV              (0x19)
/** \brief Configuration. */
#define MPU6050_REG_CONFIG                  (0x1A)
/** \brief Gyroscope configuration. */
#define MPU6050_REG_GYRO_CONFIG             (0x1B)
/** \brief Accelerometer configuration 1. */
#define MPU6050_REG_ACCEL_CONFIG            (0x1C)
/** \brief FIFO Enable. */
#define MPU6050_REG_FIFO_EN                 (0x23)
/** \brief I2C Master Control. */
#define MPU6050_REG_I2C_MST_CTRL            (0x24)
/** \brief I2C Slave 0 Address. */
#define MPU6050_REG_I2C_SLV0_ADDR           (0x25)
/** \brief I2C Slave 0 Register. */
#define MPU6050_REG_I2C_SLV0_REG            (0x26)
/** \brief I2C Slave 0 Control. */
#define MPU6050_REG_I2C_SLV0_CTRL           (0x27)
/** \brief I2C Slave 1 Address. */
#define MPU6050_REG_I2C_SLV1_ADDR           (0x28)
/** \brief I2C Slave 1 Registers. */
#define MPU6050_REG_I2C_SLV1_REG            (0x29)
/** \brief I2C Slave 1 Control. */
#define MPU6050_REG_I2C_SLV1_CTRL           (0x2A)
/** \brief I2C Slave 2 Address. */
#define MPU6050_REG_I2C_SLV2_ADDR           (0x2B)
/** \brief I2C Slave 2 Registers. */
#define MPU6050_REG_I2C_SLV2_REG            (0x2C)
/** \brief I2C Slave 2 Control. */
#define MPU6050_REG_I2C_SLV2_CTRL           (0x2D)
/** \brief I2C Slave 3 Address. */
#define MPU6050_REG_I2C_SLV3_ADDR           (0x2E)
/** \brief I2C Slave 3 Registers. */
#define MPU6050_REG_I2C_SLV3_REG            (0x2F)
/** \brief I2C Slave 3 Control. */
#define MPU6050_REG_I2C_SLV3_CTRL           (0x30)
/** \brief I2C Slave 4 Address. */
#define MPU6050_REG_I2C_SLV4_ADDR           (0x31)
/** \brief I2C Slave 4 Registers. */
#define MPU6050_REG_I2C_SLV4_REG            (0x32)
/** \brief I2C Data out, Data to be written to I2C Slave 4. */
#define MPU6050_REG_I2C_SLV4_DO             (0x33)
/** \brief I2C Slave 4 Control. */
#define MPU6050_REG_I2C_SLV4_CTRL           (0x34)
/** \brief I2C Data in, Data to be read from I2C Slave 4. */
#define MPU6050_REG_I2C_SLV4_DI             (0x35)
/** \brief I2C Master Status. */
#define MPU6050_REG_I2C_MST_STATUS          (0x36)
/** \brief Interrupt Pin / Bypass Enable Configuration. */
#define MPU6050_REG_INT_PIN_CFG             (0x37)
/** \brief Interrupt Enable. */
#define MPU6050_REG_INT_ENABLE              (0x38)
/** \brief Interrupt Status. */
#define MPU6050_REG_INT_STATUS              (0x3A)
/** \brief High byte accelerometer measurement X-axis. */
#define MPU6050_REG_ACCEL_XOUT_H            (0x3B)
/** \brief Low byte accelerometer measurement X-axis. */
#define MPU6050_REG_ACCEL_XOUT_L            (0x3C)
/** \brief High byte accelerometer measurement Y-axis. */
#define MPU6050_REG_ACCEL_YOUT_H            (0x3D)
/** \brief Low byte accelerometer measurement Y-axis. */
#define MPU6050_REG_ACCEL_YOUT_L            (0x3E)
/** \brief High byte accelerometer measurement Z-axis. */
#define MPU6050_REG_ACCEL_ZOUT_H            (0x3F)
/** \brief Low byte accelerometer measurement Z-axis. */
#define MPU6050_REG_ACCEL_ZOUT_L            (0x40)
/** \brief High byte temperature measurement. */
#define MPU6050_REG_TEMP_OUT_H              (0x41)
/** \brief Low byte temperature measurement. */
#define MPU6050_REG_TEMP_OUT_L              (0x42)
/** \brief High byte gyroscope measurement X-axis. */
#define MPU6050_REG_GYRO_XOUT_H             (0x43)
/** \brief Low byte gyroscope measurement X-axis. */
#define MPU6050_REG_GYRO_XOUT_L             (0x44)
/** \brief High byte gyroscope measurement Y-axis. */
#define MPU6050_REG_GYRO_YOUT_H             (0x45)
/** \brief Low byte gyroscope measurement Y-axis. */
#define MPU6050_REG_GYRO_YOUT_L             (0x46)
/** \brief High byte gyroscope measurement Z-axis. */
#define MPU6050_REG_GYRO_ZOUT_H             (0x47)
/** \brief Low byte gyroscope measurement Z-axis. */
#define MPU6050_REG_GYRO_ZOUT_L             (0x48)
/** \brief External sensor data 0 */
#define MPU6050_REG_EXT_SENS_DATA_00        (0x49)
/** \brief External sensor data 1 */
#define MPU6050_REG_EXT_SENS_DATA_01        (0x4A)
/** \brief External sensor data 2 */
#define MPU6050_REG_EXT_SENS_DATA_02        (0x4B)
/** \brief External sensor data 3 */
#define MPU6050_REG_EXT_SENS_DATA_03        (0x4C)
/** \brief External sensor data 4 */
#define MPU6050_REG_EXT_SENS_DATA_04        (0x4D)
/** \brief External sensor data 5 */
#define MPU6050_REG_EXT_SENS_DATA_05        (0x4E)
/** \brief External sensor data 6 */
#define MPU6050_REG_EXT_SENS_DATA_06        (0x4F)
/** \brief External sensor data 7 */
#define MPU6050_REG_EXT_SENS_DATA_07        (0x50)
/** \brief External sensor data 8 */
#define MPU6050_REG_EXT_SENS_DATA_08        (0x51)
/** \brief External sensor data 9 */
#define MPU6050_REG_EXT_SENS_DATA_09        (0x52)
/** \brief External sensor data 10 */
#define MPU6050_REG_EXT_SENS_DATA_10        (0x53)
/** \brief External sensor data 11 */
#define MPU6050_REG_EXT_SENS_DATA_11        (0x54)
/** \brief External sensor data 12 */
#define MPU6050_REG_EXT_SENS_DATA_12        (0x55)
/** \brief External sensor data 13 */
#define MPU6050_REG_EXT_SENS_DATA_13        (0x56)
/** \brief External sensor data 14 */
#define MPU6050_REG_EXT_SENS_DATA_14        (0x57)
/** \brief External sensor data 15 */
#define MPU6050_REG_EXT_SENS_DATA_15        (0x58)
/** \brief External sensor data 16 */
#define MPU6050_REG_EXT_SENS_DATA_16        (0x59)
/** \brief External sensor data 17 */
#define MPU6050_REG_EXT_SENS_DATA_17        (0x5A)
/** \brief External sensor data 18 */
#define MPU6050_REG_EXT_SENS_DATA_18        (0x5B)
/** \brief External sensor data 19 */
#define MPU6050_REG_EXT_SENS_DATA_19        (0x5C)
/** \brief External sensor data 20 */
#define MPU6050_REG_EXT_SENS_DATA_20        (0x5D)
/** \brief External sensor data 9 */
#define MPU6050_REG_EXT_SENS_DATA_21        (0x5E)
/** \brief External sensor data 22 */
#define MPU6050_REG_EXT_SENS_DATA_22        (0x5F)
/** \brief External sensor data 23 */
#define MPU6050_REG_EXT_SENS_DATA_23        (0x60)
/** \brief I2C Data out, Data to be written to I2C Slave 0. */
#define MPU6050_REG_I2C_SLV0_DO             (0x63)
/** \brief I2C Data out, Data to be written to I2C Slave 1. */
#define MPU6050_REG_I2C_SLV1_DO             (0x64)
/** \brief I2C Data out, Data to be written to I2C Slave 2. */
#define MPU6050_REG_I2C_SLV2_DO             (0x65)
/** \brief I2C Data out, Data to be written to I2C Slave 3. */
#define MPU6050_REG_I2C_SLV3_DO             (0x66)
/** \brief I2C Master Delay Control. */
#define MPU6050_REG_I2C_MST_DELAY_CTRL      (0x67)
/** \brief Digital signal path reset. */
#define MPU6050_REG_SIGNAL_PATH_RESET       (0x68)
/** \brief User control. */
#define MPU6050_REG_USER_CTRL               (0x6A)
/** \brief Power management 1. */
#define MPU6050_REG_PWR_MGMT_1              (0x6B)
/** \brief Power management 2. */
#define MPU6050_REG_PWR_MGMT_2              (0x6C)
/** \brief High bits, number of written bytes in FIFO. */
#define MPU6050_REG_FIFO_COUNTH             (0x72)
/** \brief Low bits, number of written bytes in FIFO. */
#define MPU6050_REG_FIFO_COUNTL             (0x73)
/** \brief FIFO read/write command. */
#define MPU6050_REG_FIFO_R_W                (0x74)
/** \brief Indicate to user which device is being accessed. */
#define MPU6050_REG_WHOAMI                  (0x75)

/* Configuration bits mpu6050 */
/** \brief Power Management 1 sleep bit. */
#define MPU6050_BIT_SLEEP                   (0x40)
/** \brief Power Management 1 hardware reset bit. */
#define MPU6050_BIT_H_RESET                 (0x80)
/** \brief Power Management 1 clock select bit. */
#define MPU6050_BITS_CLKSEL                 (0x07)
/** \brief Clock source internal 8MHz oscillator. */
#define MPU6050_MPU_CLK_SEL_INTERNAL        (0x00)
/** \brief PLL with X-axis gyroscope reference. */
#define MPU6050_MPU_CLK_SEL_PLLGYROX        (0x01)
/** \brief PLL with Y-asix gyroscope reference. */
#define MPU6050_MPU_CLK_SEL_PLLGYROY        (0x02)
/** \brief PLL with Z-asix gyroscope reference. */
#define MPU6050_MPU_CLK_SEL_PLLGYROZ        (0x03)
/** \brief External sync set FSYNC bit location GYRO_XOUT_L. */
#define MPU6050_MPU_EXT_SYNC_GYROX          (0x02)
/* Gyro scales. */
/** \brief Gyro Full Scale Select bit 250 DPS. */
#define MPU6050_BITS_FS_250DPS              (0x00)
/** \brief Gyro Full Scale Select bit 500 DPS. */
#define MPU6050_BITS_FS_500DPS              (0x08)
/** \brief Gyro Full Scale Select bit 1000 DPS. */
#define MPU6050_BITS_FS_1000DPS             (0x10)
/** \brief Gyro Full Scale Select bit 2000 DPS. */
#define MPU6050_BITS_FS_2000DPS             (0x18)
/* Accel scales. */
/** \brief Accel Full Scale Select bit 2G. */
#define MPU6050_BITS_FS_2G                  (0x00)
/** \brief Accel Full Scale Select bit 4G. */
#define MPU6050_BITS_FS_4G                  (0x08)
/** \brief Accel Full Scale Select bit 8G. */
#define MPU6050_BITS_FS_8G                  (0x10)
/** \brief Accel Full Scale Select bit 16G. */
#define MPU6050_BITS_FS_16G                 (0x18)
/** \brief Full Scale select mask. */
#define MPU6050_BITS_FS_MASK                (0x18)
/* DLPF configurations. */
/** \brief Digital Low Pass Filter Configuration bit 260Hz. */
#define MPU6050_BITS_DLPF_CFG_260HZ         (0x00)
/** \brief Digital Low Pass Filter Configuration bit 184Hz. */
#define MPU6050_BITS_DLPF_CFG_184HZ         (0x01)
/** \brief Digital Low Pass Filter Configuration bit 94Hz. */
#define MPU6050_BITS_DLPF_CFG_94HZ          (0x02)
/** \brief Digital Low Pass Filter Configuration bit 44Hz. */
#define MPU6050_BITS_DLPF_CFG_44HZ          (0x03)
/** \brief Digital Low Pass Filter Configuration bit 21Hz. */
#define MPU6050_BITS_DLPF_CFG_21HZ          (0x04)
/** \brief Digital Low Pass Filter Configuration bit 10Hz. */
#define MPU6050_BITS_DLPF_CFG_10HZ          (0x05)
/** \brief Digital Low Pass Filter Configuration bit 5Hz. */
#define MPU6050_BITS_DLPF_CFG_5HZ           (0x06)
/** \brief Digital Low Pass Filter Configuration bit reserved. */
#define MPU6050_BITS_DLPF_CFG_RESERVED      (0x07)
/** \brief Digital Low Pass Filter Configuration mask. */
#define MPU6050_BITS_DLPF_CFG_MASK          (0x07)
/** \brief Interrupt status cleared configuration bit. */
#define MPU6050_BIT_INT_ANYRD_2CLEAR        (0x10)
/** \brief Enable raw sensor data ready interrupt to pin bit. */
#define MPU6050_BIT_RAW_RDY_EN              (0x01)
/** \brief Reset I2C slave and set interface to SPI mode only. */
#define MPU6050_BIT_I2C_IF_DIS              (0x10)
/** \brief Direction read. */
#define MPU6050_READ_FLAG                   (0x80)
/** \brief Direction write. */
#define MPU6050_WRITE_FLAG                  (0x00)
/** \brief Enable data ready interrupt. */
#define MPU6050_BIT_RAW_RDY_EN              (0x01)
/** \brief Clear interrupt status on any ready operation. */
#define MPU6050_BIT_INT_ANYRD_2CLEAR        (0x10)
/** \brief Data ready interrupt status */
#define MPU6050_BIT_INT_STATUS_DATA         (0x01)
#define MPU6050_BIT_USER_CTRL_FIFO_RST      (0x04)
#define MPU6050_BIT_USER_CTRL_I2C_MST_EN    (0x20)
#define MPU6050_BIT_USER_CTRL_FIFO_EN       (0x40)
#define MPU6050_BIT_FIFO_ENABLE_TEMP_OUT    (0x80)
#define MPU6050_BIT_FIFO_ENABLE_GYRO_XOUT   (0x40)
#define MPU6050_BIT_FIFO_ENABLE_GYRO_YOUT   (0x20)
#define MPU6050_BIT_FIFO_ENABLE_GYRO_ZOUT   (0x10)
#define MPU6050_BIT_FIFO_ENABLE_ACCEL       (0x08)
#define MPU6050_BIT_INT_STATUS_FIFO_OVRFLW  (0x10)

/** \brief Array initializer 2. */
/** \brief Stop between slave reads. */
#define MPU6050_I2C_MST_P_NSR               (0x10)
/** \brief Delay data ready int until external sensor data is loaded. */
#define MPU6050_I2C_MST_WAIT_FOR_ES         (0x40)
/** \brief I2C master clock speed */
#define MPU6050_I2C_MST_CLOCK_400HZ         (0x0D)

#define MPU6050_DEFAULT_LOWPASS_FILTER MPU6050_BITS_DLPF_CFG_44HZ

/** \brief Retry attemps for interrupted ioctl calls.
**
**  \par Limits:
**       None.
*/
#define MPU6050_MAX_RETRY_ATTEMPTS           (2)

/** \brief Sleep time micro seconds for interrupted calls.
**
**  \par Limits:
**       None.
*/
#define MPU6050_MAX_RETRY_SLEEP_USEC         (10)

/** \brief Read data, from slave to master. */
#define MPU6050_I2C_M_READ                    (0x0001)

#define MPU6050_I2C_DEVICE_PATH              "/dev/i2c-mpu6050"


/************************************************************************
** Structure Declarations
*************************************************************************/
/**
 * \brief MPU6050 device status.
 */
typedef enum
{
    /*! MPU6050 status uninitialized */
    MPU6050_CUSTOM_UNINITIALIZED  = 0,
    /*! MPU6050 status initialized */
    MPU6050_CUSTOM_INITIALIZED   = 1
} MPU6050_Custom_Status_t;


/**
 * \brief MPU6050 block read sample for accel, temp, and gyro.
 */
typedef struct
{
    int16 val[7];
} MPU6050_Sample_t;


/**
 * \brief MPU6050 application custom data.
 */
typedef struct
{
    /*! Device file descriptor. */
    int                             DeviceFd;
    /*! The current device status. */
    MPU6050_Custom_Status_t         Status;
    /*! Filtered samples per cycle from the fifo queue. */
    float                           FifoSamplesPerCycle;
    /*! Initial fifo temperature sample. */
    float                           FifoTemp;
    /*! Last fifo temperature sample. */
    float                           FifoLastTemp;
    /*! Temperature initialized flag */
    boolean                         TempInitialized;
    /*! Current raw samples from the fifo queue. */
    MPU6050_Sample_t samples[MPU6050_MAX_FIFO_LENGTH];
    /*! First call flag. */
    boolean                         firstMeasureFlag;
} MPU6050_AppCustomData_t;



/************************************************************************
** External Global Variables
*************************************************************************/

/************************************************************************
** Function Prototypes
*************************************************************************/

/************************************************************************/
/** \brief ioctl with limited EINTR retry attempts. 
**
**  \par Description
**       This function is a wrapper for ioctl with retry attempts added.
**
**  \param [in] fh file descriptor.
**  \param [in] request code.
**  \param [in] arg pointer to a device specific struct.
**
**  \returns
**  usually 0 for success and -1 for failure, see ioctl man-page for 
**  more info.
**  \endreturns
**
*************************************************************************/
int32 MPU6050_Ioctl(int fh, int request, void *arg);


int32 MPU6050_ResetDevice(void);
boolean MPU6050_WriteReg(uint8 Addr, uint8 Data);
boolean MPU6050_ReadReg(uint8 Reg, void *Buffer, size_t Length);
boolean MPU6050_Custom_Max_Events_Not_Reached(int32 ind);
uint16 MPU6050_GetFifoCount(void);
void MPU6050_ResetFifo(void);
uint16 MPU6050_Swap16(uint16 val);
boolean MPU6050_sampleChecks(MPU6050_Sample_t *sample);

#endif /* MPU6050_I2C_H */
