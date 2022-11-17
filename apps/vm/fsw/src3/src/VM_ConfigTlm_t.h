#ifndef VM_CONFIGTLM_T_H
#define VM_CONFIGTLM_T_H

struct VM_ConfigTlm_t {

public:
	uint8 ucHeader[CFE_SB_TLM_HDR_SIZE];
	VM_ConfigTbl_t ConfigTbl;
};

#endif
