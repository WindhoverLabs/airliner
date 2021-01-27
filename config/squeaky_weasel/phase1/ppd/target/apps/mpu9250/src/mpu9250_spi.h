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

#ifndef MPU9250_SPI_H
#define MPU9250_SPI_H
/************************************************************************
** Includes
*************************************************************************/
#include "mpu9250_custom.h"
#include "cfe.h"

/************************************************************************
** Local Defines
*************************************************************************/
/* MPU 9250 register map for Gyroscope and Accelerometer */
/* Self test output generated during manufacturing tests. */
/** \brief Factory self test output for gyro X-axis.*/
#define MPU9250_REG_GYRO_X_TEST             (0x00)
/** \brief Factory self test output for gyro Y-axis.*/
#define MPU9250_REG_GYRO_Y_TEST             (0x01)
/** \brief Factory self test output for gyro Z-axis.*/
#define MPU9250_REG_GYRO_Z_TEST             (0x02)
/** \brief Factory self test output for accelerometer X-axis.*/
#define MPU9250_REG_ACCEL_X_TEST            (0x0D)
/** \brief Factory self test output for accelerometer Y-axis.*/
#define MPU9250_REG_ACCEL_Y_TEST            (0x0E)
/** \brief Factory self test output for accelerometer Z-axis.*/
#define MPU9250_REG_ACCEL_Z_TEST            (0x0F)
/* Registers used to remove DC bias from the gyro sensor data output for
 * X, Y, and Z axes. Values are subtracted from the gyro sensor values 
 * before going into the sensor registers. */
/** \brief High byte [15:8], offset X-axis. */
#define MPU9250_REG_GYRO_X_OFF_H            (0x13)
/** \brief Low byte [7:0], offset X-axis. */
#define MPU9250_REG_GYRO_X_OFF_L            (0x14)
/** \brief High byte [15:8], offset Y-axis. */
#define MPU9250_REG_GYRO_Y_OFF_H            (0x15)
/** \brief Low byte [7:0], offset Y-axis. */
#define MPU9250_REG_GYRO_Y_OFF_L            (0x16)
/** \brief High byte [15:8], offset Z-axis. */
#define MPU9250_REG_GYRO_Z_OFF_H            (0x17)
/** \brief Low byte [7:0], offset Z-axis. */
#define MPU9250_REG_GYRO_Z_OFF_L            (0x18)
/* Divides the internal sample rate to generate the sample rate that
 * controls sensor data output rate. Only effective when Fchoice = 2'b11
 * such that the average filter's output is selected.  */
