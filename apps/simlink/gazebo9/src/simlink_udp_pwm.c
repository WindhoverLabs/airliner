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
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <string.h>
#include <stdbool.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>
#include "simlink_udp_pwm.h"

#define SIMLINK_PWM_UDP_RECV_PORT    (9000)
#define SIMLINK_PWM_UDP_RECV_ADDRESS ("127.0.0.1")
#define SIMLINK_PWM_MAX_CHANNELS     (4)
#define SIMLINK_PWM_BUFFER_SIZE      (1500)


typedef enum
{
    SIMLINK_PWM_PARSE_STATE_UNKNOWN = 0,
    SIMLINK_PWM_PARSE_STATE_SYNC1   = 1,
    SIMLINK_PWM_PARSE_STATE_CH1     = 2,
    SIMLINK_PWM_PARSE_STATE_PARSE1  = 3,
    SIMLINK_PWM_PARSE_STATE_SYNC2   = 4,
    SIMLINK_PWM_PARSE_STATE_CH2     = 5,
    SIMLINK_PWM_PARSE_STATE_PARSE2  = 6,
    SIMLINK_PWM_PARSE_STATE_SYNC3   = 7,
    SIMLINK_PWM_PARSE_STATE_CH3     = 8,
    SIMLINK_PWM_PARSE_STATE_PARSE3  = 9,
    SIMLINK_PWM_PARSE_STATE_SYNC4   = 10,
    SIMLINK_PWM_PARSE_STATE_CH4     = 11,
    SIMLINK_PWM_PARSE_STATE_PARSE4  = 12
} SIMLINK_Pwm_Parse_State_t;


typedef struct
{
    SIMLINK_Pwm_Parse_State_t State;
    char                      Buffer[SIMLINK_PWM_BUFFER_SIZE];
    char                      ParseBuffer[SIMLINK_PWM_BUFFER_SIZE];
    unsigned int              Channel[SIMLINK_PWM_MAX_CHANNELS];
    int                       Socket;
    int                       Count;
} SIMLINK_Pwm_Data_t;


SIMLINK_Pwm_Data_t SIMLINK_Pwm_Data;


int SIMLINK_Pwm_Recv_Socket(const char * Buffer, unsigned int Size);
void SIMLINK_Pwm_Parse_StateReset(void);
bool SIMLINK_Pwm_ParseChar(char byte);


bool SIMLINK_Pwm_Setup_Recv_Socket(const char *Address, unsigned int Port)
{
    bool               returnBool = true;
    int                reuseaddr  = 1;
    int                status     = 0;
    struct sockaddr_in servaddr   = {0};

    SIMLINK_Pwm_Data.Socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(SIMLINK_Pwm_Data.Socket < 0)
    {
        returnBool = false;
        goto end_of_function;
    }

    setsockopt(SIMLINK_Pwm_Data.Socket, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(reuseaddr));
    bzero((void*)&servaddr, sizeof(servaddr));
    servaddr.sin_family      = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port        = htons(SIMLINK_PWM_UDP_RECV_PORT);

    status = bind(SIMLINK_Pwm_Data.Socket, (struct sockaddr *)&servaddr, sizeof(servaddr));
    if(status < 0)
    {
        returnBool = false;
        goto end_of_function;
    }

end_of_function:

    return returnBool;
}


int SIMLINK_Pwm_Recv_Socket(const char * Buffer, unsigned int Size)
{
    int returnCode = 0;

    returnCode = recv(SIMLINK_Pwm_Data.Socket, (char *)Buffer, (size_t)Size, 0);

    return returnCode;
}



void SIMLINK_Pwm_Parse_StateReset(void)
{
    SIMLINK_Pwm_Data.Count = 0;
    SIMLINK_Pwm_Data.State = SIMLINK_PWM_PARSE_STATE_UNKNOWN;
}


