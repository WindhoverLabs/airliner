#include "sbnd_app.h"
#include "ipc_lib.h"
#include "cfe.h"

#include <errno.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SBND_READ_SHM_SIZE     (0x800000)
#define SBND_WRITE_SHM_SIZE    (0x800000)

typedef struct
{
    uint8  TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint32 ReadCount;
    uint32 WriteCount;
    uint32 ReadErrors;
    uint32 WriteErrors;
} SBND_Shm_HkTlm_t;


typedef struct
{
    void *InstanceWrite;
    void *InstanceRead;
    uint32 MsgRingHandleWrite;
    uint32 MsgRingHandleRead;
    uint8 RecvBuf[65535];
    SBND_Shm_HkTlm_t HkTlm;
    IPC_Lib_ParserHandle_t ParseHandle;
} SBND_Shm_Data_t;


SBND_Shm_Data_t SBND_Shm_Data;


int32 SBND::InitCustom(void)
{
    uint32 ReadMsgRingHandle = 0;
    uint32 WriteMsgRingHandle = 0;
    key_t readKey;
    key_t writeKey;
	int readShmid;
	int writeShmid;
	int32 status = CFE_SUCCESS;
	char *readKeyFile;
	char *writeKeyFile;
	int   tempReadFd;
	int   tempWriteFd;

    readKeyFile = getenv(SBND_READ_SHMEM_KEY_FILE_ENV_VAR_NAME);
    if(0 == readKeyFile)
	{
        OS_printf("SBND:  %s environment variable not defined.  Using '%s'.\n", SBND_READ_SHMEM_KEY_FILE_ENV_VAR_NAME, SBND_DEFAULT_READ_SHMEM_KEY_FILE);
        readKeyFile = SBND_DEFAULT_READ_SHMEM_KEY_FILE;
    }

    writeKeyFile = getenv(SBND_WRITE_SHMEM_KEY_FILE_ENV_VAR_NAME);
    if(0 == writeKeyFile)
	{
        OS_printf("SBND:  %s environment variable not defined.  Using '%s'.\n", SBND_WRITE_SHMEM_KEY_FILE_ENV_VAR_NAME, SBND_DEFAULT_WRITE_SHMEM_KEY_FILE);
        writeKeyFile = SBND_DEFAULT_WRITE_SHMEM_KEY_FILE;
    }

    tempReadFd = open(readKeyFile,  O_RDWR|O_SYNC | O_CREAT, S_IRWXU );
    tempWriteFd = open(writeKeyFile,  O_RDWR|O_SYNC | O_CREAT, S_IRWXU );

	/* Make the key: */
    readKey = ftok(readKeyFile, 'R');
	if(readKey < 0)
	{
		OS_printf("SBND:  Read ftok failed (%i)\n", errno);
	    exit(-1);
	}

	writeKey = ftok(writeKeyFile, 'R');
	if(writeKey < 0)
	{
		OS_printf("SBND:  Write ftok failed (%i)\n", errno);
	    exit(-1);
	}

    /*  Create the segment: */
	readShmid = shmget(readKey, SBND_READ_SHM_SIZE, 0644 | IPC_CREAT);
	if(readShmid < 0)
	{
		OS_printf("SBND:  Read shmget failed (%i)\n", errno);
        exit(-1);
    }

	writeShmid = shmget(writeKey, SBND_WRITE_SHM_SIZE, 0644 | IPC_CREAT);
	if(writeShmid < 0)
	{
		OS_printf("SBND:  Write shmget failed (%i)\n", errno);
        exit(-1);
    }

	/* Attach to the segment to get a pointer to it: */
	SBND_Shm_Data.InstanceRead = (void*)shmat(readShmid, NULL, 0);
	if (SBND_Shm_Data.InstanceRead < (uint32)(0))
	{
		OS_printf("SBND:  Read shmat failed (%i)\n", errno);
	    exit(-1);
	}
	else
	{
		OS_printf("SBND:  Using (read) shared memory at 0x%08lx\n", SBND_Shm_Data.InstanceRead);
	}

	SBND_Shm_Data.InstanceWrite = (void*)shmat(writeShmid, NULL, 0);
	if (SBND_Shm_Data.InstanceWrite < (uint32)(0))
	{
		OS_printf("SBND:  Write shmat failed (%i)\n", errno);
	    exit(-1);
	}
	else
	{
		OS_printf("SBND:  Using (write) shared memory at 0x%08lx\n", SBND_Shm_Data.InstanceWrite);
	}

    //close(tempReadFd);
    //close(tempWriteFd);

    status = IPC_Lib_MsgRing_Init(SBND_Shm_Data.InstanceWrite,
    		                       SBND_WRITE_SHM_SIZE,
                                   &WriteMsgRingHandle);
    if(status != IPC_LIB_SUCCESS)
    {
        OS_printf("IPC_Lib_MsgRing_Init failed %d.\n", status);
        status = -1;
        goto end_of_function;
    }

    SBND_Shm_Data.MsgRingHandleWrite = WriteMsgRingHandle;

    status = IPC_Lib_MsgRing_Init(SBND_Shm_Data.InstanceRead,
                                   SBND_READ_SHM_SIZE,
                                   &ReadMsgRingHandle);
    if(status != IPC_LIB_SUCCESS)
    {
        OS_printf("IPC_Lib_MsgRing_Init failed %d.\n", status);
        status = -1;
        goto end_of_function;
    }

    SBND_Shm_Data.MsgRingHandleRead = ReadMsgRingHandle;

    /* TODO update to event. */
    OS_printf("SBND using shared write memory at 0x%08x\n", SBND_Shm_Data.InstanceWrite);
    /* TODO update to event. */
    OS_printf("SBND using shared read memory at 0x%08x\n", SBND_Shm_Data.InstanceRead);

end_of_function:
    return status;
}


