#ifndef BLOCK_DELAY_HPP
#define BLOCK_DELAY_HPP

#include "cfe.h"
#include <math/Vector10F.hpp>

namespace delay
{

class BlockDelay10FLEN10
{
private:

	/** \brief Size of vector */
    static const int LEN = 10;

    /** \brief Current index */
    uint8 m_Index;

    /** \brief Delay counter */
    int m_Delay;

    /** \brief Vector storing delay values */
    math::Vector10F m_Storage[LEN];
    

public:
	/************************************************************************/
	/** \brief Float Block Delay Constructor
	 **
	 **  \par Description
	 **       This function initializes the object.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 *************************************************************************/
    BlockDelay10FLEN10();

	/************************************************************************/
	/** \brief Float Block Delay Destructor
	 **
	 **  \par Description
	 **       This function destructs the object.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 *************************************************************************/
    ~BlockDelay10FLEN10();

	/************************************************************************/
	/** \brief Update Float Block Delay
	 **
	 **  \par Description
	 **       This function updates the object with the passed value.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   input      A #Vector10F with the new delay value
	 **
	 *************************************************************************/
    math::Vector10F Update(const math::Vector10F &input);

	/************************************************************************/
	/** \brief Initialize Float Block Delay
	 **
	 **  \par Description
	 **       This function initializes the object to have the default values.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 *************************************************************************/
    void Initialize(void);

	/************************************************************************/
	/** \brief Get Float Block Delay
	 **
	 **  \par Description
	 **       This function gets the delay at the passsed index.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   delay      A #uint8 delay index
	 **
	 *************************************************************************/
    math::Vector10F Get(uint8 delay);

protected:

};


class BlockDelayUINT64LEN10
{
private:

	/** \brief Size of vector */
    static const int LEN = 10;

    /** \brief Current index */
    uint8 m_Index;

    /** \brief Delay counter */
    int m_Delay;

    /** \brief Vector storing delay values */
    uint64 m_Storage[LEN];
    

public:

	/************************************************************************/
	/** \brief Uint Block Delay Constructor
	 **
	 **  \par Description
	 **       This function initializes the object.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 *************************************************************************/
    BlockDelayUINT64LEN10();

	/************************************************************************/
	/** \brief Uint Block Delay Destructor
	 **
	 **  \par Description
	 **       This function destructs the object.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 *************************************************************************/
    ~BlockDelayUINT64LEN10();

	/************************************************************************/
	/** \brief Uint Float Block Delay
	 **
	 **  \par Description
	 **       This function updates the object with the passed value.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   input      A #Vector10F with the new delay value
	 **
	 *************************************************************************/
    uint64 Update(const uint64 &input);

	/************************************************************************/
	/** \brief Uint Float Block Delay
	 **
	 **  \par Description
	 **       This function initializes the object to have the default values.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 *************************************************************************/
    void Initialize(void);

	/************************************************************************/
	/** \brief Get Uint Block Delay
	 **
	 **  \par Description
	 **       This function gets the delay at the passsed index.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   delay      A #uint8 delay index
	 **
	 *************************************************************************/
    uint64 Get(uint8 delay);


protected:

};

}

#endif
