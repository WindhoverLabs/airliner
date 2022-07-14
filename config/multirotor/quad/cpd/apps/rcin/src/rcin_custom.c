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
/************************************************************************
** Pragmas
*************************************************************************/

/************************************************************************
** Includes
*************************************************************************/
#include "rcin_events.h"
#include "simlib.h"
#include <time.h>
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/joystick.h>
#include "rcin_platform_cfg.h"
#include "rcin_custom.h"
#include <string.h>
#include <math.h>
#include <unistd.h>

/************************************************************************
** Local Defines
*************************************************************************/

#define RCIN_CUSTOM_AXIS_INPUT_MIN_VALUE      (-32767.0f)
#define RCIN_CUSTOM_AXIS_INPUT_MAX_VALUE      (32767.0f)
#define RCIN_CUSTOM_AXIS_OUTPUT_MIN_VALUE     (1000.0f)
#define RCIN_CUSTOM_AXIS_OUTPUT_MAX_VALUE     (2000.0f)
#define RCIN_CUSTOM_JOYSTICK_NAME_MAX_LENGTH  (80)
#define RCIN_CUSTOM_FRSKY_USB_CHAN_COUNT      (16)


typedef struct {
    float InMin;
    float InMax;
    float OutMin;
    float OutMax;
    uint32 OutChannel;
} RCIN_Custom_JoystickChannelMapping_t;

typedef struct {
    float OutMin;
    float OutMax;
    uint32 OutChannel;
} RCIN_Custom_JoystickButtonMapping_t;

RCIN_Custom_JoystickChannelMapping_t RCIN_Custom_JoystickChannelMapping[PX4_RC_INPUT_MAX_CHANNELS] = 
{
    {-32767.0f, 32767.0f, 1000.0f, 2000.0f, 0},
    {-32767.0f, 32767.0f, 1000.0f, 2000.0f, 1},
    {-32767.0f, 32767.0f, 1000.0f, 2000.0f, 2},
    {-32767.0f, 32767.0f, 1000.0f, 2000.0f, 3},
    {-32767.0f, 32767.0f, 1000.0f, 2000.0f, 4},
    {-32767.0f, 32767.0f, 1000.0f, 2000.0f, 5},
    {-32767.0f, 32767.0f, 1000.0f, 2000.0f, 6},
    {-32767.0f, 32767.0f, 1000.0f, 2000.0f, 7}
};

RCIN_Custom_JoystickButtonMapping_t RCIN_Custom_JoystickButtonMapping[PX4_RC_INPUT_MAX_CHANNELS] = 
{
    {1000.0f, 2000.0f, 8},
    {1000.0f, 2000.0f, 9},
    {1000.0f, 2000.0f, 10},
    {1000.0f, 2000.0f, 11},
    {1000.0f, 2000.0f, 12},
    {1000.0f, 2000.0f, 13},
    {1000.0f, 2000.0f, 14},
    {1000.0f, 2000.0f, 15}
};



/************************************************************************
** Local Structure Declarations
*************************************************************************/

typedef enum {

/** \brief <tt> 'RCIN - ' </tt>
**  \event <tt> 'RCIN - ' </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when a device resource encounters an 
**  error.
**
*/
    RCIN_DEVICE_ERR_EID = RCIN_EVT_CNT,

/** \brief Number of custom events 
**
**  \par Limits:
**       int32
*/
    RCIN_CUSTOM_EVT_CNT

} RCIN_CustomEventIds_t;


typedef struct
{
    int FD;
    uint16 Values[PX4_RC_INPUT_MAX_CHANNELS];
} RCIN_CustomData_t;


/************************************************************************
** External Global Variables
*************************************************************************/

/************************************************************************
** Global Variables
*************************************************************************/

/************************************************************************
** Local Variables
*************************************************************************/
RCIN_CustomData_t RCIN_CustomData;


/************************************************************************
** Local Function Definitions
*************************************************************************/
int32 RCIN_Custom_MapJSAxisToRcIn(uint32 inAxis, int32 inValue, uint32 *outValueIndex, uint32 *outValue);
int32 RCIN_Custom_MapJSButtonToRcIn(uint32 inButton, int32 inValue, uint32 *outValueIndex, uint32 *outValue);
CFE_TIME_SysTime_t RCIN_Custom_Get_Time(void);


void RCIN_Custom_InitData(void)
{
    return;
}