/** \brief Sample rate divisor. */
#define MPU9250_REG_SMPLRT_DIV              (0x19)
/** \brief Configuration. */
#define MPU9250_REG_CONFIG                  (0x1A)
/** \brief Gyroscope configuration. */
#define MPU9250_REG_GYRO_CONFIG             (0x1B)
/** \brief Accelerometer configuration 1. */
#define MPU9250_REG_ACCEL_CONFIG            (0x1C)
/** \brief Accelerometer configuration 2. */
#define MPU9250_REG_ACCEL_CONFIG2           (0x1D)
/** \brief Low Power Accelerometer Output Data Rate (ODR) Control. */
#define MPU9250_REG_LPACCEL_ODR             (0x1E)
/** \brief Wake-on Motion Threshold. */
#define MPU9250_REG_WOM_THRESH              (0x1F)
/** \brief FIFO Enable. */
#define MPU9250_REG_FIFO_EN                 (0x23)
/** \brief I2C Master Control. */
#define MPU9250_REG_I2C_MST_CTRL            (0x24)
/** \brief I2C Slave 0 Address. */
#define MPU9250_REG_I2C_SLV0_ADDR           (0x25)
/** \brief I2C Slave 0 Register. */
#define MPU9250_REG_I2C_SLV0_REG            (0x26)
/** \brief I2C Slave 0 Control. */
#define MPU9250_REG_I2C_SLV0_CTRL           (0x27)
/** \brief I2C Slave 1 Address. */
#define MPU9250_REG_I2C_SLV1_ADDR           (0x28)
/** \brief I2C Slave 1 Registers. */
#define MPU9250_REG_I2C_SLV1_REG            (0x29)
/** \brief I2C Slave 1 Control. */
#define MPU9250_REG_I2C_SLV1_CTRL           (0x2A)
/** \brief I2C Slave 2 Address. */
#define MPU9250_REG_I2C_SLV2_ADDR           (0x2B)
/** \brief I2C Slave 2 Registers. */
#define MPU9250_REG_I2C_SLV2_REG            (0x2C)
/** \brief I2C Slave 2 Control. */
#define MPU9250_REG_I2C_SLV2_CTRL           (0x2D)
/** \brief I2C Slave 3 Address. */
#define MPU9250_REG_I2C_SLV3_ADDR           (0x2E)
/** \brief I2C Slave 3 Registers. */
#define MPU9250_REG_I2C_SLV3_REG            (0x2F)
/** \brief I2C Slave 3 Control. */
#define MPU9250_REG_I2C_SLV3_CTRL           (0x30)
/** \brief I2C Slave 4 Address. */
#define MPU9250_REG_I2C_SLV4_ADDR           (0x31)
/** \brief I2C Slave 4 Registers. */
#define MPU9250_REG_I2C_SLV4_REG            (0x32)
/** \brief I2C Data out, Data to be written to I2C Slave 4. */
#define MPU9250_REG_I2C_SLV4_DO             (0x33)
/** \brief I2C Slave 4 Control. */
#define MPU9250_REG_I2C_SLV4_CTRL           (0x34)
/** \brief I2C Data in, Data to be read from I2C Slave 4. */
#define MPU9250_REG_I2C_SLV4_DI             (0x35)
/** \brief I2C Master Status. */
#define MPU9250_REG_I2C_MST_STATUS          (0x36)
/** \brief Interrupt Pin / Bypass Enable Configuration. */
#define MPU9250_REG_INT_PIN_CFG             (0x37)
/** \brief Interrupt Enable. */
#define MPU9250_REG_INT_ENABLE              (0x38)
/** \brief Interrupt Status. */
#define MPU9250_REG_INT_STATUS              (0x3A)
/** \brief High byte accelerometer measurement X-axis. */
#define MPU9250_REG_ACCEL_XOUT_H            (0x3B)
/** \brief Low byte accelerometer measurement X-axis. */
#define MPU9250_REG_ACCEL_XOUT_L            (0x3C)
/** \brief High byte accelerometer measurement Y-axis. */
#define MPU9250_REG_ACCEL_YOUT_H            (0x3D)
/** \brief Low byte accelerometer measurement Y-axis. */
#define MPU9250_REG_ACCEL_YOUT_L            (0x3E)
/** \brief High byte accelerometer measurement Z-axis. */
#define MPU9250_REG_ACCEL_ZOUT_H            (0x3F)
/** \brief Low byte accelerometer measurement Z-axis. */
#define MPU9250_REG_ACCEL_ZOUT_L            (0x40)
/** \brief High byte temperature measurement. */
#define MPU9250_REG_TEMP_OUT_H              (0x41)
/** \brief Low byte temperature measurement. */
#define MPU9250_REG_TEMP_OUT_L              (0x42)
/** \brief High byte gyroscope measurement X-axis. */
#define MPU9250_REG_GYRO_XOUT_H             (0x43)
/** \brief Low byte gyroscope measurement X-axis. */
#define MPU9250_REG_GYRO_XOUT_L             (0x44)
/** \brief High byte gyroscope measurement Y-axis. */
#define MPU9250_REG_GYRO_YOUT_H             (0x45)
/** \brief Low byte gyroscope measurement Y-axis. */
#define MPU9250_REG_GYRO_YOUT_L             (0x46)
/** \brief High byte gyroscope measurement Z-axis. */
#define MPU9250_REG_GYRO_ZOUT_H             (0x47)
/** \brief Low byte gyroscope measurement Z-axis. */
#define MPU9250_REG_GYRO_ZOUT_L             (0x48)
/** \brief External sensor data 0 */
#define MPU9250_REG_EXT_SENS_DATA_00        (0x49)
/** \brief External sensor data 1 */
#define MPU9250_REG_EXT_SENS_DATA_01        (0x4A)
/** \brief External sensor data 2 */
#define MPU9250_REG_EXT_SENS_DATA_02        (0x4B)
/** \brief I2C Data out, Data to be written to I2C Slave 0. */
#define MPU9250_REG_I2C_SLV0_DO             (0x63)
/** \brief I2C Data out, Data to be written to I2C Slave 1. */
#define MPU9250_REG_I2C_SLV1_DO             (0x64)
/** \brief I2C Data out, Data to be written to I2C Slave 2. */
#define MPU9250_REG_I2C_SLV2_DO             (0x65)
/** \brief I2C Data out, Data to be written to I2C Slave 3. */
#define MPU9250_REG_I2C_SLV3_DO             (0x66)
/** \brief I2C Master Delay Control. */
#define MPU9250_REG_I2C_MST_DELAY_CTRL      (0x67)
/** \brief Digital signal path reset. */
#define MPU9250_REG_SIGNAL_PATH_RESET       (0x68)
/** \brief Accelerometer interrupt control. */
#define MPU9250_REG_MOT_DETECT_CTRL         (0x69)
/** \brief User control. */
#define MPU9250_REG_USER_CTRL               (0x6A)
/** \brief Power management 1. */
#define MPU9250_REG_PWR_MGMT_1              (0x6B)
/** \brief Power management 2. */
#define MPU9250_REG_PWR_MGMT_2              (0x6C)
/** \brief High bits, number of written bytes in FIFO. */
#define MPU9250_REG_FIFO_COUNTH             (0x72)
/** \brief Low bits, number of written bytes in FIFO. */
#define MPU9250_REG_FIFO_COUNTL             (0x73)
/** \brief FIFO read/write command. */
#define MPU9250_REG_FIFO_R_W                (0x74)
/** \brief Indicate to user which device is being accessed. */
#define MPU9250_REG_WHOAMI                  (0x75)

