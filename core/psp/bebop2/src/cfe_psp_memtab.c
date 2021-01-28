/*
 ** File   :   cfe_psp_memtab.c
 **
 **
 **      Copyright (c) 2004-2006, United States government as represented by the
 **      administrator of the National Aeronautics Space Administration.
 **      All rights reserved.
 **
 ** Author :     Alan Cudmore
 **
 ** Purpose:     Memory Range Table for cFE/PSP.
 **
 **
 */

/*
 ** Includes
 */

#include "common_types.h"
#include "osapi.h"
#include "cfe_psp.h"
#include "cfe_psp_config.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <errno.h>

/*
 ** Defines
 */
#define EEPROM_FILE "EEPROM.DAT"

/*
 ** Valid memory map for this target.
 ** If you need to add more entries, increase CFE_PSP_MEM_TABLE_SIZE in the
 ** osconfig.h file.
 */
CFE_PSP_MemTable_t CFE_PSP_MemoryTable[CFE_PSP_MEM_TABLE_SIZE] =
{
    { CFE_PSP_MEM_RAM, CFE_PSP_MEM_SIZE_DWORD, 0, 0xFFFFFFFF,
        CFE_PSP_MEM_ATTR_READWRITE },
    { CFE_PSP_MEM_INVALID, 0, 0, 0, CFE_PSP_MEM_ATTR_READWRITE },
    { CFE_PSP_MEM_INVALID, 0, 0, 0, CFE_PSP_MEM_ATTR_READWRITE },
    { CFE_PSP_MEM_INVALID, 0, 0, 0, CFE_PSP_MEM_ATTR_READWRITE },
    { CFE_PSP_MEM_INVALID, 0, 0, 0, CFE_PSP_MEM_ATTR_READWRITE },
    { CFE_PSP_MEM_INVALID, 0, 0, 0, CFE_PSP_MEM_ATTR_READWRITE },
    { CFE_PSP_MEM_INVALID, 0, 0, 0, CFE_PSP_MEM_ATTR_READWRITE },
    { CFE_PSP_MEM_INVALID, 0, 0, 0, CFE_PSP_MEM_ATTR_READWRITE },
    { CFE_PSP_MEM_INVALID, 0, 0, 0, CFE_PSP_MEM_ATTR_READWRITE },
    { CFE_PSP_MEM_INVALID, 0, 0, 0, CFE_PSP_MEM_ATTR_READWRITE },
};

/*
 ** Simulate EEPROM by mapping in a file
 */
int32 CFE_PSP_SetupEEPROM(uint32 EEPROMSize, uint32 *EEPROMAddress)
{
    return 0;
}
