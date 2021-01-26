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

#ifndef EA_CUSTOM_H
#define EA_CUSTOM_H

/*************************************************************************
** Includes
*************************************************************************/
#include "cfe.h"
#include <unistd.h>

/*************************************************************************
** Custom Global Data Structure
*************************************************************************/

/**
**  \brief Data required to calculate external application utilization
*/
typedef struct
{
	/** \brief Total system cpu time (in jiffies) */
    unsigned long long int			total_time;

    /** \brief Total  cpu time (in jiffies) */
    unsigned long long int			p_time;

} EA_ProcData_t;

/************************************************************************/
/** \brief Initialize Child task and start external application
**
**  \par Description
**       This function spawns a CFS child task that will then call the
**       external application. The child task will perform a fork, with
**       the child of the fork executing the specified app and the
**       parent waiting for the child to finish execution. If the fork's
**       child task completes the parent will capture its return code
**       and update all relevant HK telemetry.
**
**
**  \par Assumptions, External Events, and Notes:
**       None
**
*************************************************************************/
void EA_StartAppCustom();

void EA_TermAppCustom(CFE_SB_Msg_t* MsgPtr);
void EA_PerfmonCustom(int32 pid);
EA_ProcData_t EA_ParsePidUtil(int32 pid);

#endif
