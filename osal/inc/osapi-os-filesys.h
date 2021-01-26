/*
** File: osapi-os-filesys.h
**
**      Copyright (c) 2004-2006, United States government as represented by the 
**      administrator of the National Aeronautics Space Administration.  
**      All rights reserved. This software was created at NASAs Goddard 
**      Space Flight Center pursuant to government contracts.
**
**      This is governed by the NASA Open Source Agreement and may be used, 
**      distributed and modified only pursuant to the terms of that agreement.
**
** Author:  Alan Cudmore Code 582
**
** Purpose: Contains functions prototype definitions and variables declarations
**          for the OS Abstraction Layer, File System module
**
** $Revision: 1.11 $ 
**
** $Date: 2013/12/16 12:57:41GMT-05:00 $
**
** $Log: osapi-os-filesys.h  $
** Revision 1.11 2013/12/16 12:57:41GMT-05:00 acudmore 
** Added macros for Volume name length and physical device name length
** Revision 1.10 2013/07/29 12:05:48GMT-05:00 acudmore 
** Added define for device and volume name length
** Revision 1.9 2013/07/25 14:31:21GMT-05:00 acudmore 
** Added prototype and datatype for OS_GetFsInfo
** Revision 1.8 2011/12/05 12:04:21GMT-05:00 acudmore 
** Added OS_rewinddir API
** Revision 1.7 2011/04/05 16:01:12EDT acudmore 
** Added OS_CloseFileByName and OS_CloseAllFiles
** Revision 1.6 2010/11/15 11:04:38EST acudmore 
** Added OS_FileOpenCheck function.
** Revision 1.5 2010/11/12 12:00:18EST acudmore 
** replaced copyright character with (c) and added open source notice where needed.
** Revision 1.4 2010/02/01 12:28:57EST acudmore 
** Added OS_fsBytesFree API
** Revision 1.3 2010/01/25 14:44:26EST acudmore 
** renamed "new" variable to avoid C++ reserved name conflict.
** Revision 1.2 2009/07/14 15:16:05EDT acudmore 
** Added OS_TranslatePath to the API
** Revision 1.1 2008/04/20 22:36:01EDT ruperera 
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/MKS-OSAL-REPOSITORY/src/os/inc/project.pj
** Revision 1.1 2007/10/16 16:14:52EDT apcudmore 
** Initial revision
** Member added to project d:/mksdata/MKS-OSAL-REPOSITORY/src/os/inc/project.pj
** Revision 1.1 2007/08/24 13:43:24EDT apcudmore 
** Initial revision
** Member added to project d:/mksdata/MKS-CFE-PROJECT/fsw/cfe-core/os/inc/project.pj
** Revision 1.17 2007/06/07 09:59:14EDT njyanchik 
** I replaced the second OS_cp definition with OS_mv
** Revision 1.16 2007/06/05 16:25:33EDT apcudmore 
** Increased Number of volume table entries from 10 to 14.
** Added 2 extra EEPROM disk mappings to RAD750 Volume table + 2 spares
** Added 4 spares to every other volume table.
** Revision 1.15 2007/05/25 09:17:56EDT njyanchik 
** I added the rmfs call to the OSAL and updated the unit test stubs to match
** Revision 1.14 2007/03/21 10:15:29EST njyanchik 
** I mistakenly put the wrong length in for the path in the OS_FDTableEntry structure, and I added 
** some code that will set and out of range file descriptors .IsValid flag to false in OS_FDGetInfo
** Revision 1.13 2007/03/06 11:52:46EST njyanchik 
** This change goes with the previous CP, I forgot to include it
** Revision 1.12 2007/02/28 14:57:45EST njyanchik 
** The updates for supporting copying and moving files are now supported
** Revision 1.11 2007/02/27 15:22:11EST njyanchik 
** This CP has the initial import of the new file descripor table mechanism
** Revision 1.10 2006/12/20 10:27:09EST njyanchik 
** This change package incorporates all the changes necessary for the addition
** of a new API to get the real physical drive undernieth a mount point
** Revision 1.9 2006/11/14 14:44:28GMT-05:00 njyanchik 
** Checks were added to the OS fs calls that look at the return of a function that
** changes the name of paths from abstracted to local path names.
** Revision 1.8 2006/10/30 16:12:19GMT-05:00 apcudmore 
** Updated Compact flash and RAM device names for vxWorks 6.2 changes. 
** Revision 1.7 2006/10/25 11:31:18EDT njyanchik 
** This CP incorporates changes to every bsp_voltab.c file. I increased the number
** entries in the volume table to 10. I also changed the #define in the os_filesys.h
** file for the number of entries to match.
** 
** This update also includes adding the prototype for OS_initfs in os_filesys.h
** Revision 1.6 2006/09/26 09:03:46GMT-05:00 njyanchik 
** Contains the initial import of the ES Shell commands interface
** Revision 1.5 2006/07/25 15:37:52EDT njyanchik 
** It turns out the both the FS app and the OSAL were incorrect where file descriptors are
** concerned. the file descriptors should be int32 across the board.
** Revision 1.4 2006/01/20 11:56:18EST njyanchik 
** Fixed header file information to match api document
** Revision 1.26  2005/07/12 17:13:56  nyanchik
** Moved the Volume table to a bsp table in the arch directories.
**
** Revision 1.2 2005/07/11 16:26:57EDT apcudmore 
** OSAPI 2.0 integration
** Revision 1.25  2005/07/06 16:11:17  nyanchik
** *** empty log message ***
**
** Revision 1.24  2005/07/05 18:34:55  nyanchik
** fixed issues found in code walkthrogh. Also removed the OS_Info* functions that are going in the BSP
**
** Revision 1.23  2005/06/17 19:46:34  nyanchik
** added new file system style to linux and rtems.
**
** Revision 1.22  2005/06/15 16:43:48  nyanchik
** added extra parenthesis for the .h file # defines
**
** Revision 1.21  2005/06/06 14:17:42  nyanchik
** added headers to osapi-os-core.h and osapi-os-filesys.h
**
** Revision 1.20  2005/06/02 18:04:24  nyanchik
** *** empty log message ***
**
** Revision 1.1  2005/03/15 18:26:32  nyanchik
** *** empty log message ***
**
**
** Date Written:
**
**    
*/

