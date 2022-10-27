/**
 * @file aspd4525_custom.c
 * @author Shahed Rahim (srahim@windhoverlabs.com)
 * @brief This is the file that reads the ASPD4525 data from SEDLIB
 * @version 0.1
 * @date 2022-06-10
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "cfe.h"
#include "aspd4525_events.h"
#include "aspd4525_map.h"
#include "sedlib.h"

/* Command pipe name. */
#define ASPD4525_SED_CMD_PIPE_NAME             ("IIC1_CMD")
/* Response (status) pipe name. */
#define ASPD4525_SED_STATUS_PIPE_NAME          ("IIC1_STATUS")

/* Message IDs. */
#define IIC_CMD_MSG_ID                       (SEDLIB_IIC_1_CMD_MID)
#define IIC_RESPONSE_MSG_ID                  (SEDLIB_IIC_1_TLM_MID)

/* Number of retry attempts to wait for a status message. */
#define ASPD4525_POLL_RETRY_ATTEMPTS           (10)
/* Number of milliseconds to wait for a status message per retry. */
#define ASPD4525_POLL_SLEEP_TIME_MS            (1)
#define ASPD4525_SED_MINOR_FRAME_MS            (4)

/**
 * \brief ASPD4525 device status
 */
typedef enum
{
    /*! ASPD4525 status uninitialized */
    ASPD4525_CUSTOM_UNINITIALIZED  = 0,
    /*! ASPD4525 status initialized */
    ASPD4525_CUSTOM_INITIALIZED   = 1
} ASPD4525_Custom_Status_t;


typedef struct
{
    ASPD4525_Custom_Status_t          Status;
        /** \brief Status port handle. */
    uint32                 StatusPortHandle;
    /** \brief Command port handle. */
    uint32                 CmdPortHandle;
    /** \brief Status buffer. */
    IIC_TransferResponse_t TransferResp;
    /** \brief Command buffer. */
    IIC_TransferCmd_t      TransferCmd;
    /** \brief At least one response has been received. */
    boolean                ResponseReceived;
    /** \brief Sequence count for reads. */
    uint8                  ReadCount;
    /** \brief Sequence count for writes. */
    uint8                  WriteCount;
} ASPD4525_AppCustomData_t;

ASPD4525_AppCustomData_t ASPD4525_AppCustomData;

int32 ASPD4525_SendWaitVerify(uint8 Address, uint8 Data, uint32 Size);
boolean ASPD4525_Custom_Receive(uint8 *Buffer, size_t Length);
//boolean ASPD4525_Custom_Send(uint8 Reg, uint8 Data);

void ASPD4525_Custom_InitData(void)
{
    CFE_PSP_MemSet(&ASPD4525_AppCustomData, 0, sizeof(ASPD4525_AppCustomData));
    return;
}

boolean ASPD4525_Custom_Init()
{
    boolean returnBool = TRUE;
    int32   iStatus    = CFE_SUCCESS;
    uint8   data       = 0;

    /* Get a handle to the status pipe. */
    iStatus = SEDLIB_GetPipe(ASPD4525_SED_STATUS_PIPE_NAME, 
                             sizeof(IIC_TransferResponse_t),
                             &ASPD4525_AppCustomData.StatusPortHandle);
    if(iStatus != CFE_SUCCESS)
    {
        returnBool = FALSE;
        (void) CFE_EVS_SendEvent(ASPD4525_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to initialize IIC status MsgPort. (0x%08lX)",
                                 iStatus);
        goto end_of_function;
    }

    /* Get a handle to the command pipe. */
    iStatus = SEDLIB_GetPipe(ASPD4525_SED_CMD_PIPE_NAME, 
                             sizeof(IIC_TransferCmd_t),
                             &ASPD4525_AppCustomData.CmdPortHandle);
    if(iStatus != CFE_SUCCESS)
    {
        returnBool = FALSE;
        (void) CFE_EVS_SendEvent(ASPD4525_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to initialize IIC command MsgPort. (0x%08lX)",
                                 iStatus);
        goto end_of_function;
    }

    /* Initialize command message. */
    CFE_SB_InitMsg(&ASPD4525_AppCustomData.TransferCmd,
                   IIC_CMD_MSG_ID, 
                   sizeof(ASPD4525_AppCustomData.TransferCmd), 
                   TRUE);

    /* Set command code. */
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&ASPD4525_AppCustomData.TransferCmd, IIC_SET_ADDRESS_CC);

    /* Set version number. */
    ASPD4525_AppCustomData.TransferCmd.Version = 1;

    iStatus = ASPD4525_SendWaitVerify(ASPD4525_I2C_ADDRESS, 0, 0);
    if(iStatus != CFE_SUCCESS)
    {
        returnBool = FALSE;
        (void) CFE_EVS_SendEvent(ASPD4525_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to set IIC address. (0x%08lX)",
                                 iStatus);
        goto end_of_function;
    }

