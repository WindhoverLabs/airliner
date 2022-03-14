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
#include "cfe.h"

#include "cvt_lib.h"
#include "cvt_version.h"
#include <string.h>
#include "cvt_private_types.h"
#include "osapi.h"


CVT_DataStoreTable_t CVT_DataStoreTable;


int32 CVT_LibInit(void)
{
    OS_printf ("CVT Lib Initialized.  Version %d.%d.%d",
    		CVT_MAJOR_VERSION,
			CVT_MINOR_VERSION,
			CVT_REVISION);

    return OS_SUCCESS;
}



int32 CVT_GetContainer(char *Name, uint32 Size, CVT_ContainerID_t *ContainerID)
{
	int32  status = CVT_SUCCESS;
	uint32 i = 0;

	if(Name == 0)
	{
		status = CVT_INVALID_ID;
		goto end_of_function;
	}

	/* Try to find the name already registered. */
	for(i = 0; i < CVT_MAX_REGISTRATIONS; ++i)
	{
		/* Is this entry the same name? */
		if(strcmp(Name, CVT_DataStoreTable.Registry.Registration[i].Name) == 0)
		{
			/* It is the same name.  Is the registered size the same size? */
			if(Size != CVT_DataStoreTable.Registry.Registration[i].Size)
			{
				/* This is not the correct size. */
				status = CVT_INCORRECT_SIZE;
				goto end_of_function;
			}
			else
			{
				/* It is the same size.  Return this container ID. */
				status = CVT_SUCCESS;
				*ContainerID = i;
				goto end_of_function;
			}
		}
	}

	/* If we got this far, we didn't find an entry with the same name.  Find an empty one. */
	for(i = 0; i < CVT_MAX_REGISTRATIONS; ++i)
	{
		/* Is this name empty? */
		if(0 == CVT_DataStoreTable.Registry.Registration[i].Size)
		{
			/* This is empty.  Do we have enough room to allocate from the data store? */
			if(CVT_DataStoreTable.Registry.Cursor + Size <= CVT_DATA_STORE_SIZE)
			{
				char semName[15];

				/* Yes.  Now allocate space by setting the offset and moving the cursor. */
				CVT_DataStoreTable.Registry.Registration[i].Offset = CVT_DataStoreTable.Registry.Cursor;
				CVT_DataStoreTable.Registry.Cursor += Size;

				/* Create mutex. */
				sprintf(semName, "CVT_%u", i);
				OS_MutSemCreate(&CVT_DataStoreTable.Registry.Registration[i].MutexID, semName, 0);
			}
			else
			{
				/* There isn't enough room in the data store. */
				status = CVT_DATASTORE_FULL;
				goto end_of_function;
			}

			break;
		}
	}

	/* Check to see if we found any empty value. */
	if(i == CVT_MAX_REGISTRATIONS)
	{
		/* No.  The registry is full. */
		status = CVT_REGISTRY_FULL;
		goto end_of_function;
	}

	/* Now store the name and size. */
	strncpy(CVT_DataStoreTable.Registry.Registration[i].Name, Name, CVT_CONTAINER_NAME_MAX_LENGTH);
	CVT_DataStoreTable.Registry.Registration[i].Size = Size;
	*ContainerID = i;
	status = CVT_SUCCESS;

end_of_function:

	return status;
}



int32 CVT_GetContent(const CVT_ContainerID_t ContainerID, uint32 *UpdateCount, void* Buffer, uint32 *Size)
{
	int32  status = CVT_SUCCESS;
	uint32 i = 0;

	if(ContainerID >= CVT_MAX_REGISTRATIONS)
	{
		status = CVT_INVALID_ID;
		goto end_of_function;
	}

	if(CVT_DataStoreTable.Registry.Registration[ContainerID].Size == 0)
	{
		status = CVT_INVALID_ID;
		goto end_of_function;

	}

	if(CVT_DataStoreTable.Registry.Registration[ContainerID].Size != *Size)
	{
		status = CVT_INVALID_SIZE;
		goto end_of_function;
	}

	OS_MutSemTake(CVT_DataStoreTable.Registry.Registration[ContainerID].MutexID);
	*UpdateCount = CVT_DataStoreTable.Registry.Registration[ContainerID].UpdateCount;
	*Size = CVT_DataStoreTable.Registry.Registration[i].Size;
	CFE_PSP_MemCpy(Buffer, &CVT_DataStoreTable.DataStore[CVT_DataStoreTable.Registry.Registration[i].Offset], *Size);
	OS_MutSemGive(CVT_DataStoreTable.Registry.Registration[ContainerID].MutexID);

	status = CVT_SUCCESS;

end_of_function:

    return status;
}



int32 CVT_SetContent(const CVT_ContainerID_t ContainerID, void* Buffer, uint32 Size)
{
	int32  status = CVT_SUCCESS;
	uint32 i = 0;

	if(ContainerID >= CVT_MAX_REGISTRATIONS)
	{
		status = CVT_INVALID_ID;
		goto end_of_function;
	}

	if(CVT_DataStoreTable.Registry.Registration[ContainerID].Size == 0)
	{
		status = CVT_INVALID_ID;
		goto end_of_function;

	}

	if(CVT_DataStoreTable.Registry.Registration[ContainerID].Size != Size)
	{
		status = CVT_INVALID_SIZE;
		goto end_of_function;
	}

	OS_MutSemTake(CVT_DataStoreTable.Registry.Registration[ContainerID].MutexID);
	CFE_PSP_MemCpy(&CVT_DataStoreTable.DataStore[CVT_DataStoreTable.Registry.Registration[i].Offset], Buffer, Size);
	CVT_DataStoreTable.Registry.Registration[ContainerID].UpdateCount++;
	OS_MutSemGive(CVT_DataStoreTable.Registry.Registration[ContainerID].MutexID);

	status = CVT_SUCCESS;

end_of_function:

    return status;
}




/************************/
/*  End of File Comment */
/************************/
