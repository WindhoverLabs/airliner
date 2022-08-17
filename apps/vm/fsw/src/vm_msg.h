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

#ifndef VM_MSG_H
#define VM_MSG_H

#include "vm_tbldefs.h"

/************************************************************************
 ** Pragmas
 *************************************************************************/

/************************************************************************
 ** Includes
 *************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

#include "cfe.h"

/************************************************************************
 ** Local Defines
 *************************************************************************/

/************************************************************************
 ** VM Command Codes
 *************************************************************************/

/** \vmcmd Noop
 **
 **  \par Description
 **       Implements the Noop command that demonstrates the VM task is alive
 **
 **  \vmcmdmnemonic \VM_NOOP
 **
 **  \par Command Structure
 **       #VM_NoArgCmd_t
 **
 **  \par Command Verification
 **       Successful execution of this command may be verified with
 **       the following telemetry:
 **       - \b \c \VM_CMDACPTCNT - command counter will increment
 **       - The #VM_CMD_INF_EID informational event message will be
 **         generated when the command is received
 **
 **  \par Error Conditions
 **       This command may fail for the following reason(s):
 **       - Command packet length not as expected
 **
 **  \par Evidence of failure may be found in the following telemetry:
 **       - \b \c \VM_CMDRJCTCNT - command error counter will increment
 **       - Error specific event message #VM_MSGID_ERR_EID
 **
 **  \par Criticality
 **       None
 **
 **  \sa   #VM_RESET_CC
 */
#define VM_NOOP_CC                 (0)

/** \vmcmd Reset Counters
 **
 **  \par Description
 **       Resets the VM housekeeping counters
 **
 **  \vmcmdmnemonic \VM_TLMRST
 **
 **  \par Command Structure
 **       #VM_NoArgCmd_t
 **
 **  \par Command Verification
 **       Successful execution of this command may be verified with
 **       the following telemetry:
 **       - \b \c \VM_CMDACTPCNT       - command counter will be cleared
 **       - \b \c \VM_CMDRJCTCNT       - command error counter will be cleared
 **       - The #VM_CMD_INF_EID debug event message will be
 **         generated when the command is executed
 **
 **  \par Error Conditions
 **       This command may fail for the following reason(s):
 **       - Command packet length not as expected
 **
 **  \par Evidence of failure may be found in the following telemetry:
 **       - \b \c \VM_CMDRJCTCNT - command error counter will increment
 **       - Error specific event message #VM_MSGID_ERR_EID
 **
 **  \par Criticality
 **       None
 **
 **  \sa   #VM_NOOP_CC
 */
#define VM_RESET_CC                (1)

/** \vmcmd Arm
 **
 **  \par Description
 **       Arms the vehicle
 **
 **  \vmcmdmnemonic \VM_ARM
 **
 **  \par Command Structure
 **       #VM_NoArgCmd_t
 **
 **  \par Command Verification
 **       Successful execution of this command may be verified with
 **       the following telemetry:
 **       - \b \c \VM_ARMSTATE - arming state will say armed
 **       - \b \c \VM_CMDACPTCNT - command counter will increment
 **       - The #VM_CMD_INF_EID informational event message will be
 **         generated when the command is received
 **
 **  \par Error Conditions
 **       This command may fail for the following reason(s):
 **       - Command packet length not as expected
 **
 **  \par Evidence of failure may be found in the following telemetry:
 **       - \b \c \VM_CMDRJCTCNT - command error counter will increment
 **       - Error specific event message #VM_MSGID_ERR_EID
 **
 **  \par Criticality
 **       Effects Vehicle Operation
 **
 **  \sa   #VM_VEHICLE_DISARM_CC
 */
#define VM_VEHICLE_ARM_CC          (2)

/** \vmcmd Disarm
 **
 **  \par Description
 **       Disarms the vehicle
 **
 **  \vmcmdmnemonic \VM_DISARM
 **
 **  \par Command Structure
 **       #VM_NoArgCmd_t
 **
 **  \par Command Verification
 **       Successful execution of this command may be verified with
 **       the following telemetry:
 **       - \b \c \VM_ARMSTATE - arming state will say standby
 **       - \b \c \VM_CMDACPTCNT - command counter will increment
 **       - The #VM_CMD_INF_EID informational event message will be
 **         generated when the command is received
 **
 **  \par Error Conditions
 **       This command may fail for the following reason(s):
 **       - Command packet length not as expected
 **
 **  \par Evidence of failure may be found in the following telemetry:
 **       - \b \c \VM_CMDRJCTCNT - command error counter will increment
 **       - Error specific event message #VM_MSGID_ERR_EID
 **
 **  \par Criticality
 **       Effects Vehicle Operation
 **
 **  \sa   #VM_VEHICLE_ARM_CC
 */
