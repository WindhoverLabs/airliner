/****************************************************************************
 *
 *   Copyright (c) 2017 Windhover Labs, L.L.C. All rights reserved.
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
 * 3. Neither the name Windhover Labs nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
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
 *****************************************************************************/
    
#ifndef ASPD4525_TBLDEFS_H
#define ASPD4525_TBLDEFS_H

/************************************************************************
** Pragmas
*************************************************************************/

/************************************************************************
** Includes
*************************************************************************/
#include "cfe.h"

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************
** Local Defines
*************************************************************************/
/**
 * \brief Defines the number of entries in the table
 */
#define ASPD4525_CONFIG_TABLE_MAX_ENTRIES  (1)

/**
 * \brief Defines the table identification name used for table registration.
 */
#define ASPD4525_CONFIG_TABLENAME          ("CONFIG_TBL")

/**
 * \brief Defines the table file name used for table registration.
 */
#define ASPD4525_CDS_TABLENAME  ("aspd4525_CdsTbl")

/**
 * @brief Number of layers in the atmosphere based on how air density behaves
 * 
 */
#define ASPD4525_NUM_ATM_LAYERS			(7)
/************************************************************************
** Local Structure Declarations
*************************************************************************/

/** \brief Definition for a single config table entry */
typedef struct
{
	float fPressureMinimum_PSI;								/* PSI */
	float fPressureMaximum_PSI;								/* PSI */
	float fTemperatureMinimum_Celcius;						/* degrees Celsius */
	float fTemperatureMaximum_Celcius;						/* degrees Celsius */
	float fAirGasConstantR_SI;								/* J/(mol.K) */
	float fGravitationalAccereleration_SI;					/* m/s^2 */
	float fAirMolarMass_SI;									/* kg/mol */
	float fAltitudeMeters_bs[ASPD4525_NUM_ATM_LAYERS];	    /* altitude minimum limits of different layers of atmosphere */
	float fRho_bs[ASPD4525_NUM_ATM_LAYERS];				    /* air mass density limits of different layers of atmosphere */
	float fTemp_bs[ASPD4525_NUM_ATM_LAYERS];				/* standard temperature limits of different layers of atmosphere */
	float fLapseRate_bs[ASPD4525_NUM_ATM_LAYERS];		    /* Temperature Lapse Rate of different layers of atmosphere */
	uint32 uEquationNo_bs[ASPD4525_NUM_ATM_LAYERS];		    /* Equation used in different layers of atmosphere */
	uint32 uAirDensityCalculationMode;						/* Modes described in mfa_config.h */
} ASPD4525_ConfigTblEntry_t;


/** \brief Definition for Critical Data Storage (CDS) table entry */
typedef struct
{
    int32  iParam;

    /* TODO:  Add type declaration for CDS data here. */
} ASPD4525_CdsTbl_t;

/************************************************************************
** External Global Variables
*************************************************************************/

/************************************************************************
** Global Variables
*************************************************************************/

/************************************************************************
** Local Variables
*************************************************************************/

/************************************************************************
** Local Function Prototypes
*************************************************************************/


#ifdef __cplusplus
}
#endif

#endif /* ASPD4525_TBLDEFS_H */

/************************/
/*  End of File Comment */
/************************/