#ifndef _osapi_filesys_
#define _osapi_filesys_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/stat.h>
#include "osconfig.h"

#define OS_READ_ONLY        0
#define OS_WRITE_ONLY       1
#define OS_READ_WRITE       2

#define OS_SEEK_SET         0
#define OS_SEEK_CUR         1
#define OS_SEEK_END         2

#define OS_CHK_ONLY         0
#define OS_REPAIR           1

#define FS_BASED            0
#define RAM_DISK            1
#define EEPROM_DISK         2
#define ATA_DISK            3


/*
** Number of entries in the internal volume table
*/
#define NUM_TABLE_ENTRIES 14

/*
** Length of a Device and Volume name 
*/
#define OS_FS_DEV_NAME_LEN  32
#define OS_FS_PHYS_NAME_LEN 64
#define OS_FS_VOL_NAME_LEN  32


/*
** Defines for File System Calls
*/
/*
 * NOTE - these values used to overlap with the
 * other OSAPI error codes.  They now start at -100
 * to avoid this overlap.
 */
#define OS_FS_ERR_PATH_TOO_LONG        (-103)
#define OS_FS_ERR_NAME_TOO_LONG        (-104)
#define OS_FS_ERR_DRIVE_NOT_CREATED    (-106)
#define OS_FS_ERR_DEVICE_NOT_FREE      (-107)
#define OS_FS_ERR_PATH_INVALID         (-108)


/* 
 * Map some codes used by the file API back to the generic counterparts 
 * where there is overlap between them.  Do not duplicate error codes.
 */
#define OS_FS_SUCCESS                  OS_SUCCESS
#define OS_FS_ERROR                    OS_ERROR
#define OS_FS_ERR_INVALID_POINTER      OS_INVALID_POINTER
#define OS_FS_ERR_NO_FREE_FDS          OS_ERR_NO_FREE_IDS
#define OS_FS_ERR_INVALID_FD           OS_ERR_INVALID_ID
#define OS_FS_UNIMPLEMENTED            OS_ERR_NOT_IMPLEMENTED


/* This typedef is for the OS_FS_GetErrorName function, to ensure
 * everyone is making an array of the same length
 *
 * Implementation note for developers:
 *
 * os_fs_err_name_t is now equivalent to the OSAL "os_err_name_t" typedef, 
 * to preserve source code compatibility with anything using the OS_FS_GetErrorName api
 *
 * The sizes of strings in OSAL functions are built with os_fs_err_name_t's
 * limits in mind.  Always check the uses of os_fs_err_name_t when changing
 * os_err_name_t.
 */
