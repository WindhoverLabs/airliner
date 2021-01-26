/*
**  File Name:  cfe_psp.h
**
**      Copyright (c) 2004-2011, United States Government as represented by 
**      Administrator for The National Aeronautics and Space Administration. 
**      All Rights Reserved.
**
**      This is governed by the NASA Open Source Agreement and may be used,
**      distributed and modified only pursuant to the terms of that agreement.
**
**  Author:  A. Cudmore
**
**  Purpose:  This file contains the cFE Platform Support Package(PSP) 
**            prototypes.
**            The PSP routines serve as the "glue" between the RTOS and
**            the cFE Flight Software. The routines fill gaps that are not
**            really considered part of the OS Abstraction, but are required
**            for the cFE flight software implementation. It is possible that
**            some of these routines could migrate into the OS AL.
**
**  $Log: cfe_psp.h  $
**  Revision 1.3 2009/07/29 12:04:46GMT-05:00 acudmore 
**  Added Bank parameter to EEPROM Power up/down and EEPROM write enable/disable functions.
**  Revision 1.2 2009/07/22 17:34:10EDT acudmore 
**  Added new watchdog API
**  Revision 1.1 2009/06/10 09:28:44EDT acudmore 
**  Initial revision
**  Member added to project c:/MKSDATA/MKS-REPOSITORY/CFE-PSP-REPOSITORY/fsw/inc/project.pj
**
*/

#ifndef _cfe_psp_
#define _cfe_psp_

/*
** Include Files
*/

#include "common_types.h"
#include "osapi.h"

/*
 * These internal header files should not be used in application code
 * They will not be accessible when using the newest build scripts
 */
#if !defined(_ENHANCED_BUILD_) || defined(_CFE_PSP_)
#include "cfe_psp_config.h"
#include "psp_version.h"
#endif

/*
** Macro Definitions
*/

/*
** Error and return codes
*/
#define CFE_PSP_SUCCESS                     (0)
#define CFE_PSP_ERROR                       (-1)
#define CFE_PSP_INVALID_POINTER             (-2)
#define CFE_PSP_ERROR_ADDRESS_MISALIGNED    (-3)
#define CFE_PSP_ERROR_TIMEOUT               (-4)
#define CFE_PSP_INVALID_INT_NUM             (-5)
#define CFE_PSP_INVALID_MEM_ADDR            (-21)
#define CFE_PSP_INVALID_MEM_TYPE            (-22)
#define CFE_PSP_INVALID_MEM_RANGE           (-23)
#define CFE_PSP_INVALID_MEM_WORDSIZE        (-24)
#define CFE_PSP_INVALID_MEM_SIZE            (-25)
#define CFE_PSP_INVALID_MEM_ATTR            (-26)
#define CFE_PSP_ERROR_NOT_IMPLEMENTED       (-27)
#define CFE_PSP_INVALID_MODULE_NAME         (-28)
#define CFE_PSP_INVALID_MODULE_ID           (-29)



/*
** Definitions for PSP PANIC types
*/
#define CFE_PSP_PANIC_STARTUP         1
#define CFE_PSP_PANIC_VOLATILE_DISK   2
#define CFE_PSP_PANIC_MEMORY_ALLOC    3
#define CFE_PSP_PANIC_NONVOL_DISK     4
#define CFE_PSP_PANIC_STARTUP_SEM     5
#define CFE_PSP_PANIC_CORE_APP        6
#define CFE_PSP_PANIC_GENERAL_FAILURE 7

/*
** Macros for the file loader
*/
#define BUFF_SIZE         256
#define SIZE_BYTE         1
#define SIZE_HALF         2
#define SIZE_WORD         3

/*
** Define memory types
*/
#define CFE_PSP_MEM_RAM        1
#define CFE_PSP_MEM_EEPROM     2
#define CFE_PSP_MEM_ANY        3
#define CFE_PSP_MEM_INVALID    4

/*
** Define Memory Read/Write Attributes
*/
#define CFE_PSP_MEM_ATTR_WRITE     0x01
#define CFE_PSP_MEM_ATTR_READ      0x02
#define CFE_PSP_MEM_ATTR_READWRITE 0x03

/*
** Define the Memory Word Sizes
*/
#define CFE_PSP_MEM_SIZE_BYTE     0x01
#define CFE_PSP_MEM_SIZE_WORD     0x02
#define CFE_PSP_MEM_SIZE_DWORD    0x04

/*
 * Common definition for reset types at the PSP layer
 */
/** \name Reset Types */
/** \{ */
#define CFE_PSP_RST_TYPE_PROCESSOR   1       /**< Volatile disk, Critical Data Store and User Reserved memory could still be valid */
#define CFE_PSP_RST_TYPE_POWERON     2       /**< All memory has been cleared */
#define CFE_PSP_RST_TYPE_MAX         3       /**< Placeholder to indicate 1+ the maximum value that the PSP will ever use. */
/** \} */

