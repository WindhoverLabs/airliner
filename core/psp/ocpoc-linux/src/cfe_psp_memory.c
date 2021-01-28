/******************************************************************************
 ** File:  cfe_psp_memory.c
 **
 **      OSX x86 Linux Version
 **
 **      Copyright (c) 2004-2006, United States government as represented by the
 **      administrator of the National Aeronautics Space Administration.
 **      All rights reserved. This software(cFE) was created at NASA Goddard
 **      Space Flight Center pursuant to government contracts.
 **
 **      This software may be used only pursuant to a United States government
 **      sponsored project and the United States government may not be charged
 **      for use thereof.
 **
 **
 ** Purpose:
 **   cFE PSP Memory related functions. This is the implementation of the cFE
 **   memory areas that have to be preserved, and the API that is designed to
 **   allow access to them. It also contains memory related routines to return
 **   the address of the kernel code used in the cFE checksum.
 **
 ** History:
 **   2006/09/29  A. Cudmore      | Initial version for OS X
 **
 ******************************************************************************/

/*
 **  Include Files
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/shm.h>

/*
 ** cFE includes
 */
#include "common_types.h"
#include "osapi.h"
#include "cfe_es.h"            /* For reset types */
#include "cfe_platform_cfg.h"  /* for processor ID */

/*
 ** Types and prototypes for this module
 */
#include "cfe_psp.h"            

/*
 ** PSP Specific defines
 */
#include "cfe_psp_config.h"

#define CFE_PSP_CDS_SIZE            CFE_ES_CDS_SIZE
#define CFE_PSP_RESET_AREA_SIZE     CFE_ES_RESET_AREA_SIZE
#define CFE_PSP_USER_RESERVED_SIZE  CFE_ES_USER_RESERVED_SIZE

/*
 ** Internal prototypes for this module
 */
int32 CFE_PSP_InitCDS(uint32 RestartType);
int32 CFE_PSP_InitResetArea(uint32 RestartType);
int32 CFE_PSP_InitVolatileDiskMem(uint32 RestartType);
int32 CFE_PSP_InitUserReservedArea(uint32 RestartType);

/*
 **  External Declarations
 */
extern unsigned int _init;
extern unsigned int _fini;

/*
 ** Global variables
 */
uint8 CFE_PSP_CDS[CFE_PSP_CDS_SIZE];
uint8 CFE_PSP_ResetArea[CFE_PSP_RESET_AREA_SIZE];
uint8 CFE_PSP_UserReservedArea[CFE_PSP_USER_RESERVED_SIZE];

/*
 *******************************************************************************
 ** CDS related functions
 *******************************************************************************
 */
/*
 **
 */
/******************************************************************************
 **  Function: CFE_PSP_InitCDS
 **
 **  Purpose: This function is used by the ES startup code to initialize the
 **            Critical Data store area
 **
 **
 **  Arguments:
 **    (none)
 **
 **  Return:
 **    (none)
 */
int32 CFE_PSP_InitCDS(uint32 RestartType)
{
    memset(&(CFE_PSP_CDS[0]), 0, sizeof(CFE_PSP_CDS));

    return CFE_PSP_SUCCESS;
}

/******************************************************************************
 **  Function: CFE_PSP_GetCDSSize
 **
 **  Purpose:
 **    This function fetches the size of the OS Critical Data Store area.
 **
 **  Arguments:
 **    (none)
 **
 **  Return:
 **    (none)
 */
int32 CFE_PSP_GetCDSSize(uint32 *SizeOfCDS)
{
    int32 return_code;

    if (SizeOfCDS == NULL)
    {
        return_code = CFE_PSP_ERROR;
    }
    else
    {
        *SizeOfCDS = CFE_PSP_CDS_SIZE;
        return_code = CFE_PSP_SUCCESS;
    }

    return return_code;
}

/******************************************************************************
 **  Function: CFE_PSP_WriteToCDS
 **
 **  Purpose:
 **    This function writes to the CDS Block.
 **
 **  Arguments:
 **    (none)
 **
 **  Return:
 **    (none)
 */
int32 CFE_PSP_WriteToCDS(void *PtrToDataToWrite, uint32 CDSOffset,
        uint32 NumBytes)
{
    uint8 *CopyPtr;
    int32 return_code;

    if (PtrToDataToWrite == NULL)
    {
        return_code = CFE_PSP_ERROR;
    }
    else
    {
        if ((CDSOffset < CFE_PSP_CDS_SIZE)
                && ((CDSOffset + NumBytes) <= CFE_PSP_CDS_SIZE))
        {
            CopyPtr = &CFE_PSP_CDS[CDSOffset];
            memcpy(CopyPtr, (char *) PtrToDataToWrite, NumBytes);

            return_code = CFE_PSP_SUCCESS;
        }
        else
        {
            return_code = CFE_PSP_ERROR;
        }

    } /* end if PtrToDataToWrite == NULL */

    return return_code;
}

