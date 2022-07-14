#include "cfe.h"
#include "cfe_tbl_filedef.h"
#include "ds_platform_cfg.h"
#include "ds_appdefs.h"
#include "ds_app.h"
#include "ds_msg.h"


/*
** Note: It is suggested that missions pre-define their file table
**       index numbers in a public header file to be included by
**       both the packet filter table source file and the destination
**       file table source file. Common definitions may also be used
**       when creating ground system database entries that require
**       file index numbers for command arguments.
*/
/* All events */
#define FILE_ALL_PPD_EVENTS                 (0)
#define FILE_ALL_CPD_EVENTS                 (1)
/* All housekeeping packets */
#define FILE_ALL_PPD_APP_HK_PKTS            (2)
#define FILE_ALL_CPD_APP_HK_PKTS            (3)
/* Log all flight application messages */
#define FILE_FLIGHT_APP_TLM_PKTS            (4)
/* Log just minimum flight application messages */
#define FILE_MINIMUM_FLIGHT_APP_TLM_PKTS    (5)
/* Log just sensor application messages */
#define FILE_FLIGHT_SENSOR_APP_TLM_PKTS     (6)
/* Log all flight messages at a low rate */
#define FILE_LOWRATE_FLIGHT_APP_TLM_PKTS    (7)


