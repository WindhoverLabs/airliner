#include "ECL_PitchController.hpp"
//#include <math.h>
//#include <mathlib/math/Limits.hpp>
//#include <float.h>

#include "math/Limits.hpp"
#include "math/Vector2F.hpp"
#include "math/Euler.hpp"
#include "math/Quaternion.hpp"
#include "px4lib.h"



ECL_PitchController::ECL_PitchController() :
    ECL_Controller("pitch"),
    _max_rate_neg(0.0f),
    _roll_ff(0.0f)
{

}

float ECL_PitchController::control_attitude(const struct ECL_ControlData &ctl_data)
{

	/* Do not calculate control signal with bad inputs */
	if (!(isfinite(ctl_data.pitch_setpoint) &&
		isfinite(ctl_data.roll) &&
		isfinite(ctl_data.pitch) &&
		isfinite(ctl_data.airspeed)))
	{
		/* TODO: Replace with CFE EVS Event message. */
		//warnx("not controlling pitch");
		return _rate_setpoint;
	}

	/* Calculate the error */
	float pitch_error = ctl_data.pitch_setpoint - ctl_data.pitch;

	/*  Apply P controller: rate setpoint from current error and time constant */
	_rate_setpoint =  pitch_error / _tc;

	/* limit the rate */
	if (_max_rate > 0.01f && _max_rate_neg > 0.01f) {
		if (_rate_setpoint > 0.0f) {
			_rate_setpoint = (_rate_setpoint > _max_rate) ? _max_rate : _rate_setpoint;

		} else {
			_rate_setpoint = (_rate_setpoint < -_max_rate_neg) ? -_max_rate_neg : _rate_setpoint;
		}

	}

	return _rate_setpoint;
}

float ECL_PitchController::control_bodyrate(const struct ECL_ControlData &ctl_data)
{
	/* Do not calculate control signal with bad inputs */
	if (!(isfinite(ctl_data.roll) &&
		  isfinite(ctl_data.pitch) &&
		  isfinite(ctl_data.body_y_rate) &&
		  isfinite(ctl_data.body_z_rate) &&
		  isfinite(ctl_data.yaw_rate_setpoint) &&
		  isfinite(ctl_data.airspeed_min) &&
		  isfinite(ctl_data.airspeed_max) &&
		  isfinite(ctl_data.scaler))) {
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

	_rate_error = _bodyrate_setpoint - ctl_data.body_y_rate;

	if (!lock_integrator && _k_i > 0.0f) {

		float id = _rate_error * dt * ctl_data.scaler;

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
	_last_output = _bodyrate_setpoint * _k_ff * ctl_data.scaler +
		       _rate_error * _k_p * ctl_data.scaler * ctl_data.scaler
		       + integrator_constrained;  //scaler is proportional to 1/airspeed

	return math::constrain(_last_output, -1.0f, 1.0f);
}

float ECL_PitchController::control_euler_rate(const struct ECL_ControlData &ctl_data)
{
	/* Transform setpoint to body angular rates (jacobian) */
	_bodyrate_setpoint = cosf(ctl_data.roll) * _rate_setpoint +
			     cosf(ctl_data.pitch) * sinf(ctl_data.roll) * ctl_data.yaw_rate_setpoint;

	return control_bodyrate(ctl_data);
}

