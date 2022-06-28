#include "ECL_WheelController.hpp"

#include "math/Limits.hpp"
#include "px4lib.h"
#include "geo/geo.h"


ECL_WheelController::ECL_WheelController() :
	ECL_Controller("wheel")
{
}

float ECL_WheelController::control_bodyrate(const struct ECL_ControlData &ctl_data)
{
	/* Do not calculate control signal with bad inputs */
	if (!(isfinite(ctl_data.body_z_rate) &&
	      isfinite(ctl_data.groundspeed) &&
	      isfinite(ctl_data.groundspeed_scaler))) {
		return math::constrain(_last_output, -1.0f, 1.0f);
	}

	/* get the usual dt estimate */
	uint64 dt_micros = PX4LIB_GetPX4ElapsedTimeUs(_last_run);
	_last_run = PX4LIB_GetPX4TimeUs();
	float dt = (float)dt_micros * 1e-6f;

	/* lock integral for long intervals */
	bool lock_integrator = ctl_data.lock_integrator;

	if (dt_micros > 500000) {
		lock_integrator = true;
	}

	/* input conditioning */
	float min_speed = 1.0f;

	/* Calculate body angular rate error */
	_rate_error = _rate_setpoint - ctl_data.body_z_rate; //body angular rate error

	if (!lock_integrator && _k_i > 0.0f && ctl_data.groundspeed > min_speed) {

		float id = _rate_error * dt * ctl_data.groundspeed_scaler;

		/*
		 * anti-windup: do not allow integrator to increase if actuator is at limit
		 */
		if (_last_output < -1.0f) {
			/* only allow motion to center: increase value */
			id = math::max(id, 0.0f);

		} else if (_last_output > 1.0f) {
			/* only allow motion to center: decrease value */
			id = math::min(id, 0.0f);
		}

		_integrator += id * _k_i;
	}

	/* integrator limit */
	//xxx: until start detection is available: integral part in control signal is limited here
	float integrator_constrained = math::constrain(_integrator, -_integrator_max, _integrator_max);

	/* Apply PI rate controller and store non-limited output */
	_last_output = _rate_setpoint * _k_ff * ctl_data.groundspeed_scaler +
		       ctl_data.groundspeed_scaler * ctl_data.groundspeed_scaler * (_rate_error * _k_p + integrator_constrained);

	return math::constrain(_last_output, -1.0f, 1.0f);
}


float ECL_WheelController::control_attitude(const struct ECL_ControlData &ctl_data)
{
	/* Do not calculate control signal with bad inputs */
	if (!(isfinite(ctl_data.yaw_setpoint) &&
	      isfinite(ctl_data.yaw))) {
		return _rate_setpoint;
	}

	/* Calculate the error */
	float yaw_error = _wrap_pi(ctl_data.yaw_setpoint - ctl_data.yaw);

	/*  Apply P controller: rate setpoint from current error and time constant */
	_rate_setpoint =  yaw_error / _tc;

	/* limit the rate */
	if (_max_rate > 0.01f) {
		if (_rate_setpoint > 0.0f) {
			_rate_setpoint = (_rate_setpoint > _max_rate) ? _max_rate : _rate_setpoint;

		} else {
			_rate_setpoint = (_rate_setpoint < -_max_rate) ? -_max_rate : _rate_setpoint;
		}

	}

	return _rate_setpoint;
}
