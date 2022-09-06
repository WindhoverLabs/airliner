#include "cfe.h"

typedef cpuaddr CVT_BufferPtr_t;
typedef uint32 CVT_ContainerID_t;

typedef CVT_ContainerID_t *CVT_ContainerIDPtr_t;

#define CVT_SUCCESS 		      (0)

int32 CVT_GetContainer(char *Name, uint32 Size, CVT_ContainerID_t *ContainerID) {
    return CVT_SUCCESS;
}

int32 CVT_GetContent(const CVT_ContainerID_t ContainerID, uint32 *UpdateCount, void* Buffer, uint32 *Size) {
    return CVT_SUCCESS;
}

int32 CVT_SetContent(const CVT_ContainerID_t ContainerID, void* Buffer, uint32 Size) {
    return CVT_SUCCESS;
}

