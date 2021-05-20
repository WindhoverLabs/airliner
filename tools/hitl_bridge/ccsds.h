#ifndef _ccsds_
#define _ccsds_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>


/*
** Include Files
*/


/* Macro to convert 16 bit word from platform "endianness" to Big Endian */
#ifdef SOFTWARE_BIG_BIT_ORDER
  #define CFE_MAKE_BIG16(n) (n)
#else
  #define CFE_MAKE_BIG16(n) ( (((n) << 8) & 0xFF00) | (((n) >> 8) & 0x00FF) )
#endif


/* CCSDS_TIME_SIZE is specific to the selected CFE_SB time format */
#define CCSDS_TIME_SIZE 6


/*
** Type Definitions
*/

/**********************************************************************
** Structure definitions for CCSDS headers.  All items in the structure
** must be aligned on 16-bit words.  Bitfields must be avoided since
** some compilers (such as gcc) force them into 32-bit alignment.
**********************************************************************/

#define CCSDS_MAX_LENGTH (0xffff+7)

/*----- CCSDS packet primary header. -----*/

typedef struct {

   uint8_t   StreamId[2];  /* packet identifier word (stream ID) */
      /*  bits  shift   ------------ description ---------------- */
      /* 0x07FF    0  : application ID                            */
      /* 0x0800   11  : secondary header: 0 = absent, 1 = present */
      /* 0x1000   12  : packet type:      0 = TLM, 1 = CMD        */
      /* 0xE000   13  : CCSDS version, always set to 0            */

   uint8_t   Sequence[2];  /* packet sequence word */
      /*  bits  shift   ------------ description ---------------- */
      /* 0x3FFF    0  : sequence count                            */
      /* 0xC000   14  : segmentation flags:  3 = complete packet  */

   uint8_t  Length[2];     /* packet length word */
      /*  bits  shift   ------------ description ---------------- */
      /* 0xFFFF    0  : (total packet length) - 7                 */

} CCSDS_PriHdr_t;

/*----- CCSDS command secondary header. -----*/

typedef struct {

	uint16_t  Command;      /* command secondary header */
      /*  bits  shift   ------------ description ---------------- */
      /* 0x00FF    0  : checksum, calculated by ground system     */
      /* 0x7F00    8  : command function code                     */
      /* 0x8000   15  : reserved, set to 0                        */

} CCSDS_CmdSecHdr_t;

/*----- CCSDS telemetry secondary header. -----*/

typedef struct {

	uint8_t  Time[CCSDS_TIME_SIZE];

} CCSDS_TlmSecHdr_t;

/*----- Generic combined command header. -----*/

typedef struct {
   CCSDS_PriHdr_t       PriHdr;
   CCSDS_CmdSecHdr_t    SecHdr;
} CCSDS_CmdPkt_t;

/*----- Generic combined telemetry header. -----*/

typedef struct {
   CCSDS_PriHdr_t       PriHdr;
   CCSDS_TlmSecHdr_t    SecHdr;
} CCSDS_TlmPkt_t;


/*
** Macro Definitions
*/

/**********************************************************************
** Constant values.
**********************************************************************/

/* Value of packet type for a telemetry packet. */
#define CCSDS_TLM  0
/* Value of packet type for a command packet. */
#define CCSDS_CMD  1

/* Value of secondary header flag if secondary header not present. */
#define CCSDS_NO_SEC_HDR   0
/* Value of secondary header flag if secondary header exists. */
#define CCSDS_HAS_SEC_HDR  1

#define NUM_CCSDS_APIDS        2048
#define NUM_CCSDS_PKT_TYPES    2


/**********************************************************************
** Initial values for CCSDS header fields.
**********************************************************************/

/* Initial value of the sequence count. */
#define CCSDS_INIT_SEQ      0
/* Initial value of the sequence flags. */
#define CCSDS_INIT_SEQFLG   3
/* Initial value of the command function code. */
#define CCSDS_INIT_FC       0
/* Initial value of the command checksum. */
#define CCSDS_INIT_CHECKSUM 0

