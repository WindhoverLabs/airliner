/****************************************************************************
 *
 *   Copyright (c) 2019 Windhover Labs, L.L.C. All rights reserved.
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

#include "prm_ids.h"
#include "prmlib_msgids.h"
#include "prm_lib.h"



ParamsConsumer::ParamsConsumer()
{
	CFE_PSP_MemSet(&ParamRegistration, 0, sizeof(ParamRegistration));
}



ParamsConsumer::~ParamsConsumer()
{
}



int32 ParamsConsumer::InitParams(PRMLIB_ParamRegistrations_t &Params)
{
    int32 iStatus;

    /* Init param pipe and subscribe to param messages */
    iStatus = CFE_SB_CreatePipe(&ParamPipeId,
    		Params.PipeDepth,
			Params.PipeName);
    if (iStatus != CFE_SUCCESS)
    {
        /* We failed to create the pipe for param messages.  Immediately jump
         * to the end of the function to abort initialization and return
         * the error code.
         */
        goto InitParams_Exit_Tag;
    }

    /* Subscribe to the PRMLIB_PARAM_UPDATED_MID message to receive
     * parameter requests. */
    iStatus = CFE_SB_SubscribeEx(PRMLIB_PARAM_UPDATED_MID, ParamPipeId,
            CFE_SB_Default_Qos, 1);
    if (iStatus != CFE_SUCCESS)
    {
        /* We failed to create the pipe for param messages.  Immediately jump
         * to the end of the function to abort initialization and return the
         * error code.
         */
        goto InitParams_Exit_Tag;
    }

    /* Loop through all parameters just passed to the function. */
    for(uint32 i = 0; i < PRMLIB_MAX_REGISTRATIONS; ++i)
    {
    	/* Check to see if the name is blank.  If the name is blank, we will
    	 * assume we've reached the end of the list and just terminate the
    	 * loop.
    	 */
    	if(Params.Registrations[i].Name[0] == 0)
    	{
    		/* The name is blank.  Terminate the loop. */
    		break;
    	}

        iStatus = PRMLIB_ParamRegister(Params.Registrations[i].Name,
                Params.Registrations[i].Value, Params.Registrations[i].Type);
        if(iStatus != CFE_SUCCESS)
        {
    		break;
        }

        CFE_PSP_MemCpy((void*)ParamRegistration[i].Name, (void*)Params.Registrations[i].Name, sizeof(ParamRegistration[i].Name));
        CFE_PSP_MemCpy(&ParamRegistration[i].Type, &Params.Registrations[i].Type, sizeof(ParamRegistration[i].Type));
        CFE_PSP_MemCpy(&ParamRegistration[i].Value, &Params.Registrations[i].Value, sizeof(ParamRegistration[i].Value));
    }

InitParams_Exit_Tag:

    return iStatus;
}



void ParamsConsumer::onParamsChange(PRMLIB_ParamRegistration_t *ParamsData, uint32 ParamsCount)
{
}



osalbool ParamsConsumer::onParamValidate(char Name[PRMLIB_MSG_PARAM_NAME_LEN], uint8 Value)
{
	return TRUE;
}



osalbool ParamsConsumer::onParamValidate(char Name[PRMLIB_MSG_PARAM_NAME_LEN], int8 Value)
{
	return TRUE;
}



osalbool ParamsConsumer::onParamValidate(char Name[PRMLIB_MSG_PARAM_NAME_LEN], uint16 Value)
{
	return TRUE;
}



osalbool ParamsConsumer::onParamValidate(char Name[PRMLIB_MSG_PARAM_NAME_LEN], int16 Value)
{
	return TRUE;
}



osalbool ParamsConsumer::onParamValidate(char Name[PRMLIB_MSG_PARAM_NAME_LEN], uint32 Value)
{
	return TRUE;
}



osalbool ParamsConsumer::onParamValidate(char Name[PRMLIB_MSG_PARAM_NAME_LEN], int32 Value)
{
	return TRUE;
}



osalbool ParamsConsumer::onParamValidate(char Name[PRMLIB_MSG_PARAM_NAME_LEN], float Value)
{
	return TRUE;
}



