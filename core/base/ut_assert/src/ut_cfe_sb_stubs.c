/*
**
** File: ut_cfe_sb_stubs.c
**
** $Id: ut_cfe_sb_stubs.c 1.3 2016/06/23 14:39:05EDT czogby Exp  $
**
** Copyright 2012-2013 United States Government as represented by the 
** Administrator of the National Aeronautics and Space Administration. 
** All Other Rights Reserved.  
**
** This software was created at NASA's Goddard Space Flight Center.
** This software is governed by the NASA Open Source Agreement and may be 
** used, distributed and modified only pursuant to the terms of that 
** agreement.
**
** Purpose: Unit test stubs for cFE Software Bus routines
**
** $Log: ut_cfe_sb_stubs.c  $
** Revision 1.3 2016/06/23 14:39:05EDT czogby 
** Update Stub Functions to Match Changes in Corresponding Function Prototypes in New Releases of CFE, OSAL, and PSP
** Revision 1.2 2016/03/08 18:01:51EST czogby 
** Add variable: CFE_SB_Qos_t CFE_SB_Default_Qos
** Revision 1.1 2016/02/08 20:53:31EST rperera 
** Initial revision
** Member added to project /FSW-TOOLs-REPOSITORY/ut-assert/src/project.pj
** Revision 1.7 2015/06/16 16:14:02EDT sstrege 
** Added copyright information
** Revision 1.6 2015/05/07 14:39:52EDT czogby 
** Add ability to set a custom return value to continue after a designated call count
** Revision 1.5 2015/05/01 13:59:29EDT czogby 
** Added UtAssert fail where error messages generated, fixed a comment, fixed a return type
** Revision 1.4 2015/03/17 18:23:30EDT czogby 
** Add missing Functionality to UT Assert Library
** --- Added comments ---  czogby [2015/03/31 18:29:23Z]
** Added a function to Ut_CFE_SB_Reset, added missing ability to call hook functions / set return values
** Revision 1.3 2015/03/10 15:21:47EDT czogby 
** Add Missing Functionality to UT Assert Library
** Revision 1.2 2011/03/10 11:18:33EST sslegel 
** Added hook support to CFE_SB_Subscribe, CFE_SB_SubscribeEx, and CFE_SB_Unsunscribe
** Revision 1.1 2011/02/15 11:13:03EST sslegel 
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/FSW-TOOLS-REPOSITORY/ut-assert/src/project.pj
**
*/

#include "cfe.h"
#include "ut_cfe_sb_stubs.h"
#include "ut_cfe_sb_hooks.h"

CFE_SB_Qos_t CFE_SB_Default_Qos;

Ut_CFE_SB_HookTable_t           Ut_CFE_SB_HookTable;
Ut_CFE_SB_ReturnCodeTable_t     Ut_CFE_SB_ReturnCodeTable[UT_CFE_SB_MAX_INDEX];

