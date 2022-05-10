#include "cfe.h"
#include "mfa_tbldefs.h"
#include "mfa_events.h"
#include "mfa_app.h"
#include "mfa_msgids.h"
#include "mfa_platform_cfg.h"
#include "mfa_mission_cfg.h"

#define MFA_CONFIG_TABLENAME       "CONFIG_TBL"

MFA_APPData_t MFA_AppData;

uint32 MFA_InitEvents(void)
{
    int32 status;
    uint32 exitStatus = CFE_ES_APP_RUN;
    /* Register the table with CFE */
    status = CFE_EVS_Register(0, 0, CFE_EVS_BINARY_FILTER);
    if(status != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("MFA - Failed to register with EVS (0x%08X)\n", (unsigned int)status);
        exitStatus = CFE_ES_APP_ERROR;
    }

    return exitStatus;
}

uint32 MFA_InitPipes()
{
    uint32 exitStatus = CFE_ES_APP_RUN;
    /* Init schedule pipe and subscribe to wakeup messages */
    int32 status = CFE_SB_CreatePipe(&MFA_AppData.SchPipeId,
                                 MFA_SCH_PIPE_DEPTH,
                                 MFA_SCH_PIPE_NAME);
    if (CFE_SUCCESS != status)
    {
        (void) CFE_EVS_SendEvent(MFA_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to create SCH PIPE (0x%08X)",
                                 (unsigned int)status);
        exitStatus = CFE_ES_APP_ERROR;
    }
    else
    {
        status = CFE_SB_SubscribeEx(MFA_WAKEUP_MID,
                                    MFA_AppData.SchPipeId,
                                    CFE_SB_Default_Qos,
                                    MFA_SCH_PIPE_WAKEUP_RESERVED);

        if (status != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(MFA_INIT_ERR_EID,
                                     CFE_EVS_ERROR,
                                     "Sch Pipe failed to subscribe to MFA_WAKEUP_MID. (0x%08X)",
                                     (unsigned int)status);
            exitStatus = CFE_ES_APP_ERROR;
        }


        status = CFE_SB_SubscribeEx(MFA_SEND_HK_MID,
                                    MFA_AppData.SchPipeId,
                                    CFE_SB_Default_Qos,
                                    MFA_SCH_PIPE_WAKEUP_RESERVED);

        if (status != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(MFA_INIT_ERR_EID,
                                     CFE_EVS_ERROR,
                                     "Sch Pipe failed to subscribe to MFA_SEND_HK_MID. (0x%08X)",
                                     (unsigned int)status);
            exitStatus = CFE_ES_APP_ERROR;
        }
    }

    return exitStatus;
}
uint32 MFA_InitData(void)
{
    int32 status;
    uint32 exitStatus = CFE_ES_APP_RUN;

    CFE_SB_InitMsg(&MFA_AppData.HkTlm,
                   MFA_HK_TLM_MID,
                   sizeof(MFA_AppData.HkTlm),
                   TRUE);

    return exitStatus;
}

uint32 MFA_InitTables()
{
    int32 status;
    uint32 exitStatus = CFE_ES_APP_RUN;
    /* Register Config table */
    status = CFE_TBL_Register(&MFA_AppData.ConfigTblHdl,
                               MFA_CONFIG_TABLENAME,
                               sizeof(MFA_ConfigTbl_t),
                               CFE_TBL_OPT_DEFAULT,
                               0);
    if(status != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(MFA_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to register Config table (0x%08X)",
                                 (unsigned int)status);
        exitStatus = CFE_ES_APP_ERROR;
    }

    /* Load Config table file */
    status = CFE_TBL_Load(MFA_AppData.ConfigTblHdl,
                          CFE_TBL_SRC_FILE,
                                                  MFA_CONFIG_TABLE_FILENAME);
    if(status != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(MFA_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to load Config Table (0x%08X)",
                                 (unsigned int)status);
        exitStatus = CFE_ES_APP_ERROR;
    }

    /*
    ** Get a pointer to the tables
    */
    status = CFE_TBL_GetAddress((void**)&MFA_AppData.ConfigTblPtr,
                                MFA_AppData.ConfigTblHdl);
    if(status != CFE_TBL_INFO_UPDATED)
    {
        MFA_AppData.ConfigTblPtr = 0;
            (void) CFE_EVS_SendEvent(MFA_INIT_ERR_EID, CFE_EVS_ERROR,
                                     "Failed to get Config table's address (0x%08X)",
                                     (unsigned int)status);
        exitStatus = CFE_ES_APP_ERROR;
    }

    if(CFE_ES_APP_RUN == exitStatus)
    {
        /* Send an event notifying the ground that we initialized. */
        (void) CFE_EVS_SendEvent(MFA_INITIALIZED_EID, CFE_EVS_INFORMATION, "Initialized v%u.%u.%u",
                                 MFA_MAJOR_VERSION,
                                 MFA_MINOR_VERSION,
                                 MFA_PATCH_VERSION);
    }

    return exitStatus;
}

uint32 MFA_AppInit(void)
{
    uint32 exitStatus = CFE_ES_APP_RUN;

    int32 status;
    /* Register our application */
    status = CFE_ES_RegisterApp();
    if(status != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("MFA - Failed to register the app (0x%08X)\n", (unsigned int)status);
        exitStatus = CFE_ES_APP_ERROR;
    }

    /* Register the table with CFE */
    if(CFE_ES_APP_RUN == exitStatus)
    {
      exitStatus = MFA_InitEvents();
    }

    if(status != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("MFA - Failed to register with EVS (0x%08X)\n", (unsigned int)status);
        exitStatus = CFE_ES_APP_ERROR;
    }

    /* Register the table with CFE */
    if(CFE_ES_APP_RUN == exitStatus)
    {
      exitStatus = MFA_InitTables();
    }

    /* Register the table with CFE */
    if(CFE_ES_APP_RUN == exitStatus)
    {
      exitStatus = MFA_InitPipes();
    }

    return exitStatus;
}

void MFA_AppDeInit(void)
{
    (void) CFE_EVS_Unregister();
}

void MFA_AppMain()
{
    uint32 status;
    int32 exitStatus;
    status = MFA_AppInit();
    /* Register the table with CFE */
    if(CFE_ES_APP_RUN == status)
    {
      exitStatus = MFA_InitData();
    }
    /* Enter our main cyclic loop. */
    while(CFE_ES_RunLoop(&exitStatus) == TRUE)
    {
        CFE_SB_MsgId_t* msgPtr = 0;
        CFE_SB_MsgId_t MsgId = 0;
        status = CFE_SB_RcvMsg(&msgPtr, MFA_AppData.SchPipeId, MFA_SCH_PEND_TIME);

        if (status == CFE_SUCCESS)
        {
            MsgId = CFE_SB_GetMsgId(msgPtr);
            switch (MsgId)
            {
                case MFA_WAKEUP_MID:
            {

//                (void) CFE_EVS_SendEvent(MFA_HELLO_WORLD_EID, CFE_EVS_INFORMATION, "%s", MFA_AppData.ConfigTblPtr->Message);
                MFA_AppData.HkTlm.HelloCount++;
                    break;
             }

            case MFA_SEND_HK_MID:
            {

                CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&MFA_AppData.HkTlm);
                CFE_SB_SendMsg((CFE_SB_Msg_t*)&MFA_AppData.HkTlm);
                break;
             }

        }
    }
    }
    MFA_AppDeInit();

    /* Exit the application */
    CFE_ES_ExitApp(exitStatus);
}