/******************************************************************************
 **  Function: CFE_PSP_ReadFromCDS
 **
 **  Purpose:
 **   This function reads from the CDS Block
 **
 **  Arguments:
 **    (none)
 **
 **  Return:
 **    (none)
 */
int32 CFE_PSP_ReadFromCDS(void *PtrToDataToRead, uint32 CDSOffset,
        uint32 NumBytes)
{
    uint8 *CopyPtr;
    int32 return_code;

    if (PtrToDataToRead == NULL)
    {
        return_code = CFE_PSP_ERROR;
    }
    else
    {
        if ((CDSOffset < CFE_PSP_CDS_SIZE)
                && ((CDSOffset + NumBytes) <= CFE_PSP_CDS_SIZE))
        {
            CopyPtr = &CFE_PSP_CDS[CDSOffset];
            memcpy((char *) PtrToDataToRead, CopyPtr, NumBytes);

            return_code = CFE_PSP_SUCCESS;
        }
        else
        {
            return_code = CFE_PSP_ERROR;
        }

    } /* end if PtrToDataToWrite == NULL */

    return return_code;

}

/*
 *******************************************************************************
 ** ES Reset Area related functions
 *******************************************************************************
 */

/******************************************************************************
 **  Function: CFE_PSP_InitESResetArea
 **
 **  Purpose:
 **    This function is used by the ES startup code to initialize the
 **     ES Reset Area.
 **
 **  Arguments:
 **    (none)
 **
 **  Return:
 **    (none)
 */
int32 CFE_PSP_InitResetArea(uint32 RestartType)
{
    memset(&(CFE_PSP_ResetArea[0]), 0, sizeof(CFE_PSP_ResetArea));

    return CFE_PSP_SUCCESS;
}

/*
 */
/******************************************************************************
 **  Function: CFE_PSP_GetResetArea
 **
 **  Purpose:
 **     This function returns the location and size of the ES Reset information
 **     area.  This area is preserved during a processor reset and is used to
 **     store the ER Log, System Log and reset related variables
 **
 **  Arguments:
 **    (none)
 **
 **  Return:
 **    (none)
 */
int32 CFE_PSP_GetResetArea(cpuaddr *PtrToResetArea, uint32 *SizeOfResetArea)
{
    int32 return_code;

    if (SizeOfResetArea == NULL)
    {
        return_code = CFE_PSP_ERROR;
    }
    else
    {
        uint8 *ptr = CFE_PSP_ResetArea;
        memcpy(PtrToResetArea, &ptr, sizeof(PtrToResetArea));
        *SizeOfResetArea = CFE_PSP_RESET_AREA_SIZE;
        return_code = CFE_PSP_SUCCESS;
    }

    return return_code;
}

/*
 ******************************************************************************
 ** ES User Reserved Area related functions
 ******************************************************************************
 */

/******************************************************************************
 **  Function: CFE_PSP_InitUserReservedArea
 **
 **  Purpose:
 **    This function is used by the ES startup code to initialize the
 **      ES user reserved area.
 **
 **  Arguments:
 **    (none)
 **
 **  Return:
 **    (none)
 */
int32 CFE_PSP_InitUserReservedArea(uint32 RestartType)
{
    memset(&(CFE_PSP_UserReservedArea[0]), 0, sizeof(CFE_PSP_CDS));

    return CFE_PSP_SUCCESS;

}

/******************************************************************************
 **  Function: CFE_PSP_GetUserReservedArea
 **
 **  Purpose:
 **    This function returns the location and size of the memory used for the
 **    cFE User reserved area.
 **
 **  Arguments:
 **    (none)
 **
 **  Return:
 **    (none)
 */
int32 CFE_PSP_GetUserReservedArea(cpuaddr *PtrToUserArea,
        uint32 *SizeOfUserArea)
{
    int32 return_code;

    if (SizeOfUserArea == NULL)
    {
        return_code = CFE_PSP_ERROR;
    }
    else
    {
        uint8 *ptr = CFE_PSP_UserReservedArea;
        memcpy(PtrToUserArea, &ptr, sizeof(PtrToUserArea));
        *SizeOfUserArea = CFE_PSP_USER_RESERVED_SIZE;
        return_code = CFE_PSP_SUCCESS;
    }

    return return_code;
}

/*
 ******************************************************************************
 ** ES Volatile disk memory related functions
 ******************************************************************************
 */

