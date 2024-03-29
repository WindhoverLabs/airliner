/*
** Pragmas
*/

/*
** Include Files
*/
#include <mixer/MixerTables.h>
#include "cfe_tbl_filedef.h"
#include "amc_tbldefs.h"
#include "px4_msgs.h"

/*
** Local Defines
*/

/*
** Local Structure Declarations
*/
static OS_USED CFE_TBL_FileDef_t CFE_TBL_FileDef =
{
    /* Content format: ObjName[64], TblName[38], Desc[32], TgtFileName[20], ObjSize
    **    ObjName - variable name of config table, e.g., CI_ConfigDefTbl[]
    **    TblName - app's table name, e.g., CI.CONFIG_TBL, where CI is the same app name
    **              used in cfe_es_startup.scr, and CI_defConfigTbl is the same table
    **              name passed in to CFE_TBL_Register()
    **    Desc - description of table in string format
    **    TgtFileName[20] - table file name, compiled as .tbl file extension
    **    ObjSize - size of the entire table
    */

    "AMC_MixerCfgTbl", "AMC.MIXERCFG_TBL", "AMC default mixer config table",
    "amc_mixercfg.tbl", (sizeof(AMC_Mixer_ConfigTable_t))
};


/*
** External Global Variables
*/

/*
** Global Variables
*/

