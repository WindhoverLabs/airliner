/************************************************************************
** Includes
*************************************************************************/
#include "cfe.h"
#include "cfe_tbl_filedef.h"
#include "cf_tbldefs.h"
#include "cf_platform_cfg.h"
#include "cf_msgids.h"
#include "cf_defs.h"
#include "to_platform_cfg.h"
#include "sbnd_platform_cfg.h"

static OS_USED CFE_TBL_FileDef_t CFE_TBL_FileDef =
{
    "CF_ConfigTable", "CF.ConfigTable", "CF Config Tbl",
    "cf_cfgtable.tbl", sizeof (cf_config_table_t)
};


/*
** Default playback table data - See NOTE Above
*/

cf_config_table_t     CF_ConfigTable =
{

    "CF Default Table",/* TableIdString */
    2,      /* TableVersion (integer)   */    
    4,      /* NumEngCyclesPerWakeup    */
    2,      /* NumWakeupsPerQueueChk    */
    4,      /* NumWakeupsPerPollDirChk  */
    100,    /* UplinkHistoryQDepth      */
    0,      /* Reserved1                */
    0,      /* Reserved2                */
    "10",   /* AckTimeout (secs, entered as string) */
    "2",    /* AckLimit (max timeouts, string)      */
    "5",    /* NakTimeout (secs, string)            */
    "3",    /* NakLimit (max timeouts, string)      */
    "20",   /* InactivityTimeout (secs, string)     */
    "200",  /* OutgoingFileChunkSize (bytes, string)*/
    "no",   /* SaveIncompleteFiles (yes,no, string) */
    "0.2",  /* Flight EntityId - 2 byte dotted-decimal string eg. "0.255"*/

    { /* Input Channel Array */
    
        { /* Input Channel 0 */

        	CF_GND_TO_PPD_PDU_MID,
            0, /* Output Chan for Class 2 Uplink Responses, ACK-EOF,Nak,Fin etc) */
            0, /* spare */
        
        }, /* end Input Channel 0 */

        { /* Input Channel 0 */

        	CF_CPD_TO_PPD_PDU_MID,
            1, /* Output Chan for Class 2 Uplink Responses, ACK-EOF,Nak,Fin etc) */
            0, /* spare */

        }, /* end Input Channel 0 */    
    
    }, /* end Input Channel Array */

    {   /* Playback Channel Array */

        {   /* Playback Channel #0 */  
            CF_ENTRY_IN_USE,                /* Playback Channel Entry In Use */
            CF_ENABLED,                     /* Dequeue Enable */
            CF_PPD_TO_GND_PDU_MID,          /* Space To Gnd PDU MsgId */
            100,                            /* Pending Queue Depth */
            100,                            /* History Queue Depth */
            "PPD",                          /* Playback Channel Name   */
			TO_CF_THROTTLE_SEM_NAME,        /* Handshake Semaphore Name   */
        
            {   /* Polling Directory Array */
                
                { /* Polling Directory 0 */
                    CF_ENTRY_IN_USE,/* Poll Directory In Use or Not */
					CF_ENABLED,     /* Enable State */
                    1,              /* Class (1 or 2)*/
                    2,              /* Priority */
                    CF_KEEP_FILE,   /* Preserve files after successful transfer? */
                    0,              /* Reserved1 */
                    0,              /* Reserved2 */
                    0,              /* Reserved3 */
                    "0.1",          /* Gnd EntityId - 2 byte dotted-decimal string eg. "0.255"*/
                    "/cf/downlink/cpd_to_gnd/class1/priority2/", /* SrcPath, no spaces, fwd slash at end */
					"/cpd/",                        /* DstPath, no spaces */
                },/* End Polling Directory 0 */
                
                { /* Polling Directory 1 */
                    CF_ENTRY_IN_USE,/* Poll Directory In Use or Not */
					CF_ENABLED,     /* Enable State */
                    2,              /* Class (1 or 2)*/
                    2,              /* Priority */
					CF_DELETE_FILE,   /* Preserve files after successful transfer? */
                    0,              /* Reserved1 */
                    0,              /* Reserved2 */
                    0,              /* Reserved3 */
                    "0.1",          /* Gnd EntityId - 2 byte dotted-decimal string eg. "0.255"*/
                    "/cf/downlink/cpd_to_gnd/class2/priority2/", /* SrcPath, no spaces, fwd slash at end */
					"/cpd/",                                     /* DstPath, no spaces */
                },/* End Polling Directory 1 */
                
                { /* Polling Directory 2 */
                    CF_ENTRY_IN_USE,/* Poll Directory In Use or Not */
					CF_ENABLED,     /* Enable State */
                    1,              /* Class (1 or 2)*/
                    4,              /* Priority */
                    CF_KEEP_FILE,   /* Preserve files after successful transfer? */
                    0,              /* Reserved1 */
                    0,              /* Reserved2 */
                    0,              /* Reserved3 */
                    "0.1",          /* Gnd EntityId - 2 byte dotted-decimal string eg. "0.255"*/
                    "/cf/downlink/cpd_to_gnd/class1/priority4/", /* SrcPath, no spaces, fwd slash at end */
					"/cpd/",                                     /* DstPath, no spaces */
                },/* End Polling Directory 2 */
                
                { /* Polling Directory 3 */
                    CF_ENTRY_IN_USE,/* Poll Directory In Use or Not */
					CF_ENABLED,     /* Enable State */
                    2,              /* Class (1 or 2)*/
                    4,              /* Priority */
					CF_DELETE_FILE,   /* Preserve files after successful transfer? */
                    0,              /* Reserved1 */
                    0,              /* Reserved2 */
                    0,              /* Reserved3 */
                    "0.1",          /* Gnd EntityId - 2 byte dotted-decimal string eg. "0.255"*/
                    "/cf/downlink/cpd_to_gnd/class2/priority4/", /* SrcPath, no spaces, fwd slash at end */
					"/cpd/",                                     /* DstPath, no spaces */
                },/* End Polling Directory 3 */
                
                { /* Polling Directory 4 */
                    CF_ENTRY_IN_USE,/* Poll Directory In Use or Not */
					CF_ENABLED,     /* Enable State */
                    1,              /* Class (1 or 2)*/
                    6,              /* Priority */
                    CF_KEEP_FILE,   /* Preserve files after successful transfer? */
                    0,              /* Reserved1 */
                    0,              /* Reserved2 */
                    0,              /* Reserved3 */
                    "0.1",          /* Gnd EntityId - 2 byte dotted-decimal string eg. "0.255"*/
                    "/cf/downlink/cpd_to_gnd/class1/priority6/", /* SrcPath, no spaces, fwd slash at end */
					"/cpd/",                                     /* DstPath, no spaces */
                },/* End Polling Directory 4 */
                
                { /* Polling Directory 5 */
                    CF_ENTRY_IN_USE,/* Poll Directory In Use or Not */
					CF_ENABLED,     /* Enable State */
                    2,              /* Class (1 or 2)*/
                    6,              /* Priority */
					CF_DELETE_FILE,   /* Preserve files after successful transfer? */
                    0,              /* Reserved1 */
                    0,              /* Reserved2 */
                    0,              /* Reserved3 */
                    "0.1",          /* Gnd EntityId - 2 byte dotted-decimal string eg. "0.255"*/
                    "/cf/downlink/cpd_to_gnd/class2/priority6/", /* SrcPath, no spaces, fwd slash at end */
					"/cpd/",                                     /* DstPath, no spaces */
                },/* End Polling Directory 5 */
                
                { /* Polling Directory 6 */
                    CF_ENTRY_IN_USE,/* Poll Directory In Use or Not */
					CF_ENABLED,     /* Enable State */
                    1,              /* Class (1 or 2)*/
                    2,              /* Priority */
                    CF_KEEP_FILE,   /* Preserve files after successful transfer? */
                    0,              /* Reserved1 */
                    0,              /* Reserved2 */
                    0,              /* Reserved3 */
                    "0.1",          /* Gnd EntityId - 2 byte dotted-decimal string eg. "0.255"*/
                    "/cf/downlink/ppd_to_gnd/class1/priority2/", /* SrcPath, no spaces, fwd slash at end */
					"/ppd/",                                     /* DstPath, no spaces */
                },/* End Polling Directory 6 */
                
                { /* Polling Directory 7 */
                    CF_ENTRY_IN_USE,/* Poll Directory In Use or Not */
					CF_ENABLED,     /* Enable State */
                    2,              /* Class (1 or 2)*/
                    2,              /* Priority */
					CF_DELETE_FILE,   /* Preserve files after successful transfer? */
                    0,              /* Reserved1 */
                    0,              /* Reserved2 */
                    0,              /* Reserved3 */
                    "0.1",          /* Gnd EntityId - 2 byte dotted-decimal string eg. "0.255"*/
                    "/cf/downlink/ppd_to_gnd/class2/priority2/", /* SrcPath, no spaces, fwd slash at end */
					"/ppd/",                                     /* DstPath, no spaces */
                },/* End Polling Directory 7 */
                
                { /* Polling Directory 8 */
                    CF_ENTRY_IN_USE,/* Poll Directory In Use or Not */
					CF_ENABLED,     /* Enable State */
                    1,              /* Class (1 or 2)*/
                    4,              /* Priority */
                    CF_KEEP_FILE,   /* Preserve files after successful transfer? */
                    0,              /* Reserved1 */
                    0,              /* Reserved2 */
                    0,              /* Reserved3 */
                    "0.1",          /* Gnd EntityId - 2 byte dotted-decimal string eg. "0.255"*/
                    "/cf/downlink/ppd_to_gnd/class1/priority4/", /* SrcPath, no spaces, fwd slash at end */
					"/ppd/",                                     /* DstPath, no spaces */
                },/* End Polling Directory 8 */
                
                { /* Polling Directory 9 */
                    CF_ENTRY_IN_USE,/* Poll Directory In Use or Not */
					CF_ENABLED,     /* Enable State */
                    2,              /* Class (1 or 2)*/
                    4,              /* Priority */
					CF_DELETE_FILE,   /* Preserve files after successful transfer? */
                    0,              /* Reserved1 */
                    0,              /* Reserved2 */
                    0,              /* Reserved3 */
                    "0.1",          /* Gnd EntityId - 2 byte dotted-decimal string eg. "0.255"*/
                    "/cf/downlink/ppd_to_gnd/class2/priority4/", /* SrcPath, no spaces, fwd slash at end */
					"/ppd/",                                     /* DstPath, no spaces */
                },/* End Polling Directory 9 */
                
                { /* Polling Directory 10 */
                    CF_ENTRY_IN_USE,/* Poll Directory In Use or Not */
					CF_ENABLED,     /* Enable State */
                    1,              /* Class (1 or 2)*/
                    6,              /* Priority */
                    CF_KEEP_FILE,   /* Preserve files after successful transfer? */
                    0,              /* Reserved1 */
                    0,              /* Reserved2 */
                    0,              /* Reserved3 */
                    "0.1",          /* Gnd EntityId - 2 byte dotted-decimal string eg. "0.255"*/
                    "/cf/downlink/ppd_to_gnd/class1/priority6/", /* SrcPath, no spaces, fwd slash at end */
					"/ppd/",                                     /* DstPath, no spaces */
                },/* End Polling Directory 10 */
                
                { /* Polling Directory 11 */
                    CF_ENTRY_IN_USE,/* Poll Directory In Use or Not */
					CF_ENABLED,     /* Enable State */
                    2,              /* Class (1 or 2)*/
                    6,              /* Priority */
					CF_DELETE_FILE,   /* Preserve files after successful transfer? */
                    0,              /* Reserved1 */
                    0,              /* Reserved2 */
                    0,              /* Reserved3 */
                    "0.1",          /* Gnd EntityId - 2 byte dotted-decimal string eg. "0.255"*/
                    "/cf/downlink/ppd_to_gnd/class2/priority6/", /* SrcPath, no spaces, fwd slash at end */
					"/ppd/",                                     /* DstPath, no spaces */
                },/* End Polling Directory 11 */

            }, /* End Polling Directory Array */

        },  /* End Playback Channel #0 */

        {   /* Playback Channel #1 */
            CF_ENTRY_IN_USE,                /* Playback Channel Entry In Use */
            CF_ENABLED,                     /* Dequeue Enable */
            CF_PPD_TO_CPD_PDU_MID,          /* Space To Gnd PDU MsgId */
            100,                            /* Pending Queue Depth */
            100,                            /* History Queue Depth */
            "CPD",                          /* Playback Channel Name   */
			SBND_CF_THROTTLE_SEM_NAME,      /* Handshake Semaphore Name   */

            {   /* Polling Directory Array */
                
                { /* Polling Directory 0 */
                    CF_ENTRY_IN_USE,/* Poll Directory In Use or Not */
					CF_ENABLED,     /* Enable State */
                    1,              /* Class (1 or 2)*/
                    2,              /* Priority */
                    CF_KEEP_FILE,   /* Preserve files after successful transfer? */
                    0,              /* Reserved1 */
                    0,              /* Reserved2 */
                    0,              /* Reserved3 */
                    "0.3",          /* Gnd EntityId - 2 byte dotted-decimal string eg. "0.255"*/
                    "/cf/downlink/ppd_to_gnd/class1/priority2/",  /* SrcPath, no spaces, fwd slash at end */
					"/ram/downlink/cpd_to_gnd/class1/priority2/", /* DstPath, no spaces */
                },/* End Polling Directory 0 */
                
                { /* Polling Directory 1 */
                    CF_ENTRY_IN_USE,/* Poll Directory In Use or Not */
					CF_ENABLED,     /* Enable State */
                    2,              /* Class (1 or 2)*/
                    2,              /* Priority */
					CF_DELETE_FILE,   /* Preserve files after successful transfer? */
                    0,              /* Reserved1 */
                    0,              /* Reserved2 */
                    0,              /* Reserved3 */
                    "0.3",          /* Gnd EntityId - 2 byte dotted-decimal string eg. "0.255"*/
                    "/cf/downlink/ppd_to_gnd/class2/priority2/",  /* SrcPath, no spaces, fwd slash at end */
					"/ram/downlink/cpd_to_gnd/class2/priority2/", /* DstPath, no spaces */
                },/* End Polling Directory 1 */

                { /* Polling Directory 2 */
                    CF_ENTRY_IN_USE,/* Poll Directory In Use or Not */
					CF_ENABLED,     /* Enable State */
                    1,              /* Class (1 or 2)*/
                    4,              /* Priority */
                    CF_KEEP_FILE,   /* Preserve files after successful transfer? */
                    0,              /* Reserved1 */
                    0,              /* Reserved2 */
                    0,              /* Reserved3 */
                    "0.3",          /* Gnd EntityId - 2 byte dotted-decimal string eg. "0.255"*/
                    "/cf/downlink/ppd_to_gnd/class1/priority4/",  /* SrcPath, no spaces, fwd slash at end */
					"/ram/downlink/cpd_to_gnd/class1/priority4/", /* DstPath, no spaces */
                },/* End Polling Directory 2 */

                { /* Polling Directory 3 */
                    CF_ENTRY_IN_USE,/* Poll Directory In Use or Not */
					CF_ENABLED,     /* Enable State */
                    2,              /* Class (1 or 2)*/
                    4,              /* Priority */
					CF_DELETE_FILE,   /* Preserve files after successful transfer? */
                    0,              /* Reserved1 */
                    0,              /* Reserved2 */
                    0,              /* Reserved3 */
                    "0.3",          /* Gnd EntityId - 2 byte dotted-decimal string eg. "0.255"*/
                    "/cf/downlink/ppd_to_gnd/class2/priority4/",  /* SrcPath, no spaces, fwd slash at end */
					"/ram/downlink/cpd_to_gnd/class2/priority4/", /* DstPath, no spaces */
                },/* End Polling Directory 3 */

                { /* Polling Directory 4 */
                    CF_ENTRY_IN_USE,/* Poll Directory In Use or Not */
					CF_ENABLED,     /* Enable State */
                    1,              /* Class (1 or 2)*/
                    6,              /* Priority */
                    CF_KEEP_FILE,   /* Preserve files after successful transfer? */
                    0,              /* Reserved1 */
                    0,              /* Reserved2 */
                    0,              /* Reserved3 */
                    "0.3",          /* Gnd EntityId - 2 byte dotted-decimal string eg. "0.255"*/
                    "/cf/downlink/ppd_to_gnd/class1/priority6/",  /* SrcPath, no spaces, fwd slash at end */
					"/ram/downlink/cpd_to_gnd/class1/priority6/", /* DstPath, no spaces */
                },/* End Polling Directory 4 */

                { /* Polling Directory 5 */
                    CF_ENTRY_IN_USE,/* Poll Directory In Use or Not */
					CF_ENABLED,     /* Enable State */
                    2,              /* Class (1 or 2)*/
                    6,              /* Priority */
					CF_DELETE_FILE,   /* Preserve files after successful transfer? */
                    0,              /* Reserved1 */
                    0,              /* Reserved2 */
                    0,              /* Reserved3 */
                    "0.3",          /* Gnd EntityId - 2 byte dotted-decimal string eg. "0.255"*/
                    "/cf/downlink/ppd_to_gnd/class2/priority6/",  /* SrcPath, no spaces, fwd slash at end */
					"/ram/downlink/cpd_to_gnd/class2/priority6/", /* DstPath, no spaces */
                },/* End Polling Directory 5 */
                
            }, /* End Polling Directory Array */
        
        },  /* End Playback Channel #1 */

    },  /* End Playback Channel Array */

}; /* End CF_ConfigTable */   
    

/************************/
/*  End of File Comment */
/************************/
