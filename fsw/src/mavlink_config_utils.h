    
#ifndef MAVLINK_CONFIG_UTILS_H
#define MAVLINK_CONFIG_UTILS_H

/************************************************************************
** Pragmas
*************************************************************************/

/************************************************************************
** Includes
*************************************************************************/
#include "cfe.h"
#include "mavlink_app.h"

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
/** \brief Init MAVLINK Config table
**
**  \par Description
**       This function initializes MAVLINK's Config table
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \returns
**  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS  \endcode
**  \retstmt Return codes from #CFE_TBL_Register          \endcode
**  \retstmt Return codes from #CFE_TBL_Load              \endcode
**  \retstmt Return codes from #MAVLINK_AcquireConfigPointers \endcode
**  \endreturns
**
*************************************************************************/
int32  MAVLINK_InitActionMap(void);

/************************************************************************/
/** \brief Validate MAVLINK Config table
**
**  \par Description
**       This function validates MAVLINK's Config table
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \returns
**  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS  \endcode
**  \endreturns
**
*************************************************************************/
int32  MAVLINK_ValidateActionMap(void*);

/************************************************************************/
/** \brief Obtain MAVLINK Config Table Data Pointer
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
int32 MAVLINK_AcquireParamPointers(void);

/************************************************************************/
/** \brief Process new MAVLINK Config table
**
**  \par Description
**       This function processes MAVLINK's new Config table
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
void   MAVLINK_ProcessNewConfigTbl(void);


#ifdef __cplusplus
}
#endif

#endif /* MAVLINK_CONFIG_UTILS_H */

/************************/
/*  End of File Comment */
/************************/
    
