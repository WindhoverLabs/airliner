#ifndef VM_BATTERYFAILSAFEMODE_T_H
#define VM_BATTERYFAILSAFEMODE_T_H

enum VM_BatteryFailsafeMode_t {
	/**
	 * \brief Publish a warning event.
	 */
	VM_BATTERY_FAILSAFE_MODE_WARNING = 0, 
	/**
	 * \brief Return to Launch.
	 */
	VM_BATTERY_FAILSAFE_MODE_RETURN = 1, 
	/**
	 * \brief Land immediately.
	 */
	VM_BATTERY_FAILSAFE_MODE_LAND = 2, 
	/**
	 * \brief Return to Launch if low, but land immediately if even lower.
	 */
	VM_BATTERY_FAILSAFE_MODE_RETURN_IF_CRIT_LOW_LAND_IF_DANGER_LOW = 3
};

#endif
