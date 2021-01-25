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

#ifndef CAUTIONWARNINGHELPER_HPP
#define CAUTIONWARNINGHELPER_HPP

#include "cfe.h"
#include "px4_msgs.h"


/**
 * \brief LED colors
 */
typedef enum
{
    LED_COLOR_OFF    = 0,
    LED_COLOR_RED    = 1,
    LED_COLOR_GREEN  = 2,
    LED_COLOR_BLUE   = 3,
    LED_COLOR_AMBER  = 4,
    LED_COLOR_YELLOW = 5,
    LED_COLOR_PURPLE = 6,
    LED_COLOR_CYAN   = 7,
    LED_COLOR_WHITE  = 8
} LED_Colors_t;


/**
 * \brief LED modes
 */
typedef enum
{
    LED_MODE_OFF          = 0,
    LED_MODE_ON           = 1,
    LED_MODE_DISABLED     = 2,
    LED_MODE_BLINK_SLOW   = 3,
    LED_MODE_BLINK_NORMAL = 4,
    LED_MODE_BLINK_FAST   = 5,
    LED_MODE_BREATHE      = 6
} LED_Modes_t;


/**
 * \brief Caution and Warning states
 */
typedef enum
{
    CAWS_UNINITIALIZED       = 0,
    CAWS_INITIALIZED         = 1,
    CAWS_HOME_SET            = 2,
    CAWS_MISSION_OK          = 3,
    CAWS_MISSION_FAIL        = 4,
    CAWS_POSITIVE            = 5,
    CAWS_NEUTRAL             = 6,
    CAWS_NEGATIVE            = 7,
    CAWS_FAILSAFE            = 8,
    CAWS_ALTCTL              = 9,
    CAWS_POSCTL              = 10,
    CAWS_AUTO                = 11
} CAWS_States_t;



/************************************************************************/
/** \brief Caution and Warning Helper class.
 **
 **  \par Description
 **       This class provides functions to assist with sending the RGB LED
 **       commands to change colors and modes.
 **
 **  \par Assumptions, External Events, and Notes:
 **        None
 **
 *************************************************************************/
class CautionWarningHelper
{
private:
    /************************************************************************/
    /** \brief Caution and Warning System current state.
     **
     **  \par Description
     **       This contains the current state of the Caution and Warning
     **       System (CAWS).
     **
     **  \par Assumptions, External Events, and Notes:
     **        None
     **
     *************************************************************************/
    CAWS_States_t  m_State;

    /************************************************************************/
    /** \brief LED Control Message.
     **
     **  \par Description
     **       This is the message that will be sent to the RGBLED application
     **       if/when state changes.
     **
     **  \par Assumptions, External Events, and Notes:
     **        None
     **
     *************************************************************************/
    PX4_LedControlMsg_t m_LedControlMsg;

    /************************************************************************/
    /** \brief Set RGB LED color and mode.
     **
     **  \par Description
     **       This is function is used internally to send the command to
     **       the RGB LED application to actually set the color and mode.
     **
     **  \par Assumptions, External Events, and Notes:
     **        None
     **
     *************************************************************************/
    void RGBLedSetColorAndMode(LED_Colors_t color, LED_Modes_t mode, uint8 blinks, uint8 prio);

    /************************************************************************/
    /** \brief Initialize the CAWS RGBLED component.
     **
     **  \par Description
     **       This is function is called internally to initialize internal
     **       variables required by the Caution and Warning System.
     **
     **  \par Assumptions, External Events, and Notes:
     **        None
     **
     *************************************************************************/
    void InitRGBLED(void);

    /************************************************************************/
    /** \brief De-initialize the CAWS RGBLED component.
     **
     **  \par Description
     **       This is function is called internally to deinitialize internal
     **       variables required by the Caution and Warning System.
     **
     **  \par Assumptions, External Events, and Notes:
     **        None
     **
     *************************************************************************/
    void DeInitRGBLED(void);

    /************************************************************************/
    /** \brief Set the current state of the Caution and Warning System.
     **
     **  \par Description
     **       This is function is called internally to set the state of the
     **       Caution and Warning System.
     **
     **  \par Assumptions, External Events, and Notes:
     **        None
     **
     *************************************************************************/
    osalbool SetState(CAWS_States_t state);

public:

    /************************************************************************/
    /** \brief CautionWarningHelper default constructor.
     **
     **  \par Description
     **       This is the default constructor of the CautionWarningHelper
     **       class.
     **
     **  \par Assumptions, External Events, and Notes:
     **        None
     **
     *************************************************************************/
    CautionWarningHelper();

    /************************************************************************/
    /** \brief CautionWarningHelper destructor.
     **
     **  \par Description
     **       This is the destructor for the CautionWarningHelper
     **       class.
     **
     **  \par Assumptions, External Events, and Notes:
     **        None
     **
     *************************************************************************/
    ~CautionWarningHelper();

    /************************************************************************/
    /** \brief Initialize the Caution and Warning System.
     **
     **  \par Description
     **       This function should be called after object creation, to
     **       initialize the Caution and Warning System.
     **
     **  \par Assumptions, External Events, and Notes:
     **        None
     **
     *************************************************************************/
    void InitCAWS(void);

    /************************************************************************/
    /** \brief Deinitialize the Caution and Warning System.
     **
     **  \par Description
     **       This is an optional function that can be called before the
     **       object is detroyed.
     **
     **  \par Assumptions, External Events, and Notes:
     **        None
     **
     *************************************************************************/
    void DeInitCAWS(void);

    /************************************************************************/
    /** \brief Get Caution and Warning System State
     **
     **  \par Description
     **       This function returns the current state of the Caution and
     **       Warning System.
     **
     **  \par Assumptions, External Events, and Notes:
     **        InitCAWS has been called.
     **
     *************************************************************************/
    CAWS_States_t GetState(void);

    /************************************************************************/
    /** \brief Set Caution and Warning System Status
     **
     **  \par Description
     **       This function sets the Caution and Warning System status from
     **       the PX4_VehicleStatusMsg message.  Call this when a new
     **       PX4_VehicleStatusMsg is received.
     **
     **  \par Assumptions, External Events, and Notes:
     **        InitCAWS has been called.
     **
     *************************************************************************/
    osalbool SetStatus(const PX4_VehicleStatusMsg_t *status);

};


#endif /* CAUTIONWARNINGHELPER_HPP */