#define VM_VEHICLE_DISARM_CC       (3)

/** \vmcmd Manual
 **
 **  \par Description
 **       Vehicle navigation mode is switched to manual
 **
 **  \vmcmdmnemonic \VM_MANUAL
 **
 **  \par Command Structure
 **       #VM_NoArgCmd_t
 **
 **  \par Command Verification
 **       Successful execution of this command may be verified with
 **       the following telemetry:
 **       - \b \c \VM_NAVSTATE - arming state will say manual
 **       - \b \c \VM_CMDACPTCNT - command counter will increment
 **       - The #VM_CMD_INF_EID informational event message will be
 **         generated when the command is received
 **
 **  \par Error Conditions
 **       This command may fail for the following reason(s):
 **       - Command packet length not as expected
 **
 **  \par Evidence of failure may be found in the following telemetry:
 **       - \b \c \VM_CMDRJCTCNT - command error counter will increment
 **       - Error specific event message #VM_MSGID_ERR_EID
 **
 **  \par Criticality
 **       Effects Vehicle Operation
 **
 **  \sa   #VM_SET_NAV_ALTCTL_CC               \n
 **        #VM_SET_NAV_POSCTL_CC               \n
 **        #VM_SET_NAV_AUTO_LOITER_CC          \n
 **        #VM_SET_NAV_AUTO_RTL_CC             \n
 **        #VM_SET_NAV_ACRO_CC                 \n
 **        #VM_SET_NAV_STABILIZE_CC            \n
 **        #VM_SET_NAV_RATTITUDE_CC            \n
 **        #VM_SET_NAV_AUTO_TAKEOFF_CC         \n
 **        #VM_SET_NAV_AUTO_LAND_CC            \n
 */
#define VM_SET_NAV_MANUAL_CC               (40)

/** \vmcmd Altitude Control
 **
 **  \par Description
 **       Vehicle navigation mode is switched to altitude control
 **
 **  \vmcmdmnemonic \VM_ALTCTL
 **
 **  \par Command Structure
 **       #VM_NoArgCmd_t
 **
 **  \par Command Verification
 **       Successful execution of this command may be verified with
 **       the following telemetry:
 **       - \b \c \VM_NAVSTATE - arming state will say altitude control
 **       - \b \c \VM_CMDACPTCNT - command counter will increment
 **       - The #VM_CMD_INF_EID informational event message will be
 **         generated when the command is received
 **
 **  \par Error Conditions
 **       This command may fail for the following reason(s):
 **       - Command packet length not as expected
 **
 **  \par Evidence of failure may be found in the following telemetry:
 **       - \b \c \VM_CMDRJCTCNT - command error counter will increment
 **       - Error specific event message #VM_MSGID_ERR_EID
 **
 **  \par Criticality
 **       Effects Vehicle Operation
 **
 **  \sa   #VM_SET_NAV_MANUAL_CC               \n
 **        #VM_SET_NAV_POSCTL_CC               \n
 **        #VM_SET_NAV_AUTO_LOITER_CC          \n
 **        #VM_SET_NAV_AUTO_RTL_CC             \n
 **        #VM_SET_NAV_ACRO_CC                 \n
 **        #VM_SET_NAV_STABILIZE_CC            \n
 **        #VM_SET_NAV_RATTITUDE_CC            \n
 **        #VM_SET_NAV_AUTO_TAKEOFF_CC         \n
 **        #VM_SET_NAV_AUTO_LAND_CC            \n
 */
#define VM_SET_NAV_ALTCTL_CC               (41)

