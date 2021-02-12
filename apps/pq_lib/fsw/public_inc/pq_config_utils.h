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

#ifndef PQ_CONFIG_UTILS_H
#define PQ_CONFIG_UTILS_H

/************************************************************************
** Pragmas
*************************************************************************/

/************************************************************************
** Includes
*************************************************************************/
#include "cfe.h"
#include "pq_structs.h"
#include "pq_channel.h"

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
/** \brief Init TO configuration and dump tables for a channel
**
**  \par Description
**       This function initializes the two tables for a channel
**
**  \par Assumptions, External Events, and Notes:
**       This function should be called only once per channel.
**
**
**  \param   Channel    A #PQ_ChannelData_t pointer that 
**                      references the channel data structure
**
**  \return
**  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS  \endcode
**  \retstmt Return codes from #CFE_TBL_Register          \endcode
**  \retstmt Return codes from #CFE_TBL_Load              \endcode
**  \retstmt Return codes from #PQ_AcquireConfigPointer   \endcode
**
*************************************************************************/
int32  PQ_InitTables(PQ_ChannelData_t *Channel);


/************************************************************************/
/** \brief Validate TO Configuration table
**
**  \par Description
**       This function validates TO's Configuration table
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param   configTblPtr  Pointer to config table data from cFE table services
**
**  \param   tableType     A #PQ_Table_Types_t enum
**
**  \return
**  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS  \endcode
**  If unsuccessful, 
**  #PQ_CONFIG_TABLE_RETURN_INVALID_VERSION, #PQ_CONFIG_TABLE_RETURN_NO_SECONDARY_HEADER, 
**  #PQ_CONFIG_TABLE_MSG_FLOW_MSG_LIMIT_ERR, #PQ_CONFIG_TABLE_MSG_FLOW_PQ_ID_ERR,
**  #PQ_CONFIG_TABLE_NULL_PTR_ERR, #PQ_CONFIG_TABLE_PQUEUE_STATE_ERR,
**  #PQ_CONFIG_TABLE_PQUEUE_QTYPE_ERR, #PQ_CONFIG_TABLE_PQUEUE_MSG_LIMIT_ERR 
**  #PQ_CONFIG_TABLE_NO_PQUEUES_ERR, #PQ_CONFIG_TABLE_MSG_FLOW_FILTER_ERR, 
**  #PQ_CONFIG_TABLE_QUEUE_ERR, #PQ_CONFIG_TABLE_ANOMALY_BUFFER_ERR.
**
*************************************************************************/
int32  PQ_ValidateConfigTbl(void *configTblPtr);


/************************************************************************/
/** \brief Obtain TO Config Table Data Pointer
**
**  \par Description
**       This function manages the config table
**       and obtains a pointer to its data.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param   Channel    A #PQ_ChannelData_t pointer that 
**                      references the channel data structure
**
**  \return
**  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS  \endcode
**
*************************************************************************/
int32 PQ_AcquireConfigPointer(PQ_ChannelData_t *Channel);


/************************************************************************/
/** \brief Process new TO Config table
**
**  \par Description
**       This function processes TO's new Config table
**       and set Config parameters with new values.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param   Channel    A #PQ_ChannelData_t pointer that 
**                      references the channel data structure
**  \return
**  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS  \endcode
**
*************************************************************************/
int32 PQ_ProcessNewConfigTbl(PQ_ChannelData_t *Channel);


/************************************************************************/
/** \brief Intermediate function to add a TO table type to allow
**         table validation per channel type (Ground, Onboard, Storage)
**         This function is used to help validate the Ground channel 
**         table.
**
**  \par Description
**       This function add channel type to be pass to validation function.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param   configTblPtr  Pointer to config table data from cFE table services
**
**  \return
**  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS  \endcode
**
*************************************************************************/
int32 PQ_ValidateConfigGrndTbl(void *configTblPtr);


/************************************************************************/
/** \brief This manages all the tables for a given channel
**
**  \par Description
**       This function performs the table management for a channel's
**       dump and configuration tables with cFE table services.
**
**  \par Assumptions, External Events, and Notes:
**       All channel tables have been initialized via PQ_InitCoinfigTbl().
**
**  \param initialManage T=this is the first time to call this function 
**             at app initialization, F=this is being called in the app main loop
**
**  \param   Channel    A #PQ_ChannelData_t pointer that 
**                      references the channel data structure
**
*************************************************************************/
void PQ_ManageChannelTables(osalbool initialManage, PQ_ChannelData_t *Channel);


/************************************************************************/
/** \brief Load the backup config table
**
**  \par Description
**       This function loads the built-in emergency backup TO config table.
**       For use when the config table can't be provided by cFE TBL, such as
**       the config file can't be read from the filesystem.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param   Channel    A #PQ_ChannelData_t pointer that 
**                      references the channel data structure
**
*************************************************************************/
void PQ_LoadBackupConfigTable(PQ_ChannelData_t *Channel);


#ifdef __cplusplus
}
#endif

#endif /* PQ_CONFIG_UTILS_H */

/************************/
/*  End of File Comment */
/************************/
    
