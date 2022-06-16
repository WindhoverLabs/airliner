#ifndef PX4_LIB_H
#define PX4_LIB_H


#ifdef __cplusplus
extern "C" {
#endif
#include "cfe.h"

/**
 * Enum for board and external compass rotations.
 * This enum maps from board attitude to airframe attitude.
 */
enum Rotation {
    ROTATION_NONE                       = 0,
    ROTATION_YAW_45                     = 1,
    ROTATION_YAW_90                     = 2,
    ROTATION_YAW_135                    = 3,
    ROTATION_YAW_180                    = 4,
    ROTATION_YAW_225                    = 5,
    ROTATION_YAW_270                    = 6,
    ROTATION_YAW_315                    = 7,
    ROTATION_ROLL_180                   = 8,
    ROTATION_ROLL_180_YAW_45            = 9,
    ROTATION_ROLL_180_YAW_90            = 10,
    ROTATION_ROLL_180_YAW_135           = 11,
    ROTATION_PITCH_180                  = 12,
    ROTATION_ROLL_180_YAW_225           = 13,
    ROTATION_ROLL_180_YAW_270           = 14,
    ROTATION_ROLL_180_YAW_315           = 15,
    ROTATION_ROLL_90                    = 16,
    ROTATION_ROLL_90_YAW_45             = 17,
    ROTATION_ROLL_90_YAW_90             = 18,
    ROTATION_ROLL_90_YAW_135            = 19,
    ROTATION_ROLL_270                   = 20,
    ROTATION_ROLL_270_YAW_45            = 21,
    ROTATION_ROLL_270_YAW_90            = 22,
    ROTATION_ROLL_270_YAW_135           = 23,
    ROTATION_PITCH_90                   = 24,
    ROTATION_PITCH_270                  = 25,
    ROTATION_ROLL_270_YAW_270           = 26,
    ROTATION_ROLL_180_PITCH_270         = 27,
    ROTATION_PITCH_90_YAW_180           = 28,
    ROTATION_PITCH_90_ROLL_90           = 29,
    ROTATION_YAW_293_PITCH_68_ROLL_90   = 30,
    ROTATION_PITCH_90_ROLL_270          = 31,
    ROTATION_PITCH_9_YAW_180            = 32,
    ROTATION_MAX
};


uint64 PX4LIB_GetPX4TimeUs(void);
uint64 PX4LIB_GetPX4TimeMs(void);

/*
 * Compute the delta between a timestamp taken in the past
 * and now.
 *
 */
uint64 PX4LIB_GetPX4ElapsedTimeUs(uint64 then);

#ifdef __cplusplus
}
#endif


#endif /* PX4_LIB_H */
