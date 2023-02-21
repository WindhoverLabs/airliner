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

#include "cf_custom_hooks.h"
#include "cf_test_utils.h"

#include "cf_app.h"

#include <string.h>


uint32       ReadHookCalledCnt = 0;
boolean      ReadHook_IncomingFlag = FALSE;

os_dir_t     OpendirHookDir;

uint32       ReaddirHookReturnCnt = 0;
os_dirent_t  ReaddirHookDirEntry;

uint32       SemGetInfoHookCallCnt = 0;
uint32       SemGetIdByNameHookCallCnt = 0;

uint32       ZeroCopyGetPtrHookCallCnt = 0;
uint32       ZeroCopyGetPtrHookOffset = 0;

uint32       CFE_ES_GetPoolBufHookCallCnt = 0;

uint8        CFE_SB_ZeroCopyGetPtrHook_Buf[CFE_SB_MAX_SB_MSG_SIZE];
uint8        *BufPtrs[128];


/**************************************************************************
 * CF Custom Hook Functions
 **************************************************************************/

int32 CFE_ES_GetPoolBufHook(uint32 **BufPtr, CFE_ES_MemHandle_t HandlePtr,
                            uint32 Size)
{
    uint32  Offset;
    uint8   *BytePtr;

    Offset = (CFE_ES_GetPoolBufHookCallCnt * sizeof (CF_QueueEntry_t));

    BytePtr = CF_AppData.Mem.Partition;

    BytePtr += Offset;

    *BufPtr = (uint32 *)BytePtr;

    CFE_ES_GetPoolBufHookCallCnt++;

    return Size;
}


int32 CFE_ES_PutPoolBufHook(CFE_ES_MemHandle_t HandlePtr, uint32 *BufPtr)
{
    /* Note the actual memory pool has not been deallocated:
       the CFE_ES_GetPoolBufHookCallCnt remains the same value,
       but the CF_AppData.Hk.App.MemInUse will be reduced
       as it is supposed to be in the actual environment */
    memset(BufPtr, 0x00, sizeof(CF_QueueEntry_t));

    return sizeof(CF_QueueEntry_t);
}


int32 CFE_SB_ZeroCopyGetPtrHook(uint16 MsgSize,
                                    CFE_SB_ZeroCopyHandle_t *BufferHandle)
{
    int32 ret_val;

    *BufferHandle = ZeroCopyGetPtrHookCallCnt;

    BufPtrs[ZeroCopyGetPtrHookCallCnt] =
                  &CFE_SB_ZeroCopyGetPtrHook_Buf[ZeroCopyGetPtrHookOffset];

    ret_val = (int32)&BufPtrs[ZeroCopyGetPtrHookCallCnt];

    ZeroCopyGetPtrHookOffset += MsgSize;
    ZeroCopyGetPtrHookCallCnt ++;

    return (ret_val);
}


int32 OS_statHook(const char *path, os_fstat_t *filestats)
{
    filestats->st_size = TEST_FILE_SIZE;

    return OS_FS_SUCCESS;
}


int32 OS_FDGetInfoHook (int32 filedes, OS_FDTableEntry *fd_prop)
{
    strcpy(fd_prop->Path, TestPbDir0);
    strcat(fd_prop->Path, TestPbFile1);

    return OS_FS_SUCCESS;
}


int32 OS_readHook(int32  filedes, void *buffer, uint32 nbytes)
{
    int32 readbytes;

printf("##OS_readHook: entered(%u)\n", ReadHook_IncomingFlag);
    if (ReadHook_IncomingFlag == TRUE)
    {
        if ((ReadHookCalledCnt % 2) == 0)
        {
            readbytes = TEST_FILE_SIZE;
        }
        else
        {
            readbytes = 0;
        }
    }
    else
    {
        readbytes = TEST_FILE_SIZE;
    }

    ReadHookCalledCnt ++;

    return readbytes;
}


os_dirp_t OS_opendirHook(const char *path)
{
    strcpy(OpendirHookDir.DirEnt.d_name, path);
printf("!!!OS_opendirHook entered(d_name: %s\n", OpendirHookDir.DirEnt.d_name);
    OpendirHookDir.DirEnt.d_type = OS_DT_DIR;

    return (&OpendirHookDir);
}


