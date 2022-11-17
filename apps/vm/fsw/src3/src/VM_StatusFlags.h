#ifndef VM_STATUSFLAGS_H
#define VM_STATUSFLAGS_H

struct VM_StatusFlags {

public:
	/**
	 * \brief Indicates if all sensors are initialized
	 */
	osalbool SensorsInitialized;
	/**
	 * \brief System in rtl state
	 */
	osalbool ReturnToHomeSet;
	/**
	 * \brief Indicates a valid home position (a valid home position is not always a valid launch)
	 */
	osalbool HomePositionValid;
	/**
	 * \brief Satus of the USB power supply
	 */
	osalbool UsbPowerConnected;
	/**
	 * \brief True if RC signal found atleast once
	 */
	osalbool RcSignalFoundOnce;
	/**
	 * \brief True if RC lost mode is commanded
	 */
	osalbool RcSignalLostModeIsCmded;
	/**
	 * \brief Set if RC input should be ignored temporarily
	 */
	osalbool RcInputIsTemporarilyBlocked;
};

#endif
