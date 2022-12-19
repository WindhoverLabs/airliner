/****************************************************************************
*
*   Copyright (c) 2017 Windhover Labs, L.L.C. All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
*
* 1. Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in
*    the documentation and/or other materials provided with the
*    distribution.
* 3. Neither the name Windhover Labs nor the names of its
*    contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
* COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
* OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
* AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
* ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*
*****************************************************************************/

#include "cf_test_utils.h"
#include "cf_custom_hooks.h"

#include "to_platform_cfg.h"
#include "sbnd_platform_cfg.h"
#include "structures.h"
#include "timer.h"
#include "machine.h"
#include "misc.h"
#include "cf_playback.h"
#include "cf_callbacks.h"

#include "ut_cfe_es_stubs.h"
#include "ut_cfe_evs_stubs.h"
#include "ut_cfe_evs_hooks.h"
#include "ut_cfe_sb_stubs.h"
#include "ut_cfe_tbl_stubs.h"
#include "ut_cfe_tbl_hooks.h"
#include "ut_cfe_time_stubs.h"
#include "ut_cfe_psp_memutils_stubs.h"
#include "ut_cfe_psp_timer_stubs.h"
#include "ut_cfe_fs_stubs.h"
#include "ut_osapi_stubs.h"
#include "ut_osfileapi_stubs.h"


