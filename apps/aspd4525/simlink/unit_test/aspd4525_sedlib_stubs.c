#include "cfe.h"

typedef enum
{
	SEDLIB_OK                             =   0
} SEDLIB_ReturnCode_t;

typedef enum
{
	SEDLIB_MSG_READ_NOT_APPLICABLE        = 0,
	SEDLIB_MSG_READ_PENDING_ACKNOWLEDGE   = 1,
	SEDLIB_MSG_READ_ACKNOWLEDGED          = 2
} SEDLIB_MsgReadStatus_t;

typedef enum
{
	SEDLIB_MSG_WRITE_NOT_APPLICABLE       = 0,
	SEDLIB_MSG_WRITE_PENDING_COMMIT       = 1,
	SEDLIB_MSG_WRITE_COMMITTED            = 2
} SEDLIB_MsgWriteStatus_t;

SEDLIB_ReturnCode_t SEDLIB_SendMsg(uint32 PipeHandle, CFE_SB_MsgPtr_t Msg) {
    return SEDLIB_OK;
}

SEDLIB_ReturnCode_t SEDLIB_GetPipe(char *PipeName, uint32 Size, uint32 *PipeHandle){
    return SEDLIB_OK;
}

SEDLIB_ReturnCode_t SEDLIB_GetMsgBufferForWrite(uint32 PipeHandle, CFE_SB_MsgPtr_t *Msg) {
    return SEDLIB_OK;
}

SEDLIB_ReturnCode_t SEDLIB_GetMsgBufferForRead(uint32 PipeHandle, CFE_SB_MsgPtr_t *Msg){
    return SEDLIB_OK;
}

SEDLIB_ReturnCode_t SEDLIB_GetMessageStatus(uint32 PipeHandle, SEDLIB_MsgReadStatus_t *ReadStatus, SEDLIB_MsgWriteStatus_t *WriteStatus){
    return SEDLIB_OK;
}

SEDLIB_ReturnCode_t SEDLIB_AcknowledgeMsg(uint32 PipeHandle){
    return SEDLIB_OK;
}

SEDLIB_ReturnCode_t SEDLIB_CommitMsg(uint32 PipeHandle){
    return SEDLIB_OK;
}

SEDLIB_ReturnCode_t SEDLIB_ReadMsg(uint32 PipeHandle, CFE_SB_MsgPtr_t Msg){
    return SEDLIB_OK;
}

SEDLIB_ReturnCode_t SEDLIB_WaitForResponse(uint32 RetryCount, uint32 SleepMs, uint32 PipeHandle, CFE_SB_MsgPtr_t Msg){
    return SEDLIB_OK;
}

SEDLIB_ReturnCode_t SEDLIB_SetGpio(uint32 Mask){
    return SEDLIB_OK;
}

SEDLIB_ReturnCode_t SEDLIB_ClearGpio(uint32 Mask){
    return SEDLIB_OK;
}

SEDLIB_ReturnCode_t SEDLIB_GetGpio(uint32 *Mask){
    return SEDLIB_OK;
}

SEDLIB_ReturnCode_t SEDLIB_SetDirection(uint32 Mask){
    return SEDLIB_OK;
}

SEDLIB_ReturnCode_t SEDLIB_GetGpioStatus(void){
    return SEDLIB_OK;
}
