#define _SVID_SOURCE

#include "sedlib_custom.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "sedlib_private.h"
#include <errno.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

//sudo SEDLIB_SHMEM_KEY_FILE=/home/mbenson/git/squeaky-weasel/software/airliner/public/build/obc/cpd/sitl/target/target/exe/.sedlib_shmem ./simlink
extern SEDLIB_ConfigTable_t SEDLIB_ConfigTable;


int32 SEDLIB_CustomInit(void)
{
	key_t key;
	int shmid;
	int32 status = CFE_SUCCESS;
	char *keyFile;
	int   tempFd;

    keyFile = getenv(SEDLIB_SHMEM_KEY_FILE_ENV_VAR_NAME);
    if(0 == keyFile)
	{
        OS_printf("SEDLIB:  %s environment variable not defined.  Using '%s'.\n", SEDLIB_SHMEM_KEY_FILE_ENV_VAR_NAME, SEDLIB_DEFAULT_SHMEM_KEY_FILE);
        keyFile = SEDLIB_DEFAULT_SHMEM_KEY_FILE;
    }

    tempFd = open(keyFile,  O_RDWR|O_SYNC | O_CREAT, S_IRWXU );
    close(tempFd);

	/* Make the key: */
	key = ftok(keyFile, 'R');
	if(key < 0)
	{
		OS_printf("SEDLIB:  ftok failed (%i)\n", errno);
	    exit(-1);
	}

    /*  Create the segment: */
	shmid = shmget(key, SEDLIB_SHM_SIZE, 0644 | IPC_CREAT);
	if(shmid < 0)
	{
		OS_printf("SEDLIB:  shmget failed (%i)\n", errno);
        exit(-1);
    }

	/* Attach to the segment to get a pointer to it: */
	SEDLIB_ConfigTable.BaseAddress = (uint32)shmat(shmid, NULL, 0);
	if (SEDLIB_ConfigTable.BaseAddress == (uint32)(-1))
	{
		OS_printf("SEDLIB:  shmat failed (%i)\n", errno);
	    exit(-1);
	}
	else
	{
		OS_printf("SEDLIB:  Using shared memory at 0x%08lx\n", SEDLIB_ConfigTable.BaseAddress);
	}

end_of_function:
    return status;

}
