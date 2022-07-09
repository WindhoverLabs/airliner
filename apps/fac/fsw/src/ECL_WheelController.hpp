#ifndef ECL_WHEELCONTROLLER_HPP
#define ECL_WHEELCONTROLLER_HPP

#include "ECL_Controller.hpp"

class ECL_WheelController :
	public ECL_Controller
{
public:
	ECL_WheelController();
	~ECL_WheelController() = default;

	float control_attitude(const struct ECL_ControlData &ctl_data);

	float control_bodyrate(const struct ECL_ControlData &ctl_data);

	float control_euler_rate(const struct ECL_ControlData &ctl_data) {return 0;}
};

#endif