/** \vmcmd Position Control
 **
 **  \par Description
 **       Vehicle navigation mode is switched to position control
 **
 **  \vmcmdmnemonic \VM_POSCTL
 **
 **  \par Command Structure
 **       #VM_NoArgCmd_t
 **
 **  \par Command Verification
 **       Successful execution of this command may be verified with
 **       the following telemetry:
 **       - \b \c \VM_NAVSTATE - arming state will say position control
 **       - \b \c \VM_CMDACPTCNT - command counter will increment
 **       - The #VM_CMD_INF_EID informational event message will be
 **         generated when the command is received
 **
 **  \par Error Conditions
 **       This command may fail for the following reason(s):
 **       - Command packet length not as expected
 **
 **  \par Evidence of failure may be found in the following telemetry:
 **       - \b \c \VM_CMDRJCTCNT - command error counter will increment
 **       - Error specific event message #VM_MSGID_ERR_EID
 **
 **  \par Criticality
 **       Effects Vehicle Operation
 **
 **  \sa   #VM_SET_NAV_MANUAL_CC               \n
 **        #VM_SET_NAV_ALTCTL_CC               \n
 **        #VM_SET_NAV_AUTO_LOITER_CC          \n
 **        #VM_SET_NAV_AUTO_RTL_CC             \n
 **        #VM_SET_NAV_ACRO_CC                 \n
 **        #VM_SET_NAV_STABILIZE_CC            \n
 **        #VM_SET_NAV_RATTITUDE_CC            \n
 **        #VM_SET_NAV_AUTO_TAKEOFF_CC         \n
 **        #VM_SET_NAV_AUTO_LAND_CC            \n
 */
#define VM_SET_NAV_POSCTL_CC               (42)

/** \vmcmd Auto Loiter
 **
 **  \par Description
 **       Vehicle navigation mode is switched to auto loiter
 **
 **  \vmcmdmnemonic \VM_AUTO_LOITER
 **
 **  \par Command Structure
 **       #VM_NoArgCmd_t
 **
 **  \par Command Verification
 **       Successful execution of this command may be verified with
 **       the following telemetry:
 **       - \b \c \VM_NAVSTATE - arming state will say auto loiter
 **       - \b \c \VM_CMDACPTCNT - command counter will increment
 **       - The #VM_CMD_INF_EID informational event message will be
 **         generated when the command is received
 **
 **  \par Error Conditions
 **       This command may fail for the following reason(s):
 **       - Command packet length not as expected
 **
 **  \par Evidence of failure may be found in the following telemetry:
 **       - \b \c \VM_CMDRJCTCNT - command error counter will increment
 **       - Error specific event message #VM_MSGID_ERR_EID
 **
 **  \par Criticality
 **       Effects Vehicle Operation
 **
 **  \sa   #VM_SET_NAV_MANUAL_CC               \n
 **        #VM_SET_NAV_ALTCTL_CC               \n
 **        #VM_SET_NAV_POSCTL_CC               \n
 **        #VM_SET_NAV_AUTO_RTL_CC             \n
 **        #VM_SET_NAV_ACRO_CC                 \n
 **        #VM_SET_NAV_STABILIZE_CC            \n
 **        #VM_SET_NAV_RATTITUDE_CC            \n
 **        #VM_SET_NAV_AUTO_TAKEOFF_CC         \n
 **        #VM_SET_NAV_AUTO_LAND_CC            \n
 */
#define VM_SET_NAV_AUTO_LOITER_CC          (44)

/** \vmcmd Auto RTL
 **
 **  \par Description
 **       Vehicle navigation mode is switched to auto rtl
 **
 **  \vmcmdmnemonic \VM_AUTO_RTL
 **
 **  \par Command Structure
 **       #VM_NoArgCmd_t
 **
 **  \par Command Verification
 **       Successful execution of this command may be verified with
 **       the following telemetry:
 **       - \b \c \VM_NAVSTATE - arming state will say auto rtl
 **       - \b \c \VM_CMDACPTCNT - command counter will increment
 **       - The #VM_CMD_INF_EID informational event message will be
 **         generated when the command is received
 **
 **  \par Error Conditions
 **       This command may fail for the following reason(s):
 **       - Command packet length not as expected
 **
 **  \par Evidence of failure may be found in the following telemetry:
 **       - \b \c \VM_CMDRJCTCNT - command error counter will increment
 **       - Error specific event message #VM_MSGID_ERR_EID
 **
 **  \par Criticality
 **       Effects Vehicle Operation
 **
 **  \sa   #VM_SET_NAV_MANUAL_CC               \n
 **        #VM_SET_NAV_ALTCTL_CC               \n
 **        #VM_SET_NAV_POSCTL_CC               \n
 **        #VM_SET_NAV_AUTO_LOITER_CC          \n
 **        #VM_SET_NAV_ACRO_CC                 \n
 **        #VM_SET_NAV_STABILIZE_CC            \n
 **        #VM_SET_NAV_RATTITUDE_CC            \n
 **        #VM_SET_NAV_AUTO_TAKEOFF_CC         \n
 **        #VM_SET_NAV_AUTO_LAND_CC            \n
 */
