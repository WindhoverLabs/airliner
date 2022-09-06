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

#ifndef RCIN_SBUS_H
#define RCIN_SBUS_H

/************************************************************************
** Includes
*************************************************************************/
#include "rcin_custom.h"
#include "px4_msgs.h"

#include <asm-generic/termbits.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif
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

/** \brief Device path.
**
**  \par Description:
**       The serial interface device path.
*/
#define RCIN_DEVICE_PATH                "/dev/ttyS2"

/** \brief Input speed.
**
**  \par Description:
**       The serial input speed.
*/
#define RCIN_SERIAL_INPUT_SPEED         (100000)

/** \brief Output speed.
**
**  \par Description:
**       The serial ouput speed.
*/
#define RCIN_SERIAL_OUTPUT_SPEED        (100000)

/** \brief Size of the raw RCIN message block.
**
**  \par Description:
**       The size of 
*/
#define RCIN_SERIAL_READ_SIZE           (25)

/** \brief Serial c_cc VMIN setting.
**
**  \par Description:
**       Minimum number of characters for noncanonical read (MIN).
*/
#define RCIN_SERIAL_VMIN_SETTING        RCIN_SERIAL_READ_SIZE

/** \brief Serial c_cc VTIME setting.
**
**  \par Description:
**       Timeout in deciseconds for noncanonical read (TIME).
*/
#define RCIN_SERIAL_VTIME_SETTING       (0)

/** \brief SBUS channels
**
**  \par Description:
**       The SBUS channel count.
*/
#define RCIN_SBUS_CHANNEL_COUNT         (10)

/* define range mapping here, -+100% -> 1000..2000 */
#define RCIN_SBUS_RANGE_MIN             (200.0f)
#define RCIN_SBUS_RANGE_MAX             (1800.0f)
#define RCIN_SBUS_TARGET_MIN            (1000.0f)
#define RCIN_SBUS_TARGET_MAX            (2000.0f)
/* pre-calculate the floating point stuff as far as possible at compile time */
#define RCIN_SBUS_SCALE_FACTOR ((RCIN_SBUS_TARGET_MAX - RCIN_SBUS_TARGET_MIN) / (RCIN_SBUS_RANGE_MAX - RCIN_SBUS_RANGE_MIN))
#define RCIN_SBUS_SCALE_OFFSET (int)(RCIN_SBUS_TARGET_MIN - (RCIN_SBUS_SCALE_FACTOR * RCIN_SBUS_RANGE_MIN + 0.5f))


/** \brief Max device path.
**
**  \par Description:
**       None.
*/
#define RCIN_MAX_DEVICE_PATH          OS_MAX_LOCAL_PATH_LEN

/** \brief Retry attemps for interrupted ioctl calls.
**
**  \par Limits:
**       None.
*/
#define RCIN_MAX_RETRY_ATTEMPTS       (2)

/** \brief Sleep time micro seconds for interrupted calls.
**
**  \par Limits:
**       None.
*/
#define RCIN_MAX_RETRY_SLEEP_USEC     (10)

/** \brief RCIN shared data mutex name. */
#define RCIN_MUTEX_NAME                "RCIN_MUTEX"

/************************************************************************
** Structure Declarations
*************************************************************************/

/**
 * \brief Device status
 */
typedef enum
{
    /*! Status uninitialized */
    RCIN_CUSTOM_UNINITIALIZED  = 0,
    /*! Status initialized */
    RCIN_CUSTOM_INITIALIZED   = 1,
    /*! Status initialized */
    RCIN_CUSTOM_ENABLED   = 2
} RCIN_Custom_Status_t;


typedef struct
{
    /*! Device file descriptor */
    int                             DeviceFd;
    /*! Path to device */
    char                            DevName[RCIN_MAX_DEVICE_PATH];
    /*! The terminal configuration */
    struct termios2                 TerminalConfig;
    /*! The current device status */
    RCIN_Custom_Status_t            Status;
    /*! The shared data mutex */
    uint32                          Mutex;
} RCIN_AppCustomData_t;


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
int32 RCIN_Ioctl(int fh, int request, void *arg);


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
boolean RCIN_Custom_Max_Events_Not_Reached(int32 ind);


#ifdef __cplusplus
}
#endif 

#endif /* RCIN_SBUS_H */
