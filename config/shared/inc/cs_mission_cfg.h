#ifndef CS_MISSION_CFG
#define CS_MISSION_CFG

#include "cfe_mission_cfg.h"

/*************************************************************************
 ** Macro Definitions
 *************************************************************************/

/** \cscfg default CRC algorithm
 **  
 **  \par  Description:
 **        This parameter is the algorithm used by CS to checksum
 **        the requested data.
 **
 **  \par Limits:
 **         This parameter is limited to either #CFE_ES_DEFAULT_CRC,
 **         or  #CFE_ES_CRC_16
 */
#define CS_DEFAULT_ALGORITHM                CFE_ES_DEFAULT_CRC

#endif /*CS_MISSION_CFG*/

/************************/
/*  End of File Comment */
/************************/