/*
** Reset Sub-Types
*/
/** \name Reset Sub-Types */
/** \{ */
#define CFE_PSP_RST_SUBTYPE_POWER_CYCLE           1   /**< \brief  Reset caused by power having been removed and restored */
#define CFE_PSP_RST_SUBTYPE_PUSH_BUTTON           2   /**< \brief  Reset caused by reset button on the board having been pressed */
#define CFE_PSP_RST_SUBTYPE_HW_SPECIAL_COMMAND    3   /**< \brief  Reset was caused by a reset line having been stimulated by a hardware special command */
#define CFE_PSP_RST_SUBTYPE_HW_WATCHDOG           4   /**< \brief  Reset was caused by a watchdog timer expiring */
#define CFE_PSP_RST_SUBTYPE_RESET_COMMAND         5   /**< \brief  Reset was caused by cFE ES processing a \link #CFE_ES_RESTART_CC Reset Command \endlink */
#define CFE_PSP_RST_SUBTYPE_EXCEPTION             6   /**< \brief  Reset was caused by a Processor Exception */
#define CFE_PSP_RST_SUBTYPE_UNDEFINED_RESET       7   /**< \brief  Reset was caused in an unknown manner */
#define CFE_PSP_RST_SUBTYPE_HWDEBUG_RESET         8   /**< \brief  Reset was caused by a JTAG or BDM connection */
#define CFE_PSP_RST_SUBTYPE_BANKSWITCH_RESET      9   /**< \brief  Reset reverted to a cFE POWERON due to a boot bank switch */
#define CFE_PSP_RST_SUBTYPE_MAX                   10  /**< \brief  Placeholder to indicate 1+ the maximum value that the PSP will ever use. */
/** \} */

/* Replacements for the "version" macros */
#ifdef _ENHANCED_BUILD_

#define CFE_PSP_MAJOR_VERSION          (GLOBAL_PSP_CONFIGDATA.PSP_VersionInfo.MajorVersion)
#define CFE_PSP_MINOR_VERSION          (GLOBAL_PSP_CONFIGDATA.PSP_VersionInfo.MinorVersion)
#define CFE_PSP_REVISION               (GLOBAL_PSP_CONFIGDATA.PSP_VersionInfo.Revision)
#define CFE_PSP_MISSION_REV            (GLOBAL_PSP_CONFIGDATA.PSP_VersionInfo.MissionRev)

/* For backwards compatibility */
#define CFE_PSP_SUBMINOR_VERSION       CFE_PSP_REVISION

#endif

/*
** Type Definitions
*/

/*
** Memory table type
*/
typedef struct
{
   uint32 MemoryType;
   uint32 WordSize;
   cpuaddr StartAddr;
   uint32 Size;
   uint32 Attributes;
} CFE_PSP_MemTable_t;

/*
** Function prototypes
*/

/************************************************************************/
/** \brief PSP Main
**
**  \par Description
**       PSP entry point and reset routines
**
**  \par Assumptions, External Events, and Notes:
**       CFE_PSP_Main is the entry point that the real time OS calls to start our
**       software. This routine will do any BSP/OS specific setup, then call the
**       entrypoint of the flight software ( i.e. the cFE main entry point ).
**       The flight software (i.e. cFE ) should not call this routine.
**
**  \param [in]   ModeId             #uint32 Run mode
**  \param [in]   StartupFilePath    Path to startup file
**  
*************************************************************************/
extern void          CFE_PSP_Main(uint32 ModeId, char *StartupFilePath);

/************************************************************************/
/** \brief Get time
**
**  \par Description
**       Gets the value of the time from the hardware
**
**  \par Assumptions, External Events, and Notes:
**       This call gets the local time from the hardware on the Vxworks system
**       on the mcp750s
**       on the other os/hardware setup, it will get the time the normal way
**
**  \param [in/out]   LocalTime           #OS_time_t Pointer to time struct
**  
*************************************************************************/
extern void         CFE_PSP_GetTime(OS_time_t *LocalTime);

/************************************************************************/
/** \brief Restart Processor
**
**  \par Description
**       CFE_PSP_Restart is the entry point back to the BSP to restart the processor.
**       The flight software calls this routine to restart the processor.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]   resetType       #uint32 #CFE_ES_PROCESSOR_RESET or #CFE_ES_POWERON_RESET
**  
*************************************************************************/
extern void          CFE_PSP_Restart(uint32 resetType);

/************************************************************************/
/** \brief Get restart type
**
**  \par Description
**       Retrieve the CFE PSP reset type and subtype.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in/out]   restartSubType       #uint32 Sub type of restart type
**
**  \returns
**       Restart type
**  \endreturns
**  
*************************************************************************/
extern uint32        CFE_PSP_GetRestartType(uint32 *restartSubType );
/*
** CFE_PSP_GetRestartType returns the last reset type and if a pointer to a valid
** memory space is passed in, it returns the reset sub-type in that memory.
** Right now the reset types are application specific. For the cFE they
** are defined in the cfe_es.h file.
*/