void Ut_CFE_SB_Reset(void)
{
    memset(&Ut_CFE_SB_HookTable, 0, sizeof(Ut_CFE_SB_HookTable));
    memset(&Ut_CFE_SB_ReturnCodeTable, 0, sizeof(Ut_CFE_SB_ReturnCodeTable));
    
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_CREATEPIPE_INDEX, &Ut_CFE_SB_CreatePipeHook);
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_SENDMSG_INDEX, &Ut_CFE_SB_SendMsgHook);
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_RCVMSG_INDEX, &Ut_CFE_SB_RcvMsgHook);
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_INITMSG_INDEX, &Ut_CFE_SB_InitMsgHook);
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_MSGHDRSIZE_INDEX, &Ut_CFE_SB_MsgHdrSizeHook);
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_GETUSERDATA_INDEX, &Ut_CFE_SB_GetUserDataHook);
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_GETMSGID_INDEX, &Ut_CFE_SB_GetMsgIdHook);
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_SETMSGID_INDEX, &Ut_CFE_SB_SetMsgIdHook);
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_GETUSERDATALENGTH_INDEX, &Ut_CFE_SB_GetUserDataLengthHook);
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_SETUSERDATALENGTH_INDEX, &Ut_CFE_SB_SetUserDataLengthHook);
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_GETTOTALMSGLENGTH_INDEX, &Ut_CFE_SB_GetTotalMsgLengthHook);
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_SETTOTALMSGLENGTH_INDEX, &Ut_CFE_SB_SetTotalMsgLengthHook);
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_GETMSGTIME_INDEX, &Ut_CFE_SB_GetMsgTimeHook);
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_SETMSGTIME_INDEX, &Ut_CFE_SB_SetMsgTimeHook);
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_GETCMDCODE_INDEX, &Ut_CFE_SB_GetCmdCodeHook);
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_SETCMDCODE_INDEX, &Ut_CFE_SB_SetCmdCodeHook);
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_GETCHECKSUM_INDEX, &Ut_CFE_SB_GetChecksumHook);
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_GENERATECHECKSUM_INDEX, &Ut_CFE_SB_GenerateChecksumHook);
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_VALIDATECHECKSUM_INDEX, &Ut_CFE_SB_ValidateChecksumHook);

    Ut_CFE_SB_ClearPipes();
    Ut_CFE_SB_ClearMsgQueue();
}

