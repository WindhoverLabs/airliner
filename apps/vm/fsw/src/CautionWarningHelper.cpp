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

#include "CautionWarningHelper.hpp"
#include "px4lib.h"
#include "px4lib_msgids.h"


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/*  CautionWarningHelper default constructor                       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
CautionWarningHelper::CautionWarningHelper() :
    m_State(CAWS_UNINITIALIZED)
{
} /* End CautionWarningHelper::CautionWarningHelper() */



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/*  CautionWarningHelper destructor                                */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
CautionWarningHelper::~CautionWarningHelper()
{
} /* End CautionWarningHelper::~CautionWarningHelper() */



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/*  Initialize Caution and Warning System                          */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CautionWarningHelper::InitCAWS(void)
{
    m_LedControlMsg.Color       = LED_COLOR_OFF;
    m_LedControlMsg.LedMask     = 0;
    m_LedControlMsg.Mode        = LED_MODE_OFF;
    m_LedControlMsg.NumBlinks   = 0;
    m_LedControlMsg.Priority    = 0;
    /* Set state to initialized */
    m_State = CAWS_INITIALIZED;
    InitRGBLED();
} /* End CautionWarningHelper::InitCAWS() */



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/*  De-initialize Caution and Warning System                       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CautionWarningHelper::DeInitCAWS(void)
{
    InitCAWS();
    /* Set state to uninitialized */
    m_State = CAWS_UNINITIALIZED;
    /* Deinit CAWS components */
    DeInitRGBLED();
} /* End CautionWarningHelper::DeInitCAWS() */



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/*  Initialize RGB LED                                             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CautionWarningHelper::InitRGBLED(void)
{
    CFE_SB_InitMsg(&m_LedControlMsg, PX4_LED_CONTROL_MID, 
            sizeof(m_LedControlMsg), TRUE);
    CFE_SB_TimeStampMsg((CFE_SB_MsgPtr_t)&m_LedControlMsg);
    m_LedControlMsg.LedMask = 0xFF;
    m_LedControlMsg.Color = LED_COLOR_OFF;
    m_LedControlMsg.Mode = LED_MODE_OFF;
    m_LedControlMsg.NumBlinks = 0;
    m_LedControlMsg.Priority = 0;
} /* End CautionWarningHelper::InitRGBLED() */



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/*  De-initialize RGB LED                                          */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CautionWarningHelper::DeInitRGBLED(void)
{
    /* Init and deinit are the same */
    InitRGBLED();
} /* End CautionWarningHelper::DeInitRGBLED() */



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/*  Get current state of the Caution and Warning System            */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
CAWS_States_t CautionWarningHelper::GetState(void)
{
    return m_State;
} /* End CautionWarningHelper::GetState() */



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/*  Set current state of the Caution and Warning System            */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
osalbool CautionWarningHelper::SetState(CAWS_States_t state)
{
    osalbool returnBool = TRUE;

    switch(state)
    {
        case CAWS_HOME_SET: 
        {
            RGBLedSetColorAndMode(LED_COLOR_GREEN, LED_MODE_BLINK_FAST, 0, 0);
            m_State = state;
            break;
        }

        case CAWS_MISSION_OK: 
        {
            RGBLedSetColorAndMode(LED_COLOR_GREEN, LED_MODE_BLINK_FAST, 0, 0);
            m_State = state;
            break;
        }

        case CAWS_MISSION_FAIL:
        {
            RGBLedSetColorAndMode(LED_COLOR_GREEN, LED_MODE_BLINK_FAST, 0, 0);
            m_State = state;
            break;
        }

        case CAWS_POSITIVE:
        {
            RGBLedSetColorAndMode(LED_COLOR_GREEN, LED_MODE_BLINK_FAST, 0, 0);
            m_State = state;
            break;
        }

        case CAWS_NEUTRAL:
        {
            RGBLedSetColorAndMode(LED_COLOR_WHITE, LED_MODE_BLINK_FAST, 0, 0);
            m_State = state;
            break;
        }

        case CAWS_NEGATIVE:
        {
            RGBLedSetColorAndMode(LED_COLOR_RED, LED_MODE_BLINK_FAST, 0, 0);
            m_State = state;
            break;
        }

        case CAWS_FAILSAFE:
        {
            RGBLedSetColorAndMode(LED_COLOR_PURPLE, LED_MODE_BLINK_FAST, 0, 0);
            m_State = state;
            break;
        }

        case CAWS_ALTCTL:
        {
            RGBLedSetColorAndMode(LED_COLOR_YELLOW, LED_MODE_BLINK_FAST, 0, 0);
            m_State = state;
            break;
        }

        case CAWS_POSCTL:
        {
            RGBLedSetColorAndMode(LED_COLOR_AMBER, LED_MODE_BLINK_FAST, 0, 0);
            m_State = state;
            break;
        }

        case CAWS_AUTO:
        {
            RGBLedSetColorAndMode(LED_COLOR_BLUE, LED_MODE_BLINK_FAST, 0, 0);
            m_State = state;
            break;
        }

        default:
        {
            returnBool = FALSE;
            break;
        }
    }

    return returnBool;
} /* End CautionWarningHelper::SetState() */



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/*  Set status of the Caution and Warning System from the          */
/*  PX4_VehicleStatusMsg.                                          */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
osalbool CautionWarningHelper::SetStatus(const PX4_VehicleStatusMsg_t *status)
{
    osalbool returnBool = TRUE;
    osalbool decodeNavState = TRUE;
    /* Initialize final state to the current state */
    CAWS_States_t finalState = m_State;
    static uint8 callCount = 0;
    
    /* Currently this function is called from VM wakeup so throttle the rate */
    if (callCount >= 250)
    {
        callCount = 0;
        goto end_of_function;
    }
    callCount++;
    
    if (0 == status)
    {
        returnBool = FALSE;
        goto end_of_function;
    }
    
    if(TRUE == status->RcSignalLost)
    {
        finalState = CAWS_FAILSAFE;
        goto end_of_function;
    }

    /* Decode armed state */
    switch(status->ArmingState)
    {
        case PX4_ARMING_STATE_INIT: 
        {
            finalState = CAWS_NEUTRAL;
            decodeNavState = FALSE;
            break;
        }

        case PX4_ARMING_STATE_STANDBY: 
        {
            finalState = CAWS_NEUTRAL;
            decodeNavState = FALSE;
            break;
        }

        case PX4_ARMING_STATE_ARMED:
        {
            finalState = CAWS_POSITIVE;
            decodeNavState = TRUE;
            break;
        }

        case PX4_ARMING_STATE_ARMED_ERROR:
        {
            finalState = CAWS_NEGATIVE;
            decodeNavState = FALSE;
            break;
        }

        case PX4_ARMING_STATE_STANDBY_ERROR:
        {
            finalState = CAWS_NEGATIVE;
            decodeNavState = FALSE;
            break;
        }

        case PX4_ARMING_STATE_REBOOT:
        {
            finalState = CAWS_NEGATIVE;
            decodeNavState = FALSE;
            break;
            
        }

        case PX4_ARMING_STATE_IN_AIR_RESTORE:
        {
            finalState = CAWS_NEGATIVE;
            decodeNavState = FALSE;
            break;
        }

        default:
        {
            returnBool = FALSE;
            decodeNavState = FALSE;
            break;
        }
    }
    
    if(TRUE == decodeNavState)
    {
        /* Next decode navigation state */
        switch(status->NavState)
        {
            case PX4_NAVIGATION_STATE_MANUAL:
            {
                /* Do nothing for state manual */
                break;
            }

            case PX4_NAVIGATION_STATE_ALTCTL:
            {
                finalState = CAWS_ALTCTL;
                break;
            }

            case PX4_NAVIGATION_STATE_POSCTL:
            {
                finalState = CAWS_POSCTL;
                break;
            }

            case PX4_NAVIGATION_STATE_AUTO_MISSION:
            {
                finalState = CAWS_AUTO;
                break;
            }

            case PX4_NAVIGATION_STATE_AUTO_LOITER:
            {
                finalState = CAWS_AUTO;
                break;
            }

            case PX4_NAVIGATION_STATE_AUTO_RTL:
            {
                finalState = CAWS_FAILSAFE;
                break;
            }

            case PX4_NAVIGATION_STATE_AUTO_RCRECOVER:
            {
                finalState = CAWS_AUTO;
                break;
            }

            case PX4_NAVIGATION_STATE_AUTO_RTGS:
            {
                finalState = CAWS_AUTO;
                break;
            }

            case PX4_NAVIGATION_STATE_AUTO_LANDENGFAIL:
            {
                finalState = CAWS_AUTO;
                break;
            }

            case PX4_NAVIGATION_STATE_AUTO_LANDGPSFAIL:
            {
                finalState = CAWS_AUTO;
                break;
            }

            case PX4_NAVIGATION_STATE_ACRO:
            case PX4_NAVIGATION_STATE_UNUSED:
            case PX4_NAVIGATION_STATE_DESCEND:
            case PX4_NAVIGATION_STATE_TERMINATION:
            case PX4_NAVIGATION_STATE_OFFBOARD:
            case PX4_NAVIGATION_STATE_STAB:
            case PX4_NAVIGATION_STATE_RATTITUDE:
            /* Fallthru */
            {
                break;
            }

            case PX4_NAVIGATION_STATE_AUTO_TAKEOFF:
            {
                finalState = CAWS_AUTO;
                break;
            }

            case PX4_NAVIGATION_STATE_AUTO_LAND:
            {
                finalState = CAWS_AUTO;
                break;
            }

            case PX4_NAVIGATION_STATE_AUTO_FOLLOW_TARGET:
            {
                finalState = CAWS_AUTO;
                break;
            }

            case PX4_NAVIGATION_STATE_AUTO_PRECLAND:
            {
                finalState = CAWS_AUTO;
                break;
            }

            default:
            {
                returnBool = FALSE;
                break;
            }
        }
    }

end_of_function:
    /* If the current state is not the decoded state... */
    if(finalState != m_State)
    {
        /* Set the new state */
        SetState(finalState);
    }

    return returnBool;
} /* End CautionWarningHelper::SetState() */



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/*  Set LED color and mode                                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CautionWarningHelper::RGBLedSetColorAndMode(LED_Colors_t color, LED_Modes_t mode, uint8 blinks, uint8 prio)
{
    CFE_SB_TimeStampMsg((CFE_SB_MsgPtr_t)&m_LedControlMsg);
    m_LedControlMsg.Color = color;
    m_LedControlMsg.Mode = mode;
    m_LedControlMsg.NumBlinks = blinks;
    m_LedControlMsg.Priority = prio;

    /* Send the LED control message */
    CFE_SB_SendMsg((CFE_SB_Msg_t*)&m_LedControlMsg);
} /* End CautionWarningHelper::RGBLedSetColorAndMode() */