/************************************************************************/
/** \brief Flush caches
**
**  \par Description
**       Provides a common interface to flush the processor caches. This routine
**       is in the BSP because it is sometimes implemented in hardware and
**       sometimes taken care of by the RTOS.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]   type          #uint32 Type of flush
**  \param [in]   address       #cpuaddr Pointer to TODO: this isn't implementted I don't know
**  \param [in]   size          #uint32 Size
**  
*************************************************************************/
extern void          CFE_PSP_FlushCaches(uint32 type, cpuaddr address, uint32 size);

/************************************************************************/
/** \brief Get processor ID
**
**  \par Description
**       CFE_PSP_GetProcessorId returns the CPU ID as defined by the specific board
**       and BSP.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \returns
**       Processor ID
**  \endreturns
**  
*************************************************************************/
extern uint32        CFE_PSP_GetProcessorId ( void );

/************************************************************************/
/** \brief Get spacecraft ID
**
**  \par Description
**       CFE_PSP_GetSpacecraftId retuns the Spacecraft ID (if any )
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \returns
**       Spacecraft ID
**  \endreturns
**  
*************************************************************************/
extern uint32        CFE_PSP_GetSpacecraftId ( void );

/************************************************************************/
/** \brief Get timer tick
**
**  \par Description
**       Provides a common interface to system clock tick. This routine
**       is in the BSP because it is sometimes implemented in hardware and
**       sometimes taken care of by the RTOS.
**
**  \par Assumptions, External Events, and Notes:
**       None
**  
**  \returns
**       OS system clock ticks per second
**  \endreturns
**
*************************************************************************/
extern uint32 CFE_PSP_Get_Timer_Tick(void);

/************************************************************************/
/** \brief Get timer ticks per second
**
**  \par Description
**       Provides the resolution of the least significant 32 bits of the 64 bit
**       time stamp returned by OS_BSPGet_Timebase in timer ticks per second.
**       The timer resolution for accuracy should not be any slower than 1000000
**       ticks per second or 1 us per tick
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \returns
**       The number of timer ticks per second of the time stamp returned
**       by CFE_PSP_Get_Timebase
**  \endreturns
**
*************************************************************************/
extern uint32 CFE_PSP_GetTimerTicksPerSecond(void);

/************************************************************************/
/** \brief Get timer ticks per second
**
**  \par Description
**       CFE_PSP_GetTimerLow32Rollover provides the number that the least significant
**       32 bits of the 64 bit time stamp returned by CFE_PSP_Get_Timebase rolls over.
**       If the lower 32 bits rolls at 1 second, then the CFE_PSP_TIMER_LOW32_ROLLOVER
**       will be 1000000.  if the lower 32 bits rolls at its maximum value (2^32) then
**       CFE_PSP_TIMER_LOW32_ROLLOVER will be 0.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \returns
**       The number that the least significant 32 bits of the 64 bit time stamp
**       returned by CFE_PSP_Get_Timebase rolls over.
**  \endreturns
**
*************************************************************************/
extern uint32 CFE_PSP_GetTimerLow32Rollover(void);

/************************************************************************/
/** \brief Get timebase
**
**  \par Description
**       Provides a common interface to system timebase. This routine
**       is in the BSP because it is sometimes implemented in hardware and
**       sometimes taken care of by the RTOS.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in/out]   Tbu           #uint32 Pointer to seconds
**  \param [in/out]   Tbl           #uint32 Pointer to microseconds
**  
*************************************************************************/
extern void CFE_PSP_Get_Timebase(uint32 *Tbu, uint32 *Tbl);

/************************************************************************/
/** \brief Get decrementer
**
**  \par Description
**       Provides a common interface to decrementer counter. This routine
**       is in the PSP because it is sometimes implemented in hardware and
**       sometimes taken care of by the RTOS.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \returns
**       Timebase register value
**  \endreturns
**
*************************************************************************/
extern uint32 CFE_PSP_Get_Dec(void);

/************************************************************************/
/** \brief Initialize Processor Reserved Memory
**
**  \par Description
**       This function performs the top level reserved memory initialization.
**
**  \par Assumptions, External Events, and Notes:
**       CFE_PSP_InitProcessorReservedMemory initializes all of the memory in the
**       BSP that is preserved on a processor reset. The memory includes the
**       Critical Data Store, the ES Reset Area, the Volatile Disk Memory, and
**       the User Reserved Memory. In general, the memory areas will be initialized
**       ( cleared ) on a Power On reset, and preserved during a processor reset.
**
**  \param [in]   RestartType           #uint32 #CFE_ES_PROCESSOR_RESET or #CFE_ES_POWERON_RESET
**
**  \returns
**  \retcode #CFE_PSP_ERROR \endcode TODO: verify
**  \retcode #CFE_PSP_SUCCESS \endcode
**  \endreturns
** 
*************************************************************************/
extern int32 CFE_PSP_InitProcessorReservedMemory(uint32 RestartType );

/************************************************************************/
/** \brief Get CDS size
**
**  \par Description
**       This function fetches the size of the OS Critical Data Store area.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in/out]   SizeOfCDS           #uint32 Pointer to size
**
*************************************************************************/
extern int32 CFE_PSP_GetCDSSize(uint32 *SizeOfCDS);