typedef os_err_name_t os_fs_err_name_t;


/*
** Internal structure of the OS volume table for 
** mounted file systems and path translation
*/
typedef struct
{
    char   DeviceName [OS_FS_DEV_NAME_LEN];
    char   PhysDevName [OS_FS_PHYS_NAME_LEN];
    uint32 VolumeType;
    uint8  VolatileFlag;
    uint8  FreeFlag;
    uint8  IsMounted;
    char   VolumeName [OS_FS_VOL_NAME_LEN];
    char   MountPoint [OS_MAX_PATH_LEN];
    uint32 BlockSize;

}OS_VolumeInfo_t;

typedef struct
{
    int32   OSfd;                   /* The underlying OS's file descriptor */
    char    Path[OS_MAX_PATH_LEN];  /* The path of the file opened */
    uint32   User;                  /* The task id of the task who opened the file*/
    uint8   IsValid;                /* Whether or not this entry is valid */
}OS_FDTableEntry;

typedef struct
{
   uint32   MaxFds;                /* Total number of file descriptors */
   uint32   FreeFds;               /* Total number that are free */
   uint32   MaxVolumes;            /* Maximum number of volumes */
   uint32   FreeVolumes;           /* Total number of volumes free */
} os_fsinfo_t; 

/* modified to posix calls, since all of the 
 * applicable OSes use the posix calls */

typedef struct stat         os_fstat_t;


typedef enum
{
    OS_DT_FILE = 0,
    OS_DT_DIR  = 1
} os_dt_t;

typedef struct
{
    char    d_name[1000];
    os_dt_t d_type;
} os_dirent_t;

typedef struct
{
    void*       OSfd;              /* The underlying OS's file descriptor */
    uint8       IsValid;           /* Whether or not this entry is valid */
    os_dirent_t DirEnt;
} os_dir_t;

typedef os_dir_t* os_dirp_t;

/* still don't know what this should be*/
typedef unsigned long int   os_fshealth_t; 

/*
 * Exported Functions
*/


/******************************************************************************
** Standard File system API
******************************************************************************/
/*
 * Initializes the File System functions 
*/
/************************************************************************/
/** \brief Initialization of API
**
**  \par Description
**       Initialize the tables that the OS FS API uses to keep track of information
**       about objects
**
**  \par Assumptions, External Events, and Notes:
**       None
**  
**  \returns
**  \retcode #OS_SUCCESS  \retdesc \copydoc OS_SUCCESS \endcode
**  \retcode #OS_ERROR  \retdesc \copydoc OS_ERROR \endcode
**  \endreturns
**
*************************************************************************/
int32           OS_FS_Init(void);

/************************************************************************/
/** \brief Create file
**
**  \par Description
**       Creates a file specified by const char *path, with read/write 
**       permissions by access. The file is also automatically opened by the
**       create call.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]   path         File path
**  \param [in]   access       #OS_WRITE_ONLY or #OS_READ_WRITE
**  
**  \returns
**  \retcode #OS_FS_ERR_INVALID_POINTER if path is NULL \endcode
**  \retcode #OS_FS_ERR_PATH_TOO_LONG if path exceeds the maximum number of chars \endcode
**  \retcode #OS_FS_ERR_PATH_INVALID if path cannot be parsed \endcode
**  \retcode #OS_FS_ERR_NAME_TOO_LONG if the name of the file is too long \endcode
**  \retcode #OS_FS_ERROR if permissions are unknown or OS call fails \endcode
**  \retcode #OS_FS_ERR_NO_FREE_FDS if there are no free file descripors left \endcode
**  \retcode #OS_FS_SUCCESS if success \endcode
**  \endreturns
**
*************************************************************************/
int32           OS_creat  (const char *path, int32  access);

/************************************************************************/
/** \brief Open file
**
**  \par Description
**       Opens a file. access parameters are OS_READ_ONLY,OS_WRITE_ONLY, or 
**       OS_READ_WRITE
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]   path         File path
**  \param [in]   access       #OS_READ_ONLY, #OS_WRITE_ONLY, or #OS_READ_WRITE
**  \param [in]   mode         #uint32 bitmask for permissions
**  
**  \returns
**  \retcode #OS_FS_ERR_INVALID_POINTER if path is NULL \endcode
**  \retcode #OS_FS_ERR_PATH_TOO_LONG if path exceeds the maximum number of chars \endcode
**  \retcode #OS_FS_ERR_PATH_INVALID if path cannot be parsed \endcode
**  \retcode #OS_FS_ERR_NAME_TOO_LONG if the name of the file is too long \endcode
**  \retcode #OS_FS_ERROR if permissions are unknown or OS call fails \endcode
**  \retcode #OS_FS_ERR_NO_FREE_FDS if there are no free file descripors left \endcode
**  \retcode #OS_FS_SUCCESS if success \endcode
**  File descriptor if success
**  \endreturns
**
*************************************************************************/
int32           OS_open   (const char *path,  int32 access,  uint32 mode);

