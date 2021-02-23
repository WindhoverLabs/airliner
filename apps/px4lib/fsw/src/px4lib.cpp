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

/*************************************************************************
** Includes
*************************************************************************/
#include "cfe.h"
#include "px4lib_version.h"
#include "px4lib.h"
#include <time.h>
#include <errno.h>
#include "cfs_utils.h"

extern "C" {

/*************************************************************************
** Macro Definitions
*************************************************************************/


/*************************************************************************
** Private Function Prototypes
*************************************************************************/
int32 PX4LIB_LibInit(void);
int32 PX4LIB_MixerCallback(cpuaddr Handle,
                        uint8 ControlGroup,
                        uint8 ControlIndex,
                        float &Control);


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* CFS Library Initialization Routine                              */
/* cFE requires that a library have an initialization routine      */ 
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 PX4LIB_LibInit(void)
{
    OS_printf ("PX4 Library Initialized.  Version %d.%d.%d.%d\n",
    		PX4LIB_MAJOR_VERSION,
			PX4LIB_MINOR_VERSION,
			PX4LIB_REVISION,
			PX4LIB_MISSION_REV);
                
    return CFE_SUCCESS;
 
}/* End PX4LIB_LibInit */


//int32 PX4LIB_MixerCallback(cpuaddr Handle,
//                        uint8 ControlGroup,
//                        uint8 ControlIndex,
//                        float &Control)
//{
//    Control = 0.0;
//    return 0;
//}



//uint64 PX4LIB_GetPX4TimeUs(void)
//{
//    struct timespec ts;
//    int returnCode = 0;
//    uint64 outTime = 0;
//
//    returnCode = clock_gettime(CLOCK_MONOTONIC, &ts);
//    if (-1 == returnCode)
//    {
//        OS_printf("PX4LIB_GetPX4Time clock_gettime errno: %i", errno);
//        goto end_of_function;
//    }
//
//    outTime = (uint64)(ts.tv_sec) * 1000000;
//    outTime += ts.tv_nsec / 1000;
//
//end_of_function:
//    return outTime;
//}
//
//
//
//uint64 PX4LIB_GetPX4TimeMs(void)
//{
//    struct timespec ts;
//    int returnCode = 0;
//    uint64 outTime = 0;
//
//    returnCode = clock_gettime(CLOCK_MONOTONIC, &ts);
//    if (-1 == returnCode)
//    {
//        OS_printf("PX4LIB_GetPX4Time clock_gettime errno: %i", errno);
//        goto end_of_function;
//    }
//
//    outTime = ts.tv_sec * 10000;
//    outTime += ts.tv_nsec / 1000000;
//
//end_of_function:
//    return outTime;
//}


uint64 PX4LIB_GetPX4TimeUs(void)
{
    return CFE_TIME_GetTimeInMicros();
}



uint64 PX4LIB_GetPX4TimeMs(void)
{
    uint64 result = CFE_TIME_GetTimeInMicros() / 1000;

    return result;
}


}

/************************/
/*  End of File Comment */
/************************/
