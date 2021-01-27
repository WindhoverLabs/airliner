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
#ifndef HMC5883_I2C_H
#define HMC5883_I2C_H

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************
** Includes
*************************************************************************/
#include "hmc5883_custom.h"
#include "cfe.h"

/************************************************************************
** Local Defines
*************************************************************************/
/** \brief I2C device path. */
#define HMC5883_I2C_DEVICE_PATH                   "/dev/i2c-4"

/** \brief I2C slave address of the HMC5883 device in hex. */
#define HMC5883_I2C_SLAVE_ADDRESS                 (0x1e)

/** \brief HMC5883 expected ID A value. */
#define HMC5883_ID_A                              ('H')

/** \brief HMC5883 expected ID B value. */
#define HMC5883_ID_B                              ('4')

/** \brief HMC5883 expected ID C value. */
#define HMC5883_ID_C                              ('3')

/** \brief Read data, from slave to master.
**
**  \par Limits:
**       None.
*/
#define HMC5883_I2C_M_READ                        (0x0001)

/** \brief The measurement interval (150Hz).
**
**  \par Limits:
**       Max for the HMC5883 is 160Hz.
*/
#define HMC5883_CONVERSION_INTERVAL_US            (1000000/150)

/* HMC5883 device registers */
/** \brief Configuration register A.
**
**  \par Description:
**       Used for setting the data output rate and measurement config.
*/
#define HMC5883_I2C_REG_CONFIG_A                  (0x00)

/** \brief Configuration register B.
**
**  \par Description:
**       Used for setting device gain.
*/
#define HMC5883_I2C_REG_CONFIG_B                  (0x01)

/** \brief Mode register.
**
**  \par Description:
**       Used to select the operating mode of the device.
*/
#define HMC5883_I2C_REG_MODE                      (0x02)

/** \brief Data Output X Registers A.
**
**  \par Description:
**       Used to store the measurement result from channel X.
*/
#define HMC5883_I2C_REG_DATA_OUT_X_MSB            (0x03)

/** \brief Data Output X Registers B.
**
**  \par Description:
**       Used to store the measurement result from channel X.
*/
#define HMC5883_I2C_REG_DATA_OUT_X_LSB            (0x04)

/** \brief Data Output Z Registers A.
**
**  \par Description:
**       Used to store the measurement result from channel Z.
*/
#define HMC5883_I2C_REG_DATA_OUT_Z_MSB            (0x05)

/** \brief Data Output Z Registers B.
**
**  \par Description:
**       Used to store the measurement result from channel Z.
*/
#define HMC5883_I2C_REG_DATA_OUT_Z_LSB            (0x06)

/** \brief Data Output Y Registers A.
**
**  \par Description:
**       Used to store the measurement result from channel Y.
*/
#define HMC5883_I2C_REG_DATA_OUT_Y_MSB            (0x07)

/** \brief Data Output Y Registers B.
**
**  \par Description:
**       Used to store the measurement result from channel Y.
*/
#define HMC5883_I2C_REG_DATA_OUT_Y_LSB            (0x08)

/** \brief Status register.
**
**  \par Description:
**       Used to indicate device status.
*/
#define HMC5883_I2C_REG_STATUS                    (0x09)

/** \brief Identification Register A.
**
**  \par Description:
**       Used to identify the device.
*/
#define HMC5883_REG_ID_A                          (0x0a)

/** \brief Identification Register B.
**
**  \par Description:
**       Used to identify the device.
*/
#define HMC5883_REG_ID_B                          (0x0b)

/** \brief Identification Register C.
**
**  \par Description:
**       Used to identify the device.
*/
#define HMC5883_REG_ID_C                          (0x0c)

/** \brief Temperature output register (HMC5983).
**
**  \par Description:
**       MSB for temperature measurements.
*/
#define HMC5983_REG_TEMP_OUT_MSB                  (0x31)

/** \brief Temperature output register (HMC5983).
**
**  \par Description:
**       LSB for temperature measurements.
*/
#define HMC5983_REG_TEMP_OUT_LSB                  (0x32)

/* HMC5883 mask */
/** \brief Continuous measurement mode with a rate of 75Hz.
**
**  \par Description:
**       Sets the maximum rate for continuous measurement mode.
*/
//#define HMC5883_BITS_CONFIG_A_CONTINUOUS_75HZ     (0x6 << 2)

#define HMC5883_BITS_CONFIG_A_DEFAULT             (0)

/** \brief Continuous measurement mode. 
**
**  \par Description:
**       Operating mode continuous-measurement mode.
*/
#define HMC5883_BITS_MODE_CONTINUOUS_MODE         (0x00)

/** \brief Single measurement mode.
**
**  \par Description:
**       Operating mode single-measurement mode.
*/
#define HMC5883_BITS_MODE_SINGLE_MODE             (0x01)

/* Retry settings */
/** \brief Retry attemps for interrupted ioctl calls.
**
**  \par Limits:
**       None.
*/
#define HMC5883_MAX_RETRY_ATTEMPTS                (5)

/** \brief Sleep time micro seconds for interrupted calls.
**
**  \par Limits:
**       None.
*/
#define HMC5883_MAX_RETRY_SLEEP_USEC              (10)


/************************************************************************
** Structure Declarations
*************************************************************************/
/**
 * \brief HMC5883 device status
 */
typedef enum
{
    /*! HMC5883 status uninitialized */
    HMC5883_CUSTOM_UNINITIALIZED  = 0,
    /*! HMC5883 status initialized */
    HMC5883_CUSTOM_INITIALIZED   = 1
} HMC5883_Custom_Status_t;


typedef struct
{
    /*! Device file descriptor */
    int                             DeviceFd;
    /*! The current device status */
    HMC5883_Custom_Status_t          Status;
} HMC5883_AppCustomData_t;


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
int32 HMC5883_Ioctl(int fh, int request, void *arg);

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
boolean HMC5883_Custom_Max_Events_Not_Reached(int32 ind);


/************************************************************************/
/** \brief Receives data via I2C from the HMC5883.
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
boolean HMC5883_Custom_Receive(uint8 Reg, uint8 *Buffer, size_t Length);


/************************************************************************/
/** \brief Send data via I2C to the HMC5883.
**
**  \par Description
**       This function sends data via I2C to the device.
**
**  \par Assumptions, External Events, and Notes:
**       The device must be initialized before this function is called.
**
**  \param [in]    Reg    The target (recipient) register.
**
**  \param [in]    Data   The data to send.                             
**
**  \returns    boolean   True for success, false for failure. 
**
*************************************************************************/
boolean HMC5883_Custom_Send(uint8 Reg, uint8 Data);


#ifdef __cplusplus
}
#endif 

#endif /* HMC5883_I2C_H */