/************************************************************************/
/** \brief Close file
**
**  \par Description
**       Closes a file. 
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]   filedes         #int32 File descriptor
**  
**  \returns
**  \retcode #OS_FS_ERR_INVALID_FD if the file descriptor passed in is invalid \endcode
**  \retcode #OS_FS_ERROR  if file  descriptor could not be closed \endcode
**  \retcode #OS_FS_SUCCESS if success \endcode
**  File descriptor if success
**  \endreturns
**
*************************************************************************/
int32           OS_close  (int32  filedes);

/************************************************************************/
/** \brief Read file
**
**  \par Description
**       Reads up to nbytes from a file, and puts them into buffer. 
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]       filedes        #int32 File descriptor
**  \param [in/out]   buffer         Pointer to buffer to populate
**  \param [in]       nbytes         #uint32 Bytes to read
**  
**  \returns
**  \retcode #OS_FS_ERR_INVALID_POINTER if buffer is a null pointer \endcode
**  \retcode #OS_FS_ERR_INVALID_FD if the file descriptor passed in is invalid \endcode
**  \retcode #OS_FS_ERROR  if file  descriptor could not be closed \endcode
**  Number of bytes read if success
**  \endreturns
**
*************************************************************************/
int32           OS_read   (int32  filedes, void *buffer, uint32 nbytes);

/************************************************************************/
/** \brief Write to file
**
**  \par Description
**       Writes to a file. Copies up to a maximum of nbtyes of buffer to the file
**       described in filedes
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]       filedes        #int32 File descriptor
**  \param [in/out]   buffer         Pointer to buffer to write from
**  \param [in]       nbytes         #uint32 Bytes to write
**  
**  \returns
**  \retcode #OS_FS_ERR_INVALID_POINTER if buffer is a null pointer \endcode
**  \retcode #OS_FS_ERR_INVALID_FD if the file descriptor passed in is invalid \endcode
**  \retcode #OS_FS_ERROR  if file  descriptor could not be closed \endcode
**  Number of bytes read if success
**  \endreturns
**
*************************************************************************/
int32           OS_write  (int32  filedes, const void *buffer, uint32 nbytes);

/************************************************************************/
/** \brief Chmod file
**
**  \par Description
**       Writes to a file. Copies up to a maximum of nbtyes of buffer to the file
**       described in filedes
**
**  \par Assumptions, External Events, and Notes:
**       Not implemented
**
**  \param [in]   path         File path
**  \param [in]   access       #OS_READ_ONLY, #OS_WRITE_ONLY, or #OS_READ_WRITE
**  
*************************************************************************/
int32           OS_chmod  (const char *path, uint32 access);

/************************************************************************/
/** \brief Open file
**
**  \par Description
**       Returns information about a file or directory in a os_fs_stat structure
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]       path         File path
**  \param [in/out]   filestats    #os_fstat_t Stats struct
**  
**  \returns
**  \retcode #OS_FS_ERR_INVALID_POINTER if path is NULL \endcode
**  \retcode #OS_FS_ERR_PATH_TOO_LONG if path exceeds the maximum number of chars \endcode
**  \retcode #OS_FS_ERR_PATH_INVALID if path cannot be parsed \endcode
**  \retcode #OS_FS_ERR_NAME_TOO_LONG if the name of the file is too long \endcode
**  \retcode #OS_FS_ERROR if permissions are unknown or OS call fails \endcode
**  \retcode #OS_FS_SUCCESS if success \endcode
**  File descriptor if success
**  \endreturns
**
*************************************************************************/
int32           OS_stat   (const char *path, os_fstat_t  *filestats);

