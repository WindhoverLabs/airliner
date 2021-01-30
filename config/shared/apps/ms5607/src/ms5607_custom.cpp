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
#include "cfe.h"
#include "ms5607_events.h"
#include "ms5607_perfids.h"
#include "ms5607_app.h"
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "simlib.h"
#include <math.h>
#include "ms5607_custom.h"

/************************************************************************
** Local Defines
*************************************************************************/

/* Simulated PROM values */
/** \brief Simulated PROM value C1 */
#define MS5607_CUSTOM_C1        (40127)
/** \brief Simulated PROM value C2 */
#define MS5607_CUSTOM_C2        (36924)
/** \brief Simulated PROM value C3 */
#define MS5607_CUSTOM_C3        (23317)
/** \brief Simulated PROM value C4 */
#define MS5607_CUSTOM_C4        (23282)
/** \brief Simulated PROM value C5 */
#define MS5607_CUSTOM_C5        (33464)
/** \brief Simulated PROM value C6 */
#define MS5607_CUSTOM_C6        (28312)


/************************************************************************
** Local Structure Declarations
*************************************************************************/

/** \brief Simulated custom app state */
typedef enum {
    /** \brief State application uninitialized */
    MS5607_CUSTOM_UNINITIALIZED = 0,
    /** \brief State D1 Conversion */
    MS5607_CUSTOM_D1_CONV       = 1,
    /** \brief State D2 Conversion */
    MS5607_CUSTOM_D2_CONV       = 2
} MS5607_Custom_State_t;


typedef struct
{
    /** \brief Simulated custom app state */
    MS5607_Custom_State_t State;
    /** \brief MS5607 simulated D1 output */
    uint32 D1;
    /** \brief MS5607 simulated D2 output */
    uint32 D2;
} MS5607_AppCustomData_t;


typedef enum {

/** \brief <tt> 'MS5607 - ' </tt>
**  \event <tt> 'MS5607 - ' </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when a device resource encounters an 
**  error.
**
*/
    MS5607_DEVICE_ERR_EID = MS5607_EVT_CNT,

/** \brief <tt> 'MS5607 - ' </tt>
**  \event <tt> 'MS5607 - ' </tt>
**  
**  \par Type: Info
**
**  \par Cause:
**
**  This event message is issued when a device successfully complete a
**  self test.
**
*/
    MS5607_DEVICE_INF_EID,
/** \brief Number of custom events 
**
**  \par Limits:
**       int32
*/
    MS5607_CUSTOM_EVT_CNT
} MS5607_CustomEventIds_t;


/************************************************************************
** External Global Variables
*************************************************************************/
extern MS5607 oMS5607;

/************************************************************************
** Global Variables
*************************************************************************/
MS5607_AppCustomData_t MS5607_AppCustomData;

/************************************************************************
** Local Variables
*************************************************************************/

/************************************************************************
** Local Function Definitions
*************************************************************************/

void MS5607_Custom_InitData(void)
{
    /* Set all struct zero values */
    bzero(&MS5607_AppCustomData, sizeof(MS5607_AppCustomData));
}


boolean MS5607_Custom_Init()
{
    return TRUE;
}


boolean MS5607_Custom_Uninit(void)
{
    return TRUE;
}


void MS5607_Critical_Cleanup(void)
{
    return;
}


boolean MS5607_ReadPROM(uint8 Addr, uint16 *returnVal)
{
    switch(Addr)
    {
        case 0:
        {
            /* Reserved for manufacturer, magic number added for sim */
            *returnVal = 777;
            break;
        }

        case 1:
        {
            *returnVal = MS5607_CUSTOM_C1;
            break;
        }
        case 2:
        {
            *returnVal = MS5607_CUSTOM_C2;
            break;
        }
        case 3:
        {
            *returnVal = MS5607_CUSTOM_C3;
            break;
        }
        case 4:
        {
            *returnVal = MS5607_CUSTOM_C4;
            break;
        }
        case 5:
        {
            *returnVal = MS5607_CUSTOM_C5;
            break;
        }
        case 6:
        {
            *returnVal = MS5607_CUSTOM_C6;
            break;
        }
        case 7:
        {
            *returnVal = 0x0F & oMS5607.CRC4(oMS5607.MS5607_Coefficients);
            break;
        }

        default:
        {
            *returnVal = 0;
            break;
        }
    }

    return (TRUE);
}