/************************************************************************/
/** \brief Write to CDS
**
**  \par Description
**       This function writes to the CDS Block.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in/out]   PtrToDataToWrite        #uint32 Pointer to data source
**  \param [in]       CDSOffset               #uint32 Offset for write location in CDS
**  \param [in]       NumBytes                #uint32 Bytes to write
**
*************************************************************************/
extern int32 CFE_PSP_WriteToCDS(void *PtrToDataToWrite, uint32 CDSOffset, uint32 NumBytes);

/************************************************************************/
/** \brief Write to CDS
**
**  \par Description
**       This function reads from the CDS Block
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in/out]   PtrToDataToRead         #uint32 Buffer to read data into
**  \param [in]       CDSOffset               #uint32 Offset for read location in CDS
**  \param [in]       NumBytes                #uint32 Bytes to read
**
*************************************************************************/
extern int32 CFE_PSP_ReadFromCDS(void *PtrToDataToRead, uint32 CDSOffset, uint32 NumBytes);

/************************************************************************/
/** \brief Get reset area
**
**  \par Description
**       This function returns the location and size of the ES Reset information area. 
**       This area is preserved during a processor reset and is used to store the 
**       ER Log, System Log and reset related variables
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in/out]   PtrToResetArea         #cpuaddr Pointer reset area
**  \param [in/out]   SizeOfResetArea        #uint32 Size of reset area
**
**  \returns
**  \retcode #CFE_PSP_ERROR \endcode
**  \retcode #CFE_PSP_SUCCESS \endcode
**  \endreturns
**
*************************************************************************/
extern int32 CFE_PSP_GetResetArea (cpuaddr *PtrToResetArea, uint32 *SizeOfResetArea);

/************************************************************************/
/** \brief Get user reserved area
**
**  \par Description
**       This function returns the location and size of the memory used for the cFE
**       User reserved area.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in/out]   PtrToUserArea         #cpuaddr Pointer user area
**  \param [in/out]   SizeOfUserArea        #uint32 Size of user area
**
**  \returns
**  \retcode #CFE_PSP_ERROR \endcode
**  \retcode #CFE_PSP_SUCCESS \endcode
**  \endreturns
**
*************************************************************************/
extern int32 CFE_PSP_GetUserReservedArea(cpuaddr *PtrToUserArea, uint32 *SizeOfUserArea );

/************************************************************************/
/** \brief Get volatile area
**
**  \par Description
**       This function returns the location and size of the memory used for the cFE
**       volatile disk.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in/out]   PtrToVolDisk         #cpuaddr Pointer volatile area
**  \param [in/out]   SizeOfVolDisk        #uint32 Size of volatile area
**
**  \returns
**  \retcode #CFE_PSP_ERROR \endcode
**  \retcode #CFE_PSP_SUCCESS \endcode
**  \endreturns
**
*************************************************************************/
extern int32 CFE_PSP_GetVolatileDiskMem(cpuaddr *PtrToVolDisk, uint32 *SizeOfVolDisk );

/************************************************************************/
/** \brief Get kernel segment
**
**  \par Description
**       This function returns the start and end address of the kernel text segment.
**       It may not be implemented on all architectures.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in/out]   PtrToKernelSegment         #cpuaddr Pointer volatile area
**  \param [in/out]   SizeOfKernelSegment        #uint32 Size of volatile area
**
**  \returns
**  \retcode #CFE_PSP_ERROR \endcode
**  \retcode #CFE_PSP_SUCCESS \endcode
**  \endreturns
**
*************************************************************************/
extern int32 CFE_PSP_GetKernelTextSegmentInfo(cpuaddr *PtrToKernelSegment, uint32 *SizeOfKernelSegment);

/************************************************************************/
/** \brief Get cfe text segment
**
**  \par Description
**       This function returns the start and end address of the CFE text segment.
**       It may not be implemented on all architectures.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in/out]   PtrToCFESegment         #cpuaddr Pointer cfe segment
**  \param [in/out]   SizeOfCFESegment        #uint32 Size of cfe segment
**
**  \returns
**  \retcode #CFE_PSP_ERROR \endcode
**  \retcode #CFE_PSP_SUCCESS \endcode
**  \endreturns
**
*************************************************************************/
extern int32 CFE_PSP_GetCFETextSegmentInfo(cpuaddr *PtrToCFESegment, uint32 *SizeOfCFESegment);

/************************************************************************/
/** \brief Initialize watchdog
**
**  \par Description
**       To setup the timer resolution and/or other settings custom to this platform.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
*************************************************************************/
extern void CFE_PSP_WatchdogInit(void);

/************************************************************************/
/** \brief Enable watchdog
**
**  \par Description
**       CFE_PSP_WatchdogEnable enables the watchdog timer.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
*************************************************************************/
extern void CFE_PSP_WatchdogEnable(void);

/************************************************************************/
/** \brief Disable watchdog
**
**  \par Description
**       CFE_PSP_WatchdogEnable disables the watchdog timer.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
*************************************************************************/
extern void CFE_PSP_WatchdogDisable(void);