/* Configuration bits mpu9250 */
/** \brief Power Management 1 sleep bit. */
#define MPU9250_BIT_SLEEP                   (0x40)
/** \brief Power Management 1 hardware reset bit. */
#define MPU9250_BIT_H_RESET                 (0x80)
/** \brief Power Management 1 clock select bit. */
#define MPU9250_BITS_CLKSEL                 (0x07)

#define MPU9250_MPU_CLK_SEL_PLLGYROX        (0x01)
#define MPU9250_MPU_CLK_SEL_PLLGYROZ        (0x03)
#define MPU9250_MPU_EXT_SYNC_GYROX          (0x02)
/** \brief Gyro Full Scale Select bit 250 DPS. */
#define MPU9250_BITS_FS_250DPS              (0x00)
/** \brief Gyro Full Scale Select bit 500 DPS. */
#define MPU9250_BITS_FS_500DPS              (0x08)
/** \brief Gyro Full Scale Select bit 1000 DPS. */
#define MPU9250_BITS_FS_1000DPS             (0x10)
/** \brief Gyro Full Scale Select bit 2000 DPS. */
#define MPU9250_BITS_FS_2000DPS             (0x18)
/** \brief Accel Full Scale Select bit 2G. */
#define MPU9250_BITS_FS_2G                  (0x00)
/** \brief Accel Full Scale Select bit 4G. */
#define MPU9250_BITS_FS_4G                  (0x08)
/** \brief Accel Full Scale Select bit 8G. */
#define MPU9250_BITS_FS_8G                  (0x10)
/** \brief Accel Full Scale Select bit 16G. */
#define MPU9250_BITS_FS_16G                 (0x18)
/** \brief Digital Low Pass Filter Configuration bit 250Hz. */
#define MPU9250_BITS_FS_MASK                (0x18)
#define MPU9250_BITS_DLPF_CFG_250HZ         (0x00)
/** \brief Digital Low Pass Filter Configuration bit 184Hz. */
#define MPU9250_BITS_DLPF_CFG_184HZ         (0x01)
/** \brief Digital Low Pass Filter Configuration bit 92Hz. */
#define MPU9250_BITS_DLPF_CFG_92HZ          (0x02)
/** \brief Digital Low Pass Filter Configuration bit 41Hz. */
#define MPU9250_BITS_DLPF_CFG_41HZ          (0x03)
/** \brief Digital Low Pass Filter Configuration bit 20Hz. */
#define MPU9250_BITS_DLPF_CFG_20HZ          (0x04)
/** \brief Digital Low Pass Filter Configuration bit 10Hz. */
#define MPU9250_BITS_DLPF_CFG_10HZ          (0x05)
/** \brief Digital Low Pass Filter Configuration bit 5Hz. */
#define MPU9250_BITS_DLPF_CFG_5HZ           (0x06)
/** \brief Digital Low Pass Filter Configuration bit 3600Hz. */
#define MPU9250_BITS_DLPF_CFG_3600HZ        (0x07)
#define MPU9250_BITS_DLPF_CFG_MASK          (0x07)
/** \brief Interrupt status cleared configuration bit. */
#define MPU9250_BIT_INT_ANYRD_2CLEAR        (0x10)
/** \brief Enable raw sensor data ready interrupt to pin bit. */
#define MPU9250_BIT_RAW_RDY_EN              (0x01)
/** \brief Reset I2C slave and set interface to SPI mode only. */
#define MPU9250_BIT_I2C_IF_DIS              (0x10)