#if 0
    /* Attempt to read an ID register to check custom receive call.
     * Who Am I validated in platform independent code. This also 
     * flushes any old data following a reset.
     */
    returnBool = ASPD4525_Custom_Receive(ASPD4525_REG_ID_A, &data, 1);
    if(FALSE == returnBool)
    {
        (void) CFE_EVS_SendEvent(ASPD4525_INIT_ERR_EID, CFE_EVS_ERROR,
            "ASPD4525_Custom_Receive failed");
        /* If receive was unsuccessful return FALSE */
        goto end_of_function;
    }
#endif

    ASPD4525_AppCustomData.Status = ASPD4525_CUSTOM_INITIALIZED;

end_of_function:
    return returnBool;
}

boolean ASPD4525_Custom_Uninit(void)
{
    boolean returnBool = TRUE;

    ASPD4525_AppCustomData.Status = ASPD4525_CUSTOM_UNINITIALIZED;

    return returnBool;
}

boolean ASPD4525_Custom_Measure(int16 *pressureDiffP, int16 *temperatureP, uint8 *statusP)
{
    boolean returnBool = FALSE;
    
    uint8 Register = 0;
    uint8 Message = 0;

    uint32 data;

    /* to store x, z, and y raw values*/
    uint8 Result[4] = { 0 };

    /* Null pointer check */
    if(0 == pressureDiffP || 0 == temperatureP || 0 == statusP)
    {
        (void) CFE_EVS_SendEvent(ASPD4525_DEVICE_ERR_EID, CFE_EVS_ERROR,
                                 "ASPD4525 Custom_Measure Null Pointer");
        returnBool = FALSE;
        goto end_of_function;
    }

    #if 0
    /* Issue a measure command */
    returnBool = ASPD4525_Custom_Send(Register, Message);
    if(FALSE == returnBool)
    {
        /* If send was unsuccessful return FALSE */
        goto end_of_function;
    }
    /* wait for it to complete (milliseconds). */
    (void) OS_TaskDelay(ASPD4525_CONVERSION_INTERVAL_US/1000);

    #endif
    
    /* Read the x, z, and y values from the device */
    returnBool = ASPD4525_Custom_Receive(Result, sizeof(Result));
    if(FALSE == returnBool)
    {
        /* If receive was unsuccessful return FALSE */
        goto end_of_function;
    }

    data = (((uint32)Result[0])<<24) | (((uint32)Result[1])<<16) | (((uint32)Result[2])<<8) | (((uint32)Result[3])<<0);

    *statusP =       (data & ASPD4525_STATUS_MASK) >> ASPD4525_STATUS_SHIFT;
    *pressureDiffP = (data & ASPD4525_DIFF_PRESSURE_MASK) >> ASPD4525_DIFF_PRESSURE_SHIFT;
    *temperatureP =  (data & ASPD4525_TEMPERATURE_MASK) >> ASPD4525_TEMPERATURE_SHIFT;

end_of_function:
    return returnBool;
}


