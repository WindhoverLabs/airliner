#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "common_types.h"
#include "osapi.h"
#include "utassert.h"
#include "uttest.h"
#include "utbsp.h"

/* OS Constructs */

void TestTaskStartStopPart1 (void);
void TestTaskStartStopPart2 (void);
void SenderTaskPart1Main(void);
void SenderTaskPart2Main(void);
void ReceiverTaskMain(void);
uint32 GetReceiverNum(void);

#define RECEIVER_TASK_COUNT 		20
#define QUEUE_DEPTH	        		10
#define RECEIVER_TASK_STACK_SIZE 	16535
#define SENDER_TASK_STACK_SIZE		16535
#define CYCLE_COUNT					100
#define BURST_MAX					30
#define SENDER_TASK_PRIORITY		30
#define RECEIVER_TASK_PRIORITY		50

/* *************************************** MAIN ************************************** */
void OS_Application_Startup(void)
{
    if (OS_API_Init() != OS_SUCCESS)
    {
        UtAssert_Abort("OS_API_Init() failed");
    }

    UtTest_Add(TestTaskStartStopPart1, NULL, NULL, "Task Start and Stop Part 1");
    UtTest_Add(TestTaskStartStopPart2, NULL, NULL, "Task Start and Stop Part 2");

} /* end OS_Application Startup */


typedef struct
{
	uint32 Queue[RECEIVER_TASK_COUNT];
	uint32 ReceiverTask[RECEIVER_TASK_COUNT];
	uint32 BinSem;
	uint32 Mutex;
	uint32 Failures;
	uint32 ListenerFailures[RECEIVER_TASK_COUNT];
	uint32 ListenerMutex[RECEIVER_TASK_COUNT];
	uint32 ReceiverNum;
} SenderTaskData_t;

typedef struct
{
	uint32 taskID;
	uint32 value;
} TestMsg_t;


SenderTaskData_t SenderTaskData;


int randint(int n) {
  if ((n - 1) == RAND_MAX) {
    return rand();
  } else {
    // Chop off all of the values that would cause skew...
    long end = RAND_MAX / n; // truncate skew
    end *= n;

    // ... and ignore results from rand() that fall above that limit.
    // (Worst case the loop condition should succeed 50% of the time,
    // so we can expect to bail out of this loop pretty quickly.)
    int r;
    while ((r = rand()) >= end);

    return r % n;
  }
}


uint32 GetReceiverNum(void)
{
	uint32 returnNum = 0;

	OS_MutSemTake(SenderTaskData.Mutex);
	returnNum = SenderTaskData.ReceiverNum;

	SenderTaskData.ReceiverNum++;

	OS_MutSemGive(SenderTaskData.Mutex);

	return returnNum;
}


/* ********************************************** TASKS******************************* */