#define VM_SET_NAV_AUTO_RTL_CC             (45)

/** \vmcmd Acrobatic
 **
 **  \par Description
 **       Vehicle navigation mode is switched to acrobatic
 **
 **  \vmcmdmnemonic \VM_ACRO
 **
 **  \par Command Structure
 **       #VM_NoArgCmd_t
 **
 **  \par Command Verification
 **       Successful execution of this command may be verified with
 **       the following telemetry:
 **       - \b \c \VM_NAVSTATE - arming state will say acrobatic
 **       - \b \c \VM_CMDACPTCNT - command counter will increment
 **       - The #VM_CMD_INF_EID informational event message will be
 **         generated when the command is received
 **
 **  \par Error Conditions
 **       This command may fail for the following reason(s):
 **       - Command packet length not as expected
 **
 **  \par Evidence of failure may be found in the following telemetry:
 **       - \b \c \VM_CMDRJCTCNT - command error counter will increment
 **       - Error specific event message #VM_MSGID_ERR_EID
 **
 **  \par Criticality
 **       Effects Vehicle Operation
 **
 **  \sa   #VM_SET_NAV_MANUAL_CC               \n
 **        #VM_SET_NAV_ALTCTL_CC               \n
 **        #VM_SET_NAV_POSCTL_CC               \n
 **        #VM_SET_NAV_AUTO_LOITER_CC          \n
 **        #VM_SET_NAV_AUTO_RTL_CC             \n
 **        #VM_SET_NAV_STABILIZE_CC            \n
 **        #VM_SET_NAV_RATTITUDE_CC            \n
 **        #VM_SET_NAV_AUTO_TAKEOFF_CC         \n
 **        #VM_SET_NAV_AUTO_LAND_CC            \n
 */
#define VM_SET_NAV_ACRO_CC                 (50)

/** \vmcmd Stabilize
 **
 **  \par Description
 **       Vehicle navigation mode is switched to stabilize
 **
 **  \vmcmdmnemonic \VM_STABILIZE
 **
 **  \par Command Structure
 **       #VM_NoArgCmd_t
 **
 **  \par Command Verification
 **       Successful execution of this command may be verified with
 **       the following telemetry:
 **       - \b \c \VM_NAVSTATE - arming state will say stabilize
 **       - \b \c \VM_CMDACPTCNT - command counter will increment
 **       - The #VM_CMD_INF_EID informational event message will be
 **         generated when the command is received
 **
 **  \par Error Conditions
 **       This command may fail for the following reason(s):
 **       - Command packet length not as expected
 **
 **  \par Evidence of failure may be found in the following telemetry:
 **       - \b \c \VM_CMDRJCTCNT - command error counter will increment
 **       - Error specific event message #VM_MSGID_ERR_EID
 **
 **  \par Criticality
 **       Effects Vehicle Operation
 **
 **  \sa   #VM_SET_NAV_MANUAL_CC               \n
 **        #VM_SET_NAV_ALTCTL_CC               \n
 **        #VM_SET_NAV_POSCTL_CC               \n
 **        #VM_SET_NAV_AUTO_LOITER_CC          \n
 **        #VM_SET_NAV_AUTO_RTL_CC             \n
 **        #VM_SET_NAV_ACRO_CC                 \n
 **        #VM_SET_NAV_RATTITUDE_CC            \n
 **        #VM_SET_NAV_AUTO_TAKEOFF_CC         \n
 **        #VM_SET_NAV_AUTO_LAND_CC            \n
 */
#define VM_SET_NAV_STABILIZE_CC            (54)

