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

#include "cfe.h"
#include "ut_cfe_evs_hooks.h"
#include "ut_cfe_time_stubs.h"
#include "ut_cfe_psp_timer_stubs.h"
#include "ut_cfe_psp_memutils_stubs.h"
#include "ut_cfe_tbl_stubs.h"
#include "ut_cfe_tbl_hooks.h"
#include "ut_cfe_fs_stubs.h"
#include "ut_cfe_time_stubs.h"
#include "ut_osapi_stubs.h"
#include "ut_osfileapi_stubs.h"
#include "ut_cfe_sb_stubs.h"
#include "ut_cfe_es_stubs.h"
#include "ut_cfe_evs_stubs.h"

#include "amc_test_utils.hpp"
#include "ut_amc_custom_stubs.h"

#include <time.h>


extern Ut_CFE_PSP_MEMUTILS_HookTable_t         Ut_CFE_PSP_MEMUTILS_HookTable;
extern Ut_CFE_PSP_MEMUTILS_ReturnCodeTable_t
                  Ut_CFE_PSP_MEMUTILS_ReturnCodeTable[UT_CFE_PSP_MEMUTILS_MAX_INDEX];

extern Ut_CFE_PSP_TIMER_HookTable_t            Ut_CFE_PSP_TIMER_HookTable;
extern Ut_CFE_PSP_TIMER_ReturnCodeTable_t
                  Ut_CFE_PSP_TIMER_ReturnCodeTable[UT_CFE_PSP_TIMER_MAX_INDEX];

namespace math
{
    const float &constrain(const float &val, const float &min_val, const float &max_val)
    {
        return (val < min_val) ? min_val : ((val > max_val) ? max_val : val);
    }
}

/*
 * Config table for testing
 */
AMC_ConfigTbl_t ConfigTblUnitTest = {
    900, 1100, 2000, AMC_PWM_DISARM_BEHAVIOR_IGNORE, 1450, /* Actuator  1 - Right Aileron    */
    900, 1100, 2000, AMC_PWM_DISARM_BEHAVIOR_IGNORE, 1450, /* Actuator  2 - Elevator         */
    900, 1100, 2000, AMC_PWM_DISARM_BEHAVIOR_SAFE,   1450, /* Actuator  3 - Left Throttle    */
    900, 1100, 2000, AMC_PWM_DISARM_BEHAVIOR_IGNORE, 1450, /* Actuator  4 - Rudder/Nose Gear */
    900, 1100, 2000, AMC_PWM_DISARM_BEHAVIOR_IGNORE, 1450, /* Actuator  5 - Unused           */
    900, 1100, 2000, AMC_PWM_DISARM_BEHAVIOR_IGNORE, 1450, /* Actuator  6 - Left Aileron     */
    900, 1100, 2000, AMC_PWM_DISARM_BEHAVIOR_IGNORE, 1450, /* Actuator  7 - Unused           */
    900, 1100, 2000, AMC_PWM_DISARM_BEHAVIOR_SAFE,   1450, /* Actuator  8 - Right Throttle   */
    900, 1100, 2000, AMC_PWM_DISARM_BEHAVIOR_IGNORE, 1450, /* Actuator  9 - Left Flap        */
    900, 1100, 2000, AMC_PWM_DISARM_BEHAVIOR_IGNORE, 1450  /* Actuator 10 - Right Flap       */
};

/*
 * Invalid Config table for testing
 */
AMC_ConfigTbl_t ConfigTblUnitTest_Invalid = {
    2000, 1100, 900, AMC_PWM_DISARM_BEHAVIOR_IGNORE, 1450, /* Actuator  1 - Right Aileron    */
    2000, 1100, 900, AMC_PWM_DISARM_BEHAVIOR_IGNORE, 1450, /* Actuator  2 - Elevator         */
    2000, 1100, 900, AMC_PWM_DISARM_BEHAVIOR_SAFE,   1450, /* Actuator  3 - Left Throttle    */
    2000, 1100, 900, AMC_PWM_DISARM_BEHAVIOR_IGNORE, 1450, /* Actuator  4 - Rudder/Nose Gear */
    2000, 1100, 900, AMC_PWM_DISARM_BEHAVIOR_IGNORE, 1450, /* Actuator  5 - Unused           */
    2000, 1100, 900, AMC_PWM_DISARM_BEHAVIOR_IGNORE, 1450, /* Actuator  6 - Left Aileron     */
    2000, 1100, 900, AMC_PWM_DISARM_BEHAVIOR_IGNORE, 1450, /* Actuator  7 - Unused           */
    2000, 1100, 900, AMC_PWM_DISARM_BEHAVIOR_SAFE,   1450, /* Actuator  8 - Right Throttle   */
    2000, 1100, 900, AMC_PWM_DISARM_BEHAVIOR_IGNORE, 1450, /* Actuator  9 - Left Flap        */
    2000, 1100, 900, AMC_PWM_DISARM_BEHAVIOR_IGNORE, 1450  /* Actuator 10 - Right Flap       */
};

