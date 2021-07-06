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

#ifndef TO_CHANNEL_H
#define TO_CHANNEL_H

/************************************************************************
** Pragmas
*************************************************************************/

/************************************************************************
** Local Defines
*************************************************************************/

/**
 * \brief The Channel index out of range.
 */
#define TO_CHANNEL_OUT_OF_RANGE_ERR     (-1)

/**
 * \brief \brief The table data for channel is missing.
 */
#define TO_CHANNEL_PTR_NULL_ERR         (-2)

/**
 * \brief The channel open failure.
 */
#define TO_CHANNEL_OPEN_ERR             (-3)

/************************************************************************
** Includes
*************************************************************************/
#include "cfe.h"
#include "to_tbldefs.h"

/**
** \brief Channel States.
*
*  \par TO channels can have three states:
* 
*   TO_CHANNEL_UNKNOWN
*   TO_CHANNEL_OPENED
*   TO_CHANNEL_CLOSED  
*         
*  \note All channels that are opened will never be commanded to close.
*        The only time where a channel is closed is during clean-up.
*/
typedef enum
{

   /** \brief Channel is unknown.
     *
     * \par
     * This state means the channel was either never initialized or failed to initialized
     * and is in an unusable state (no data flow).
     */
    TO_CHANNEL_UNKNOWN = 0,

    /** \brief Channel closed state.
     *
     * \par
     * This state means that the channel is not operational (no data flow).     
     */
    TO_CHANNEL_CLOSED = 1,
    
   /** \brief Channel opened state.
     *
     * \par
     * This state means the channel is operational (data can flow).     
     */
    TO_CHANNEL_OPENED = 2,

    /** \brief This is a count of Channel states and should not be used. */
    TO_CHANNEL_STATE_COUNT

} TO_ChannelState_t OS_ALIGN(4);

/*Make sure uint8 can store enum*/
CompileTimeAssert((TO_CHANNEL_STATE_COUNT <= 0xff), TO_ChannelState_t_less_than_256);

/**
** \brief Definition of Channel Data.
*
*/
struct TO_ChannelData_t
{
    /* Config table-related */

    /** \brief Config Table Handle
     *
     *  \note CFE_TBL_Handle_t is a int16
     */
    CFE_TBL_Handle_t     ConfigTblHdl;

    /** \brief Dump Table Handle */
    CFE_TBL_Handle_t     DumpTblHdl;

    /** \brief Config Table Pointer
     *
     * \par
     * A pointer to config table data from cFE table services.
     *
     * \par
     * Note, this pointer can be null if the table data is not current or valid.
     * The reasons for this can occur outside of nominal app function and the
     * pointer could be restored to a non-null value at another time.
     */
    TO_ChannelTbl_t*     ConfigTblPtr;

    /** \brief Pointer to the memory resident backup table, if the channel
     * was configured to have one.  If there is no backup table, this is null.
     */
    TO_ChannelTbl_t*     BackupTblPtr;

    /** \brief Dump Table Pointer */
    TO_ChannelDumpTbl_t  DumpTbl;

    /** \brief Mutex to protect the Channel data*/
    uint32               MutexID;
    
    /** \totlm Sent Channel Telemetry
    \brief Count of telemetry messages sent out the channel interface.
    Counter begins at zero with app initialization and rolls over. */

    /** \totlm Channel Bytes Sent
    \brief The number of bytes sent out the channel's interface
    Counter begins at zero with app initialization and rolls over. */
    uint32               BytesSent;    

    /** \brief Struct for single output channel queue entry*/
    TO_OutputQueue_t     OutputQueue;
    
    /** \brief Channel index */
    uint32               channelIdx;

    /** \brief Config Table Name */
    char                 ConfigTableName[CFE_TBL_MAX_NAME_LENGTH]; 

    /** \brief Config Table File Name */
    char                 ConfigTableFileName[OS_MAX_PATH_LEN]; 

    /** \brief Dump Table Name */
    char                 DumpTableName[CFE_TBL_MAX_NAME_LENGTH]; 

    /** \brief Channel Name */
    char                 ChannelName[OS_MAX_API_NAME];

    /** \brief To hold the value of TO_ChannelState_t */
    uint8                State;

    uint32               SentMsgCount;

};

/************************************************************************
** Functions
*************************************************************************/