void Ut_CFE_SB_SetFunctionHook(uint32 Index, void *FunPtr)
{
    if      (Index == UT_CFE_SB_CREATEPIPE_INDEX)         { Ut_CFE_SB_HookTable.CFE_SB_CreatePipe = FunPtr; }
    else if (Index == UT_CFE_SB_DELETEPIPE_INDEX)         { Ut_CFE_SB_HookTable.CFE_SB_DeletePipe = FunPtr; }
    else if (Index == UT_CFE_SB_SUBSCRIBEEX_INDEX)        { Ut_CFE_SB_HookTable.CFE_SB_SubscribeEx = FunPtr; }
    else if (Index == UT_CFE_SB_SUBSCRIBE_INDEX)          { Ut_CFE_SB_HookTable.CFE_SB_Subscribe = FunPtr; }
    else if (Index == UT_CFE_SB_SUBSCRIBELOCAL_INDEX)     { Ut_CFE_SB_HookTable.CFE_SB_SubscribeLocal = FunPtr; }
    else if (Index == UT_CFE_SB_UNSUBSCRIBE_INDEX)        { Ut_CFE_SB_HookTable.CFE_SB_Unsubscribe = FunPtr; }
    else if (Index == UT_CFE_SB_UNSUBSCRIBELOCAL_INDEX)   { Ut_CFE_SB_HookTable.CFE_SB_UnsubscribeLocal = FunPtr; }
    else if (Index == UT_CFE_SB_SENDMSG_INDEX)            { Ut_CFE_SB_HookTable.CFE_SB_SendMsg = FunPtr; }
    else if (Index == UT_CFE_SB_PASSMSG_INDEX)            { Ut_CFE_SB_HookTable.CFE_SB_PassMsg = FunPtr; }
    else if (Index == UT_CFE_SB_RCVMSG_INDEX)             { Ut_CFE_SB_HookTable.CFE_SB_RcvMsg = FunPtr; }
    else if (Index == UT_CFE_SB_GETLASTSENDERID_INDEX)    { Ut_CFE_SB_HookTable.CFE_SB_GetLastSenderId = FunPtr; }
    else if (Index == UT_CFE_SB_ZEROCOPYGETPTR_INDEX)     { Ut_CFE_SB_HookTable.CFE_SB_ZeroCopyGetPtr = FunPtr; }
    else if (Index == UT_CFE_SB_ZEROCOPYRELEASEPTR_INDEX) { Ut_CFE_SB_HookTable.CFE_SB_ZeroCopyReleasePtr = FunPtr; }
    else if (Index == UT_CFE_SB_ZEROCOPYSEND_INDEX)       { Ut_CFE_SB_HookTable.CFE_SB_ZeroCopySend = FunPtr; }
    else if (Index == UT_CFE_SB_ZEROCOPYPASS_INDEX)       { Ut_CFE_SB_HookTable.CFE_SB_ZeroCopyPass = FunPtr; }
    else if (Index == UT_CFE_SB_INITMSG_INDEX)            { Ut_CFE_SB_HookTable.CFE_SB_InitMsg = FunPtr; }
    else if (Index == UT_CFE_SB_MSGHDRSIZE_INDEX)         { Ut_CFE_SB_HookTable.CFE_SB_MsgHdrSize = FunPtr; }
    else if (Index == UT_CFE_SB_GETUSERDATA_INDEX)        { Ut_CFE_SB_HookTable.CFE_SB_GetUserData = FunPtr; }
    else if (Index == UT_CFE_SB_GETMSGID_INDEX)           { Ut_CFE_SB_HookTable.CFE_SB_GetMsgId = FunPtr; }
    else if (Index == UT_CFE_SB_SETMSGID_INDEX)           { Ut_CFE_SB_HookTable.CFE_SB_SetMsgId = FunPtr; }
    else if (Index == UT_CFE_SB_GETUSERDATALENGTH_INDEX)  { Ut_CFE_SB_HookTable.CFE_SB_GetUserDataLength = FunPtr; }
    else if (Index == UT_CFE_SB_SETUSERDATALENGTH_INDEX)  { Ut_CFE_SB_HookTable.CFE_SB_SetUserDataLength = FunPtr; }
    else if (Index == UT_CFE_SB_GETTOTALMSGLENGTH_INDEX)  { Ut_CFE_SB_HookTable.CFE_SB_GetTotalMsgLength = FunPtr; }
    else if (Index == UT_CFE_SB_SETTOTALMSGLENGTH_INDEX)  { Ut_CFE_SB_HookTable.CFE_SB_SetTotalMsgLength = FunPtr; }
    else if (Index == UT_CFE_SB_GETMSGTIME_INDEX)         { Ut_CFE_SB_HookTable.CFE_SB_GetMsgTime = FunPtr; }
    else if (Index == UT_CFE_SB_SETMSGTIME_INDEX)         { Ut_CFE_SB_HookTable.CFE_SB_SetMsgTime = FunPtr; }
    else if (Index == UT_CFE_SB_TIMESTAMPMSG_INDEX)       { Ut_CFE_SB_HookTable.CFE_SB_TimeStampMsg = FunPtr; }
    else if (Index == UT_CFE_SB_GETCMDCODE_INDEX)         { Ut_CFE_SB_HookTable.CFE_SB_GetCmdCode = FunPtr; }
    else if (Index == UT_CFE_SB_SETCMDCODE_INDEX)         { Ut_CFE_SB_HookTable.CFE_SB_SetCmdCode = FunPtr; }
    else if (Index == UT_CFE_SB_GETCHECKSUM_INDEX)        { Ut_CFE_SB_HookTable.CFE_SB_GetChecksum = FunPtr; }
    else if (Index == UT_CFE_SB_GENERATECHECKSUM_INDEX)   { Ut_CFE_SB_HookTable.CFE_SB_GenerateChecksum = FunPtr; }
    else if (Index == UT_CFE_SB_VALIDATECHECKSUM_INDEX)   { Ut_CFE_SB_HookTable.CFE_SB_ValidateChecksum = FunPtr; }
    else if (Index == UT_CFE_SB_CLEANUPAPP_INDEX)         { Ut_CFE_SB_HookTable.CFE_SB_CleanUpApp = FunPtr; }
    else
    {
        printf("Unsupported SB Index In SetFunctionHook Call %lu\n", Index);
        UtAssert_True(FALSE, "Unsupported SB Index In SetFunctionHook Call");
    }
}

void Ut_CFE_SB_SetReturnCode(uint32 Index, int32 RtnVal, uint32 CallCnt)
{
    if (Index < UT_CFE_SB_MAX_INDEX) {
        Ut_CFE_SB_ReturnCodeTable[Index].Value = RtnVal;
        Ut_CFE_SB_ReturnCodeTable[Index].Count = CallCnt;
    }
    else
    {
        printf("Unsupported SB Index In SetReturnCode Call %lu\n", Index);
        UtAssert_True(FALSE, "Unsupported SB Index In SetReturnCode Call");
    }
}

