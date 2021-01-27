#include "bat_app.h"
#include <unistd.h>
#include <cstring>

#define AMC_VOLT_PIPE_DEPTH    (1)
#define AMC_VOLT_PIPE_NAME     ("AMC_VOLT_PIPE")
#define AMC_VOLT_PIPE_RESERVED (1)

CFE_SB_PipeId_t VoltPipeId;

#pragma pack(push, 1)
typedef struct
{
    uint16 rpm_front_left;
    uint16 rpm_front_right;
    uint16 rpm_back_right;
    uint16 rpm_back_left;
    uint16 battery_voltage_mv;
    uint8  status;
    uint8  error;
    uint8  motors_in_fault;
    uint8  temperatur_c;
    uint8  checksum;
} AMC_BLDC_Observation_t;
#pragma pack(pop)


typedef struct
{
    uint8 TlmHeader[CFE_SB_TLM_HDR_SIZE];
    AMC_BLDC_Observation_t observation;
} AMC_BebopObservationMsg_t;


int32 BAT::InitDevice(void)
{
    int32 ret = CFE_SUCCESS;

    ret = CFE_SB_CreatePipe(&VoltPipeId, AMC_VOLT_PIPE_DEPTH,
            AMC_VOLT_PIPE_NAME);
    if(CFE_SUCCESS != ret)
    {
        return ret;
    }

    ret = CFE_SB_SubscribeEx(AMC_OUT_DATA_MID, VoltPipeId,
            CFE_SB_Default_Qos, AMC_VOLT_PIPE_RESERVED);

    return ret;
}


void BAT::CloseDevice(void)
{
    
}


int32 BAT::ReadDevice(float &Voltage, float &Current)
{
    int32 iStatus = CFE_SUCCESS;
    CFE_SB_Msg_t* MsgPtr = 0;
    CFE_SB_MsgId_t MsgId;

    static AMC_BebopObservationMsg_t observationMsg = {0};

    iStatus = CFE_SB_RcvMsg(&MsgPtr, VoltPipeId, CFE_SB_PEND_FOREVER);
    if(CFE_SUCCESS == iStatus)
    {
        if(AMC_OUT_DATA_MID == CFE_SB_GetMsgId(MsgPtr) && 
           CFE_SB_GetTotalMsgLength(MsgPtr) <= sizeof(observationMsg))
        {
            memcpy(&observationMsg, MsgPtr, sizeof(observationMsg));
        }
    }

    Voltage = (float) observationMsg.observation.battery_voltage_mv / 1000.0f;
}
