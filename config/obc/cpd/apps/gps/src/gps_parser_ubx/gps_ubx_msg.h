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

#ifndef _GPS_UBX_MSG_H_
#define _GPS_UBX_MSG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "common_types.h"
#include "px4_msgs.h"

#define GPS_COEF_SIZE                      (8)

/* RX NAV-PVT message content details */
/*   Bitfield "valid" masks */
/**< validDate (Valid UTC Date) */
#define GPS_NAV_PVT_VALID_VALIDDATE        (0x01)
/**< validTime (Valid UTC Time) */
#define GPS_NAV_PVT_VALID_VALIDTIME        (0x02)
/**< fullyResolved (1 = UTC Time of Day has been fully resolved (no seconds uncertainty)) */
#define GPS_NAV_PVT_VALID_FULLYRESOLVED    (0x04)

/*   Bitfield "flags" masks */
/**< gnssFixOK (A valid fix (i.e within DOP & accuracy masks)) */
#define GPS_NAV_PVT_FLAGS_GNSSFIXOK        (0x01)
/**< diffSoln (1 if differential corrections were applied) */
#define GPS_NAV_PVT_FLAGS_DIFFSOLN         (0x02)
/**< psmState (Power Save Mode state (see Power Management)) */
#define GPS_NAV_PVT_FLAGS_PSMSTATE         (0x1C)
/**< headVehValid (Heading of vehicle is valid) */
#define GPS_NAV_PVT_FLAGS_HEADVEHVALID     (0x20)
/**< Carrier phase range solution (RTK mode) */
#define GPS_NAV_PVT_FLAGS_CARRSOLN         (0xC0)

/* RX NAV-TIMEUTC message content details */
/*   Bitfield "valid" masks */
/**< validTOW (1 = Valid Time of Week) */
#define GPS_NAV_TIMEUTC_VALID_VALIDTOW      (0x01)
/**< validWKN (1 = Valid Week Number) */
#define GPS_NAV_TIMEUTC_VALID_VALIDKWN      (0x02)
/**< validUTC (1 = Valid UTC Time) */
#define GPS_NAV_TIMEUTC_VALID_VALIDUTC      (0x04)
/**< utcStandard (0..15 = UTC standard identifier) */
#define GPS_NAV_TIMEUTC_VALID_UTCSTANDARD   (0xF0)

#define GPS_MON_HW_VP_PIN_COUNT             (17)

typedef struct
{
    uint32 iTOW;
    uint8  aopCfg;
    uint8  status;
} GPS_NAV_AOPSTATUS_t;


typedef struct
{
    uint32 iTOW;
    uint8  version;
    int32  roll;
    int32  pitch;
    int32  heading;
    uint32 accRoll;
    uint32 accPitch;
    uint32 accHeading;
} GPS_NAV_ATT_t;


typedef struct
{
    uint32 iTOW;
    int32  clkB;
    int32  clkD;
    uint32 tAcc;
    uint32 fAcc;
} GPS_NAV_CLOCK_t;


typedef struct
{
    uint32 iTOW;
    int32  age;
    int16  baseId;
    int16  baseHealth;
    uint8  numCh;
    uint8  status;
} GPS_NAV_DGPS_t;


typedef struct
{
    uint32 iTOW;
    uint16 gDOP;
    uint16 pDOP;
    uint16 tDOP;
    uint16 vDOP;
    uint16 hDOP;
    uint16 nDOP;
    uint16 eDOP;
} GPS_NAV_DOP_t;


typedef struct
{
    uint32 iTOW;
} GPS_NAV_EOE_t;


typedef struct
{
    uint32 iTOW;
    uint8  version;
    uint8  status;
    uint8  numFences;
    uint8  combState;
} GPS_NAV_GEOFENCE_t;


typedef struct
{
    uint8  version;
    uint32 iTOW;
    uint32 distance;
    uint32 totalDistance;
    uint32 distanceStd;
} GPS_NAV_ODO_t;


typedef struct
{
    uint32 iTOW;
    uint8  version;
    uint8  numSv;
} GPS_NAV_ORB_t;


