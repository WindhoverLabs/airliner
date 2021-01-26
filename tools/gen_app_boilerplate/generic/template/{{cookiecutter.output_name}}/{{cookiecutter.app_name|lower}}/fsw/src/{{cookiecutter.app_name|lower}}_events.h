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
    
#ifndef {{cookiecutter.app_name}}_EVENTS_H
#define {{cookiecutter.app_name}}_EVENTS_H

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************
** Local Defines
*************************************************************************/


/* Event IDs
 * Conventions: _EID is the event identifier.  _EVT_CNT is the total number of
 * events and should always be last.  Events can be added before _EVT_CNT.
 * For long-term maintenance, consider not removing an event but replacing it
 * with an unused, reserved, enum to preserve the IDs later in the list. */
typedef enum {

/** \brief <tt> Value of zero is reserved, and should not be used. </tt> */
    {{cookiecutter.app_name}}_RESERVED_EID = 0,  /* Do not use this event ID */

/** \brief <tt> '{{cookiecutter.app_name}} - ' </tt>
**  \event <tt> '{{cookiecutter.app_name}} - ' </tt>
**  
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  TODO fill this in
**
*/
    {{cookiecutter.app_name}}_INF_EID,

/** \brief <tt> '{{cookiecutter.app_name}} Initialized. Version \%d.\%d.\%d' </tt>
**  \event <tt> '{{cookiecutter.app_name}} Initialized. Version \%d.\%d.\%d' </tt>
**  
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This event message is issued when the CFS {{cookiecutter.app_name}} Task has
**  completed initialization.
**
**  The first \c %d field contains the Application's Major Version Number
**  The second \c %d field contains the Application's Minor Version Number
**  The third \c %d field contains the Application's Revision Number
*/
    {{cookiecutter.app_name}}_INIT_INF_EID,

/** \brief <tt> '{{cookiecutter.app_name}} - ' </tt>
**  \event <tt> '{{cookiecutter.app_name}} - ' </tt>
**  
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This message is generated when a table is initialized.
**
*/
    {{cookiecutter.app_name}}_CONFIG_TABLE_INF_EID,

/** \brief <tt> '{{cookiecutter.app_name}} - ' </tt>
**  \event <tt> '{{cookiecutter.app_name}} - ' </tt>
**  
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This event message is issued when TBD
**
*/
    {{cookiecutter.app_name}}_CDS_INF_EID,

/** \brief <tt> '{{cookiecutter.app_name}} - Recvd $x cmd (%us)' </tt>
**  \event <tt> '{{cookiecutter.app_name}} - Recvd $x cmd (%us)' </tt>
**  
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This event message is issued when the CFS {{cookiecutter.app_name}} Task has
**  received and processed a command.
**
*/
    {{cookiecutter.app_name}}_CMD_INF_EID,

/** \brief <tt> '{{cookiecutter.app_name}} - ' </tt>
**  \event <tt> '{{cookiecutter.app_name}} - ' </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when TBD
**
*/
    {{cookiecutter.app_name}}_ERR_EID,

/** \brief <tt> '{{cookiecutter.app_name}} - ' </tt>
**  \event <tt> '{{cookiecutter.app_name}} - ' </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS {{cookiecutter.app_name}} Task has
**  had an error in initialization.
**
*/
    {{cookiecutter.app_name}}_INIT_ERR_EID,

/** \brief <tt> '{{cookiecutter.app_name}} - ' </tt>
**  \event <tt> '{{cookiecutter.app_name}} - ' </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS {{cookiecutter.app_name}} Task has
**  had an error with the configuration table.
**
*/
    {{cookiecutter.app_name}}_CONFIG_TABLE_ERR_EID,

/** \brief <tt> '{{cookiecutter.app_name}} - ' </tt>
**  \event <tt> '{{cookiecutter.app_name}} - ' </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS {{cookiecutter.app_name}} Task has
**  had an error in CDS.
**
*/
    {{cookiecutter.app_name}}_CDS_ERR_EID,

/** \brief <tt> '{{cookiecutter.app_name}} - $commandError' </tt>
**  \event <tt> '{{cookiecutter.app_name}} - $commandError' </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS {{cookiecutter.app_name}} Task has
**  had an error processing a command.
**
*/
    {{cookiecutter.app_name}}_CMD_ERR_EID,

/** \brief <tt> '{{cookiecutter.app_name}}: SB pipe read error (0x%08X), app will exit' </tt>
**  \event <tt> '{{cookiecutter.app_name}}: SB pipe read error (0x%08X), app will exit' </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS {{cookiecutter.app_name}} Task has
**  had an error reading from a pipe.
**
*/
    {{cookiecutter.app_name}}_PIPE_ERR_EID,

/** \brief <tt> '{{cookiecutter.app_name}} - Recvd invalid $type msgId (0x%04x)' </tt>
**  \event <tt> '{{cookiecutter.app_name}} - Recvd invalid $type msgId (0x%04x)' </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS {{cookiecutter.app_name}} Task has
**  received an invalid message ID.
**
*/
    {{cookiecutter.app_name}}_MSGID_ERR_EID,

/** \brief <tt> '{{cookiecutter.app_name}} - Rcvd invalid msgLen: msgId=0x%08X, cmdCode=%d, msgLen=%d, expectedLen=%d" </tt>
**  \event <tt> '{{cookiecutter.app_name}} - Rcvd invalid msgLen: msgId=0x%08X, cmdCode=%d, msgLen=%d, expectedLen=%d" </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS {{cookiecutter.app_name}} Task has
**  received a message with a bad length.
**
*/
    {{cookiecutter.app_name}}_MSGLEN_ERR_EID,


/** \brief <tt> This is a count of all the app events and should not be used. </tt> */
    {{cookiecutter.app_name}}_EVT_CNT
} {{cookiecutter.app_name}}_EventIds_t;


#ifdef __cplusplus
}
#endif

#endif /* {{cookiecutter.app_name}}_EVENTS_H */

/************************/
/*  End of File Comment */
/************************/
    