/* Note: the stream ID and length are always explicitly set for a packet,
** so default values are not required.  */


/**********************************************************************
** Macros for reading and writing bit fields in a 16-bit integer.
** These are used to implement the read and write macros below.
**********************************************************************/

/* Read bits specified by 'mask' from 'word' and shift down by 'shift'. */
#define CCSDS_RD_BITS(word,mask,shift) \
   (((word) & mask) >> shift)

/* Shift 'value' up by 'shift' and write to those bits in 'word' that
** are specified by 'mask'.  Other bits in 'word' are unchanged.   */
#define CCSDS_WR_BITS(word,mask,shift,value) \
   ((word) = (uint16_t)(((word) & ~mask) | (((value) & (mask >> shift)) << shift)))


/**********************************************************************
** Macros for reading and writing the fields in a CCSDS header.  All
** of the macros are used in a similar way:
**
**   CCSDS_RD_xxx(header)        -- Read field xxx from header.
**   CCSDS_WR_xxx(header,value)  -- Write value to field xxx of header.
**
** Note that 'header' is a reference to the actual header structure,
** not to a pointer to the structure.  If using a pointer, one must
** refer to the structure as *pointer.
**
** The CCSDS_WR_xxx macros may refer to 'header' more than once; thus
** the expression for 'header' must NOT contain any side effects.
**********************************************************************/

/* Read entire stream ID from primary header. */
#define CCSDS_RD_SID(phdr)         (((phdr).StreamId[0] << 8) + ((phdr).StreamId[1]))
/* Write entire stream ID to primary header. */
#define CCSDS_WR_SID(phdr,value)   ( ((phdr).StreamId[0] = (value >> 8)   ) ,\
                                     ((phdr).StreamId[1] = (value & 0xff) ) )

/* Read application ID from primary header. */
#define CCSDS_RD_APID(phdr)         (CCSDS_RD_SID(phdr) & 0x07FF)
/* Write application ID to primary header. */
#define CCSDS_WR_APID(phdr,value)  ((((phdr).StreamId[0] = ((phdr).StreamId[0] & 0xF8) | ((value >> 8) & 0x07))) ,\
                                   (((phdr).StreamId[1]  = ((value)) & 0xff)) )

/* Read secondary header flag from primary header. */
#define CCSDS_RD_SHDR(phdr)         (((phdr).StreamId[0] & 0x08) >> 3)
/* Write secondary header flag to primary header. */
#define CCSDS_WR_SHDR(phdr,value)   ((phdr).StreamId[0] = ((phdr).StreamId[0] & 0xf7) | ((value << 3) & 0x08))

/* Read packet type (0=TLM,1=CMD) from primary header. */
#define CCSDS_RD_TYPE(phdr)         (((phdr).StreamId[0] & 0x10) >> 4)
/* Write packet type (0=TLM,1=CMD) to primary header. */
#define CCSDS_WR_TYPE(phdr,value)   ((phdr).StreamId[0] = ((phdr).StreamId[0] & 0xEF) | ((value << 4) & 0x10))

/* Read CCSDS version from primary header. */
#define CCSDS_RD_VERS(phdr)        (((phdr).StreamId[0] & 0xE0) >> 5)
/* Write CCSDS version to primary header. */
#define CCSDS_WR_VERS(phdr,value)  ((phdr).StreamId[0] = ((phdr).StreamId[0] & 0x1F) | ((value << 5) & 0xE0))

/* Read sequence count from primary header. */
#define CCSDS_RD_SEQ(phdr)         ((((phdr).Sequence[0] & 0x3F) << 8) + ((phdr).Sequence[1]))
/* Write sequence count to primary header. */
#define CCSDS_WR_SEQ(phdr,value)   ((((phdr).Sequence[0] = ((phdr).Sequence[0] & 0xC0) | ((value >> 8) & 0x3f))) ,\
                                   (((phdr).Sequence[1]  = ((value)) & 0xff)) )