cf_config_table_t  CF_ConfigTableUnitTest =
{
    "CF Default Table",/* TableIdString            */
    2,                 /* TableVersion (integer)   */
    3,                 /* NumEngCyclesPerWakeup    */
    1,                 /* NumWakeupsPerQueueChk    */
    1,                 /* NumWakeupsPerPollDirChk  */
    100,               /* UplinkHistoryQDepth      */
    0,                 /* Reserved1                */
    0,                 /* Reserved2                */
    "10",              /* AckTimeout (secs, entered as string) */
    "2",               /* AckLimit (max timeouts, string)      */
    "5",               /* NakTimeout (secs, string)            */
    "3",               /* NakLimit (max timeouts, string)      */
    "20",              /* InactivityTimeout (secs, string)     */
    "200",             /* OutgoingFileChunkSize (bytes, string)*/
    "no",              /* SaveIncompleteFiles (yes,no, string) */
    "0.3",             /* Flight EntityId - 2 byte dotted-decimal string eg. "0.255"*/

    { /* Input Channel Array */

        { /* Input Channel 0 */
            CF_PPD_TO_CPD_PDU_MID,
            0, /* Output Chan for Class 2 Uplink Responses, ACK-EOF,Nak,Fin etc) */
            0  /* spare */
        }, /* end Input Channel 0 */

        { /* Input Channel 1 */
            CF_GND_TO_CPD_PDU_MID,
            1, /* Output Chan for Class 2 Uplink Responses, ACK-EOF,Nak,Fin etc) */
            0  /* spare */
        } /* end Input Channel 1 */

    }, /* end Input Channel Array */

    { /* Playback Channel Array */

        { /* Playback Channel #0 */
            CF_ENTRY_IN_USE,          /* Playback Channel Entry In Use */
            CF_ENABLED,               /* Dequeue Enable */
            CF_CPD_TO_PPD_PDU_MID,    /* Space To Gnd PDU MsgId */
            100,                      /* Pending Queue Depth */
            100,                      /* History Queue Depth */
            "PPD",                    /* Playback Channel Name   */
            SBND_CF_THROTTLE_SEM_NAME,/* Handshake Semaphore Name   */

            {  /* Polling Directory Array */

                { /* Polling Directory 0 */
                    CF_ENTRY_IN_USE,/* Poll Directory In Use or Not */
                    CF_ENABLED,     /* Enable State */
                    1,              /* Class (1 or 2)*/
                    2,              /* Priority */
                    CF_KEEP_FILE,   /* Preserve files after successful transfer? */
                    0,              /* Reserved1 */
                    0,              /* Reserved2 */
                    0,              /* Reserved3 */
                    "0.2",          /* Gnd EntityId - 2 byte dotted-decimal string eg. "0.255"*/
                    "/ram/downlink/cpd_to_gnd/class1/priority2/", /* SrcPath, no spaces, fwd slash at end */
                    "/cf/downlink/cpd_to_gnd/class1/priority2/",  /* DstPath, no spaces */
                }, /* End Polling Directory 0 */

                { /* Polling Directory 1 */
                    CF_ENTRY_IN_USE,/* Poll Directory In Use or Not */
                    CF_ENABLED,     /* Enable State */
                    2,              /* Class (1 or 2)*/
                    2,              /* Priority */
                    CF_DELETE_FILE, /* Preserve files after successful transfer? */
                    0,              /* Reserved1 */
                    0,              /* Reserved2 */
                    0,              /* Reserved3 */
                    "0.2",          /* Gnd EntityId - 2 byte dotted-decimal string eg. "0.255"*/
                    "/ram/downlink/cpd_to_gnd/class2/priority2/", /* SrcPath, no spaces, fwd slash at end */
                    "/cf/downlink/cpd_to_gnd/class2/priority2/",  /* DstPath, no spaces */
                }, /* End Polling Directory 1 */

                { /* Polling Directory 2 */
                    CF_ENTRY_IN_USE,/* Poll Directory In Use or Not */
                    CF_ENABLED,     /* Enable State */
                    1,              /* Class (1 or 2)*/
                    4,              /* Priority */
                    CF_KEEP_FILE,   /* Preserve files after successful transfer? */
                    0,              /* Reserved1 */
                    0,              /* Reserved2 */
                    0,              /* Reserved3 */
                    "0.2",          /* Gnd EntityId - 2 byte dotted-decimal string eg. "0.255"*/
                    "/ram/downlink/cpd_to_gnd/class1/priority4/", /* SrcPath, no spaces, fwd slash at end */
                    "/cf/downlink/cpd_to_gnd/class1/priority4/",  /* DstPath, no spaces */
                }, /* End Polling Directory 2 */

                { /* Polling Directory 3 */
                    CF_ENTRY_IN_USE,/* Poll Directory In Use or Not */
                    CF_ENABLED,     /* Enable State */
                    2,              /* Class (1 or 2)*/
                    4,              /* Priority */
                    CF_DELETE_FILE, /* Preserve files after successful transfer? */
                    0,              /* Reserved1 */
                    0,              /* Reserved2 */
                    0,              /* Reserved3 */
                    "0.2",          /* Gnd EntityId - 2 byte dotted-decimal string eg. "0.255"*/
                    "/ram/downlink/cpd_to_gnd/class2/priority4/", /* SrcPath, no spaces, fwd slash at end */
                    "/cf/downlink/cpd_to_gnd/class2/priority4/",  /* DstPath, no spaces */
                }, /* End Polling Directory 3 */

                { /* Polling Directory 4 */
                    CF_ENTRY_IN_USE,/* Poll Directory In Use or Not */
                    CF_ENABLED,     /* Enable State */
                    1,              /* Class (1 or 2)*/
                    6,              /* Priority */
                    CF_KEEP_FILE,   /* Preserve files after successful transfer? */
                    0,              /* Reserved1 */
                    0,              /* Reserved2 */
                    0,              /* Reserved3 */
                    "0.2",          /* Gnd EntityId - 2 byte dotted-decimal string eg. "0.255"*/
                    "/ram/downlink/cpd_to_gnd/class1/priority6/", /* SrcPath, no spaces, fwd slash at end */
                    "/cf/downlink/cpd_to_gnd/class1/priority6/",  /* DstPath, no spaces */
                }, /* End Polling Directory 4 */

                { /* Polling Directory 5 */
                    CF_ENTRY_IN_USE,/* Poll Directory In Use or Not */
                    CF_ENABLED,     /* Enable State */
                    2,              /* Class (1 or 2)*/
                    6,              /* Priority */
                    CF_DELETE_FILE, /* Preserve files after successful transfer? */
                    0,              /* Reserved1 */
                    0,              /* Reserved2 */
                    0,              /* Reserved3 */
                    "0.2",          /* Gnd EntityId - 2 byte dotted-decimal string eg. "0.255"*/
                    "/ram/downlink/cpd_to_gnd/class2/priority6/", /* SrcPath, no spaces, fwd slash at end */
                    "/cf/downlink/cpd_to_gnd/class2/priority6/",  /* DstPath, no spaces */
                } /* End Polling Directory 5 */

            } /* End Polling Directory Array */

        }, /* End Playback Channel #0 */

        { /* Playback Channel #1 */
            CF_ENTRY_IN_USE,           /* Playback Channel Entry In Use */
            CF_DISABLED,               /* Dequeue Enable */
            CF_CPD_TO_GND_PDU_MID,     /* Space To Gnd PDU MsgId */
            100,                       /* Pending Queue Depth */
            100,                       /* History Queue Depth */
            "CPD",                     /* Playback Channel Name   */
            TO_CF_THROTTLE_SEM_NAME,   /* Handshake Semaphore Name   */

            { /* Polling Directory Array */

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
                    "/ram/downlink/ppd_to_gnd/class1/priority2/", /* SrcPath, no spaces, fwd slash at end */
                    "/ppd/",                                      /* DstPath, no spaces */
                }, /* End Polling Directory 0 */

                { /* Polling Directory 1 */
                    CF_ENTRY_IN_USE,/* Poll Directory In Use or Not */
                    CF_ENABLED,     /* Enable State */
                    2,              /* Class (1 or 2)*/
                    2,              /* Priority */
                    CF_DELETE_FILE, /* Preserve files after successful transfer? */
                    0,              /* Reserved1 */
                    0,              /* Reserved2 */
                    0,              /* Reserved3 */
                    "0.1",          /* Gnd EntityId - 2 byte dotted-decimal string eg. "0.255"*/
                    "/ram/downlink/ppd_to_gnd/class2/priority2/", /* SrcPath, no spaces, fwd slash at end */
                    "/ppd/",                                      /* DstPath, no spaces */
                }, /* End Polling Directory 1 */

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
                    "/ram/downlink/ppd_to_gnd/class1/priority4/", /* SrcPath, no spaces, fwd slash at end */
                    "/ppd/",                                      /* DstPath, no spaces */
                }, /* End Polling Directory 2 */

                { /* Polling Directory 3 */
                    CF_ENTRY_IN_USE,/* Poll Directory In Use or Not */
                    CF_ENABLED,     /* Enable State */
                    2,              /* Class (1 or 2)*/
                    4,              /* Priority */
                    CF_DELETE_FILE, /* Preserve files after successful transfer? */
                    0,              /* Reserved1 */
                    0,              /* Reserved2 */
                    0,              /* Reserved3 */
                    "0.1",          /* Gnd EntityId - 2 byte dotted-decimal string eg. "0.255"*/
                    "/ram/downlink/ppd_to_gnd/class2/priority4/", /* SrcPath, no spaces, fwd slash at end */
                    "/ppd/",                                      /* DstPath, no spaces */
                }, /* End Polling Directory 3 */

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
                    "/ram/downlink/ppd_to_gnd/class1/priority6/", /* SrcPath, no spaces, fwd slash at end */
                    "/ppd/",                                      /* DstPath, no spaces */
                }, /* End Polling Directory 4 */

                { /* Polling Directory 5 */
                    CF_ENTRY_IN_USE,/* Poll Directory In Use or Not */
                    CF_ENABLED,     /* Enable State */
                    2,              /* Class (1 or 2)*/
                    6,              /* Priority */
                    CF_DELETE_FILE, /* Preserve files after successful transfer? */
                    0,              /* Reserved1 */
                    0,              /* Reserved2 */
                    0,              /* Reserved3 */
                    "0.1",          /* Gnd EntityId - 2 byte dotted-decimal string eg. "0.255"*/
                    "/ram/downlink/ppd_to_gnd/class2/priority6/", /* SrcPath, no spaces, fwd slash at end */
                    "/ppd/",                                      /* DstPath, no spaces */
                }, /* End Polling Directory 5 */

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
                    "/ram/downlink/cpd_to_gnd/class1/priority2/", /* SrcPath, no spaces, fwd slash at end */
                    "/cpd/",                                      /* DstPath, no spaces */
                }, /* End Polling Directory 6 */

                { /* Polling Directory 7 */
                    CF_ENTRY_IN_USE,/* Poll Directory In Use or Not */
                    CF_ENABLED,     /* Enable State */
                    2,              /* Class (1 or 2)*/
                    2,              /* Priority */
                    CF_DELETE_FILE, /* Preserve files after successful transfer? */
                    0,              /* Reserved1 */
                    0,              /* Reserved2 */
                    0,              /* Reserved3 */
                    "0.1",         /* Gnd EntityId - 2 byte dotted-decimal string eg. "0.255"*/
                    "/ram/downlink/cpd_to_gnd/class2/priority2/", /* SrcPath, no spaces, fwd slash at end */
                    "/cpd/",                                      /* DstPath, no spaces */
                }, /* End Polling Directory 7 */

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
                    "/ram/downlink/cpd_to_gnd/class1/priority4/", /* SrcPath, no spaces, fwd slash at end */
                    "/cpd/",                                      /* DstPath, no spaces */
                }, /* End Polling Directory 8 */

                { /* Polling Directory 9 */
                    CF_ENTRY_IN_USE,/* Poll Directory In Use or Not */
                    CF_ENABLED,     /* Enable State */
                    2,              /* Class (1 or 2)*/
                    4,              /* Priority */
                    CF_DELETE_FILE, /* Preserve files after successful transfer? */
                    0,              /* Reserved1 */
                    0,              /* Reserved2 */
                    0,              /* Reserved3 */
                    "0.1",          /* Gnd EntityId - 2 byte dotted-decimal string eg. "0.255"*/
                    "/ram/downlink/cpd_to_gnd/class2/priority4/", /* SrcPath, no spaces, fwd slash at end */
                    "/cpd/",                                      /* DstPath, no spaces */
                }, /* End Polling Directory 9 */

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
                    "/ram/downlink/cpd_to_gnd/class1/priority6/", /* SrcPath, no spaces, fwd slash at end */
                    "/cpd/",                                      /* DstPath, no spaces */
                }, /* End Polling Directory 10 */

                { /* Polling Directory 11 */
                    CF_ENTRY_IN_USE,/* Poll Directory In Use or Not */
                    CF_ENABLED,     /* Enable State */
                    2,              /* Class (1 or 2)*/
                    6,              /* Priority */
                    CF_DELETE_FILE, /* Preserve files after successful transfer? */
                    0,              /* Reserved1 */
                    0,              /* Reserved2 */
                    0,              /* Reserved3 */
                    "0.1",          /* Gnd EntityId - 2 byte dotted-decimal string eg. "0.255"*/
                    "/ram/downlink/cpd_to_gnd/class2/priority6/", /* SrcPath, no spaces, fwd slash at end */
                    "/cpd/",                                      /* DstPath, no spaces */
                } /* End Polling Directory 11 */

            } /* End Polling Directory Array */

        } /* End Playback Channel #1 */

    } /* End Playback Channel Array */

}; /* End CF_ConfigTableUnitTest */