/** \vmcmd Rattitude
 **
 **  \par Description
 **       Vehicle navigation mode is switched to rattitude
 **
 **  \vmcmdmnemonic \VM_RATTITUDE
 **
 **  \par Command Structure
 **       #VM_NoArgCmd_t
 **
 **  \par Command Verification
 **       Successful execution of this command may be verified with
 **       the following telemetry:
 **       - \b \c \VM_NAVSTATE - arming state will say rattitude
 **       - \b \c \VM_CMDACPTCNT - command counter will increment
 **       - The #VM_CMD_INF_EID informational event message will be
 **         generated when the command is received
 **
 **  \par Error Conditions
 **       This command may fail for the following reason(s):
 **       - Command packet length not as expected
 **
 **  \par Evidence of failure may be found in the following telemetry:
 **       - \b \c \VM_CMDRJCTCNT - command error counter will increment
 **       - Error specific event message #VM_MSGID_ERR_EID
 **
 **  \par Criticality
 **       Effects Vehicle Operation
 **
 **  \sa   #VM_SET_NAV_MANUAL_CC               \n
 **        #VM_SET_NAV_ALTCTL_CC               \n
 **        #VM_SET_NAV_POSCTL_CC               \n
 **        #VM_SET_NAV_AUTO_LOITER_CC          \n
 **        #VM_SET_NAV_AUTO_RTL_CC             \n
 **        #VM_SET_NAV_ACRO_CC                 \n
 **        #VM_SET_NAV_STABILIZE_CC            \n
 **        #VM_SET_NAV_AUTO_TAKEOFF_CC         \n
 **        #VM_SET_NAV_AUTO_LAND_CC            \n
 */
#define VM_SET_NAV_RATTITUDE_CC            (55)

/** \vmcmd Auto Takeoff
 **
 **  \par Description
 **       Vehicle navigation mode is switched to auto takeoff
 **
 **  \vmcmdmnemonic \VM_AUTO_TAKEOFF
 **
 **  \par Command Structure
 **       #VM_NoArgCmd_t
 **
 **  \par Command Verification
 **       Successful execution of this command may be verified with
 **       the following telemetry:
 **       - \b \c \VM_NAVSTATE - arming state will say auto takeoff
 **       - \b \c \VM_CMDACPTCNT - command counter will increment
 **       - The #VM_CMD_INF_EID informational event message will be
 **         generated when the command is received
 **
 **  \par Error Conditions
 **       This command may fail for the following reason(s):
 **       - Command packet length not as expected
 **
 **  \par Evidence of failure may be found in the following telemetry:
 **       - \b \c \VM_CMDRJCTCNT - command error counter will increment
 **       - Error specific event message #VM_MSGID_ERR_EID
 **
 **  \par Criticality
 **       Effects Vehicle Operation
 **
 **  \sa   #VM_SET_NAV_MANUAL_CC               \n
 **        #VM_SET_NAV_ALTCTL_CC               \n
 **        #VM_SET_NAV_POSCTL_CC               \n
 **        #VM_SET_NAV_AUTO_LOITER_CC          \n
 **        #VM_SET_NAV_AUTO_RTL_CC             \n
 **        #VM_SET_NAV_ACRO_CC                 \n
 **        #VM_SET_NAV_STABILIZE_CC            \n
 **        #VM_SET_NAV_RATTITUDE_CC            \n
 **        #VM_SET_NAV_AUTO_LAND_CC            \n
 */
#define VM_SET_NAV_AUTO_TAKEOFF_CC         (56)

/** \vmcmd Auto Land
 **
 **  \par Description
 **
 **  \par Command Verification
 **       Successful execution of this command may be verified with
 **       the following telemetry:
 **       - \b \c \VM_NAVSTATE - arming state will say auto land
 **       - \b \c \VM_CMDACPTCNT - command counter will increment
 **       - The #VM_CMD_INF_EID informational event message will be
 **         generated when the command is received
 **
 **  \par Error Conditions
 **       This command may fail for the following reason(s):
 **       - Command packet length not as expected
 **
 **  \par Evidence of failure may be found in the following telemetry:
 **       - \b \c \VM_CMDRJCTCNT - command error counter will increment
 **       - Error specific event message #VM_MSGID_ERR_EID
 **
 **  \par Criticality
 **       Effects Vehicle Operation
 **
 **  \sa   #VM_SET_NAV_MANUAL_CC               \n
 **        #VM_SET_NAV_ALTCTL_CC               \n
 **        #VM_SET_NAV_POSCTL_CC               \n
 **        #VM_SET_NAV_AUTO_LOITER_CC          \n
 **        #VM_SET_NAV_AUTO_RTL_CC             \n
 **        #VM_SET_NAV_ACRO_CC                 \n
 **        #VM_SET_NAV_STABILIZE_CC            \n
 **        #VM_SET_NAV_RATTITUDE_CC            \n
 **        #VM_SET_NAV_AUTO_TAKEOFF_CC         \n
 */