void SenderTaskPart1Main(void)
{
	uint32 i = 0;
	uint32 cycle = 0;
    int32 status;
    TestMsg_t testMsg;

    OS_printf("Starting Sender Task Part 1 (Delete queues before threads).\n");
    OS_TaskRegister();

    SenderTaskData.Failures = 0;
    SenderTaskData.ReceiverNum = 0;

    OS_printf("Creating mutex.\n");
	status = OS_MutSemCreate(&SenderTaskData.Mutex, "MUTEX", 0);
	UtAssert_True(status == OS_SUCCESS, "Sender Task: Error creating mutex.");

    OS_printf("Creating queue and child task pairs.\n");
    for(i = 0; i < RECEIVER_TASK_COUNT; ++i)
    {
    	char queueName[OS_MAX_API_NAME];
    	char taskName[OS_MAX_API_NAME];

    	/* Create queue to send messages to receiver task. */
    	snprintf(queueName, OS_MAX_API_NAME, "QUEUE_%u", i);
    	status = OS_QueueCreate(&SenderTaskData.Queue[i], queueName, QUEUE_DEPTH, sizeof(void*), 0);
    	UtAssert_True(status == OS_SUCCESS, "Sender Task: Error creating output queue #%u.\n", i);

		/* Create receiver task. */
    	snprintf(taskName, OS_MAX_API_NAME, "TASK_%u", i);
    	status = OS_TaskCreate(&SenderTaskData.ReceiverTask[i], taskName, ReceiverTaskMain, 0, RECEIVER_TASK_STACK_SIZE, RECEIVER_TASK_PRIORITY, 0);
    	UtAssert_True(status == OS_SUCCESS, "Sender Task: Error creating receiver task #%u.\n", i);
    }

    /* Wait for threads to start. */
    OS_TaskDelay(1000);

    /* Initialize the output messages */
    testMsg.taskID = 0;
    testMsg.value = 100;

    OS_printf("Created all queue and task pairs.  Starting main loop.\n");
    for(cycle = 0; cycle < CYCLE_COUNT; ++cycle)
    {
    	uint32 iMsg = 0;

    	/* Get a random number from 1 to BURST_MAX.  This will be the burst
    	 * rate, or the number of messages we send to the child threads
    	 * in a quick burst.  Keep in mind this may overflow queues.
    	 */
    	uint32 burstCount = randint(BURST_MAX-1) + 1;

    	/* Now fill the queues */
    	for(iMsg = 0; iMsg < burstCount; ++iMsg)
    	{
        	uint32 iReceiver = 0;
            for(iReceiver = 0; iReceiver < RECEIVER_TASK_COUNT; ++iReceiver)
            {
            	testMsg.taskID = iReceiver;
            	TestMsg_t *testMsgOut = (TestMsg_t*)malloc(sizeof(testMsg));
            	memcpy(testMsgOut, &testMsg, sizeof(testMsg));
            	status = OS_QueuePut(SenderTaskData.Queue[iReceiver], &testMsgOut, sizeof(testMsgOut), 0);
            	if((status != OS_SUCCESS) && (status != OS_QUEUE_FULL))
            	{
            		SenderTaskData.Failures++;
            		OS_printf("Sender Task: Error in QueuePut().  status=%i cycle=%u iMsg=%u iReceiver=%u.\n", status, cycle, iMsg, iReceiver);
            	}
            	if(status == OS_QUEUE_FULL)
            	{
            		free(testMsgOut);
            		break;
            	}
            }

        	testMsg.value++;
    	}
    	/* Now wait a bit for the queues to empty out. */
        OS_TaskDelay(100);
    }

	UtAssert_True(SenderTaskData.Failures == 0, "Sender Task: OS_QueuePut reported errors.\n", i);

    OS_printf("Deleting queues\n");
    for(i = 0; i < RECEIVER_TASK_COUNT; ++i)
    {
    	/* Delete queue to send messages to receiver task. */
    	status = OS_QueueDelete(SenderTaskData.Queue[i]);
    	UtAssert_True(status == OS_SUCCESS, "Sender Task: Error deleting queue #%u.\n", i);
    }

    /* Let this scenario soak a bit. */
    OS_TaskDelay(100);

    OS_printf("Deleting threads.\n");
    for(i = 0; i < RECEIVER_TASK_COUNT; ++i)
    {
		/* Delete receiver task. */
    	status = OS_TaskDelete(SenderTaskData.ReceiverTask[i]);
    	UtAssert_True(status == OS_SUCCESS, "Sender Task: Error deleting receiver task #%u.\n", i);
    }

    for(i = 0; i < RECEIVER_TASK_COUNT; ++i)
    {
    	status = OS_MutSemTake(SenderTaskData.ListenerMutex[i]);
    	UtAssert_True(status == OS_SUCCESS, "Failed to lock listener mutex %u\n", i);
    	status = OS_MutSemDelete(SenderTaskData.ListenerMutex[i]);
    	UtAssert_True(status == OS_SUCCESS, "Failed to delete listener mutex %u\n", i);
    }

    /* Now delete the mutex. */
	status = OS_MutSemDelete(SenderTaskData.Mutex);
	UtAssert_True(status == OS_SUCCESS, "Sender Task: Error deleting mutex.\n");

	/* Signal the original thread that we're ready for termination. */
	status = OS_BinSemGive(SenderTaskData.BinSem);
	UtAssert_True(status == OS_SUCCESS, "Sender Task: Error in OS_BinSemGive.\n");

	while(1)
	{
		OS_TaskDelay(1000);
	}
}



