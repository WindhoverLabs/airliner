#include "pq_structs.h"
#include "cfe.h"

PQ_AppData_t PQ_AppData;


int32 PQ_LibInit(void)
{
    
    OS_printf ("PQ Lib Initialized.\n");
                
    return OS_SUCCESS;
 
}/* End PQ_LibInit */
