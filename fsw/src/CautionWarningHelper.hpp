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
    CAWS_FAILSAFE            = 8
} CAWS_States_t;


class CautionWarningHelper
{
private:
    /* Helper state */
    uint8  m_State;
    /* LED Control Message */
    PX4_LedControlMsg_t m_LedControlMsg;
    /* Private functions */
    void RGBLedSetColorAndMode(uint8 color, uint8 mode, uint8 blinks, uint8 prio);
    /* Init CAWS RGBLED component */
    void InitRGBLED(void);
    /* Deinit CAWS RGBLED component */
    void DeInitRGBLED(void);

public:
    CautionWarningHelper();
    ~CautionWarningHelper();
    /* Initialize caution and warning */
    void InitCAWS(void);
    /* Initialize caution and warning */
    void DeInitCAWS(void);
    /* Get the current CAWS state */
    uint8 GetState(void);
    /* Set the current CAWS state */
    void SetState(uint8 state);

protected:

};



#endif /* CAUTIONWARNINGHELPER_HPP */