int32 ASPD4525_SendWaitVerify(uint8 Address, uint8 Data, uint32 Size)
{
    int32               iStatus    = SEDLIB_OK;
    SEDLIB_ReturnCode_t returnCode = SEDLIB_OK;
    CFE_SB_MsgId_t      msgID;

    /* Set the sequence count. */
    ASPD4525_AppCustomData.TransferCmd.Action[0].Count = ASPD4525_AppCustomData.WriteCount++;
    /* Set the address. */
    ASPD4525_AppCustomData.TransferCmd.Action[0].Buffer[0] = Address;
    /* Set the data. */
    ASPD4525_AppCustomData.TransferCmd.Action[0].Buffer[1] = Data;
    /* Set the size. */
    ASPD4525_AppCustomData.TransferCmd.Action[0].TransferByteCount = Size;

    /* Send the message. */
    returnCode = SEDLIB_SendMsg(ASPD4525_AppCustomData.CmdPortHandle, 
                                (CFE_SB_MsgPtr_t)&ASPD4525_AppCustomData.TransferCmd);
    if(returnCode != SEDLIB_OK)
    {
        (void) CFE_EVS_SendEvent(ASPD4525_DEVICE_ERR_EID, CFE_EVS_ERROR,
                                 "SEDLIB_SendMsg error (0x%08X).",
                                 returnCode);
        iStatus = -1;
        goto end_of_function;
    }

    /* Wait for a minor frame. */
    (void) OS_TaskDelay(ASPD4525_SED_MINOR_FRAME_MS);

    /* Wait for response. */
    returnCode = SEDLIB_WaitForResponse(ASPD4525_POLL_RETRY_ATTEMPTS, 
                                        ASPD4525_POLL_SLEEP_TIME_MS, 
                                        ASPD4525_AppCustomData.StatusPortHandle, 
                                        (CFE_SB_MsgPtr_t)&ASPD4525_AppCustomData.TransferResp);
    if(returnCode != SEDLIB_MSG_FRESH_OK)
    {
        (void) CFE_EVS_SendEvent(ASPD4525_DEVICE_ERR_EID, CFE_EVS_ERROR,
                                 "SEDLIB_WaitForResponse error (0x%08X).",
                                 returnCode);
        iStatus = -1;
        goto end_of_function;
    }

    /* Verify the return message ID. */
    msgID = CFE_SB_GetMsgId((CFE_SB_MsgPtr_t)&ASPD4525_AppCustomData.TransferResp);
    if(msgID != IIC_RESPONSE_MSG_ID)
    {
        (void) CFE_EVS_SendEvent(ASPD4525_DEVICE_ERR_EID, CFE_EVS_ERROR,
                                 "Response message ID error.");
        iStatus = -1;
        goto end_of_function;
    }

    /* Verify the return status. */
    if(ASPD4525_AppCustomData.TransferResp.Response[0].Status != SEDLIB_OK)
    {
        (void) CFE_EVS_SendEvent(ASPD4525_DEVICE_ERR_EID, CFE_EVS_ERROR,
                                 "Response return status error %d.",
                                 ASPD4525_AppCustomData.TransferResp.Response[0].Status);
        iStatus = -1;
        goto end_of_function;
    }

    /* Check the sequence count. */
    if(ASPD4525_AppCustomData.ResponseReceived)
    {
        if(ASPD4525_AppCustomData.TransferResp.Response[0].Count == ASPD4525_AppCustomData.ReadCount)
        {
            (void) CFE_EVS_SendEvent(ASPD4525_DEVICE_ERR_EID, CFE_EVS_ERROR,
                                 "Sequence count error.");
            iStatus = -1;
            goto end_of_function;
        }
    }
    else
    {
        ASPD4525_AppCustomData.ResponseReceived = TRUE;
    }

    /* Set the last sequence count. */
    ASPD4525_AppCustomData.ReadCount = ASPD4525_AppCustomData.TransferResp.Response[0].Count;

end_of_function:
    return iStatus;
}


boolean ASPD4525_Custom_Receive(uint8 *Buffer, size_t Length)
{
    int32   iStatus    = CFE_SUCCESS;
    boolean returnBool = TRUE;

    /* Set command code. */
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&ASPD4525_AppCustomData.TransferCmd, IIC_WRITE_CC);

    //iStatus = ASPD4525_SendWaitVerify(ASPD4525_I2C_ADDRESS, 0, 0);
    iStatus = ASPD4525_SendWaitVerify(ASPD4525_I2C_ADDRESS, 0, 1);
    if(iStatus != CFE_SUCCESS)
    {
        returnBool = FALSE;
        (void) CFE_EVS_SendEvent(ASPD4525_DEVICE_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to write. (0x%08lX)",
                                 iStatus);
        goto end_of_function;
    }

    /* Set command code. */
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&ASPD4525_AppCustomData.TransferCmd, IIC_READ_CC);

    /* Read starting at that register. */
    iStatus = ASPD4525_SendWaitVerify(ASPD4525_I2C_ADDRESS, 0, Length);
    if(iStatus != CFE_SUCCESS)
    {
        returnBool = FALSE;
        (void) CFE_EVS_SendEvent(ASPD4525_DEVICE_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to read. (0x%08lX)",
                                 iStatus);
        goto end_of_function;
    }

    if (Buffer!=NULL) {
        /* Copy the buffer. */
        CFE_PSP_MemCpy((void *)Buffer, (void *)&ASPD4525_AppCustomData.TransferResp.Response[0].Buffer[0], Length);
    }

end_of_function:
    return returnBool;
}