/************************************************************************/
/** \brief File offset
**
**  \par Description
**       Sets the read/write pointer to a specific offset in a specific file. 
**       Whence is either OS_SEEK_SET, OS_SEEK_CUR, or OS_SEEK_END
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]       filedes        #int32 File descriptor
**  \param [in]       offset         #int32 Offset amount
**  \param [in]       whence         #uint32 Offset type
**  
**  \returns
**  \retcode #OS_FS_ERR_INVALID_FD if the file descriptor passed in is invalid \endcode
**  the new offset from the beginning of the file
**  \endreturns
**
*************************************************************************/
int32           OS_lseek  (int32  filedes, int32 offset, uint32 whence);

/************************************************************************/
/** \brief Remove file
**
**  \par Description
**       Removes a given filename from the drive
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]       path         File path
**  
**  \returns
**  \retcode #OS_FS_ERR_INVALID_POINTER if path is NULL \endcode
**  \retcode #OS_FS_ERR_PATH_TOO_LONG if path exceeds the maximum number of chars \endcode
**  \retcode #OS_FS_ERR_PATH_INVALID if path cannot be parsed \endcode
**  \retcode #OS_FS_ERR_NAME_TOO_LONG if the name of the file is too long \endcode
**  \retcode #OS_FS_ERROR if permissions are unknown or OS call fails \endcode
**  \retcode #OS_FS_SUCCESS if success \endcode
**  \endreturns
**
*************************************************************************/
int32           OS_remove (const char *path);

/************************************************************************/
/** \brief Rename file
**
**  \par Description
**       Renames a file
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]       path         File path
**  
**  \returns
**  \retcode #OS_FS_ERR_INVALID_POINTER if path is NULL \endcode
**  \retcode #OS_FS_ERR_PATH_TOO_LONG if path exceeds the maximum number of chars \endcode
**  \retcode #OS_FS_ERR_PATH_INVALID if path cannot be parsed \endcode
**  \retcode #OS_FS_ERR_NAME_TOO_LONG if the name of the file is too long \endcode
**  \retcode #OS_FS_ERROR if permissions are unknown or OS call fails \endcode
**  \retcode #OS_FS_SUCCESS if success \endcode
**  \endreturns
**
*************************************************************************/
int32           OS_rename (const char *old_filename, const char *new_filename);

/************************************************************************/
/** \brief Copy file
**
**  \par Description
**       Copies a single file from src to dest
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]       src         Source file path
**  \param [in]       dest        Destination file path
**  
**  \returns
**  \retcode #OS_FS_ERR_INVALID_POINTER if path is NULL \endcode
**  \retcode #OS_FS_ERR_PATH_TOO_LONG if path exceeds the maximum number of chars \endcode
**  \retcode #OS_FS_ERR_PATH_INVALID if path cannot be parsed \endcode
**  \retcode #OS_FS_ERR_NAME_TOO_LONG if the name of the file is too long \endcode
**  \retcode #OS_FS_ERROR if permissions are unknown or OS call fails \endcode
**  \retcode #OS_FS_SUCCESS if success \endcode
**  \endreturns
**
*************************************************************************/
int32 OS_cp (const char *src, const char *dest);

/************************************************************************/
/** \brief Move file
**
**  \par Description
**       Moves a single file from src to dest
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]       src         Source file path
**  \param [in]       dest        Destination file path
**  
**  \returns
**  \retcode #OS_FS_ERR_INVALID_POINTER if path is NULL \endcode
**  \retcode #OS_FS_ERR_PATH_TOO_LONG if path exceeds the maximum number of chars \endcode
**  \retcode #OS_FS_ERR_PATH_INVALID if path cannot be parsed \endcode
**  \retcode #OS_FS_ERR_NAME_TOO_LONG if the name of the file is too long \endcode
**  \retcode #OS_FS_ERROR if permissions are unknown or OS call fails \endcode
**  \retcode #OS_FS_SUCCESS if success \endcode
**  \endreturns
**
*************************************************************************/
int32 OS_mv (const char *src, const char *dest);

/************************************************************************/
/** \brief Get file info
**
**  \par Description
**       Copies the information of the given file descriptor into a structure passed in
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]       filedes        #int32 File descriptor
**  \param [in]       fd_prop        #OS_FDTableEntry Pointer to file property struct
**  
**  \returns
**  \retcode #OS_FS_ERR_INVALID_FD if the file descriptor passed in is invalid \endcode
**  \retcode #OS_FS_SUCCESS if success \endcode
**  \endreturns
**
*************************************************************************/
int32 OS_FDGetInfo (int32 filedes, OS_FDTableEntry *fd_prop);