/******************************************************************************
 **  Function: CFE_PSP_InitVolatileDiskMem
 **
 **  Purpose:
 **   This function is used by the ES startup code to initialize the memory
 **   used by the volatile disk. On a desktop/posix platform this is currently
 **   a no-op, because the volatile disk is being mapped to the desktop.
 **
 **  Arguments:
 **    (none)
 **
 **  Return:
 **    (none)
 */
int32 CFE_PSP_InitVolatileDiskMem(uint32 RestartType)
{
    int32 return_code;

    /*
     ** Here, we want to clear out the volatile ram disk contents
     ** on a power on reset
     */

    return_code = CFE_PSP_SUCCESS;
    return return_code;

}

/******************************************************************************
 **  Function: CFE_PSP_GetVolatileDiskMem
 **
 **  Purpose:
 **    This function returns the location and size of the memory used for the
 **    cFE volatile disk.
 **
 **  Arguments:
 **    (none)
 **
 **  Return:
 **    (none)
 */
int32 CFE_PSP_GetVolatileDiskMem(cpuaddr *PtrToVolDisk, uint32 *SizeOfVolDisk)
{
    int32 return_code;

    if (SizeOfVolDisk == NULL)
    {
        return_code = CFE_PSP_ERROR;
    }
    else
    {
        memset(&(PtrToVolDisk), 0, sizeof(PtrToVolDisk));
        *SizeOfVolDisk = 0;
        return_code = CFE_PSP_SUCCESS;
    }

    return return_code;

}

/*
 ******************************************************************************
 ** ES BSP Top Level Reserved memory initialization
 ******************************************************************************
 */

/******************************************************************************
 **  Function: CFE_PSP_InitProcessorReservedMemory
 **
 **  Purpose:
 **    This function performs the top level reserved memory initialization.
 **
 **  Arguments:
 **    (none)
 **
 **  Return:
 **    (none)
 */
int32 CFE_PSP_InitProcessorReservedMemory(uint32 RestartType)
{
    int32 return_code;

    if (RestartType == CFE_ES_PROCESSOR_RESET)
    {
        return_code = CFE_PSP_InitCDS( CFE_ES_PROCESSOR_RESET);
        return_code = CFE_PSP_InitResetArea( CFE_ES_PROCESSOR_RESET);
        return_code = CFE_PSP_InitVolatileDiskMem( CFE_ES_PROCESSOR_RESET);
        return_code = CFE_PSP_InitUserReservedArea( CFE_ES_PROCESSOR_RESET);
    }
    else
    {
        return_code = CFE_PSP_InitCDS( CFE_ES_POWERON_RESET);
        return_code = CFE_PSP_InitResetArea( CFE_ES_POWERON_RESET);
        return_code = CFE_PSP_InitVolatileDiskMem( CFE_ES_POWERON_RESET);
        return_code = CFE_PSP_InitUserReservedArea( CFE_ES_POWERON_RESET);
    }

    return return_code;
}

/*
 ******************************************************************************
 ** ES BSP kernel memory segment functions
 ******************************************************************************
 */

/******************************************************************************
 **  Function: CFE_PSP_GetKernelTextSegmentInfo
 **
 **  Purpose:
 **    This function returns the start and end address of the kernel text
 **    segment.  It may not be implemented on all architectures.
 **
 **  Arguments:
 **    (none)
 **
 **  Return:
 **    (none)
 */
int32 CFE_PSP_GetKernelTextSegmentInfo(cpuaddr *PtrToKernelSegment,
        uint32 *SizeOfKernelSegment)
{
    int32 return_code;

    /*
     ** Prevent warnings by referencing parameters
     */
    if (PtrToKernelSegment == NULL || SizeOfKernelSegment == NULL)
    {
        return_code = CFE_PSP_ERROR;
    }
    else
    {
        return_code = CFE_PSP_ERROR_NOT_IMPLEMENTED;
    }

    return return_code;

}

/******************************************************************************
 **  Function: CFE_PSP_GetCFETextSegmentInfo
 **
 **  Purpose:
 **    This function returns the start and end address of the CFE text segment.
 **     It may not be implemented on all architectures.
 **
 **  Arguments:
 **    (none)
 **
 **  Return:
 **    (none)
 */
int32 CFE_PSP_GetCFETextSegmentInfo(cpuaddr *PtrToCFESegment,
        uint32 *SizeOfCFESegment)
{
    int32 return_code;
    uint32 Address;

    if (SizeOfCFESegment == NULL)
    {
        return_code = CFE_PSP_ERROR;
    }
    else
    {
        Address = (uint32)(&_init);
        memcpy(PtrToCFESegment, &Address, sizeof(PtrToCFESegment));
        *SizeOfCFESegment = (uint32)((uint32) & _fini - (uint32) & _init);

        return_code = CFE_PSP_SUCCESS;
    }

    return return_code;
}