#define VM_SET_NAV_AUTO_LAND_CC            (57)

/** \vmcmd Send Configuration
 **
 **  \par Description
 **
 **  \par Command Verification
 **       Successful execution of this command may be verified with
 **       the following telemetry:
 **       - \b \c \VM_CMDACPTCNT - command counter will increment
 **       - The #VM_CMD_INF_EID informational event message will be
 **         generated when the command is received
 **
 **  \par Error Conditions
 **       This command may fail for the following reason(s):
 **       - Command packet length not as expected
 **
 **  \par Evidence of failure may be found in the following telemetry:
 **       - \b \c \VM_CMDRJCTCNT - command error counter will increment
 **       - Error specific event message #VM_MSGID_ERR_EID
 **
 **  \par Criticality
 **       Effects Vehicle Operation
 **
 **  \sa   #VM_BatteryFailsafeMode_t           \n
 */
#define VM_SEND_CONFIGURATION_CC    (58)

/************************************************************************
 ** Local Structure Declarations
 *************************************************************************/


/**
 * \brief Vehicle manager status flags
 */
typedef struct
{
    /** \brief Indicates if all sensors are initialized */
    osalbool SensorsInitialized;

    /** \brief System in rtl state */
    osalbool ReturnToHomeSet;

    /** \brief Indicates a valid home position (a valid home position is not always a valid launch) */
    osalbool HomePositionValid;

    /** \brief Satus of the USB power supply */
    osalbool UsbPowerConnected;

    /** \brief True if RC signal found atleast once */
    osalbool RcSignalFoundOnce;

    /** \brief True if RC lost mode is commanded */
    osalbool RcSignalLostModeIsCmded;

    /** \brief Set if RC input should be ignored temporarily */
    osalbool RcInputIsTemporarilyBlocked;

} VM_StatusFlags;


/**
 * \brief RC navigation mode switched
 */
typedef struct {

    /** \brief Position control is selected */
    osalbool inPosCtl;
    /** \brief Return to launch is selected  */
    osalbool inRtl;
    /** \brief Auto loiter is selected  */
    osalbool inLoiter;
    /** \brief Manual is selected  */
    osalbool inManual;
    /** \brief Takeoff is selected  */
    osalbool inTakeoff;
    /** \brief Altitude control is selected  */
    osalbool inAltCtl;

} VM_Modes;


/**
 **  \brief Battery failsafe modes.
 **  These define how VM reacts to battery low or failure.
 */
typedef enum
{
    /** \brief Publish a warning event. */
    VM_BATTERY_FAILSAFE_MODE_WARNING                               = 0,
    /** \brief Return to Launch. */
    VM_BATTERY_FAILSAFE_MODE_RETURN                                = 1,
    /** \brief Land immediately. */
    VM_BATTERY_FAILSAFE_MODE_LAND                                  = 2,
    /** \brief Return to Launch if low, but land immediately if even lower. */
    VM_BATTERY_FAILSAFE_MODE_RETURN_IF_CRIT_LOW_LAND_IF_DANGER_LOW = 3,
} VM_BatteryFailsafeMode_t;


/**
 **  \brief No Arguments Command
 **  For command details see #VM_NOOP_CC, #VM_RESET_CC
 **  Also see #VM_SEND_HK_MID
 */
typedef struct
{
    uint8 ucCmdHeader[CFE_SB_CMD_HDR_SIZE];
} VM_NoArgCmd_t;

/**
 **  \brief VM Configuration
 **  For command details see #VM_SEND_CONFIGURATION_CC
 */
typedef struct
{
    uint8                    ucHeader[CFE_SB_TLM_HDR_SIZE];
    VM_ConfigTbl_t           ConfigTbl;
} VM_ConfigTlm_t;

/** \vmtlm Housekeeping data
 **
 **  \par Description
 **       VM application housekeeping data
 **
 **  \par Criticality
 **       None
 */
