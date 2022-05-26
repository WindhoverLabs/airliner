/**
 * @file mfa_tbldefs.h
 * @author Shahed Rahim (srahim@windhoverlabs.com)
 * @brief This function defines the config table to configure and calibrate the MFA app
 * @version 1.0.0
 * @date 2022-05-17
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef MFA_TBLDEFS_H
#define MFA_TBLDEFS_H

#include "cfe.h"

/**
 * @brief Number of layers in the atmosphere based on how air density behaves
 * 
 */
#define MFA_TBLDEFS_NUM_ATM_LAYERS			(7)

/**
 * @brief This structure defines the calibration config table for the ASPD-4525 using the MFA app
 * 
 */
typedef struct {
	double pressureMinimum_PSI;								/* PSI */
	double pressureMaximum_PSI;								/* PSI */
	double temperatureMinimum_Celcius;						/* degrees Celsius */
	double temperatureMaximum_Celcius;						/* degrees Celsius */
	double airGasConstantR_SI;								/* J/(mol.K) */
	double gravitationalAccereleration_SI;					/* m/s^2 */
	double airMolarMass_SI;									/* kg/mol */
	double altitudeMeters_bs[MFA_TBLDEFS_NUM_ATM_LAYERS];	/* altitude minimum limits of different layers of atmosphere */
	double rho_bs[MFA_TBLDEFS_NUM_ATM_LAYERS];				/* air mass density limits of different layers of atmosphere */
	double Temp_bs[MFA_TBLDEFS_NUM_ATM_LAYERS];				/* standard temperature limits of different layers of atmosphere */
	double LapseRate_bs[MFA_TBLDEFS_NUM_ATM_LAYERS];		/* Temperature Lapse Rate of different layers of atmosphere */
	uint32 equationNo_bs[MFA_TBLDEFS_NUM_ATM_LAYERS];		/* Equation used in different layers of atmosphere */
	uint32 airDensityCalculationMode;						/* Modes described in mfa_config.h */
} MFA_ConfigTbl_t;

#endif /*MFA_TBLDEFS_H*/