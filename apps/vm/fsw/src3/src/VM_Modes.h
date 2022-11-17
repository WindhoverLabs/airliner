#ifndef VM_MODES_H
#define VM_MODES_H

struct VM_Modes {

public:
	/**
	 * \brief Position control is selected
	 */
	osalbool inPosCtl;
	/**
	 * \brief Return to launch is selected
	 */
	osalbool inRtl;
	/**
	 * \brief Auto loiter is selected
	 */
	osalbool inLoiter;
	/**
	 * \brief Manual is selected
	 */
	osalbool inManual;
	/**
	 * \brief Takeoff is selected
	 */
	osalbool inTakeoff;
	/**
	 * \brief Altitude control is selected
	 */
	osalbool inAltCtl;
};

#endif
