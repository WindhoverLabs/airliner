/****************************************************************************
 *
 *   Copyright (c) 2016-2017 Windhover Labs, L.L.C. All rights reserved.
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
 * 3. Neither the name Windhover Labs nor the names of its contributors 
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
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
 ****************************************************************************/

/*************************************************************************
** Includes
*************************************************************************/

#include "pb_lib.h"
#include "pb_version.h"

PBLIB_AppData_t PBLIB_AppData;


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Register CFE ES serialization functions                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
uint32 CFE_ES_RegisterSerializationFuncs()
{
	int32 Status = CFE_SUCCESS;

	/* Register each message with PBL */
	Status = PBLIB_RegisterCmdMessage(0x1806, 20, "CFE_ES_SetMaxPRCountCmd_t"); 
	if (Status != CFE_SUCCESS)
	{
		goto CFE_ES_RegisterSerializationFuncs_Exit_Tag;
	}
	
	Status = PBLIB_RegisterTlmMessage(0x080F, "CFE_ES_ShellPacket_t");
	if (Status != CFE_SUCCESS)
	{
		goto CFE_ES_RegisterSerializationFuncs_Exit_Tag;
	}
	
	Status = PBLIB_RegisterCmdMessage(0x1806, 7, "CFE_ES_AppReloadCmd_t"); 
	if (Status != CFE_SUCCESS)
	{
		goto CFE_ES_RegisterSerializationFuncs_Exit_Tag;
	}
	
	Status = PBLIB_RegisterTlmMessage(0x0800, "CFE_ES_HkPacket_t");
	if (Status != CFE_SUCCESS)
	{
		goto CFE_ES_RegisterSerializationFuncs_Exit_Tag;
	}
	
	Status = PBLIB_RegisterCmdMessage(0x1806, 22, "CFE_ES_TlmPoolStatsCmd_t"); 
	if (Status != CFE_SUCCESS)
	{
		goto CFE_ES_RegisterSerializationFuncs_Exit_Tag;
	}
	
	Status = PBLIB_RegisterCmdMessage(0x1806, 14, "CFE_ES_PerfStartCmd_t"); 
	if (Status != CFE_SUCCESS)
	{
		goto CFE_ES_RegisterSerializationFuncs_Exit_Tag;
	}
	
	Status = PBLIB_RegisterCmdMessage(0x1806, 3, "CFE_ES_ShellCmd_t"); 
	if (Status != CFE_SUCCESS)
	{
		goto CFE_ES_RegisterSerializationFuncs_Exit_Tag;
	}
	
	Status = PBLIB_RegisterCmdMessage(0x1806, 24, "CFE_ES_QueryAllTasksCmd_t"); 
	if (Status != CFE_SUCCESS)
	{
		goto CFE_ES_RegisterSerializationFuncs_Exit_Tag;
	}
	
	Status = PBLIB_RegisterCmdMessage(0x1806, 17, "CFE_ES_PerfSetTrigMaskCmd_t"); 
	if (Status != CFE_SUCCESS)
	{
		goto CFE_ES_RegisterSerializationFuncs_Exit_Tag;
	}
	
	Status = PBLIB_RegisterTlmMessage(0x080B, "CFE_ES_OneAppTlm_t");
	if (Status != CFE_SUCCESS)
	{
		goto CFE_ES_RegisterSerializationFuncs_Exit_Tag;
	}
	
	Status = PBLIB_RegisterCmdMessage(0x1806, 9, "CFE_ES_QueryAllCmd_t"); 
	if (Status != CFE_SUCCESS)
	{
		goto CFE_ES_RegisterSerializationFuncs_Exit_Tag;
	}
	
	Status = PBLIB_RegisterCmdMessage(0x1806, 2, "CFE_ES_RestartCmd_t"); 
	if (Status != CFE_SUCCESS)
	{
		goto CFE_ES_RegisterSerializationFuncs_Exit_Tag;
	}
	
	Status = PBLIB_RegisterCmdMessage(0x1806, 16, "CFE_ES_PerfSetFilterMaskCmd_t"); 
	if (Status != CFE_SUCCESS)
	{
		goto CFE_ES_RegisterSerializationFuncs_Exit_Tag;
	}
	
	Status = PBLIB_RegisterCmdMessage(0x1806, 21, "CFE_ES_DeleteCDSCmd_t"); 
	if (Status != CFE_SUCCESS)
	{
		goto CFE_ES_RegisterSerializationFuncs_Exit_Tag;
	}
	
	Status = PBLIB_RegisterCmdMessage(0x1806, 6, "CFE_ES_AppNameCmd_t"); 
	if (Status != CFE_SUCCESS)
	{
		goto CFE_ES_RegisterSerializationFuncs_Exit_Tag;
	}
	
	Status = PBLIB_RegisterCmdMessage(0x1806, 15, "CFE_ES_PerfStopCmd_t"); 
	if (Status != CFE_SUCCESS)
	{
		goto CFE_ES_RegisterSerializationFuncs_Exit_Tag;
	}
	
	Status = PBLIB_RegisterCmdMessage(0x1806, 18, "CFE_ES_OverWriteSysLogCmd_t"); 
	if (Status != CFE_SUCCESS)
	{
		goto CFE_ES_RegisterSerializationFuncs_Exit_Tag;
	}
	
	Status = PBLIB_RegisterCmdMessage(0x1806, 11, "CFE_ES_WriteSyslogCmd_t"); 
	if (Status != CFE_SUCCESS)
	{
		goto CFE_ES_RegisterSerializationFuncs_Exit_Tag;
	}
	
	Status = PBLIB_RegisterCmdMessage(0x1806, 5, "CFE_ES_AppNameCmd_t"); 
	if (Status != CFE_SUCCESS)
	{
		goto CFE_ES_RegisterSerializationFuncs_Exit_Tag;
	}
	
	Status = PBLIB_RegisterCmdMessage(0x1806, 4, "CFE_ES_StartAppCmd_t"); 
	if (Status != CFE_SUCCESS)
	{
		goto CFE_ES_RegisterSerializationFuncs_Exit_Tag;
	}
	
	Status = PBLIB_RegisterCmdMessage(0x1806, 8, "CFE_ES_AppNameCmd_t"); 
	if (Status != CFE_SUCCESS)
	{
		goto CFE_ES_RegisterSerializationFuncs_Exit_Tag;
	}
	
	Status = PBLIB_RegisterCmdMessage(0x1806, 23, "CFE_ES_DumpCDSRegCmd_t"); 
	if (Status != CFE_SUCCESS)
	{
		goto CFE_ES_RegisterSerializationFuncs_Exit_Tag;
	}
	
	Status = PBLIB_RegisterCmdMessage(0x1806, 13, "CFE_ES_WriteERlogCmd_t"); 
	if (Status != CFE_SUCCESS)
	{
		goto CFE_ES_RegisterSerializationFuncs_Exit_Tag;
	}

	Status = PBLIB_RegisterTlmMessage(0x0a34, "PX4_PositionSetpointTripletMsg_t");
	if (Status != CFE_SUCCESS)
	{
		goto CFE_ES_RegisterSerializationFuncs_Exit_Tag;
	}

    Status = PBLIB_RegisterTlmMessage(0x0a27, "PX4_ManualControlSetpointMsg_t");
	if (Status != CFE_SUCCESS)
	{
		goto CFE_ES_RegisterSerializationFuncs_Exit_Tag;
	}

    Status = PBLIB_RegisterTlmMessage(0x0a4d, "PX4_VehicleCommandMsg_t");
	if (Status != CFE_SUCCESS)
	{
		goto CFE_ES_RegisterSerializationFuncs_Exit_Tag;
	}	

	Status = PBLIB_RegisterTlmMessage(0x0c55, "SENS_HkTlm_t");
	if (Status != CFE_SUCCESS)
	{
		goto CFE_ES_RegisterSerializationFuncs_Exit_Tag;
	}

	Status = PBLIB_RegisterTlmMessage(0x0c54, "GPS_HkTlm_t");
	if (Status != CFE_SUCCESS)
	{
		goto CFE_ES_RegisterSerializationFuncs_Exit_Tag;
	}
	
    Status = PBLIB_RegisterTlmMessage(0x0a50, "PX4_VehicleGlobalPositionMsg_t");
	if (Status != CFE_SUCCESS)
	{
		goto CFE_ES_RegisterSerializationFuncs_Exit_Tag;
	}

CFE_ES_RegisterSerializationFuncs_Exit_Tag:
    return Status;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* CFS Library Initialization Routine                              */
/* cFE requires that a library have an initialization routine      */ 
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 PBLIB_LibInit(void)
{
	int32 Status = CFE_SUCCESS;

	/* Create mutex for registration table */
	Status = OS_MutSemCreate(&PBLIB_AppData.RegTblMutex, PBLIB_REGTBL_MUTEX_NAME, 0);
	if (Status != CFE_SUCCESS)
	{
		goto PBLIB_LibInit_Exit_Tag;
	}

	Status = CFE_ES_RegisterSerializationFuncs();
	if (Status != CFE_SUCCESS)
	{
		goto PBLIB_LibInit_Exit_Tag;
	}

PBLIB_LibInit_Exit_Tag:
	if (Status == CFE_SUCCESS)
	{
		OS_printf ("PBLIB Initialized.  Version %d.%d.%d.%d\n",
    		PBLIB_MAJOR_VERSION,
			PBLIB_MINOR_VERSION,
			PBLIB_REVISION,
			PBLIB_MISSION_REV);
	}

    return Status;
 
}/* End PBLIB_LibInit */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Register Command Message		                                   */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 PBLIB_RegisterCmdMessage(CFE_SB_MsgId_t msgId, uint16 cmdCode, char *msgName)
{
    int32 		status = -2;
    int32 i = 0;

    if(msgId == 0)
    {
    	status = -1;
    	goto PBLIB_RegisterCmdMessage_Exit_Tag;
    }

    /* Lock the mutex */
	OS_MutSemTake(PBLIB_AppData.RegTblMutex);

	/* Find an empty registration entry */
    for(i = 0; i < PBLIB_REG_MAX_ENTRY; ++i)
    {
    	if(PBLIB_AppData.RegisteredFuncs[i].msgId == 0)
    	{
    		PBLIB_AppData.RegisteredFuncs[i].msgId = msgId;
    		PBLIB_AppData.RegisteredFuncs[i].cmdCode = cmdCode;
    		strcpy(PBLIB_AppData.RegisteredFuncs[i].msgName, msgName);
    		status = 0;
    		break;
    	}
    }

    /* Unlock the mutex */
	OS_MutSemGive(PBLIB_AppData.RegTblMutex);

PBLIB_RegisterCmdMessage_Exit_Tag:
    return status;
}/* End PBLIB_RegisterCmdMessage */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Register Telemetry Message		                               */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 PBLIB_RegisterTlmMessage(CFE_SB_MsgId_t msgId, char *msgName)
{
    int32 		status = -2;
    int32 i = 0;

    if(msgId == 0)
    {
    	status = -1;
    	goto PBLIB_RegisterTlmMessage_Exit_Tag;
    }

    /* Lock the mutex */
	OS_MutSemTake(PBLIB_AppData.RegTblMutex);

	/* Find an empty registration entry */
    for(i = 0; i < PBLIB_REG_MAX_ENTRY; ++i)
    {
    	if(PBLIB_AppData.RegisteredFuncs[i].msgId == 0)
    	{
    		PBLIB_AppData.RegisteredFuncs[i].msgId = msgId;
    		PBLIB_AppData.RegisteredFuncs[i].cmdCode = 0;
    		strcpy(PBLIB_AppData.RegisteredFuncs[i].msgName, msgName);
    		status = 0;
    		break;
    	}
    }

    /* Unlock the mutex */
	OS_MutSemGive(PBLIB_AppData.RegTblMutex);

PBLIB_RegisterTlmMessage_Exit_Tag:
    return status;
}/* End PBLIB_RegisterTlmMessage */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Deregister Message		                                       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 PBLIB_DeregisterCmdMessage(CFE_SB_MsgId_t msgId, uint16 cmdCode)
{
    int32 		status = -2;
    int32 i = 0;

    if(msgId == 0)
    {
    	status = -1;
    	goto PBLIB_DeregisterCmdMessage_Exit_Tag;
    }

    /* Lock the mutex */
	OS_MutSemTake(PBLIB_AppData.RegTblMutex);

	/* Find the registration entry */
    for(i = 0; i < PBLIB_REG_MAX_ENTRY; ++i)
    {
    	if(PBLIB_AppData.RegisteredFuncs[i].msgId == msgId && PBLIB_AppData.RegisteredFuncs[i].cmdCode == cmdCode)
    	{
    		PBLIB_AppData.RegisteredFuncs[i].msgId = 0;
    		PBLIB_AppData.RegisteredFuncs[i].cmdCode = 0;
    		strcpy(PBLIB_AppData.RegisteredFuncs[i].msgName, "\0");
    		status = 0;
    		break;
    	}
    }

    /* Unlock the mutex */
	OS_MutSemGive(PBLIB_AppData.RegTblMutex);

PBLIB_DeregisterCmdMessage_Exit_Tag:
    return status;
}/* End PBLIB_DeregisterCmdMessage */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Deregister Telemetry Message		                               */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 PBLIB_DeregisterTlmMessage(CFE_SB_MsgId_t msgId)
{
    int32 		status = -2;
    int32 i = 0;

    if(msgId == 0)
    {
    	status = -1;
    	goto PBLIB_DeregisterTlmMessage_Exit_Tag;
    }

    /* Lock the mutex */
	OS_MutSemTake(PBLIB_AppData.RegTblMutex);

	/* Find the registration entry */
    for(i = 0; i < PBLIB_REG_MAX_ENTRY; ++i)
    {
    	if(PBLIB_AppData.RegisteredFuncs[i].msgId == msgId && PBLIB_AppData.RegisteredFuncs[i].cmdCode == 0)
    	{
    		PBLIB_AppData.RegisteredFuncs[i].msgId = 0;
    		PBLIB_AppData.RegisteredFuncs[i].cmdCode = 0;
    		strcpy(PBLIB_AppData.RegisteredFuncs[i].msgName, "\0");
    		status = 0;
    		break;
    	}
    }

    /* Unlock the mutex */
	OS_MutSemGive(PBLIB_AppData.RegTblMutex);

PBLIB_DeregisterTlmMessage_Exit_Tag:
    return status;
}/* End PBLIB_DeregisterCmdMessage */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Get Command Serialization Function Address                      */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
PBLib_EncodeFuncPtr_t PBLIB_GetCmdSerializationFunc(CFE_SB_MsgId_t msgId, uint16 cmdCode)
{
	PBLib_EncodeFuncPtr_t funcAddr = 0;
	char encFuncName[64];
	int32 status = 0;
    int32 i = 0;

    if(msgId == 0)
    {
    	goto PBLIB_GetCmdSerializationFunc_Exit_Tag;
    }

    /* Lock the mutex */
	OS_MutSemTake(PBLIB_AppData.RegTblMutex);

	/* Find the registration entry */
    for(i = 0; i < PBLIB_REG_MAX_ENTRY; ++i)
    {
    	if(PBLIB_AppData.RegisteredFuncs[i].msgId == msgId && PBLIB_AppData.RegisteredFuncs[i].cmdCode == cmdCode)
    	{
    		strcpy(encFuncName, PBLIB_AppData.RegisteredFuncs[i].msgName);
			strcat(encFuncName, "_Enc");
			status = OS_SymbolLookup((cpuaddr*)&funcAddr, (const char*)&encFuncName);
    		break;
    	}
    }

    /* Unlock the mutex */
	OS_MutSemGive(PBLIB_AppData.RegTblMutex);


PBLIB_GetCmdSerializationFunc_Exit_Tag:
    return funcAddr;
}/* End PBLIB_GetCmdSerializationFunc */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Get Telemetry Serialization Function Address                    */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
PBLib_EncodeFuncPtr_t PBLIB_GetTlmSerializationFunc(CFE_SB_MsgId_t msgId)
{
	PBLib_EncodeFuncPtr_t funcAddr = 0;
	char encFuncName[64];
	int32 status = 0;
    int32 i = 0;

    if(msgId == 0)
    {
    	goto PBLIB_GetTlmSerializationFunc_Exit_Tag;
    }

    /* Lock the mutex */
	OS_MutSemTake(PBLIB_AppData.RegTblMutex);

	/* Find the registration entry */
    for(i = 0; i < PBLIB_REG_MAX_ENTRY; ++i)
    {
    	if(PBLIB_AppData.RegisteredFuncs[i].msgId == msgId && PBLIB_AppData.RegisteredFuncs[i].cmdCode == 0)
    	{
    		strcpy(encFuncName, PBLIB_AppData.RegisteredFuncs[i].msgName);
			strcat(encFuncName, "_Enc");
			status = OS_SymbolLookup((cpuaddr*)&funcAddr, (const char*)&encFuncName);
    		break;
    	}
    }

    /* Unlock the mutex */
	OS_MutSemGive(PBLIB_AppData.RegTblMutex);


PBLIB_GetTlmSerializationFunc_Exit_Tag:
    return funcAddr;
}/* End PBLIB_GetCmdSerializationFunc */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Get Command Deserialization Function Address                    */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
PBLib_DecodeFuncPtr_t PBLIB_GetCmdDeserializationFunc(CFE_SB_MsgId_t msgId, uint16 cmdCode)
{
	PBLib_DecodeFuncPtr_t funcAddr = 0;
	char decFuncName[64];
	int32 status = 0;
    int32 i = 0;

    if(msgId == 0)
    {
    	goto PBLIB_GetCmdDeserializationFunc_Exit_Tag;
    }

    /* Lock the mutex */
	OS_MutSemTake(PBLIB_AppData.RegTblMutex);

    /* Find the registration entry */
    for(i = 0; i < PBLIB_REG_MAX_ENTRY; ++i)
    {
    	if(PBLIB_AppData.RegisteredFuncs[i].msgId == msgId && PBLIB_AppData.RegisteredFuncs[i].cmdCode == cmdCode)
    	{
    		strcpy(decFuncName, PBLIB_AppData.RegisteredFuncs[i].msgName);
			strcat(decFuncName, "_Dec");
    		status = OS_SymbolLookup((cpuaddr*)&funcAddr, (const char*)&decFuncName);
    		break;
    	}
    }

    /* Unlock the mutex */
	OS_MutSemGive(PBLIB_AppData.RegTblMutex);

PBLIB_GetCmdDeserializationFunc_Exit_Tag:
    return funcAddr;
}/* End PBLIB_GetCmdSerializationFunc */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Get Telemetry Deserialization Function Address                  */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
PBLib_DecodeFuncPtr_t PBLIB_GetTlmDeserializationFunc(CFE_SB_MsgId_t msgId)
{
	PBLib_DecodeFuncPtr_t funcAddr = 0;
	char decFuncName[64];
	int32 status = 0;
    int32 i = 0;

    if(msgId == 0)
    {
    	goto PBLIB_GetTlmDeserializationFunc_Exit_Tag;
    }

    /* Lock the mutex */
	OS_MutSemTake(PBLIB_AppData.RegTblMutex);

    /* Find the registration entry */
    for(i = 0; i < PBLIB_REG_MAX_ENTRY; ++i)
    {
    	if(PBLIB_AppData.RegisteredFuncs[i].msgId == msgId && PBLIB_AppData.RegisteredFuncs[i].cmdCode == 0)
    	{
    		strcpy(decFuncName, PBLIB_AppData.RegisteredFuncs[i].msgName);
			strcat(decFuncName, "_Dec");
    		status = OS_SymbolLookup((cpuaddr*)&funcAddr, (const char*)&decFuncName);
    		break;
    	}
    }

    /* Unlock the mutex */
	OS_MutSemGive(PBLIB_AppData.RegTblMutex);

PBLIB_GetTlmDeserializationFunc_Exit_Tag:
    return funcAddr;
}/* End PBLIB_GetTlmSerializationFunc */

/************************/
/*  End of File Comment */
/************************/