void CF_Test_Setup(void)
{
    /* initialize test environment to default state for every test */
    CFE_PSP_MemSet(&CF_AppData, 0x00, sizeof(CF_AppData));

    CFE_ES_GetPoolBufHookCallCnt = 0;

    Ut_CFE_ES_Reset();
    Ut_CFE_EVS_Reset();
    Ut_CFE_FS_Reset();
    Ut_CFE_SB_Reset();
    Ut_CFE_TBL_Reset();
    Ut_CFE_TIME_Reset();
    Ut_OSAPI_Reset();
    Ut_OSFILEAPI_Reset();

#if 1
    Ut_CFE_TBL_AddTable(CF_CONFIG_TABLE_FILENAME, (void*)&CF_ConfigTable);
#else
    Ut_CFE_TBL_AddTable(CF_CONFIG_TABLE_FILENAME, (void*)&CF_ConfigTableUnitTest);
#endif
}


void CF_Test_TearDown(void)
{
}


int32 CF_TstUtil_VerifyListOrder(char *OrderGiven)
{
    CF_QueueEntry_t   *PtrToEntry;
    char              Buf[64];
    uint32            i=0;

    PtrToEntry = CF_AppData.Chan[0].PbQ[CF_PB_PENDINGQ].HeadPtr;

    while(PtrToEntry != NULL)
    {
        sprintf(&Buf[i],"%d",(int)PtrToEntry->TransNum);
        PtrToEntry = PtrToEntry->Next;
        i++;
    }

    if(strncmp(OrderGiven,Buf,64)==0)
    {
        return CF_SUCCESS;
    }
    else
    {
        printf("VerfiyList is comparing given %s with %s\n",
               OrderGiven,Buf);

        return CF_ERROR;
    }
}