/*
 * AMC Mixer Config table for testing
 */
AMC_Mixer_ConfigTable_t AMC_MixerCfgTblUnitTest = {
    /* Multirotor Mixer */
    {
        0,             /* Rotor Count or 0 to disable the Multirotor Mixer  */
        1.0,           /* Roll Scale                                        */
        1.0,           /* Pitch Scale                                       */
        1.0,           /* Yaw Scale                                         */
        0.0,           /* Idle Speed                                        */
        1.0,           /* Delta Out Max                                     */
        MIXER_QUAD_X,  /* Rotor Geometry                                    */
        {
            /* Roll Scale | Pitch Scale |  Yaw Scale  | Output Scale |  Rotor # */
            { -0.707107,    0.707107,      1.000000,    1.000000 },   /*  0  */
            {  0.707107,   -0.707107,      1.000000,    1.000000 },   /*  1  */
            {  0.707107,    0.707107,     -1.000000,    1.000000 },   /*  2  */
            { -0.707107,   -0.707107,     -1.000000,    1.000000 },   /*  3  */
            {  0.000000,    0.000000,      0.000000,    0.000000 },   /*  4  */
            {  0.000000,    0.000000,      0.000000,    0.000000 },   /*  5  */
            {  0.000000,    0.000000,      0.000000,    0.000000 },   /*  6  */
            {  0.000000,    0.000000,      0.000000,    0.000000 }    /*  7  */
        }
    },

    /* Ctrl Count,
     *     Neg Scale, Pos Scale, Offset, Min Output, Max Output      */
    /*         Group, Index, Neg Scale, Pos Scale, Offset, Min Output, Max Output */

    /* Simple Mixers */
    {
        /* Actuator 1 - Right Aileron */
        {
            1,         /* Control Count */
            /* Output Scaler */
            {
                1.0,   /* Negative Scale */
                1.0,   /* Positive Scale */
                0.0,   /* Offset         */
                -1.0,  /* Minimum Output */
                1.0    /* Maximum Output */
            },
            /* Controls: actual size of the array is set by control count */
            {
                {
                    0,                         /* Control Group */
                    PX4_ACTUATOR_CONTROL_ROLL, /* Control Index */
                    /* Scaler */
                    {
                        1.0,                   /* Negative Scale */
                        1.0,                   /* Positive Scale */
                        0.0,                   /* Offset         */
                        -1.0,                  /* Minimum Output */
                        1.0                    /* Maximum Output */
                    }
                }
            }
        },
        /* Actuator 2 - Elevator */
        {
            1,         /* Control Count */
            /* Output Scaler */
            {
                1.0,   /* Negative Scale */
                1.0,   /* Positive Scale */
                0.0,   /* Offset         */
                -1.0,  /* Minimum Output */
                1.0    /* Maximum Output */
            },
            /* Controls */
            {
                {
                    0,  /* Control Group */
                    PX4_ACTUATOR_CONTROL_PITCH, /* Control Index */
                    /* Scaler */
                    {
                        1.0,                    /* Negative Scale */
                        1.0,                    /* Positive Scale */
                        0.0,                    /* Offset         */
                        -1.0,                   /* Minimum Output */
                        1.0                     /* Maximum Output */
                    }
                }
            }
        },
        /* Actuator 3 - Left Throttle */
        {
            1,         /* Control Count */
            /* Output Scaler */
            {
                1.0,   /* Negative Scale */
                1.0,   /* Positive Scale */
                0.0,   /* Offset         */
                -1.0,  /* Minimum Output */
                1.0    /* Maximum Output */
            },
            /* Controls */
            {
                {
                    0,                              /* Control Group */
                    PX4_ACTUATOR_CONTROL_THROTTLE,  /* Control Index */
                    /* Scaler */
                    {
                        0.0,                        /* Negative Scale */
                        2.0,                        /* Positive Scale */
                        -1.0,                       /* Offset         */
                        -1.0,                       /* Minimum Output */
                        1.0                         /* Maximum Output */
                    }
                }
            }
        },
        /* Actuator 4 - Rudder/Nose Gear */
        {
            1,         /* Control Count */
            /* Output Scaler */
            {
                1.0,   /* Negative Scale */
                1.0,   /* Positive Scale */
                0.0,   /* Offset         */
                -1.0,  /* Minimum Output */
                1.0    /* Maximum Output */
            },
            /* Controls */
            {
                {
                    0,                           /* Control Group */
                    PX4_ACTUATOR_CONTROL_YAW,    /* Control Index */
                    /* Scaler */
                    {
                        -1.0,                    /* Negative Scale */
                        -1.0,                    /* Positive Scale */
                        0.0,                     /* Offset         */
                        -1.0,                    /* Minimum Output */
                        1.0                      /* Maximum Output */
                    }
                }
            }
        },
        /* Actuator 5 - Unused */
        {
            1,         /* Control Count */
            /* Output Scaler */
            {
                1.0,   /* Negative Scale */
                1.0,   /* Positive Scale */
                0.0,   /* Offset         */
                -1.0,  /* Minimum Output */
                1.0    /* Maximum Output */
            },
            /* Controls */
            {
                {
                    0,                                 /* Control Group */
                    PX4_ACTUATOR_CONTROL_LANDING_GEAR, /* Control Index */
                    /* Scaler */
                    {
                        1.0,                           /* Negative Scale */
                        1.0,                           /* Positive Scale */
                        0.0,                           /* Offset         */
                        -1.0,                          /* Minimum Output */
                        1.0                            /* Maximum Output */
                    }
                }
            }
        },
        /* Actuator 6 - Unused */
        {
            1,         /* Control Count */
            /* Output Scaler */
            {
                1.0,   /* Negative Scale */
                1.0,   /* Positive Scale */
                0.0,   /* Offset         */
                -1.0,  /* Minimum Output */
                1.0    /* Maximum Output */
            },
            /* Controls */
            {
                {
                    0,                                  /* Control Group */
                    PX4_ACTUATOR_CONTROL_LANDING_GEAR,  /* Control Index */
                    /* Scaler */
                    {
                        1.0,                            /* Negative Scale */
                        1.0,                            /* Positive Scale */
                        0.0,                            /* Offset         */
                        -1.0,                           /* Minimum Output */
                        1.0                             /* Maximum Output */
                    }
                }
            }
        },
        /* Actuator 7 - Left Aileron */
        {
            1,         /* Control Count */
            /* Output Scaler */
            {
                1.0,   /* Negative Scale */
                1.0,   /* Positive Scale */
                0.0,   /* Offset         */
                -1.0,  /* Minimum Output */
                1.0    /* Maximum Output */
            },
            /* Controls */
            {
                {
                    0,                          /* Control Group */
                    PX4_ACTUATOR_CONTROL_ROLL,  /* Control Index */
                    /* Scaler */
                    {
                         1.0,                   /* Negative Scale */
                         1.0,                   /* Positive Scale */
                         0.0,                   /* Offset         */
                        -1.0,                   /* Minimum Output */
                         1.0                    /* Maximum Output */
                    }
                }
            }
        },
        /* Actuator 8 - Right Throttle */
        {
            1,         /* Control Count */
            /* Output Scaler */
            {
                1.0,   /* Negative Scale */
                1.0,   /* Positive Scale */
                0.0,   /* Offset         */
                -1.0,  /* Minimum Output */
                1.0    /* Maximum Output */
            },
            /* Controls */
            {
                {
                    0,                              /* Control Group */
                    PX4_ACTUATOR_CONTROL_THROTTLE,  /* Control Index */
                    /* Scaler */
                    {
                        0.0,                        /* Negative Scale */
                        2.0,                        /* Positive Scale */
                        -1.0,                       /* Offset         */
                        -1.0,                       /* Minimum Output */
                        1.0                         /* Maximum Output */
                    }
                }
            }
        },
        /* Actuator 9 - Left Flap */
        {
            1,        /* Control Count */
            /* Output Scaler */
            {
                1.0,  /* Negative Scale */
                1.0,  /* Positive Scale */
                0.0,  /* Offset         */
                0.0,  /* Minimum Output */
                0.0   /* Maximum Output */
            },
            /* Controls */
            {
                {
                    0,                           /* Control Group */
                    PX4_ACTUATOR_CONTROL_FLAPS,  /* Control Index */
                    /* Scaler */
                    {
                        1.0,                     /* Negative Scale */
                        1.0,                     /* Positive Scale */
                        0.0,                     /* Offset         */
                        0.0,                     /* Minimum Output */
                        0.0                      /* Maximum Output */
                    }
                }
            }
        },
        /* Actuator 10 - Right Flap */
        {
            1,        /* Control Count */
            /* Output Scaler */
            {
                1.0,  /* Negative Scale */
                1.0,  /* Positive Scale */
                0.0,  /* Offset         */
                0.0,  /* Minimum Output */
                0.0   /* Maximum Output */
            },
            /* Controls */
            {
                {
                    0,                           /* Control Group */
                    PX4_ACTUATOR_CONTROL_FLAPS,  /* Control Index */
                    /* Scaler */
                    {
                        1.0,                     /* Negative Scale */
                        1.0,                     /* Positive Scale */
                        0.0,                     /* Offset         */
                        0.0,                     /* Minimum Output */
                        0.0                      /* Maximum Output */
                    }
                }
            }
        }
    }
};