boolean Ut_CFE_SB_UseReturnCode(uint32 Index)
{
    if (Ut_CFE_SB_ReturnCodeTable[Index].Count > 0)
    {
        Ut_CFE_SB_ReturnCodeTable[Index].Count--;
        if (Ut_CFE_SB_ReturnCodeTable[Index].Count == 0)
            return(TRUE);
    }
    else if (Ut_CFE_SB_ReturnCodeTable[Index].ContinueReturnCodeAfterCountZero == TRUE)
    {
        return(TRUE);
    }
    
    return(FALSE);
}

void Ut_CFE_SB_ContinueReturnCodeAfterCountZero(uint32 Index)
{
    Ut_CFE_SB_ReturnCodeTable[Index].ContinueReturnCodeAfterCountZero = TRUE;
}

int32 CFE_SB_CreatePipe (CFE_SB_PipeId_t *PipeIdPtr, uint16 Depth, const char *PipeName)
{
    /* Check for specified return */
    if (Ut_CFE_SB_UseReturnCode(UT_CFE_SB_CREATEPIPE_INDEX))
        return Ut_CFE_SB_ReturnCodeTable[UT_CFE_SB_CREATEPIPE_INDEX].Value;
 
    /* Check for Function Hook */
    if (Ut_CFE_SB_HookTable.CFE_SB_CreatePipe)
        return Ut_CFE_SB_HookTable.CFE_SB_CreatePipe(PipeIdPtr, Depth, PipeName);
        
    return CFE_SUCCESS;
}

int32 CFE_SB_DeletePipe (CFE_SB_PipeId_t PipeId)
{
    /* Check for specified return */
    if (Ut_CFE_SB_UseReturnCode(UT_CFE_SB_DELETEPIPE_INDEX))
        return Ut_CFE_SB_ReturnCodeTable[UT_CFE_SB_DELETEPIPE_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_CFE_SB_HookTable.CFE_SB_DeletePipe)
        return Ut_CFE_SB_HookTable.CFE_SB_DeletePipe(PipeId);
        
    return CFE_SUCCESS;
}

int32 CFE_SB_SubscribeEx (CFE_SB_MsgId_t MsgId, CFE_SB_PipeId_t PipeId,
                          CFE_SB_Qos_t Quality, uint16 MsgLim)
{
    /* Check for specified return */
    if (Ut_CFE_SB_UseReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX))
        return Ut_CFE_SB_ReturnCodeTable[UT_CFE_SB_SUBSCRIBEEX_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_CFE_SB_HookTable.CFE_SB_SubscribeEx)
        return Ut_CFE_SB_HookTable.CFE_SB_SubscribeEx(MsgId, PipeId, Quality, MsgLim);

    return CFE_SUCCESS;
}

int32 CFE_SB_Subscribe(CFE_SB_MsgId_t MsgId, CFE_SB_PipeId_t PipeId)
{
    /* Check for specified return */
    if (Ut_CFE_SB_UseReturnCode(UT_CFE_SB_SUBSCRIBE_INDEX))
        return Ut_CFE_SB_ReturnCodeTable[UT_CFE_SB_SUBSCRIBE_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_CFE_SB_HookTable.CFE_SB_Subscribe)
        return Ut_CFE_SB_HookTable.CFE_SB_Subscribe(MsgId, PipeId);
    
    return CFE_SUCCESS;
}

int32 CFE_SB_SubscribeLocal(CFE_SB_MsgId_t   MsgId, CFE_SB_PipeId_t  PipeId, uint16 MsgLim)
{
    /* Check for specified return */
    if (Ut_CFE_SB_UseReturnCode(UT_CFE_SB_SUBSCRIBELOCAL_INDEX))
        return Ut_CFE_SB_ReturnCodeTable[UT_CFE_SB_SUBSCRIBELOCAL_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_CFE_SB_HookTable.CFE_SB_SubscribeLocal)
        return Ut_CFE_SB_HookTable.CFE_SB_SubscribeLocal(MsgId, PipeId, MsgLim);
    
    return CFE_SUCCESS;
}