boolean RCIN_Custom_Init(void)
{
    boolean rc       = FALSE;
    int numOfAxis    = 0;
    int numOfButtons = 0;
    uint32 i         = 0;
    char nameOfJoystick[RCIN_CUSTOM_JOYSTICK_NAME_MAX_LENGTH];

    /* Initialize the cached value array.  Set all values to the center positions. */
    for(i = 0; i < PX4_RC_INPUT_MAX_CHANNELS; ++i)
    {
        RCIN_CustomData.Values[i] = 0xFFFF;
    }

    /* Now open the joystick device. */
    RCIN_CustomData.FD = open(RCIN_CUSTOM_JOYSTICK_PATH, O_RDONLY, O_NONBLOCK);
    if(RCIN_CustomData.FD < 0)
    {
        OS_printf("RCIN:  Could not bind to joystick at '%s'.  %s.\n", RCIN_CUSTOM_JOYSTICK_PATH, strerror(errno));

        rc = FALSE;
    }
    else
    {
        /* Use non-blocking mode */
        fcntl( RCIN_CustomData.FD, F_SETFL, O_NONBLOCK );

        /* Get the joystick capabilities and present them to the user. */
        ioctl( RCIN_CustomData.FD, JSIOCGAXES, &numOfAxis );
        ioctl( RCIN_CustomData.FD, JSIOCGBUTTONS, &numOfButtons );
        ioctl( RCIN_CustomData.FD, JSIOCGNAME(RCIN_CUSTOM_JOYSTICK_NAME_MAX_LENGTH), &nameOfJoystick );

        OS_printf("RCIN:  Joystick detected: %s\n\t%d axis\n\t%d buttons\n\n",
                nameOfJoystick,
                numOfAxis,
                numOfButtons);

        rc = TRUE;
    }

    return (rc);
}


int32 RCIN_Custom_MapJSAxisToRcIn(uint32 inAxis, int32 inValue, uint32 *outValueIndex, uint32 *outValue)
{
    int32 rc     = -1;
    float fValue = 0.0f;

    /* Check inputs. */
    if(outValueIndex == 0)
    {
        rc = -1;
        goto end_of_function;
    }

    if(outValue == 0)
    {
        rc = -1;
        goto end_of_function;
    }

    /* Just map Axis to Value Index directly, for now. */
    *outValueIndex = RCIN_Custom_JoystickChannelMapping[inAxis].OutChannel;


    /* Use the mapping equation:  Y = (X-A)/(B-A) * (D-C) + C */
    fValue = (inValue - RCIN_Custom_JoystickChannelMapping[inAxis].InMin) /
            (RCIN_Custom_JoystickChannelMapping[inAxis].InMax- RCIN_Custom_JoystickChannelMapping[inAxis].InMin) *
            (RCIN_Custom_JoystickChannelMapping[inAxis].OutMax - RCIN_Custom_JoystickChannelMapping[inAxis].OutMin) +
            RCIN_Custom_JoystickChannelMapping[inAxis].OutMin;

    *outValue = fValue;

    /* Constrain the value to between the output minimum and maximum
     * values.
     */
    if(*outValue < RCIN_Custom_JoystickChannelMapping[inAxis].OutMin)
    {
        *outValue = RCIN_Custom_JoystickChannelMapping[inAxis].OutMin;
    }

    if(*outValue > RCIN_Custom_JoystickChannelMapping[inAxis].OutMax)
    {
        *outValue = RCIN_Custom_JoystickChannelMapping[inAxis].OutMax;
    }

    rc = 0;

end_of_function:

    return (rc);
}


int32 RCIN_Custom_MapJSButtonToRcIn(uint32 inButton, int32 inValue, uint32 *outValueIndex, uint32 *outValue)
{
    int32 rc = -1;

    /* Check inputs. */
    if(outValueIndex == 0)
    {
        rc = -1;
        goto end_of_function;
    }

    if(outValue == 0)
    {
        rc = -1;
        goto end_of_function;
    }
    
    /* Just map Axis to Value Index directly, for now. */
    *outValueIndex = RCIN_Custom_JoystickButtonMapping[inButton].OutChannel;

    if(inValue == 0) /* Off position */
    {
        *outValue = RCIN_Custom_JoystickButtonMapping[inButton].OutMin;
    }
    else if(inValue == 1) /* On position */
    {
        *outValue = RCIN_Custom_JoystickButtonMapping[inButton].OutMax;
    }
    else /* Invalid position */
    {
        *outValue = RCIN_CUSTOM_AXIS_OUTPUT_MAX_VALUE + 1;
    }

    rc = 0;

end_of_function:

    return (rc);
}


