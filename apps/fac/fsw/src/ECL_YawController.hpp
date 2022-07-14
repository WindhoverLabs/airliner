#ifndef ECL_YAWCONTROLLER_HPP
#define ECL_YAWCONTROLLER_HPP

#include "ECL_Controller.hpp"

class ECL_YawController :
	public ECL_Controller
{
public:
	ECL_YawController();
	~ECL_YawController() = default;

	float control_attitude(const struct ECL_ControlData &ctl_data);
	float control_euler_rate(const struct ECL_ControlData &ctl_data);
	float control_bodyrate(const struct ECL_ControlData &ctl_data);

	/* Additional setters */
	void set_coordinated_min_speed(float coordinated_min_speed)
	{
		_coordinated_min_speed = coordinated_min_speed;
	}

	void set_coordinated_method(int32_t coordinated_method)
	{
		_coordinated_method = coordinated_method;
	}

	enum {
		COORD_METHOD_OPEN = 0,
		COORD_METHOD_CLOSEACC = 1
	};

protected:
	float _coordinated_min_speed;
	float _max_rate;

	int32_t _coordinated_method;

	float control_attitude_impl_openloop(const struct ECL_ControlData &ctl_data);

	float control_attitude_impl_accclosedloop(const struct ECL_ControlData &ctl_data);
};

#endif
