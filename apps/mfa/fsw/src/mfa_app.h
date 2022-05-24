#ifndef MFA_APP_H
#define MFA_APP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "cfe.h"
#include "mfa_tbldefs.h"
#include "mfa_msgs.h"


#define MFA_MAJOR_VERSION    (1)
#define MFA_MINOR_VERSION    (0)
#define MFA_PATCH_VERSION    (0)


typedef struct {
	CFE_TBL_Handle_t   ConfigTblHdl;
	MFA_ConfigTbl_t   *ConfigTblPtr;
	MFA_HkTlm_t        HkTlm;
	CFE_SB_PipeId_t    SchPipeId;
} MFA_AppData_t;


uint32 MFA_AppInit(void);
void   MFA_AppMain(void);
void   MFA_AppDeinit(void);
uint32 MFA_InitEvents(void);
uint32 MFA_InitTables(void);
uint32 MFA_InitData(void);
uint32 MFA_InitPipes(void);


#ifdef __cplusplus
}
#endif

#endif /* For MFA_APP_H */