typedef struct
{
    uint32 iTOW;
    int32  ecefX;
    int32  ecefY;
    int32  ecefZ;
    uint32 pAcc;
} GPS_NAV_POSECEF_t;


typedef struct
{
    uint32 iTOW;
    int32  lon;
    int32  lat;
    int32  height;
    int32  hMSL;
    uint32 hAcc;
    uint32 vAcc;
} GPS_NAV_POSLLH_t;


typedef struct
{
    uint32 iTOW;
    uint16 year;
    uint8  month;
    uint8  day;
    uint8  hour;
    uint8  min;
    uint8  sec;
    uint8  valid;
    uint32 tAcc;
    int32   nano;
    uint8  fixType;
    uint8  flags;
    uint8  flags2;
    uint8  numSV;
    int32  lon;
    int32  lat;
    int32  height;
    int32  hMSL;
    uint32 hAcc;
    uint32 vAcc;
    int32  velN;
    int32  velE;
    int32  velD;
    int32  gSpeed;
    int32  headMot;
    uint32 sAcc;
    uint32 headAcc;
    uint16 pDOP;
    uint16 reserved2;
    uint32 reserved3;
    int32  headVeh;
    int16  magDec;
    uint16 magAcc;
} GPS_NAV_PVT_t;


typedef struct
{
    uint32 iTOW;
    uint8  version;
    uint8  numSvs;
} GPS_NAV_SAT_t;


typedef struct
{
    uint32 iTOW;
    uint8  geo;
    uint8  mode;
    int8   sys;
    uint8  service;
    uint8  cnt;
} GPS_NAV_SBAS_t;


typedef struct
{
    uint32 iTOW;
    int32  fTOW;
    int16  week;
    uint8  gpsFix;
    uint8  flags;
    int32  ecefX;
    int32  ecefY;
    int32  ecefZ;
    uint32 pAcc;
    int32  ecefVX;
    int32  ecefVY;
    int32  ecefVZ;
    uint32 sAcc;
    uint16 pDOP;
    uint8  numSV;
} GPS_NAV_SOL_t;


typedef struct
{
    uint32 iTOW;
    uint8  gpsFix;
    uint8  flags;
    uint8  fixStat;
    uint8  flags2;
    uint32 ttff;
    uint32 msss;
} GPS_NAV_STATUS_t;


typedef struct
{
    uint32 iTOW;
    uint8  numCh;
    uint8  globalFlags;
    uint16 reserved2;
} GPS_NAV_SVINFO_P1_t;


typedef struct
{
    uint8  chn;
    uint8  svid;
    uint8  flags;
    uint8  quality;
    uint8  cno;
    int8   elev;
    int16  azim;
    int32  prRes;
} GPS_NAV_SVINFO_P2_t;


typedef struct
{
    GPS_NAV_SVINFO_P1_t svinfo;
    GPS_NAV_SVINFO_P2_t numCh[PX4_SAT_INFO_MAX_SATELLITES];
} GPS_NAV_SVINFO_Combined_t;

typedef struct
{
    uint32 iTOW;
    uint32 SOW;
    int32  fSOW;
    int16  week;
    int8   leapS;
    uint8  valid;
    uint32  tAcc;
} GPS_NAV_TIMEBDS_t;


typedef struct
{
    uint32 iTOW;
    uint32 galTow;
    int32  fGalTow;
    int16  galWno;
    int8   leapS;
    uint8  valid;
    uint32 tAcc;
} GPS_NAV_TIMEGAL_t;


typedef struct
{
    uint32 iTOW;
    uint32 TOD;
    int32  fTOD;
    uint16 Nt;
    uint8  N4;
    uint8  valid;
    uint32 tAcc;
} GPS_NAV_TIMEGLO_t;


typedef struct
{
    uint32 iTOW;
    int32  fTOW;
    int16  week;
    int8   leapS;
    uint8  valid;
    uint32 tAcc;
} GPS_NAV_TIMEGPS_t;


