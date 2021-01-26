/*************************************************************************
** Includes
*************************************************************************/
#include "sim_ifx.h"
#include <ctype.h>
#include <string.h>
#include "sim_version.h"

/*************************************************************************
** Macro Definitions
*************************************************************************/
/**

/*************************************************************************
** Private Function Prototypes
*************************************************************************/
int32 SIM_LibInit(void);


int32 SIM_LibInit(void)
{
    OS_printf ("SIM Interface Initialized.  Version %d.%d.%d.%d\n",
                SIM_MAJOR_VERSION,
                SIM_MINOR_VERSION,
                SIM_REVISION,
                SIM_MISSION_REV);
                
    return OS_SUCCESS;
 
}/* End CFS_LibInit */



/************************/
/*  End of File Comment */
/************************/