boolean RCIN_Custom_Measure(PX4_InputRcMsg_t *Measure)
{
    // error Count currently handled outside measure
    //uint16 errorCount = 0;
    int bytesRead                 = 0;
    boolean returnBool            = TRUE;
    uint32 i                      = 0;
    uint8 RSSI                    = 100;
    uint16 ChannelCount           = RCIN_CUSTOM_FRSKY_USB_CHAN_COUNT;
    static uint32 totalFrameCount = 0;
    struct js_event js;
    uint16 Channel[RCIN_CUSTOM_FRSKY_USB_CHAN_COUNT];
    int readStatus;


    /* Null pointer check */
    if (0 == Measure)
    {
        (void) CFE_EVS_SendEvent(RCIN_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "RCIN device read null pointer error");
        returnBool = FALSE;
        goto end_of_function;
    }

    /* Loop until event queue is empty.  At the start of each iteration,
     * read the joystick state. */
    readStatus = read(RCIN_CustomData.FD, &js, sizeof(struct js_event));
    while(readStatus > 0)
    {
        /* Act on what the event is. */
        switch (js.type & ~JS_EVENT_INIT)
        {
            case JS_EVENT_AXIS:
            {
                int32 rc;
                uint32 valueIndex = 0;
                uint32 value = 0;

                /* Convert the JS axis number to the RC Input index, and the
                 * JS position to the equivalent RC Input position for the
                 * hardware we are emulating.  This allows us to accurately
                 * emulate the input from the real RC hardware.
                 */
                RCIN_Custom_MapJSAxisToRcIn(js.number, js.value, &valueIndex, &value);

                /* Now, only assign the value if it will fit.  Make sure the
                 * axis number is less than the maximum channels.
                 */
                if(valueIndex < PX4_RC_INPUT_MAX_CHANNELS)
                {
                    RCIN_CustomData.Values[valueIndex] = value;
                }
                break;
            }
            case JS_EVENT_BUTTON:
            {
                int32 rc;
                uint32 valueIndex = 0;
                uint32 value = 0;

                /* Convert the JS axis number to the RC Input index, and the
                 * JS position to the equivalent RC Input position for the
                 * hardware we are emulating.  This allows us to accurately
                 * emulate the input from the real RC hardware.
                 */
                RCIN_Custom_MapJSButtonToRcIn(js.number, js.value, &valueIndex, &value);

                /* Now, only assign the value if it will fit.  Make sure the
                 * axis number is less than the maximum channels.
                 */
                if(valueIndex < PX4_RC_INPUT_MAX_CHANNELS)
                {
                    RCIN_CustomData.Values[valueIndex] = value;
                }
                break;
            }
        }

        readStatus = read(RCIN_CustomData.FD, &js, sizeof(struct js_event));
    }

    /* Now copy the cached values on the struct passed by the caller. */
    for(i = 0; i < PX4_RC_INPUT_MAX_CHANNELS; ++i)
    {
        Measure->Values[i] = RCIN_CustomData.Values[i];
        //OS_printf("%i: %i    ", i, Measure->Values[i]);
    }
    //OS_printf("\n");

    /* Channel count */
    Measure->ChannelCount      = RCIN_CUSTOM_FRSKY_USB_CHAN_COUNT;
    Measure->RSSI              = RSSI;
    // For now handle outside measure function call.
    //Measure->RcLostFrameCount = errorCount;
    Measure->RcTotalFrameCount = totalFrameCount;
    Measure->RcPpmFrameLength  = 0;
    Measure->RcFailsafe        = FALSE;
    Measure->RcLost            = FALSE;
    Measure->InputSource       = PX4_RC_INPUT_SOURCE_PX4IO_SBUS;
    
    totalFrameCount++;

end_of_function:
    return (returnBool);
}


boolean RCIN_Custom_Uninit(void)
{
    return (TRUE);
}


boolean RCIN_Custom_Max_Events_Not_Reached(int32 ind)
{
    boolean returnBool = FALSE;

    if ((ind < RCIN_MAX_EVENT_FILTERS) && (ind > 0))
    {
        returnBool = TRUE;
    }

    return (returnBool);
}


int32 RCIN_Custom_Init_EventFilters(int32 ind, CFE_EVS_BinFilter_t *EventTbl)
{
    int32 customEventCount = ind;
    
    /* Null check */
    if(0 == EventTbl)
    {
        customEventCount = -1;
        goto end_of_function;
    }

    if(TRUE == RCIN_Custom_Max_Events_Not_Reached(customEventCount))
    {
        EventTbl[  customEventCount].EventID = RCIN_DEVICE_ERR_EID;
        EventTbl[customEventCount++].Mask    = CFE_EVS_FIRST_16_STOP;
    }
    else
    {
        customEventCount = -1;
        goto end_of_function;
    }
    
end_of_function:

    return (customEventCount);
}
