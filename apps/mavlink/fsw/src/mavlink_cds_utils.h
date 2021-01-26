
#ifndef MAVLINK_CDS_UTILS_H
#define MAVLINK_CDS_UTILS_H

/************************************************************************
** Pragmas
*************************************************************************/

/************************************************************************
** Includes
*************************************************************************/
#include "mavlink_tbldefs.h"

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
/** \brief Init MAVLINK CDS tables
**
**  \par Description
**       This function initializes MAVLINK's CDS tables
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \returns
**  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS \endcode
**  \retstmt Return codes from #CFE_ES_RegisterCDS       \endcode
**  \retstmt Return codes from #CFE_ES_CopyToCDS         \endcode
**  \endreturns
**
*************************************************************************/
int32  MAVLINK_InitCdsTbl(void);

/************************************************************************/
/** \brief Update MAVLINK CDS tables
**
**  \par Description
**       This function updates MAVLINK's CDS tables
**
**  \par Assumptions, External Events, and Notes:
**       None
**
*************************************************************************/
void   MAVLINK_UpdateCdsTbl(void);

/************************************************************************/
/** \brief Save MAVLINK CDS tables
**
**  \par Description
**       This function saves MAVLINK's CDS tables
**
**  \par Assumptions, External Events, and Notes:
**       None
**
*************************************************************************/
void   MAVLINK_SaveCdsTbl(void);


#ifdef __cplusplus
}
#endif
 
#endif /* MAVLINK_CDS_UTILS_H */

/************************/
/*  End of File Comment */
/************************/
    
