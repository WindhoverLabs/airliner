#ifndef LC_MISSION_CFG_H
#define LC_MISSION_CFG_H

#include "msg_ids.h"

/** \lccfg RTS Request Message ID
**  
**  \par Description:
**       Message ID that will be used by the /LC_ExecuteRTS function
**       to construct the RTS request message.
**        
**       We define this here to allow the LC application to be built 
**       without including headers from the application (e.g. SC) that
**       will receive the command.  Obviously, this definition must
**       match the message ID expectyed by the receiving application.
**         
**       A mission may choose to modify the /LC_ExecuteRTS function not 
**       to use this or define it using a message ID constant that is 
**       defined numerically elsewhere.
**
**  \par Limits:
**       This parameter shouldn't be larger than the value set for 
**       #CFE_SB_HIGHEST_VALID_MSGID.
*/
#define LC_RTS_REQ_MID                    SC_CMD_MID

/** \lccfg RTS Request Command Code
**  
**  \par Description:
**       Command code that will be used by the /LC_ExecuteRTS function
**       to construct the RTS request message.
**        
**       We define this here to allow the LC application to be built 
**       without including headers from the application (e.g. SC) that
**       will receive the command.  Obviously, this definition must
**       match the command code expected by the receiving application.
**
**  \par Limits:
**       This parameter can't be larger than an unsigned 16 bit
**       integer (65535).  Of course we all know that the preceding
**       limit describes the size of the variable returned by the
**       API function #CFE_SB_GetCmdCode.  And that for as long as CFE
**       uses the existing CCSDS packet format implementation, command
**       codes are only 7 bits (unsigned) with an upper limit of 127
**       and a lower limit of zero.
*/
#define LC_RTS_REQ_CC                     4

#endif /*LC_MISSION_CFG_H*/

/************************/
/*  End of File Comment */
/************************/