/* Default CI config table data */
AMC_Mixer_ConfigTable_t AMC_MixerCfgTbl =
{
    {
        4,           /* Rotor Count or 0 to disable the Multirotor Mixer  */
        1.0, /* Roll Scale                                        */
        1.0, /* Pitch Scale                                       */
        1.0, /* Yaw Scale                                         */
        0.0, /* Idle Speed                                        */
        1.0, /* Delta Out Max                                     */
        MIXER_QUAD_X,   /* Rotor Geometry                     */
        {
                /*
                 *               Front
                 *
                 *          #2           #0
                 *
                 *  Left                       Right
                 *
                 *          #1           #3
                 *
                 *               Back
                 *
                 */
        /* Rotor # | Roll Scale  |  Pitch Scale  |  Yaw Scale  |  Output Scale */
        /*  0  */  { -0.707107,    0.707107,        1.000000,    1.000000 },
        /*  1  */  {  0.707107,   -0.707107,        1.000000,    1.000000 },
        /*  2  */  {  0.707107,    0.707107,       -1.000000,    1.000000 },
        /*  3  */  { -0.707107,   -0.707107,       -1.000000,    1.000000 },
        /*  4  */  {  0.000000,    0.000000,        0.000000,    0.000000 },
        /*  5  */  {  0.000000,    0.000000,        0.000000,    0.000000 },
        /*  6  */  {  0.000000,    0.000000,        0.000000,    0.000000 },
        /*  7  */  {  0.000000,    0.000000,        0.000000,    0.000000 }
        }
    },

    /* Ctrl Count,
     *     Neg Scale, Pos Scale, Offset, Min Output, Max Output      */
    /*         Group, Index, Neg Scale, Pos Scale, Offset, Min Output, Max Output */

    /* Simple Mixers */
    {
        /* Actuator 1 - Right Aileron */
        {
            0,    /* Control Count */
            /* Output Scaler */
            {
                0.0,  /* Negative Scale */
                0.0,  /* Positive Scale */
                0.0,  /* Offset         */
                0.0,  /* Minimum Output */
                0.0   /* Maximum Output */
            },
            /* Controls */
            {
                {   0,  /* Control Group */
                    0,  /* Control Index */
                    /* Scaler */
                    {
                         0.0, /* Negative Scale */
                         0.0, /* Positive Scale */
                         0.0, /* Offset            */
                         0.0, /* Minimum Output */
                         0.0  /* Maximum Output */
                    }
                }
            }
        },
        /* Actuator 2 - Elevator */
        {
            0,    /* Control Count */
            /* Output Scaler */
            {
               0.0,  /* Negative Scale */
               0.0,  /* Positive Scale */
               0.0,  /* Offset         */
               0.0,  /* Minimum Output */
               0.0   /* Maximum Output */
           },
           /* Controls */
           {
               {   0,  /* Control Group */
                   0,  /* Control Index */
                   /* Scaler */
                   {
                      0.0, /* Negative Scale */
                      0.0, /* Positive Scale */
                      0.0, /* Offset         */
                      0.0, /* Minimum Output */
                      0.0  /* Maximum Output */
                   }
               }
            },
        },
        /* Actuator 3 - Left Throttle */
        {
            0,     /* Control Count */
            /* Output Scaler */
            {
               0.0,  /* Negative Scale */
               0.0,  /* Positive Scale */
               0.0,  /* Offset         */
               0.0,  /* Minimum Output */
               0.0   /* Maximum Output */
           },
           /* Controls */
           {
               {
                   0,  /* Control Group */ 
                   0,  /* Control Index */
                   /* Scaler */
                   {
                      0.0, /* Negative Scale */
                      0.0, /* Positive Scale */
                      0.0, /* Offset         */
                      0.0, /* Minimum Output */
                      0.0  /* Maximum Output */
                   }
               }
            },
        },
        /* Actuator 4 - Rudder/Nose Gear */
        {
            0,     /* Control Count */
            /* Output Scaler */
            {
               0.0,  /* Negative Scale */
               0.0,  /* Positive Scale */
               0.0,  /* Offset         */
               0.0,  /* Minimum Output */
               0.0   /* Maximum Output */
           },
           /* Controls */
           {
               {   0,  /* Control Group */
                   0,  /* Control Index */
                   /* Scaler */
                   {  0.0, /* Negative Scale */
                      0.0, /* Positive Scale */
                      0.0, /* Offset         */
                      0.0, /* Minimum Output */
                      0.0  /* Maximum Output */
                   }
               }
            },
        },
        /* Actuator 5 - Unused */
        {
            0,     /* Control Count */
            /* Output Scaler */
            {
                 0.0,  /* Negative Scale */
                 0.0,  /* Positive Scale */
                 0.0,  /* Offset         */
                 0.0,  /* Minimum Output */
                 0.0   /* Maximum Output */
            },
            /* Controls */
            {
                {   0,  /* Control Group */
                    0,  /* Control Index */
                    /* Scaler */
                    {
                         0.0, /* Negative Scale */
                         0.0, /* Positive Scale */
                         0.0, /* Offset         */
                         0.0, /* Minimum Output */
                         0.0  /* Maximum Output */
                    }
                }
            }
        },
        /* Actuator 6 - Left Aileron */
        {
            0,     /* Control Count */
            /* Output Scaler */
            {
                0.0,  /* Negative Scale */
                0.0,  /* Positive Scale */
                0.0,  /* Offset         */
                0.0,  /* Minimum Output */
                0.0   /* Maximum Output */
            },
            /* Controls */
            {
                {   0,  /* Control Group */
                    0,  /* Control Index */
                    /* Scaler */
                    {
                        0.0, /* Negative Scale */
                        0.0, /* Positive Scale */
                        0.0, /* Offset         */
                        0.0, /* Minimum Output */
                        0.0  /* Maximum Output */
                    }
                }
            }
        },
        /* Actuator 7 - Unused */
        {
            0,     /* Control Count */
            /* Output Scaler */
            {
                 0.0,  /* Negative Scale */
                 0.0,  /* Positive Scale */
                 0.0,  /* Offset         */
                 0.0,  /* Minimum Output */
                 0.0   /* Maximum Output */
            },
            /* Controls */
            {
                {   0,  /* Control Group */
                    0,  /* Control Index */
                    /* Scaler */
                    {
                         0.0, /* Negative Scale */
                         0.0, /* Positive Scale */
                         0.0, /* Offset         */
                         0.0, /* Minimum Output */
                         0.0  /* Maximum Output */
                    }
                }
            },
        },
        /* Actuator 8 - Right Throttle */
        {
            0,     /* Control Count */
            /* Output Scaler */
            {
                 0.0,  /* Negative Scale */
                 0.0,  /* Positive Scale */
                 0.0,  /* Offset         */
                 0.0,  /* Minimum Output */
                 0.0   /* Maximum Output */
            },
            /* Controls */
            {
                {   0,  /* Control Group */
                    0,  /* Control Index */
                    /* Scaler */
                    {
                     0.0, /* Negative Scale */
                         0.0, /* Positive Scale */
                         0.0, /* Offset         */
                         0.0, /* Minimum Output */
                         0.0  /* Maximum Output */
                    }
                }
            }
        },
        /* Actuator 9 - Left Flap */
        {
            0,     /* Control Count */
            /* Output Scaler */
            {
                0.0,  /* Negative Scale */
                0.0,  /* Positive Scale */
                0.0,  /* Offset         */
                0.0,  /* Minimum Output */
                0.0   /* Maximum Output */
            },
            /* Controls */
            {
                {   0,  /* Control Group */
                    0,  /* Control Index */
                    /* Scaler */
                    {
                    0.0, /* Negative Scale */
                        0.0, /* Positive Scale */
                        0.0, /* Offset            */
                        0.0, /* Minimum Output */
                        0.0  /* Maximum Output */
                    }
                }
            }
        },
        /* Actuator 10 - Right Flap */
        {
            0,     /* Control Count */
            /* Output Scaler */
            {
                0.0,  /* Negative Scale */
                0.0,  /* Positive Scale */
                0.0,  /* Offset         */
                0.0,  /* Minimum Output */
                0.0   /* Maximum Output */
            },
            /* Controls */
            {
                {   0,  /* Control Group */
                    0,  /* Control Index */
                    /* Scaler */
                    {
                    0.0, /* Negative Scale */
                        0.0, /* Positive Scale */
                        0.0, /* Offset            */
                        0.0, /* Minimum Output */
                        0.0  /* Maximum Output */
                    }
                }
            }
        }
    }
};

/*
** Local Variables
*/

/*
** Function Prototypes
*/

/*
** Function Definitions
*/

/*=======================================================================================
** End of file ci_config.c
**=====================================================================================*/
    
