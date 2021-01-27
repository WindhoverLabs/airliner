#include "cfe.h"
#include "cfe_tbl_filedef.h"
#include "sc_platform_cfg.h"    /* defines table buffer size */
#include "sc_msgdefs.h"         /* defines SC command code values */
#include "msg_ids.h"          /* defines SC packet msg ID's */


/*
** Execution time for each command
*/
#define CMD1_TIME       0
#define CMD2_TIME	0

/*
** Calculate checksum for each command
*/
#define CMD1_XSUM       0x0093
#define CMD2_XSUM       0x0092

/*
** Optional command data values
*/
#define CMD2_ARG_1      1
#define CMD2_ARG_2      1

/*
** Command packet segment flags and sequence counter
** - 2 bits of segment flags (0xC000 = start and end of packet)
** - 14 bits of sequence count (unused for command packets)
*/
#define PKT_FLAGS       0xC000

/*
** Length of cmd pkt data (in bytes minus one) that follows primary header (thus, 0xFFFF = 64k)
*/
#define CMD1_LENGTH     1
#define CMD2_LENGTH     5

/*
** Command code for landing gear extend.
*/
#define CMD1_EXTEND_CC  2

/*
** Command code for LC set actionpoint state.
*/
#define CMD2_LC_SET_AP_STATE_CC 3

/*
** Sample cFE Table Header
*/
static CFE_TBL_FileDef_t CFE_TBL_FileDef __attribute__((__used__)) =
{
    "RTS_Table003", "SC.RTS_TBL003", "SC Gear Extend RTS_TBL003",
    "sc_rts003.tbl", (SC_RTS_BUFF_SIZE * sizeof(uint16))
};


/*
** Sample RTS Table Data
*/
uint16 RTS_Table003[SC_RTS_BUFF_SIZE] =
{
  /*  cmd time,  <---------------------------- cmd pkt primary header ---------------------------->  <----- cmd pkt 2nd header ---->   <-- opt data ---> */
     CMD1_TIME,  CFE_MAKE_BIG16(LGC_CMD_MID), CFE_MAKE_BIG16(PKT_FLAGS), CFE_MAKE_BIG16(CMD1_LENGTH), ((CMD1_EXTEND_CC << 8) | CMD1_XSUM),
     CMD2_TIME,  CFE_MAKE_BIG16(LC_CMD_MID),  CFE_MAKE_BIG16(PKT_FLAGS), CFE_MAKE_BIG16(CMD2_LENGTH), ((CMD2_LC_SET_AP_STATE_CC << 8) | CMD2_XSUM), CMD2_ARG_1, CMD2_ARG_2, 
};

/************************/
/*  End of File Comment */
/************************/