void CF_TstUtil_CreateOnePendingQueueEntry(void)
{
    CF_PlaybackFileCmd_t  PbFileCmdMsg;

    /* reset CF globals etc */
    CF_AppInit();

    /* reset the transactions seq number used by the engine */
    misc__set_trans_seq_num(1);

    /* Execute a playback file command so that one queue entry is added
       to the pending queue */
    CFE_SB_InitMsg((void*)&PbFileCmdMsg, CF_CMD_MID,
                   sizeof(CF_PlaybackFileCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&PbFileCmdMsg, CF_PLAYBACK_FILE_CC);
    PbFileCmdMsg.Class = 1;
    PbFileCmdMsg.Channel = 0;
    PbFileCmdMsg.Priority = 0;
    PbFileCmdMsg.Preserve = 0;
    strcpy(PbFileCmdMsg.PeerEntityId, "2.25");
    strcpy(PbFileCmdMsg.SrcFilename, "/cf/testfile.txt");
    strcpy(PbFileCmdMsg.DstFilename, "gndpath/");

    /* force the GetPoolBuf call for the queue entry to return
       something valid */
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX,
                              (void*)&CFE_ES_GetPoolBufHook);

    /* execute the playback file cmd to get a queue entry on the
       pending queue */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&PbFileCmdMsg;
    CF_AppPipe((CFE_SB_MsgPtr_t)&PbFileCmdMsg);
}


