#include "mailbox_map.h"
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/uio.h>
#include <sys/types.h>
#include "uio_mailbox.h"
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <poll.h>
#include <stdint.h>
//#include <netdb.h>
//#include <sys/types.h>


int UIOMB_FD = 0;

/* Open and memory map uio. */
void *UIOMB_InitDevice(const char *path, unsigned int size, unsigned int blocking_delay)
{
    void     *mailbox_ptr = 0;
    uint32_t  unmask = 1;
    ssize_t   rv;

    UIOMB_FD = open(path, O_RDWR);
    if (UIOMB_FD < 0)
    {
        /* TODO update to event. */
        printf("ERROR: Invalid UIO device file:%s.\n", path);
        goto end_of_function;
    }

    mailbox_ptr = mmap(0, size, PROT_READ|PROT_WRITE, MAP_SHARED, UIOMB_FD, 0);
    if (MAP_FAILED == mailbox_ptr)
    {
        /* TODO update to event. */
    	printf("ERROR: mmap call failure. errno %d\n", errno);
        goto end_of_function;
    }
    else
    {
        /* TODO update to event. */
        printf("Opened UIO at 0x%08x\n", mailbox_ptr);
    }

    /* Set Send Threshold    (SIT) */
    UIOMB_UIO_Write(mailbox_ptr, MAILBOX_SEND_INT_REG, 4);

    /* Set Receive Threshold (RIT) */
    UIOMB_UIO_Write(mailbox_ptr, MAILBOX_RECV_INT_REG, 4);

    /* Reset FIFO Q's*/
    UIOMB_UIO_Write(mailbox_ptr, MAILBOX_INT_ENABLE_REG, MAILBOX_INT_RTA_BIT);

    /* Unmask interrupts. */
    rv = write(UIOMB_FD, &unmask, sizeof(unmask));
    if (rv != (ssize_t)sizeof(unmask))
    {
        printf("ERROR:  Failed to unmask UIO interrupts.\n");
        close(UIOMB_FD);
        mailbox_ptr = 0;
    }

    /* Set Interrupt Enable */
    UIOMB_UIO_Write(mailbox_ptr, MAILBOX_RECV_INT_REG, MAILBOX_INT_RTA_BIT);

end_of_function:
    return mailbox_ptr;
}



bool UIOMB_MB_IsFull(void *instance)
{
    unsigned int reg = 0;

    reg = UIOMB_UIO_Read(instance, MAILBOX_STATUS_REG);

    return reg & MAILBOX_STATUS_FIFO_FULL_BIT;
}



bool UIOMB_MB_IsFullError(void *instance)
{
    unsigned int reg = 0;

    reg = UIOMB_UIO_Read(instance, MAILBOX_ERROR_REG);

    return reg & MAILBOX_ERROR_FULL_BIT;
}



bool UIOMB_MB_IsEmpty(void *instance)
{
    unsigned int reg = 0;

    reg = UIOMB_UIO_Read(instance, MAILBOX_STATUS_REG);

    return reg & MAILBOX_STATUS_FIFO_EMPTY_BIT;
}



bool UIOMB_MB_IsEmptyError(void *instance)
{
    unsigned int reg = 0;

    reg = UIOMB_UIO_Read(instance, MAILBOX_ERROR_REG);

    return reg & MAILBOX_ERROR_EMPTY_BIT;
}



bool UIOMB_MB_Write(void *instance, const unsigned int *buffer, unsigned int size, unsigned int blocking_delay)
{
    unsigned int i = 0;

    /*
     * Attempt to write to the mailbox until "size" has been written.
     */
    for(i = 0; i < size/4; ++i)
    {
        /* While the mailbox is full. */
        while(UIOMB_MB_IsFull(instance))
        {
            /* Sleep to allow the receiver to read from the mailbox. */
            usleep(blocking_delay);
        }

        /* Write one word at a time and increment the buffer pointer. */
        UIOMB_UIO_Write(instance, MAILBOX_WRITE_REG, *buffer++);
    }

    return i;
}



uint32_t UIOMB_MB_Read(void *instance, unsigned int *buffer, unsigned int size)
{
	uint32_t status = 0;
    unsigned int i = 0;

    /*
     * Attempt to read the mailbox until the buffer is full or the
     * mailbox is empty.
     */
    for(i = 0; i < size/4; ++i)
    {
        bool isEmpty   = false;

        isEmpty = UIOMB_MB_IsEmpty(instance);
        if(isEmpty == true)
        {
            /* Break out of the loop and return size read. */
            break;
        }

        /* Read one word and increment the buffer pointer. */
        *buffer++ = UIOMB_UIO_Read(instance, MAILBOX_READ_REG);
        /* Set the return status to the size read so far in bytes. */
        status = i + 1;
        /* Check if the mailbox is empty. */
    }

    /* Convert from words to bytes. */
    status = status * 4;

    return status;
}




bool UIOMB_UIO_WaitForInterrupt(int timeout_ms)
{
	bool occurred = false;

    // wait for the interrupt
    struct pollfd pfd = {.fd = UIOMB_FD, .events = POLLIN};
    int rv = poll(&pfd, 1, timeout_ms);
    // clear the interrupt
    if (rv >= 1)
    {
        //UIOMB_UIO_Write(instance, MAILBOX_INT_STATUS_REG, regStatus);
    	int reenable = 1;
    	read(UIOMB_FD, (void *)&reenable, sizeof(int));   // &reenable -> &pending
    	occurred = true;

    	write(UIOMB_FD, (void *)&reenable, sizeof(int));
    }
    else if (rv == 0)
    {
        // this indicates a timeout, will be caught by device busy flag
    	occurred = false;
    }
    else
    {
        printf("ERROR:  WaitForInterrupt\n");
    	occurred = false;
    }

    return occurred;
}



/* Write to shared memory. */
void UIOMB_UIO_Write(void *instance, const unsigned int offset, unsigned int value)
{
    *((volatile unsigned *)((unsigned long)instance + offset)) = value;
}

/* Read from shared memory. */
unsigned int UIOMB_UIO_Read(void *instance, unsigned int offset)
{
    return *((volatile unsigned *)((unsigned long)instance + offset));
}






