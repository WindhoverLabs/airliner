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
    3,     /* NumEngCyclesPerWakeup    */
    1,      /* NumWakeupsPerQueueChk    */
    1,      /* NumWakeupsPerPollDirChk  */
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
    "0.24", /* Flight EntityId - 2 byte dotted-decimal string eg. "0.255"*/

    { /* Input Channel Array */
    
        { /* Input Channel 0 */
            
            CF_INCOMING_PDU_MID,
            0, /* Output Chan for Class 2 Uplink Responses, ACK-EOF,Nak,Fin etc) */
            0, /* spare */
        
        }, /* end Input Channel 0 */    
    
    }, /* end Input Channel Array */

    {   /* Playback Channel Array */

        {   /* Playback Channel #0 */  
            CF_ENTRY_IN_USE,                /* Playback Channel Entry In Use */
            CF_ENABLED,                     /* Dequeue Enable */
            CF_SPACE_TO_GND_PDU_MID,        /* Space To Gnd PDU MsgId */
            100,                            /* Pending Queue Depth */
            100,                            /* History Queue Depth */
            "OutputChan0",                  /* Playback Channel Name   */
			TO_CF_THROTTLE_SEM_NAME,        /* Handshake Semaphore Name   */
        
            {   /* Polling Directory Array */
                
                { /* Polling Directory 0 */
                    CF_ENTRY_IN_USE,/* Poll Directory In Use or Not */
		            CF_ENABLED,     /* Enable State */
                    1,              /* Class (1 or 2)*/
                    0,              /* Priority */
                    CF_KEEP_FILE,   /* Preserve files after successful transfer? */
                    0,              /* Reserved1 */
                    0,              /* Reserved2 */
                    0,              /* Reserved3 */
                    "0.23",         /* Gnd EntityId - 2 byte dotted-decimal string eg. "0.255"*/
                    "/ram/downlink/ch0/dir0/", /* SrcPath, no spaces, fwd slash at end */
                    "/",    /* DstPath, no spaces */
                },/* End Polling Directory 0 */
                
                { /* Polling Directory 1 */
                    CF_ENTRY_IN_USE,/* Poll Directory In Use or Not */
		            CF_ENABLED,     /* Enable State */
                    2,              /* Class (1 or 2)*/
                    0,              /* Priority */                  
                    CF_DELETE_FILE, /* Preserve files after successful transfer? */
                    0,              /* Reserved1 */
                    0,              /* Reserved2 */
                    0,              /* Reserved3 */
                    "0.23",         /* Gnd EntityId - 2 byte dotted-decimal string eg. "0.255"*/
                    "/ram/downlink/ch0/dir1/", /* SrcPathname */
                    "/",         /* DestPathname */
                    
                },/* End Polling Directory 1 */
                
                
                { /* Polling Directory 2 */
                    CF_ENTRY_IN_USE,/* Poll Directory In Use or Not */
                    CF_DISABLED,    /* Enable State */
                    1,              /* Class (1 or 2)*/
                    5,              /* Priority */                  
                    CF_DELETE_FILE, /* Preserve files after successful transfer? */                    
                    0,              /* Reserved1 */
                    0,              /* Reserved2 */
                    0,              /* Reserved3 */
                    "0.23",         /* Gnd EntityId - 2 byte dotted-decimal string eg. "0.255"*/
                    "/ram/downlink/ch0/dir2/", /* SrcPathname */
                    "ch0/dir2/",    /* DstPathname */
                },/* End Polling Directory 2 */
                
                { /* Polling Directory 3 */
                    CF_ENTRY_IN_USE,/* Poll Directory In Use or Not */
                    CF_DISABLED,    /* Enable State */
                    1,              /* Class (1 or 2)*/
                    0,              /* Priority */                  
                    CF_KEEP_FILE,   /* Preserve files after successful transfer? */                    
                    0,              /* Reserved1 */
                    0,              /* Reserved2 */
                    0,              /* Reserved3 */
                    "0.23",         /* Gnd EntityId - 2 byte dotted-decimal string eg. "0.255"*/
                    "/ram/downlink/ch0/dir3/", /* SrcPathname */
                    "ch0/dir3/",         /* DestPathname */
                    
                },/* End Polling Directory 3 */
                
                { /* Polling Directory 4 */
                    CF_ENTRY_IN_USE,/* Poll Directory In Use or Not */
                    CF_DISABLED,    /* Enable State */
                    1,              /* Class (1 or 2)*/
                    5,              /* Priority */                  
                    CF_DELETE_FILE, /* Preserve files after successful transfer? */                    
                    0,              /* Reserved1 */
                    0,              /* Reserved2 */
                    0,              /* Reserved3 */
                    "0.23",         /* Gnd EntityId - 2 byte dotted-decimal string eg. "0.255"*/
                    "/ram/downlink/ch0/dir4/", /* SrcPathname */
                    "ch0/dir4/",    /* DstPathname */
                },/* End Polling Directory 4 */
                
                { /* Polling Directory 5 */
                    CF_ENTRY_IN_USE,/* Poll Directory In Use or Not */
                    CF_DISABLED,    /* Enable State */
                    1,              /* Class (1 or 2)*/
                    0,              /* Priority */                  
                    CF_KEEP_FILE,   /* Preserve files after successful transfer? */                    
                    0,              /* Reserved1 */
                    0,              /* Reserved2 */
                    0,              /* Reserved3 */
                    "0.23",         /* Gnd EntityId - 2 byte dotted-decimal string eg. "0.255"*/
                    "/ram/downlink/ch0/dir5/", /* SrcPathname */
                    "ch0/dir5/",         /* DestPathname */
                    
                },/* End Polling Directory 5 */
                
                { /* Polling Directory 6 */
                    CF_ENTRY_IN_USE,/* Poll Directory In Use or Not */
                    CF_DISABLED,    /* Enable State */
                    1,              /* Class (1 or 2)*/
                    5,              /* Priority */                  
                    CF_DELETE_FILE, /* Preserve files after successful transfer? */                    
                    0,              /* Reserved1 */
                    0,              /* Reserved2 */
                    0,              /* Reserved3 */
                    "0.23",         /* Gnd EntityId - 2 byte dotted-decimal string eg. "0.255"*/
                    "/ram/downlink/ch0/dir6/", /* SrcPathname */
                    "ch0/dir6/",    /* DstPathname */
                },/* End Polling Directory 6 */
                
                { /* Polling Directory 7 */
                    CF_ENTRY_IN_USE,/* Poll Directory In Use or Not */
                    CF_DISABLED,    /* Enable State */
                    1,              /* Class (1 or 2)*/
                    0,              /* Priority */                  
                    CF_KEEP_FILE,   /* Preserve files after successful transfer? */                    
                    0,              /* Reserved1 */
                    0,              /* Reserved2 */
                    0,              /* Reserved3 */
                    "0.23",         /* Gnd EntityId - 2 byte dotted-decimal string eg. "0.255"*/
                    "/ram/downlink/ch0/dir7/", /* SrcPathname */
                    "ch0/dir7/",         /* DestPathname */
                    
                },/* End Polling Directory 7 */                
                
            }, /* End Polling Directory Array */
        
        },  /* End Playback Channel #0 */
        

        {   /* Playback Channel #1 */  
            CF_ENTRY_UNUSED,                /* Playback Channel Entry In Use */
            CF_DISABLED,                    /* Dequeue Enable */
            CF_SPACE_TO_GND_PDU_MID,        /* Space To Gnd PDU MsgId */
            100,                            /* Pending Queue Depth */
            100,                            /* History Queue Depth */
            "OutputChan1",                  /* Playback Channel Name   */
            "TO_CF_CH1_SEM",                /* Handshake Semaphore Name   */
        
            {   /* Polling Directory Array */
                
                { /* Polling Directory 0 */
                    CF_ENTRY_UNUSED,/* Poll Directory In Use or Not */
                    CF_DISABLED,    /* Enable State */
                    1,              /* Class (1 or 2)*/
                    0,              /* Priority */                  
                    CF_DELETE_FILE, /* Preserve files after successful transfer? */                    
                    0,              /* Reserved1 */
                    0,              /* Reserved2 */
                    0,              /* Reserved3 */
                    "0.23",         /* Gnd EntityId - 2 byte dotted-decimal string eg. "0.255"*/
                    "/ram/downlink/ch1/dir0/", /* SrcPathname */
                    "ch1/dir0/",    /* DstPathname */
                },/* End Polling Directory 0 */
                
                { /* Polling Directory 1 */
                    CF_ENTRY_UNUSED,/* Poll Directory In Use or Not */
                    CF_DISABLED,    /* Enable State */
                    1,              /* Class (1 or 2)*/
                    0,              /* Priority */                  
                    CF_DELETE_FILE, /* Preserve files after successful transfer? */                    
                    0,              /* Reserved1 */
                    0,              /* Reserved2 */
                    0,              /* Reserved3 */
                    "0.23",         /* Gnd EntityId - 2 byte dotted-decimal string eg. "0.255"*/
                    "/ram/downlink/ch1/dir1/", /* SrcPathname */
                    "ch1/dir1/",    /* DstPathname */
                }, /* End Polling Directory 1 */
                
                { /* Polling Directory 2 */
                    CF_ENTRY_UNUSED,/* Poll Directory In Use or Not */
                    CF_DISABLED,    /* Enable State */
                    1,              /* Class (1 or 2)*/
                    0,              /* Priority */                  
                    CF_DELETE_FILE, /* Preserve files after successful transfer? */                    
                    0,              /* Reserved1 */
                    0,              /* Reserved2 */
                    0,              /* Reserved3 */
                    "0.23",         /* Gnd EntityId - 2 byte dotted-decimal string eg. "0.255"*/
                    "/ram/downlink/ch1/dir2/", /* SrcPathname */
                    "ch1/dir2/",    /* DstPathname */
                },/* End Polling Directory 2 */
                
                { /* Polling Directory 3 */
                    CF_ENTRY_UNUSED,/* Poll Directory In Use or Not */
                    CF_DISABLED,    /* Enable State */
                    1,              /* Class (1 or 2)*/
                    0,              /* Priority */                  
                    CF_DELETE_FILE, /* Preserve files after successful transfer? */                    
                    0,              /* Reserved1 */
                    0,              /* Reserved2 */
                    0,              /* Reserved3 */
                    "0.23",         /* Gnd EntityId - 2 byte dotted-decimal string eg. "0.255"*/
                    "/ram/downlink/ch1/dir3/", /* SrcPathname */
                    "ch1/dir3/",    /* DstPathname */
                }, /* End Polling Directory 3 */
                
                { /* Polling Directory 4 */
                    CF_ENTRY_UNUSED,/* Poll Directory In Use or Not */
                    CF_DISABLED,    /* Enable State */
                    1,              /* Class (1 or 2)*/
                    0,              /* Priority */                  
                    CF_DELETE_FILE, /* Preserve files after successful transfer? */                    
                    0,              /* Reserved1 */
                    0,              /* Reserved2 */
                    0,              /* Reserved3 */
                    "0.23",         /* Gnd EntityId - 2 byte dotted-decimal string eg. "0.255"*/
                    "/ram/downlink/ch1/dir4/", /* SrcPathname */
                    "ch1/dir4/",    /* DstPathname */
                },/* End Polling Directory 4 */
                
                { /* Polling Directory 5 */
                    CF_ENTRY_UNUSED,/* Poll Directory In Use or Not */
                    CF_DISABLED,    /* Enable State */
                    1,              /* Class (1 or 2)*/
                    0,              /* Priority */                  
                    CF_DELETE_FILE, /* Preserve files after successful transfer? */                    
                    0,              /* Reserved1 */
                    0,              /* Reserved2 */
                    0,              /* Reserved3 */
                    "0.23",         /* Gnd EntityId - 2 byte dotted-decimal string eg. "0.255"*/
                    "/ram/downlink/ch1/dir5/", /* SrcPathname */
                    "ch1/dir5/",    /* DstPathname */
                }, /* End Polling Directory 5 */
                
                { /* Polling Directory 6 */
                    CF_ENTRY_UNUSED,/* Poll Directory In Use or Not */
                    CF_DISABLED,    /* Enable State */
                    1,              /* Class (1 or 2)*/
                    0,              /* Priority */                  
                    CF_DELETE_FILE, /* Preserve files after successful transfer? */                    
                    0,              /* Reserved1 */
                    0,              /* Reserved2 */
                    0,              /* Reserved3 */
                    "0.23",         /* Gnd EntityId - 2 byte dotted-decimal string eg. "0.255"*/
                    "/ram/downlink/ch1/dir6/", /* SrcPathname */
                    "ch1/dir6/",    /* DstPathname */
                },/* End Polling Directory 6 */
                
                { /* Polling Directory 7 */
                    CF_ENTRY_UNUSED,/* Poll Directory In Use or Not */
                    CF_DISABLED,    /* Enable State */
                    1,              /* Class (1 or 2)*/
                    0,              /* Priority */                  
                    CF_DELETE_FILE, /* Preserve files after successful transfer? */                    
                    0,              /* Reserved1 */
                    0,              /* Reserved2 */
                    0,              /* Reserved3 */
                    "0.23",         /* Gnd EntityId - 2 byte dotted-decimal string eg. "0.255"*/
                    "/ram/downlink/ch1/dir7/", /* SrcPathname */
                    "ch1/dir7/",    /* DstPathname */
                }, /* End Polling Directory 7 */                
                
            }, /* End Polling Directory Array */
        
        },  /* End Playback Channel #1 */

    },  /* End Playback Channel Array */

}; /* End CF_ConfigTable */   
    

/************************/
/*  End of File Comment */
/************************/
