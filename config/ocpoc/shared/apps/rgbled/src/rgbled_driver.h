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

#ifndef RGBLED_DRIVER_H
#define RGBLED_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************
** Includes
*************************************************************************/
#include "rgbled_custom.h"
#include "priorities.h"

/************************************************************************
** Local Defines
*************************************************************************/
/** \brief I2C slave address of the TCA62724FMG LED driver in hex.
**
**  \par Description:
**       8-bit address in little-endian format is 1010101(R/W) where R/W
**       bit-0 indicates read (bit set to "H") or write mode (bit set to 
**       "L"). This is the 7-bit address without the R/W bit.
*/
#define RGBLED_I2C_ADDRESS              (0x55)

/** \brief I2C address write mode.
**
**  \par Description:
**       Bit-0 of the I2C slave address for write mode. 
*/
#define RGBLED_I2C_ADDRESS_WRITE        (0x00)

/** \brief I2C address read mode.
**
**  \par Description:
**       Bit-0 of the I2C slave address for read mode. 
*/
#define RGBLED_I2C_ADDRESS_READ         (0x01)

/** \brief I2C sub-address for PWM Output 0 connected to the blue LED.
**
**  \par Description:
**       8-bit address in little-endian format is (AI)0000001 where AI
**       is the auto-increment OFF (bit set to "H") or auto-increment ON
**       (bit set to "L"). This is the 8-bit address with AI OFF.
*/
#define RGBLED_I2C_SUB_ADDR_PWM0        (0x81)

/** \brief I2C sub-address for PWM Output 1 connected to the green LED.
**
**  \par Description:
**       8-bit address in little-endian format is (AI)0000010 where AI
**       is the auto-increment OFF (bit set to "H") or auto-increment ON
**       (bit set to "L"). This is the 8-bit address with AI OFF.
*/
#define RGBLED_I2C_SUB_ADDR_PWM1        (0x82)

/** \brief I2C sub-address for PWM Output 2 connected to the red LED.
**
**  \par Description:
**       8-bit address in little-endian format is (AI)0000011 where AI
**       is the auto-increment OFF (bit set to "H") or auto-increment ON
**       (bit set to "L"). This is the 8-bit address with AI OFF.
*/
#define RGBLED_I2C_SUB_ADDR_PWM2        (0x83)

/** \brief I2C sub-address for settings.
**
**  \par Description:
**       8-bit address in little-endian format is (AI)0000100 where AI
**       is the auto-increment OFF (bit set to "H") or auto-increment ON
**       (bit set to "L"). This is the 8-bit address with AI OFF.
*/
#define RGBLED_I2C_SUB_ADDR_SETTINGS    (0x84)

/** \brief I2C setting NOT powersave mode (NOT SHDN).
**
**  \par Description:
**       Bit-0 of the "ENABLE/NOT SHDN DATA" byte. Set to "H" output 
**       blinks at PWM0, PWM1, PWM2 rate. 
*/
#define RGBLED_I2C_SET_NOT_POWERSAVE    (0x01)

/** \brief I2C setting powersave mode (NOT SHDN).
**
**  \par Description:
**       Bit-0 of the "ENABLE/NOT SHDN DATA" byte. Set to "L" 
**       power-saving mode.
*/
#define RGBLED_I2C_SET_POWERSAVE        (0x00)

/** \brief I2C setting enable (ENABLE).
**
**  \par Description:
**       Bit-1 of the "ENABLE/NOT SHDN DATA" byte. Set to "H" output
**       blinks at PWM0, PWM1, PWM2 rate.
*/
#define RGBLED_I2C_SET_ENABLE           (0x02)

/** \brief I2C setting NOT enable (ENABLE).
**
**  \par Description:
**       Bit-1 of the "ENABLE/NOT SHDN DATA" byte. Set to "L" output
**       is OFF.
*/
#define RGBLED_I2C_SET_DISABLE          (0x00)

/** \brief Max brightness setting for an LED (max PWM duty cycle).
**
**  \par Description:
**       The PWM ON Duty DATA byte uses 4-bits (bit-3 through bit 0) to 
**       set brightness.
*/
#define RGBLED_MAX_BRIGHTNESS           (15)

/** \brief Device path.
**
**  \par Description:
**       The I2C interface device path. The "-1" is the adapter number. 
*/
#define RGBLED_DEVICE_PATH              "/dev/i2c-1"

/** \brief Max device path.
**
**  \par Description:
**       None.
*/
#define RGBLED_MAX_DEVICE_PATH          OS_MAX_LOCAL_PATH_LEN