os_dirent_t *OS_readdirHook (os_dirp_t directory)
{
    if (strcmp(directory->DirEnt.d_name, TestPbDir0) == 0)
    {
        if(ReaddirHookReturnCnt == 0)
        {
            strcpy(ReaddirHookDirEntry.d_name, ".");
            ReaddirHookDirEntry.d_type = OS_DT_DIR;
        }
        else if(ReaddirHookReturnCnt == 1)
        {
            strcpy(ReaddirHookDirEntry.d_name, "..");
            ReaddirHookDirEntry.d_type = OS_DT_DIR;
        }
        else if(ReaddirHookReturnCnt == 2)
        {
            strcpy(ReaddirHookDirEntry.d_name, TestPbFile3);
            ReaddirHookDirEntry.d_type = OS_DT_FILE;
        }
        else if(ReaddirHookReturnCnt == 3)
        {
            strcpy(ReaddirHookDirEntry.d_name,
              "ThisFilenameIsTooLongItExceeds64ThisFilenameIsTooLongItIs65charas");
            ReaddirHookDirEntry.d_type = 5;
        }
        else if(ReaddirHookReturnCnt == 4)
        {
            strcpy(ReaddirHookDirEntry.d_name,
              "ThisFilenameIsTooLongWhenThePathIsAttachedToIt.ItIs63Characters");
            ReaddirHookDirEntry.d_type = 5;
        }
        else if(ReaddirHookReturnCnt == 5)
        {
            strcpy(ReaddirHookDirEntry.d_name, TestPbFile2);
            ReaddirHookDirEntry.d_type = OS_DT_FILE;
        }
        else if(ReaddirHookReturnCnt == 6)
        {
            strcpy(ReaddirHookDirEntry.d_name, TestPbFile1);
            ReaddirHookDirEntry.d_type = OS_DT_FILE;
        }
        else
        {
            return NULL;
        }
    }
    else if (strcmp(directory->DirEnt.d_name, TestPbDir1) == 0)
    {
        return NULL;
    }
    else if (strcmp(directory->DirEnt.d_name, TestPbDir2) == 0)
    {
        if (ReaddirHookReturnCnt == 7)
        {
            strcpy(ReaddirHookDirEntry.d_name, TestPbFile4);
            ReaddirHookDirEntry.d_type = OS_DT_FILE;
        }
        else if (ReaddirHookReturnCnt == 8)
        {
            strcpy(ReaddirHookDirEntry.d_name, TestPbFile5);
            ReaddirHookDirEntry.d_type = OS_DT_FILE;
        }
        else if (ReaddirHookReturnCnt == 9)
        {
            strcpy(ReaddirHookDirEntry.d_name, TestPbFile6);
            ReaddirHookDirEntry.d_type = OS_DT_FILE;
        }
        else
        {
            return NULL;
        }
    }
    else
    {
        return NULL;
    }

    ReaddirHookReturnCnt++;

    return(&ReaddirHookDirEntry);
}


int32 OS_CountSemGetIdByNameHook(uint32 *sem_id, const char *sem_name)
{
    *sem_id = SemGetIdByNameHookCallCnt;
    SemGetIdByNameHookCallCnt ++;

    return OS_SUCCESS;
}


int32 OS_CountSemGetInfoHook(uint32 sem_id, OS_count_sem_prop_t *count_prop)
{
    count_prop->creator = 1;
    count_prop->value = 10 - SemGetInfoHookCallCnt;

    SemGetInfoHookCallCnt ++;

    return OS_SUCCESS;
}


void CFE_PSP_GetTimeHook(OS_time_t *LocalTime)
{
    int              iStatus;
    struct timespec  time;

    iStatus = clock_gettime(CLOCK_REALTIME, &time);
    if (iStatus == 0)
    {
        LocalTime->seconds = time.tv_sec;
        LocalTime->microsecs = time.tv_nsec / 1000;
    }

    return;
}


CFE_TIME_SysTime_t  CFE_TIME_GetTimeHook(void)
{
    int                 iStatus;
    CFE_TIME_SysTime_t  CfeTime;
    struct timespec     time;

    iStatus = clock_gettime(CLOCK_REALTIME, &time);
    if (iStatus == 0)
    {
        CfeTime.Seconds = time.tv_sec;
        CfeTime.Subseconds = time.tv_nsec / 1000;
    }

    return CfeTime;
}


void CFE_SB_TimeStampMsgHook(CFE_SB_MsgPtr_t MsgPtr)
{
    CFE_SB_SetMsgTime(MsgPtr, CFE_TIME_GetTime());

    return;
}