/* Read sequence flags from primary header. */
#define CCSDS_RD_SEQFLG(phdr)       (((phdr).Sequence[0] & 0xC0) >> 6)
/* Write sequence flags to primary header. */
#define CCSDS_WR_SEQFLG(phdr,value) ((phdr).Sequence[0] = ((phdr).Sequence[0] & 0x3F) | ((value << 6) & 0xC0) )

/* Read total packet length from primary header. */
#define CCSDS_RD_LEN(phdr)     ( ( (phdr).Length[0] << 8) + (phdr).Length[1] + 7)
/* Write total packet length to primary header. */
#define CCSDS_WR_LEN(phdr,value)   ((((phdr).Length[0] = ((value) - 7) >> 8)) ,\
                                   (((phdr).Length[1] = ((value) - 7) & 0xff)) )

/* Read function code from command secondary header. */
#define CCSDS_RD_FC(shdr)           CCSDS_RD_BITS((shdr).Command, 0x7F00, 8)
/* Write function code to command secondary header. */
#define CCSDS_WR_FC(shdr,value)     CCSDS_WR_BITS((shdr).Command, 0x7F00, 8, value)

/* Read checksum from command secondary header. */
#define CCSDS_RD_CHECKSUM(shdr)     CCSDS_RD_BITS((shdr).Command, 0x00FF, 0)
/* Write checksum to command secondary header. */
#define CCSDS_WR_CHECKSUM(shdr,val) CCSDS_WR_BITS((shdr).Command, 0x00FF, 0, val)


/**********************************************************************
** Macros for clearing a CCSDS header to a standard initial state.  All
** of the macros are used in a similar way:
**   CCSDS_CLR_xxx_HDR(header)      -- Clear header of type xxx.
**********************************************************************/

/* Clear primary header. */
#define CCSDS_CLR_PRI_HDR(phdr) \
  ( (phdr).StreamId[0] = 0,\
    (phdr).StreamId[1] = 0,\
    (phdr).Sequence[0] = (CCSDS_INIT_SEQFLG << 6),\
    (phdr).Sequence[1] = 0,\
    (phdr).Length[0] = 0, \
    (phdr).Length[1] = 0 )

/* Clear command secondary header. */
#define CCSDS_CLR_CMDSEC_HDR(shdr) \
  ( (shdr).Command = (CCSDS_INIT_CHECKSUM << 0) | (CCSDS_INIT_FC << 8) )


#define CCSDS_WR_SEC_HDR_SEC(shdr, value)    shdr.Time[0] = ((value>>24) & 0xFF),  \
                                             shdr.Time[1] = ((value>>16) & 0xFF),  \
                                             shdr.Time[2] = ((value>>8)  & 0xFF),  \
                                             shdr.Time[3] = ((value)     & 0xFF)

#define CCSDS_RD_SEC_HDR_SEC(shdr)           (((uint32)shdr.Time[0]) << 24) | \
                                             (((uint32)shdr.Time[1]) << 16) | \
                                             (((uint32)shdr.Time[2]) << 8)  | \
                                             ((uint32)shdr.Time[3])

/* Clear telemetry secondary header. */
#if (CFE_SB_PACKET_TIME_FORMAT == CFE_SB_TIME_32_16_SUBS)
  /* 32 bits seconds + 16 bits subseconds */
  #define CCSDS_CLR_TLMSEC_HDR(shdr) \
  ( (shdr).Time[0] = 0,\
    (shdr).Time[1] = 0,\
    (shdr).Time[2] = 0,\
    (shdr).Time[3] = 0,\
    (shdr).Time[4] = 0,\
    (shdr).Time[5] = 0 )


