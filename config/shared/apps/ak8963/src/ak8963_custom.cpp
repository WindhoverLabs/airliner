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
#include "ak8963_app.h"
#include "ak8963_i2c.h"
#include "ak8963_events.h"
#include "ak8963_perfids.h"
#include "px4lib.h"
#include "simlib.h"
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <time.h>

extern AK8963 oAK8963;

#define AK8963_SELFTEST_DELAY_TIME (15000)

/************************************************************************
** Local Defines
*************************************************************************/

/************************************************************************
** Local Structure Declarations
*************************************************************************/
typedef enum {

/** \brief <tt> 'AK8963 - ' </tt>
**  \event <tt> 'AK8963 - ' </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when a device resource encounters an 
**  error.
**
*/
    AK8963_DEVICE_ERR_EID = AK8963_EVT_CNT,

/** \brief Number of custom events 
**
**  \par Limits:
**       int32
*/
    AK8963_CUSTOM_EVT_CNT
} AK8963_CustomEventIds_t;


/************************************************************************
** External Global Variables
*************************************************************************/

/************************************************************************
** Global Variables
*************************************************************************/
AK8963_AppCustomData_t AK8963_AppCustomData;

/************************************************************************
** Local Variables
*************************************************************************/

/************************************************************************
** Local Function Definitions
*************************************************************************/
void AK8963_Custom_InitData(void)
{
    /* Set all struct zero values */
    bzero(&AK8963_AppCustomData, sizeof(AK8963_AppCustomData));
    AK8963_AppCustomData.SelfTestMode = FALSE;
    return;
}


boolean AK8963_Custom_Init()
{
    boolean returnBool = TRUE;
    AK8963_AppCustomData.Status = AK8963_CUSTOM_INITIALIZED;

end_of_function:
    return (returnBool);
}

boolean AK8963_PowerOn(void)
{
    return (TRUE);
}

boolean AK8963_Read_Mag(int16 *X, int16 *Y, int16 *Z)
{
    boolean returnBool = TRUE;

    float calX_f = 0.0f;
    float calY_f = 0.0f;
    float calZ_f = 0.0f;

    /* Temp for inverse roation */
    float temp   = 0.0f;

    /* Null pointer check */
    if(0 == X || 0 == Y || 0 == Z)
    {
        (void) CFE_EVS_SendEvent(AK8963_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "AK8963 Read_Gyro Null Pointer");
    }

    SIMLIB_GetMag(&calX_f, &calY_f, &calZ_f);

    if(FALSE == AK8963_AppCustomData.SelfTestMode)
    {
        /* Apply inverse rotation */
        temp   = calY_f;
        calY_f = calX_f * -1;
        calX_f = temp;
        
        *X = (((calX_f / oAK8963.Diag.Calibration.MagXScale) + oAK8963.Diag.Calibration.MagXOffset)) / (AK8963_MAG_UNIT / AK8963_MAG_DIVIDER);
        *Y = ((calY_f / oAK8963.Diag.Calibration.MagYScale) + oAK8963.Diag.Calibration.MagYOffset) / (AK8963_MAG_UNIT / AK8963_MAG_DIVIDER);
        *Z = ((calZ_f / oAK8963.Diag.Calibration.MagZScale) + oAK8963.Diag.Calibration.MagZOffset) / (AK8963_MAG_UNIT / AK8963_MAG_DIVIDER);
    }
    else if (TRUE == AK8963_AppCustomData.SelfTestMode)
    {
        /* Expected value from self-calibrate with inverse scaling */
        *X = 1.16f / (1.0f / 660.0f);
        *Y = 1.08f / (1.0f / 660.0f);
        *Z = 1.08f / (1.0f / 660.0f);
    }

    return (returnBool);
}

boolean AK8963_Read_MagDeviceID(uint8 *Value)
{
    *Value = AK8963_WAI_ID;
    return (TRUE);
}

boolean AK8963_Read_MagAdj(uint8 *X, uint8 *Y, uint8 *Z)
{
    *X = 64;
    *Y = 64;
    *Z = 64;

    return (TRUE);
}

boolean AK8963_Custom_ValidateID(void)
{
    return (TRUE);
}


boolean AK8963_Custom_Send(uint8 Reg, uint8 Data)
{
    return (TRUE);
}


boolean AK8963_Custom_Uninit(void)
{
    boolean returnBool = TRUE;
    AK8963_AppCustomData.Status = AK8963_CUSTOM_UNINITIALIZED;

    return (returnBool);
}


int32 AK8963_Custom_Init_EventFilters(int32 ind, CFE_EVS_BinFilter_t *EventTbl)
{
    return (0);
}


boolean AK8963_Apply_Platform_Rotation(float *X, float *Y, float *Z)
{
    boolean returnBool = TRUE;
    float temp         = 0;

    /* Null pointer check */
    if(0 == X || 0 == Y || 0 == Z)
    {
        (void) CFE_EVS_SendEvent(AK8963_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "AK8963 Apply_Platform_Rotation Null Pointer");
        returnBool = FALSE;
        goto end_of_function;
    }
    
    /* The standard external mag by 3DR has x pointing to the
     * right, y pointing backwards, and z down, therefore switch x
     * and y and invert y. */
    temp = *X;
    *X = -*Y;
    *Y = temp;

end_of_function:

    return (returnBool);
}


void AK8963_Get_Rotation(uint8 *Rotation)
{
    
    /* Null pointer check */
    if(0 == Rotation)
    {
        goto end_of_function;
    }

    *Rotation = ROTATION_YAW_90;

end_of_function:
    return;
}


boolean AK8963_RunSelfTest(void)
{
	/* Simulate delays we would see with the real hardware. */
    usleep(AK8963_SELFTEST_DELAY_TIME);

    /* Return and report success. */
    return (true);
}
