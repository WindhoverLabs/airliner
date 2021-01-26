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

#include "prm_lib.h"
#include "prm_version.h"

PRMLIB_AppData_t PRMLIB_AppData;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* CFS Library Initialization Routine                              */
/* cFE requires that a library have an initialization routine      */ 
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 PRMLIB_LibInit(void)
{
	int32 Status = CFE_SUCCESS;

	/* Create mutex for param table */
	Status = OS_MutSemCreate(&PRMLIB_AppData.ParamTblMutex, PRMLIB_PARAM_TBL_MUTEX_NAME, 0);
	if (Status != CFE_SUCCESS)
	{
		goto PRMLIB_LibInit_Exit_Tag;
	}

	PRMLIB_InitDefaultParameters(); //TODO:Remove
PRMLIB_LibInit_Exit_Tag:
	if (Status == CFE_SUCCESS)
	{
		OS_printf ("PRMLIB Initialized.  Version %d.%d.%d.%d\n",
    		PRM_MAJOR_VERSION,
			PRM_MINOR_VERSION,
			PRM_REVISION,
			PRM_MISSION_REV);
	}

    return Status;
 
}/* End PRMLIB_LibInit */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/*       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void PRMLIB_InitDefaultParameters(void)
{
//	PRMLIB_ParamData_t param;
//	PRMLIB_ParamData_t param2;
//	PRMLIB_ParamData_t param3;
//
//	uint32 val = 899;
//	uint32 val2 = 999;
//	uint32 val3 = 1999;
//
//	strcpy(param.name, "PWN_DISARMED");
//	strcpy(param2.name, "PWN_MIN");
//	strcpy(param3.name, "PWN_MAX");
//	param.type = TYPE_UINT32;
//	param2.type = TYPE_UINT32;
//	param3.type = TYPE_UINT32;
//	PRMLIB_SetParamValue(&param, &val);
//	PRMLIB_SetParamValue(&param2, &val2);
//	PRMLIB_SetParamValue(&param3, &val3);
//
//	PRMLIB_ParamTblData_t e1 = {1, param};
//	PRMLIB_ParamTblData_t e2 = {1, param2};
//	PRMLIB_ParamTblData_t e3 = {1, param3};
//
//	PRMLIB_AppData.ParamTbl[0] = e1;
//	PRMLIB_AppData.ParamTbl[1] = e2;
//	PRMLIB_AppData.ParamTbl[2] = e3;
//
//	PRMLIB_UpdateParamCount();

}/* End PRMLIB_LibInit */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Add Parameter to Table			                               */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 PRMLIB_AddParam(PRMLIB_ParamData_t param)
{
	int32 Status = -1;
    int i = 0;

	/* Lock the mutex */
	OS_MutSemTake(PRMLIB_AppData.ParamTblMutex);

	/* Iterate over table to find first empty index */
	for(i = 0; i < PRMLIB_PARAM_TBL_MAX_ENTRY; ++i)
	{
		if (PRMLIB_AppData.ParamTbl[i].enabled == 0)
		{
			/* Update parameter message with current table index values */
			PRMLIB_AppData.ParamTbl[i].enabled = 1;
			memcpy(PRMLIB_AppData.ParamTbl[i].param_data.name, param.name,
					sizeof(param.name)); //need to clear string?
			PRMLIB_AppData.ParamTbl[i].param_data.type = param.type;
			PRMLIB_SetParamValue(&PRMLIB_AppData.ParamTbl[i].param_data, &param.value);
			Status = CFE_SUCCESS;
			break;
		}
	}

	/* Increment total parameter count */
	PRMLIB_AppData.ParamCount++;

	/* Unlock the mutex */
	OS_MutSemGive(PRMLIB_AppData.ParamTblMutex);

	return Status;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Get Parameter Data			                               */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 PRMLIB_GetParamData(PRMLIB_ParamData_t* InOutParam, uint16* ParamIndex, uint16* ParamCount)
{
	int32 Status = -1;
	uint16 idx = 0;
    int i = 0;

	/* Lock the mutex */
	OS_MutSemTake(PRMLIB_AppData.ParamTblMutex);

	/* Iterate over table to find parameter */
	for(i = 0; i < PRMLIB_PARAM_TBL_MAX_ENTRY; ++i)
	{
		/* Only check enabled parameters */
		if (PRMLIB_AppData.ParamTbl[i].enabled == 1)
		{
			if (PRMLIB_ParamsEqual(*InOutParam, PRMLIB_AppData.ParamTbl[i].param_data))
			{
				/* Update parameter message with current table index values */
				memcpy(&InOutParam->value, &PRMLIB_AppData.ParamTbl[i].param_data.value, PRMLIB_PARAM_VALUE_MAX_LEN);
				InOutParam->type = PRMLIB_AppData.ParamTbl[i].param_data.type;
				*ParamIndex = idx;
				Status = CFE_SUCCESS;
				break;
			}

			idx++;
		}
	}

	*ParamCount = PRMLIB_AppData.ParamCount;

	/* Unlock the mutex */
	OS_MutSemGive(PRMLIB_AppData.ParamTblMutex);

	return Status;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Get Parameter Value			                               */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 PRMLIB_GetParamDataAtIndex(PRMLIB_ParamData_t* InOutParam, uint16 ParamIndex)
{
	int32 Status = -1;
	uint16 idx = 0;
    int i = 0;

	/* Lock the mutex */
	OS_MutSemTake(PRMLIB_AppData.ParamTblMutex);

	/* Iterate over table to find parameter */
	for(i = 0; i < PRMLIB_PARAM_TBL_MAX_ENTRY; ++i)
	{
		/* Only check enabled parameters */
		if (PRMLIB_AppData.ParamTbl[i].enabled == 1)
		{
			if (idx == ParamIndex)
			{
				/* Update parameter message with current table index values */
				strcpy(InOutParam->name, PRMLIB_AppData.ParamTbl[i].param_data.name);
				memcpy(&InOutParam->value, &PRMLIB_AppData.ParamTbl[i].param_data.value, PRMLIB_PARAM_VALUE_MAX_LEN);
				InOutParam->type = PRMLIB_AppData.ParamTbl[i].param_data.type;
				Status = CFE_SUCCESS;
				break;
			}

			idx++;
		}
	}

	/* Unlock the mutex */
	OS_MutSemGive(PRMLIB_AppData.ParamTblMutex);

	return Status;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Test Parameter Equality			                               */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
boolean PRMLIB_ParamsEqual(PRMLIB_ParamData_t param1, PRMLIB_ParamData_t param2)
{
	boolean equal = TRUE;

	if (strcmp(param1.name, param2.name) != 0)
	{
		equal = FALSE;
		goto PRMLIB_ParamsEqual_Exit_Tag;
	}

PRMLIB_ParamsEqual_Exit_Tag:
	return equal;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Check if Parameter Exists		                               */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
boolean PRMLIB_ParamExists(char param_name[])
{
	boolean paramExists = FALSE;
    int i = 0;

	/* Lock the mutex */
	OS_MutSemTake(PRMLIB_AppData.ParamTblMutex);

	/* Iterate over table to find parameter */
	for(i = 0; i < PRMLIB_PARAM_TBL_MAX_ENTRY; ++i)
	{
		/* Only check enabled parameters */
		if (PRMLIB_AppData.ParamTbl[i].enabled == 1)
		{
			if (strcmp(param_name, PRMLIB_AppData.ParamTbl[i].param_data.name) == 0)
			{
				paramExists = TRUE;
				break;
			}
		}
	}

	/* Unlock the mutex */
	OS_MutSemGive(PRMLIB_AppData.ParamTblMutex);

	return paramExists;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Update Parameter Value			                               */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 PRMLIB_UpdateParam(PRMLIB_ParamData_t param_data)
{
	int32 Status = -1;
    int i = 0;

	/* Lock the mutex */
	OS_MutSemTake(PRMLIB_AppData.ParamTblMutex);

	/* Iterate over table to find parameter */
	for(i = 0; i < PRMLIB_PARAM_TBL_MAX_ENTRY; ++i)
	{
		/* Only check enabled parameters */
		if (PRMLIB_AppData.ParamTbl[i].enabled == 1)
		{
			if (PRMLIB_ParamsEqual(param_data, PRMLIB_AppData.ParamTbl[i].param_data))
			{
				/* Update parameter message with current table index values */
				memcpy(&PRMLIB_AppData.ParamTbl[i].param_data.value, &param_data.value, PRMLIB_PARAM_VALUE_MAX_LEN);
				PRMLIB_AppData.ParamTbl[i].param_data.type = param_data.type;
				Status = CFE_SUCCESS;
				break;
			}
		}
	}

	/* Unlock the mutex */
	OS_MutSemGive(PRMLIB_AppData.ParamTblMutex);

	return Status;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Remove Parameter 				                               */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 PRMLIB_RemoveParam(PRMLIB_ParamData_t param_data)
{
	int32 Status;
    int i = 0;

	/* Lock the mutex */
	OS_MutSemTake(PRMLIB_AppData.ParamTblMutex);

	/* Iterate over table to find parameter */
	for(i = 0; i < PRMLIB_PARAM_TBL_MAX_ENTRY; ++i)
	{
		/* Only check enabled parameters */
		if (PRMLIB_AppData.ParamTbl[i].enabled == 1)
		{
			if (PRMLIB_ParamsEqual(param_data, PRMLIB_AppData.ParamTbl[i].param_data))
			{
				/* Clear parameter data */
				//PRMLIB_AppData.ParamTbl[i].param_data = {"",0};
				PRMLIB_AppData.ParamTbl[i].enabled = 0; // Note: this could cause collisions
				Status = CFE_SUCCESS;
			}
		}
	}

	PRMLIB_AppData.ParamCount--;

	/* Unlock the mutex */
	OS_MutSemGive(PRMLIB_AppData.ParamTblMutex);

	return Status;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Update Total Parameter Count	                                   */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void PRMLIB_UpdateParamCount()
{
	uint16 count = 0;
    int i = 0;

	/* Lock the mutex */
	OS_MutSemTake(PRMLIB_AppData.ParamTblMutex);

	/* Iterate over table and count enabled entries */
	for(i = 0; i < PRMLIB_PARAM_TBL_MAX_ENTRY; ++i)
	{
		if (PRMLIB_AppData.ParamTbl[i].enabled == 1)
		{
			count++;
		}
	}

	PRMLIB_AppData.ParamCount = count;

	/* Unlock the mutex */
	OS_MutSemGive(PRMLIB_AppData.ParamTblMutex);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* 				                               */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
uint16 PRMLIB_GetParamCount()
{
	return PRMLIB_AppData.ParamCount;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* 				                               */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void PRMLIB_GetParams(PRMLIB_ParamData_t* params, uint16* ParamCount)
{
	uint16 idx = 0;
    int i = 0;

	/* Lock the mutex */
	OS_MutSemTake(PRMLIB_AppData.ParamTblMutex);

	/* Iterate over table and get all params */
	for(i = 0; i < PRMLIB_AppData.ParamCount; ++i)
	{
		if (PRMLIB_AppData.ParamTbl[i].enabled == 1)
		{
			strcpy(params[idx].name, PRMLIB_AppData.ParamTbl[i].param_data.name);
			params[idx].type = PRMLIB_AppData.ParamTbl[i].param_data.type;
			PRMLIB_SetParamValue(&params[idx], &PRMLIB_AppData.ParamTbl[i].param_data.value);
			idx++;
		}
	}

	*ParamCount = PRMLIB_AppData.ParamCount;

	/* Unlock the mutex */
	OS_MutSemGive(PRMLIB_AppData.ParamTblMutex);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* 				                               */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 PRMLIB_GetParamById(const char name[], PRMLIB_ParamData_t* InOutParam)
{
	int32 Status = -1;
    int i = 0;

	/* Lock the mutex */
	OS_MutSemTake(PRMLIB_AppData.ParamTblMutex);

	/* Iterate over table and get all params */
	for(i = 0; i < PRMLIB_AppData.ParamCount; ++i)
	{
		if (PRMLIB_AppData.ParamTbl[i].enabled == 1)
		{
			if (strcmp(name, PRMLIB_AppData.ParamTbl[i].param_data.name) == 0)
			{
				InOutParam = &PRMLIB_AppData.ParamTbl[i].param_data;
				Status = CFE_SUCCESS;
			}
		}
	}

	/* Unlock the mutex */
	OS_MutSemGive(PRMLIB_AppData.ParamTblMutex);

	return Status;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* 				                               */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 PRMLIB_GetParamValueById(const char name[], void* InOutValue)
{
	int32 Status = -1;
    int i = 0;

	/* Lock the mutex */
	OS_MutSemTake(PRMLIB_AppData.ParamTblMutex);

	/* Iterate over table and get all params */
	for(i = 0; i < PRMLIB_AppData.ParamCount; ++i)
	{
		if (PRMLIB_AppData.ParamTbl[i].enabled == 1)
		{
			if (strcmp(name, PRMLIB_AppData.ParamTbl[i].param_data.name) == 0)
			{
				PRMLIB_GetParamValue(PRMLIB_AppData.ParamTbl[i].param_data, InOutValue);
				Status = CFE_SUCCESS;
			}
		}
	}

	/* Unlock the mutex */
	OS_MutSemGive(PRMLIB_AppData.ParamTblMutex);

	return Status;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* 				                               */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 PRMLIB_ParamRegister(const char name[], void* inOutValue, PRMLIB_ParamType_t type)
{
	int32 Status = -1;
	boolean ParamExists = FALSE;
	PRMLIB_ParamData_t param = {0};
    int i = 0;

	/* Lock the mutex */
	OS_MutSemTake(PRMLIB_AppData.ParamTblMutex);

	/* Iterate over table and get all params */
	for(i = 0; i < PRMLIB_AppData.ParamCount; ++i)
	{
		if (PRMLIB_AppData.ParamTbl[i].enabled == 1)
		{
			if (strcmp(name, PRMLIB_AppData.ParamTbl[i].param_data.name) == 0)
			{
				PRMLIB_GetParamValue(PRMLIB_AppData.ParamTbl[i].param_data, inOutValue);
				ParamExists = TRUE;
				Status = CFE_SUCCESS;
			}
		}
	}

	if(ParamExists == FALSE)
	{
		strcpy(param.name, name);
		param.type = type;
		PRMLIB_SetParamValue(&param, inOutValue);
		Status = PRMLIB_AddParam(param);
	}

	/* Unlock the mutex */
	OS_MutSemGive(PRMLIB_AppData.ParamTblMutex);

	return Status;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* 				                               */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void PRMLIB_PrintParam(PRMLIB_ParamData_t param)
{
	uint8 val_uint8 	= 0;
	int8 val_int8 		= 0;
	uint16 val_uint16 	= 0;
	int16 val_int16		= 0;
	uint32 val_uint32	= 0;
	int32 val_int32 	= 0;
	float val_float		= 0.0;

	switch(param.type)
	{
		case TYPE_UINT8:
			memcpy(&val_uint8, param.value, sizeof(uint8));
			break;
		case TYPE_INT8:
			memcpy(&val_int8, param.value, sizeof(int8));
			break;
		case TYPE_UINT16:
			memcpy(&val_uint16, param.value, sizeof(uint16));
			break;
		case TYPE_INT16:
			memcpy(&val_int16, param.value, sizeof(int16));
			break;
		case TYPE_UINT32:
			memcpy(&val_uint32, &param.value, sizeof(uint32));
			break;
		case TYPE_INT32:
			memcpy(&val_int32, param.value, sizeof(int32));
			break;
		case TYPE_REAL32:
			memcpy(&val_float, param.value, sizeof(float));
			break;
		default:
			OS_printf("Unsupported type encountered: %u\n", param.type);
			break;
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* 				                               */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void PRMLIB_GetParamValue(PRMLIB_ParamData_t param, void* val)
{
	uint8 val_uint8 	= 0;
	int8 val_int8		= 0;
	uint16 val_uint16	= 0;
	int16 val_int16		= 0;
	uint32 val_uint32	= 0;
	int32 val_int32 	= 0;
	float val_float		= 0.0;

	switch(param.type)
	{
		case TYPE_UINT8:
			memcpy(&val_uint8, param.value, sizeof(uint8));
			memcpy(val, &val_uint8, sizeof(uint8));
			break;
		case TYPE_INT8:
			memcpy(&val_int8, param.value, sizeof(int8));
			memcpy(val, &val_int8, sizeof(int8));
			break;
		case TYPE_UINT16:
			memcpy(&val_uint16, param.value, sizeof(uint16));
			memcpy(val, &val_uint16, sizeof(uint16));
			break;
		case TYPE_INT16:
			memcpy(&val_int16, param.value, sizeof(int16));
			memcpy(val, &val_int16, sizeof(int16));
			break;
		case TYPE_UINT32:
			memcpy(&val_uint32, param.value, sizeof(uint32));
			memcpy(val, &val_uint32, sizeof(uint32));
			break;
		case TYPE_INT32:
			memcpy(&val_int32, param.value, sizeof(int32));
			memcpy(val, &val_int32, sizeof(int32));
			break;
		case TYPE_REAL32:
			memcpy(&val_float, param.value, sizeof(float));
			memcpy(val, &val_float, sizeof(float));
			break;
		default:
			OS_printf("Unsupported type encountered: %u\n", param.type);
			break;
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* 				                               */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void PRMLIB_SetParamValue(PRMLIB_ParamData_t* param, void* val)
{
	uint8 val_uint8 	= 0;
	int8 val_int8 		= 0;
	uint16 val_uint16 	= 0;
	int16 val_int16		= 0;
	uint32 val_uint32	= 0;
	int32 val_int32 	= 0;
	float val_float		= 0.0;

	switch(param->type)
	{
		case TYPE_UINT8:
			memcpy(&val_uint8, val, sizeof(uint8));
			memcpy(param->value, &val_uint8, sizeof(uint8));
			break;
		case TYPE_INT8:
			memcpy(&val_int8, val, sizeof(int8));
			memcpy(param->value, &val_int8, sizeof(int8));
			break;
		case TYPE_UINT16:
			memcpy(&val_uint16, val, sizeof(uint16));
			memcpy(param->value, &val_uint16, sizeof(uint16));
			break;
		case TYPE_INT16:
			memcpy(&val_int16, val, sizeof(int16));
			memcpy(param->value, &val_int16, sizeof(int16));
			break;
		case TYPE_UINT32:
			memcpy(&val_uint32, val, sizeof(uint32));
			memcpy(param->value, &val_uint32, sizeof(val_uint32));
			break;
		case TYPE_INT32:
			memcpy(&val_int32, val, sizeof(int32));
			memcpy(param->value, &val_int32, sizeof(int32));
			break;
		case TYPE_REAL32:
			memcpy(&val_float, val, sizeof(float));
			memcpy(param->value, &val_float, sizeof(float));
			break;
		default:
			OS_printf("Unsupported type encountered: %u\n", param->type);
			break;
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* 				                               */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void PRMLIB_SetValue(void* destVal, void* srcVal, PRMLIB_ParamType_t type)
{
	uint8 val_uint8 	= 0;
	int8 val_int8 		= 0;
	uint16 val_uint16 	= 0;
	int16 val_int16		= 0;
	uint32 val_uint32	= 0;
	int32 val_int32 	= 0;
	float val_float		= 0.0;

	switch(type)
	{
		case TYPE_UINT8:
			memcpy(&val_uint8, srcVal, sizeof(uint8));
			memcpy(destVal, &val_uint8, sizeof(uint8));
			break;
		case TYPE_INT8:
			memcpy(&val_int8, srcVal, sizeof(int8));
			memcpy(destVal, &val_int8, sizeof(int8));
			break;
		case TYPE_UINT16:
			memcpy(&val_uint16, srcVal, sizeof(uint16));
			memcpy(destVal, &val_uint16, sizeof(uint16));
			break;
		case TYPE_INT16:
			memcpy(&val_int16, srcVal, sizeof(int16));
			memcpy(destVal, &val_int16, sizeof(int16));
			break;
		case TYPE_UINT32:
			memcpy(&val_uint32, srcVal, sizeof(uint32));
			memcpy(destVal, &val_uint32, sizeof(val_uint32));
			break;
		case TYPE_INT32:
			memcpy(&val_int32, srcVal, sizeof(int32));
			memcpy(destVal, &val_int32, sizeof(int32));
			break;
		case TYPE_REAL32:
			memcpy(&val_float, srcVal, sizeof(float));
			memcpy(destVal, &val_float, sizeof(float));
			break;
		default:
			OS_printf("Unsupported type encountered: %u\n", type);
			break;
	}
}

/************************/
/*  End of File Comment */
/************************/