/************************************************************************/
/** \brief File open check
**
**  \par Description
**       Checks to see if a file is open 
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]       Filename         File path
**  
**  \returns
**  \retcode #OS_FS_ERROR if the file is not open  \endcode
**  \retcode #OS_FS_SUCCESS if the file is open  \endcode
**  \endreturns
**
*************************************************************************/
int32 OS_FileOpenCheck(char *Filename);

/************************************************************************/
/** \brief Close all files
**
**  \par Description
**       Closes All open files that were opened through the OSAL 
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \returns
**  \retcode #OS_FS_ERROR if one or more file close returned an error  \endcode
**  \retcode #OS_FS_SUCCESS if the files were all closed without error  \endcode
**  \endreturns
**
*************************************************************************/
int32 OS_CloseAllFiles(void);

/************************************************************************/
/** \brief Close file by name
**
**  \par Description
**       Allows a file to be closed by name.
**
**  \par Assumptions, External Events, and Notes:
**       This will only work if the name passed in is the same name used to open 
**       the file.
**
**  \param [in]       src         Source file path
**  \param [in]       dest        Destination file path
**  
**  \returns
**  \retcode #OS_FS_ERR_PATH_INVALID if path cannot be parsed \endcode
**  \retcode #OS_FS_ERROR if the file close returned an error \endcode
**  \retcode #OS_FS_SUCCESS if success \endcode
**  \endreturns
**
*************************************************************************/
int32 OS_CloseFileByName(char *Filename);


/******************************************************************************
** Directory API 
******************************************************************************/

/************************************************************************/
/** \brief Make directory
**
**  \par Description
**       Makes a directory specified by path.
**
**  \par Assumptions, External Events, and Notes:
**       The access parameter is currently unused.
**
**  \param [in]   path         File path
**  \param [in]   access       #OS_READ_ONLY, #OS_WRITE_ONLY, or #OS_READ_WRITE
**  
**  \returns
**  \retcode #OS_FS_ERR_INVALID_POINTER if path is NULL \endcode
**  \retcode #OS_FS_ERR_PATH_TOO_LONG if path exceeds the maximum number of chars \endcode
**  \retcode #OS_FS_ERR_PATH_INVALID if path cannot be parsed \endcode
**  \retcode #OS_FS_ERROR if permissions are unknown or OS call fails \endcode
**  \retcode #OS_FS_SUCCESS if success \endcode
**  \endreturns
**
*************************************************************************/
int32           OS_mkdir   (const char *path, uint32 access);

/************************************************************************/
/** \brief Open directory
**
**  \par Description
**       Opens a directory for searching
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]   path         File path
**  
**  \returns
**  NULL if there is an error  
**  Pointer to a directory if success
**  \endreturns
**
*************************************************************************/
os_dirp_t       OS_opendir (const char *path);

/************************************************************************/
/** \brief Close directory
**
**  \par Description
**       Closes a directory
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]   path         File path
**  \param [in]   access       #OS_READ_ONLY, #OS_WRITE_ONLY, or #OS_READ_WRITE
**  
**  \returns
**  \retcode #OS_FS_ERROR if permissions are unknown or OS call fails \endcode
**  \retcode #OS_FS_SUCCESS if success \endcode
**  \endreturns
**
*************************************************************************/
int32           OS_closedir(os_dirp_t directory);

/************************************************************************/
/** \brief Rewind directory
**
**  \par Description
**       Rewinds the directory pointer
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]   directory         #os_dirp_t 
**  
*************************************************************************/
void           OS_rewinddir(os_dirp_t directory);

/************************************************************************/
/** \brief Get next directory entry
**
**  \par Description
**       Obtains directory entry data for the next file from an open directory
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]   directory         #os_dirp_t 
**  
**  \returns
**  a pointer to the next entry for success
**  NULL if error or end of directory is reached
**  \endreturns
**
*************************************************************************/
os_dirent_t *   OS_readdir (os_dirp_t directory);

/************************************************************************/
/** \brief Remove directory
**
**  \par Description
**       Removes a directory from  the structure (must be an empty directory)
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]   path         File path
**  
**  \returns
**  \retcode #OS_FS_ERR_INVALID_POINTER if path is NULL \endcode
**  \retcode #OS_FS_ERR_PATH_TOO_LONG if path exceeds the maximum number of chars \endcode
**  \retcode #OS_FS_ERR_PATH_INVALID if path cannot be parsed \endcode
**  \retcode #OS_FS_ERROR if permissions are unknown or OS call fails \endcode
**  \retcode #OS_FS_SUCCESS if success \endcode
**  \endreturns
**
*************************************************************************/
int32           OS_rmdir   (const char *path);

