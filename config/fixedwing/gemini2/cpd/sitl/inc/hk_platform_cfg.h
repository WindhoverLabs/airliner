#ifndef HK_PLATFORM_CFG_H
#define HK_PLATFORM_CFG_H



/*************************************************************************
** Macro definitions
**************************************************************************/

/**
**  \hkcfg Application Pipe Depth 
**
**  \par Description:
**       Dictates the pipe depth of the hk command pipe.
**
**  \par Limits
**		 The minimum size of this paramater is 1
**       The maximum size dictated by cFE platform configuration 
**		 parameter  CFE_SB_MAX_PIPE_DEPTH
*/
#define HK_PIPE_DEPTH       40


/**
**  \hkcfg Discard Incomplete Combo Packets 
**
**  \par Description:
**       Dictates whether combo packets that have not had all data contents
**       updated since last requested will be discarded (YES = 1) or sent 
**       anyway (NO = 0).
**
**  \par Limits
**       This parameter can be set to 0 or 1 only.
*/
#define HK_DISCARD_INCOMPLETE_COMBO          0


/**
**  \hkcfg Maximum Number of HK Copy Table Entries 
**
**  \par Description:
**       Dictates the number of elements in the hk copy table.
**
**  \par Limits
**       The maximum size of this paramater is 8192
*/
#define HK_COPY_TABLE_ENTRIES          128


/**
**  \hkcfg Number of bytes in the HK Memory Pool 
**
**  \par Description:
**       The HK memory pool contains the memory needed for the output packets.
**       The output packets are dynamically allocated from this pool when the
**       HK copy table is initially processed or loaded with new data.
**
**  \par Limits
**       The Housekeeping app does not place a limit on this parameter, but there is
**       an overhead cost in the memory pool.  The value must be larger than what is
**       needed.
*/
#define HK_NUM_BYTES_IN_MEM_POOL        (6 * 1024)


/**
**  \hkcfg Name of the HK Copy Table 
**
**  \par Description:
**       This parameter defines the name of the HK Copy Table. 
**
**  \par Limits
**       The Housekeeping app does not place a limit on this parameter
*/
#define HK_COPY_TABLE_NAME      "CopyTable"


/**
**  \hkcfg Name of the HK Run-time Table 
**
**  \par Description:
**       This parameter defines the name of the HK Run-time Table. 
**
**  \par Limits
**       The Housekeeping app does not place a limit on this parameter
*/
#define HK_RUNTIME_TABLE_NAME       "RuntimeTable"


/**
**  \hkcfg HK Copy Table Filename
**
**  \par Description:
**       The value of this constant defines the filename of the HK Copy Table
**
**  \par Limits
**       The length of each string, including the NULL terminator cannot exceed 
**       the #OS_MAX_PATH_LEN value.
*/

#define HK_COPY_TABLE_FILENAME  "/cf/apps/hk_cpy_tbl.tbl"

/** \hkcfg Mission specific version number for HK application
**  
**  \par Description:
**       An application version number consists of four parts:
**       major version number, minor version number, revision
**       number and mission specific revision number. The mission
**       specific revision number is defined here and the other
**       parts are defined in "hk_version.h".
**
**  \par Limits:
**       Must be defined as a numeric value that is greater than
**       or equal to zero.
*/
#define HK_MISSION_REV            0


#endif /* HK_PLATFORM_CFG_H */

/************************/
/*  End of File Comment */
/************************/
