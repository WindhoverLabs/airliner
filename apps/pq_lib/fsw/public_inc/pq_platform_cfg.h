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

#ifndef PQ_PLATFORM_CFG_H
#define PQ_PLATFORM_CFG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "cfe_platform_cfg.h"

/*
** Platform Configuration Parameter Definitions
*/

/** \pqcfg Maximum message length
**
**  \par Description:
**       The maximum size that an output telemetry message can be.  This is in
**       bytes.  If a message exceeds this size, it will be dropped by the
**       classifier.
**
*/
#define PQ_MAX_MSG_LENGTH             (32767)

/**
**  \cfeescfg Define PQ Memory Pool Block Sizes
**
**  \par Description:
**       PQ Memory Pool Block Sizes
**
**  \par Limits
**       These sizes MUST be increasing and MUST be an integral multiple of 4.
**       The number of block sizes defined cannot exceed
**       #CFE_ES_MAX_MEMPOOL_BLOCK_SIZES
*/
#define PQ_MEM_BLOCK_SIZE_01           (32)
#define PQ_MEM_BLOCK_SIZE_02           (64)
#define PQ_MEM_BLOCK_SIZE_03           (128)
#define PQ_MEM_BLOCK_SIZE_04           (256)
#define PQ_MEM_BLOCK_SIZE_05           (512)
#define PQ_MEM_BLOCK_SIZE_06           (2048)
#define PQ_MEM_BLOCK_SIZE_07           (8192)
#define PQ_MAX_BLOCK_SIZE              (PQ_MEM_BLOCK_SIZE_07 + PQ_MEM_BLOCK_SIZE_07)
#define PQ_MAX_MEMPOOL_BLK_SIZES       (8)

/** \pqcfg Channel output queue depth.
 *
**  \par Description:  This defines how many messages can be queued into the
**       channel output queue by the scheduler.
*/
#define PQ_OUTPUT_QUEUE_DEPTH          (20)

/**
**  \pqcfg Number of bytes in the Message Memory Pool
**
**  \par Description:
**       The message memory pool contains the memory needed for the queued messages packets.
**       The queued messages are dynamically allocated from this pool when the
**       messages are received and queued, and deallocated when messages are transmitted
**       or a new table is loaded.
**
**  \par Limits
**       The PQ app does not place a limit on this parameter, but there is
**       an overhead cost in the memory pool.  The value must be larger than what is
**       needed.
*/
#define PQ_NUM_BYTES_IN_MEM_POOL       (200 * PQ_MAX_BLOCK_SIZE)

#define PQ_EVENT_ID_OFFSET             (100)

#ifdef __cplusplus
}
#endif

#endif /* PQ_PLATFORM_CFG_H */

/************************/
/*  End of File Comment */
/************************/
    