boolean MS5607_D1Conversion(void)
{
    MS5607_AppCustomData.State = MS5607_CUSTOM_D1_CONV;

    return (TRUE);
}


boolean MS5607_D2Conversion(void)
{
    MS5607_AppCustomData.State = MS5607_CUSTOM_D2_CONV;

    return (TRUE);
}


boolean MS5607_ReadADCResult(uint32 *returnVal)
{
    boolean result = FALSE;

    if(returnVal == 0)
    {
        result = FALSE;
    }
    else
    {
        switch(MS5607_AppCustomData.State)
        {
            case MS5607_CUSTOM_D1_CONV:
            {
                float Altitude = 0.0f;
                float Temperature = 0.0f;
                /* tropospheric properties (0-11km) for standard atmosphere */
                /* temperature at base height in Kelvin */
                const double T1 = 15.0 + 273.15;
                /* temperature gradient in degrees per metre */
                const double a  = -6.5 / 1000;
                /* gravity constant in m/s/s */
                const double g  = 9.80665;
                /* ideal gas constant in J/kg/K */
                const double R  = 287.05;
                /* current pressure at MSL in kPa */
                double p1 = 101325 / 1000.0;
                /* measured pressure in kPa */
                double p = 0.0f;
                /* Difference between actual and reference temperature. */
                int32 dT = 0;
                /* Offset at actual temperature */
                int64 OFF = 0;
                /* Sensitivity at actual temperature */
                int64 SENS = 0;

                /* Get simulation measurements */
                SIMLIB_GetPressureAltitude(&Altitude);
                SIMLIB_GetTemp(&Temperature);

                if(0 == Temperature)
                {
                    /* If the sim isn't publishing temperature */
                    Temperature = 20.0;
                }

                /* Alternate form 
                 * p = p1*pow((a*((T1/a)+Altitude))/T1,(-g/(a*R)));
                 * Inverse operations to get back to raw pressure and
                 * temperature.
                 */
                p = p1*pow(((a*(Altitude)+T1)/T1),(-g/(a*R)));

                dT = (0x800000*((Temperature*100)-2000)) / MS5607_CUSTOM_C6;

                MS5607_AppCustomData.D2 = dT + (MS5607_CUSTOM_C5 << 8);

                OFF = ((int64)MS5607_CUSTOM_C2 << 17)  + (((int64)MS5607_CUSTOM_C4 * dT) >> 6);

                SENS = ((int64)MS5607_CUSTOM_C1 << 16) + (((int64)dT * MS5607_CUSTOM_C3) >> 7);

                MS5607_AppCustomData.D1 = (0x200000*(OFF+0x8000*(p*1000)))/SENS;

                *returnVal = MS5607_AppCustomData.D1;
                result = TRUE;

                break;
            }
            case MS5607_CUSTOM_D2_CONV:
            {
                *returnVal = MS5607_AppCustomData.D2;

                result = TRUE;

                break;
            }
            default:
            {
                *returnVal = 0;

                result = FALSE;

                break;
            }
        }
    }

    return (result);
}


boolean MS5607_Custom_Max_Events_Not_Reached(int32 ind)
{
    boolean returnBool = FALSE;

    if ((ind < CFE_EVS_MAX_EVENT_FILTERS) && (ind > 0))
    {
        returnBool = TRUE;
    }

    return (returnBool);
}


int32 MS5607_Custom_Init_EventFilters(int32 ind, CFE_EVS_BinFilter_t *EventTbl)
{
    int32 customEventCount = ind;
    
    /* Null check */
    if(0 == EventTbl)
    {
        customEventCount = -1;
        goto end_of_function;
    }

    if(TRUE == MS5607_Custom_Max_Events_Not_Reached(customEventCount))
    {
        EventTbl[  customEventCount].EventID = MS5607_DEVICE_ERR_EID;
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

