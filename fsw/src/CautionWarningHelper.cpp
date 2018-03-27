#include "CautionWarningHelper.hpp"
#include "px4lib.h"
#include "msg_ids.h"

CautionWarningHelper::CautionWarningHelper() :
    m_State(CAWS_UNINITIALIZED)
{
};


CautionWarningHelper::~CautionWarningHelper()
{
};


void CautionWarningHelper::InitCAWS(void)
{
    m_LedControlMsg.Color       = 0;
    m_LedControlMsg.LedMask     = 0;
    m_LedControlMsg.Mode        = 0;
    m_LedControlMsg.NumBlinks   = 0;
    m_LedControlMsg.Priority    = 0;
    m_LedControlMsg.Timestamp   = 0;
    /* Set state to initialized */
    m_State = CAWS_INITIALIZED;
    InitRGBLED();
}


void CautionWarningHelper::DeInitCAWS(void)
{
    InitCAWS();
    /* Set state to uninitialized */
    m_State = CAWS_UNINITIALIZED;
    /* Deinit CAWS components */
    DeInitRGBLED();
}


void CautionWarningHelper::InitRGBLED(void)
{
    CFE_SB_InitMsg(&m_LedControlMsg, PX4_LED_CONTROL_MID, 
            sizeof(m_LedControlMsg), TRUE);
    m_LedControlMsg.Timestamp = PX4LIB_GetPX4TimeUs();
    m_LedControlMsg.LedMask = 0xFF;
    m_LedControlMsg.Color = LED_COLOR_OFF;
    m_LedControlMsg.Mode = LED_MODE_OFF;
    m_LedControlMsg.NumBlinks = 0;
    m_LedControlMsg.Priority = 0;
}


void CautionWarningHelper::DeInitRGBLED(void)
{
    /* Init and deinit are the same */
    InitRGBLED();
}


uint8 CautionWarningHelper::GetState(void)
{
    return m_State;
}


boolean CautionWarningHelper::SetState(uint8 state)
{
    boolean returnBool = TRUE;

    switch(state)
    {
        case CAWS_HOME_SET: 
        {
            RGBLedSetColorAndMode(LED_COLOR_GREEN, LED_MODE_BLINK_FAST, 0, 0);
            m_State = state;
        }
        case CAWS_MISSION_OK: 
        {
            RGBLedSetColorAndMode(LED_COLOR_GREEN, LED_MODE_BLINK_FAST, 0, 0);
            m_State = state;
        }
        case CAWS_MISSION_FAIL:
        {
            RGBLedSetColorAndMode(LED_COLOR_GREEN, LED_MODE_BLINK_FAST, 0, 0);
            m_State = state;
        }
        case CAWS_POSITIVE:
        {
            RGBLedSetColorAndMode(LED_COLOR_GREEN, LED_MODE_BLINK_FAST, 0, 0);
            m_State = state;
        }
        case CAWS_NEUTRAL:
        {
            RGBLedSetColorAndMode(LED_COLOR_WHITE, LED_MODE_BLINK_FAST, 0, 0);
            m_State = state;
        }
        case CAWS_NEGATIVE:
        {
            RGBLedSetColorAndMode(LED_COLOR_RED, LED_MODE_BLINK_FAST, 0, 0);
            m_State = state;
        }
        case CAWS_FAILSAFE:
        {
            RGBLedSetColorAndMode(LED_COLOR_PURPLE, LED_MODE_BLINK_FAST, 0, 0);
            m_State = state;
        }
        case CAWS_ALTCTL:
        {
            RGBLedSetColorAndMode(LED_COLOR_YELLOW, LED_MODE_BLINK_FAST, 0, 0);
            m_State = state;
        }
        case CAWS_POSCTL:
        {
            RGBLedSetColorAndMode(LED_COLOR_AMBER, LED_MODE_BLINK_FAST, 0, 0);
            m_State = state;
        }
        case CAWS_AUTO:
        {
            RGBLedSetColorAndMode(LED_COLOR_BLUE, LED_MODE_BLINK_FAST, 0, 0);
            m_State = state;
        }
        default:
        {
            returnBool = FALSE;
        }
    }
    return returnBool;
}


boolean CautionWarningHelper::SetStatus(const PX4_VehicleStatusMsg_t *status)
{
    boolean returnBool = TRUE;
    /* Initialize final state to the current state */
    uint8 finalState = m_State;
    static uint8 callCount = 0;
    
    /* TODO, currently this function is called from VM wakeup so
     * throttle the rate */
    if (255 == callCount)
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
            goto end_of_function;
        }
        case PX4_ARMING_STATE_STANDBY: 
        {
            finalState = CAWS_NEUTRAL;
            goto end_of_function;
        }
        case PX4_ARMING_STATE_ARMED:
        {
            finalState = CAWS_POSITIVE;
            break;
        }
        case PX4_ARMING_STATE_ARMED_ERROR:
        {
            finalState = CAWS_NEGATIVE;
            goto end_of_function;
        }
        case PX4_ARMING_STATE_STANDBY_ERROR:
        {
            finalState = CAWS_NEGATIVE;
            goto end_of_function;
        }
        case PX4_ARMING_STATE_REBOOT:
        {
            finalState = CAWS_NEGATIVE;
            goto end_of_function;
            
        }
        case PX4_ARMING_STATE_IN_AIR_RESTORE:
        {
            finalState = CAWS_NEGATIVE;
            goto end_of_function;
        }
        default:
        {
            returnBool = FALSE;
            goto end_of_function;
        }
    }
    
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
        {
            /* TODO */
            break;
        }
        case PX4_NAVIGATION_STATE_UNUSED:
        {
            /* TODO */
            break;
        }
        case PX4_NAVIGATION_STATE_DESCEND:
        {
            /* TODO */
            break;
        }
        case PX4_NAVIGATION_STATE_TERMINATION:
        {
            /* TODO */
            break;
        }
        case PX4_NAVIGATION_STATE_OFFBOARD:
        {
            /* TODO */
            break;
        }
        case PX4_NAVIGATION_STATE_STAB:
        {
            /* TODO */
            break;
        }
        case PX4_NAVIGATION_STATE_RATTITUDE:
        {
            /* TODO */
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
            goto end_of_function;
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
}


void CautionWarningHelper::RGBLedSetColorAndMode(uint8 color, uint8 mode, uint8 blinks, uint8 prio)
{
    m_LedControlMsg.Timestamp = PX4LIB_GetPX4TimeUs();
    m_LedControlMsg.Color = color;
    m_LedControlMsg.Mode = mode;
    m_LedControlMsg.NumBlinks = blinks;
    m_LedControlMsg.Priority = prio;

    /* Send the LED control message */
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&m_LedControlMsg);
    CFE_SB_SendMsg((CFE_SB_Msg_t*)&m_LedControlMsg);
}

