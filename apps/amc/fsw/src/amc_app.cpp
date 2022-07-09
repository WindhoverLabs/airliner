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

/************************************************************************
 ** Pragmas
 *************************************************************************/

/************************************************************************
 ** Includes
 *************************************************************************/

#ifdef __cplusplus
extern "C"
{
#endif

#include <string.h>
#include <errno.h>

#include "cfe.h"

#include "amc_app.h"
#include "amc_msg.h"
#include "amc_version.h"
#include <math.h>
#include "px4lib.h"
#include "px4lib.h"
#include "px4lib_msgids.h"

/* TODO:  Delete this when the PWM is no longer simulated on the PX4 side. */
#define PWM_SIM_DISARMED_MAGIC (900)

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Instantiate the application object.                             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
AMC oAMC;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Default constructor.                                            */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
AMC::AMC(void)
{
	for(uint32 i = 0; i < AMC_MULTIROTOR_MIXER_MAX_MIXERS; ++i)
	{
		MultirotorMixerObject[i].set_callback(AMC::ControlCallback);
	}

	for(uint32 i = 0; i < AMC_SIMPLE_MIXER_MAX_MIXERS; ++i)
	{
		SimpleMixerObject[i].set_callback(AMC::ControlCallback);
	}

    return;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Destructor constructor.                                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
AMC::~AMC()
{
    return;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize event tables.                                        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 AMC::InitEvent(void)
{
    int32 iStatus=CFE_SUCCESS;

    /* Register the table with CFE */
    iStatus = CFE_EVS_Register(0, 0, CFE_EVS_BINARY_FILTER);
    if (iStatus != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog(
                "AMC - Failed to register with EVS (0x%08X)\n",
                (unsigned int)iStatus);
    }

    return iStatus;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize Message Pipes                                        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 AMC::InitPipes(void)
{
    int32 iStatus=CFE_SUCCESS;

    /* Init schedule pipe and subscribe to wakeup messages */
    iStatus = InitSchPipe();
    if (iStatus != CFE_SUCCESS)
    {
        /* We failed to create the SCH pipe for scheduler messages.
         * An event was already raised.  Just abort the function.
         */
        goto AMC_InitPipe_Exit_Tag;

    }

    /* Init command pipe and subscribe to command messages */
    iStatus = InitCmdPipe();
    if (iStatus != CFE_SUCCESS)
    {
        /* We failed to create the CMD pipe for command messages.
         * An event was already raised.  Just abort the function.
         */
        goto AMC_InitPipe_Exit_Tag;

    }

    /* Init data pipe and subscribe to data messages from the other
     * applications. */
    iStatus = InitDataPipe();
    if (iStatus != CFE_SUCCESS)
    {
        /* We failed to create the DATA pipe for data messages.
         * An event was already raised.  Just abort the function.
         */
        goto AMC_InitPipe_Exit_Tag;

    }

AMC_InitPipe_Exit_Tag:
    return iStatus;

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize Message Pipes                                        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 AMC::InitSchPipe(void)
{
    int32 iStatus=CFE_SUCCESS;

    /* First, create all the pipes. */
    /* Init schedule pipe and subscribe to wakeup messages */
    iStatus = CFE_SB_CreatePipe(&SchPipeId,
            AMC_SCH_PIPE_DEPTH,
            AMC_SCH_PIPE_NAME);
    if (iStatus != CFE_SUCCESS)
    {
        /* We failed to create the pipe for scheduler messages.  Raise an
         * event and immediately jump to the end of the function to abort
         * initialization.
         */
        CFE_EVS_SendEvent(AMC_PIPE_INIT_ERR_EID, CFE_EVS_ERROR,
                "Failed to create SCH pipe (0x%08X)",
                (unsigned int)iStatus);

        goto AMC_InitPipe_Exit_Tag;
    }

    /* Subscribe to the AMC_UPDATE_MOTORS_MID message. */
    iStatus = CFE_SB_SubscribeEx(AMC_UPDATE_MOTORS_MID, SchPipeId,
            CFE_SB_Default_Qos, AMC_SCH_PIPE_WAKEUP_RESERVED);
    if (iStatus != CFE_SUCCESS)
    {
        /* The subscribe failed.  Raise an event and immediately jump
         * to the end of the function to abort initialization.
         */
        CFE_EVS_SendEvent(AMC_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                "Sch Pipe failed to subscribe to AMC_UPDATE_MOTORS_MID. \
                (0x%08X)", (unsigned int)iStatus);

        goto AMC_InitPipe_Exit_Tag;
    }

    /* Subscribe to the AMC_SEND_HK_MID message. */
    iStatus = CFE_SB_SubscribeEx(AMC_SEND_HK_MID, SchPipeId,
            CFE_SB_Default_Qos, AMC_SCH_PIPE_SEND_HK_RESERVED);
    if (iStatus != CFE_SUCCESS)
    {
        /* The subscribe failed.  Raise an event and immediately jump
         * to the end of the function to abort initialization.
         */
        CFE_EVS_SendEvent(AMC_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                "SCH Pipe failed to subscribe to AMC_SEND_HK_MID. \
                (0x%08X)",
                (unsigned int)iStatus);

        goto AMC_InitPipe_Exit_Tag;
    }

AMC_InitPipe_Exit_Tag:
    return iStatus;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize Message Pipes                                        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 AMC::InitCmdPipe(void)
{
    int32 iStatus=CFE_SUCCESS;

    /* Init command pipe and subscribe to command messages */
    iStatus = CFE_SB_CreatePipe(&CmdPipeId,
            AMC_CMD_PIPE_DEPTH,
            AMC_CMD_PIPE_NAME);
    if (iStatus != CFE_SUCCESS)
    {
        /* We failed to create the pipe for command messages.  Raise an
         * event and immediately jump to the end of the function to abort
         * initialization.
         */
        CFE_EVS_SendEvent(AMC_PIPE_INIT_ERR_EID, CFE_EVS_ERROR,
                "Failed to create CMD pipe (0x%08X)",
                (unsigned int)iStatus);

        goto AMC_InitPipe_Exit_Tag;
    }

    /* Subscribe to the AMC_CMD_MID message to receive commands. */
    iStatus = CFE_SB_SubscribeEx(AMC_CMD_MID, CmdPipeId,
            CFE_SB_Default_Qos, 1);
    if (iStatus != CFE_SUCCESS)
    {
        /* We failed to create the pipe for command messages.  Raise an
         * event and immediately jump to the end of the function to abort
         * initialization.
         */
        CFE_EVS_SendEvent(AMC_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                "CMD Pipe failed to subscribe to AMC_CMD_MID. (0x%08X)",
                (unsigned int)iStatus);

        goto AMC_InitPipe_Exit_Tag;
    }

AMC_InitPipe_Exit_Tag:
    return iStatus;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize Message Pipes                                        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 AMC::InitDataPipe(void)
{
    int32 iStatus=CFE_SUCCESS;

    /* Init command pipe and subscribe to command messages */
    iStatus = CFE_SB_CreatePipe(&DataPipeId,
            AMC_DATA_PIPE_DEPTH,
            AMC_DATA_PIPE_NAME);
    if (iStatus != CFE_SUCCESS)
    {
        /* We failed to create the pipe for command messages.  Raise an
         * event and immediately jump to the end of the function to abort
         * initialization.
         */
        CFE_EVS_SendEvent(AMC_PIPE_INIT_ERR_EID, CFE_EVS_ERROR,
                "Failed to create DATA pipe (0x%08X)",
                (unsigned int)iStatus);

        goto AMC_InitPipe_Exit_Tag;
    }

    /* Subscribe to the PX4_ACTUATOR_ARMED_MID message. */
    iStatus = CFE_SB_SubscribeEx(PX4_ACTUATOR_ARMED_MID, DataPipeId,
            CFE_SB_Default_Qos, 1);
    if (iStatus != CFE_SUCCESS)
    {
        /* The subscribe failed.  Raise an event and immediately jump
         * to the end of the function to abort initialization.
         */
        CFE_EVS_SendEvent(AMC_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                "DATA Pipe failed to subscribe to PX4_ACTUATOR_ARMED_MID. \
                (0x%08X)",
                (unsigned int)iStatus);

        goto AMC_InitPipe_Exit_Tag;
    }

    /* Subscribe to the PX4_ACTUATOR_CONTROLS_0_MID message. */
    iStatus = CFE_SB_SubscribeEx(PX4_ACTUATOR_CONTROLS_0_MID, DataPipeId,
            CFE_SB_Default_Qos, 1);
    if (iStatus != CFE_SUCCESS)
    {
        /* The subscribe failed.  Raise an event and immediately jump
         * to the end of the function to abort initialization.
         */
        CFE_EVS_SendEvent(AMC_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                "DATA Pipe failed to subscribe to \
                PX4_ACTUATOR_CONTROLS_0_MID. (0x%08X)",
                (unsigned int)iStatus);

        goto AMC_InitPipe_Exit_Tag;
    }

    /* Subscribe to the PX4_ACTUATOR_CONTROLS_1_MID message. */
    iStatus = CFE_SB_SubscribeEx(PX4_ACTUATOR_CONTROLS_1_MID, DataPipeId,
            CFE_SB_Default_Qos, 1);
    if (iStatus != CFE_SUCCESS)
    {
        /* The subscribe failed.  Raise an event and immediately jump
         * to the end of the function to abort initialization.
         */
        CFE_EVS_SendEvent(AMC_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                "DATA Pipe failed to subscribe to \
                PX4_ACTUATOR_CONTROLS_1_MID. (0x%08X)",
                (unsigned int)iStatus);

        goto AMC_InitPipe_Exit_Tag;
    }

    /* Subscribe to the PX4_ACTUATOR_CONTROLS_2_MID message. */
    iStatus = CFE_SB_SubscribeEx(PX4_ACTUATOR_CONTROLS_2_MID, DataPipeId,
            CFE_SB_Default_Qos, 1);
    if (iStatus != CFE_SUCCESS)
    {
        /* The subscribe failed.  Raise an event and immediately jump
         * to the end of the function to abort initialization.
         */
        CFE_EVS_SendEvent(AMC_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                "DATA Pipe failed to subscribe to \
                PX4_ACTUATOR_CONTROLS_2_MID. (0x%08X)",
                (unsigned int)iStatus);

        goto AMC_InitPipe_Exit_Tag;
    }

    /* Subscribe to the PX4_ACTUATOR_CONTROLS_3_MID message. */
    iStatus = CFE_SB_SubscribeEx(PX4_ACTUATOR_CONTROLS_3_MID, DataPipeId,
            CFE_SB_Default_Qos, 1);
    if (iStatus != CFE_SUCCESS)
    {
        /* The subscribe failed.  Raise an event and immediately jump
         * to the end of the function to abort initialization.
         */
        CFE_EVS_SendEvent(AMC_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                "DATA Pipe failed to subscribe to \
                PX4_ACTUATOR_CONTROLS_3_MID. (0x%08X)",
                (unsigned int)iStatus);

        goto AMC_InitPipe_Exit_Tag;
    }

AMC_InitPipe_Exit_Tag:
    return iStatus;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize Global Variables                                     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void AMC::InitData(void)
{
    /* Init actuator outputs message */
    CFE_SB_InitMsg(&ActuatorOutputs,
            PX4_ACTUATOR_OUTPUTS_MID, sizeof(ActuatorOutputs), TRUE);

    /* Init housekeeping message. */
    CFE_SB_InitMsg(&HkTlm,
            AMC_HK_TLM_MID, sizeof(HkTlm), TRUE);

    CFE_PSP_MemSet(&CVT.ActuatorArmed, 0, sizeof(CVT.ActuatorArmed));
    CFE_PSP_MemSet(&CVT.ActuatorControls0, 0, sizeof(CVT.ActuatorControls0));
    CFE_PSP_MemSet(&CVT.ActuatorControls1, 0, sizeof(CVT.ActuatorControls1));
    CFE_PSP_MemSet(&CVT.ActuatorControls2, 0, sizeof(CVT.ActuatorControls2));
    CFE_PSP_MemSet(&CVT.ActuatorControls3, 0, sizeof(CVT.ActuatorControls3));
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* AMC initialization                                              */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 AMC::InitApp(void)
{
    int32 iStatus = CFE_SUCCESS;

    /* Initialize the application to use CFE Events. */
    iStatus = InitEvent();
    if (iStatus != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("AMC - Failed to init events (0x%08X)\n",
                (unsigned int)iStatus);
        goto AMC_InitApp_Exit_Tag;
    }

    /* Initialize the application to receive messages. */
    iStatus = InitPipes();
    if (iStatus != CFE_SUCCESS)
    {
        goto AMC_InitApp_Exit_Tag;
    }

    /* Initialize all internal data. */
    InitData();

    /* Initialize the application to use tables. */
    iStatus = InitConfigTbl();
    if (iStatus != CFE_SUCCESS)
    {
        goto AMC_InitApp_Exit_Tag;
    }

    /* Initialize the Multirotor Mixer object. */
	for(uint32 i = 0; i < AMC_MULTIROTOR_MIXER_MAX_MIXERS; ++i)
	{
	    iStatus = MultirotorMixerObject[i].Initialize();
	    if (iStatus != CFE_SUCCESS)
	    {
	        CFE_EVS_SendEvent(AMC_MIXER_INIT_ERR_EID, CFE_EVS_ERROR,
	                "Failed to init Multirotor mixer (0x%08x)",
	                (unsigned int)iStatus);
	        goto AMC_InitApp_Exit_Tag;
	    }
	}

	for(uint32 i = 0; i < AMC_SIMPLE_MIXER_MAX_MIXERS; ++i)
	{
	    /* Initialize the Simple mixer object. */
	    iStatus = SimpleMixerObject[i].Initialize();
	    if (iStatus != CFE_SUCCESS)
	    {
	        CFE_EVS_SendEvent(AMC_MIXER_INIT_ERR_EID, CFE_EVS_ERROR,
	                "Failed to init Simple mixer (0x%08x)",
	                (unsigned int)iStatus);
	        goto AMC_InitApp_Exit_Tag;
	    }
	}

    /* Initialize the PwmLimit object for use. */
    PwmLimit_Init(&PwmLimit);

    /* Register the cleanup callback */
    iStatus = OS_TaskInstallDeleteHandler(CleanupCallback);
    if (iStatus != CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(AMC_DELETE_CB_REG_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to init register cleanup callback (0x%08X)",
                                 (unsigned int)iStatus);
        goto AMC_InitApp_Exit_Tag;
    }

    /* Initialize the hardware device for use. */
    iStatus = InitDevice();
    if (iStatus != CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(AMC_DEVICE_INIT_ERR_EID, CFE_EVS_ERROR,
                "Failed to init device (0x%08x)",
                (unsigned int)iStatus);
        goto AMC_InitApp_Exit_Tag;
    }

AMC_InitApp_Exit_Tag:
    if (iStatus == CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(AMC_INIT_INF_EID, CFE_EVS_INFORMATION,
                "Initialized.  Version %d.%d.%d.%d",
                AMC_MAJOR_VERSION,
                AMC_MINOR_VERSION,
                AMC_REVISION,
                AMC_MISSION_REV);
    }

    return iStatus;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Cleanup prior to exit                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void AMC::CleanupCallback()
{
    oAMC.StopMotors();
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Receive and Process Messages                                    */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 AMC::RcvSchPipeMsg(int32 iBlocking)
{
    int32 iStatus=CFE_SUCCESS;
    CFE_SB_Msg_t* MsgPtr=NULL;
    CFE_SB_MsgId_t MsgId;

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(AMC_MAIN_TASK_PERF_ID);

    /* Wait for WakeUp messages from scheduler */
    iStatus = CFE_SB_RcvMsg(&MsgPtr, SchPipeId, iBlocking);

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(AMC_MAIN_TASK_PERF_ID);

    if (iStatus == CFE_SUCCESS)
    {
        MsgId = CFE_SB_GetMsgId(MsgPtr);
        switch (MsgId)
        {
            case AMC_UPDATE_MOTORS_MID:
            {
                ProcessDataPipe();
                break;
            }

            case AMC_SEND_HK_MID:
            {
                ReportHousekeeping();
                // TODO: Move these somewhere more appropriate later
                ProcessCmdPipe();
                break;
            }

            default:
            {
                CFE_EVS_SendEvent(AMC_MSGID_ERR_EID, CFE_EVS_ERROR,
                        "Recvd invalid SCH msgId (0x%04X)",
                        (unsigned short)MsgId);
            }
        }
    }
    else if (iStatus == CFE_SB_TIME_OUT)
    {
        /* TODO: If there's no incoming message within a specified time (via the
         * iBlocking arg, you can do something here, or nothing.  
         * Note, this section is dead code only if the iBlocking arg
         * is CFE_SB_PEND_FOREVER. */
        UpdateMotors();
        iStatus = CFE_SUCCESS;
    }
    else
    {
        CFE_EVS_SendEvent(AMC_RCVMSG_ERR_EID, CFE_EVS_ERROR,
                "SCH pipe read error (0x%08X).", (unsigned int)iStatus);
    }

    return iStatus;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process Incoming Commands                                       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void AMC::ProcessCmdPipe(void)
{
    int32 iStatus = CFE_SUCCESS;
    CFE_SB_Msg_t* CmdMsgPtr=NULL;
    CFE_SB_MsgId_t CmdMsgId;
    bool contProcessing = true;

    /* Process command messages until the pipe is empty */
    while (contProcessing)
    {
        iStatus = CFE_SB_RcvMsg(&CmdMsgPtr, CmdPipeId, CFE_SB_POLL);
        if(iStatus == CFE_SUCCESS)
        {
            /* We did receive a message.  Process it. */
            CmdMsgId = CFE_SB_GetMsgId(CmdMsgPtr);
            switch (CmdMsgId)
            {
                case AMC_CMD_MID:
                {
                    /* We did receive a command.  Process it. */
                    ProcessAppCmds(CmdMsgPtr);
                    break;
                }

                default:
                {
                    /* Bump the command error counter for an unknown command.
                     * (This should only occur if it was subscribed to with
                     * this pipe, but not handled in this switch-case.) */
                    HkTlm.usCmdErrCnt++;
                    CFE_EVS_SendEvent(AMC_MSGID_ERR_EID, CFE_EVS_ERROR,
                            "Recvd invalid CMD msgId (0x%04X)",
                            (unsigned short)CmdMsgId);
                    break;
                }
            }
        }
        else if (iStatus == CFE_SB_NO_MESSAGE)
        {
            /* The command pipe is empty.  Break the function and continue
             * on. */
            contProcessing = false;
        }
        else
        {
            /* Something failed.  Quit the loop. */
            CFE_EVS_SendEvent(AMC_RCVMSG_ERR_EID, CFE_EVS_ERROR,
                    "CMD pipe read error (0x%08X)", (unsigned int)iStatus);
            contProcessing = false;
        }
    }
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process Incoming Data Messages                                  */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void AMC::ProcessDataPipe(void)
{
    int32 iStatus = CFE_SUCCESS;
    CFE_SB_Msg_t* MsgPtr=NULL;
    CFE_SB_MsgId_t MsgId;
    bool contProcessing = true;

    if(HkTlm.DebugArmed == TRUE)
    {
        if(HkTlm.ArmedTimeout > 0)
        {
            --HkTlm.ArmedTimeout;
            if(HkTlm.ArmedTimeout == 0)
            {
                CFE_EVS_SendEvent(AMC_ARM_TIMEOUT_ERR_EID, CFE_EVS_ERROR,
                            "Debug armed timeout expired.");
                HkTlm.DebugArmed = FALSE;
            }
        }
    }

    /* Process command messages until the pipe is empty */
    while (contProcessing)
    {
        iStatus = CFE_SB_RcvMsg(&MsgPtr, DataPipeId, CFE_SB_POLL);
        if(iStatus == CFE_SUCCESS)
        {
            /* We did receive a message.  Process it. */
            MsgId = CFE_SB_GetMsgId(MsgPtr);
            switch (MsgId)
            {
                case PX4_ACTUATOR_ARMED_MID:
                {
                    CFE_PSP_MemCpy(&CVT.ActuatorArmed, MsgPtr, sizeof(CVT.ActuatorArmed));
                    UpdateMotors();
                    break;
                }

                case PX4_ACTUATOR_CONTROLS_0_MID:
                {
                    CFE_PSP_MemCpy(&CVT.ActuatorControls0, MsgPtr,
                        sizeof(CVT.ActuatorControls0));
                    break;
                }

                case PX4_ACTUATOR_CONTROLS_1_MID:
                {
                    CFE_PSP_MemCpy(&CVT.ActuatorControls1, MsgPtr,
                        sizeof(CVT.ActuatorControls1));
                    break;
                }

                case PX4_ACTUATOR_CONTROLS_2_MID:
                {
                    CFE_PSP_MemCpy(&CVT.ActuatorControls2, MsgPtr,
                        sizeof(CVT.ActuatorControls2));
                    break;
                }

                case PX4_ACTUATOR_CONTROLS_3_MID:
                {
                    CFE_PSP_MemCpy(&CVT.ActuatorControls3, MsgPtr,
                        sizeof(CVT.ActuatorControls3));
                    break;
                }

                default:
                {
                    /* Bump the command error counter for an unknown command.
                     * (This should only occur if it was subscribed to with
                     * this pipe, but not handled in this switch-case.) */
                    HkTlm.usCmdErrCnt++;
                    CFE_EVS_SendEvent(AMC_MSGID_ERR_EID, CFE_EVS_ERROR,
                            "Recvd invalid DATA msgId (0x%04X)",
                            (unsigned short)MsgId);
                    break;
                }
            }
        }
        else if (iStatus == CFE_SB_NO_MESSAGE)
        {
            /* The command pipe is empty.  Break the function and continue
             * on. */
            contProcessing = false;
        }
        else
        {
            /* Something failed.  Quit the loop. */
            CFE_EVS_SendEvent(AMC_RCVMSG_ERR_EID, CFE_EVS_ERROR,
                    "DATA pipe read error (0x%08X)", (unsigned int)iStatus);
            contProcessing = false;
        }
    }

    UpdateMotors();
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process AMC Commands                                            */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void AMC::ProcessAppCmds(CFE_SB_Msg_t* MsgPtr)
{
    uint32 uiCmdCode=0;

    if (MsgPtr != NULL)
    {
        uiCmdCode = CFE_SB_GetCmdCode(MsgPtr);

        static uint16 debug_pwm[AMC_MAX_MOTOR_OUTPUTS] = {0};

        switch (uiCmdCode)
        {
            case AMC_NOOP_CC:
            {
                /* A NoOp command was received.  Increment the counter,
                 * and raise a NOOP event. */
                HkTlm.usCmdCnt++;
                CFE_EVS_SendEvent(AMC_CMD_NOOP_EID, CFE_EVS_INFORMATION,
                        "Recvd NOOP. Version %d.%d.%d.%d",
                        AMC_MAJOR_VERSION,
                        AMC_MINOR_VERSION,
                        AMC_REVISION,
                        AMC_MISSION_REV);
                break;
            }

            case AMC_RESET_CC:
            {
                /* A RESET command was received.  Reset both success and
                 * error counters. */
                HkTlm.usCmdCnt = 0;
                HkTlm.usCmdErrCnt = 0;
                break;
            }

            case AMC_ARM_DEBUG_CC:
            {
                if(HkTlm.DebugArmed == FALSE)
                {
                    /* Increment the command counter. */
                    HkTlm.usCmdCnt++;
                    CFE_EVS_SendEvent(AMC_ARM_DEBUG_INF_EID, CFE_EVS_INFORMATION,
                            "Debug mode armed.");
                    HkTlm.DebugArmed = TRUE;
                    HkTlm.ArmedTimeout = AMC_ARM_DEBUG_TIMEOUT;
                }
                else
                {
                    /* Increment the error counter. */
                    HkTlm.usCmdErrCnt++;
                    CFE_EVS_SendEvent(AMC_ARM_DEBUG_INF_EID, CFE_EVS_INFORMATION,
                            "Failed to arm debug mode. Debug mode is already armed.");
                }
                break;
            }

            case AMC_DISARM_DEBUG_CC:
            {
                if(HkTlm.DebugArmed == TRUE)
                {
                    /* Increment the command counter. */
                    HkTlm.usCmdCnt++;
                    CFE_EVS_SendEvent(AMC_DISARM_DEBUG_INF_EID, CFE_EVS_INFORMATION,
                            "Debug mode disarmed.");
                    HkTlm.DebugArmed = FALSE;
                    HkTlm.ArmedTimeout = 0;
                }
                else
                {
                    /* Increment the error counter. */
                    HkTlm.usCmdErrCnt++;
                    CFE_EVS_SendEvent(AMC_DISARM_DEBUG_ERR_EID, CFE_EVS_ERROR,
                            "Failed to disarm debug mode. Debug mode is not armed.");
                }
                break;
            }

            case AMC_ENGAGE_DEBUG_CC:
            {
                if(HkTlm.DebugArmed == TRUE)
                {
                    /* Increment the command counter. */
                    HkTlm.usCmdCnt++;
                    CFE_EVS_SendEvent(AMC_ENGAGE_DEBUG_INF_EID, CFE_EVS_INFORMATION,
                            "Debug mode engaged.");
                    HkTlm.DebugEngaged = TRUE;
                    HkTlm.DebugArmed = FALSE;
                    HkTlm.ArmedTimeout = 0;
                    memset(&debug_pwm[0], 0x0, sizeof(debug_pwm));
                }
                else
                {
                    /* Increment the error counter. */
                    HkTlm.usCmdErrCnt++;
                    CFE_EVS_SendEvent(AMC_ENGAGE_DEBUG_ERR_EID, CFE_EVS_ERROR,
                            "Failed to engage debug, debug mode is not armed.");
                }
                break;
            }

            case AMC_DISENGAGE_DEBUG_CC:
            {
                if(HkTlm.DebugEngaged == TRUE)
                {
                    /* Increment the command counter. */
                    HkTlm.usCmdCnt++;
                    CFE_EVS_SendEvent(AMC_DISENGAGE_DEBUG_INF_EID, CFE_EVS_INFORMATION,
                            "Debug mode disengaged.");
                    HkTlm.DebugEngaged = FALSE;
                    memset(&debug_pwm[0], 0x0, sizeof(debug_pwm));
                }
                else
                {
                    /* Increment the error counter. */
                    HkTlm.usCmdErrCnt++;
                    CFE_EVS_SendEvent(AMC_DISENGAGE_DEBUG_ERR_EID, CFE_EVS_ERROR,
                            "Failed to disengage debug mode. Debug is not engaged.");
                }
                break;
            }

            case AMC_DEBUG_CMD_CC:
            {
                if(HkTlm.DebugEngaged == TRUE)
                {
                    AMC_DebugCmd_t debugCmd = {0};
                    CFE_PSP_MemCpy(&debugCmd, MsgPtr, sizeof(debugCmd));
                    CFE_EVS_SendEvent(AMC_CMD_DEBUG_INF_EID, CFE_EVS_INFORMATION,
                            "Received debug command index %hu value %hu", 
                            debugCmd.Index, debugCmd.Cmd);

                    if(debugCmd.Index < AMC_MAX_MOTOR_OUTPUTS)
                    {
                        debug_pwm[debugCmd.Index] = debugCmd.Cmd;
                        /* Increment the command counter. */
                        HkTlm.usCmdCnt++;
                        SetMotorOutputs(debug_pwm);
                    }
                    else
                    {
                        CFE_EVS_SendEvent(AMC_CMD_DEBUG_ERR_EID, CFE_EVS_ERROR,
                                "Invalid index received");
                        /* Increment the error counter. */
                        HkTlm.usCmdErrCnt++;
                    }
                }
                else
                {
                    CFE_EVS_SendEvent(AMC_CMD_DEBUG_ERR_EID, CFE_EVS_ERROR,
                            "Debug command received with debug mode disengaged");
                    /* Increment the error counter. */
                    HkTlm.usCmdErrCnt++;
                }
                break;
            }

            case AMC_SIMPLE_SET_OUTPUT_SCALER_CC:
            {
            	AMC_SimpleSetOutputScalerCmd_t *cmd = (AMC_SimpleSetOutputScalerCmd_t*)MsgPtr;

            	if(cmd->MixerIndex >= AMC_SIMPLE_MIXER_MAX_MIXERS)
            	{
                    CFE_EVS_SendEvent(AMC_CMD_DEBUG_ERR_EID, CFE_EVS_ERROR,
                            "Invalid Mixer index.");
                    /* Increment the error counter. */
                    HkTlm.usCmdErrCnt++;
            	}
            	else
            	{
            		int32 status = SimpleMixerObject[cmd->MixerIndex].SetOutputScaler(cmd->OutputScaler);

            		if(status < 0)
            		{
                        HkTlm.usCmdErrCnt++;
            		}
            		else
            		{
                        HkTlm.usCmdCnt++;
            		}
            	}

            	break;
            }

            case AMC_SIMPLE_SET_CONTROL_CC:
            {
            	AMC_SimpleSetControlCmd_t *cmd = (AMC_SimpleSetControlCmd_t*)MsgPtr;

            	if(cmd->MixerIndex >= AMC_SIMPLE_MIXER_MAX_MIXERS)
            	{
                    CFE_EVS_SendEvent(AMC_CMD_DEBUG_ERR_EID, CFE_EVS_ERROR,
                            "Invalid Mixer index.");
                    /* Increment the error counter. */
                    HkTlm.usCmdErrCnt++;
            	}
            	else
            	{
            		int32 status = SimpleMixerObject[cmd->MixerIndex].SetControl(
            				cmd->Control,
							cmd->Group,
							cmd->Index);

            		if(status < 0)
            		{
                        HkTlm.usCmdErrCnt++;
            		}
            		else
            		{
                        HkTlm.usCmdCnt++;
            		}
            	}

            	break;
            }

            case AMC_SIMPLE_SET_CONTROL_SCALER_CC:
            {
            	AMC_SimpleSetControlScalerCmd_t *cmd = (AMC_SimpleSetControlScalerCmd_t*)MsgPtr;

            	if(cmd->MixerIndex >= AMC_SIMPLE_MIXER_MAX_MIXERS)
            	{
                    CFE_EVS_SendEvent(AMC_CMD_DEBUG_ERR_EID, CFE_EVS_ERROR,
                            "Invalid Mixer index.");
                    /* Increment the error counter. */
                    HkTlm.usCmdErrCnt++;
            	}
            	else
            	{
            		int32 status = SimpleMixerObject[cmd->MixerIndex].SetControlScaler(
            				cmd->Control,
							cmd->Scaler);

            		if(status < 0)
            		{
                        HkTlm.usCmdErrCnt++;
            		}
            		else
            		{
                        HkTlm.usCmdCnt++;
            		}
            	}

            	break;
            }

            default:
            {
                /* An unknown command was received.  Increment the command
                 * error counter and raise an event. */
                HkTlm.usCmdErrCnt++;
                CFE_EVS_SendEvent(AMC_CC_ERR_EID, CFE_EVS_ERROR,
                        "Recvd invalid command code (%u)",
                        (unsigned int)uiCmdCode);
                break;
            }
        }
    }
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Send AMC Housekeeping                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void AMC::ReportHousekeeping(void)
{
    uint32 i = 0;

    HkTlm.Timestamp = ActuatorOutputs.Timestamp;
    HkTlm.Count = ActuatorOutputs.Count;

    for(i = 0; i < PX4_ACTUATOR_OUTPUTS_MAX; ++i)
    {
    	HkTlm.Output[i] = ActuatorOutputs.Output[i];
    }

    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&HkTlm);
    CFE_SB_SendMsg((CFE_SB_Msg_t*)&HkTlm);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Publish Output Data                                             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void AMC::SendActuatorOutputs(void)
{
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&ActuatorOutputs);
    CFE_SB_SendMsg((CFE_SB_Msg_t*)&ActuatorOutputs);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Verify Command Length                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
boolean AMC::VerifyCmdLength(CFE_SB_Msg_t* MsgPtr,
        uint16 usExpectedLen)
{
    boolean bResult = TRUE;
    uint16 usMsgLen = 0;

    if (MsgPtr != NULL)
    {
        usMsgLen = CFE_SB_GetTotalMsgLength(MsgPtr);

        if (usExpectedLen != usMsgLen)
        {
            bResult = FALSE;
            CFE_SB_MsgId_t MsgId = CFE_SB_GetMsgId(MsgPtr);
            uint16 usCmdCode = CFE_SB_GetCmdCode(MsgPtr);

            CFE_EVS_SendEvent(AMC_MSGLEN_ERR_EID, CFE_EVS_ERROR,
                    "Rcvd invalid msgLen: msgId=0x%08X, cmdCode=%d, "
                    "msgLen=%d, expectedLen=%d",
                    MsgId, usCmdCode, usMsgLen, usExpectedLen);
            HkTlm.usCmdErrCnt++;
        }
    }

    return bResult;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* AMC Application C style main entry point.                       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
extern "C" void AMC_AppMain(void)
{
    oAMC.AppMain();
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* AMC Application C++ style main entry point.                     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void AMC::AppMain(void)
{
    /* Register the application with Executive Services */
    uiRunStatus = CFE_ES_APP_RUN;

    int32 iStatus = CFE_ES_RegisterApp();
    if (iStatus != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("AMC - Failed to register the app (0x%08X)\n",
                (unsigned int)iStatus);
    }

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(AMC_MAIN_TASK_PERF_ID);

    /* Perform application initializations */
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = InitApp();
    }

    if (iStatus == CFE_SUCCESS)
    {
        /* Do not perform performance monitoring on startup sync */
        CFE_ES_PerfLogExit(AMC_MAIN_TASK_PERF_ID);
        CFE_ES_WaitForStartupSync(AMC_STARTUP_TIMEOUT_MSEC);
        CFE_ES_PerfLogEntry(AMC_MAIN_TASK_PERF_ID);
    }
    else
    {
        uiRunStatus = CFE_ES_APP_ERROR;
    }

    /* Application main loop */
    while (CFE_ES_RunLoop(&uiRunStatus) == TRUE)
    {
        RcvSchPipeMsg(AMC_SCH_PIPE_PEND_TIME);

        iStatus = AcquireConfigPointers();
        if(iStatus != CFE_SUCCESS)
        {
            /* We apparently tried to load a new table but failed.  Terminate
             * the application. */
            uiRunStatus = CFE_ES_APP_ERROR;
        }
    }

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(AMC_MAIN_TASK_PERF_ID);

    /* Exit the application */
    CFE_ES_ExitApp(uiRunStatus);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Command all motors to stop.                                     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void AMC::StopMotors(void)
{
    uint16 disarmed_pwm[AMC_MAX_MOTOR_OUTPUTS];

    for (uint32 i = 0; i < AMC_MAX_MOTOR_OUTPUTS; i++)
    {
        disarmed_pwm[i] = ConfigTblPtr->PwmDisarmed;
    }

    SetMotorOutputs(disarmed_pwm);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Mix actuator controls and update motor speeds accordingly.      */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void AMC::UpdateMotors(void)
{
    const uint16 reverse_mask = 0;
    uint16 disarmed_pwm[AMC_MAX_MOTOR_OUTPUTS];
    uint16 min_pwm[AMC_MAX_MOTOR_OUTPUTS];
    uint16 max_pwm[AMC_MAX_MOTOR_OUTPUTS];
    uint16 pwm[AMC_MAX_MOTOR_OUTPUTS];

    for (uint32 i = 0; i < AMC_MAX_MOTOR_OUTPUTS; i++)
    {
        disarmed_pwm[i] = ConfigTblPtr->PwmDisarmed;
        min_pwm[i] = ConfigTblPtr->PwmMin;
        max_pwm[i] = ConfigTblPtr->PwmMax;
    }

    /* Never actuate any motors unless the system is armed.  Check to see if
     * its armed, or in lock down before continuing
     */
    if (CVT.ActuatorArmed.Lockdown || CVT.ActuatorArmed.ManualLockdown)
    {
        if(HkTlm.DebugEngaged != TRUE)
        {
            SetMotorOutputs(disarmed_pwm);
        }
    }
    else if(CVT.ActuatorArmed.Armed)
    {
        ActuatorOutputs.Timestamp = PX4LIB_GetPX4TimeUs();

        /* Do mixing */
        ActuatorOutputs.Count = 0;
        /* Disable unused ports by setting their output to NaN */
//        for (size_t i = ActuatorOutputs.Count;
//                i < sizeof(ActuatorOutputs.Output)
//                        / sizeof(ActuatorOutputs.Output[0]);
//                i++)
//        {
//            ActuatorOutputs.Output[i] = NAN;
//        }

		for(uint32 i = 0; i < AMC_MULTIROTOR_MIXER_MAX_MIXERS; ++i)
		{
			ActuatorOutputs.Count += MultirotorMixerObject[i].mix(ActuatorOutputs.Output, 0, 0);
		}

		for(uint32 i = 0; i < AMC_SIMPLE_MIXER_MAX_MIXERS; ++i)
		{
			ActuatorOutputs.Count += SimpleMixerObject[i].mix(&ActuatorOutputs.Output[ActuatorOutputs.Count], 0, 0);
		}

        PwmLimit_Calc(
                CVT.ActuatorArmed.Armed,
                FALSE/*_armed.prearmed*/,
                ActuatorOutputs.Count,
                reverse_mask,
                disarmed_pwm,
                min_pwm,
                max_pwm,
                ActuatorOutputs.Output,
                pwm,
                &PwmLimit);

        if(!CVT.ActuatorArmed.InEscCalibrationMode)
        {
            if(HkTlm.DebugEngaged != TRUE)
            {
                SetMotorOutputs(pwm);
            }
        }

        CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&ActuatorOutputs);
        CFE_SB_SendMsg((CFE_SB_Msg_t*)&ActuatorOutputs);
    }
    else
    {
        if(HkTlm.DebugEngaged != TRUE)
        {
            SetMotorOutputs(disarmed_pwm);
        }
    }
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Mixer callback to get the actual control value.                 */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 AMC::ControlCallback(
        uint8 ControlGroup,
        uint8 ControlIndex,
        float &Control)
{
    int32 status = 0;
    PX4_ActuatorControlsMsg_t *controls = 0;

    if(ControlGroup >= PX4_ACTUATOR_CONTROL_COUNT)
    {
    	status = -1;
    	goto end_of_function;
    }

    if(ControlIndex >= PX4_ACTUATOR_CONTROL_COUNT)
    {
    	status = -1;
    	goto end_of_function;
    }

    switch(ControlGroup)
    {
        case 0:
        {
        	controls = &oAMC.CVT.ActuatorControls0;
        	break;
        }

        case 1:
        {
        	controls = &oAMC.CVT.ActuatorControls1;
        	break;
        }

        case 2:
        {
        	controls = &oAMC.CVT.ActuatorControls2;
        	break;
        }

        case 3:
        {
        	controls = &oAMC.CVT.ActuatorControls3;
        	break;
        }
    }

    Control = controls->Control[ControlIndex];

end_of_function:

    return status;
}


#ifdef __cplusplus
}
#endif

/************************/
/*  End of File Comment */
/************************/