/************************************************************************/
/** \brief Service watchdog
**
**  \par Description
**       CFE_PSP_WatchdogService services the watchdog timer according to the 
**       value set in WatchDogSet.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
*************************************************************************/
extern void CFE_PSP_WatchdogService(void);

/************************************************************************/
/** \brief Service watchdog
**
**  \par Description
**       CFE_PSP_WatchdogGet gets the watchdog time in milliseconds 
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \returns
**  The current watchdog value 
**  \endreturns
**
*************************************************************************/
extern uint32 CFE_PSP_WatchdogGet(void);

/************************************************************************/
/** \brief Service watchdog
**
**  \par Description
**       CFE_PSP_WatchdogSet sets the watchdog time in milliseconds
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]   WatchdogValue        #uint32 Value
**
*************************************************************************/
extern void CFE_PSP_WatchdogSet(uint32 WatchdogValue);

/************************************************************************/
/** \brief Panic
**
**  \par Description
**       Provides a common interface to abort the cFE startup process and return
**       back to the OS.
**
**  \par Assumptions, External Events, and Notes:
**       CFE_PSP_Panic is called by the cFE Core startup code when it needs to abort the
**       cFE startup. This should not be called by applications.
**
**  \param [in]   ErrorCode            #uint32 Error code
**
*************************************************************************/
extern void CFE_PSP_Panic(int32 ErrorCode);

/************************************************************************/
/** \brief Initialize SSR
**
**  \par Description
**       Initializes the Solid State Recorder device. This can be filled in for the platform.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]   bus            #uint32 Bus
**  \param [in]   device         #uint32 Device 
**  \param [in]   DeviceName     Device Name
**
**  \returns
**  \retcode #CFE_PSP_ERROR \endcode
**  \retcode #CFE_PSP_SUCCESS \endcode
**  \endreturns
**
*************************************************************************/
extern int32 CFE_PSP_InitSSR(uint32 bus, uint32 device, char *DeviceName );

/************************************************************************/
/** \brief Initialize SSR
**
**  \par Description
**       CFE_PSP_Decompress will uncompress the source file to the file specified in the
**       destination file name. The Decompress uses the "gzip" algorithm. Files can
**       be compressed using the "gzip" program available on almost all host platforms.
**
**  \par Assumptions, External Events, and Notes:
**       Not implemented
**
**  \param [in]   srcFileName        Source file
**  \param [in]   dstFileName        Desitination file
**
**  \returns
**  \retcode #CFE_PSP_ERROR \endcode
**  \retcode #CFE_PSP_SUCCESS \endcode
**  \endreturns
**
*************************************************************************/
extern int32 CFE_PSP_Decompress( char * srcFileName, char * dstFileName );

/************************************************************************/
/** \brief Attach exceptions
**
**  \par Description
**       CFE_PSP_AttachExceptions will setup the exception environment for the chosen platform
**       On a board, this can be configured to look at a debug flag or switch in order to
**       keep the standard OS exeption handlers, rather than restarting the system
**
**  \par Assumptions, External Events, and Notes:
**       None
**
*************************************************************************/
extern void  CFE_PSP_AttachExceptions(void);

/************************************************************************/
/** \brief Set exception environment
**
**  \par Description
**       CFE_PSP_SetDefaultExceptionEnvironment defines the CPU and FPU exceptions that are enabled for each cFE Task/App
**
**  \par Assumptions, External Events, and Notes:
**       The exception environment is local to each task Therefore this must be
**       called for each task that that wants to do floating point and catch exceptions
**
*************************************************************************/
extern void CFE_PSP_SetDefaultExceptionEnvironment(void);


/*
** I/O Port API
*/

/************************************************************************/
/** \brief Read 1 byte
**
**  \par Description
**       Read 1 byte of memory.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]       PortAddress      #cpuaddr Address to be read
**  \param [in/out]   ByteValue        Value of memory location
**
**  \returns
**  \retcode #CFE_PSP_SUCCESS \endcode
**  \endreturns
**
*************************************************************************/
int32 CFE_PSP_PortRead8         (cpuaddr PortAddress, uint8 *ByteValue);

/************************************************************************/
/** \brief Write 1 byte
**
**  \par Description
**       Write 1 byte of memory.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]       PortAddress      #cpuaddr Address to be written to
**  \param [in/out]   ByteValue        The content pointed by this argument will be 
**                                     copied to the address
**
**  \returns
**  \retcode #CFE_PSP_SUCCESS \endcode
**  \endreturns
**
*************************************************************************/
int32 CFE_PSP_PortWrite8        (cpuaddr PortAddress, uint8 ByteValue);

/************************************************************************/
/** \brief Read 2 bytes
**
**  \par Description
**       Read 2 bytes of memory.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]       PortAddress      #cpuaddr Address to be read
**  \param [in/out]   ByteValue        Value of memory location
**
**  \returns
**  \retcode #CFE_PSP_SUCCESS \endcode
**  \endreturns
**
*************************************************************************/
int32 CFE_PSP_PortRead16        (cpuaddr PortAddress, uint16 *uint16Value);

