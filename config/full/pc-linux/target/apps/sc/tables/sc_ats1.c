#include "cfe.h"
#include "cfe_tbl_filedef.h"

#include "sc_platform_cfg.h"    /* defines table buffer size */
#include "sc_msgdefs.h"         /* defines SC command code values */
#include "sc_msgids.h"          /* defines SC packet msg ID's */


/*
** Arbitrary spacecraft time for start of sample ATS
*/
#define TEST_TIME     1000000


/*
** Execution time for each sample command
*/
#define CMD1_TIME     (TEST_TIME + 30)
#define CMD2_TIME     (TEST_TIME + 35)
#define CMD3_TIME     (TEST_TIME + 40)
#define CMD4_TIME     (TEST_TIME + 100)


/*
** Create execution time as two 16 bit values
*/
#define CMD1_TIME_A   ((uint16) ((uint32) CMD1_TIME >> 16))
#define CMD2_TIME_A   ((uint16) ((uint32) CMD2_TIME >> 16))
#define CMD3_TIME_A   ((uint16) ((uint32) CMD3_TIME >> 16))
#define CMD4_TIME_A   ((uint16) ((uint32) CMD4_TIME >> 16))

#define CMD1_TIME_B   ((uint16) ((uint32) CMD1_TIME))
#define CMD2_TIME_B   ((uint16) ((uint32) CMD2_TIME))
#define CMD3_TIME_B   ((uint16) ((uint32) CMD3_TIME))
#define CMD4_TIME_B   ((uint16) ((uint32) CMD4_TIME))


/*
** Calculate checksum for each sample command
*/
#define CMD1_XSUM     0x008F
#define CMD2_XSUM     0x008B
#define CMD3_XSUM     0x0088
#define CMD4_XSUM     0x008E


/*
** Optional command data values
*/
#define CMD2_ARG      1
#define CMD3_ARG      1


/*
** Command packet segment flags and sequence counter
** - 2 bits of segment flags (0xC000 = start and end of packet)
** - 14 bits of sequence count (unused for command packets)
*/
#define PKT_FLAGS     0xC000


/*
** Length of cmd pkt data (in bytes minus one) that follows primary header (thus, 0xFFFF = 64k)
*/
#define CMD1_LENGTH   1
#define CMD2_LENGTH   3
#define CMD3_LENGTH   3
#define CMD4_LENGTH   1


/*
** Sample ATS_TBL1 Table Header
*/
static CFE_TBL_FileDef_t CFE_TBL_FileDef __attribute__((__used__)) =
{
    "ATS_Table1", "SC.ATS_TBL1", "SC Sample ATS_TBL1",
    "sc_ats1.tbl", (SC_ATS_BUFF_SIZE * sizeof(uint16))
};


/*
** Sample ATS_TBL1 Table Data
*/
uint16 ATS_Table1[SC_ATS_BUFF_SIZE] =
{
  /* cmd num, <---- cmd exe time ---->   <---------------------------- cmd pkt primary header ---------------------------->  <----- cmd pkt 2nd header ---->   <-- opt data ---> */
           1, CMD1_TIME_A, CMD1_TIME_B,  CFE_MAKE_BIG16(SC_CMD_MID), CFE_MAKE_BIG16(PKT_FLAGS), CFE_MAKE_BIG16(CMD1_LENGTH), ((SC_NOOP_CC << 8) | CMD1_XSUM),
           2, CMD2_TIME_A, CMD2_TIME_B,  CFE_MAKE_BIG16(SC_CMD_MID), CFE_MAKE_BIG16(PKT_FLAGS), CFE_MAKE_BIG16(CMD2_LENGTH), ((SC_ENABLE_RTS_CC << 8) | CMD2_XSUM), CMD2_ARG,
           3, CMD3_TIME_A, CMD3_TIME_B,  CFE_MAKE_BIG16(SC_CMD_MID), CFE_MAKE_BIG16(PKT_FLAGS), CFE_MAKE_BIG16(CMD3_LENGTH), ((SC_START_RTS_CC << 8) | CMD3_XSUM), CMD3_ARG,
           4, CMD4_TIME_A, CMD4_TIME_B,  CFE_MAKE_BIG16(SC_CMD_MID), CFE_MAKE_BIG16(PKT_FLAGS), CFE_MAKE_BIG16(CMD4_LENGTH), ((SC_RESET_COUNTERS_CC << 8) | CMD4_XSUM),
           0, 0, 0, 0, 0, 0, 0
};

/************************/
/*  End of File Comment */
/************************/