/*
** Sample Destination File Table Data
*/
DS_DestFileTable_t DS_DestFileTable =
{
  /* .Descriptor = */ "Events",
  /* .File       = */
  {
    /* File Index 00 -- PPD event packets only */
    {
#if (DS_MOVE_FILES == TRUE)
      /* .Movename      = */ DS_EMPTY_STRING,
#endif
      /* .Pathname      = */ "/cf/log/",
      /* .Basename      = */ "ppdevt",
      /* .Extension     = */ ".log",

      /* .FileNameType  = */ DS_BY_COUNT,
      /* .EnableState   = */ DS_ENABLED,
      /* .MaxFileSize   = */ (1024 * 1024 * 200),         /* 200 M-bytes */
      /* .MaxFileAge    = */ (60 * 10),                   /* 10 minutes  */
      /* .SequenceCount = */ 0,
    },
    /* File Index 01 -- CPD event packets only */
	{
#if (DS_MOVE_FILES == TRUE)
      /* .Movename      = */ DS_EMPTY_STRING,
#endif
      /* .Pathname      = */ "/cf/log/",
      /* .Basename      = */ "cpdevt",
      /* .Extension     = */ ".log",
      /* .FileNameType  = */ DS_BY_COUNT,
      /* .EnableState   = */ DS_ENABLED,
      /* .MaxFileSize   = */ (1024 * 1024 * 200),         /* 200 M-bytes */
      /* .MaxFileAge    = */ (60 * 10),                   /* 10 minutes  */
      /* .SequenceCount = */ 0,
    },
    /* File Index 02 -- PPD housekeeping packets */
    {
#if (DS_MOVE_FILES == TRUE)
      /* .Movename      = */ DS_EMPTY_STRING,
#endif
      /* .Pathname      = */ "/cf/log/",
      /* .Basename      = */ "ppdhk",
      /* .Extension     = */ ".log",

      /* .FileNameType  = */ DS_BY_COUNT,
      /* .EnableState   = */ DS_ENABLED,
      /* .MaxFileSize   = */ (1024 * 1024 * 200),         /* 200 M-bytes */
      /* .MaxFileAge    = */ (60 * 10),                   /* 10 minutes  */
      /* .SequenceCount = */ 0,
    },
    /* File Index 03 -- CPD housekeeping packets */
    {
#if (DS_MOVE_FILES == TRUE)
      /* .Movename      = */ DS_EMPTY_STRING,
#endif
      /* .Pathname      = */ "/cf/log/",
      /* .Basename      = */ "cpdhk",
      /* .Extension     = */ ".log",

      /* .FileNameType  = */ DS_BY_COUNT,
      /* .EnableState   = */ DS_ENABLED,
      /* .MaxFileSize   = */ (1024 * 1024 * 200),         /* 200 M-bytes */
      /* .MaxFileAge    = */ (60 * 10),                   /* 10 minutes  */
      /* .SequenceCount = */ 0,
    },
    /* File Index 04 -- all flight application messages */
    {
#if (DS_MOVE_FILES == TRUE)
      /* .Movename      = */ DS_EMPTY_STRING,
#endif
      /* .Pathname      = */ "/cf/log/",
      /* .Basename      = */ "allflt",
      /* .Extension     = */ ".log",

      /* .FileNameType  = */ DS_BY_COUNT,
      /* .EnableState   = */ DS_ENABLED,
      /* .MaxFileSize   = */ (1024 * 1024 * 200),         /* 200 M-bytes */
      /* .MaxFileAge    = */ (60 * 10),                   /* 10 minutes  */
      /* .SequenceCount = */ 0,
    },
    /* File Index 05 -- minimum flight application messages */
	{
#if (DS_MOVE_FILES == TRUE)
      /* .Movename      = */ DS_EMPTY_STRING,
#endif
      /* .Pathname      = */ "/cf/log/",
      /* .Basename      = */ "minflt",
      /* .Extension     = */ ".log",

      /* .FileNameType  = */ DS_BY_COUNT,
      /* .EnableState   = */ DS_DISABLED,
      /* .MaxFileSize   = */ (1024 * 1024 * 200),         /* 200 M-bytes */
      /* .MaxFileAge    = */ (60 * 10),                   /* 10 minutes  */
      /* .SequenceCount = */ 0,
    },
    /* File Index 06 -- sensor application messages */
    {
#if (DS_MOVE_FILES == TRUE)
      /* .Movename      = */ DS_EMPTY_STRING,
#endif
      /* .Pathname      = */ "/cf/log/",
      /* .Basename      = */ "sensor",
      /* .Extension     = */ ".log",

      /* .FileNameType  = */ DS_BY_COUNT,
      /* .EnableState   = */ DS_DISABLED,
      /* .MaxFileSize   = */ (1024 * 1024 * 200),         /* 200 M-bytes */
      /* .MaxFileAge    = */ (60 * 10),                   /* 10 minutes  */
      /* .SequenceCount = */ 0,
    },
    /* File Index 07 -- flight messages at a low rate */
    {
#if (DS_MOVE_FILES == TRUE)
      /* .Movename      = */ DS_EMPTY_STRING,
#endif
      /* .Pathname      = */ "/cf/log/",
      /* .Basename      = */ "lowflt",
      /* .Extension     = */ ".log",

      /* .FileNameType  = */ DS_BY_COUNT,
      /* .EnableState   = */ DS_DISABLED,
      /* .MaxFileSize   = */ (1024 * 1024 * 200),         /* 200 M-bytes */
      /* .MaxFileAge    = */ (60 * 10),                   /* 10 minutes  */
      /* .SequenceCount = */ 0,
    },
    /* File Index 07 -- flight messages at a low rate */
    {
#if (DS_MOVE_FILES == TRUE)
      /* .Movename      = */ DS_EMPTY_STRING,
#endif
	  /* .Pathname      = */ DS_EMPTY_STRING,
      /* .Basename      = */ DS_EMPTY_STRING,
      /* .Extension     = */ DS_EMPTY_STRING,

      /* .FileNameType  = */ DS_UNUSED,
      /* .EnableState   = */ DS_UNUSED,
      /* .MaxFileSize   = */ DS_UNUSED,
      /* .MaxFileAge    = */ DS_UNUSED,
      /* .SequenceCount = */ DS_UNUSED,
    },
    /* File Index 06 */
    {
#if (DS_MOVE_FILES == TRUE)
      /* .Movename      = */ DS_EMPTY_STRING,
#endif
      /* .Pathname      = */ DS_EMPTY_STRING,
      /* .Basename      = */ DS_EMPTY_STRING,
      /* .Extension     = */ DS_EMPTY_STRING,

      /* .FileNameType  = */ DS_UNUSED,
      /* .EnableState   = */ DS_UNUSED,
      /* .MaxFileSize   = */ DS_UNUSED,
      /* .MaxFileAge    = */ DS_UNUSED,
      /* .SequenceCount = */ DS_UNUSED,
    },
    /* File Index 07 */
    {
#if (DS_MOVE_FILES == TRUE)
      /* .Movename      = */ DS_EMPTY_STRING,
#endif
      /* .Pathname      = */ DS_EMPTY_STRING,
      /* .Basename      = */ DS_EMPTY_STRING,
      /* .Extension     = */ DS_EMPTY_STRING,

      /* .FileNameType  = */ DS_UNUSED,
      /* .EnableState   = */ DS_UNUSED,
      /* .MaxFileSize   = */ DS_UNUSED,
      /* .MaxFileAge    = */ DS_UNUSED,
      /* .SequenceCount = */ DS_UNUSED,
    },
    /* File Index 08 */
    {
#if (DS_MOVE_FILES == TRUE)
      /* .Movename      = */ DS_EMPTY_STRING,
#endif
      /* .Pathname      = */ DS_EMPTY_STRING,
      /* .Basename      = */ DS_EMPTY_STRING,
      /* .Extension     = */ DS_EMPTY_STRING,

      /* .FileNameType  = */ DS_UNUSED,
      /* .EnableState   = */ DS_UNUSED,
      /* .MaxFileSize   = */ DS_UNUSED,
      /* .MaxFileAge    = */ DS_UNUSED,
      /* .SequenceCount = */ DS_UNUSED,
    },
    /* File Index 09 */
    {
#if (DS_MOVE_FILES == TRUE)
      /* .Movename      = */ DS_EMPTY_STRING,
#endif
      /* .Pathname      = */ DS_EMPTY_STRING,
      /* .Basename      = */ DS_EMPTY_STRING,
      /* .Extension     = */ DS_EMPTY_STRING,

      /* .FileNameType  = */ DS_UNUSED,
      /* .EnableState   = */ DS_UNUSED,
      /* .MaxFileSize   = */ DS_UNUSED,
      /* .MaxFileAge    = */ DS_UNUSED,
      /* .SequenceCount = */ DS_UNUSED,
    },
    /* File Index 10 */
    {
#if (DS_MOVE_FILES == TRUE)
      /* .Movename      = */ DS_EMPTY_STRING,
#endif
      /* .Pathname      = */ DS_EMPTY_STRING,
      /* .Basename      = */ DS_EMPTY_STRING,
      /* .Extension     = */ DS_EMPTY_STRING,

      /* .FileNameType  = */ DS_UNUSED,
      /* .EnableState   = */ DS_UNUSED,
      /* .MaxFileSize   = */ DS_UNUSED,
      /* .MaxFileAge    = */ DS_UNUSED,
      /* .SequenceCount = */ DS_UNUSED,
    },
    /* File Index 11 */
    {
#if (DS_MOVE_FILES == TRUE)
      /* .Movename      = */ DS_EMPTY_STRING,
#endif
      /* .Pathname      = */ DS_EMPTY_STRING,
      /* .Basename      = */ DS_EMPTY_STRING,
      /* .Extension     = */ DS_EMPTY_STRING,

      /* .FileNameType  = */ DS_UNUSED,
      /* .EnableState   = */ DS_UNUSED,
      /* .MaxFileSize   = */ DS_UNUSED,
      /* .MaxFileAge    = */ DS_UNUSED,
      /* .SequenceCount = */ DS_UNUSED,
    },
    /* File Index 12 */
    {
#if (DS_MOVE_FILES == TRUE)
      /* .Movename      = */ DS_EMPTY_STRING,
#endif
      /* .Pathname      = */ DS_EMPTY_STRING,
      /* .Basename      = */ DS_EMPTY_STRING,
      /* .Extension     = */ DS_EMPTY_STRING,

      /* .FileNameType  = */ DS_UNUSED,
      /* .EnableState   = */ DS_UNUSED,
      /* .MaxFileSize   = */ DS_UNUSED,
      /* .MaxFileAge    = */ DS_UNUSED,
      /* .SequenceCount = */ DS_UNUSED,
    }
  }
};

/*
** Sample Destination File Table Header
*/
CFE_TBL_FILEDEF(DS_DestFileTable, DS.FILE_TBL, DS Destination File Table,ds_file_tbl.tbl)


/************************/
/*  End of File Comment */
/************************/
