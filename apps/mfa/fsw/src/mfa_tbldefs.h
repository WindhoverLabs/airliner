#ifndef MFA_TBLDEFS_H
#define MFA_TBLDEFS_H

#ifdef __cplusplus
extern "C" {
#endif

/* TODO:  Add documentation. */
#define MFA_MESSAGE_MAX_SIZE (100)

typedef struct {
	char Message[MFA_MESSAGE_MAX_SIZE];
} MFA_ConfigTbl_t;

#ifdef __cplusplus
}
#endif

#endif
