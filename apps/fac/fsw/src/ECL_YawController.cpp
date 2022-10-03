#include "ECL_YawController.hpp"

#include "math/Limits.hpp"
#include "px4lib.h"




ECL_YawController::ECL_YawController() :
	ECL_Controller("yaw"),
	_coordinated_min_speed(1.0f),
	_coordinated_method(0)
{
}

float ECL_YawController::control_attitude(const struct ECL_ControlData &ctl_data)
{
	switch (_coordinated_method) {
	case COORD_METHOD_OPEN:
		return control_attitude_impl_openloop(ctl_data);

	case COORD_METHOD_CLOSEACC:
		return control_attitude_impl_accclosedloop(ctl_data);

	default:
		static uint64 last_print = 0;

		if (PX4LIB_GetPX4ElapsedTimeUs(last_print) > 5e6) {
			/* TODO: Replace with CFE EVS event message */
			//warnx("invalid param setting FW_YCO_METHOD");
			printf("invalid param setting FW_YCO_METHOD\n");
			last_print = PX4LIB_GetPX4TimeUs();
		}
	}

	return _rate_setpoint;
}

float ECL_YawController::control_attitude_impl_openloop(const struct ECL_ControlData &ctl_data)
{
	/* Do not calculate control signal with bad inputs */
	if (!(isfinite(ctl_data.roll) &&
	      isfinite(ctl_data.pitch) &&
		  isfinite(ctl_data.roll_rate_setpoint) &&
		  isfinite(ctl_data.pitch_rate_setpoint))) {
		return _rate_setpoint;
	}

	float constrained_roll;
	bool inverted = false;

	/* roll is used as feedforward term and inverted flight needs to be considered */
	if (fabsf(ctl_data.roll) < math::radians(90.0f)) {
		/* not inverted, but numerically still potentially close to infinity */
		constrained_roll = math::constrain(ctl_data.roll, math::radians(-80.0f), math::radians(80.0f));

	} else {
		inverted = true;

		// inverted flight, constrain on the two extremes of -pi..+pi to avoid infinity
		//note: the ranges are extended by 10 deg here to avoid numeric resolution effects
		if (ctl_data.roll > 0.0f) {
			/* right hemisphere */
			constrained_roll = math::constrain(ctl_data.roll, math::radians(100.0f), math::radians(180.0f));

		} else {
			/* left hemisphere */
			constrained_roll = math::constrain(ctl_data.roll, math::radians(-180.0f), math::radians(-100.0f));
		}
	}

	constrained_roll = math::constrain(constrained_roll, -fabsf(ctl_data.roll_setpoint), fabsf(ctl_data.roll_setpoint));


	if (!inverted) {
		/* Calculate desired yaw rate from coordinated turn constraint / (no side forces) */
		_rate_setpoint = tanf(constrained_roll) * cosf(ctl_data.pitch) * 9.81f / (ctl_data.airspeed < ctl_data.airspeed_min ?
				 ctl_data.airspeed_min : ctl_data.airspeed);
	}

	/* limit the rate */ //XXX: move to body angluar rates

	if (_max_rate > 0.01f) {
		_rate_setpoint = (_rate_setpoint > _max_rate) ? _max_rate : _rate_setpoint;
		_rate_setpoint = (_rate_setpoint < -_max_rate) ? -_max_rate : _rate_setpoint;
	}

	if (!isfinite(_rate_setpoint)) {
		/* TODO: Replace with CFE EVS event message. */
		//warnx("yaw rate sepoint not finite");
		_rate_setpoint = 0.0f;
	}

	return _rate_setpoint;
}

float ECL_YawController::control_bodyrate(const struct ECL_ControlData &ctl_data)
{
	/* Do not calculate control signal with bad inputs */
	if (!(isfinite(ctl_data.roll) && isfinite(ctl_data.pitch) && isfinite(ctl_data.body_y_rate) &&
	      isfinite(ctl_data.body_z_rate) && isfinite(ctl_data.pitch_rate_setpoint) &&
	      isfinite(ctl_data.airspeed_min) && isfinite(ctl_data.airspeed_max) &&
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

	/* input conditioning */
	float airspeed = ctl_data.airspeed;

	if (!isfinite(airspeed)) {
		/* airspeed is NaN, +- INF or not available, pick center of band */
		airspeed = 0.5f * (ctl_data.airspeed_min + ctl_data.airspeed_max);

	} else if (airspeed < ctl_data.airspeed_min) {
		airspeed = ctl_data.airspeed_min;
	}

	/* Close the acceleration loop if _coordinated_method wants this: change body_rate setpoint */
	if (_coordinated_method == COORD_METHOD_CLOSEACC) {
		// XXX lateral acceleration needs to go into integrator with a gain
		//_bodyrate_setpoint -= (ctl_data.acc_body_y / (airspeed * cosf(ctl_data.pitch)));
	}

	/* Calculate body angular rate error */
	_rate_error = _bodyrate_setpoint - ctl_data.body_z_rate; // body angular rate error

	if (!lock_integrator && _k_i > 0.0f && airspeed > 0.5f * ctl_data.airspeed_min) {

		float id = _rate_error * dt;

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
	_last_output = (_bodyrate_setpoint * _k_ff + _rate_error * _k_p + integrator_constrained) * ctl_data.scaler *
		       ctl_data.scaler;  //scaler is proportional to 1/airspeed


	return math::constrain(_last_output, -1.0f, 1.0f);
}

float ECL_YawController::control_attitude_impl_accclosedloop(const struct ECL_ControlData &ctl_data)
{
	/* dont set a rate setpoint */
	return 0.0f;
}

float ECL_YawController::control_euler_rate(const struct ECL_ControlData &ctl_data)
{
	/* Transform setpoint to body angular rates (jacobian) */
	_bodyrate_setpoint = -sinf(ctl_data.roll) * ctl_data.pitch_rate_setpoint +
			     cosf(ctl_data.roll) * cosf(ctl_data.pitch) * _rate_setpoint;

	return control_bodyrate(ctl_data);

}