void SenderTaskPart2Main(void)
{
	uint32 i = 0;
	uint32 cycle = 0;
    int32 status;
    TestMsg_t testMsg;

    OS_printf("Starting Sender Task Part 2 (Delete threads before queues).\n");
    OS_TaskRegister();

    SenderTaskData.Failures = 0;
    SenderTaskData.ReceiverNum = 0;

    OS_printf("Creating mutex.\n");
	status = OS_MutSemCreate(&SenderTaskData.Mutex, "MUTEX", 0);
	UtAssert_True(status == OS_SUCCESS, "Sender Task: Error creating mutex.");

    OS_printf("Creating queue and child task pairs.\n");
    for(i = 0; i < RECEIVER_TASK_COUNT; ++i)
    {
    	char queueName[OS_MAX_API_NAME];
    	char taskName[OS_MAX_API_NAME];

    	/* Create queue to send messages to receiver task. */
    	snprintf(queueName, OS_MAX_API_NAME, "QUEUE_%u", i);
    	status = OS_QueueCreate(&SenderTaskData.Queue[i], queueName, QUEUE_DEPTH, sizeof(void*), 0);
    	UtAssert_True(status == OS_SUCCESS, "Sender Task: Error creating output queue #%u.\n", i);

		/* Create receiver task. */
    	snprintf(taskName, OS_MAX_API_NAME, "TASK_%u", i);
    	status = OS_TaskCreate(&SenderTaskData.ReceiverTask[i], taskName, ReceiverTaskMain, 0, RECEIVER_TASK_STACK_SIZE, RECEIVER_TASK_PRIORITY, 0);
    	UtAssert_True(status == OS_SUCCESS, "Sender Task: Error creating receiver task #%u.\n", i);
    }

    /* Wait for threads to start. */
    OS_TaskDelay(1000);

    /* Initialize the output messages */
    testMsg.taskID = 0;
    testMsg.value = 100;

    OS_printf("Created all queue and task pairs.  Starting main loop.\n");
	for(cycle = 0; cycle < CYCLE_COUNT; ++cycle)
	{
		uint32 iMsg = 0;

		/* Get a random number from 1 to BURST_MAX.  This will be the burst
		 * rate, or the number of messages we send to the child threads
		 * in a quick burst.  Keep in mind this may overflow queues.
		 */
		uint32 burstCount = randint(BURST_MAX-1) + 1;

		/* Now fill the queues */
		for(iMsg = 0; iMsg < burstCount; ++iMsg)
		{
			uint32 iReceiver = 0;
			for(iReceiver = 0; iReceiver < RECEIVER_TASK_COUNT; ++iReceiver)
			{
				testMsg.taskID = iReceiver;
				TestMsg_t *testMsgOut = (TestMsg_t*)malloc(sizeof(testMsg));
				memcpy(testMsgOut, &testMsg, sizeof(testMsg));
				status = OS_QueuePut(SenderTaskData.Queue[iReceiver], &testMsgOut, sizeof(testMsgOut), 0);
				if((status != OS_SUCCESS) && (status != OS_QUEUE_FULL))
				{
					SenderTaskData.Failures++;
					OS_printf("Sender Task: Error in QueuePut().  status=%i cycle=%u iMsg=%u iReceiver=%u.\n", status, cycle, iMsg, iReceiver);
				}
				if(status == OS_QUEUE_FULL)
				{
					free(testMsgOut);
					break;
				}
			}

			testMsg.value++;
		}
		/* Now wait a bit for the queues to empty out. */
		OS_TaskDelay(100);
	}

	UtAssert_True(SenderTaskData.Failures == 0, "Sender Task: OS_QueuePut reported errors.\n", i);

    OS_printf("Deleting threads.\n");
    for(i = 0; i < RECEIVER_TASK_COUNT; ++i)
    {
		/* Delete receiver task. */
    	status = OS_TaskDelete(SenderTaskData.ReceiverTask[i]);
    	UtAssert_True(status == OS_SUCCESS, "Sender Task: Error deleting receiver task #%u.\n", i);
    }

    /* Let this scenario soak a bit. */
    OS_TaskDelay(100);

    OS_printf("Deleting queues\n");
    for(i = 0; i < RECEIVER_TASK_COUNT; ++i)
    {
    	/* Delete queue to send messages to receiver task. */
    	status = OS_QueueDelete(SenderTaskData.Queue[i]);
    	UtAssert_True(status == OS_SUCCESS, "Sender Task: Error deleting queue #%u.\n", i);

    }

    for(i = 0; i < RECEIVER_TASK_COUNT; ++i)
    {
    	status = OS_MutSemTake(SenderTaskData.ListenerMutex[i]);
    	UtAssert_True(status == OS_SUCCESS, "Failed to lock listener mutex %u\n", i);
    	status = OS_MutSemDelete(SenderTaskData.ListenerMutex[i]);
    	UtAssert_True(status == OS_SUCCESS, "Failed to delete listener mutex %u\n", i);
    }

    /* Now delete the mutex. */
	status = OS_MutSemDelete(SenderTaskData.Mutex);
	UtAssert_True(status == OS_SUCCESS, "Sender Task: Error deleting mutex.\n");

	/* Signal the original thread that we're ready for termination. */
	status = OS_BinSemGive(SenderTaskData.BinSem);
	UtAssert_True(status == OS_SUCCESS, "Sender Task: Error in OS_BinSemGive.\n");

	while(1)
	{
		OS_TaskDelay(1000);
	}
}