void CF_TstUtil_CreateOnePbActiveQueueEntry(void)
{
    CF_TstUtil_CreateOnePendingQueueEntry();

    /* Force OS_stat to return a valid size and success */
    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_STAT_INDEX,
                                 (void*)&OS_statHook);

    CF_StartNextFile(0);
}


void CF_TstUtil_CreateOnePbHistoryQueueEntry(void)
{
    CF_CARSCmd_t    CmdMsg;

    /* Setup Inputs */
    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CF_CARSCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg, CF_ABANDON_CC);
    strcpy(CmdMsg.Trans,"All");

    CF_TstUtil_CreateOnePbActiveQueueEntry();

    cfdp_cycle_each_transaction();

    /* Send Abandon Cmd */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe((CFE_SB_MsgPtr_t)&CmdMsg);

    cfdp_cycle_each_transaction();

    cfdp_cycle_each_transaction();
}


void CF_TstUtil_CreateOneUpActiveQueueEntry(void)
{
    INDICATION_TYPE IndType = IND_MACHINE_ALLOCATED;
    TRANS_STATUS    TransInfo;

    /* reset CF globals etc */
    CF_AppInit();

    TransInfo.role =  CLASS_1_RECEIVER;
    TransInfo.trans.number = 500;
    TransInfo.trans.source_id.value[0] = 0;
    TransInfo.trans.source_id.value[1] = 23;

    /* force the GetPoolBuf call for the queue entry to return
       something valid */
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX,
                              (void*)&CFE_ES_GetPoolBufHook);

    CF_Indication (IndType,TransInfo);
}


void CF_TstUtil_CreateOneUpHistoryQueueEntry(void)
{
    INDICATION_TYPE IndType = IND_MACHINE_DEALLOCATED;
    TRANS_STATUS    TransInfo;

    CF_TstUtil_CreateOneUpActiveQueueEntry();

    TransInfo.role =  CLASS_1_RECEIVER;
    TransInfo.trans.number = 500;
    TransInfo.trans.source_id.value[0] = 0;
    TransInfo.trans.source_id.value[1] = 23;
    TransInfo.final_status = FINAL_STATUS_SUCCESSFUL;
    strcpy(TransInfo.md.dest_file_name,"/ram/uploadedfile.txt");

    CF_Indication (IndType,TransInfo);
}


void CF_ResetEngine(void)
{

    CF_CARSCmd_t  CmdMsg;

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CF_CARSCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg, CF_ABANDON_CC);
    strcpy(CmdMsg.Trans,"All");

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe((CFE_SB_MsgPtr_t)&CmdMsg);

    cfdp_cycle_each_transaction();

    misc__set_trans_seq_num(1);
}


void CF_Test_PrintCmdMsg(void *pMsg, uint32 size)
{
    unsigned char *pBuff;
    int           i = 0;

    pBuff = (unsigned char*)pMsg;
    printf("Emulated Cmd message:");
    for (i = 0; i < size; i++)
    {
        printf("0x%02x ", *pBuff);
        pBuff++;
    }
    printf("\n");

    return;
}


time_t CF_Test_GetTimeFromMsg(CFE_TIME_SysTime_t cfe_time)
{
    time_t   local_time;

    local_time = (time_t)cfe_time.Seconds;

    return local_time;
}
