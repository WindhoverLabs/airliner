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

#ifndef VM_PLATFORM_CFG_H
#define VM_PLATFORM_CFG_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** VM Platform Configuration Parameter Definitions
*/

/** \brief Mission specific version number for VM application
**  
**  \par Description:
**       An application version number consists of four parts:
**       major version number, minor version number, revision
**       number and mission specific revision number. The mission
**       specific revision number is defined here and the other
**       parts are defined in "amc_version.h".
**
**  \par Limits:
**       Must be defined as a numeric value that is greater than
**       or equal to zero.
*/
#define VM_MISSION_REV                (0)

/** \brief Pipe depth for the Scheduler pipe 
**
**  \par Limits:
**       minimum of 1, max of CFE_SB_MAX_PIPE_DEPTH.
*/
#define VM_SCH_PIPE_DEPTH             (2)

/** \brief Pipe name for the Scheduler pipe 
**
**  \par Limits:
**       Note, this name must fit in OS_MAX_API_NAME.
*/
#define VM_SCH_PIPE_NAME              ("VM_SCH_PIPE")

/** \brief The SB pend behavior type for the Scheduler pipe.
**
**  \par Limits:
**       One of: CFE_SB_POLL, CFE_SB_PEND_FOREVER, or the 
**       number of milliseconds to wait for a new message (recommended).
**       Note, using CFE_SB_PEND_FOREVER may cause an unresponsive
**       application if no messages arrive on this pipe.
*/
#define VM_SCH_PIPE_PEND_TIME         (2000)

/** \brief The number of WAKEUP messages to reserve on the Scheduler pipe.
**
**  \par Limits:
**       minimum of 1, max limited to CFE_SB_MAX_PIPE_DEPTH-1.  Note the
**       VM_WAKEUP_MID_MAX_MSG_COUNT and VM_SEND_HK_MID_MAX_MSG_COUNT
**       must be less than VM_SCH_PIPE_DEPTH.
*/
#define VM_WAKEUP_MID_MAX_MSG_COUNT   (1)

/** \brief The number of SEND_HK messages to reserve on the Scheduler pipe.
**
**  \par Limits:
**       minimum of 1, max of CFE_SB_MAX_PIPE_DEPTH.  Note the
**       VM_WAKEUP_MID_MAX_MSG_COUNT and VM_SEND_HK_MID_MAX_MSG_COUNT
**       must be less than VM_SCH_PIPE_DEPTH.
*/
#define VM_SEND_HK_MID_MAX_MSG_COUNT  (1)

/** \brief Pipe depth for the command pipe
**
**  \par Limits:
**       minimum of 1, max of CFE_SB_MAX_PIPE_DEPTH.
*/
#define VM_CMD_PIPE_DEPTH             (4)

/** \brief Pipe name for the Scheduler pipe 
**
**  \par Limits:
**       Note, this name must fit in OS_MAX_API_NAME.
*/
#define VM_CMD_PIPE_NAME              ("VM_CMD_PIPE")


/** \brief Pipe depth for the parameters pipe
**
**  \par Limits:
**       minimum of 1, max of CFE_SB_MAX_PIPE_DEPTH.
*/
#define VM_PARAM_PIPE_DEPTH             (4)

/** \brief Pipe name for the parameters pipe
**
**  \par Limits:
**       Note, this name must fit in OS_MAX_API_NAME.
*/
#define VM_PARAM_PIPE_NAME              ("VM_PARAM_PIPE")


/** \brief The config table default filename
**
**  \par Limits:
**       The length of each string, including the NULL terminator cannot exceed
**       the #OS_MAX_PATH_LEN value.
*/
#define VM_CONFIG_TABLE_FILENAME      ("/cf/apps/vm_config.tbl")

/** \brief The timeout value, in milliseconds, to wait for ES application startup sync.
**
**  \par Limits:
**       This parameter must be at least 1000 (ms).
*/
#define VM_STARTUP_TIMEOUT_MSEC    (1000)

/** \brief The timeout value, in microseconds, to wait until VM starts checking battery
**         voltage.
**
**  \par Limits:
**       None
*/
#define VM_BATTERY_VOLTAGE_CHECK_DELAY (6000000)

/** \brief The minimum battery voltage to be considered valid.
**
**  \par Limits:
**       Must be greater than or equal to FLT_EPSILON.
*/
#define VM_MINIMUM_VALID_BATTERY_VOLTAGE (2.0f)


#ifdef __cplusplus
}
#endif

#endif /* VM_PLATFORM_CFG_H */

/************************/
/*  End of File Comment */
/************************/
    
