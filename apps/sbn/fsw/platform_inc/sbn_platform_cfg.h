/******************************************************************************
** File: sbn_platform_cfg.h
**
**      Copyright (c) 2004-2016, United States government as represented by the
**      administrator of the National Aeronautics Space Administration.
**      All rights reserved. This software(cFE) was created at NASA's Goddard
**      Space Flight Center pursuant to government contracts.
**
**      This software may be used only pursuant to a United States government
**      sponsored project and the United States government may not be charged
**      for use thereof.
**
** Purpose:
**      This header file contains prototypes for private functions and type
**      definitions for the Software Bus Network Application.
**
** Authors:   J. Wilmot/GSFC Code582
**            R. McGraw/SSI
**            C. Knight/ARC Code TI
******************************************************************************/
#include "cfe.h"

#ifndef _sbn_platform_cfg_h
#define _sbn_platform_cfg_h

/** @brief Maximum number of networks allowed. */
#define SBN_MAX_NETS                16

/** @brief Maximum number of peers per network allowed. */
#define SBN_MAX_PEERS_PER_NET       32

/** @brief Maximum number of subscriptions allowed per peer allowed. */
#define SBN_MAX_SUBS_PER_PEER       256 

/** @brief Maximum length of the name field in the `SbnPeerData.dat` file. */
#define SBN_MAX_PEER_NAME_LEN       32

/** @brief Maximum length of the network field in the `SbnPeerData.dat` file. */
#define SBN_MAX_NET_NAME_LEN        16

/**
 * @brief At most process this many SB messages per peer per wakeup.
 * (To prevent starvation if a peer is babbling.)
 */
#define SBN_MAX_MSG_PER_WAKEUP        32

/**
 * @brief In the polling configuration, how long (in milliseconds) to wait for
 * a SCH wakeup message before SBN times out and processes. (Note, should
 * really be significantly longer than the expected time between SCH wakeup
 * messages.)
 */
#define SBN_MAIN_LOOP_DELAY           200

/**
 * @brief If I haven't sent a message to a peer in this amount of time (in
 * seconds), call the poll function of the API in case it needs to perform
 * some connection maintenance.
 */
#define SBN_POLL_TIME        5

/**
 * @brief For each peer, a pipe is created to receive messages that the peer has
 * subscribed to. The pipe should be deep enough to handle all messages that
 * will queue between wakeups.
 */
#define SBN_PEER_PIPE_DEPTH           64

/**
 * @brief The maximum number of messages that will be queued for a particular
 * message ID for a particular peer.
 */
#define SBN_DEFAULT_MSG_LIM           8

/**
 * @brief The maximum number of subscription messages that will be queued
 * between wakeups.
 */
#define SBN_SUB_PIPE_DEPTH            256

/**
 * @brief The maximum number of subscription messages for a single message ID
 * that will be queued between wakeups. (These are received when updates occur
 * after SBN starts up.)
 */
#define SBN_MAX_ONESUB_PKTS_ON_PIPE   256

/**
 * @brief The maximum number of subscription messages for all message IDs that
 * will be queued between wakeups. (These are received on SBN startup.)
 */
#define SBN_MAX_ALLSUBS_PKTS_ON_PIPE  64

/** @brief The volatile memory location for the peer data file. */
#define SBN_VOL_PEER_FILENAME         "/ram/SbnPeerData.dat"

/** @brief The non-volatile memory location for the peer data file. */
#define SBN_NONVOL_PEER_FILENAME      "/cf/SbnPeerData.dat"

/**
 * @brief The maximum length of a line of configuration data in the peer data
 * file.
 */
#define SBN_PEER_FILE_LINE_SZ         128

/** @brief The volatile memory location for the module data file. */
#define SBN_VOL_MODULE_FILENAME       "/ram/SbnModuleData.dat"

/** @brief The non-volatile memory location for the module data file. */
#define SBN_NONVOL_MODULE_FILENAME    "/cf/SbnModuleData.dat"

/**
 * @brief The maximum length of a line of configuration data in the peer data
 * file.
 */
#define SBN_MODULE_FILE_LINE_SZ       128

/** @brief Maximum number of protocol modules. */
#define SBN_MAX_INTERFACE_TYPES       8

/**
 * @brief SBN modules can provide status messages for housekeeping requests,
 * this is the maximum length those messages can be.
 */
#define SBN_MOD_STATUS_MSG_SZ         128

/**
 * @brief Define this to use one task per peer pipe to send messages (each
 * task blocks on read). Otherwise, pipes will be polled periodically.
 */
#undef SBN_SEND_TASK

/**
 * @brief Define this to use one task per peer to receive messages (each
 * task blocks on read). Otherwise, another method (e.g. select) must be used
 * to prevent blocking.
 */
#undef SBN_RECV_TASK

/**
 * @brief If defined, remapping is enabled at boot time.
 */
#define SBN_REMAP_ENABLED

#endif /* _sbn_platform_cfg_h_ */