int32 CFE_SB_Unsubscribe(CFE_SB_MsgId_t MsgId, CFE_SB_PipeId_t PipeId)
{
    /* Check for specified return */
    if (Ut_CFE_SB_UseReturnCode(UT_CFE_SB_UNSUBSCRIBE_INDEX))
        return Ut_CFE_SB_ReturnCodeTable[UT_CFE_SB_UNSUBSCRIBE_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_CFE_SB_HookTable.CFE_SB_Unsubscribe)
        return Ut_CFE_SB_HookTable.CFE_SB_Unsubscribe(MsgId, PipeId);
    
    return CFE_SUCCESS;
}

int32 CFE_SB_UnsubscribeLocal(CFE_SB_MsgId_t MsgId, CFE_SB_PipeId_t PipeId)
{
    /* Check for specified return */
    if (Ut_CFE_SB_UseReturnCode(UT_CFE_SB_UNSUBSCRIBELOCAL_INDEX))
        return Ut_CFE_SB_ReturnCodeTable[UT_CFE_SB_UNSUBSCRIBELOCAL_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_CFE_SB_HookTable.CFE_SB_UnsubscribeLocal)
        return Ut_CFE_SB_HookTable.CFE_SB_UnsubscribeLocal(MsgId, PipeId);
    
    return CFE_SUCCESS;
}

int32 CFE_SB_SendMsg (CFE_SB_Msg_t *MsgPtr)
{
    /* Check for specified return */
    if (Ut_CFE_SB_UseReturnCode(UT_CFE_SB_SENDMSG_INDEX))
        return Ut_CFE_SB_ReturnCodeTable[UT_CFE_SB_SENDMSG_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_CFE_SB_HookTable.CFE_SB_SendMsg)
        return Ut_CFE_SB_HookTable.CFE_SB_SendMsg(MsgPtr);

    return CFE_SUCCESS;
}

int32 CFE_SB_PassMsg (CFE_SB_Msg_t *MsgPtr)
{
    /* Check for specified return */
    if (Ut_CFE_SB_UseReturnCode(UT_CFE_SB_PASSMSG_INDEX))
        return Ut_CFE_SB_ReturnCodeTable[UT_CFE_SB_PASSMSG_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_CFE_SB_HookTable.CFE_SB_PassMsg)
        return Ut_CFE_SB_HookTable.CFE_SB_PassMsg(MsgPtr);

    return CFE_SUCCESS;
}

int32 CFE_SB_RcvMsg (CFE_SB_MsgPtr_t *BufPtr, CFE_SB_PipeId_t PipeId,
                    int32 TimeOut)
{
    /* Check for specified return */
    if (Ut_CFE_SB_UseReturnCode(UT_CFE_SB_RCVMSG_INDEX))
        return Ut_CFE_SB_ReturnCodeTable[UT_CFE_SB_RCVMSG_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_CFE_SB_HookTable.CFE_SB_RcvMsg)
        return Ut_CFE_SB_HookTable.CFE_SB_RcvMsg(BufPtr,PipeId,TimeOut);
    
    return CFE_SUCCESS;
}

uint32  CFE_SB_GetLastSenderId(CFE_SB_SenderId_t **Ptr,CFE_SB_PipeId_t  PipeId)
{
    /* Check for specified return */
    if (Ut_CFE_SB_UseReturnCode(UT_CFE_SB_GETLASTSENDERID_INDEX))
        return Ut_CFE_SB_ReturnCodeTable[UT_CFE_SB_GETLASTSENDERID_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_CFE_SB_HookTable.CFE_SB_GetLastSenderId)
        return Ut_CFE_SB_HookTable.CFE_SB_GetLastSenderId(Ptr, PipeId);

    return CFE_SUCCESS;
}

