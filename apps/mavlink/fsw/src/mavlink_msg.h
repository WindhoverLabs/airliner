    
#ifndef MAVLINK_MSG_H
#define MAVLINK_MSG_H

/************************************************************************
** Pragmas
*************************************************************************/

/************************************************************************
** Includes
*************************************************************************/
#include "cfe.h"

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************
** Local Defines
*************************************************************************/

/************************************************************************
** MAVLINK Command Codes
*************************************************************************/

/** \mavlinkcmd Noop 
**  
**  \par Description
**       Implements the Noop command that demonstrates the MAVLINK task is alive
**
**  \mavlinkcmdmnemonic \MAVLINK_NOOP
**
**  \par Command Structure
**       #MAVLINK_NoArgCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \MAVLINK_CMDACPTCNT - command counter will increment
**       - The #MAVLINK_CMD_INF_EID informational event message will be 
**         generated when the command is received
** 
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
** 
**  \par Evidence of failure may be found in the following telemetry: 
**       - \b \c \MAVLINK_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #MAVLINK_MSGID_ERR_EID
**
**  \par Criticality
**       None
**
**  \sa #MAVLINK_RESET_CC
*/
#define MAVLINK_NOOP_CC                 (0)

/** \mavlinkcmd Reset Counters
**  
**  \par Description
**       Resets the mavlink housekeeping counters
**
**  \mavlinkcmdmnemonic \MAVLINK_TLMRST
**
**  \par Command Structure
**       #MAVLINK_NoArgCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \MAVLINK_CMDACTPCNT       - command counter will be cleared
**       - \b \c \MAVLINK_CMDRJCTCNT       - command error counter will be cleared
**       - The #MAVLINK_CMD_INF_EID debug event message will be 
**         generated when the command is executed
** 
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
** 
**  \par Evidence of failure may be found in the following telemetry: 
**       - \b \c \MAVLINK_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #MAVLINK_MSGID_ERR_EID
**
**  \par Criticality
**       None
**
**  \sa #MAVLINK_NOOP_CC
*/
#define MAVLINK_RESET_CC                (1)


/************************************************************************
** Local Structure Declarations
*************************************************************************/

/** 
**  \brief No Arguments Command
**  For command details see #MAVLINK_NOOP_CC, #MAVLINK_RESET_CC
**  Also see #MAVLINK_SEND_HK_MID
*/
typedef struct
{
    uint8  ucCmdHeader[CFE_SB_CMD_HDR_SIZE];
} MAVLINK_NoArgCmd_t;

/** 
**  \brief TODO Elaborate this struct
**  Boilerplate example of application-specific incoming data
*/
typedef struct
{
    uint8   TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint32  counter;

    /* TODO:  Add input data to this application here, such as raw data read from I/O
    **        devices.
    **        Option: for data that is already defined by another app, include
    **        that app's message header above.
    */

} MAVLINK_InData_t;

/** 
**  \brief TODO Elaborate this struct
**  Boilerplate example of application-specific outgoing data
*/
typedef struct
{
    uint8   ucTlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint32  uiCounter;
} MAVLINK_OutData_t;

/** 
**  \brief MAVLINK application housekeeping data
*/
typedef struct
{
    /** \brief cFE SB Tlm Msg Hdr */
    uint8              TlmHeader[CFE_SB_TLM_HDR_SIZE];

    /** \mavlinktlmmnemonic \MAVLINK_CMDACPTCNT
        \brief Count of accepted commands */
    uint8              usCmdCnt;   

    /** \mavlinktlmmnemonic \MAVLINK_CMDRJCTCNT
        \brief Count of failed commands */
    uint8              usCmdErrCnt; 

    /** \mavlinktlmmnemonic \MAVLINK_CMDACPTCNT
        \brief Count of accepted commands */
    uint8              paramCmdCnt;

    /** \mavlinktlmmnemonic \MAVLINK_CMDRJCTCNT
        \brief Count of failed commands */
    uint8              paramCmdErrCnt;

    boolean			   HeartbeatActive;

    uint8 SystemStatus;
    uint8 BaseMode;
    uint32 CustomMode;

} MAVLINK_HkTlm_t;

#ifdef __cplusplus
}
#endif

#endif /* MAVLINK_MSG_H */

/************************/
/*  End of File Comment */
/************************/