typedef struct
{
    /** \brief cFE SB Tlm Msg Hdr */
    uint8 TlmHeader[CFE_SB_TLM_HDR_SIZE];

    /** \vmtlmmnemonic \VM_CMDACPTCNT
     \brief Count of accepted commands.  Cleared by Reset Command. */
    uint8 usCmdCnt;

    /** \vmtlmmnemonic \VM_CMDRJCTCNT
     \brief Count of failed commands.  Cleared by Reset Command.  */
    uint8 usCmdErrCnt;

    /** \vmtlmmnemonic \VM_ARMSTATE
     \brief Current arm state */
    uint32 ArmingState;

    /** \vmtlmmnemonic \VM_NAVSTATE
     \brief Current navigation state */
    uint32 NavState;

    /** \brief Number of times the main loop has woken up. */
    uint32 WakeupCount;

    /** \brief Number of times the SensorMag message has been received. */
    uint32 SensorMagMsgCount;

    /** \brief Number of times the Sensor Gyro message has been received. */
    uint32 SensorGyroMsgCount;

    /** \brief Number of times the Battery Status message has been received. */
    uint32 BatteryStatusMsgCount;

    /** \brief Number of times the Telemetry Status message has been received. */
    uint32 TelemetryStatusMsgCount;

    /** \brief Number of times the Subsystem Info message has been received. */
    uint32 SubsystemInfoMsgCount;

    /** \brief Number of times the Vehicle Attitude message has been received. */
    uint32 VehicleAttitudeMsgCount;

    /** \brief Number of times the Vehicle Local Position message has been received. */
    uint32 VehicleLocalPositionMsgCount;

    /** \brief Number of times the Vehicle Land Detected message has been received. */
    uint32 VehicleLandDetectedMsgCount;

    /** \brief Number of times the Mission Result message has been received. */
    uint32 MissionResultMsgCount;

    /** \brief Number of times the Manual Control Setpoint message has been received. */
    uint32 ManualControlSetpointMsgCount;

    /** \brief Number of times the Position Setpoint Triplet message has been received. */
    uint32 PositionSetpointTripletMsgCount;

    /** \brief Number of times the Sensor Accel message has been received. */
    uint32 SensorAccelMsgCount;

    /** \brief Number of times the Safety message has been received. */
    uint32 SafetyMsgCount;

    /** \brief Number of times the Sensor Correction message has been received. */
    uint32 SensorCorrectionMsgCount;

    /** \brief Number of times the Sensor Combined message has been received. */
    uint32 SensorCombinedMsgCount;

    /** \brief Number of times the Vehicle Command message has been received. */
    uint32 VehicleCommandMsgCount;

    /** \brief Number of times the Vehicle Global Position message has been received. */
    uint32 VehicleGlobalPositionMsgCount;

    /** \brief Number of times the Vehicle GPS Position message has been received. */
    uint32 VehicleGpsPositionMsgCount;

    /** \brief Number of times the Vehicle Control Mode message has been received. */
    uint32 VehicleControlModeMsgCount;

    /** \brief True if home position is not set and local variables are not initialization */
    osalbool NotInitialized;

    /** \brief Timestamps vm at boot */
    uint64 BootTimestamp;

    /** \brief status flag variable */
    VM_StatusFlags StatusFlags;

    /** \brief True if local position is valid */
    osalbool LocalPositionIsValid;

    /** \brief True if previously landed */
    osalbool PrevLanded;

    /** \brief True if previously in flight */
    osalbool PrevInFlight;

    /** \brief Records a count when vehicle is disarmed with stick  */
    uint32 StickOffCounter;

    /** \brief Records a count when vehicle is armed with stick */
    uint32 StickOnCounter;

    /** \brief Arming switch in manual control setpoint message  */
    uint32 LastSpManArmSwitch;

    /** \brief True when vehicle's battery is low and a contingency action is implemented */
    osalbool LowBatteryVoltageActionsDone;

    /** \brief True when vehicle's battery is critical and a contingency action is implemented */
    osalbool CriticalBatteryVoltageActionsDone;

    /** \brief True when vehicle's battery is dangerously low and a contingency action is implemented */
    osalbool EmergencyBatteryVoltageActionsDone;

    /** \brief Timestamps the moment rc signal is lost */
    uint64 RCSignalLostTimestamp;

    /** \brief True when arming status changes with the vehicle */
    osalbool ArmingStateChanged;

    /** \brief An instance rc navigation mode  */
    VM_Modes PreviousModes;

} VM_HkTlm_t;

#ifdef __cplusplus
}
#endif

#endif /* VM_MSG_H */

/************************/
/*  End of File Comment */
/************************/