/******************************************************************************
** System Level API 
******************************************************************************/

/************************************************************************/
/** \brief Make file system
**
**  \par Description
**       Makes a file system on the target
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]   address         TODO: Not used
**  \param [in]   devname         Name of device
**  \param [in]   volname         Name of volume
**  \param [in]   blocksize       Block size for file system
**  \param [in]   numblocks       Number of blocks
**  
**  \returns
**  \retcode #OS_FS_ERR_INVALID_POINTER if path is NULL \endcode
**  \retcode #OS_FS_ERR_DRIVE_NOT_CREATED if the OS calls to create the the drive failed \endcode
**  \retcode #OS_FS_ERR_DEVICE_NOT_FREE if the volume table is full \endcode
**  \retcode #OS_FS_SUCCESS if success \endcode
**  \endreturns
**
*************************************************************************/
int32           OS_mkfs        (char *address,const char *devname, const char *volname,
                                uint32 blocksize, uint32 numblocks);

/************************************************************************/
/** \brief Mount file system
**
**  \par Description
**       Mounts a drive.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]   devname         Name of device
**  \param [in]   mountpoint      Mount point for the file system
**  
**  \returns
**  \retcode #OS_FS_ERR_INVALID_POINTER if path is NULL \endcode
**  \retcode #OS_FS_ERR_PATH_TOO_LONG if path exceeds the maximum number of chars \endcode
**  \retcode #OS_FS_ERROR if permissions are unknown or OS call fails \endcode
**  \retcode #OS_FS_SUCCESS if success \endcode
**  \endreturns
**
*************************************************************************/
int32           OS_mount       (const char *devname, const char *mountpoint);

/************************************************************************/
/** \brief Initialize file system
**
**  \par Description
**       Inititalizes a file system on the target
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]   address         TODO: Not used
**  \param [in]   devname         Name of device
**  \param [in]   volname         Name of volume
**  \param [in]   blocksize       Block size for file system
**  \param [in]   numblocks       Number of blocks
**  
**  \returns
**  \retcode #OS_FS_ERR_INVALID_POINTER if path is NULL \endcode
**  \retcode #OS_FS_ERR_PATH_TOO_LONG if path exceeds the maximum number of chars \endcode
**  \retcode #OS_FS_ERR_DEVICE_NOT_FREE if the volume table is full \endcode
**  \retcode #OS_FS_ERR_DRIVE_NOT_CREATED on error \endcode
**  \retcode #OS_FS_SUCCESS if success \endcode
**  \endreturns
**
*************************************************************************/
int32           OS_initfs      (char *address,const char *devname, const char *volname,
                                uint32 blocksize, uint32 numblocks);

/************************************************************************/
/** \brief Initialize file system
**
**  \par Description
**       Inititalizes a file system on the target
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]   devname         Name of device
**  
**  \returns
**  \retcode #OS_FS_ERR_INVALID_POINTER if path is NULL \endcode
**  \retcode #OS_FS_ERROR is the drive specified cannot be located \endcode
**  \retcode #OS_FS_SUCCESS if success \endcode
**  \endreturns
**
*************************************************************************/
int32           OS_rmfs        (const char *devname);

/************************************************************************/
/** \brief Unmount file system
**
**  \par Description
**       Unmounts a drive.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]   mountpoint      Mount point for the file system
**  
**  \returns
**  \retcode #OS_FS_ERR_INVALID_POINTER if path is NULL \endcode
**  \retcode #OS_FS_ERR_PATH_TOO_LONG if path exceeds the maximum number of chars \endcode
**  \retcode #OS_FS_ERROR if permissions are unknown or OS call fails \endcode
**  \retcode #OS_FS_SUCCESS if success \endcode
**  \endreturns
**
*************************************************************************/
int32           OS_unmount     (const char *mountpoint);

/************************************************************************/
/** \brief Get blocks free
**
**  \par Description
**       Returns the number of free blocks in a volume
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]   name         Name of volume
**  
**  \returns
**  \retcode #OS_FS_ERR_INVALID_POINTER if path is NULL \endcode
**  \retcode #OS_FS_ERR_PATH_TOO_LONG if path exceeds the maximum number of chars \endcode
**  \retcode #OS_FS_ERROR if permissions are unknown or OS call fails \endcode
**  The number of blocks free in a volume if success
**  \endreturns
**
*************************************************************************/
int32           OS_fsBlocksFree (const char *name);

