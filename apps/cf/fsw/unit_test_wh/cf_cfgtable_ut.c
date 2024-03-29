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

#include "cf_test_utils.h"

#include "sbnd_platform_cfg.h"
#include "to_platform_cfg.h"


cf_config_table_t  CF_ConfigTableUnitTest =
{
    "CF Default Table",/* TableIdString            */
    2,                 /* TableVersion (integer)   */
    3,                 /* NumEngCyclesPerWakeup    */
    1,                 /* NumWakeupsPerQueueChk    */
    1,                 /* NumWakeupsPerPollDirChk  */
    100,               /* UplinkHistoryQDepth      */
    0,                 /* Reserved1                */
    0,                 /* Reserved2                */
    "10",              /* AckTimeout (secs, entered as string) */
    "2",               /* AckLimit (max timeouts, string)      */
    "5",               /* NakTimeout (secs, string)            */
    "3",               /* NakLimit (max timeouts, string)      */
    "20",              /* InactivityTimeout (secs, string)     */
    "200",             /* OutgoingFileChunkSize (bytes, string)*/
    "no",              /* SaveIncompleteFiles (yes,no, string) */
    "0.3",             /* Flight EntityId - 2 byte dotted-decimal string eg. "0.255"*/

    { /* Input Channel Array */

        { /* Input Channel 0 */
            CF_PPD_TO_CPD_PDU_MID,
            0, /* Output Chan for Class 2 Uplink Responses, ACK-EOF,Nak,Fin etc) */
            0  /* spare */
        }, /* end Input Channel 0 */

        { /* Input Channel 1 */
            CF_GND_TO_CPD_PDU_MID,
            1, /* Output Chan for Class 2 Uplink Responses, ACK-EOF,Nak,Fin etc) */
            0  /* spare */
        } /* end Input Channel 1 */

    }, /* end Input Channel Array */

    { /* Playback Channel Array */

        { /* Playback Channel #0 */
            CF_ENTRY_IN_USE,          /* Playback Channel Entry In Use */
            CF_ENABLED,               /* Dequeue Enable */
            CF_CPD_TO_PPD_PDU_MID,    /* Space To Gnd PDU MsgId */
            100,                      /* Pending Queue Depth */
            100,                      /* History Queue Depth */
            "PPD",                    /* Playback Channel Name   */
            SBND_CF_THROTTLE_SEM_NAME,/* Handshake Semaphore Name   */

            {  /* Polling Directory Array */

                { /* Polling Directory 0 */
                    CF_ENTRY_IN_USE,/* Poll Directory In Use or Not */
                    CF_ENABLED,     /* Enable State */
                    1,              /* Class (1 or 2)*/
                    2,              /* Priority */
                    CF_KEEP_FILE,   /* Preserve files after successful transfer? */
                    0,              /* Reserved1 */
                    0,              /* Reserved2 */
                    0,              /* Reserved3 */
                    "0.2",          /* Gnd EntityId - 2 byte dotted-decimal string eg. "0.255"*/
                    "/ram/dn/cpd/cl1/pr2/", /* SrcPath, no spaces, fwd slash at end */
                    "/cf/dn/cpd/cl1/pr2/",  /* DstPath, no spaces */
                }, /* End Polling Directory 0 */

                { /* Polling Directory 1 */
                    CF_ENTRY_IN_USE,/* Poll Directory In Use or Not */
                    CF_ENABLED,     /* Enable State */
                    2,              /* Class (1 or 2)*/
                    2,              /* Priority */
                    CF_DELETE_FILE, /* Preserve files after successful transfer? */
                    0,              /* Reserved1 */
                    0,              /* Reserved2 */
                    0,              /* Reserved3 */
                    "0.2",          /* Gnd EntityId - 2 byte dotted-decimal string eg. "0.255"*/
                    "/ram/dn/cpd/cl2/pr2/", /* SrcPath, no spaces, fwd slash at end */
                    "/cf/dn/cpd/cl2/pr2/",  /* DstPath, no spaces */
                }, /* End Polling Directory 1 */

                { /* Polling Directory 2 */
                    CF_ENTRY_IN_USE,/* Poll Directory In Use or Not */
                    CF_ENABLED,     /* Enable State */
                    1,              /* Class (1 or 2)*/
                    4,              /* Priority */
                    CF_KEEP_FILE,   /* Preserve files after successful transfer? */
                    0,              /* Reserved1 */
                    0,              /* Reserved2 */
                    0,              /* Reserved3 */
                    "0.2",          /* Gnd EntityId - 2 byte dotted-decimal string eg. "0.255"*/
                    "/ram/dn/cpd/cl1/pr4/", /* SrcPath, no spaces, fwd slash at end */
                    "/cf/dn/cpd/cl1/pr4/",  /* DstPath, no spaces */
                }, /* End Polling Directory 2 */

                { /* Polling Directory 3 */
                    CF_ENTRY_IN_USE,/* Poll Directory In Use or Not */
                    CF_ENABLED,     /* Enable State */
                    2,              /* Class (1 or 2)*/
                    4,              /* Priority */
                    CF_DELETE_FILE, /* Preserve files after successful transfer? */
                    0,              /* Reserved1 */
                    0,              /* Reserved2 */
                    0,              /* Reserved3 */
                    "0.2",          /* Gnd EntityId - 2 byte dotted-decimal string eg. "0.255"*/
                    "/ram/dn/cpd/cl2/pr4/", /* SrcPath, no spaces, fwd slash at end */
                    "/cf/dn/cpd/cl2/pr4/",  /* DstPath, no spaces */
                }, /* End Polling Directory 3 */

                { /* Polling Directory 4 */
                    CF_ENTRY_IN_USE,/* Poll Directory In Use or Not */
                    CF_ENABLED,     /* Enable State */
                    1,              /* Class (1 or 2)*/
                    6,              /* Priority */
                    CF_KEEP_FILE,   /* Preserve files after successful transfer? */
                    0,              /* Reserved1 */
                    0,              /* Reserved2 */
                    0,              /* Reserved3 */
                    "0.2",          /* Gnd EntityId - 2 byte dotted-decimal string eg. "0.255"*/
                    "/ram/dn/cpd/cl1/pr6/", /* SrcPath, no spaces, fwd slash at end */
                    "/cf/dn/cpd/cl1/pr6/",  /* DstPath, no spaces */
                }, /* End Polling Directory 4 */

                { /* Polling Directory 5 */
                    CF_ENTRY_IN_USE,/* Poll Directory In Use or Not */
                    CF_ENABLED,     /* Enable State */
                    2,              /* Class (1 or 2)*/
                    6,              /* Priority */
                    CF_DELETE_FILE, /* Preserve files after successful transfer? */
                    0,              /* Reserved1 */
                    0,              /* Reserved2 */
                    0,              /* Reserved3 */
                    "0.2",          /* Gnd EntityId - 2 byte dotted-decimal string eg. "0.255"*/
                    "/ram/dn/cpd/cl2/pr6/", /* SrcPath, no spaces, fwd slash at end */
                    "/cf/dn/cpd/cl2/pr6/",  /* DstPath, no spaces */
                } /* End Polling Directory 5 */

            } /* End Polling Directory Array */

        }, /* End Playback Channel #0 */

        { /* Playback Channel #1 */
            CF_ENTRY_IN_USE,           /* Playback Channel Entry In Use */
            CF_DISABLED,               /* Dequeue Enable */
            CF_CPD_TO_GND_PDU_MID,     /* Space To Gnd PDU MsgId */
            100,                       /* Pending Queue Depth */
            100,                       /* History Queue Depth */
            "CPD",                     /* Playback Channel Name   */
            TO_CF_THROTTLE_SEM_NAME,   /* Handshake Semaphore Name   */

            { /* Polling Directory Array */

                { /* Polling Directory 0 */
                    CF_ENTRY_IN_USE,/* Poll Directory In Use or Not */
                    CF_ENABLED,     /* Enable State */
                    1,              /* Class (1 or 2)*/
                    2,              /* Priority */
                    CF_KEEP_FILE,   /* Preserve files after successful transfer? */
                    0,              /* Reserved1 */
                    0,              /* Reserved2 */
                    0,              /* Reserved3 */
                    "0.1",          /* Gnd EntityId - 2 byte dotted-decimal string eg. "0.255"*/
                    "/ram/dn/ppd/cl1/pr2/", /* SrcPath, no spaces, fwd slash at end */
                    "/ppd/",                /* DstPath, no spaces */
                }, /* End Polling Directory 0 */

                { /* Polling Directory 1 */
                    CF_ENTRY_IN_USE,/* Poll Directory In Use or Not */
                    CF_ENABLED,     /* Enable State */
                    2,              /* Class (1 or 2)*/
                    2,              /* Priority */
                    CF_DELETE_FILE, /* Preserve files after successful transfer? */
                    0,              /* Reserved1 */
                    0,              /* Reserved2 */
                    0,              /* Reserved3 */
                    "0.1",          /* Gnd EntityId - 2 byte dotted-decimal string eg. "0.255"*/
                    "/ram/dn/ppd/cl2/pr2/", /* SrcPath, no spaces, fwd slash at end */
                    "/ppd/",                /* DstPath, no spaces */
                }, /* End Polling Directory 1 */

                { /* Polling Directory 2 */
                    CF_ENTRY_IN_USE,/* Poll Directory In Use or Not */
                    CF_ENABLED,     /* Enable State */
                    1,              /* Class (1 or 2)*/
                    4,              /* Priority */
                    CF_KEEP_FILE,   /* Preserve files after successful transfer? */
                    0,              /* Reserved1 */
                    0,              /* Reserved2 */
                    0,              /* Reserved3 */
                    "0.1",          /* Gnd EntityId - 2 byte dotted-decimal string eg. "0.255"*/
                    "/ram/dn/ppd/cl1/pr4/", /* SrcPath, no spaces, fwd slash at end */
                    "/ppd/",                /* DstPath, no spaces */
                }, /* End Polling Directory 2 */

                { /* Polling Directory 3 */
                    CF_ENTRY_IN_USE,/* Poll Directory In Use or Not */
                    CF_ENABLED,     /* Enable State */
                    2,              /* Class (1 or 2)*/
                    4,              /* Priority */
                    CF_DELETE_FILE, /* Preserve files after successful transfer? */
                    0,              /* Reserved1 */
                    0,              /* Reserved2 */
                    0,              /* Reserved3 */
                    "0.1",          /* Gnd EntityId - 2 byte dotted-decimal string eg. "0.255"*/
                    "/ram/dn/ppd/cl2/pr4/", /* SrcPath, no spaces, fwd slash at end */
                    "/ppd/",                /* DstPath, no spaces */
                }, /* End Polling Directory 3 */

                { /* Polling Directory 4 */
                    CF_ENTRY_IN_USE,/* Poll Directory In Use or Not */
                    CF_ENABLED,     /* Enable State */
                    1,              /* Class (1 or 2)*/
                    6,              /* Priority */
                    CF_KEEP_FILE,   /* Preserve files after successful transfer? */
                    0,              /* Reserved1 */
                    0,              /* Reserved2 */
                    0,              /* Reserved3 */
                    "0.1",          /* Gnd EntityId - 2 byte dotted-decimal string eg. "0.255"*/
                    "/ram/dn/ppd/cl1/pr6/", /* SrcPath, no spaces, fwd slash at end */
                    "/ppd/",                /* DstPath, no spaces */
                }, /* End Polling Directory 4 */

                { /* Polling Directory 5 */
                    CF_ENTRY_IN_USE,/* Poll Directory In Use or Not */
                    CF_ENABLED,     /* Enable State */
                    2,              /* Class (1 or 2)*/
                    6,              /* Priority */
                    CF_DELETE_FILE, /* Preserve files after successful transfer? */
                    0,              /* Reserved1 */
                    0,              /* Reserved2 */
                    0,              /* Reserved3 */
                    "0.1",          /* Gnd EntityId - 2 byte dotted-decimal string eg. "0.255"*/
                    "/ram/dn/ppd/cl2/pr6/", /* SrcPath, no spaces, fwd slash at end */
                    "/ppd/",                /* DstPath, no spaces */
                }, /* End Polling Directory 5 */

                { /* Polling Directory 6 */
                    CF_ENTRY_IN_USE,/* Poll Directory In Use or Not */
                    CF_ENABLED,     /* Enable State */
                    1,              /* Class (1 or 2)*/
                    2,              /* Priority */
                    CF_KEEP_FILE,   /* Preserve files after successful transfer? */
                    0,              /* Reserved1 */
                    0,              /* Reserved2 */
                    0,              /* Reserved3 */
                    "0.1",          /* Gnd EntityId - 2 byte dotted-decimal string eg. "0.255"*/
                    "/ram/dn/cpd/cl1/pr2/", /* SrcPath, no spaces, fwd slash at end */
                    "/cpd/",                /* DstPath, no spaces */
                }, /* End Polling Directory 6 */

                { /* Polling Directory 7 */
                    CF_ENTRY_IN_USE,/* Poll Directory In Use or Not */
                    CF_ENABLED,     /* Enable State */
                    2,              /* Class (1 or 2)*/
                    2,              /* Priority */
                    CF_DELETE_FILE, /* Preserve files after successful transfer? */
                    0,              /* Reserved1 */
                    0,              /* Reserved2 */
                    0,              /* Reserved3 */
                    "0.1",          /* Gnd EntityId - 2 byte dotted-decimal string eg. "0.255"*/
                    "/ram/dn/cpd/cl2/pr2/", /* SrcPath, no spaces, fwd slash at end */
                    "/cpd/",                /* DstPath, no spaces */
                }, /* End Polling Directory 7 */

                { /* Polling Directory 8 */
                    CF_ENTRY_IN_USE,/* Poll Directory In Use or Not */
                    CF_ENABLED,     /* Enable State */
                    1,              /* Class (1 or 2)*/
                    4,              /* Priority */
                    CF_KEEP_FILE,   /* Preserve files after successful transfer? */
                    0,              /* Reserved1 */
                    0,              /* Reserved2 */
                    0,              /* Reserved3 */
                    "0.1",          /* Gnd EntityId - 2 byte dotted-decimal string eg. "0.255"*/
                    "/ram/dn/cpd/cl1/pr4/", /* SrcPath, no spaces, fwd slash at end */
                    "/cpd/",                /* DstPath, no spaces */
                }, /* End Polling Directory 8 */

                { /* Polling Directory 9 */
                    CF_ENTRY_IN_USE,/* Poll Directory In Use or Not */
                    CF_ENABLED,     /* Enable State */
                    2,              /* Class (1 or 2)*/
                    4,              /* Priority */
                    CF_DELETE_FILE, /* Preserve files after successful transfer? */
                    0,              /* Reserved1 */
                    0,              /* Reserved2 */
                    0,              /* Reserved3 */
                    "0.1",          /* Gnd EntityId - 2 byte dotted-decimal string eg. "0.255"*/
                    "/ram/dn/cpd/cl2/pr4/", /* SrcPath, no spaces, fwd slash at end */
                    "/cpd/",                /* DstPath, no spaces */
                }, /* End Polling Directory 9 */

                { /* Polling Directory 10 */
                    CF_ENTRY_IN_USE,/* Poll Directory In Use or Not */
                    CF_ENABLED,     /* Enable State */
                    1,              /* Class (1 or 2)*/
                    6,              /* Priority */
                    CF_KEEP_FILE,   /* Preserve files after successful transfer? */
                    0,              /* Reserved1 */
                    0,              /* Reserved2 */
                    0,              /* Reserved3 */
                    "0.1",          /* Gnd EntityId - 2 byte dotted-decimal string eg. "0.255"*/
                    "/ram/dn/cpd/cl1/pr6/", /* SrcPath, no spaces, fwd slash at end */
                    "/cpd/",                /* DstPath, no spaces */
                }, /* End Polling Directory 10 */

                { /* Polling Directory 11 */
                    CF_ENTRY_IN_USE,/* Poll Directory In Use or Not */
                    CF_ENABLED,     /* Enable State */
                    2,              /* Class (1 or 2)*/
                    6,              /* Priority */
                    CF_DELETE_FILE, /* Preserve files after successful transfer? */
                    0,              /* Reserved1 */
                    0,              /* Reserved2 */
                    0,              /* Reserved3 */
                    "0.1",          /* Gnd EntityId - 2 byte dotted-decimal string eg. "0.255"*/
                    "/ram/dn/cpd/cl2/pr6/", /* SrcPath, no spaces, fwd slash at end */
                    "/cpd/",                /* DstPath, no spaces */
                } /* End Polling Directory 11 */

            } /* End Polling Directory Array */

        } /* End Playback Channel #1 */

    } /* End Playback Channel Array */

}; /* End CF_ConfigTableUnitTest */