#define MPU9250_READ_FLAG                   (0x80)
#define MPU9250_BIT_RAW_RDY_EN              (0x01)
#define MPU9250_BIT_INT_ANYRD_2CLEAR        (0x10)
/** \brief The default value of the MPU9250 device ID. */
#define MPU9250_WHOAMI_9250                 (0x71)
/** \brief The I2C address of the Magnetometer. */
#define MPU9250_AK8963_I2C_ADDR             (0x0c)
/** \brief The default value of the AK8963 device ID. */
#define MPU9250_AK8963_Device_ID            (0x48)

/* AK8963 Register map for the Magnetometer */
/* Read-only registers */
/** \brief Device ID. */
#define MPU9250_AK8963_WIA                  (0x00)
/** \brief Device information. */
#define MPU9250_AK8963_INFO                 (0x01)
/** \brief Status 1. */
#define MPU9250_AK8963_ST1                  (0x02)
/** \brief Measurement data X-axis lower 8-bits */
#define MPU9250_AK8963_HXL                  (0x03)
/** \brief Measurement data X-axis upper 8-bits */
#define MPU9250_AK8963_HXH                  (0x04)
/** \brief Measurement data Y-axis lower 8-bits */
#define MPU9250_AK8963_HYL                  (0x05)
/** \brief Measurement data Y-axis upper 8-bits */
#define MPU9250_AK8963_HYH                  (0x06)
/** \brief Measurement data Z-axis lower 8-bits */
#define MPU9250_AK8963_HZL                  (0x07)
/** \brief Measurement data Z-axis upper 8-bits */
#define MPU9250_AK8963_HZH                  (0x08)
/** \brief Status 2. */
#define MPU9250_AK8963_ST2                  (0x09)
/* Write/Read registers */
/** \brief Control 1. */
#define MPU9250_AK8963_CNTL1                (0x0A)
/** \brief Control 2. */
#define MPU9250_AK8963_CNTL2                (0x0B)
/** \brief Self-test control. */
#define MPU9250_AK8963_ASTC                 (0x0C)
//#define MPU9250_AK8963_TS1                  (0x0D)
//#define MPU9250_AK8963_TS2                  (0x0E)
/** \brief I2C disable. */
#define MPU9250_AK8963_I2CDIS               (0x0F)
/* Read-only registers (ROM) */
/** \brief Sensitivity adjustment value X-axis. */
#define MPU9250_AK8963_ASAX                 (0x10)
/** \brief Sensitivity adjustment value Y-axis. */
#define MPU9250_AK8963_ASAY                 (0x11)
/** \brief Sensitivity adjustment value Z-axis. */
#define MPU9250_AK8963_ASAZ                 (0x12)

