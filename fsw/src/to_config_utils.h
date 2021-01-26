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

#ifndef TO_CONFIG_UTILS_H
#define TO_CONFIG_UTILS_H

/************************************************************************
** Pragmas
*************************************************************************/

/************************************************************************
** Includes
*************************************************************************/
#include "cfe.h"
#include "to_app.h"
#include "to_channel.h"

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
**  \param   channel    A #TO_ChannelData_t pointer that 
**                      references the channel data structure
**
**  \return
**  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS  \endcode
**  \retstmt Return codes from #CFE_TBL_Register          \endcode
**  \retstmt Return codes from #CFE_TBL_Load              \endcode
**  \retstmt Return codes from #TO_AcquireConfigPointer   \endcode
**
*************************************************************************/
int32  TO_InitTables(TO_ChannelData_t *channel);


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
**  \param   tableType     A #TO_Table_Types_t enum
**
**  \return
**  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS  \endcode
**  If unsuccessful, 
**  #TO_CONFIG_TABLE_RETURN_INVALID_VERSION, #TO_CONFIG_TABLE_RETURN_NO_SECONDARY_HEADER, 
**  #TO_CONFIG_TABLE_MSG_FLOW_MSG_LIMIT_ERR, #TO_CONFIG_TABLE_MSG_FLOW_PQ_ID_ERR,
**  #TO_CONFIG_TABLE_NULL_PTR_ERR, #TO_CONFIG_TABLE_PQUEUE_STATE_ERR,
**  #TO_CONFIG_TABLE_PQUEUE_QTYPE_ERR, #TO_CONFIG_TABLE_PQUEUE_MSG_LIMIT_ERR 
**  #TO_CONFIG_TABLE_NO_PQUEUES_ERR, #TO_CONFIG_TABLE_MSG_FLOW_FILTER_ERR, 
**  #TO_CONFIG_TABLE_QUEUE_ERR, #TO_CONFIG_TABLE_ANOMALY_BUFFER_ERR.
**
*************************************************************************/
int32  TO_ValidateConfigTbl(void *configTblPtr);


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
**  \param   channel    A #TO_ChannelData_t pointer that 
**                      references the channel data structure
**
**  \return
**  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS  \endcode
**
*************************************************************************/
int32 TO_AcquireConfigPointer(TO_ChannelData_t *channel);


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
**  \param   channel    A #TO_ChannelData_t pointer that 
**                      references the channel data structure
**  \return
**  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS  \endcode
**
*************************************************************************/
int32 TO_ProcessNewConfigTbl(TO_ChannelData_t *channel);


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
int32 TO_ValidateConfigGrndTbl(void *configTblPtr);



/************************************************************************/
/** \brief This manages all the tables in the app
**
**  \par Description
**       This function performs the table management for each channel's
**       dump and configuration tables with cFE table services.
**
**  \par Assumptions, External Events, and Notes:
**       All tables have been initialized via TO_InitCoinfigTbl().
**       This is called during app cyclic operations.
**
*************************************************************************/
void TO_ManageAllAppTables(void);


/************************************************************************/
/** \brief This manages all the tables for a given channel
**
**  \par Description
**       This function performs the table management for a channel's
**       dump and configuration tables with cFE table services.
**
**  \par Assumptions, External Events, and Notes:
**       All channel tables have been initialized via TO_InitCoinfigTbl().
**
**  \param initialManage T=this is the first time to call this function 
**             at app initialization, F=this is being called in the app main loop
**
**  \param ChannelID The index of the channel to manage
**
*************************************************************************/
void TO_ManageChannelTables(osalbool initialManage, uint16 ChannelID);


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
**  \param   channel    A #TO_ChannelData_t pointer that 
**                      references the channel data structure
**
*************************************************************************/
void TO_LoadBackupConfigTable(TO_ChannelData_t *channel);


#ifdef __cplusplus
}
#endif

#endif /* TO_CONFIG_UTILS_H */

/************************/
/*  End of File Comment */
/************************/
    