/** \brief Retry attemps for interrupted ioctl calls.
**
**  \par Limits:
**       None.
*/
#define RGBLED_MAX_RETRY_ATTEMPTS       (2)

/** \brief Sleep time micro seconds for interrupted calls.
**
**  \par Limits:
**       None.
*/
#define RGBLED_MAX_RETRY_SLEEP_USEC     (10)

/** \brief Read data, from slave to master.
**
**  \par Limits:
**       None.
*/
#define RGBLED_I2C_M_READ               (0x0001)
/** \brief Streaming task priority
**
**  \par Limits:
**       0 to MAX_PRIORITY (usually 255)
*/
//#define RGBLED_SELFTEST_TASK_PRIORITY    (50)

/** \brief Streaming task name
**
**  \par Limits:
**       OS_MAX_API_NAME
*/
#define RGBLED_SELFTEST_TASK_NAME        ("RGBLED_TEST")

/************************************************************************
** Structure Declarations
*************************************************************************/

/**
 * \brief LED device status
 */
typedef enum
{
    /*! RGBLED status uninitialized */
    RGBLED_CUSTOM_UNINITIALIZED  = 0,
    /*! RGBLED status initialized */
    RGBLED_CUSTOM_INITIALIZED   = 1,
    /*! RGBLED status enabled */
    RGBLED_CUSTOM_ENABLED       = 2
} RGBLED_Custom_Status_t;


/**
 * \brief LED device settings
 */
typedef struct
{
    /*! RGBLED device enabled */
    boolean     Enabled;
    /*! RGBLED NOT powersave */
    boolean     NotPowerSave;
    /*! RGBLED duty cycle of red LED */
    uint8       RedDutyCycle;
    /*! RGBLED duty cycle of green LED */
    uint8       GreenDutyCycle;
    /*! RGBLED duty cycle of blue LED */
    uint8       BlueDutyCycle;
} RGBLED_Device_Settings_t;


typedef struct
{
    /*! Device file descriptor */
    int                             DeviceFd;
    /*! Path to device */
    char                            DevName[RGBLED_MAX_DEVICE_PATH];
    /*! The current device settings */
    RGBLED_Device_Settings_t        Settings;
    /*! The current device status */
    RGBLED_Custom_Status_t          Status;
    /*! Streaming child task identifier */
    uint32                          ChildTaskID;
    /*! Streaming task function pointer */
    CFE_ES_ChildTaskMainFuncPtr_t   SelfTestTask;
} RGBLED_AppCustomData_t;


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
int32 RGBLED_Ioctl(int fh, int request, void *arg);

/************************************************************************/
/** \brief Send via I2C to the device.
**
**  \par Description
**       Sends data to the device target (recipient).
**
**  \param [in] Buffer    The buffer to send.
**
**  \param [in] Length    The length of the buffer to write.
**
**  \returns TRUE for success, FALSE for failure.
**
*************************************************************************/
boolean RGBLED_Custom_Send(uint8 *Buffer, size_t Length);

/************************************************************************/
/** \brief Receive data via I2C from the device.
**
**  \par Description
**       Receives data to the device target (sender).
**
**  \param [out] Buffer    The buffer to write to.
**
**  \param [in] Length    The length of the buffer to read.
**
**  \returns TRUE for success, FALSE for failure.
**
*************************************************************************/
boolean RGBLED_Custom_Receive(uint8 *Buffer, size_t Length);

/************************************************************************/
/** \brief Start a self-test.
**
**  \par Description
**       Starts a self-test and returns to the device to its original
**       state.
**
*************************************************************************/
void RGBLED_Custom_SelfTest_Task(void);

/************************************************************************/
/** \brief Get the current state of the device.
**
**  \par Description
**       Gets the current device settings.
**
**  \returns TRUE for success, FALSE for failure.
**
*************************************************************************/
boolean RGBLED_Custom_GetSettings(void);

/************************************************************************/
/** \brief Validate the device settings.
**
**  \par Description
**       Validate the current device settings.
**
**  \returns TRUE for success, FALSE for failure.
**
*************************************************************************/
boolean RGBLED_Custom_Validate(void);

/************************************************************************/
/** \brief Enable the device.
**
**  \par Description
**       Enable the device.
**
**  \returns TRUE for success, FALSE for failure.
**
*************************************************************************/
boolean RGBLED_Custom_Enable(void);

/************************************************************************/
/** \brief Disable the device.
**
**  \par Description
**       Disable the device.
**
**  \returns TRUE for success, FALSE for failure.
**
*************************************************************************/
boolean RGBLED_Custom_Disable(void);

#ifdef __cplusplus
}
#endif 

#endif /* RGBLED_DRIVER_H */