CFE_SB_Msg_t  *CFE_SB_ZeroCopyGetPtr(uint16  MsgSize,CFE_SB_ZeroCopyHandle_t *BufferHandle)
{
    /* Can't specify return value - Ut_CFE_SB_SetReturnCode can only return integers */

    /* Check for Function Hook */
    if (Ut_CFE_SB_HookTable.CFE_SB_ZeroCopyGetPtr)
        return (CFE_SB_Msg_t  *)Ut_CFE_SB_HookTable.CFE_SB_ZeroCopyGetPtr(MsgSize, BufferHandle);

    return NULL;
}

int32 CFE_SB_ZeroCopyReleasePtr(CFE_SB_Msg_t  *Ptr2Release,CFE_SB_ZeroCopyHandle_t BufferHandle)
{
    /* Check for specified return */
    if (Ut_CFE_SB_UseReturnCode(UT_CFE_SB_ZEROCOPYRELEASEPTR_INDEX))
        return Ut_CFE_SB_ReturnCodeTable[UT_CFE_SB_ZEROCOPYRELEASEPTR_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_CFE_SB_HookTable.CFE_SB_ZeroCopyReleasePtr)
        return Ut_CFE_SB_HookTable.CFE_SB_ZeroCopyReleasePtr(Ptr2Release, BufferHandle);

    return CFE_SUCCESS;
}

int32 CFE_SB_ZeroCopySend(CFE_SB_Msg_t   *MsgPtr, CFE_SB_ZeroCopyHandle_t BufferHandle)
{
    /* Check for specified return */
    if (Ut_CFE_SB_UseReturnCode(UT_CFE_SB_ZEROCOPYSEND_INDEX))
        return Ut_CFE_SB_ReturnCodeTable[UT_CFE_SB_ZEROCOPYSEND_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_CFE_SB_HookTable.CFE_SB_ZeroCopySend)
        return Ut_CFE_SB_HookTable.CFE_SB_ZeroCopySend(MsgPtr, BufferHandle);

    return CFE_SUCCESS;
}

int32 CFE_SB_ZeroCopyPass(CFE_SB_Msg_t   *MsgPtr, CFE_SB_ZeroCopyHandle_t BufferHandle)
{
    /* Check for specified return */
    if (Ut_CFE_SB_UseReturnCode(UT_CFE_SB_ZEROCOPYPASS_INDEX))
        return Ut_CFE_SB_ReturnCodeTable[UT_CFE_SB_ZEROCOPYPASS_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_CFE_SB_HookTable.CFE_SB_ZeroCopyPass)
        return Ut_CFE_SB_HookTable.CFE_SB_ZeroCopyPass(MsgPtr, BufferHandle);

    return CFE_SUCCESS;
}

void CFE_SB_InitMsg (void *MsgPtr, CFE_SB_MsgId_t MsgId, uint16 Length, boolean Clear)
{
    /* Can't specify return value - this is a void function */

    /* Check for Function Hook */
    if (Ut_CFE_SB_HookTable.CFE_SB_InitMsg)
        Ut_CFE_SB_HookTable.CFE_SB_InitMsg(MsgPtr,MsgId,Length,Clear);

    return;       
}

uint16 CFE_SB_MsgHdrSize (CFE_SB_MsgId_t MsgId)
{
    /* Check for specified return */
    if (Ut_CFE_SB_UseReturnCode(UT_CFE_SB_MSGHDRSIZE_INDEX))
        return Ut_CFE_SB_ReturnCodeTable[UT_CFE_SB_MSGHDRSIZE_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_CFE_SB_HookTable.CFE_SB_MsgHdrSize)
        return Ut_CFE_SB_HookTable.CFE_SB_MsgHdrSize(MsgId);
 
    return CFE_SUCCESS;
}

void *CFE_SB_GetUserData(CFE_SB_MsgPtr_t MsgPtr)
{
    /* Can't specify return value - this is a void function */

    /* Check for Function Hook */
    if (Ut_CFE_SB_HookTable.CFE_SB_GetUserData)
        return Ut_CFE_SB_HookTable.CFE_SB_GetUserData(MsgPtr);

    return  NULL;    
}