/* Masks Mag */
/** \brief Data ready. */
#define MPU9250_ST1_DRDY_MASK               (0x01)
/** \brief Data overrun. */
#define MPU9250_ST1_DOR_MASK                (0x02)
/** \brief Output bit setting (mirror). */
#define MPU9250_ST2_BITM_MASK               (0x10)
/** \brief Magnetic sensor overflow. */
#define MPU9250_ST2_HOFL_MASK               (0x08)
/** \brief AK8963 fuse ROM access mode. */
#define MPU9250_AK8963_FUSE_MODE            (0x0F)
/** \brief AK8963 continues measurement mode 1 */
#define MPU9250_AK8963_MEA1_MODE            (0x02)
/** \brief AK8963 output bit setting. */
#define MPU9250_AK8963_16BIT_ADC            (0x10)

/* Masks IMU */
/** \brief Data ready. */
#define MPU9250_ST_INT_RDY_MASK             (0x01)
/** \brief Fsync interrupt occurred. */
#define MPU9250_ST_INT_FSYNC_MASK           (0x04)
/** \brief Fifo overflow interrupt occurred. */
#define MPU9250_ST_INT_FIFO_OFL_MASK        (0x05)
/** \brief Wake on motion interrupt occurred. */
#define MPU9250_ST_INT_WOM_MASK             (0x07)

/** \brief Array initializer. */
#define MPU_InitRegNum                      (20)
/** \brief Array initializer 2. */
/** \brief Stop between slave reads. */
#define MPU9250_I2C_MST_P_NSR               (0x10)
/** \brief Delay data ready int until external sensor data is loaded. */
#define MPU9250_I2C_MST_WAIT_FOR_ES         (0x40)
/** \brief I2C master clock speed */
#define MPU9250_I2C_MST_CLOCK_400HZ         (0x0D)

#define MPU9250_DEFAULT_LOWPASS_FILTER MPU9250_BITS_DLPF_CFG_250HZ

/** \brief Retry attemps for interrupted ioctl calls.
**
**  \par Limits:
**       None.
*/
#define MPU9250_MAX_RETRY_ATTEMPTS           (2)

/** \brief Sleep time micro seconds for interrupted calls.
**
**  \par Limits:
**       None.
*/
#define MPU9250_MAX_RETRY_SLEEP_USEC         (10)

#define MPU9250_SPI_DEVICE_PATH              "/dev/spidev1.0"
#define MPU9250_SPI_DEVICE_MODE              (3)
#define MPU9250_SPI_DEVICE_BITS              (8)
#define MPU9250_SPI_DEVICE_SPEED             (1000000)
#define MPU9250_SPI_TX_DELAY                 (0)
#define MPU9250_SPI_RX_DELAY                 (0)

/************************************************************************
** Structure Declarations
*************************************************************************/
/**
 * \brief MPU9250 device status
 */
typedef enum
{
    /*! MPU9250 status uninitialized */
    MPU9250_CUSTOM_UNINITIALIZED  = 0,
    /*! MPU9250 status initialized */
    MPU9250_CUSTOM_INITIALIZED   = 1
} MPU9250_Custom_Status_t;


typedef struct
{
    /*! Device file descriptor */
    int                             DeviceFd;
    /*! The current device status */
    MPU9250_Custom_Status_t          Status;
    uint8                           MagAdjX;
    uint8                           MagAdjY;
    uint8                           MagAdjZ;
} MPU9250_AppCustomData_t;


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
int32 MPU9250_Ioctl(int fh, int request, void *arg);


int32 MPU9250_ResetDevice(void);
boolean MPU9250_WriteReg(uint8 Addr, uint8 Data);
boolean MPU9250_Custom_Max_Events_Not_Reached(int32 ind);


#endif /* MPU9250_SPI_H */