#define CCSDS_WR_SEC_HDR_SUBSEC(shdr, value) shdr.Time[4] = ((value>>8)  & 0xFF),  \
                                             shdr.Time[5] = ((value)     & 0xFF)

#define CCSDS_RD_SEC_HDR_SUBSEC(shdr)        (((uint32)shdr.Time[4]) << 8)  | \
                                             ((uint32)shdr.Time[5])
#elif ((CFE_SB_PACKET_TIME_FORMAT == CFE_SB_TIME_32_32_SUBS) ||\
       (CFE_SB_PACKET_TIME_FORMAT == CFE_SB_TIME_32_32_M_20))
  /* 32 bits seconds + 32 bits subseconds */
  #define CCSDS_CLR_TLMSEC_HDR(shdr) \
  ( (shdr).Time[0] = 0,\
    (shdr).Time[1] = 0,\
    (shdr).Time[2] = 0,\
    (shdr).Time[3] = 0,\
    (shdr).Time[4] = 0,\
    (shdr).Time[5] = 0,\
    (shdr).Time[6] = 0,\
    (shdr).Time[7] = 0 )

#define CCSDS_WR_SEC_HDR_SUBSEC(shdr, value) shdr.Time[4] = ((value>>24) & 0xFF),  \
                                             shdr.Time[5] = ((value>>16) & 0xFF),  \
                                             shdr.Time[6] = ((value>>8)  & 0xFF),  \
                                             shdr.Time[7] = ((value)     & 0xFF)

#define CCSDS_RD_SEC_HDR_SUBSEC(shdr)        (((uint32)shdr.Time[4]) << 24) | \
                                             (((uint32)shdr.Time[5]) << 16) | \
                                             (((uint32)shdr.Time[6]) << 8)  | \
                                             ((uint32)shdr.Time[7])
#endif



/**********************************************************************
** Macros for extracting fields from a stream ID.  All of the macros
** are used in a similar way:
**
**   CCSDS_SID_xxx(sid)          -- Extract field xxx from sid.
**********************************************************************/

/* Extract application ID from stream ID. */
#define CCSDS_SID_APID(sid)   CCSDS_RD_BITS(sid, 0x07FF, 0)

/* Extract secondary header flag from stream ID. */
#define CCSDS_SID_SHDR(sid)   CCSDS_RD_BITS(sid, 0x0800, 11)

/* Extract packet type (0=TLM,1=CMD) from stream ID. */
#define CCSDS_SID_TYPE(sid)   CCSDS_RD_BITS(sid, 0x1000, 12)

/* Extract CCSDS version from stream ID. */
#define CCSDS_SID_VERS(sid)   CCSDS_RD_BITS(sid, 0xE000, 13)


/**********************************************************************
** Macros for frequently used combinations of operations.
**
**   CCSDS_INC_SEQ(phdr)           -- Increment sequence count.
**********************************************************************/

/* Increment sequence count in primary header by 1. */
#define CCSDS_INC_SEQ(phdr) \
   CCSDS_WR_SEQ(phdr, (CCSDS_RD_SEQ(phdr)+1))


/*********************************************************************/

/*
** Exported Functions
*/

/******************************************************************************
**  Function:  CCSDS_InitPkt()
**
**  Purpose:
**    Initialize a CCSDS packet.  The primary header is initialized with
**    specified values, and if the Clear flag is set, the rest of the packet
**    is filled with zeros.
**
**  Arguments:
**    PktPtr   : Pointer to primary header of packet.
**    StreamId : Stream ID to use for the packet.
**    Length   : Length of the packet in bytes.
**    Clear    : Indicates whether to clear the entire packet:
**                TRUE = fill sequence count and packet data with zeros
**                       (used after a cold restart)
**                FALSE = leave sequence count and packet data unchanged
**                       (used after a warm restart if data must be preserved)
**
**  Return:
**    (none)
*/

void CCSDS_InitPkt (CCSDS_PriHdr_t  *PktPtr,
		            uint16_t         StreamId,
		            uint16_t         Length,
		            uint8_t          Clear );