/*
 * Function Definitions
 */

void AMC_Test_Setup(void)
{
    /* initialize test environment to default state for every test */
    Ut_CFE_EVS_Reset();
    Ut_CFE_FS_Reset();
    Ut_CFE_TIME_Reset();
    Ut_CFE_TBL_Reset();
    Ut_CFE_SB_Reset();
    Ut_CFE_ES_Reset();
    Ut_OSAPI_Reset();
    Ut_OSFILEAPI_Reset();
    Ut_AMC_Custom_Reset();

#if 0
    Ut_CFE_TBL_AddTable(AMC_CONFIG_TABLE_FILENAME, (void *) &ConfigTblUnitTest);
    Ut_CFE_TBL_AddTable(AMC_MIXER_CONFIG_TABLE_FILENAME, (void *) &AMC_MixerCfgTblUnitTest);
#else
    Ut_CFE_TBL_AddTable(AMC_CONFIG_TABLE_FILENAME, (void *) &AMC_ConfigTbl);
    Ut_CFE_TBL_AddTable(AMC_MIXER_CONFIG_TABLE_FILENAME, (void *) &AMC_MixerCfgTbl);
#endif

    memset(&Ut_CFE_PSP_MEMUTILS_HookTable, 0, sizeof(Ut_CFE_PSP_MEMUTILS_HookTable));
    memset(&Ut_CFE_PSP_MEMUTILS_ReturnCodeTable, 0, sizeof(Ut_CFE_PSP_MEMUTILS_ReturnCodeTable));

    memset(&Ut_CFE_PSP_TIMER_HookTable, 0, sizeof(Ut_CFE_PSP_TIMER_HookTable));
    memset(&Ut_CFE_PSP_TIMER_ReturnCodeTable, 0, sizeof(Ut_CFE_PSP_TIMER_ReturnCodeTable));
}

