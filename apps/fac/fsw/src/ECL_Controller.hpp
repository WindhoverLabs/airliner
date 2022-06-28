#ifndef ECL_CONTROLLER_HPP
#define ECL_CONTROLLER_HPP

#include <math.h>
#include "cfe.h"

struct ECL_ControlData {
	float roll;
	float pitch;
	float yaw;
	float body_x_rate;
	float body_y_rate;
	float body_z_rate;
	float roll_setpoint;
	float pitch_setpoint;
	float yaw_setpoint;
	float roll_rate_setpoint;
	float pitch_rate_setpoint;
	float yaw_rate_setpoint;
	float airspeed_min;
	float airspeed_max;
	float airspeed;
	float scaler;
	float groundspeed;
	float groundspeed_scaler;
	bool  lock_integrator;
};

class ECL_Controller
{
public:
	ECL_Controller(const char *name);
	~ECL_Controller() = default;

	virtual float control_attitude(const struct ECL_ControlData &ctl_data) = 0;
	virtual float control_euler_rate(const struct ECL_ControlData &ctl_data) = 0;
	virtual float control_bodyrate(const struct ECL_ControlData &ctl_data) = 0;

	/* Setters */
	void set_time_constant(float time_constant);
	void set_k_p(float k_p);
	void set_k_i(float k_i);
	void set_k_ff(float k_ff);
	void set_integrator_max(float max);
	void set_max_rate(float max_rate);
	void set_bodyrate_setpoint(float rate) {_bodyrate_setpoint = rate;}

	/* Getters */
	float get_rate_error();
	float get_desired_rate();
	float get_desired_bodyrate();

	void  reset_integrator();

	void init(void);

protected:
	uint64 _last_run;
	float _tc;
	float _k_p;
	float _k_i;
	float _k_ff;
	float _integrator_max;
	float _max_rate;
	float _last_output;
	float _integrator;
	float _rate_error;
	float _rate_setpoint;
	float _bodyrate_setpoint;
	float constrain_airspeed(float airspeed, float minspeed, float maxspeed);
};

#endif