/******************************************************************************
**  Function:  CCSDS_LoadCheckSum()
**
**  Purpose:
**    Compute and load a checksum for a CCSDS command packet that has a
**    secondary header.
**
**  Arguments:
**    PktPtr   : Pointer to header of command packet.  The packet must
**               have a secondary header and the length in the primary
**               header must be correct.  The checksum field in the packet
**               will be modified.
**
**  Return:
**    (none)
*/

void CCSDS_LoadCheckSum (CCSDS_CmdPkt_t *PktPtr);

/******************************************************************************
**  Function:  CCSDS_ValidCheckSum()
**
**  Purpose:
**    Determine whether a checksum in a command packet is valid.
**
**  Arguments:
**    PktPtr   : Pointer to header of command packet.  The packet must
**               have a secondary header and the length in the primary
**               header must be correct.
**
**  Return:
**    TRUE if checksum of packet is valid; FALSE if not.
**    A valid checksum is 0.
*/

uint8_t CCSDS_ValidCheckSum (CCSDS_CmdPkt_t *PktPtr);

/******************************************************************************
**  Function:  CCSDS_ComputeCheckSum()
**
**  Purpose:
**    Compute the checksum for a command packet.  The checksum is the XOR of
**    all bytes in the packet; a valid checksum is zero.
**
**  Arguments:
**    PktPtr   : Pointer to header of command packet.  The packet must
**               have a secondary header and the length in the primary
**               header must be correct.
**
**  Return:
**    TRUE if checksum of packet is valid; FALSE if not.
*/

uint8_t CCSDS_ComputeCheckSum (CCSDS_CmdPkt_t *PktPtr);




typedef union {
    CCSDS_PriHdr_t      Hdr;   /**< \brief CCSDS Primary Header #CCSDS_PriHdr_t */
    uint32_t            Dword; /**< \brief Forces minimum of 32-bit alignment for this object */
    uint8_t             Byte[sizeof(CCSDS_PriHdr_t)];   /**< \brief Allows byte-level access */
}CFE_SB_Msg_t;

typedef CFE_SB_Msg_t *CFE_SB_MsgPtr_t;

typedef uint16_t CFE_SB_MsgId_t;

CFE_SB_MsgId_t CFE_SB_GetMsgId(CFE_SB_MsgPtr_t MsgPtr);

void *CFE_SB_GetUserData(CFE_SB_MsgPtr_t MsgPtr);
uint16_t CFE_SB_MsgHdrSize(CFE_SB_MsgId_t MsgId);



/**< \brief Generic Software Bus Telemetry Header Type Definition */
typedef struct{
    CCSDS_PriHdr_t      Pri;/**< \brief CCSDS Primary Header #CCSDS_PriHdr_t */
    CCSDS_TlmSecHdr_t   Sec;/**< \brief CCSDS Telemetry Secondary Header #CCSDS_TlmSecHdr_t */
}CFE_SB_TlmHdr_t;

/**< \brief Generic Software Bus Command Header Type Definition */
typedef struct{
    CCSDS_PriHdr_t      Pri;/**< \brief CCSDS Primary Header #CCSDS_PriHdr_t */
    CCSDS_CmdSecHdr_t   Sec;/**< \brief CCSDS Command Secondary Header #CCSDS_CmdSecHdr_t */
}CFE_SB_CmdHdr_t;

#define CFE_SB_CMD_HDR_SIZE     (sizeof(CFE_SB_CmdHdr_t))/**< \brief Size of #CFE_SB_CmdHdr_t in bytes */
#define CFE_SB_TLM_HDR_SIZE     (sizeof(CFE_SB_TlmHdr_t))/**< \brief Size of #CFE_SB_TlmHdr_t in bytes */



#ifdef __cplusplus
}
#endif


#endif  /* _ccsds_ */
/*****************************************************************************/
