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

#ifndef MS5607_I2C_H
#define MS5607_I2C_H

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************
** Includes
*************************************************************************/
#include "ms5607_custom.h"
#include "ms5607_platform_cfg.h"
#include "cfe.h"

/************************************************************************
** Local Defines
*************************************************************************/
/* I2C Configuration */
/** \brief I2C Device path of the MS5607 */
#define MS5607_I2C_DEVICE_PATH              MS5607_DEVICE_PATH

/** \brief I2C slave address of the MS5607 device in hex. */
#define MS5607_I2C_SLAVE_ADDRESS            (0x77)

/** \brief Read data, from slave to master. */
#define MS5607_I2C_M_READ                   (0x0001)

/** \brief D1 and D2 conversion interval. */
#define MS5607_CONVERSION_INTERVAL          (10000)

/** \brief After reset sleep interval. */
#define MS5607_RESET_INTERVAL               (100000)

/* I2C Commands */
/** \brief MS5607 command reset */
#define MS5607_I2C_CMD_RESET                (0x1e)

/** \brief MS5607 command convert D1 (OSR=256) */
#define MS5607_I2C_CMD_CONVERT_D1_MASK      (0x40)

/** \brief MS5607 command convert D2 (OSR=256) */
#define MS5607_I2C_CMD_CONVERT_D2_MASK      (0x50)

/** \brief MS5607 command mask convert OSR 256 */
#define MS5607_I2C_CMD_OSR_256_MASK         (0x00)

/** \brief MS5607 command mask convert OSR 512 */
#define MS5607_I2C_CMD_OSR_512_MASK         (0x02)

/** \brief MS5607 command mask convert OSR 1024 */
#define MS5607_I2C_CMD_OSR_1024_MASK        (0x04)

/** \brief MS5607 command mask convert OSR 2048 */
#define MS5607_I2C_CMD_OSR_2048_MASK        (0x06)

/** \brief MS5607 command mask convert OSR 4096 */
#define MS5607_I2C_CMD_OSR_4096_MASK        (0x08)

/** \brief MS5607 command ADC read */
#define MS5607_I2C_CMD_ADC_READ             (0x00)

/** \brief MS5607 command mask PROM read */
#define MS5607_I2C_CMD_PROM_READ_MASK       (0xa0)

/** \brief MS5607 command PROM read shift */
#define MS5607_I2C_CMD_PROM_ADDR_SHIFT      (1)

/** \brief MS5607 command convert D1 with OSR mask */
#define MS5607_I2C_CMD_CONVERT_D1           (MS5607_I2C_CMD_CONVERT_D1_MASK + MS5607_I2C_CMD_OSR_2048_MASK)

/** \brief MS5607 command convert D2 with OSR mask */
#define MS5607_I2C_CMD_CONVERT_D2           (MS5607_I2C_CMD_CONVERT_D2_MASK + MS5607_I2C_CMD_OSR_2048_MASK)

/* Custom Layer Configuration */
/** \brief Retry attemps for interrupted ioctl calls. */
#define MS5607_MAX_RETRY_ATTEMPTS           (2)

/** \brief Sleep time micro seconds for interrupted calls. */
#define MS5607_MAX_RETRY_SLEEP_USEC         (10)

/************************************************************************
** Structure Declarations
*************************************************************************/
/**
 * \brief MS5607 device status
 */
typedef enum
{
    /*! status uninitialized */
    MS5607_CUSTOM_UNINITIALIZED  = 0,
    /*! status initialized */
    MS5607_CUSTOM_INITIALIZED    = 1
} MS5607_Custom_Status_t;


typedef struct
{
    /*! Device file descriptor */
    int                             DeviceFd;
    /*! The current device status */
    MS5607_Custom_Status_t          Status;
} MS5607_AppCustomData_t;


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
int32 MS5607_Ioctl(int fh, int request, void *arg);

/************************************************************************/
/** \brief Determines if the maximum of event filters has been reached.
**
**  \par Description
**       This function checks if an index has reached the maximum
**       number of events.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]    ind    The current index to check.
**                             
**
**  \returns    boolean
**
*************************************************************************/
boolean MS5607_Custom_Max_Events_Not_Reached(int32 ind);

/************************************************************************/
/** \brief Receives data via I2C from the MS5607.
**
**  \par Description
**       This function receives data via I2C from the device.
**
**  \par Assumptions, External Events, and Notes:
**       The device must be initialized before this function is called.
**
**  \param [in]    Reg    The target (sender) register to read from.
**
**  \param [out]   Buffer The buffer to save the data received.                           
**
**  \param [in]    Length The length of data to read.
**
**  \returns    boolean   True for success, false for failure. 
**
*************************************************************************/
boolean MS5607_Custom_Receive(uint8 Reg, uint8 *Buffer, size_t Length);


/************************************************************************/
/** \brief Send command via I2C to the MS5607.
**
**  \par Description
**       This function sends commands via I2C to the device.
**
**  \par Assumptions, External Events, and Notes:
**       The device must be initialized before this function is called.
**
**  \param [in]    Reg    The target (recipient) register.
**
**  \returns    boolean   True for success, false for failure. 
**
*************************************************************************/
boolean MS5607_Custom_Send(uint8 Reg);


/************************************************************************/
/** \brief Reset the device.
**
**  \par Description
**       This function must be used at initialization.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \returns    ioctl return code.
**
*************************************************************************/
int32 MS5607_ResetDevice(void);

#ifdef __cplusplus
}
#endif 

#endif /* MS5607_I2C_H */