void ReceiverTaskMain(void)
{
    uint32 status;
    uint32 receiverNum = 0;
    uint32 i = 0;
    uint32 maxCycle = 0;

    OS_TaskRegister();

    /* Get our receiver number so we know which queue to read. */
    receiverNum = GetReceiverNum();

    SenderTaskData.ListenerFailures[receiverNum] = 0;

    /* Create and lock a mutex now so we can test what happens with
     * the mutex when the thread is terminated.
     */
	char mutexName[OS_MAX_API_NAME];
	snprintf(mutexName, OS_MAX_API_NAME, "MUTEX_%u", receiverNum);
	status = OS_MutSemCreate(&SenderTaskData.ListenerMutex[receiverNum], mutexName, 0);
	UtAssert_True(status == OS_SUCCESS, "Receiver #%u failed to create mutex.  status=%i", receiverNum, status);
	status = OS_MutSemTake(SenderTaskData.ListenerMutex[receiverNum]);
	UtAssert_True(status == OS_SUCCESS, "Receiver #%u failed to lock mutex", receiverNum);

    /* Now start the main loop.  Loop for (CYCLE_COUNT*BURST_MAX)+1 to
     * ensure we end with an infinite pend while we wait for the sender thread to terminate
     * us.
     */
    maxCycle = (CYCLE_COUNT*BURST_MAX)+1;
    for(i = 0; i < maxCycle; ++i)
    {
        uint32 sizeCopied = 0;
        uint32 data_received = 0;

    	status = OS_QueueGet(SenderTaskData.Queue[receiverNum], (void*)&data_received, sizeof(data_received), &sizeCopied, OS_PEND);
    	if((status != OS_SUCCESS) && (status != OS_ERR_INVALID_ID))
    	{
    		SenderTaskData.ListenerFailures[receiverNum]++;
    		OS_printf("Listener #%u: Error calling OS_QueueGet.  Cycle=%u  status=%i\n", receiverNum, i, status);
    	}

    	if(status == OS_ERR_INVALID_ID)
    	{
    		break;
    	}

    	if(data_received == 0)
    	{
    		++SenderTaskData.ListenerFailures[receiverNum];
    		OS_printf("Listener #%u received null message.", receiverNum);
    	}
    	else
        {
            TestMsg_t *testMsg = (TestMsg_t*)data_received;
    		if(testMsg->taskID != receiverNum)
    		{
    			++SenderTaskData.ListenerFailures[receiverNum];
    			OS_printf("Listener #%u: Received incorrect message from %u\n", receiverNum, testMsg->taskID);
    		}
        	free(testMsg);
    	}
    }

    while(1)
    {
    	OS_TaskDelay(1000);
    }
}



void TestTaskStartStopPart1(void)
{
    int status;
    uint32 failures = 0;
    uint32 senderTask = 0;
    uint32 i = 0;
    uint32 validTaskCount = 0;
    uint32 queueID = 0;

	OS_printf("Creating Binary Semaphore\n");
	status = OS_BinSemCreate(&SenderTaskData.BinSem, "BINSEM", 0, 0);
	UtAssert_True(status == OS_SUCCESS, "Test Task Start Stop: Error in OS_BinSemCreate.\n");

	OS_printf("Creating Sender Task\n");
	status = OS_TaskCreate(&senderTask, "SENDER_TASK", SenderTaskPart1Main, 0, SENDER_TASK_STACK_SIZE, SENDER_TASK_PRIORITY, 0);
	UtAssert_True(status == OS_SUCCESS, "Test Task Start Stop: Error creating sender task.  err=#%i.\n", status);

	/* Now wait for the Sender Task to signal that its done and ready for
	 * termination. */
	OS_printf("Waiting for Sender Task to complete.\n");
	status = OS_BinSemTake(SenderTaskData.BinSem);
	UtAssert_True(status == OS_SUCCESS, "Test Task Start Stop: Error in OS_BinSemTake.\n");

	/* Now delete the sender task */
	status = OS_TaskDelete(senderTask);
	UtAssert_True(status == OS_SUCCESS, "Test Task Start Stop: Failed to delete the sender task.\n");

	for(i = 0; i < RECEIVER_TASK_COUNT; ++i)
	{
		UtAssert_True(SenderTaskData.ListenerFailures[i] == 0, "Receiver #%u encounter errors.\n", i);
	}

	/* Now ensure all tasks and queues were in fact deleted from the OSAL. */
	for(i = 0; i < RECEIVER_TASK_COUNT; ++i)
	{
    	char queueName[OS_MAX_API_NAME];
    	char taskName[OS_MAX_API_NAME];
    	uint32 taskID;
    	uint32 queueID;

    	/* Try to get the queue. */
    	snprintf(queueName, OS_MAX_API_NAME, "QUEUE_%u\n", i);
		status = OS_QueueGetIdByName(&queueID, queueName);
		UtAssert_True(status == OS_ERR_NAME_NOT_FOUND, "Queue %u not deleted.\n", i);

    	/* Try to get the task. */
    	snprintf(taskName, OS_MAX_API_NAME, "TASK_%u\n", i);
		status = OS_QueueGetIdByName(&queueID, taskName);
		UtAssert_True(status == OS_ERR_NAME_NOT_FOUND, "Task %u not deleted.\n", i);
	}

	status = OS_BinSemDelete(SenderTaskData.BinSem);
	UtAssert_True(status == OS_SUCCESS, "Test Task Start Stop: Error in OS_BinSemDelete.\n");

    /* Now make sure the sender task was deleted. */
	status = OS_QueueGetIdByName(&queueID, "SENDER_TASK");
	UtAssert_True(status == OS_ERR_NAME_NOT_FOUND, "SENDER_TASK not deleted.\n", i);

}/* end TestTasks */




