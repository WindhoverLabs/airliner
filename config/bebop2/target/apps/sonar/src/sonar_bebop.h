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

#ifndef SONAR_BEBOP_H
#define SONAR_BEBOP_H

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************
** Includes
*************************************************************************/
#include "sonar_custom.h"

/************************************************************************
** Local Defines
*************************************************************************/
/** \brief Measure interval.
**
**  \par Description:
**       Update frequency 16.66 Hz (reading 8192 samples at 160 kHz
**       51200 us).
*/
#define SONAR_MEASURE_INTERVAL_US               (60000)

/** \brief Device path. */
#define SONAR_DEVICE_PATH                       ("/dev/spidev1.0")

/** \brief Device ADC path. */
#define SONAR_ADC_DEVICE_PATH                   ("/sys/bus/iio/devices/iio:device0")

/** \brief ADC Channel. */
#define SONAR_ADC_CHANNEL                       ("scan_elements/in_voltage2_en")

#define SONAR_MAX_DISTANCE                      (8.5f)

#define SONAR_BUFFER_LEN                        (8192)

#define SONAR_PULSE_LEN                         (32)

#define SONAR_SPEED_OF_SOUND                    (343.2f)

#define SONAR_ADC_SAMPLING_FREZ_HZ              (160000.0f)

/** \brief Request threshold.
**
**  \par Description:
**       Threshold to detect the beginning of the send pulse.
*/
#define SONAR_REQUEST_THRESHOLD                (3276)

/** \brief Send pulse length.
**
**  \par Description:
**       The mic records the transmitted signal at the beginning of 
**       received signal and defines when it should have ended. Used to
**       detect if the vehicle is bellow the block distance. 
*/
#define SONAR_SEND_PULSE_LEN                    (420)

/** \brief Noise level threshold.
**
**  \par Description:
**       Defines the end of the transmitted signal. A valid received
**       signal must be above this value to be accepted.
*/
#define SONAR_NOISE_LEVEL_THRESHOLD             (1000)

/** \brief SPI Device path of the SONAR */
#define SONAR_SPI_DEVICE_PATH              SONAR_DEVICE_PATH

/** \brief SPI device mode */
#define SONAR_SPI_DEVICE_MODE              (3)

/** \brief SPI device bits per word */
#define SONAR_SPI_DEVICE_BITS              (8)

/** \brief SPI device speed */
#define SONAR_SPI_DEVICE_SPEED             (320000)

/** \brief Retry attemps for interrupted ioctl calls. */
#define SONAR_MAX_RETRY_ATTEMPTS           (2)

/** \brief Sleep time micro seconds for interrupted calls. */
#define SONAR_MAX_RETRY_SLEEP_USEC         (10)

#define SONAR_SPI_DIR_READ                 (0x80)

#define SONAR_SPI_DIR_WRITE                (0x00)

/************************************************************************
** Structure Declarations
*************************************************************************/
/**
 * \brief Sonar device status
 */
typedef enum
{
    /*! status uninitialized */
    SONAR_CUSTOM_UNINITIALIZED  = 0,
    /*! status initialized */
    SONAR_CUSTOM_INITIALIZED    = 1
} SONAR_Custom_Status_t;


typedef struct
{
    /*! Device file descriptor */
    int                         DeviceFd;
    /*! The current device status */
    SONAR_Custom_Status_t       Status;
    uint8                       SendPulse[SONAR_PULSE_LEN];
    uint16                      ReadBuffer[SONAR_BUFFER_LEN];
    uint16                      FilteredBuffer[SONAR_BUFFER_LEN];
    uint16                      SendLength;
    uint16                      MaximumSignalVal;
    boolean                     BufferEnabled;
    char                        ADCDevicePath[50];
} SONAR_AppCustomData_t;

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
int32 SONAR_Ioctl(int fh, int request, void *arg);

void SONAR_Custom_InitData(void);

int SONAR_ADC_Init(void);

int SONAR_ADC_Enable(void);

int SONAR_ADC_Disable(void);

void SONAR_Custom_InitData(void);

int SONAR_ADC_Read(uint16 *buffer, uint16 length);

int SONAR_ADC_Write(const char *path, int value);


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
boolean SONAR_Custom_Max_Events_Not_Reached(int32 ind);


#ifdef __cplusplus
}
#endif 

#endif /* SONAR_BEBOP_H */
