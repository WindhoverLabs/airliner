
#include "ipc_lib.h"

int32 IPC_Lib_MemCpy(void *dst, void *src, uint32 size)
{
    int32 status = 0;

    status = CFE_PSP_MemCpy(dst, src, size);

    return status;
}
