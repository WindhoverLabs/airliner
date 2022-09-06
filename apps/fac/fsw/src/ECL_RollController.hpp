#ifndef ECL_ROLLCONTROLLER_HPP
#define ECL_ROLLCONTROLLER_HPP

#include "ECL_Controller.hpp"

class ECL_RollController :
	public ECL_Controller
{
public:
	ECL_RollController();
	~ECL_RollController() = default;

	float control_attitude(const struct ECL_ControlData &ctl_data);
	float control_euler_rate(const struct ECL_ControlData &ctl_data);
	float control_bodyrate(const struct ECL_ControlData &ctl_data);
};

#endif
