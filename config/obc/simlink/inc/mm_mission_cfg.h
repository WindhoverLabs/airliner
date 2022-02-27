#ifndef MM_MISSION_CFG_H
#define MM_MISSION_CFG_H

/************************************************************************
** Includes
*************************************************************************/
#include "cfe_mission_cfg.h"

/** \mmcfg CRC type for interrupts disabled loads
**  
**  \par Description:
**       CFE CRC type to use when processing the "memory load with 
**       interrupts disabled" (#MM_LOAD_MEM_WID_CC) command.
**
**  \par Limits:
**       This must be one of the CRC types supported by the 
**       #CFE_ES_CalculateCRC function.
*/
#define MM_LOAD_WID_CRC_TYPE     CFE_ES_DEFAULT_CRC

/** \mmcfg CRC type for load files
**  
**  \par Description:
**       CFE CRC type to use when processing memory loads
**       from a file.
**
**  \par Limits:
**       This must be one of the CRC types supported by the 
**       #CFE_ES_CalculateCRC function.
*/
#define MM_LOAD_FILE_CRC_TYPE    CFE_ES_DEFAULT_CRC

/** \mmcfg CRC type for dump files
**  
**  \par Description:
**       CFE CRC type to use when processing memory dumps
**       to a file.
**
**  \par Limits:
**       This must be one of the CRC types supported by the 
**       #CFE_ES_CalculateCRC function.
*/
#define MM_DUMP_FILE_CRC_TYPE    CFE_ES_DEFAULT_CRC

#endif /*MM_MISSION_CFG_H*/

/************************/
/*  End of File Comment */
/************************/