/************************************************************************/
/** \brief Configures and initializes a single channel.
**
**  \par Description
**       Initializes channel specific data and structures. Sets up channel pipe
**       as well as initializing the cf throttling semaphore for each channel.
**
**  \par Assumptions, External Events, and Notes:
**       Semaphore name must be unique among channels; initialization will fail if two
**       channels attempt to use the same semaphore.
**       The pointers for ChannelName, ConfigTableName, ConfigTableFileName,
**       DumpTableName, and CfCntSemName are not checked for NULL since those
**       are passed in as arguments that have been macro defined in to_platform_cfg.h
**       and are checked in a verify header to ensure the macros exist.
**
**       Only the index is range-checked and no other argument checking is performed. 
**
**  \param[in]   index
**                Index of the channel, cannot be greater than TO_MAX_CHANNELS
**
**  \param[in]   ChannelName
**                Name of channel, will be prefixed with TO_ .
**                NOTE: Since each channel has a child task thread with the same name,
**                and in RTEMS threads must have names of 4 chars, this param should be a single char.
**
**                The 4-char limit is an RTEMS limit.  The RTEMS OSAL only takes the first 4 chars of 
**                the name per RTEMS limits for the underlying RTEMS message queue but still 
**                preserves the full name given to it in the OSAL
**                The 1-char limit helps with keeping the name short enough to view it via 
**                RTEMS reports
**
**  \param[in]   ConfigTableName
**                Name of the config table for the channel as referred to by TBL services
**
**  \param[in]   ConfigTableFileName
**                File path of the channel config table
**
**  \param[in]   DumpTableName
**                Name of the dump table for the channel as referred to by TBL services
**
**  \param[in]   CfCntSemMax
**                The initial maximum value of the CF throttling semaphore for this channel.
**
**  \param[in]   CfCntSemName
**                Name of the CF throttling semaphore, must be unique among each channel.
**
**  \return
**  CFE_SUCCESS if successful.  #TO_CHANNEL_OUT_OF_RANGE_ERR or OSAL error if unsuccessful.
**
*************************************************************************/
int32 TO_Channel_OpenChannel(const uint32 index, const char *ChannelName,
        const char *ConfigTableName, const char *ConfigTableFileName,
        TO_ChannelTbl_t *BackupTblPtr, const char *DumpTableName,
        const uint32 CfCntSemMax, const char *CfCntSemName);


/************************************************************************/
/**
** \brief Forwards all channel's telemetry data to TO_Classifier
**        and TO_Scheduler
**
** \par Assumptions, External Events, and Notes:
**      Assumes channel pointer is not NULL
*************************************************************************/
void TO_Channel_ProcessTelemetryAll(void);


/************************************************************************/
/**
** \brief Resets all channel's message flow, priority queue,
**        and output queue counters.
**
** \par Assumptions, External Events, and Notes:
**      Assumes channel pointer is not NULL
*************************************************************************/
void TO_Channel_ResetCountsAll(void);


/************************************************************************/
/**
** \brief Set's a channel reference based on a valid input index and
**        calls TO_Channel_LockByRef.
**
** \par Assumptions, External Events, and Notes:
**      If the input index is out of range, an event is generated
**      #TO_CHANNEL_OUT_OF_RANGE_ERR_EID
**
** \param[in]   index
**              Index of the channel
*************************************************************************/
void TO_Channel_LockByIndex(uint16 index);


/************************************************************************/
/**
** \brief Set's a channel reference based on a valid input index and
**        calls TO_Channel_UnlockByRef.
**
** \par Assumptions, External Events, and Notes:
**      If the input index is out of range, an event is generated
**      #TO_CHANNEL_OUT_OF_RANGE_ERR_EID
**
** \param[in]   index
**              Index of the channel
*************************************************************************/
void TO_Channel_UnlockByIndex(uint16 index);


/************************************************************************/
/**
** \brief The mutex object referenced by the channel pointer shall be locked 
**        by calling this function.
**
** \par Assumptions, External Events, and Notes:
**      Assumes channel pointer is not NULL
*************************************************************************/
void TO_Channel_LockByRef(TO_ChannelData_t *channel);


/************************************************************************/
/**
** \brief The mutex object referenced by the channel pointer shall be unlocked 
**        by calling this function.
**
** \par Assumptions, External Events, and Notes:
**      Assumes channel pointer is not NULL
*************************************************************************/
void TO_Channel_UnlockByRef(TO_ChannelData_t *channel);


/************************************************************************/
/**
** \brief Loops through channel less than TO_MAX_CHANNELS and initializes a 
**        reference to each channel's channel data, sets the channel's index,
**        and creates each channel's mutex.
**
** \par Assumptions, External Events, and Notes:
**      None.
*************************************************************************/
void TO_Channel_InitAll(void);


/************************************************************************/
/**
** \brief Loops through channel less than TO_MAX_CHANNELS and calls functions 
**        to teardown each channel's priority queue, 
**        output queue, and message flow(s).  It also deletes each channel's mutex.
**
** \par Assumptions, External Events, and Notes:
**      None.
*************************************************************************/
void TO_Channel_CleanupAll(void);


/************************************************************************/
/**
** \brief Returns a input channel's state.
**
** \par Assumptions, External Events, and Notes:
**      If the input index is out of range, the channel state is TO_CHANNEL_UNKNOWN.
**
**  \param[in]   index
**               Index of the channel
**
**  \return
**  TO_CHANNEL_CLOSED, TO_CHANNEL_OPENED, or TO_CHANNEL_UNKNOWN
**  
*************************************************************************/
uint8 TO_Channel_State(uint16 index);


/************************************************************************/
/** \brief Flush priority and output queues for the channel.
**
**  \par Description
**       This function is called by command from the operators, and will wipe
**       all messages from output queue and priority queue buffers without resetting
**       the current table.
**
**  \param[in]   index
**               Index of the channel
**
**  \return
**  \retcode TRUE  \retdesc The command succeeded \endcode
**  \retcode FALSE \retdesc The command failed, if index is greater than or equal
**                          to TO_MAX_CHANNELS or if #TO_PriorityQueue_TeardownAll
**                          or #TO_OutputQueue_Teardown do not return CFE_SUCCESS\endcode
**  
*************************************************************************/
osalbool TO_Channel_Flush(uint16 index);


#endif