/************************************************************************/
/** \brief Write 2 bytes
**
**  \par Description
**       Write 2 bytes of memory.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]       PortAddress      #cpuaddr Address to be written to
**  \param [in/out]   ByteValue        The content pointed by this argument will be 
**                                     copied to the address
**
**  \returns
**  \retcode #CFE_PSP_SUCCESS \endcode
**  \endreturns
**
*************************************************************************/
int32 CFE_PSP_PortWrite16       (cpuaddr PortAddress, uint16 uint16Value);

/************************************************************************/
/** \brief Read 4 bytes
**
**  \par Description
**       Read 4 bytes of memory.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]       PortAddress      #cpuaddr Address to be read
**  \param [in/out]   ByteValue        Value of memory location
**
**  \returns
**  \retcode #CFE_PSP_SUCCESS \endcode
**  \endreturns
**
*************************************************************************/
int32 CFE_PSP_PortRead32        (cpuaddr PortAddress, uint32 *uint32Value);

/************************************************************************/
/** \brief Write 4 bytes
**
**  \par Description
**       Write 4 bytes of memory.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]       PortAddress      #cpuaddr Address to be written to
**  \param [in/out]   ByteValue        The content pointed by this argument will be 
**                                     copied to the address
**
**  \returns
**  \retcode #CFE_PSP_SUCCESS \endcode
**  \endreturns
**
*************************************************************************/
int32 CFE_PSP_PortWrite32       (cpuaddr PortAddress, uint32 uint32Value);

/*
** Memory API
*/

/************************************************************************/
/** \brief Read 1 byte
**
**  \par Description
**       Read 1 byte of memory.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]       MemoryAddress    #cpuaddr Address to be read
**  \param [in/out]   ByteValue        Value of memory location
**
**  \returns
**  \retcode #CFE_PSP_SUCCESS \endcode
**  \endreturns
**
*************************************************************************/
int32 CFE_PSP_MemRead8          (cpuaddr MemoryAddress, uint8 *ByteValue);

/************************************************************************/
/** \brief Write 1 byte
**
**  \par Description
**       Write 1 byte of memory.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]       MemoryAddress    #cpuaddr Address to be written to
**  \param [in/out]   ByteValue        The content pointed by this argument will be 
**                                     copied to the address
**
**  \returns
**  \retcode #CFE_PSP_SUCCESS \endcode
**  \endreturns
**
*************************************************************************/
int32 CFE_PSP_MemWrite8         (cpuaddr MemoryAddress, uint8 ByteValue);

/************************************************************************/
/** \brief Read 2 bytes
**
**  \par Description
**       Read 2 bytes of memory.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]       MemoryAddress    #cpuaddr Address to be read
**  \param [in/out]   ByteValue        Value of memory location
**
**  \returns
**  \retcode #CFE_PSP_SUCCESS \endcode
**  \endreturns
**
*************************************************************************/
int32 CFE_PSP_MemRead16         (cpuaddr MemoryAddress, uint16 *uint16Value);

/************************************************************************/
/** \brief Write 2 bytes
**
**  \par Description
**       Write 2 bytes of memory.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]       MemoryAddress    #cpuaddr Address to be written to
**  \param [in/out]   ByteValue        The content pointed by this argument will be 
**                                     copied to the address
**
**  \returns
**  \retcode #CFE_PSP_SUCCESS \endcode
**  \endreturns
**
*************************************************************************/
int32 CFE_PSP_MemWrite16        (cpuaddr MemoryAddress, uint16 uint16Value);

/************************************************************************/
/** \brief Read 4 bytes
**
**  \par Description
**       Read 4 bytes of memory.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]       MemoryAddress    #cpuaddr Address to be read
**  \param [in/out]   ByteValue        Value of memory location
**
**  \returns
**  \retcode #CFE_PSP_SUCCESS \endcode
**  \endreturns
**
*************************************************************************/
int32 CFE_PSP_MemRead32         (cpuaddr MemoryAddress, uint32 *uint32Value);

/************************************************************************/
/** \brief Write 4 bytes
**
**  \par Description
**       Write 4 bytes of memory.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]       MemoryAddress    #cpuaddr Address to be written to
**  \param [in/out]   ByteValue        The content pointed by this argument will be 
**                                     copied to the address
**
**  \returns
**  \retcode #CFE_PSP_SUCCESS \endcode
**  \endreturns
**
*************************************************************************/
int32 CFE_PSP_MemWrite32        (cpuaddr MemoryAddress, uint32 uint32Value);

/************************************************************************/
/** \brief Memory copy
**
**  \par Description
**       Copies 'size' byte from memory address pointed by 'src' to memory
**       address pointed by ' dst' For now we are using the standard c library
**       call 'memcpy' but if we find we need to make it more efficient then
**       we'll implement it in assembly.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]     dest        Pointer to memory to be written to
**  \param [in]     src         Pointer to memory to be read from
**  \param [in]     n           Size of data to copy
**
**  \returns
**  \retcode #CFE_PSP_SUCCESS \endcode
**  \endreturns
**
*************************************************************************/
int32 CFE_PSP_MemCpy            (void *dest, void *src, uint32 n);

