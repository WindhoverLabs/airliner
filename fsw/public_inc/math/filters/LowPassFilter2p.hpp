// -*- tab-width: 4; Mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-

/****************************************************************************
 *
 *   Copyright (C) 2012 PX4 Development Team. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name PX4 nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/// @file	LowPassFilter.h
/// @brief	A class to implement a second order low pass filter
/// Author: Leonard Hall <LeonardTHall@gmail.com>
/// Adapted for PX4 by Andrew Tridgell

//#pragma once
#ifndef LOWPASSFILTER2P_H
#define LOWPASSFILTER2P_H

namespace math
{
class LowPassFilter2p
{
public:
	/************************************************************************/
	/** \brief Low Pass Filter Constructor
	 **
	 **  \par Description
	 **       This function initializes the object with passed values.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   sample_freq      A #float specifying frequency of the sample
	 **  \param [in]   cutoff_freq      A #float specifying the cutoff frequency
	 **
	 *************************************************************************/
	LowPassFilter2p(float sample_freq, float cutoff_freq) :
		_cutoff_freq(cutoff_freq),
		_a1(0.0f),
		_a2(0.0f),
		_b0(0.0f),
		_b1(0.0f),
		_b2(0.0f),
		_delay_element_1(0.0f),
		_delay_element_2(0.0f)
	{
		// set initial parameters
		set_cutoff_frequency(sample_freq, cutoff_freq);
	}

	/************************************************************************/
	/** \brief Low Pass Filter Mutator
	 **
	 **  \par Description
	 **       This function updates the object attributes with passed values.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   sample_freq      A #float specifying frequency of the sample
	 **  \param [in]   cutoff_freq      A #float specifying the cutoff frequency
	 **
	 *************************************************************************/
	void set_cutoff_frequency(float sample_freq, float cutoff_freq);

	/************************************************************************/
	/** \brief Low Pass Filter Mutator
	 **
	 **  \par Description
	 **       This function updates the object attributes with passed values.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   sample      A #float of a new raw value to add to the filter
	 **
	 *************************************************************************/
	float apply(float sample);

	/************************************************************************/
	/** \brief Low Pass Filter Accessor
	 **
	 **  \par Description
	 **       This function returns the cutoff frequency
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
 	 **  \returns
	 **  Cutoff frequency
	 **  \endreturns
	 **
	 *************************************************************************/
	float get_cutoff_freq() const
	{
		return _cutoff_freq;
	}

	/************************************************************************/
	/** \brief Low Pass Filter Mutator
	 **
	 **  \par Description
	 **       This function resets the filter state to passed value.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   sample      A #float of the new raw value
	 **
	 **  \returns
	 **  Filtered sample
	 **  \endreturns
	 **
	 *************************************************************************/
	float reset(float sample);

private:
	/** \brief Cutoff Frequency */
	float           _cutoff_freq;

	/** \brief Equation variable */
	float           _a1;

	/** \brief Equation variable */
	float           _a2;

	/** \brief Equation variable */
	float           _b0;

	/** \brief Equation variable */
	float           _b1;

	/** \brief Equation variable */
	float           _b2;

	/** \brief Buffered sample -1 */
	float           _delay_element_1;


	/** \brief Buffered sample -2*/
	float           _delay_element_2;
};

} // namespace math
#endif // LOWPASSFILTER2P_H
