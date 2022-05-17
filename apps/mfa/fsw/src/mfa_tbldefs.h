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

/**
 * @brief This structure defines the calibration config table for the ASPD-4525 using the MFA app
 * 
 */
typedef struct {
	double pressureMinimum_PSI;				/* PSI */
	double pressureMaximum_PSI;				/* PSI */
	double temperatureMinimum_Celcius;		/* degrees Celsius */
	double temperatureMaximum_Celcius;		/* degrees Celsius */
	double airGasConstantR_SI;				/* J/(mol.K) */
	double gravitationalAccereleration_SI;	/* m/s^2 */
	double airMolarMass_SI;					/* kg/mol */
} MFA_ConfigTbl_t;

#endif /*MFA_TBLDEFS_H*/