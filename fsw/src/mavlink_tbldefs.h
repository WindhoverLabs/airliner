    
#ifndef MAVLINK_TBLDEFS_H
#define MAVLINK_TBLDEFS_H

/************************************************************************
** Pragmas
*************************************************************************/

/************************************************************************
** Includes
*************************************************************************/
#include "cfe.h"
#include "mavlink_platform_cfg.h"

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************
** Local Defines
*************************************************************************/
/**
 * \brief Defines the number of entries in the table
 */
#define MAVLINK_PARAM_TABLE_MAX_ENTRIES  (10)

/**
 * \brief Defines the table identification name used for table registration.
 */
#define MAVLINK_ACTION_MAP_TABLENAME          ("ACTION_MAP")

/**
 * \brief Defines the table file name used for table registration.
 */
#define MAVLINK_CDS_TABLENAME  ("mavlink_CdsTbl")

/************************************************************************
** Local Structure Declarations
*************************************************************************/

/** \brief  */
typedef enum
{
	ACTION_PASSTHRU,
	ACTION_HANDLE
} MAVLINK_MsgAction_t;


/** \brief Definition for a single config table entry */
typedef struct
{
	uint8 				  MsgId;
	MAVLINK_MsgAction_t   Action;
} MAVLINK_ActionMapData_t;

/** \brief Definition for a single config table entry */
typedef struct
{
    MAVLINK_ActionMapData_t   ActionMap[MAVLINK_ACTION_MAP_ENTRIES];
} MAVLINK_ActionMapTbl_t;


/** \brief Definition for Critical Data Storage (CDS) table entry */
typedef struct
{
    int32  iParam;

    /* TODO:  Add type declaration for CDS data here. */
} MAVLINK_CdsTbl_t;

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


#ifdef __cplusplus
}
#endif

#endif /* MAVLINK_TBLDEFS_H */

/************************/
/*  End of File Comment */
/************************/