typedef struct
{
    uint32 iTOW;
    uint8  version;
    uint8  srcOfCurrLs;
    int8   currLs;
    uint8  srcOfLsChange;
    int8   lsChange;
    int32  timeToLsEvent;
    uint16 dateOfLsGpsWn;
    uint16 dateOfLsGpsDn;
    uint8  valid;
} GPS_NAV_TIMELS_t;


typedef struct
{
    uint32 iTOW;
    uint32 tAcc;
    int32  nano;
    uint16 year;
    uint8  month;
    uint8  day;
    uint8  hour;
    uint8  min;
    uint8  sec;
    uint8  valid;
} GPS_NAV_TIMEUTC_t;


typedef struct
{
    uint32 iTOW;
    int32  ecefVX;
    int32  ecefVY;
    int32  ecefVZ;
    uint32 sAcc;
} GPS_NAV_VELECEF_t;


typedef struct
{
    uint32 iTOW;
    int32  velN;
    int32  velE;
    int32  velD;
    uint32 speed;
    uint32 gSpeed;
    int32  heading;
    uint32 sAcc;
    uint32 cAcc;
} GPS_NAV_VELNED_t;


typedef struct
{
    uint32 pinSel;
    uint32 pinBank;
    uint32 pinDir;
    uint32 pinVal;
    uint16 noisePerMS;
    uint16 agcCnt;
    uint8  aStatus;
    uint8  aPower;
    uint8  flags;
    uint8  reserved1;
    uint32 usedMask;
    uint8  vp[GPS_MON_HW_VP_PIN_COUNT];
    uint8  jamInd;
    uint16 reserved2;
    uint32 pinIrq;
    uint32 pullH;
    uint32 pullL;
} GPS_MON_HW_t;


typedef struct
{
    uint32  clearMask;
    uint32  saveMask;
    uint32  loadMask;
    uint8   deviceMask;
} GPS_CFG_CFG_t;


typedef struct
{
    uint16   mask;
    uint8    dynModel;
    uint8    fixMode;
    int32    fixedAlt;
    uint32   fixedAltVar;
    int8     minElev;
    uint8    drLimit;
    uint16   pDop;
    uint16   tDop;
    uint16   pAcc;
    uint16   tAcc;
    uint8    staticHoldThresh;
    uint8    dgnssTimeout;
    uint8    cnoThreshNumSVs;
    uint8    cnoThresh;
    uint16   reserved1;
    uint16   staticHoldMaxDist;
    uint8    utcStandard;
    uint8    reserved2;
    uint32   reserved3;
} GPS_CFG_NAV5_t;


typedef struct
{
    uint8  mode;
    uint8  usage;
    uint8  maxSBAS;
    uint8  scanmode2;
    uint32 scanmode1;
} GPS_CFG_SBAS_t;


typedef struct
{
    uint8    version;
    uint8    reserved1;
    uint16   flags;
    int32    ecefXOrLat;
    int32    ecefYOrLon;
    int32    ecefZOrAlt;
    int8     ecefXOrLatHP;
    int8     ecefYOrLonHP;
    int8     ecefZOrAltHP;
    uint8    reserved2;
    uint32   fixedPosAcc;
    uint32   svinMinDur;
    uint32   svinAccLimit;
    uint8    reserved3[8];
} GPS_CFG_TMODE3_t;


typedef struct
{
    uint8       portID;
    uint8       reserved1;
    uint16      txReady;
    uint32      mode;
    uint32      baudRate;
    uint16      inProtoMask;
    uint16      outProtoMask;
    uint16      flags;
    uint16      reserved2;
} GPS_CFG_PRT_t;


typedef struct
{
    uint8 msgClass;
    uint8 msgID;
    uint8 rate[6];
} GPS_CFG_MSG_t;


typedef struct
{
    uint8 clsID;
    uint8 msgID;
} GPS_ACK_ACK_t;


typedef struct
{
    uint8 clsID;
    uint8 msgID;
} GPS_ACK_NAK_t;


#ifdef __cplusplus
}
#endif 

#endif /* _GPS_UBX_MSG_H_ */