/************************************************************************/
/** \brief Memory set
**
**  \par Description
**       Copies 'size' number of byte of value 'value' to memory address pointed
**       by 'dst' .For now we are using the standard c library call 'memset'
**       but if we find we need to make it more efficient then we'll implement
**       it in assembly.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]     dest        Pointer to memory to be written to
**  \param [in]     value       Value to set memory equal to
**  \param [in]     n           Size of data to set
**
**  \returns
**  \retcode #CFE_PSP_SUCCESS \endcode
**  \endreturns
**
*************************************************************************/
int32 CFE_PSP_MemSet            (void *dest, uint8 value, uint32 n);

/************************************************************************/
/** \brief Validate memory range
**
**  \par Description
**       Validate the memory range and type using the global CFE_PSP_MemoryTable
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]     Address       A 32 bit starting address of the memory range
**  \param [in]     Size          A 32 bit size of the memory range ( Address + Size = End Address )
**  \param [in]     MemoryType    The memory type to validate, including but not limited to:
**                                CFE_PSP_MEM_RAM, CFE_PSP_MEM_EEPROM, or CFE_PSP_MEM_ANY
**                                Any defined CFE_PSP_MEM_* enumeration can be specified
**
**  \returns
**  \retcode #CFE_PSP_INVALID_MEM_ADDR -- Starting address is not valid \endcode
**  \retcode #CFE_PSP_INVALID_MEM_TYPE -- Memory type associated with the range does not match the passed in type. \endcode
**  \retcode #CFE_PSP_INVALID_MEM_RANGE -- The Memory range associated with the address is not large enough to contain
**           Address + Size. \endcode
**  \retcode #CFE_PSP_SUCCESS Memory range and type information is valid and can be used. \endcode
**  \endreturns
**
*************************************************************************/
int32  CFE_PSP_MemValidateRange (cpuaddr Address, uint32 Size, uint32 MemoryType);

/************************************************************************/
/** \brief Memory ranges
**
**  \par Description
**       Return the number of memory ranges in the CFE_PSP_MemoryTable
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \returns
**      Positive integer number of entries in the memory range table
**  \endreturns
**
*************************************************************************/
uint32 CFE_PSP_MemRanges        (void);

/************************************************************************/
/** \brief Set memory range
**
**  \par Description
**       Validate the memory range and type using the global CFE_PSP_MemoryTable
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]     RangeNum        A 32 bit integer ( starting with 0 ) specifying the MemoryTable entry.
**  \param [in]     MemoryType      The memory type to validate, including but not limited to:
**                                      CFE_PSP_MEM_RAM, CFE_PSP_MEM_EEPROM, or CFE_PSP_MEM_ANY
**                                      Any defined CFE_PSP_MEM_* enumeration can be specified
**  \param [in]     Address         A 32 bit starting address of the memory range
**  \param [in]     Size            A 32 bit size of the memory range ( Address + Size = End Address )
**  \param [in]     WordSize        The minimum addressable size of the range:
**                                      ( CFE_PSP_MEM_SIZE_BYTE, CFE_PSP_MEM_SIZE_WORD, CFE_PSP_MEM_SIZE_DWORD )
**  \param [in]     Attributes      The attributes of the Memory Range:
**                                      (CFE_PSP_MEM_ATTR_WRITE, CFE_PSP_MEM_ATTR_READ, CFE_PSP_MEM_ATTR_READWRITE)
**  \returns
**  \retcode #CFE_PSP_INVALID_MEM_ADDR -- Starting address is not valid \endcode
**  \retcode #CFE_PSP_INVALID_MEM_TYPE -- Memory type associated with the range does not match the passed in type. \endcode
**  \retcode #CFE_PSP_INVALID_MEM_RANGE -- The index into the table is invalid \endcode
**  \retcode #CFE_PSP_INVALID_MEM_WORDSIZE -- The WordSIze parameter is not one of the predefined types. \endcode
**  \retcode #OP_INVALID_MEM_SIZE -- The Memory range associated with the address is not large enough to contain
**               Address + Size. \endcode
**  \retcode #CFE_PSP_SUCCESS Memory range set successfuly. \endcode
**  \endreturns
**
*************************************************************************/
int32  CFE_PSP_MemRangeSet      (uint32 RangeNum, uint32 MemoryType, cpuaddr StartAddr,
                                 uint32 Size,     uint32 WordSize,   uint32 Attributes);