void TestTaskStartStopPart2(void)
{
    int status;
    uint32 failures = 0;
    uint32 senderTask = 0;
    uint32 i = 0;
    uint32 validTaskCount = 0;
    uint32 queueID = 0;

	OS_printf("Creating Binary Semaphore\n");
	status = OS_BinSemCreate(&SenderTaskData.BinSem, "BINSEM", 0, 0);
	UtAssert_True(status == OS_SUCCESS, "Test Task Start Stop: Error in OS_BinSemCreate.\n");

	OS_printf("Creating Sender Task\n");
	status = OS_TaskCreate(&senderTask, "SENDER_TASK", SenderTaskPart2Main, 0, SENDER_TASK_STACK_SIZE, SENDER_TASK_PRIORITY, 0);
	UtAssert_True(status == OS_SUCCESS, "Test Task Start Stop: Error creating sender task.  err=#%i.\n", status);

	/* Now wait for the Sender Task to signal that its done and ready for
	 * termination. */
	OS_printf("Waiting for Sender Task to complete.\n");
	status = OS_BinSemTake(SenderTaskData.BinSem);
	UtAssert_True(status == OS_SUCCESS, "Test Task Start Stop: Error in OS_BinSemTake.\n");

	/* Now delete the sender task */
	status = OS_TaskDelete(senderTask);
	UtAssert_True(status == OS_SUCCESS, "Test Task Start Stop: Failed to delete the sender task.\n");

	for(i = 0; i < RECEIVER_TASK_COUNT; ++i)
	{
		UtAssert_True(SenderTaskData.ListenerFailures[i] == 0, "Receiver #%u encounter errors.\n", i);
	}

	/* Now ensure all tasks and queues were in fact deleted from the OSAL. */
	for(i = 0; i < RECEIVER_TASK_COUNT; ++i)
	{
    	char queueName[OS_MAX_API_NAME];
    	char taskName[OS_MAX_API_NAME];
    	uint32 taskID;
    	uint32 queueID;

    	/* Try to get the queue. */
    	snprintf(queueName, OS_MAX_API_NAME, "QUEUE_%u\n", i);
		status = OS_QueueGetIdByName(&queueID, queueName);
		UtAssert_True(status == OS_ERR_NAME_NOT_FOUND, "Queue %u not deleted.\n", i);

    	/* Try to get the queue. */
    	snprintf(taskName, OS_MAX_API_NAME, "TASK_%u\n", i);
		status = OS_QueueGetIdByName(&queueID, taskName);
		UtAssert_True(status == OS_ERR_NAME_NOT_FOUND, "Task %u not deleted.\n", i);
	}

	status = OS_BinSemDelete(SenderTaskData.BinSem);
	UtAssert_True(status == OS_SUCCESS, "Test Task Start Stop: Error in OS_BinSemDelete.\n");

    /* Now make sure the sender task was deleted. */
	status = OS_QueueGetIdByName(&queueID, "SENDER_TASK");
	UtAssert_True(status == OS_ERR_NAME_NOT_FOUND, "SENDER_TASK not deleted.\n", i);

}/* end TestTasks */


