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
    
#ifndef ASPD4525_MSG_H
#define ASPD4525_MSG_H

/************************************************************************
** Pragmas
*************************************************************************/

/************************************************************************
** Includes
*************************************************************************/
#include "cfe.h"
#include "aspd4525_config.h"

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************
** Local Defines
*************************************************************************/

/************************************************************************
** ASPD4525 Command Codes
*************************************************************************/

/** \aspd4525cmd Noop 
**  
**  \par Description
**       Implements the Noop command that demonstrates the ASPD4525 task is alive
**
**  \aspd4525cmdmnemonic \ASPD4525_NOOP
**
**  \par Command Structure
**       #ASPD4525_NoArgCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \ASPD4525_CMDACPTCNT - command counter will increment
**       - The #ASPD4525_CMD_INF_EID informational event message will be 
**         generated when the command is received
** 
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
** 
**  \par Evidence of failure may be found in the following telemetry: 
**       - \b \c \ASPD4525_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #ASPD4525_MSGID_ERR_EID
**
**  \par Criticality
**       None
**
**  \sa #ASPD4525_RESET_CC
*/
#define ASPD4525_NOOP_CC                 (0)

/** \aspd4525cmd Reset Counters
**  
**  \par Description
**       Resets the aspd4525 housekeeping counters
**
**  \aspd4525cmdmnemonic \ASPD4525_TLMRST
**
**  \par Command Structure
**       #ASPD4525_NoArgCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \ASPD4525_CMDACTPCNT       - command counter will be cleared
**       - \b \c \ASPD4525_CMDRJCTCNT       - command error counter will be cleared
**       - The #ASPD4525_CMD_INF_EID debug event message will be 
**         generated when the command is executed
** 
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
** 
**  \par Evidence of failure may be found in the following telemetry: 
**       - \b \c \ASPD4525_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #ASPD4525_MSGID_ERR_EID
**
**  \par Criticality
**       None
**
**  \sa #ASPD4525_NOOP_CC
*/
#define ASPD4525_RESET_CC                (1)
#define ASPD4525_MAN_CALIB_CC            (2)
#define ASPD4525_LAB_CALIB_CC            (3)

#define ASPD4525_TEMP_CALIB_CC           (5)
#define ASPD4525_PHYSICS_CALIB_CC        (6)
#define ASPD4525_AIR_COL_CALIB_CC        (7)

#define ASPD4525_SET_AIR_DENSITY_MODE_CC (8)

/************************************************************************
** Local Structure Declarations
*************************************************************************/

/** 
**  \brief No Arguments Command
**  For command details see #ASPD4525_NOOP_CC, #ASPD4525_RESET_CC
**  Also see #ASPD4525_SEND_HK_MID
*/
typedef struct
{
    uint8  ucCmdHeader[CFE_SB_CMD_HDR_SIZE];
} ASPD4525_NoArgCmd_t;

typedef struct {
    uint8 CmdHeader[CFE_SB_CMD_HDR_SIZE];
	float fPressureMinimum_PSI;								/* PSI */
	float fPressureMaximum_PSI;								/* PSI */
	float fTemperatureMinimum_Celcius;						/* degrees Celsius */
	float fTemperatureMaximum_Celcius;						/* degrees Celsius */
	float fAirGasConstantR_SI;								/* J/(mol.K) */
	float fGravitationalAccereleration_SI;					/* m/s^2 */
	float fAirMolarMass_SI;									/* kg/mol */
} ASPD4525_ManCalibArgCmd_t;

typedef struct {
    uint8 CmdHeader[CFE_SB_CMD_HDR_SIZE];
    uint32 uPCountLow;
	float fVelocityLow_SI;                                  /* m/s */
    uint32 uPCountHigh;
	float fVelocityHigh_SI;                                 /* m/s */
} ASPD4525_LabCalibArgCmd_t;

typedef struct {
    uint8 CmdHeader[CFE_SB_CMD_HDR_SIZE];
    uint32 uTCountLow;
	float fTemperatureLow_Celcius;                          /* degrees Celsius */
    uint32 uTCountHigh;
	float fTemperatureHigh_Celcius;                         /* degrees Celsius */
} ASPD4525_TempCalibArgCmd_t;

