#ifndef MFA_TBLDEFS_H
#define MFA_TBLDEFS_H

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