int32 SBND::ReadCustom(void)
{
    int32 status = 0;
    uint32 Status = 0;
    uint8 Byte = 0;
    uint32 Size = sizeof(SBND_Shm_Data.RecvBuf);

    while(1)
    {
        status = IPC_Lib_MsgRing_Read(SBND_Shm_Data.MsgRingHandleRead, (uint8 *)&Byte, sizeof(Byte));
        if(status == IPC_LIB_QUEUE_EMPTY)
        {
            status = 0;
            goto end_of_function;
        }

        if(status != IPC_LIB_SUCCESS)
        {
            OS_printf("IPC_Lib_MsgRing_ReadMsg returned error %d\n", status);
            SBND_Shm_Data.HkTlm.ReadErrors++;
            goto end_of_function;
        }

        Status = IPC_Lib_ParseMessage(&SBND_Shm_Data.ParseHandle,
                                      Byte,
                                      &SBND_Shm_Data.RecvBuf[0],
                                      &Size);
        if(Status == IPC_LIB_MESSAGE_COMPLETE)
        {
            break;
        }
    }

    /* Put on the bus */
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&SBND_Shm_Data.RecvBuf[0]);
    CFE_SB_SendMsg((CFE_SB_Msg_t*)&SBND_Shm_Data.RecvBuf[0]);
    status = CFE_SB_GetTotalMsgLength((CFE_SB_Msg_t*)&SBND_Shm_Data.RecvBuf[0]);

end_of_function:
    return status;
}


int32 SBND::WriteCustom(uint8 *buffer, uint16 size)
{
    int32  status = 0;

    /* Attempt to send. */
    status = IPC_Lib_MsgRing_WriteParser(SBND_Shm_Data.MsgRingHandleWrite, buffer, size);
    if(status == IPC_LIB_QUEUE_FULL)
    {
        status = 0;
        goto end_of_function;
    }

    if(status != IPC_LIB_SUCCESS)
    {
        OS_printf("IPC_Lib_MsgRing_Write failed %d.\n", status);
        SBND_Shm_Data.HkTlm.WriteErrors++;
        goto end_of_function;
    }
    
    SBND_Shm_Data.HkTlm.WriteCount++;

end_of_function:
    return status;
}