bool SIMLINK_Pwm_ParseChar(char byte)
{
    bool returnBool = false;

    /* Buffer overflow protection. */
    if(SIMLINK_Pwm_Data.Count >= sizeof(SIMLINK_Pwm_Data.ParseBuffer))
    {
        /* TODO add event. */
        SIMLINK_Pwm_Parse_StateReset();
        goto end_of_function;
    }

    switch(SIMLINK_Pwm_Data.State)
    {
        case SIMLINK_PWM_PARSE_STATE_UNKNOWN:
        {
            if(byte == '#')
            {
                SIMLINK_Pwm_Data.State = SIMLINK_PWM_PARSE_STATE_SYNC1;
            }
            break;
        }
        case SIMLINK_PWM_PARSE_STATE_SYNC1:
        {
            if(byte == '1')
            {
                SIMLINK_Pwm_Data.State = SIMLINK_PWM_PARSE_STATE_CH1;
            }
            else
            {
                /* TODO add error event. */
                SIMLINK_Pwm_Parse_StateReset();
            }
            break;
        }
        case SIMLINK_PWM_PARSE_STATE_CH1:
        {
            if(byte == ':')
            {
                SIMLINK_Pwm_Data.State = SIMLINK_PWM_PARSE_STATE_PARSE1;
            }
            else
            {
                /* TODO add error event. */
                SIMLINK_Pwm_Parse_StateReset();
            }
            break;
        }
        case SIMLINK_PWM_PARSE_STATE_PARSE1:
        {
            if(byte == '#')
            {
                SIMLINK_Pwm_Data.State = SIMLINK_PWM_PARSE_STATE_SYNC2;
                SIMLINK_Pwm_Data.Channel[0] = atoi(&SIMLINK_Pwm_Data.ParseBuffer[0]);
                memset((void *)&SIMLINK_Pwm_Data.ParseBuffer[0], 0x0, sizeof(&SIMLINK_Pwm_Data.ParseBuffer));
                SIMLINK_Pwm_Data.Count = 0;
                break;
            }
            SIMLINK_Pwm_Data.ParseBuffer[SIMLINK_Pwm_Data.Count] = byte;
            SIMLINK_Pwm_Data.Count++;
            break;
        }
        case SIMLINK_PWM_PARSE_STATE_SYNC2:
        {
            if(byte == '2')
            {
                SIMLINK_Pwm_Data.State = SIMLINK_PWM_PARSE_STATE_CH2;
            }
            else
            {
                /* TODO add error event. */
                SIMLINK_Pwm_Parse_StateReset();
            }
            break;
        }
        case SIMLINK_PWM_PARSE_STATE_CH2:
        {
            if(byte == ':')
            {
                SIMLINK_Pwm_Data.State = SIMLINK_PWM_PARSE_STATE_PARSE2;
            }
            else
            {
                /* TODO add error event. */
                SIMLINK_Pwm_Parse_StateReset();
            }
            break;
        }
        case SIMLINK_PWM_PARSE_STATE_PARSE2:
        {
            if(byte == '#')
            {
                SIMLINK_Pwm_Data.State = SIMLINK_PWM_PARSE_STATE_SYNC3;
                SIMLINK_Pwm_Data.Channel[1] = atoi(&SIMLINK_Pwm_Data.ParseBuffer[0]);
                memset((void *)&SIMLINK_Pwm_Data.ParseBuffer[0], 0x0, sizeof(&SIMLINK_Pwm_Data.ParseBuffer));
                SIMLINK_Pwm_Data.Count = 0;
                break;
            }
            SIMLINK_Pwm_Data.ParseBuffer[SIMLINK_Pwm_Data.Count] = byte;
            SIMLINK_Pwm_Data.Count++;
            break;
        }
        case SIMLINK_PWM_PARSE_STATE_SYNC3:
        {
            if(byte == '3')
            {
                SIMLINK_Pwm_Data.State = SIMLINK_PWM_PARSE_STATE_CH3;
            }
            else
            {
                /* TODO add error event. */
                SIMLINK_Pwm_Parse_StateReset();
            }
            break;
        }
        case SIMLINK_PWM_PARSE_STATE_CH3:
        {
            if(byte == ':')
            {
                SIMLINK_Pwm_Data.State = SIMLINK_PWM_PARSE_STATE_PARSE3;
            }
            else
            {
                /* TODO add error event. */
                SIMLINK_Pwm_Parse_StateReset();
            }
            break;
        }
        case SIMLINK_PWM_PARSE_STATE_PARSE3:
        {
            if(byte == '#')
            {
                SIMLINK_Pwm_Data.State = SIMLINK_PWM_PARSE_STATE_SYNC4;
                SIMLINK_Pwm_Data.Channel[2] = atoi(&SIMLINK_Pwm_Data.ParseBuffer[0]);
                memset((void *)&SIMLINK_Pwm_Data.ParseBuffer[0], 0x0, sizeof(&SIMLINK_Pwm_Data.ParseBuffer));
                SIMLINK_Pwm_Data.Count = 0;
                break;
            }
            SIMLINK_Pwm_Data.ParseBuffer[SIMLINK_Pwm_Data.Count] = byte;
            SIMLINK_Pwm_Data.Count++;
            break;
        }
        case SIMLINK_PWM_PARSE_STATE_SYNC4:
        {
            if(byte == '4')
            {
                SIMLINK_Pwm_Data.State = SIMLINK_PWM_PARSE_STATE_CH4;
            }
            else
            {
                /* TODO add error event. */
                SIMLINK_Pwm_Parse_StateReset();
            }
            break;
        }
        case SIMLINK_PWM_PARSE_STATE_CH4:
        {
            if(byte == ':')
            {
                SIMLINK_Pwm_Data.State = SIMLINK_PWM_PARSE_STATE_PARSE4;
            }
            else
            {
                /* TODO add error event. */
                SIMLINK_Pwm_Parse_StateReset();
            }
            break;
        }
        case SIMLINK_PWM_PARSE_STATE_PARSE4:
        {
            if(byte == ':')
            {
                SIMLINK_Pwm_Data.Channel[3] = atoi(&SIMLINK_Pwm_Data.ParseBuffer[0]);
                returnBool = true;
                SIMLINK_Pwm_Parse_StateReset();
                break;
            }
            SIMLINK_Pwm_Data.ParseBuffer[SIMLINK_Pwm_Data.Count] = byte;
            SIMLINK_Pwm_Data.Count++;
            break;
        }
        default:
        {
            /* TODO add error event. */
            SIMLINK_Pwm_Parse_StateReset();
        }
    }

end_of_function:
    return returnBool;
}


bool SIMLINK_Pwm_Receive(unsigned int *Channels)
{
    int  i           = 0;
    bool done        = false;
    int  returnValue = 0;

    if(Channels == 0)
    {
        goto end_of_function;
    }

    returnValue = SIMLINK_Pwm_Recv_Socket((char *)&SIMLINK_Pwm_Data.Buffer[0], sizeof(SIMLINK_Pwm_Data.Buffer));
    if(returnValue > 0)
    {
        for(i = 0; i < returnValue; ++i)
        {
            done = SIMLINK_Pwm_ParseChar(SIMLINK_Pwm_Data.Buffer[i]);
            if(done == true)
            {
                Channels[0] = SIMLINK_Pwm_Data.Channel[0];
                Channels[1] = SIMLINK_Pwm_Data.Channel[1];
                Channels[2] = SIMLINK_Pwm_Data.Channel[2];
                Channels[3] = SIMLINK_Pwm_Data.Channel[3];
            }
        }
    }

end_of_function:

    return done;
}