void AMC_Test_Setup_CfgTblInvalid(void)
{
    /* initialize test environment to default state for every test */
    Ut_CFE_EVS_Reset();
    Ut_CFE_FS_Reset();
    Ut_CFE_TIME_Reset();
    Ut_CFE_TBL_Reset();
    Ut_CFE_SB_Reset();
    Ut_CFE_ES_Reset();
    Ut_OSAPI_Reset();
    Ut_OSFILEAPI_Reset();
    Ut_AMC_Custom_Reset();

    Ut_CFE_TBL_AddTable(AMC_CONFIG_TABLE_FILENAME, (void *) &ConfigTblUnitTest_Invalid);
    Ut_CFE_TBL_AddTable(AMC_MIXER_CONFIG_TABLE_FILENAME, (void *) &AMC_MixerCfgTblUnitTest);

    memset(&Ut_CFE_PSP_MEMUTILS_HookTable, 0, sizeof(Ut_CFE_PSP_MEMUTILS_HookTable));
    memset(&Ut_CFE_PSP_MEMUTILS_ReturnCodeTable, 0, sizeof(Ut_CFE_PSP_MEMUTILS_ReturnCodeTable));

    memset(&Ut_CFE_PSP_TIMER_HookTable, 0, sizeof(Ut_CFE_PSP_TIMER_HookTable));
    memset(&Ut_CFE_PSP_TIMER_ReturnCodeTable, 0, sizeof(Ut_CFE_PSP_TIMER_ReturnCodeTable));
}

