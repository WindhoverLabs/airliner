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

#include "to_app.h"
#include "to_scheduler.h"
#include "to_output_queue.h"


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Run the Scheduler algorithm                                     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void TO_Scheduler_Run(TO_ChannelData_t *channel)
{
    uint32 i;
    int32 status = 0;
    CFE_SB_MsgPtr_t msgPtr = NULL;
    uint32 nBytesCopied = 0;

    if (NULL == channel->ConfigTblPtr)
    {
        /* If there is no table data we can't access a priority queue. */
        return;
    }    

    for (i = 0; i < TO_MAX_PRIORITY_QUEUES; ++i)
    {
        status = OS_SUCCESS;
        osalbool term = FALSE;

        while(!term)
        {
			if(FALSE == TO_OutputQueue_IsFull(channel))
			{
				if (&channel->ConfigTblPtr->PriorityQueue[i].State != TO_PQUEUE_UNUSED)
				{
					status = TO_PriorityQueue_Get(channel, i, &msgPtr);
					if (OS_SUCCESS == status)
					{
						TO_OutputQueue_QueueMsg(channel, msgPtr);
					}
					else
					{
						term = TRUE;
					}
				}
			}
			else
			{
				term = TRUE;
			}
        }
    }
}
