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

#ifndef FPC_VERSION_H
#define FPC_VERSION_H

#ifdef __cplusplus
extern "C" {
#endif

/** \brief Major version number for FPC application
 **
 **  \par Description:
 **       An application version number consists of four parts:
 **       major version number, minor version number, revision
 **       number and mission specific revision number. The mission
 **       specific revision number is defined in "ci_platform_cfg.h".
 **
 **  \par Limits:
 **       Must be defined as a numeric value that is greater than
 **       or equal to zero.
 */
#define FPC_MAJOR_VERSION    (0)

/** \brief Minor version number for FPC application
 **
 **  \par Description:
 **       An application version number consists of four parts:
 **       major version number, minor version number, revision
 **       number and mission specific revision number. The mission
 **       specific revision number is defined in "fpc_platform_cfg.h".
 **
 **  \par Limits:
 **       Must be defined as a numeric value that is greater than
 **       or equal to zero.
 */
#define FPC_MINOR_VERSION    (0)

/** \brief Revision version number for FPC application
 **
 **  \par Description:
 **       An application version number consists of four parts:
 **       major version number, minor version number, revision
 **       number and mission specific revision number. The mission
 **       specific revision number is defined in "fpc_platform_cfg.h".
 **
 **  \par Limits:
 **       Must be defined as a numeric value that is greater than
 **       or equal to zero.
 */
#define FPC_REVISION         (0)

#ifdef __cplusplus
}
#endif

#endif /* FPC_VERSION_H */

/************************/
/*  End of File Comment */
/************************/
