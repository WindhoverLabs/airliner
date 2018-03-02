#include "CautionWarningHelper.hpp"
#include "px4lib.h"


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
    m_LedControlMsg.Timestamp = PX4LIB_GetPX4TimeUs();
    m_LedControlMsg.LedMask = 0xFF;
    m_LedControlMsg.Color = LED_COLOR_OFF;
    m_LedControlMsg.Mode = LED_MODE_OFF;
    m_LedControlMsg.NumBlinks = 0;
    m_LedControlMsg.Priority = 0;

    /* Send the LED control message */
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&m_LedControlMsg);
    CFE_SB_SendMsg((CFE_SB_Msg_t*)&m_LedControlMsg);
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


void CautionWarningHelper::SetState(uint8 state)
{
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
    }
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

