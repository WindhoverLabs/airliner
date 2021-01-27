/*
** Include Files
*/
#include "cfe_tbl_filedef.h"
#include "flow_tbldefs.h"


/* Default FLOW config table data */
FLOW_ConfigTbl_t FLOW_ConfigTbl =
{
		/** \brief Horizontal field of view.
		 *
		 *  \par Limits:
		 *       default 0.088.
		 */
		0.088f,
		/** \brief Size of sliding window.
		 *
		 *  \par Limits:
		 *       Min > Max (incr.) 2 > 10, default 8.
		 */
		8,
		/** \brief Number of blocks of sliding window.
		 *
		 *  \par Limits:
		 *       Min > Max (incr.) 2 > 10, default 5.
		 */
		5,
		/** \brief Default image output rate.
		 *
		 *  \par Limits:
		 *       default 15.
		 */
		10,
		/** \brief Default image width.
		 *
		 *  \par Limits:
		 *       default 64.
		 */
		64,
		/** \brief Default image height.
		 *
		 *  \par Limits:
		 *       default 64.
		 */
		64,
		/** \brief Default image search size.
		 *
		 *  \par Limits:
		 *       default 6.
		 */
		6,
		/** \brief Default feature threshold.
		 *
		 *  \par Limits:
		 *       default 30.
		 */
		30,
		/** \brief Default value threshold.
		 *
		 *  \par Limits:
		 *       default 3000.
		 */
		3000,
		/** \brief Mean count threshold.
		 *
		 *  \par Limits:
		 *       default 10.
		 */
		10
};


/* Table file header */
CFE_TBL_FILEDEF(FLOW_ConfigTbl, FLOW.CONFIG_TBL, FLOW default config table, flow_config.tbl )

/*=======================================================================================
** End of file flow_config.c
**=====================================================================================*/
    
