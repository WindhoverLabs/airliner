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
#ifndef FLOW_VERIFY_H
#define FLOW_VERIFY_H


#ifndef FLOW_GYRO_PIPE_DEPTH
	#error FLOW_GYRO_PIPE_DEPTH must be defined.
#elif (FLOW_GYRO_PIPE_DEPTH  <  1)
    #error FLOW_GYRO_PIPE_DEPTH cannot be less than 1!
#elif (FLOW_GYRO_PIPE_DEPTH  >  CFE_SB_MAX_PIPE_DEPTH)
    #error FLOW_GYRO_PIPE_DEPTH cannot be greater than CFE_SB_MAX_PIPE_DEPTH!
#endif

#ifndef FLOW_GYRO_PIPE_NAME
    #error FLOW_GYRO_PIPE_NAME must be defined!
#endif

#ifndef FLOW_SCH_PIPE_DEPTH
	#error FLOW_SCH_PIPE_DEPTH must be defined.
#elif (FLOW_SCH_PIPE_DEPTH  <  1)
    #error FLOW_SCH_PIPE_DEPTH cannot be less than 1!
#elif (FLOW_SCH_PIPE_DEPTH  >  CFE_SB_MAX_PIPE_DEPTH)
    #error FLOW_SCH_PIPE_DEPTH cannot be greater than CFE_SB_MAX_PIPE_DEPTH!
#endif

#ifndef FLOW_SCH_PIPE_NAME
    #error FLOW_SCH_PIPE_NAME must be defined!
#endif

#ifndef FLOW_CMD_PIPE_DEPTH
	#error FLOW_CMD_PIPE_DEPTH must be defined.
#elif (FLOW_CMD_PIPE_DEPTH  <  1)
    #error FLOW_CMD_PIPE_DEPTH cannot be less than 1!
#elif (FLOW_CMD_PIPE_DEPTH  >  CFE_SB_MAX_PIPE_DEPTH)
    #error FLOW_CMD_PIPE_DEPTH cannot be greater than CFE_SB_MAX_PIPE_DEPTH!
#endif

#ifndef FLOW_CMD_PIPE_NAME
    #error FLOW_CMD_PIPE_NAME must be defined!
#endif

#ifndef FLOW_SENSOR_GYRO_MAX_MSG_COUNT
	#error FLOW_SENSOR_GYRO_MAX_MSG_COUNT must be defined!
#elif (FLOW_SENSOR_GYRO_MAX_MSG_COUNT  <  1)
    #error FLOW_SENSOR_GYRO_MAX_MSG_COUNT cannot be less than 1!
#elif (FLOW_SENSOR_GYRO_MAX_MSG_COUNT  >  FLOW_GYRO_PIPE_DEPTH)
    #error FLOW_SENSOR_GYRO_MAX_MSG_COUNT cannot be greater than FLOW_GYRO_PIPE_DEPTH!
#endif

#ifndef FLOW_WAKEUP_MID_MAX_MSG_COUNT
	#error FLOW_WAKEUP_MID_MAX_MSG_COUNT must be defined.
#elif (FLOW_WAKEUP_MID_MAX_MSG_COUNT  <  1)
    #error FLOW_WAKEUP_MID_MAX_MSG_COUNT cannot be less than 1!
#elif (FLOW_WAKEUP_MID_MAX_MSG_COUNT  >  CFE_SB_MAX_PIPE_DEPTH-1)
    #error FLOW_WAKEUP_MID_MAX_MSG_COUNT cannot be greater than CFE_SB_MAX_PIPE_DEPTH-1!
#endif


#endif /* FLOW_VERIFY_H */

/************************/
/*  End of File Comment */
/************************/
