#include "pq_structs.h"
#include "cfe.h"
#include "pq_events.h"

PQ_AppData_t PQ_AppData;


int32 PQ_LibInit(void)
{
    
    OS_printf ("PQ Lib Initialized.\n");
                
    return OS_SUCCESS;
 
}/* End PQ_LibInit */



int32 PQ_InitData(void)
{
    int32 status = CFE_SUCCESS;

    status = OS_MutSemCreate(&PQ_AppData.MutexID, "PQ_APP", 0);
    if (status != OS_SUCCESS)
    {
        CFE_EVS_SendEvent(PQ_CREATE_APPDATA_MUTEX_ERR_EID,
                CFE_EVS_ERROR,
                "Init Data: Failed to create PQ_AppData mutex: 0x%08lX",
                (unsigned long)status);

        return status;
    }

    PQ_AppData.HkTlm.ChannelMaxMem = PQ_NUM_BYTES_IN_MEM_POOL;
    
    return CFE_SUCCESS;
}


int32 PQ_AppInit(void)
{
    PQ_Channel_InitAll();

    return CFE_SUCCESS;
}


int32 PQ_AppCleanup(void)
{
    //PQ_Channel_CleanupAll();
    OS_MutSemDelete(PQ_AppData.MutexID);

    return CFE_SUCCESS;
}