/************************************************************************/
/** \brief Get memory range
**
**  \par Description
**       Validate the memory range and type using the global CFE_PSP_MemoryTable
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]     RangeNum        A 32 bit integer ( starting with 0 ) specifying the MemoryTable entry.
**  \param [in]     *MemoryType     A pointer to the 32 bit integer where the Memory Type is stored.
**                                      Any defined CFE_PSP_MEM_* enumeration can be specified
**  \param [in]     *Address        A pointer to the 32 bit integer where the 32 bit starting address of the memory range
**                                      is stored.
**  \param [in]     *Size           A pointer to the 32 bit integer where the 32 bit size of the memory range 
**                                      is stored.
**  \param [in]     WordSize        A pointer to the 32 bit integer where the the minimum addressable size of the range:
**                                      ( CFE_PSP_MEM_SIZE_BYTE, CFE_PSP_MEM_SIZE_WORD, CFE_PSP_MEM_SIZE_DWORD )
**  \param [in]     Attributes      The attributes of the Memory Range:
**                                      (CFE_PSP_MEM_ATTR_WRITE, CFE_PSP_MEM_ATTR_READ, CFE_PSP_MEM_ATTR_READWRITE)
**  \returns
**  \retcode #CFE_PSP_INVALID_POINTER   -- Parameter error \endcode
**  \retcode #CFE_PSP_INVALID_MEM_RANGE -- The index into the table is invalid \endcode
**  \retcode #CFE_PSP_SUCCESS Memory range returned successfuly. \endcode
**  \endreturns
**
*************************************************************************/
int32  CFE_PSP_MemRangeGet      (uint32 RangeNum, uint32 *MemoryType, cpuaddr *StartAddr,
                                 uint32 *Size,    uint32 *WordSize,   uint32 *Attributes);

/************************************************************************/
/** \brief Write 1 byte
**
**  \par Description
**       Write 1 byte to eeprom memory.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]       MemoryAddress    #cpuaddr Address to be written to
**  \param [in/out]   ByteValue        The content pointed by this argument will be 
**                                     copied to the address
**
**  \returns
**  \retcode #CFE_PSP_ERROR_TIMEOUT write operation did not go through after a specific timeout. \endcode
**  \retcode #CFE_PSP_ERROR_ADD_MISALIGNED The Address is not aligned to 16 bit addressing scheme. \endcode
**  \retcode #CFE_PSP_SUCCESS if successful \endcode
**  \endreturns
**
*************************************************************************/
int32 CFE_PSP_EepromWrite8      (cpuaddr MemoryAddress, uint8 ByteValue);

/************************************************************************/
/** \brief Write 2 bytes
**
**  \par Description
**       Write 2 bytes to eeprom memory.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]       MemoryAddress    #cpuaddr Address to be written to
**  \param [in/out]   ByteValue        The content pointed by this argument will be 
**                                     copied to the address
**
**  \returns
**  \retcode #CFE_PSP_ERROR_TIMEOUT write operation did not go through after a specific timeout. \endcode
**  \retcode #CFE_PSP_ERROR_ADD_MISALIGNED The Address is not aligned to 16 bit addressing scheme. \endcode
**  \retcode #CFE_PSP_SUCCESS if successful \endcode
**  \endreturns
**
*************************************************************************/
int32 CFE_PSP_EepromWrite16     (cpuaddr MemoryAddress, uint16 uint16Value);

/************************************************************************/
/** \brief Write 4 bytes
**
**  \par Description
**       Write 4 bytes to eeprom memory.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]       MemoryAddress    #cpuaddr Address to be written to
**  \param [in/out]   ByteValue        The content pointed by this argument will be 
**                                     copied to the address
**
**  \returns
**  \retcode #CFE_PSP_ERROR_TIMEOUT write operation did not go through after a specific timeout. \endcode
**  \retcode #CFE_PSP_ERROR_ADD_MISALIGNED The Address is not aligned to 16 bit addressing scheme. \endcode
**  \retcode #CFE_PSP_SUCCESS if successful \endcode
**  \endreturns
**
*************************************************************************/
int32 CFE_PSP_EepromWrite32     (cpuaddr MemoryAddress, uint32 uint32Value);

/************************************************************************/
/** \brief Enable eeprom write
**
**  \par Description
**       Enable the eeprom for write operation
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]       Bank      #uint32 Which bank of EEPROM 
**
**  \returns
**  \retcode #CFE_PSP_SUCCESS if successful \endcode
**  \endreturns
**
*************************************************************************/
int32 CFE_PSP_EepromWriteEnable (uint32 Bank);

/************************************************************************/
/** \brief Disable eeprom
**
**  \par Description
**       Disable  the eeprom from write operation
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]       Bank      #uint32 Which bank of EEPROM 
**
**  \returns
**  \retcode #CFE_PSP_SUCCESS if successful \endcode
**  \endreturns
**
*************************************************************************/
int32 CFE_PSP_EepromWriteDisable(uint32 Bank);

/************************************************************************/
/** \brief Power up eeprom
**
**  \par Description
**       Power up the eeprom
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]       Bank      #uint32 Which bank of EEPROM 
**
**  \returns
**  \retcode #CFE_PSP_SUCCESS if successful \endcode
**  \endreturns
**
*************************************************************************/
int32 CFE_PSP_EepromPowerUp     (uint32 Bank);

/************************************************************************/
/** \brief Power down eeprom
**
**  \par Description
**       Power down the eeprom
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]       Bank      #uint32 Which bank of EEPROM 
**
**  \returns
**  \retcode #CFE_PSP_SUCCESS if successful \endcode
**  \endreturns
**
*************************************************************************/
int32 CFE_PSP_EepromPowerDown   (uint32 Bank);

#endif  /* _cfe_psp_ */
