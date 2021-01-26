
#ifndef MAVLINK_TEST_UTILS_H
#define MAVLINK_TEST_UTILS_H

/*
 * Includes
 */

#include "mavlink_app.h"

extern MAVLINK_AppData_t  MAVLINK_AppData;

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Function Definitions
 */

void MAVLINK_Test_Setup(void);
void MAVLINK_Test_TearDown(void);


#ifdef __cplusplus
}
#endif

#endif /* MAVLINK_TEST_UTILS_H */