CFE_SB_MsgId_t CFE_SB_GetMsgId (CFE_SB_MsgPtr_t MsgPtr)
{
    /* Check for specified return */
    if (Ut_CFE_SB_UseReturnCode(UT_CFE_SB_GETMSGID_INDEX))
        return Ut_CFE_SB_ReturnCodeTable[UT_CFE_SB_GETMSGID_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_CFE_SB_HookTable.CFE_SB_GetMsgId)
        return Ut_CFE_SB_HookTable.CFE_SB_GetMsgId(MsgPtr);
    
    return CFE_SUCCESS;
}

void  CFE_SB_SetMsgId (CFE_SB_MsgPtr_t MsgPtr, CFE_SB_MsgId_t MsgId)
{
    /* Can't specify return value - this is a void function */

    /* Check for Function Hook */
    if (Ut_CFE_SB_HookTable.CFE_SB_SetMsgId)
        Ut_CFE_SB_HookTable.CFE_SB_SetMsgId(MsgPtr,MsgId);
    
    return;
}

uint16 CFE_SB_GetUserDataLength(CFE_SB_MsgPtr_t MsgPtr)
{
    /* Check for specified return */
    if (Ut_CFE_SB_UseReturnCode(UT_CFE_SB_GETUSERDATALENGTH_INDEX))
        return Ut_CFE_SB_ReturnCodeTable[UT_CFE_SB_GETUSERDATALENGTH_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_CFE_SB_HookTable.CFE_SB_GetUserDataLength)
        return Ut_CFE_SB_HookTable.CFE_SB_GetUserDataLength(MsgPtr);
    
    return CFE_SUCCESS;
}/* end CFE_SB_GetUserDataLength */

void CFE_SB_SetUserDataLength(CFE_SB_MsgPtr_t MsgPtr,uint16 DataLength)
{
    /* Can't specify return value - this is a void function */

    /* Check for Function Hook */
    if (Ut_CFE_SB_HookTable.CFE_SB_SetUserDataLength)
        Ut_CFE_SB_HookTable.CFE_SB_SetUserDataLength(MsgPtr,DataLength);
    
    return;
}

uint16 CFE_SB_GetTotalMsgLength(CFE_SB_MsgPtr_t MsgPtr)
{
    /* Check for specified return */
    if (Ut_CFE_SB_UseReturnCode(UT_CFE_SB_GETTOTALMSGLENGTH_INDEX))
        return Ut_CFE_SB_ReturnCodeTable[UT_CFE_SB_GETTOTALMSGLENGTH_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_CFE_SB_HookTable.CFE_SB_GetTotalMsgLength)
        return Ut_CFE_SB_HookTable.CFE_SB_GetTotalMsgLength(MsgPtr);
    
    return CFE_SUCCESS;
}

void CFE_SB_SetTotalMsgLength(CFE_SB_MsgPtr_t MsgPtr,uint16 TotalLength)
{
    /* Can't specify return value - this is a void function */

    /* Check for Function Hook */
    if (Ut_CFE_SB_HookTable.CFE_SB_SetTotalMsgLength)
        Ut_CFE_SB_HookTable.CFE_SB_SetTotalMsgLength(MsgPtr,TotalLength);
    
    return;
}

CFE_TIME_SysTime_t CFE_SB_GetMsgTime (CFE_SB_MsgPtr_t MsgPtr)
{
    CFE_TIME_SysTime_t  Time;

    /* Can't specify return value - ReturnCodeTable can only store integer values */

    /* Check for Function Hook */
    if (Ut_CFE_SB_HookTable.CFE_SB_GetMsgTime)
        return Ut_CFE_SB_HookTable.CFE_SB_GetMsgTime(MsgPtr);
        
    Time.Seconds    =  0;
    Time.Subseconds =  0;    
    
    return Time;
}

