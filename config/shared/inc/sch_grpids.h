#ifndef SCH_GRPIDS_H
#define SCH_GRPIDS_H

/*
** Pragmas
*/

/*
** Include Files
*/

/*
** Local Defines
 */

/* This is an example of using schedule groups to collect an app or service
 * that sends multiple messages from SCH into a single group for more
 * convenient operational control.
 */
#define SCH_GROUP_NONE					(0)
#define SCH_GROUP_CFEHK					(0x00000001) /* All cFE HK */
#define SCH_GROUP_CI					(0x00000002)
#define SCH_GROUP_TO					(0x00000010)
#define SCH_GROUP_HK					(0x00000020)
#define SCH_GROUP_HS					(0x00000040)
#define SCH_GROUP_MD					(0x00000080)
#define SCH_GROUP_CS					(0x00000100)
#define SCH_GROUP_SC					(0x00000200)
#define SCH_GROUP_CF					(0x00000400)
/*#define SCH_GROUP_					(0x00000800) Currently unused */
/*#define SCH_GROUP_					(0x00001000) Currently unused */
/*#define SCH_GROUP_					(0x00002000) Currently unused */
/*#define SCH_GROUP_					(0x00004000) Currently unused */
/*#define SCH_GROUP_					(0x00008000) Currently unused */
/*#define SCH_GROUP_					(0x00010000) Currently unused */
/*#define SCH_GROUP_					(0x00020000) Currently unused */
/*#define SCH_GROUP_					(0x00040000) Currently unused */
/*#define SCH_GROUP_					(0x00080000) Currently unused */
/*#define SCH_GROUP_					(0x00100000) Currently unused */
/*#define SCH_GROUP_					(0x00200000) Currently unused */
/*#define SCH_GROUP_					(0x00400000) Currently unused */
/*#define SCH_GROUP_					(0x00800000) Currently unused */
/*#define SCH_GROUP_					(0x01000000) Currently unused */
/*#define SCH_GROUP_					(0x02000000) Currently unused */
/*#define SCH_GROUP_					(0x04000000) Currently unused */

#endif /* SCH_GRPIDS_H */

/*=======================================================================================
** End of file sch_grpids.h
**=====================================================================================*/

    
