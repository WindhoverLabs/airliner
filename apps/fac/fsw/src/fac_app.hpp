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

#ifndef FAC_APP_HPP
#define FAC_APP_HPP

#include "cfe.h"

#include "fac_platform_cfg.h"
#include "fac_mission_cfg.h"
#include "fac_private_ids.h"
#include "fac_private_types.h"
#include "fac_perfids.h"
#include "fac_msgids.h"
#include "fac_msg.h"
#include "fac_events.h"
#include "fac_config_utils.hpp"
#include "fac_cds_utils.hpp"


#define FAC_TIMEOUT_MSEC             	(1000)


typedef struct
{
   uint32  MainAppId;
   uint32  uiAppRunStatus;
   boolean bCdsTaskCreated;
   boolean bCmdsTaskCreated;
   boolean bAppAwaken;

   FAC_HkTlm_t       HkTlm;
} FAC_AppData_t;


/************************************************************************/
/** \brief CFS Fixedwing Attitude Control App Task (FAC) application
**           entry point
**
**  \par Description
**       CFS Fixedwing Attitude Control App Task application entry point.
**       This function performs app initialization, then waits for the
**       cFE ES Startup Sync, then executes the RPR main processing loop.
**
**  \par Assumptions, External Events, and Notes:
**       If there is an unrecoverable failure during initialization the
**       main loop is never executed and the application will exit.
**
*************************************************************************/
#ifdef __cplusplus
extern "C" {
void  FAC_AppMain(void);
}
#endif

#endif /* FAC_APP_HPP */