void AMC_Test_TearDown(void) {
    memset(&Ut_CFE_PSP_MEMUTILS_HookTable, 0, sizeof(Ut_CFE_PSP_MEMUTILS_HookTable));
    memset(&Ut_CFE_PSP_MEMUTILS_ReturnCodeTable, 0, sizeof(Ut_CFE_PSP_MEMUTILS_ReturnCodeTable));

    memset(&Ut_CFE_PSP_TIMER_HookTable, 0, sizeof(Ut_CFE_PSP_TIMER_HookTable));
    memset(&Ut_CFE_PSP_TIMER_ReturnCodeTable, 0, sizeof(Ut_CFE_PSP_TIMER_ReturnCodeTable));
}

double GetConfigTblChecksum(AMC *pAMC)
{
    int    i;
    double Checksum = 0.0;

    for (i = 0; i < AMC_MAX_MOTOR_OUTPUTS; i++)
    {
        Checksum += pAMC->ConfigTblPtr->Channel[i].PwmSafe + pAMC->ConfigTblPtr->Channel[i].PwmMin
                   + pAMC->ConfigTblPtr->Channel[i].PwmMax + pAMC->ConfigTblPtr->Channel[i].DisarmBehavior
                   + pAMC->ConfigTblPtr->Channel[i].PwmInitial;
    }

    return Checksum;
}

double GetMultirotorMixerConfigTblChecksum(AMC *pAMC)
{
    int              i, j;
    int              rotor_count = 0;
    double           Checksum = 0.0;
    AMC_MultirotorMixer_Config_t  *pMulti = NULL;

    for (i = 0; i < AMC_MULTIROTOR_MIXER_MAX_MIXERS; i++)
    {
        pMulti = &pAMC->MixerConfigTblPtr->Multirotor[i];
        rotor_count = pMulti->RotorCount;
        if (rotor_count > 0)
        {
            Checksum += pMulti->RotorCount + pMulti->RollScale + pMulti->PitchScale
                       + pMulti->YawScale + pMulti->IdleSpeed + pMulti->DeltaOutMax
                       + pMulti->Geometry;
            for (j = 0; j < rotor_count; j++)
            {
                Checksum += pMulti->RotorConfig[j].RollScale + pMulti->RotorConfig[j].PitchScale
                           + pMulti->RotorConfig[j].YawScale + pMulti->RotorConfig[j].OutScale;
            }
        }
    }

    return Checksum;
}

double GetSimpleMixerConfigTblChecksum(AMC *pAMC)
{
    int              i, j;
    int              control_count = 0;
    double           Checksum = 0.0;
    AMC_SimpleMixer_Config_t  *pSimple = NULL;

    for (i = 0; i < AMC_SIMPLE_MIXER_MAX_MIXERS; i++)
    {
        pSimple = &pAMC->MixerConfigTblPtr->Simple[i];
        control_count = pSimple->ControlCount;
        if (control_count > 0)
        {
            Checksum += pSimple->ControlCount
                       + pSimple->OutputScaler.NegativeScale + pSimple->OutputScaler.PositiveScale
                       + pSimple->OutputScaler.Offset + pSimple->OutputScaler.MinOutput
                       + pSimple->OutputScaler.MaxOutput;

            for (j = 0; j < control_count; j++)
            {
                Checksum += pSimple->Controls[j].ControlGroup + pSimple->Controls[j].ControlIndex;
                Checksum += pSimple->Controls[j].Scaler.NegativeScale
                           + pSimple->Controls[j].Scaler.PositiveScale
                           + pSimple->Controls[j].Scaler.Offset
                           + pSimple->Controls[j].Scaler.MinOutput
                           + pSimple->Controls[j].Scaler.MaxOutput;
            }
        }
    }

    return Checksum;
}

void AMC_Test_PrintCmdMsg(void *pMsg, uint32 size)
{
    unsigned char *pBuff;
    int           i = 0;

    pBuff = (unsigned char*)pMsg;
    printf("Emulated Cmd message:");
    for (i = 0; i < size; i++)
    {
        printf("0x%02x ", *pBuff);
        pBuff++;
    }
    printf("\n");

    return;
}

time_t AMC_Test_GetTimeFromTimestamp(uint64 timestamp)
{
    time_t  local_time;

    local_time = (time_t)(timestamp / 1000000);

    return local_time;
}

time_t AMC_Test_GetTimeFromMsg(CFE_TIME_SysTime_t cfe_time)
{
    time_t   local_time;

    local_time = (time_t)cfe_time.Seconds;

    return local_time;
}
