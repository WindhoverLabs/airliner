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


uint32       ReaddirHookCallCnt = 0;
os_dirent_t  ReaddirHookDirEntry;

uint32       CFE_ES_GetPoolBufHookCallCnt = 0;


/**************************************************************************
 * CF Custom Hook Functions
 **************************************************************************/

int32 CFE_ES_GetPoolBufHook(uint32 **BufPtr, CFE_ES_MemHandle_t HandlePtr,
                            uint32 Size)
{
    uint32  Offset;
    uint8   *BytePtr;
printf("###CFE_ES_GetPoolBufHook entered: HookCallCnt(%lu)\n", CFE_ES_GetPoolBufHookCallCnt);

    Offset = (CFE_ES_GetPoolBufHookCallCnt * sizeof (CF_QueueEntry_t));

    BytePtr = (uint8 *)&CF_AppData.Mem.Partition;

    BytePtr += Offset;

    *BufPtr = (uint32 *)BytePtr;

    CFE_ES_GetPoolBufHookCallCnt++;

    return Size;
}


int32 CFE_ES_PutPoolBufHook(CFE_ES_MemHandle_t HandlePtr, uint32 *BufPtr)
{
printf("###CFE_ES_PutPoolBufHook entered\n");

    /* Note the actual memory pool has not been deallocated:
       the CFE_ES_GetPoolBufHookCallCnt remains the same value,
       but the CF_AppData.Hk.App.MemInUse will be reduced
       as it is supposed to be in the actual environment */
    memset(BufPtr, 0x00, sizeof(CF_QueueEntry_t));

    return sizeof(CF_QueueEntry_t);
}


int32 OS_statHook(const char *path, os_fstat_t *filestats)
{
    filestats->st_size = 123;

    return OS_FS_SUCCESS;
}


int32 OS_FDGetInfoHook (int32 filedes, OS_FDTableEntry *fd_prop)
{
    strcpy(fd_prop->Path, TestPbDir);
    strcat(fd_prop->Path, TestPbFile1);

    return OS_FS_SUCCESS;
}


os_dirent_t *  OS_readdirHook (os_dirp_t directory)
{
printf("###OS_readdirHook entered:HookCallCnt(%lu)\n", ReaddirHookCallCnt);
    ReaddirHookCallCnt++;

    if(ReaddirHookCallCnt == 1)
    {
        strcpy(ReaddirHookDirEntry.d_name, ".");
        ReaddirHookDirEntry.d_type = OS_DT_DIR;
    }
    else if(ReaddirHookCallCnt == 2)
    {
        strcpy(ReaddirHookDirEntry.d_name, "..");
        ReaddirHookDirEntry.d_type = OS_DT_DIR;
    }
    else if(ReaddirHookCallCnt == 3)
    {
        strcpy(ReaddirHookDirEntry.d_name, TestPbFile3);
        ReaddirHookDirEntry.d_type = OS_DT_FILE;
    }
    else if(ReaddirHookCallCnt == 4)
    {
        strcpy(ReaddirHookDirEntry.d_name,
           "ThisFilenameIsTooLongItExceeds64ThisFilenameIsTooLongItIs65charas");
        ReaddirHookDirEntry.d_type = 5;
    }
    else if(ReaddirHookCallCnt == 5)
    {
        strcpy(ReaddirHookDirEntry.d_name,
           "ThisFilenameIsTooLongWhenThePathIsAttachedToIt.ItIs63Characters");
        ReaddirHookDirEntry.d_type = 5;
    }
    else if(ReaddirHookCallCnt == 6)
    {
        strcpy(ReaddirHookDirEntry.d_name, TestPbFile2);
        ReaddirHookDirEntry.d_type = OS_DT_FILE;
    }
    else if(ReaddirHookCallCnt == 7)
    {
        strcpy(ReaddirHookDirEntry.d_name, TestPbFile1);
        ReaddirHookDirEntry.d_type = OS_DT_FILE;
    }
    else
    {
        return NULL;
    }

    return(&ReaddirHookDirEntry);
}


void Test_CF_GetPSPTimeHook(OS_time_t *LocalTime)
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


CFE_TIME_SysTime_t  Test_CF_GetCFETimeHook(void)
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


void Test_CF_SBTimeStampMsgHook(CFE_SB_MsgPtr_t MsgPtr)
{
    CFE_SB_SetMsgTime(MsgPtr, CFE_TIME_GetTime());

    return;
}
