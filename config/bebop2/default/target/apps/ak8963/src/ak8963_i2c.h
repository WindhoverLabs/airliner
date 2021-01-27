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

#ifndef AK8963_I2C_H
#define AK8963_I2C_H
/************************************************************************
** Includes
*************************************************************************/
#include "ak8963_custom.h"
#include "cfe.h"

/************************************************************************
** Local Defines
*************************************************************************/
/** \brief I2C device path. */
#define AK8963_I2C_DEVICE_PATH               "/dev/i2c-akm8963"

/** \brief I2C slave address. */
#define AK8963_I2C_SLAVE_ADDRESS             (0x0D)

/** \brief Read data, from slave to master. */
#define AK8963_I2C_M_READ                    (0x0001)

/** \brief Retry attemps for interrupted ioctl calls. */
#define AK8963_MAX_RETRY_ATTEMPTS           (2)

/** \brief Sleep time micro seconds for interrupted calls. */
#define AK8963_MAX_RETRY_SLEEP_USEC          (10)

/** \brief Max self-test read attempts.  */
#define AK8963_SELFTEST_READ_STATUS_ATTEMPTS (100)

/* AK8963 Register map for the Magnetometer */
/** \brief Device ID. */
#define AK8963_REG_WIA                       (0x00)
/** \brief Device information. */
#define AK8963_REG_INFO                      (0x01)
/** \brief Status 1. */
#define AK8963_REG_ST1                       (0x02)
/** \brief Measurement data X-axis lower 8-bits. */
#define AK8963_REG_HXL                       (0x03)
/** \brief Measurement data X-axis upper 8-bits. */
#define AK8963_REG_HXH                       (0x04)
/** \brief Measurement data Y-axis lower 8-bits. */
#define AK8963_REG_HYL                       (0x05)
/** \brief Measurement data Y-axis upper 8-bits. */
#define AK8963_REG_HYH                       (0x06)
/** \brief Measurement data Z-axis lower 8-bits. */
#define AK8963_REG_HZL                       (0x07)
/** \brief Measurement data Z-axis upper 8-bits. */
#define AK8963_REG_HZH                       (0x08)
/** \brief Status 2. */
#define AK8963_REG_ST2                       (0x09)
/** \brief Control 1. */
#define AK8963_REG_CNTL1                     (0x0A)
/** \brief Control 2. */
#define AK8963_REG_CNTL2                     (0x0B)
/** \brief Self-test control. */
#define AK8963_REG_ASTC                      (0x0C)
/** \brief Test 1 (DO NOT ACCESS). */
#define AK8963_REG_TS1                       (0x0D)
/** \brief Test 2 (DO NOT ACCESS). */
#define AK8963_REG_TS2                       (0x0E)
/** \brief I2C disable. */
#define AK8963_REG_I2CDIS                    (0x0F)
/** \brief Sensitivity adjustment value X-axis. */
#define AK8963_REG_ASAX                      (0x10)
/** \brief Sensitivity adjustment value Y-axis. */
#define AK8963_REG_ASAY                      (0x11)
/** \brief Sensitivity adjustment value Z-axis. */
#define AK8963_REG_ASAZ                      (0x12)

/* AK8963 Bit Mask */
/** \brief Mask power-down mode. */
#define AK8963_BITS_CNTL1_MODE_POWER_DOWN    (0x00)
/** \brief Single measurement mode */
#define AK8963_BITS_CNTL1_MODE_SINGLE        (0x01)
/** \brief Mask continues measurement mode 1. */
#define AK8963_BITS_CNTL1_MODE_CONTINOUS1    (0x02)
/** \brief Mask external trigger measurement mode. */
#define AK8963_BITS_CNTL1_MODE_EXTERNAL      (0x04)
/** \brief Mask continues measurement mode 2. */
#define AK8963_BITS_CNTL1_MODE_CONTINOUS2    (0x06)
/** \brief Mask self-test mode. */
#define AK8963_BITS_CNTL1_MODE_SELF_TEST     (0x08)
/** \brief Mask fuse ROM access mode. */
#define AK8963_BITS_CNTL1_MODE_ROM_ACCESS    (0x0F)
/** \brief Mask output bit setting 14 bit. */
#define AK8963_BITS_CNTL1_OUTPUT_14BIT       (0x00)
/** \brief Mask output bit setting 16 bit. */
#define AK8963_BITS_CNTL1_OUTPUT_16BIT       (0x10)
/** \brief Mask soft reset. */
#define AK8963_BITS_CNTL2_SOFT_RESET         (0x01)
/** \brief Mask self-test control normal. */
#define AK8963_BITS_ASTC_NORMAL              (0x00)
/** \brief Mask self-test generate field. */
#define AK8963_BITS_ASTC_SELF_TEST           (0x40)
/** \brief Mask Data ready. */
#define AK8963_BITS_ST1_DRDY                 (0x01)
/** \brief Mask Data overrun. */
#define AK8963_BITS_ST1_DOR                  (0x02)
/** \brief Mask Magnetic sensor overflow. */
#define AK8963_BITS_ST2_HOFL                 (0x08)
/** \brief Mask Output bit setting (mirror). */
#define AK8963_BITS_ST2_BITM                 (0x10)

/************************************************************************
** Structure Declarations
*************************************************************************/
/**
 * \brief AK8963 device status
 */
typedef enum
{
    /*! AK8963 status uninitialized */
    AK8963_CUSTOM_UNINITIALIZED  = 0,
    /*! AK8963 status initialized */
    AK8963_CUSTOM_INITIALIZED   = 1
} AK8963_Custom_Status_t;


typedef struct
{
    /*! Device file descriptor */
    int                             DeviceFd;
    /*! The current device status */
    AK8963_Custom_Status_t          Status;
    float                           MagAdjX;
    float                           MagAdjY;
    float                           MagAdjZ;
} AK8963_AppCustomData_t;


#pragma pack(push, 1)
typedef struct
{
    int16 val[3];
    uint8 st2;
} AK8963_Sample_t;
#pragma pack(pop)

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
int32 AK8963_Ioctl(int fh, int request, void *arg);

boolean AK8963_WriteReg(uint8 Addr, uint8 Data);
boolean AK8963_ReadReg(uint8 Reg, void *Buffer, size_t Length);
boolean AK8963_Custom_Max_Events_Not_Reached(int32 ind);
boolean AK8963_Custom_In_Range(float value, float min, float max);

#endif /* AK8963_SPI_H */
