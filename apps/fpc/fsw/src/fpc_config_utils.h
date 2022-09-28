/****************************************************************************
 *
 *   Copyright (c) 2022 Windhover Labs, L.L.C. All rights reserved.
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

#ifndef FPC_CONFIG_UTILS_H
#define FPC_CONFIG_UTILS_H

/************************************************************************
 ** Pragmas
 *************************************************************************/

/************************************************************************
 ** Includes
 *************************************************************************/
#include "cfe.h"
#include "fpc_app.h"

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************
 ** Local Defines
 *************************************************************************/

/************************************************************************
 ** Local Structure Declarations
 *************************************************************************/

/************************************************************************
 ** External Global Variables
 *************************************************************************/

/************************************************************************
 ** Global Variables
 *************************************************************************/

/************************************************************************
 ** Local Variables
 *************************************************************************/

/************************************************************************
 ** Local Function Prototypes
 *************************************************************************/

/************************************************************************/
/** \brief Init FPC Config table
 **
 **  \par Description
 **       This function initializes FPC's Config table
 **
 **  \par Assumptions, External Events, and Notes:
 **       None
 **
 **  \returns
 **  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS  \endcode
 **  \retstmt Return codes from #CFE_TBL_Register          \endcode
 **  \retstmt Return codes from #CFE_TBL_Load              \endcode
 **  \retstmt Return codes from #FPC_AcquireConfigPointers \endcode
 **  \endreturns
 **
 *************************************************************************/
int32 FPC_InitConfigTbl(void);

/************************************************************************/
/** \brief Validate FPC Config table
 **
 **  \par Description
 **       This function validates FPC's Config table
 **
 **  \par Assumptions, External Events, and Notes:
 **       None
 **
 **  \returns
 **  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS  \endcode
 **  \endreturns
 **
 *************************************************************************/
int32 FPC_ValidateConfigTbl(void*);

/************************************************************************/
/** \brief Obtain FPC Config Table Data Pointer
 **
 **  \par Description
 **       This function manages the config table
 **       and obtains a pointer to its data.
 **
 **  \par Assumptions, External Events, and Notes:
 **       None
 **
 **  \returns
 **  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS  \endcode
 **  \endreturns
 **
 *************************************************************************/
int32 FPC_AcquireConfigPointers(void);

/************************************************************************/
/** \brief Process new FPC Config table
 **
 **  \par Description
 **       This function processes FPC's new Config table
 **       and set Config parameters with new values.
 **
 **  \par Assumptions, External Events, and Notes:
 **       None
 **
 **  \returns
 **  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS  \endcode
 **  \endreturns
 **
 *************************************************************************/
void FPC_ProcessNewConfigTbl(void);

#ifdef __cplusplus
}
#endif

#endif /* FPC_CONFIG_UTILS_H */

/************************/
/*  End of File Comment */
/************************/

