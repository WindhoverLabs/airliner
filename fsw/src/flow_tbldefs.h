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

#ifndef FLOW_TBLDEFS_H
#define FLOW_TBLDEFS_H

/************************************************************************
** Pragmas
*************************************************************************/

/************************************************************************
** Includes
*************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

#include "cfe.h"
#include "flow_platform_cfg.h"

/************************************************************************
** Local Defines
*************************************************************************/

/**
 * \brief Defines the table identification name used for the
 * configuration table registration.
 */
#define FLOW_CONFIG_TABLENAME ("CONFIG_TBL")


/** \brief Definition for a single config table entry */
typedef struct
{
	/** \flowcfg HFOV
	 *
	 *  \brief Horizontal field of view.(radians)
	 *
	 *  \par Limits:
	 *       Min > Max (incr.) 0 > 2PI, default 0.088.
	 */
	float HFOV;
	/** \flowcfg FLOW_TILE_SIZE
	 *
	 *  \brief Size of sliding window.
	 *
	 *  \par Limits:
	 *       Min > Max (incr.) 2 > 10, default 8.
	 */
	int32 FLOW_TILE_SIZE;
	/** \flowcfg FLOW_NUM_BLOCKS
	 *
	 *  \brief Number of blocks of sliding window.
	 *
	 *  \par Limits:
	 *       Min > Max (incr.) 2 > 8, default 5.
	 */
	int32 FLOW_NUM_BLOCKS;
	/** \flowcfg FLOW_DEFAULT_OUTPUT_RATE
	 *
	 *  \brief Default image output rate.
	 *
	 *  \par Limits:
	 *      Min > Max (incr.) 2 > 10, default 10.
	 */
	int32 FLOW_DEFAULT_OUTPUT_RATE;
	/** \flowcfg FLOW_DEFAULT_IMAGE_WIDTH
	 *
	 *  \brief Default image width.
	 *
	 *  \par Limits:
	 *       default 64.
	 */
	int32 FLOW_DEFAULT_IMAGE_WIDTH;
	/** \flowcfg FLOW_DEFAULT_IMAGE_HEIGHT
	 *
	 *  \brief Default image height.
	 *
	 *  \par Limits:
	 *       default 64.
	 */
	int32 FLOW_DEFAULT_IMAGE_HEIGHT;
	/** \flowcfg FLOW_DEFAULT_SEARCH_SIZE
	 *
	 *  \brief Default image search size.
	 *
	 *  \par Limits:
	 *       Min > Max (incr.) 2 > 10, default 6.
	 */
	int32 FLOW_DEFAULT_SEARCH_SIZE;
	/** \flowcfg FLOW_DEFAULT_FEATURE_THRESHOLD
	 *
	 *  \brief Default feature threshold.
	 *
	 *  \par Limits:
	 *       Min > Max (incr.) 10 > 50, default 30.
	 */
	int32 FLOW_DEFAULT_FEATURE_THRESHOLD;
	/** \flowcfg FLOW_DEFAULT_VALUE_THRESHOLD
	 *
	 *  \brief Default value threshold.
	 *
	 *  \par Limits:
	 *       Min > Max (incr.) 10 > 5000, default 3000.
	 */
	int32 FLOW_DEFAULT_VALUE_THRESHOLD;
	/** \flowcfg FLOW_MEAN_COUNT_THRESHOLD
	 *
	 *  \brief Mean count threshold.
	 *
	 *  \par Limits:
	 *       Min > Max (incr.) 2 > 20, default 10.
	 */
	int32 FLOW_MEAN_COUNT_THRESHOLD;

} FLOW_ConfigTbl_t;


#ifdef __cplusplus
}
#endif

#endif /* FLOW_TBLDEFS_H */

/************************/
/*  End of File Comment */
/************************/
