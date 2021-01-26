#ifndef MULTIROTOR_MIXER_H
#define MULTIROTOR_MIXER_H

#include "cfe.h"
#include "Mixer.h"
#include "MixerTables.h"


class UT_MultirotorMixer : public Mixer
{
public:
	/**
	 * Constructor.
	 *
	 * @param control_cb		Callback invoked to read inputs.
	 * @param cb_handle		Passed to control_cb.
	 * @param geometry		The selected geometry.
	 * @param roll_scale		Scaling factor applied to roll inputs
	 *				compared to thrust.
	 * @param pitch_scale		Scaling factor applied to pitch inputs
	 *				compared to thrust.
	 * @param yaw_wcale		Scaling factor applied to yaw inputs compared
	 *				to thrust.
	 * @param idle_speed		Minimum rotor control output value; usually
	 *				tuned to ensure that rotors never stall at the
	 * 				low end of their control range.
	 */
	UT_MultirotorMixer();
	~UT_MultirotorMixer();

    int32 SetConfigTablePtr(MultirotorMixer_ConfigTablePtr_t &ConfigTablePtr);



	/**
	 * Factory method.
	 *
	 * Given a pointer to a buffer containing a text description of the mixer,
	 * returns a pointer to a new instance of the mixer.
	 *
	 * @param control_cb		The callback to invoke when fetching a
	 *				control value.
	 * @param cb_handle		Handle passed to the control callback.
	 * @param buf			Buffer containing a text description of
	 *				the mixer.
	 * @param buflen		Length of the buffer in bytes, adjusted
	 *				to reflect the bytes consumed.
	 * @return			A new MultirotorMixer instance, or nullptr
	 *				if the text format is bad.
	 */
	static MultirotorMixer		*from_text(Mixer::ControlCallback control_cb,
			cpuaddr cb_handle,
			const char *buf,
			uint32 &buflen);

	virtual uint32		mix(float *outputs, uint32 space, uint16 *status_reg);
	virtual uint16		get_saturation_status(void);
	virtual void		groups_required(uint32 &groups);

	uint32 set_trim(float trim)
	{
		return 0;
	}

	/**
	 * @brief      Sets the thrust factor used to calculate mapping from desired thrust to pwm.
	 *
	 * @param[in]  val   The value
	 */
	virtual void			set_thrust_factor(float val) {m_ThrustFactor = val;}

private:
	float 			   m_ThrustFactor;

	typedef union {
		struct {
			uint16 motor_pos	: 1; // 0 - true when any motor has saturated in the positive direction
			uint16 motor_neg	: 1; // 1 - true when any motor has saturated in the negative direction
			uint16 roll_pos	    : 1; // 2 - true when a positive roll demand change will increase saturation
			uint16 roll_neg	    : 1; // 3 - true when a negative roll demand change will increase saturation
			uint16 pitch_pos	: 1; // 4 - true when a positive pitch demand change will increase saturation
			uint16 pitch_neg	: 1; // 5 - true when a negative pitch demand change will increase saturation
			uint16 yaw_pos	    : 1; // 6 - true when a positive yaw demand change will increase saturation
			uint16 yaw_neg	    : 1; // 7 - true when a negative yaw demand change will increase saturation
			uint16 thrust_pos	: 1; // 8 - true when a positive thrust demand change will increase saturation
			uint16 thrust_neg	: 1; // 9 - true when a negative thrust demand change will increase saturation
		} flags;
		uint16 value;
	} SaturationStatus_t;

	SaturationStatus_t m_SaturationStatus;

	void update_saturation_status(uint32 index, bool clipping_high, bool clipping_low);

	float 			   m_OutputsPrev[MULTIROTOR_MIXER_MAX_ROTOR_COUNT];
	MultirotorMixer_ConfigTablePtr_t m_ConfigTablePtr;

	/* do not allow to copy due to ptr data members */
	MultirotorMixer(const MultirotorMixer &);
	MultirotorMixer operator=(const MultirotorMixer &);
};


#endif
