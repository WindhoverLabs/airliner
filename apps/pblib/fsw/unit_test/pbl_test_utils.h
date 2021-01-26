
#ifndef PBLIB_TEST_UTILS_H
#define PBLIB_TEST_UTILS_H

/*
 * Includes
 */

#include "pb_lib.h"

extern PBLIB_AppData_t  PBLIB_AppData;

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Function Definitions
 */

void PBLIB_Test_Setup(void);
void PBLIB_Test_Setup_InitTbls(void);
void PBLIB_Test_TearDown(void);


#ifdef __cplusplus
}
#endif

#endif /* PBLIB_TEST_UTILS_H */

