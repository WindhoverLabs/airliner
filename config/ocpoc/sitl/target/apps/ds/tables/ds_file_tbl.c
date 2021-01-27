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
#define FILE_ALL_EVENTS                     0
/* All housekeeping packets */
#define FILE_ALL_APP_HK_PKTS                1
/* Log all flight application messages */
#define FILE_FLIGHT_APP_TLM_PKTS            2
/* Log just default flight application messages */
#define FILE_FLIGHT_DEFAULT_APP_TLM_PKTS    3
/* Log just sensor application messages */
#define FILE_FLIGHT_SENSOR_APP_TLM_PKTS     4
/* Log all flight messages at a low rate */
#define FILE_FLIGHT_MIN_APP_TLM_PKTS        5
/* CFE core applications commands */
#define FILE_CORE_APP_CMD_PKTS              6


/*
** Sample Destination File Table Data
*/
DS_DestFileTable_t DS_DestFileTable =
{
  /* .Descriptor = */ "Events",
  /* .File       = */
  {
    /* File Index 00 -- event packets only */
    {
#if (DS_MOVE_FILES == TRUE)
      /* .Movename      = */ DS_EMPTY_STRING,
#endif
      /* .Pathname      = */ "/cf/log/",
      /* .Basename      = */ "events",
      /* .Extension     = */ ".dat",

      /* .FileNameType  = */ DS_BY_COUNT,
      /* .EnableState   = */ DS_DISABLED,
      /* .MaxFileSize   = */ (1024 * 1024 * 200),         /* 200 M-bytes */
      /* .MaxFileAge    = */ (60 * 60 * 2),               /* 2 hours */
      /* .SequenceCount = */ 1000,
    },
    /* File Index 01 -- application housekeeping packets */
    {
#if (DS_MOVE_FILES == TRUE)
      /* .Movename      = */ DS_EMPTY_STRING,
#endif
      /* .Pathname      = */ "/cf/log/",
      /* .Basename      = */ "hk",
      /* .Extension     = */ ".hk",

      /* .FileNameType  = */ DS_BY_COUNT,
      /* .EnableState   = */ DS_DISABLED,
      /* .MaxFileSize   = */ (1024 * 1024 * 200),         /* 200 M-bytes */
      /* .MaxFileAge    = */ (60 * 60 * 2),             /* 2 hours */
      /* .SequenceCount = */ 2000,
    },
    /* File Index 02 -- application telemetry packets */
    {
#if (DS_MOVE_FILES == TRUE)
      /* .Movename      = */ DS_EMPTY_STRING,
#endif
      /* .Pathname      = */ "/cf/log/",
      /* .Basename      = */ "flight",
      /* .Extension     = */ ".tlm",

      /* .FileNameType  = */ DS_BY_COUNT,
      /* .EnableState   = */ DS_DISABLED,
      /* .MaxFileSize   = */ (1024 * 1024 * 200),       /* 200 M-bytes */
      /* .MaxFileAge    = */ (60 * 60 * 2),             /* 2 hours */
      /* .SequenceCount = */ 3000,
    },
    /* File Index 03 -- hardware telemetry packets */
    {
#if (DS_MOVE_FILES == TRUE)
      /* .Movename      = */ DS_EMPTY_STRING,
#endif
      /* .Pathname      = */ "/cf/log/",
      /* .Basename      = */ "default",
      /* .Extension     = */ ".tlm",

      /* .FileNameType  = */ DS_BY_COUNT,
      /* .EnableState   = */ DS_DISABLED,
      /* .MaxFileSize   = */ (1024 * 1024 * 200),       /* 200 M-bytes */
      /* .MaxFileAge    = */ (60 * 60 * 2),             /* 2 hours */
      /* .SequenceCount = */ 4000,
    },
    /* File Index 04 -- cFE housekeeping packets */
    {
#if (DS_MOVE_FILES == TRUE)
      /* .Movename      = */ DS_EMPTY_STRING,
#endif
      /* .Pathname      = */ "/cf/log/",
      /* .Basename      = */ "sensor",
      /* .Extension     = */ ".tlm",

      /* .FileNameType  = */ DS_BY_COUNT,
      /* .EnableState   = */ DS_DISABLED,
      /* .MaxFileSize   = */ (1024 * 1024 * 200),       /* 200 M-bytes */
      /* .MaxFileAge    = */ (60 * 60 * 2),             /* 2 hours */
      /* .SequenceCount = */ 5000,
    },
    /* File Index 05 -- cFE telemetry packets */
    {
#if (DS_MOVE_FILES == TRUE)
      /* .Movename      = */ DS_EMPTY_STRING,
#endif
      /* .Pathname      = */ "/cf/log/",
      /* .Basename      = */ "minimum",
      /* .Extension     = */ ".tlm",

      /* .FileNameType  = */ DS_BY_COUNT,
      /* .EnableState   = */ DS_DISABLED,
      /* .MaxFileSize   = */ (1024 * 1024 * 200),       /* 200 M-bytes */
      /* .MaxFileAge    = */ (60 * 60 * 2),             /* 2 hours */
      /* .SequenceCount = */ 6000,
    },
    /* File Index 06 */
    {
#if (DS_MOVE_FILES == TRUE)
      /* .Movename      = */ DS_EMPTY_STRING,
#endif
      /* .Pathname      = */ "/cf/log/",
      /* .Basename      = */ "commands",
      /* .Extension     = */ ".cmd",

      /* .FileNameType  = */ DS_BY_COUNT,
      /* .EnableState   = */ DS_DISABLED,
      /* .MaxFileSize   = */ (1024 * 1024 * 200),       /* 200 M-bytes */
      /* .MaxFileAge    = */ (60 * 60 * 2),             /* 2 hours */
      /* .SequenceCount = */ 7000,
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
    },
    /* File Index 13 */
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
    /* File Index 14 */
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
    /* File Index 15 */
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
  }
};

/*
** Sample Destination File Table Header
*/
CFE_TBL_FILEDEF(DS_DestFileTable, DS.FILE_TBL, DS Destination File Table,ds_file_tbl.tbl)


/************************/
/*  End of File Comment */
/************************/