/************************************************************************/
/** \brief Get bytes free
**
**  \par Description
**       Returns the number of free bytes in a volume
**
**  \par Assumptions, External Events, and Notes:
**       The 64 bit data type to support filesystems that
**       are greater than 4 Gigabytes
**
**  \param [in]   name         Name of volume
**  
**  \returns
**  \retcode #OS_FS_ERR_INVALID_POINTER if path is NULL \endcode
**  \retcode #OS_FS_ERR_PATH_TOO_LONG if path exceeds the maximum number of chars \endcode
**  \retcode #OS_FS_ERROR if permissions are unknown or OS call fails \endcode
**  The number of blocks free in a volume if success
**  \endreturns
**
*************************************************************************/
int32 OS_fsBytesFree (const char *name, uint64 *bytes_free);

/************************************************************************/
/** \brief Check filesystem
**
**  \par Description
**       Checks the drives for inconsisenties and either repairs it or not
**
**  \par Assumptions, External Events, and Notes:
**       Not implemented
**
**  \param [in]   name         Name of volume
**  \param [in]   repair       #boolean Repair flag
**  
**  \returns
**  \retcode #OS_FS_ERR_INVALID_POINTER if path is NULL \endcode
**  \retcode #OS_FS_ERROR if permissions are unknown or OS call fails \endcode
**  \retcode #OS_FS_SUCCESS if success \endcode
**  \endreturns
**
*************************************************************************/
os_fshealth_t   OS_chkfs       (const char *name, boolean repair);

/************************************************************************/
/** \brief Get drive name
**
**  \par Description
**       Returns the name of the physical volume associated with the drive,
**       when given the OSAL mount point of the drive
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in/out]   PhysDriveName         Name of volume
**  \param [in]       MountPoint            Mount point for the file system
**  
**  \returns
**  \retcode #OS_FS_ERR_INVALID_POINTER if path is NULL \endcode
**  \retcode #OS_FS_ERROR if permissions are unknown or OS call fails \endcode
**  \retcode #OS_FS_SUCCESS if success \endcode
**  \endreturns
**
*************************************************************************/
int32       OS_FS_GetPhysDriveName  (char * PhysDriveName, char * MountPoint);

/************************************************************************/
/** \brief Translate path
**
**  \par Description
**       Because of the abstraction of the filesystem across OSes, we have to change
**       the name of the {file, directory, drive} to be what the OS can actually 
**       accept
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]           VirtualPath         Path to convert
**  \param [in/out]       LocalPath           Converted path
**  
**  \returns
**  \retcode #OS_FS_ERR_INVALID_POINTER if path is NULL \endcode
**  \retcode #OS_FS_ERR_PATH_TOO_LONG if path exceeds the maximum number of chars \endcode
**  \retcode #OS_FS_ERR_PATH_INVALID if path cannot be parsed \endcode
**  \retcode #OS_FS_SUCCESS if success \endcode
**  \endreturns
**
*************************************************************************/
int32       OS_TranslatePath ( const char *VirtualPath, char *LocalPath);


/************************************************************************/
/** \brief Get file system info
**
**  \par Description
**       Returns information about the file system in an os_fsinfo_t
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in/out]       filesys_info        #os_fsinfo_t Pointer to info struct
**  
**  \returns
**  \retcode #OS_FS_ERR_INVALID_POINTER if path is NULL \endcode
**  \retcode #OS_FS_SUCCESS if success \endcode
**  \endreturns
**
*************************************************************************/
int32       OS_GetFsInfo(os_fsinfo_t  *filesys_info);

/******************************************************************************
** Shell API
******************************************************************************/

/* executes the shell command passed into is and writes the output of that 
 * command to the file specified by the given OSAPI file descriptor */
/* --------------------------------------------------------------------------------------
    Name: OS_ShellOutputToFile
    
    Purpose: Takes a shell command in and writes the output of that command to the specified file
    
    Returns: OS_FS_ERROR if the command was not executed properly
             OS_FS_ERR_INVALID_FD if the file descriptor passed in is invalid
             OS_SUCCESS if success
 ---------------------------------------------------------------------------------------*/

int32 OS_ShellOutputToFile(const char* Cmd, int32 OS_fd);


#ifdef __cplusplus
}
#endif


#endif
