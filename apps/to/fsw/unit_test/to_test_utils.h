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

#ifndef TO_TEST_UTILS_H
#define TO_TEST_UTILS_H

/*
 * Includes
 */

#include "to_app.h"

extern TO_AppData_t  TO_AppData;

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Function Definitions
 */

void TO_Test_Setup_NoConfig(void);
void TO_Test_Setup_EmptyConfig(void);
void TO_Test_Setup_NoPQueueConfig(void);
void TO_Test_Setup_FullConfig1(void);
void TO_Test_Setup_FullConfig2(void);
//void TO_Test_Setup_InvalidConfig(void);
void TO_Test_TearDown(void);
void TO_Test_Setup_FullConfig6(void); /* Almost full TO Config table 143 message flows */
void TO_Test_Setup_FullConfig7(void); /* Full TO Config table 144 message flows */


#ifdef __cplusplus
}
#endif

#endif /* TO_TEST_UTILS_H */

