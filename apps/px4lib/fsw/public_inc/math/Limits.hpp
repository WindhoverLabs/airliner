/****************************************************************************
 *
 *   Copyright (c) 2013 PX4 Development Team. All rights reserved.
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

/**
 * @file Limits.hpp
 *
 * Limiting / constrain helper functions
 */

#pragma once

namespace math
{

/************************************************************************/
/** \brief Float Min
 **
 **  \par Description
 **       This function return the smaller of two passed floats
 **
 **  \par Assumptions, External Events, and Notes:
 **       None
 **
 **  \param [in]   a      A #float to compare
 **  \param [in]   b      A #float to compare
 **
 **  \returns
 **  Min of two arguments
 **  \endreturns
 *************************************************************************/
const float &min(const float &a, const float &b);

/************************************************************************/
/** \brief Float Max
 **
 **  \par Description
 **       This function return the larger of two passed floats
 **
 **  \par Assumptions, External Events, and Notes:
 **       None
 **
 **  \param [in]   a      A #float to compare
 **  \param [in]   b      A #float to compare
 **
 **  \returns
 **  Max of two arguments
 **  \endreturns
 **
 *************************************************************************/
const float &max(const float &a, const float &b);

/************************************************************************/
/** \brief Constrain
 **
 **  \par Description
 **       This function constrains a value between a passed range.
 **
 **  \par Assumptions, External Events, and Notes:
 **       None
 **
 **  \param [in]   val    A #float index of value to constrain
 **  \param [in]   min    A #float index of value to constrain
 **  \param [in]   max    A #float index of value to constrain
 **
 **  \returns
 **  Constrained value
 **  \endreturns
 **
 *************************************************************************/
const float &constrain(const float &val, const float &min_val, const float &max_val);

/************************************************************************/
/** \brief Float Degrees To Radians
 **
 **  \par Description
 **       This function converts degrees to radians
 **
 **  \par Assumptions, External Events, and Notes:
 **       None
 **
 **  \param [in]   degrees      A #float value in degrees
 **
 **  \returns
 **  Value in radians
 **  \endreturns
 **
 *************************************************************************/
float radians(float degrees);

/************************************************************************/
/** \brief Double Degrees To Radians
 **
 **  \par Description
 **       This function converts degrees to radians
 **
 **  \par Assumptions, External Events, and Notes:
 **       None
 **
 **  \param [in]   degrees      A #double value in degrees
 **
 **  \returns
 **  Value in radians
 **  \endreturns
 **
 *************************************************************************/
double radians(double degrees);

/************************************************************************/
/** \brief Float Radians To Degrees
 **
 **  \par Description
 **       This function converts radians to degrees
 **
 **  \par Assumptions, External Events, and Notes:
 **       None
 **
 **  \param [in]   degrees      A #float value in radians
 **
 **  \returns
 **  Value in degrees
 **  \endreturns
 **
 *************************************************************************/
float degrees(float radians);

/************************************************************************/
/** \brief Double Radians To Degrees
 **
 **  \par Description
 **       This function converts radians to degrees
 **
 **  \par Assumptions, External Events, and Notes:
 **       None
 **
 **  \param [in]   degrees      A #double value in radians
 **
 **  \returns
 **  Value in degrees
 **  \endreturns
 **
 *************************************************************************/
double degrees(double radians);

}
