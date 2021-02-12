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

#include "pq_structs.h"
#include "pq_scheduler.h"
#include "pq_output_queue.h"


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Run the Scheduler algorithm                                     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void PQ_Scheduler_Run(PQ_ChannelData_t *Channel)
{
    uint32 i;
    PQ_PriorityQueue_t *pqueue = NULL;
    PQ_OutputQueue_t *oqueue   = NULL;
    int32 status = 0;
    void *buffer = NULL;
    uint32 nBytesCopied = 0;
    
    oqueue = &Channel->OutputQueue;
    
    if (NULL == Channel->ConfigTblPtr)
    {
        /* If there is no table data we can't access a priority queue. */
        return;
    }    

    for (i = 0; i < PQ_MAX_PRIORITY_QUEUES; ++i)
    {
        status = OS_SUCCESS;
        pqueue = &Channel->ConfigTblPtr->PriorityQueue[i];
        
        if (pqueue->State != PQ_PQUEUE_UNUSED)
        {
            if (Channel->DumpTbl.PriorityQueue[i].OSALQueueID != OS_MAX_QUEUES)
            {                
                while ((OS_SUCCESS == status) && (oqueue->CurrentlyQueuedCnt < PQ_OUTPUT_QUEUE_DEPTH))
                {
                    status =  OS_QueueGet(
                            Channel->DumpTbl.PriorityQueue[i].OSALQueueID,
                            &buffer, sizeof(buffer), &nBytesCopied, OS_CHECK);
                            
                    if (OS_SUCCESS == status)
                    {
                        Channel->DumpTbl.PriorityQueue[i].CurrentlyQueuedCnt--;
                        status = PQ_OutputQueue_QueueMsg(Channel, buffer);
                        if (CFE_SUCCESS == status)
                        {
                            oqueue->CurrentlyQueuedCnt++;
                            if (oqueue->HighwaterMark < oqueue->CurrentlyQueuedCnt)
                            {
                                oqueue->HighwaterMark = oqueue->CurrentlyQueuedCnt;
                            }
                        }
                    }
                }
            }
        }
    }
}
