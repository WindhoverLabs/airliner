
/**
 * @file Expo.hpp
 *
 * So called exponential curve function implementation.
 * It is essentially a linear combination between a linear and a cubic function.
 * It's used in the range [-1,1]
 */

#ifndef EXPO_HPP
#define EXPO_HPP

namespace math
{

/************************************************************************/
/** \brief Sign Function
 **
 **  \par Description
 **       This function calculates the sign of a number
 **
 **  \par Assumptions, External Events, and Notes:
 **       None
 **
 **  \returns
 **  An #int of the correct sign
 **  \endreturns
 **
 *************************************************************************/
int signf(float val);

/************************************************************************/
/** \brief Exponential TODO
 **
 **  \par Description
 **       This function
 **
 **  \par Assumptions, External Events, and Notes:
 **       None
 **
 **  \param [in]       value      Value of #float to
 **  \param [in]  	   e   		  Exponential?
 **
 **
 **  \returns
 **
 **  \endreturns
 *************************************************************************/
const float expof(const float &value, const float &e);

/************************************************************************/
/** \brief Deadzone
 **
 **  \par Description
 **       This function gets the deadzone of a float value.
 **
 **  \par Assumptions, External Events, and Notes:
 **       None
 **
 **  \param [in]       value          Value of #float to get deadzone of.
 **  \param [in]  	   dz   		  Desired deadzone for this value.
 **
 **
 **  \returns
 **  The deadzone for this value
 **  \endreturns
 *************************************************************************/
const float deadzonef(const float &value, const float &dz);

/************************************************************************/
/** \brief Exponential TODO
 **
 **  \par Description
 **       This function
 **
 **  \par Assumptions, External Events, and Notes:
 **       None
 **
 **  \param [in]       value      Value of #float to
 **  \param [in]  	   e   		  Exponential?
 **
 **
 **  \returns
 **
 **  \endreturns
 *************************************************************************/
const float expof_deadzone(const float &value, const float &e, const float &dz);

}

#endif
