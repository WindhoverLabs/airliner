#ifndef CVT_LIB_H
#define CVT_LIB_H

#include "cfe.h"

#include "cvt_platform_cfg.h"
#include "cvt_mission_cfg.h"

typedef cpuaddr CVT_BufferPtr_t;
typedef uint32 CVT_ContainerID_t;

typedef CVT_ContainerID_t *CVT_ContainerIDPtr_t;

/* Return codes. */
#define CVT_SUCCESS 		      (0)
#define CVT_INCORRECT_SIZE 	      (-1)
#define CVT_REGISTRY_FULL         (-2)
#define CVT_INVALID_ID            (-3)
#define CVT_INVALID_SIZE          (-4)
#define CVT_DATASTORE_FULL        (-5)


int32 CVT_GetContainer(char *Name, uint32 Size, CVT_ContainerID_t *ContainerID);
int32 CVT_GetContent(const CVT_ContainerID_t ContainerID, uint32 *UpdateCount, void* Buffer, uint32 *Size);
int32 CVT_SetContent(const CVT_ContainerID_t ContainerID, void* Buffer, uint32 Size);


#endif /* CVT_LIB_H */
