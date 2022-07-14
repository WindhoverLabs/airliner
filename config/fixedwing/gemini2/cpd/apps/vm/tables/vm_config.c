/*
** Include Files
*/
#include "cfe_tbl_filedef.h"
#include "vm_tbldefs.h"

static OS_USED CFE_TBL_FileDef_t CFE_TBL_FileDef =
{
    "VM_ConfigTbl", "VM.CONFIG_TBL", "VM default config table",
    "vm_config.tbl", (sizeof(VM_ConfigTbl_t))
};

/* Default ULR config table data */
VM_ConfigTbl_t VM_ConfigTbl =
{
    /* COM_RC_IN_MODE       */     2,
    /* COM_ARM_SWISBTN      */     0,
    /* COM_RC_ARM_HYST      */  1000,
    /* MAV_SYS_ID           */     1,
    /* MAV_COMP_ID          */     1,
    /* COM_RC_LOSS_T        */     2,
    /* COM_LOW_BAT_ACT      */     1,
    /* COM_HOME_H_T         */   5.0,
    /* COM_HOME_V_T         */  10.0,
    /* HOME_POS_ALT_PADDING */   0.0,
    /* VEHICLE_TYPE         */   VM_VEHICLE_TYPE_FIXED_WING

};

/*=======================================================================================
** End of file vm_config.c
**=====================================================================================*/
    
