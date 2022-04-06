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

#ifndef SLIP_H
#define SLIP_H

#include "cfe.h"

/* SLIP special character codes
 */
#define SLIP_END        	 0xC0    /* Indicates end of packet */
#define SLIP_ESC             0xDB    /* Indicates byte stuffing */
#define SLIP_ESC_END         0xDC    /* ESC ESC_END means END data byte */
#define SLIP_ESC_ESC         0xDD    /* ESC ESC_ESC means ESC data byte */

typedef enum
{
	SLIP_BUFFER_FULL_OK    =  1,
	SLIP_OK                =  0,
	SLIP_INVALID_ARGS_ERR  = -1,
	SLIP_BUFFER_FULL_ERR   = -2,
} SLIP_ReturnCode_t;


typedef enum
{
	SLIP_NOMINAL           = 0,
	SLIP_ENCODING_END      = 1,
	SLIP_ENCODING_ESC_ESC  = 2,
	SLIP_ENCODING_ESC_END  = 3
} SLIP_EncoderState_t;


typedef struct
{
	uint8               *Buffer;
	uint32               BufferSize;
	uint32               BytesInBuffer;
	SLIP_EncoderState_t  State;
} SLIP_EncoderHandle_t;


int32 SLIP_LibInit(void);
SLIP_ReturnCode_t SLIP_EncoderInit(SLIP_EncoderHandle_t *Encoder, uint8 *Buffer, uint32 Size);
SLIP_ReturnCode_t SLIP_EnqueueData(SLIP_EncoderHandle_t *Encoder, uint8 Data);



#endif