int32 CFE_SB_SetMsgTime (CFE_SB_MsgPtr_t MsgPtr, CFE_TIME_SysTime_t time)
{
    /* Check for specified return */
    if (Ut_CFE_SB_UseReturnCode(UT_CFE_SB_SETMSGTIME_INDEX))
        return Ut_CFE_SB_ReturnCodeTable[UT_CFE_SB_SETMSGTIME_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_CFE_SB_HookTable.CFE_SB_SetMsgTime)
        return Ut_CFE_SB_HookTable.CFE_SB_SetMsgTime(MsgPtr, time);

    return CFE_SUCCESS;
}

void CFE_SB_TimeStampMsg (CFE_SB_MsgPtr_t MsgPtr)
{
    /* Can't specify return value - this is a void function */

    /* Check for Function Hook */
    if (Ut_CFE_SB_HookTable.CFE_SB_TimeStampMsg)
        Ut_CFE_SB_HookTable.CFE_SB_TimeStampMsg(MsgPtr);

    return;
}

uint16 CFE_SB_GetCmdCode (CFE_SB_MsgPtr_t MsgPtr)
{
    /* Check for specified return */
    if (Ut_CFE_SB_UseReturnCode(UT_CFE_SB_GETCMDCODE_INDEX))
        return Ut_CFE_SB_ReturnCodeTable[UT_CFE_SB_GETCMDCODE_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_CFE_SB_HookTable.CFE_SB_GetCmdCode)
        return Ut_CFE_SB_HookTable.CFE_SB_GetCmdCode(MsgPtr);

    return CFE_SUCCESS;
}

int32 CFE_SB_SetCmdCode (CFE_SB_MsgPtr_t MsgPtr, uint16 CmdCode)
{
    /* Check for specified return */
    if (Ut_CFE_SB_UseReturnCode(UT_CFE_SB_SETCMDCODE_INDEX))
        return Ut_CFE_SB_ReturnCodeTable[UT_CFE_SB_SETCMDCODE_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_CFE_SB_HookTable.CFE_SB_SetCmdCode)
        return Ut_CFE_SB_HookTable.CFE_SB_SetCmdCode(MsgPtr,CmdCode);

    return CFE_SUCCESS;
}

uint16 CFE_SB_GetChecksum(CFE_SB_MsgPtr_t MsgPtr)
{
    /* Check for specified return */
    if (Ut_CFE_SB_UseReturnCode(UT_CFE_SB_GETCHECKSUM_INDEX))
        return Ut_CFE_SB_ReturnCodeTable[UT_CFE_SB_GETCHECKSUM_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_CFE_SB_HookTable.CFE_SB_GetChecksum)
        return Ut_CFE_SB_HookTable.CFE_SB_GetChecksum(MsgPtr);

    return CFE_SUCCESS;
}

void CFE_SB_GenerateChecksum(CFE_SB_MsgPtr_t MsgPtr)
{
    /* Can't specify return value - this is a void function */

    /* Check for Function Hook */
    if (Ut_CFE_SB_HookTable.CFE_SB_GenerateChecksum)
        Ut_CFE_SB_HookTable.CFE_SB_GenerateChecksum(MsgPtr);

    return;
}

boolean CFE_SB_ValidateChecksum(CFE_SB_MsgPtr_t MsgPtr)
{
    /* Check for specified return */
    if (Ut_CFE_SB_UseReturnCode(UT_CFE_SB_VALIDATECHECKSUM_INDEX))
        return Ut_CFE_SB_ReturnCodeTable[UT_CFE_SB_VALIDATECHECKSUM_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_CFE_SB_HookTable.CFE_SB_ValidateChecksum)
        return Ut_CFE_SB_HookTable.CFE_SB_ValidateChecksum(MsgPtr);

    return(TRUE);
}

int32 CFE_SB_CleanUpApp (uint32 AppId)
{
    /* Check for specified return */
    if (Ut_CFE_SB_UseReturnCode(UT_CFE_SB_CLEANUPAPP_INDEX))
        return Ut_CFE_SB_ReturnCodeTable[UT_CFE_SB_CLEANUPAPP_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_CFE_SB_HookTable.CFE_SB_CleanUpApp)
        return Ut_CFE_SB_HookTable.CFE_SB_CleanUpApp(AppId);

    return CFE_SUCCESS;
}