osalbool ParamsConsumer::onParamValidate(void* Address, uint8 Value)
{
	return TRUE;
}



osalbool ParamsConsumer::onParamValidate(void* Address, int8 Value)
{
	return TRUE;
}



osalbool ParamsConsumer::onParamValidate(void* Address, uint16 Value)
{
	return TRUE;
}



osalbool ParamsConsumer::onParamValidate(void* Address, int16 Value)
{
	return TRUE;
}



osalbool ParamsConsumer::onParamValidate(void* Address, uint32 Value)
{
	return TRUE;
}



osalbool ParamsConsumer::onParamValidate(void* Address, int32 Value)
{
	return TRUE;
}



osalbool ParamsConsumer::onParamValidate(void* Address, float Value)
{
	return TRUE;
}



int32 ParamsConsumer::CheckParams(void)
{
    int32 iStatus = CFE_SUCCESS;
    CFE_SB_Msg_t* MsgPtr = 0;
    CFE_SB_MsgId_t MsgId = 0;
    osalbool contProcessing = TRUE;
    PRMLIB_ParamRegistration_t paramDataOut[PRMLIB_MAX_REGISTRATIONS];
    uint32 paramsOut = 0;

    /* Process param messages until the pipe is empty */
    while (contProcessing)
    {
        iStatus = CFE_SB_RcvMsg(&MsgPtr, ParamPipeId, CFE_SB_POLL);
        if(iStatus == CFE_SUCCESS)
        {
            /* We did receive a message.  Process it. */
            MsgId = CFE_SB_GetMsgId(MsgPtr);
            switch (MsgId)
            {
                case PRMLIB_PARAM_UPDATED_MID:
                {
                    /* We did receive a parameter request.  Process it. */
                	PRMLIB_UpdatedParamMsg_t *paramMsg = (PRMLIB_UpdatedParamMsg_t*)MsgPtr;

                	PRMLIB_ParamRegistration_t *paramRegistration = GetParamRegistration(paramMsg->name);
                	if(paramRegistration != 0)
                	{
                        osalbool isValid;
                		uint8 newValue[8];
                		int8 val_int8 		= 0;
                		uint16 val_uint16 	= 0;
                		int16 val_int16		= 0;
                		uint32 val_uint32	= 0;
                		int32 val_int32 	= 0;
                		float val_float		= 0.0;

                        iStatus = PRMLIB_GetParamValueById(paramMsg->name, (cpuaddr*)newValue);
                        if(iStatus != CFE_SUCCESS)
                        {
                        	contProcessing = FALSE;
                        }

                        switch(paramRegistration->Type)
                        {
                            case TYPE_UINT8:
                            {
                        		uint8 typedValue = 0;
                                CFE_PSP_MemCpy(&typedValue, newValue, sizeof(typedValue));
                                isValid = onParamValidate(paramMsg->name, typedValue);
                                if(isValid == TRUE)
                                {
                                    isValid = onParamValidate(paramRegistration->Value, typedValue);
                                    if(isValid == TRUE)
                                    {
                                        CFE_PSP_MemCpy(paramRegistration->Value, &typedValue, sizeof(typedValue));
                                    }
                                }
                                break;
                            }

                            case TYPE_INT8:
                            {
                        		int8 typedValue = 0;
                                CFE_PSP_MemCpy(&typedValue, newValue, sizeof(typedValue));
                                isValid = onParamValidate(paramMsg->name, typedValue);
                                if(isValid == TRUE)
                                {
                                    isValid = onParamValidate(paramRegistration->Value, typedValue);
                                    if(isValid == TRUE)
                                    {
                                        CFE_PSP_MemCpy(paramRegistration->Value, &typedValue, sizeof(typedValue));
                                    }
                                }
                                break;
                            }

                            case TYPE_UINT16:
                            {
                        		uint16 typedValue = 0;
                                CFE_PSP_MemCpy(&typedValue, newValue, sizeof(typedValue));
                                isValid = onParamValidate(paramMsg->name, typedValue);
                                if(isValid == TRUE)
                                {
                                    isValid = onParamValidate(paramRegistration->Value, typedValue);
                                    if(isValid == TRUE)
                                    {
                                        CFE_PSP_MemCpy(paramRegistration->Value, &typedValue, sizeof(typedValue));
                                    }
                                }
                                break;
                            }

                            case TYPE_INT16:
                            {
                        		int16 typedValue = 0;
                                CFE_PSP_MemCpy(&typedValue, newValue, sizeof(typedValue));
                                isValid = onParamValidate(paramMsg->name, typedValue);
                                if(isValid == TRUE)
                                {
                                    isValid = onParamValidate(paramRegistration->Value, typedValue);
                                    if(isValid == TRUE)
                                    {
                                        CFE_PSP_MemCpy(paramRegistration->Value, &typedValue, sizeof(typedValue));
                                    }
                                }
                                break;
                            }

                            case TYPE_UINT32:
                            {
                        		uint32 typedValue = 0;
                                CFE_PSP_MemCpy(&typedValue, newValue, sizeof(typedValue));
                                isValid = onParamValidate(paramMsg->name, typedValue);
                                if(isValid == TRUE)
                                {
                                    isValid = onParamValidate(paramRegistration->Value, typedValue);
                                    if(isValid == TRUE)
                                    {
                                        CFE_PSP_MemCpy(paramRegistration->Value, &typedValue, sizeof(typedValue));
                                    }
                                }
                                break;
                            }

                            case TYPE_INT32:
                            {
                        		int32 typedValue = 0;
                                CFE_PSP_MemCpy(&typedValue, newValue, sizeof(typedValue));
                                isValid = onParamValidate(paramMsg->name, typedValue);
                                if(isValid == TRUE)
                                {
                                    isValid = onParamValidate(paramRegistration->Value, typedValue);
                                    if(isValid == TRUE)
                                    {
                                        CFE_PSP_MemCpy(paramRegistration->Value, &typedValue, sizeof(typedValue));
                                    }
                                }
                                break;
                            }

                            case TYPE_REAL32:
                            {
                        		float typedValue = 0;
                                CFE_PSP_MemCpy(&typedValue, newValue, sizeof(typedValue));
                                isValid = onParamValidate(paramMsg->name, typedValue);
                                if(isValid == TRUE)
                                {
                                    isValid = onParamValidate(paramRegistration->Value, typedValue);
                                    if(isValid == TRUE)
                                    {
                                        CFE_PSP_MemCpy(paramRegistration->Value, &typedValue, sizeof(typedValue));
                                    }
                                }
                                break;
                            }

                            default:
                            {
                        		isValid = FALSE;
                                break;
                            }
                        }

                        if(isValid == TRUE)
                        {
                            CFE_PSP_MemCpy(&paramDataOut[paramsOut], paramRegistration, sizeof(paramDataOut[paramsOut]));
                            paramsOut++;
                        }
                    }
                    break;
                }

                default:
                {
                    contProcessing = FALSE;
                    iStatus = -1;
                    break;
                }
            }
        }
        else if (iStatus == CFE_SB_NO_MESSAGE)
        {
            /* The pipe is empty.  Break the loop and continue on. */
            contProcessing = FALSE;
        }
        else
        {
            /* Something failed.  Quit the loop. */
            iStatus = -2;
            contProcessing = FALSE;
        }
    }

    if(paramsOut > 0)
    {
    	onParamsChange(paramDataOut, paramsOut);
    }

    return iStatus;
}



PRMLIB_ParamRegistration_t* ParamsConsumer::GetParamRegistration(const char *Name)
{
	PRMLIB_ParamRegistration_t *result = 0;

    for(uint32 i = 0; i < PRMLIB_MAX_REGISTRATIONS; ++i)
    {
    	if(strncmp(ParamRegistration[i].Name, Name, sizeof(ParamRegistration[i].Name)) == 0)
    	{
    		/* The name is the same.  Return this object. */
    		result = &ParamRegistration[i];
    	}
    }

    return result;
}