typedef struct {
    uint8 CmdHeader[CFE_SB_CMD_HDR_SIZE];
	float fAirGasConstantR_SI;								/* J/(mol.K) */
	float fGravitationalAccereleration_SI;					/* m/s^2 */
	float fAirMolarMass_SI;									/* kg/mol */
} ASPD4525_PhysicsCalibArgCmd_t;

typedef struct {
    uint8 CmdHeader[CFE_SB_CMD_HDR_SIZE];

	float fh_b0;								            /* m */
	float fh_b1;								            /* m */
	float fh_b2;								            /* m */
	float fh_b3;								            /* m */
	float fh_b4;								            /* m */
	float fh_b5;								            /* m */
	float fh_b6;								            /* m */

	float frho_b0;								            /* kg/m^3 */
	float frho_b1;								            /* kg/m^3 */
	float frho_b2;								            /* kg/m^3 */
	float frho_b3;								            /* kg/m^3 */
	float frho_b4;								            /* kg/m^3 */
	float frho_b5;								            /* kg/m^3 */
	float frho_b6;								            /* kg/m^3 */

	float fT_b0;								            /* K (kelvins) */
	float fT_b1;								            /* K (kelvins) */
	float fT_b2;								            /* K (kelvins) */
	float fT_b3;								            /* K (kelvins) */
	float fT_b4;								            /* K (kelvins) */
	float fT_b5;								            /* K (kelvins) */
	float fT_b6;								            /* K (kelvins) */

	float fL_b0;								            /* K/m */
	float fL_b1;								            /* K/m */
	float fL_b2;								            /* K/m */
	float fL_b3;								            /* K/m */
	float fL_b4;								            /* K/m */
	float fL_b5;								            /* K/m */
	float fL_b6;								            /* K/m */

} ASPD4525_AirColCalibArgCmd_t;


typedef struct {
    uint8 CmdHeader[CFE_SB_CMD_HDR_SIZE];
	ASPD4525_CONFIG_AirDensity_Modes_t uAirDensityCalculationMode;	/* Modes described in aspd4525_config.h */
} ASPD4525_AirDensityModeArgCmd_t;


/** 
**  \brief TODO Elaborate this struct
**  Boilerplate example of application-specific incoming data
*/
typedef struct
{
    uint8   TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint32  counter;

    /* TODO:  Add input data to this application here, such as raw data read from I/O
    **        devices.
    **        Option: for data that is already defined by another app, include
    **        that app's message header above.
    */

} ASPD4525_InData_t;

/** 
**  \brief TODO Elaborate this struct
**  Boilerplate example of application-specific outgoing data
*/
typedef struct
{
    uint8   ucTlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint32  uiCounter;
} ASPD4525_OutData_t;

/** 
**  \brief ASPD4525 application housekeeping data
*/
typedef struct
{
    /** \brief cFE SB Tlm Msg Hdr */
    uint8              TlmHeader[CFE_SB_TLM_HDR_SIZE];

    /** \aspd4525tlmmnemonic \ASPD4525_CMDACPTCNT
        \brief Count of accepted commands */
    uint8              usCmdCnt;   

    /** \aspd4525tlmmnemonic \ASPD4525_CMDRJCTCNT
        \brief Count of failed commands */
    uint8              usCmdErrCnt; 

    uint32 uPressureCount;
    uint32 uTemperatureCount;
    uint32 uStatus;
    float  fTemperature;
    float  fTrueAirSpeedUnfiltered;
    float  fIndicatedAirSpeed;
    float  fTrueAirSpeed;

	float fPressureMinimum_PSI;										/* PSI */
	float fPressureMaximum_PSI;										/* PSI */
	float fTemperatureMinimum_Celcius;								/* degrees Celsius */
	float fTemperatureMaximum_Celcius;								/* degrees Celsius */

	ASPD4525_CONFIG_AirDensity_Modes_t uAirDensityCalculationMode;	/* Modes described in aspd4525_config.h */
} ASPD4525_HkTlm_t;


#ifdef __cplusplus
}
#endif

#endif /* ASPD4525_MSG_H */

/************************/
/*  End of File Comment */
/************************/
