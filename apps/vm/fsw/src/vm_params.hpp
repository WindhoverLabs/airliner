PRMLIB_ParamRegistrations_t ParamRegistrations =
{
    VM_PARAM_PIPE_NAME,
    VM_PARAM_PIPE_DEPTH,
    PARAM_ID_COM_RC_IN_MODE,       &ConfigTblPtr->COM_RC_IN_MODE,       TYPE_UINT32,
    PARAM_ID_COM_ARM_SWISBTN,      &ConfigTblPtr->COM_ARM_SWISBTN,      TYPE_UINT32,
    PARAM_ID_COM_RC_ARM_HYST,      &ConfigTblPtr->COM_RC_ARM_HYST,      TYPE_UINT32,
    PARAM_ID_MAV_SYS_ID,           &ConfigTblPtr->MAV_SYS_ID,           TYPE_UINT32,
    PARAM_ID_MAV_COMP_ID,          &ConfigTblPtr->MAV_COMP_ID,          TYPE_UINT32,
    PARAM_ID_COM_RC_LOSS_T,        &ConfigTblPtr->COM_RC_LOSS_T,        TYPE_REAL32,
    PARAM_ID_COM_LOW_BAT_ACT,      &ConfigTblPtr->COM_LOW_BAT_ACT,      TYPE_UINT32,
    PARAM_ID_COM_HOME_H_T,         &ConfigTblPtr->COM_HOME_H_T,         TYPE_REAL32,
    PARAM_ID_COM_HOME_V_T,         &ConfigTblPtr->COM_HOME_V_T,         TYPE_REAL32,
    PARAM_ID_HOME_POS_ALT_PADDING, &ConfigTblPtr->HOME_POS_ALT_PADDING, TYPE_REAL32,
    ""
};
