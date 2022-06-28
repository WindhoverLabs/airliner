#ifndef ECL_PITCHCONTROLLER_HPP
#define ECL_PITCHCONTROLLER_HPP

#include "ECL_Controller.hpp"

class ECL_PitchController :
	public ECL_Controller
{
public:
	ECL_PitchController();
   ~ECL_PitchController() = default;

	float control_attitude(const struct ECL_ControlData &ctl_data);
	float control_euler_rate(const struct ECL_ControlData &ctl_data);
	float control_bodyrate(const struct ECL_ControlData &ctl_data);

	/* Additional Setters */
	void set_max_rate_pos(float max_rate_pos)
	{
		_max_rate = max_rate_pos;
	}

	void set_max_rate_neg(float max_rate_neg)
	{
		_max_rate_neg = max_rate_neg;
	}

	void set_roll_ff(float roll_ff)
	{
		_roll_ff = roll_ff;
	}

	protected:
		float _max_rate_neg;
		float _roll_ff;
};

#endif
