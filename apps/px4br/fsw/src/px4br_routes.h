/*
 * px4_routes.h
 *
 *  Created on: Oct 22, 2016
 *      Author: mbenson
 */

#ifndef SRC_PX4BR_ROUTES_H_
#define SRC_PX4BR_ROUTES_H_

#include "cfe.h"
#include "px4br_msgids.h"
#include "px4br_app.h"

#define PX4BR_MAX_ROUTE_COUNT		200

typedef int32 (*PX4_ENCODE_FUNC)(const char*, char*, int32);
typedef int32 (*PX4_DECODE_FUNC)(const char*, int32, char*);

typedef struct
{
	char 				OrbName[PX4BR_NAME_FIELD_LENGTH];
	PX4_ENCODE_FUNC		EncodeFunc;
	PX4_DECODE_FUNC		DecodeFunc;
	CFE_SB_MsgId_t		MsgID;
	uint32				EncodedSize;
	uint32				MsgDepth;
	uint32				InCount;
	uint32				OutCount;
} PX4BR_Route_t;

#endif /* SRC_PX4BR_ROUTES_H_ */
