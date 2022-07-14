    
#ifndef FPC_MISSION_CFG_H
#define FPC_MISSION_CFG_H

#ifdef __cplusplus
extern "C" {
#endif
#include <math.h>

/*
** FPC Mission Configuration Parameter Definitions
*/

/* TODO:  Add mission configuration parameter definitions here, if necessary. */

#define FPC_SB_TIMEOUT         CFE_SB_PEND_FOREVER  /* Can be a value in milliseconds */

#define DEG_TO_RADIANS(d) (d * M_PI / 180.0)
#ifdef __cplusplus
}
#endif

#endif /* FPC_MISSION_CFG_H */

/************************/
/*  End of File Comment */
/************************/
