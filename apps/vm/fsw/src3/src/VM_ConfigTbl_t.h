#ifndef VM_CONFIGTBL_T_H
#define VM_CONFIGTBL_T_H

struct VM_ConfigTbl_t {

public:
	/**
	 * \vmcfg COM_RC_IN_MODE
	 * 
	 * \brief RC control input mode
	 * 
	 * \par Limits:
	 * Min > Max (incr.) 0 > #COM_RC_IN_MODE_MAX.
	 */
	uint32 COM_RC_IN_MODE;
	/**
	 * \vmcfg COM_ARM_SWISBTN
	 * 
	 * \brief Arm switch is only a button
	 * 
	 * \par Limits:
	 * Min > Max (incr.) 0 > 1, default 0.
	 */
	uint32 COM_ARM_SWISBTN;
	/**
	 * \vmcfg COM_RC_ARM_HYST
	 * 
	 * \brief RC input arm/disarm command duration
	 * 
	 * \par Limits:
	 * Min > Max (incr.) #COM_RC_ARM_HYST_MIN > #COM_RC_ARM_HYST_MAX.
	 */
	uint32 COM_RC_ARM_HYST;
	/**
	 * \vmcfg MAV_SYS_ID
	 * 
	 * \brief System ID
	 * 
	 * \par Limits:
	 * Min > Max (incr.) #MAV_SYS_ID_MIN > #MAV_SYS_ID_MAX.
	 */
	uint32 MAV_SYS_ID;
	/**
	 * \vmcfg MAV_COMP_ID
	 * 
	 * \brief Component ID
	 * 
	 * \par Limits:
	 * Min > Max (incr.) #MAV_COMP_ID_MIN > #MAV_COMP_ID_MAX.
	 */
	uint32 MAV_COMP_ID;
	/**
	 * \vmcfg COM_RC_LOSS_T
	 * 
	 * \brief RC loss time threshold
	 * 
	 * \par Limits:
	 * Min > Max (incr.) 0.0 > #COM_RC_LOSS_T_MAX.
	 */
	float COM_RC_LOSS_T;
	/**
	 * \vmcfg COM_LOW_BAT_ACT
	 * 
	 * \brief Battery failsafe mode
	 * 
	 * \par Limits:
	 * Min > Max (incr.) 0 > #COM_LOW_BAT_ACT
	 */
	uint32 COM_LOW_BAT_ACT;
	/**
	 * \vmcfg COM_HOME_H_T
	 * 
	 * \brief Home set horizontal threshold
	 * 
	 * \par Limits:
	 * Min > Max (incr.) #COM_HOME_H_T_MIN > #COM_HOME_H_T_MAX
	 */
	float COM_HOME_H_T;
	/**
	 * \vmcfg COM_HOME_V_T
	 * 
	 * \brief Home set vertical threshold
	 * 
	 * \par Limits:
	 * Min > Max (incr.) #COM_HOME_V_T_MIN > #COM_HOME_V_T_MAX
	 */
	float COM_HOME_V_T;
	/**
	 * \vmcfg HOME_POS_ALT_PADDING
	 * 
	 * \brief Padding for home position altitude message, to avoid hover above ground.
	 * 
	 * \par Limits:
	 * None.
	 */
	float HOME_POS_ALT_PADDING;
	/**
	 * TODO
	 */
	VM_VehicleType_t VEHICLE_TYPE;
};

#endif
