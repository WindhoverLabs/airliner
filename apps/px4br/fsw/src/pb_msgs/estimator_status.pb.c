/* Automatically generated nanopb constant definitions */
/* Generated by nanopb-0.3.6 at Wed Oct 26 00:46:10 2016. */

#include "estimator_status.pb.h"

/* @@protoc_insertion_point(includes) */
#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif



const pb_field_t px4_estimator_status_pb_fields[12] = {
    PB_FIELD(  1, UINT64  , REQUIRED, STATIC  , FIRST, px4_estimator_status_pb, timestamp, timestamp, 0),
    PB_FIELD(  2, FLOAT   , REPEATED, STATIC  , OTHER, px4_estimator_status_pb, states, timestamp, 0),
    PB_FIELD(  3, FLOAT   , REQUIRED, STATIC  , OTHER, px4_estimator_status_pb, n_states, states, 0),
    PB_FIELD(  4, FLOAT   , REPEATED, STATIC  , OTHER, px4_estimator_status_pb, vibe, n_states, 0),
    PB_FIELD(  5, FLOAT   , REPEATED, STATIC  , OTHER, px4_estimator_status_pb, covariances, vibe, 0),
    PB_FIELD(  6, UINT32  , REQUIRED, STATIC  , OTHER, px4_estimator_status_pb, gps_check_fail_flags, covariances, 0),
    PB_FIELD(  7, UINT32  , REQUIRED, STATIC  , OTHER, px4_estimator_status_pb, control_mode_flags, gps_check_fail_flags, 0),
    PB_FIELD(  8, UINT32  , REQUIRED, STATIC  , OTHER, px4_estimator_status_pb, filter_fault_flags, control_mode_flags, 0),
    PB_FIELD(  9, UINT32  , REQUIRED, STATIC  , OTHER, px4_estimator_status_pb, nan_flags, filter_fault_flags, 0),
    PB_FIELD( 10, UINT32  , REQUIRED, STATIC  , OTHER, px4_estimator_status_pb, health_flags, nan_flags, 0),
    PB_FIELD( 11, UINT32  , REQUIRED, STATIC  , OTHER, px4_estimator_status_pb, timeout_flags, health_flags, 0),
    PB_LAST_FIELD
};


/* @@protoc_insertion_point(eof) */
