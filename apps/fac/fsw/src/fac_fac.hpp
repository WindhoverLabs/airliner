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

#ifndef FAC_FPC_HPP
#define FAC_FPC_HPP

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


class FixedwingAttitudeControl
{
public:
   FixedwingAttitudeControl();
   ~FixedwingAttitudeControl();

   int32 InitApp();
   int32 RcvSchMsg(int32 iBlocking);

private:
   CFE_SB_PipeId_t     SchPipeId;

   CFE_EVS_BinFilter_t EventTbl[FAC_EVT_CNT];

   int32 InitEvent();
   int32 InitPipe();
   int32 InitHk();

   void    ReportHousekeeping();
   void    SendOutData();
};


#endif /* FAC_FPC_HPP */
