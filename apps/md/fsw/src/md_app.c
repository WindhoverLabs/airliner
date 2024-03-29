/*************************************************************************
** File:
**   $Id: md_app.c 1.6 2017/05/22 14:56:30EDT mdeschu Exp  $
**
**  Copyright (c) 2007-2014 United States Government as represented by the 
**  Administrator of the National Aeronautics and Space Administration. 
**  All Other Rights Reserved.  
**
**  This software was created at NASA's Goddard Space Flight Center.
**  This software is governed by the NASA Open Source Agreement and may be 
**  used, distributed and modified only pursuant to the terms of that 
**  agreement.
**
** Purpose: 
**   CFS Memory Dwell Application top-level procedures.
**
** 
*************************************************************************/

#include "cfe.h"
#include "md_app.h"
#include "md_events.h"
#include "md_tbldefs.h"
#include "md_verify.h"
#include "md_cmds.h"
#include <string.h>
#include "md_dwell_tbl.h"
#include "md_dwell_pkt.h"
#include "md_utils.h"
#include "md_perfids.h"
#include "md_version.h"

MD_AppData_t MD_AppData;

/* Constant Data */

const MD_CmdHandlerTblRec_t MD_CmdHandlerTbl[] = {
/*   Message ID,    Command Code,            Msg Size,     Msg/Cmd/Terminator */
{  MD_CMD_MID,   MD_NOOP_CC,          sizeof(MD_NoArgsCmd_t),  MD_CMD_MSGTYPE},
{  MD_CMD_MID,   MD_RESET_CNTRS_CC,   sizeof(MD_NoArgsCmd_t),  MD_CMD_MSGTYPE},
{  MD_CMD_MID,   MD_START_DWELL_CC,   sizeof(MD_CmdStartStop_t),MD_CMD_MSGTYPE},
{  MD_CMD_MID,   MD_STOP_DWELL_CC,    sizeof(MD_CmdStartStop_t),MD_CMD_MSGTYPE},
{  MD_CMD_MID,   MD_JAM_DWELL_CC,     sizeof(MD_CmdJam_t),     MD_CMD_MSGTYPE},
#if MD_SIGNATURE_OPTION == 1   
{  MD_CMD_MID,   MD_SET_SIGNATURE_CC, sizeof(MD_CmdSetSignature_t), MD_CMD_MSGTYPE},
#endif
{             0,                   0,                 0,       MD_TERM_MSGTYPE}
};

void MD_AppMain ( void )
{
   CFE_SB_MsgId_t     MessageID;
   int32              Status;
   uint8              TblIndex;
   boolean            IsRegistered;

   /* 
   ** Register the Application with Executive Services 
   */
   Status = CFE_ES_RegisterApp();
   if(Status != CFE_SUCCESS)
   {
      IsRegistered = FALSE;
      CFE_ES_WriteToSysLog
              ("MD_APP: Call to CFE_ES_RegisterApp Failed:RC=0x%08X\n",(unsigned int)Status);
      MD_AppData.RunStatus = CFE_ES_APP_ERROR;
   }/* end if */
   else
   {  /* Successfully registered */
      IsRegistered = TRUE;
      MD_AppData.RunStatus = CFE_ES_APP_RUN;
      
      /* Create the first Performance Log entry */
      CFE_ES_PerfLogEntry(MD_APPMAIN_PERF_ID);  
         
      /* Initialize the task */
      Status = MD_AppInit();

      if (Status != CFE_SUCCESS) 
      {
         CFE_ES_WriteToSysLog("MD:Application Init Failed,RC=0x%08X\n", (unsigned int)Status);      
         MD_AppData.RunStatus = CFE_ES_APP_ERROR;
      }
   }
   

   /* Loop Forever, or until ES exit command, or SB error  */
   while ( CFE_ES_RunLoop(&MD_AppData.RunStatus) == TRUE) 
   {
      /* Copy any newly loaded tables */
      for (TblIndex=0; TblIndex < MD_NUM_DWELL_TABLES; TblIndex++)
      {         
             
         MD_ManageDwellTable(TblIndex);
         
      } /* end for each table loop */

      CFE_ES_PerfLogExit(MD_APPMAIN_PERF_ID); 
      
      /* Wait for next message */
      Status =   CFE_SB_RcvMsg (&MD_AppData.MsgPtr, MD_AppData.CmdPipe, CFE_SB_PEND_FOREVER );

      if (Status != CFE_SUCCESS)
      {
         /* 
         ** Exit on pipe read error
         */
         CFE_EVS_SendEvent(MD_PIPE_ERR_EID, CFE_EVS_ERROR,
                    "SB Pipe Read Error, App will exit. Pipe Return Status = 0x%08X", (unsigned int)Status);         
          
         MD_AppData.RunStatus = CFE_ES_APP_ERROR;
          
      }

      CFE_ES_PerfLogEntry(MD_APPMAIN_PERF_ID);         

      /* Process Executive Request */
      if(Status == CFE_SUCCESS)
      {
        MessageID = CFE_SB_GetMsgId(MD_AppData.MsgPtr);
                 
        switch(MessageID)
        {
            case MD_WAKEUP_MID:
            
                if (CFE_SB_GetTotalMsgLength(MD_AppData.MsgPtr) != sizeof(MD_NoArgsCmd_t))
                {
                    CFE_EVS_SendEvent( MD_MSG_LEN_ERR_EID, CFE_EVS_ERROR,
                                      "Msg with Bad length Rcvd: ID = 0x%04X, Exp Len = %d, Len = %d",
                                       MessageID,  
                                       sizeof(MD_NoArgsCmd_t), 
                                       CFE_SB_GetTotalMsgLength(MD_AppData.MsgPtr));
                }
                else
                {
                    MD_DwellLoop();
                }
                break ;

            case MD_CMD_MID:
            
                MD_ExecRequest( MD_AppData.MsgPtr );
                break ;

            case MD_SEND_HK_MID:
            
                if (CFE_SB_GetTotalMsgLength(MD_AppData.MsgPtr) != sizeof(MD_NoArgsCmd_t))
                {
                    CFE_EVS_SendEvent( MD_MSG_LEN_ERR_EID, CFE_EVS_ERROR,
                                      "Msg with Bad length Rcvd: ID = 0x%04X, Exp Len = %d, Len = %d",
                                       MessageID,  
                                       sizeof(MD_NoArgsCmd_t), 
                                       CFE_SB_GetTotalMsgLength(MD_AppData.MsgPtr));
                }
                else
                {
                    MD_HkStatus();
                }
                break;

            default:
                CFE_EVS_SendEvent(MD_MID_ERR_EID, CFE_EVS_ERROR, 
                    "Msg with Invalid message ID Rcvd -- ID = 0x%04X",
                     MessageID );
                break;
        }
    }
        
        
   }  /* end while forever loop */

   
   /*
   ** Performance Log Exit Stamp.
   */
   if (IsRegistered == TRUE)
   {
      CFE_ES_PerfLogExit(MD_APPMAIN_PERF_ID); 
   }

    /* 
    ** Exit the Application 
    */
    CFE_ES_ExitApp(MD_AppData.RunStatus);


} /* End of MD_AppMain */


/******************************************************************************/

int32 MD_AppInit( void )
{
    /*
    ** Locals
    */
    int32   Status; 
    
    /* Initialize local control structures */
    MD_InitControlStructures();
    
    /* Register for event services...*/
    Status = CFE_EVS_Register(NULL, 0, CFE_EVS_NO_FILTER);
    
    if(Status != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog
                 ("MD_APP:Call to CFE_EVS_Register Failed:RC=0x%08X\n", (unsigned int)Status);
    }/* end if */

    /*
    ** Set up for Software Bus Services
    */
    if(Status == CFE_SUCCESS)
    {
        Status = MD_InitSoftwareBusServices();
    }
        
    /*
    ** Register for Table Services
    */
    if(Status == CFE_SUCCESS)
    {
        Status = MD_InitTableServices(  );
        
    }/* end if */

    /*
    ** Issue Event Message
    */
    if(Status == CFE_SUCCESS)
    {
        Status = CFE_EVS_SendEvent (MD_INIT_INF_EID, CFE_EVS_INFORMATION,
               "MD Initialized.  Version %d.%d.%d.%d",
                MD_MAJOR_VERSION,
                MD_MINOR_VERSION, 
                MD_REVISION, 
                MD_MISSION_REV);               


    }/* end if */
    
    return (Status);
     
} /* End of MD_AppInit */

/******************************************************************************/
void MD_InitControlStructures(void)
{
    uint16  TblIndex;
    uint16  EntryIndex;
    MD_DwellPacketControl_t *DwellControlPtr;
    MD_DwellControlEntry_t  *DwellEntryPtr;
    
    for (TblIndex=0; TblIndex < MD_NUM_DWELL_TABLES; TblIndex++)
    {
        DwellControlPtr = &MD_AppData.MD_DwellTables[TblIndex];
        DwellControlPtr->Enabled = 0;    
        DwellControlPtr->AddrCount = 0;      
        DwellControlPtr->Rate = 0; 
        /* Countdown must be set to 1 since it's decremented at the top of */
        /* the dwell loop. */           
        DwellControlPtr->Countdown = 1;      
	    DwellControlPtr->PktOffset = 0;    
	    DwellControlPtr->CurrentEntry = 0; 
	    DwellControlPtr->DataSize = 0;        
        DwellControlPtr->Filler = 0;    
        
        for (EntryIndex = 0; EntryIndex < MD_DWELL_TABLE_SIZE; EntryIndex++)
        {
            DwellEntryPtr = &DwellControlPtr->Entry[EntryIndex];
            DwellEntryPtr->Length = 0;      
	        DwellEntryPtr->Delay = 0;          
            DwellEntryPtr->ResolvedAddress = 0; 
        } /* end for EntryIndex loop */
        
#if MD_SIGNATURE_OPTION == 1   
        strncpy(DwellControlPtr->Signature,"", MD_SIGNATURE_FIELD_LENGTH);          
#endif

    } /* end for TblIndex loop */
    
    return;
}
/******************************************************************************/
int32 MD_InitSoftwareBusServices( void )
{
    int32    Status;
    uint16   TblIndex;

   /*
   ** Initialize housekeeping telemetry packet (clear user data area) 
   */
   CFE_SB_InitMsg(&MD_AppData.HkPkt, MD_HK_TLM_MID, MD_HK_TLM_LNGTH, TRUE);
    
   /*
   ** Initialize dwell packets (clear user data area) 
   */
   for (TblIndex = 0; TblIndex < MD_NUM_DWELL_TABLES; TblIndex++)
   {
       CFE_SB_InitMsg(&MD_AppData.MD_DwellPkt[TblIndex], 
                      (CFE_SB_MsgId_t) MD_DWELL_PKT_MID_BASE + TblIndex, 
                       MD_DWELL_PKT_LNGTH, /* this is max pkt size */
                       TRUE); /* clear data area and set seq count to zero. */
                       
#if MD_SIGNATURE_OPTION == 1   
       MD_AppData.MD_DwellPkt[TblIndex].Signature[0]='\0';
#endif
   }

    /* 
    **  Create Software Bus message pipe 
    */
    Status = CFE_SB_CreatePipe(&MD_AppData.CmdPipe,
                                MD_PIPE_DEPTH,MD_PIPE_NAME);
    
    if(Status != CFE_SUCCESS)
    {
       CFE_ES_WriteToSysLog("MD_APP: Error creating cmd pipe:RC=0x%08X\n",
                            (unsigned int)Status);
    }/* end if */                                                                


    /*  
    ** Subscribe to Housekeeping request commands 
    */
    if(Status == CFE_SUCCESS)
    {

        Status = CFE_SB_Subscribe(MD_SEND_HK_MID, MD_AppData.CmdPipe);
            
        if(Status != CFE_SUCCESS)
        {
            CFE_ES_WriteToSysLog
               ("MD_APP: Error subscribing to HK Request:RC=0x%08X\n",
                (unsigned int)Status);
        }/* end if */

    }/* end if */                                                                
    

    /*  
    ** Subscribe to MD ground command packets
    */
    if(Status == CFE_SUCCESS)
    {

        Status = CFE_SB_Subscribe(MD_CMD_MID,MD_AppData.CmdPipe);
            
        if(Status != CFE_SUCCESS)
        {
            CFE_ES_WriteToSysLog
                  ("MD_APP:Error subscribing to gnd cmds:RC=0x%08X\n",
                   (unsigned int)Status);
        }/* end if */

    }/* end if */
        

    /* 
    ** Subscribe to MD wakeup packets
    */
    if(Status == CFE_SUCCESS)
    {

        Status = CFE_SB_Subscribe(MD_WAKEUP_MID,MD_AppData.CmdPipe);
            
        if(Status != CFE_SUCCESS)
        {
            CFE_ES_WriteToSysLog
                ("MD_APP:Error subscribing to wakeup message:RC=0x%08X\n",
                 (unsigned int)Status);
        }/* end if */

    }/* end if */

    
    return (Status);

} /* End of MD_InitSoftwareBusServices */

/******************************************************************************/

int32 MD_InitTableServices( void )
{
    int32                   Status; 
    int32                   GetAddressResult; 
    uint8                   TblIndex;
    boolean                 RecoveredValidTable;        /* for current table */
    boolean                 TableInitValidFlag = TRUE;  /* for all tables so far*/
    MD_DwellTableLoad_t     InitMemDwellTable;
    MD_DwellTableLoad_t*    MD_LoadTablePtr = 0; 
    uint16                  TblRecos = 0; /* Number of Tables Recovered */
    uint16                  TblInits = 0; /* Number of Tables Initialized */
    char                    TblFileName[OS_MAX_PATH_LEN];

    /*  Prepare Data Structure used for loading Initial Table Data    */
    
    CFE_PSP_MemSet(&InitMemDwellTable,0,sizeof(MD_DwellTableLoad_t)); 

    /* 
    ** For each table, load recovered data if available and valid.
    ** Otherwise, load initial data.
    */

    for ( TblIndex = 0; 
          (TblIndex < MD_NUM_DWELL_TABLES) && (TableInitValidFlag == TRUE); 
          TblIndex++)
    {
        RecoveredValidTable = FALSE;
        
        /* Prepare Table Name */
        snprintf(MD_AppData.MD_TableName[TblIndex], 
                 CFE_TBL_MAX_NAME_LENGTH + 1, /* allows total of CFE_TBL_MAX_NAME_LENGTH characters to be copied */
                 "%s%d", MD_DWELL_TABLE_BASENAME, TblIndex + 1); 

        snprintf(TblFileName, 
                 OS_MAX_PATH_LEN, /* allows total of CFE_TBL_MAX_NAME_LENGTH characters to be copied */
                 MD_TBL_FILENAME_FORMAT, TblIndex + 1); 
        
        /* Register Dwell Table #tblnum */
        Status = CFE_TBL_Register(
                &MD_AppData.MD_TableHandle[TblIndex],  /* Table Handle (to be returned) */
                MD_AppData.MD_TableName[TblIndex],     /*   Table Name */
                sizeof(MD_DwellTableLoad_t),           /* Size of Table being Registered */
                CFE_TBL_OPT_DEFAULT | CFE_TBL_OPT_CRITICAL,  
                &MD_TableValidationFunc);              /* Ptr to table validation function */
      
        /* Check for recovered table data */
        if (Status == CFE_TBL_INFO_RECOVERED_TBL)
        {
            /* Restore dwell table contents from CDS */
                    
            GetAddressResult = CFE_TBL_GetAddress ( (void *) &MD_LoadTablePtr,  
                                            MD_AppData.MD_TableHandle[TblIndex]);
                    
            if(GetAddressResult != CFE_TBL_INFO_UPDATED)
            {
                CFE_EVS_SendEvent(MD_NO_TBL_COPY_ERR_EID, CFE_EVS_ERROR, 
"Didn't update MD tbl #%d due to unexpected CFE_TBL_GetAddress return: 0x%08X", 
                        TblIndex+1, (unsigned int)GetAddressResult); 
            }

            else  /* GetAddressResult == CFE_TBL_INFO_UPDATED*/
            {
                if(MD_TableValidationFunc((void*)MD_LoadTablePtr) ==
                    CFE_SUCCESS)
                {
                    MD_CopyUpdatedTbl( MD_LoadTablePtr, TblIndex);
                    
                    if (MD_LoadTablePtr->Enabled == MD_DWELL_STREAM_ENABLED)
                    {
                       MD_StartDwellStream ( (uint16) TblIndex);
                    }

                    RecoveredValidTable=TRUE;
                    TblRecos++;
                    CFE_EVS_SendEvent(MD_RECOVERED_TBL_VALID_INF_EID, CFE_EVS_INFORMATION, 
                       "Recovered Dwell Table #%d is valid and has been copied to the MD App", 
                        TblIndex+1); 
                }
                else
                {   
                    CFE_EVS_SendEvent(MD_RECOVERED_TBL_NOT_VALID_ERR_EID, CFE_EVS_ERROR, 
                       "MD App will reinitialize Dwell Table #%d because recovered table is not valid", 
                        TblIndex+1); 
                }
                
            } /* end if(GetAddressResult != CFE_TBL_INFO_UPDATED) */
            
            /* Unlock Table */
            CFE_TBL_ReleaseAddress(MD_AppData.MD_TableHandle[TblIndex] );

        } /* end if Register indicates recovered data */

        else if (Status == CFE_TBL_ERR_INVALID_SIZE)
        {
            CFE_EVS_SendEvent (MD_DWELL_TBL_TOO_LARGE_CRIT_EID, CFE_EVS_CRITICAL,
                              "Dwell Table(s) are too large to register: %d > %d bytes, %d > %d entries",
                               MD_TBL_LOAD_LNGTH, CFE_TBL_MAX_SNGL_TABLE_SIZE,
                               MD_DWELL_TABLE_SIZE, 
                              (uint16) ((CFE_TBL_MAX_SNGL_TABLE_SIZE - sizeof(uint32) ) / sizeof (MD_TableLoadEntry_t)));
            TableInitValidFlag = FALSE;
        }   /* end if */
        
        else if (Status != CFE_SUCCESS)
        {
            CFE_EVS_SendEvent (MD_TBL_REGISTER_CRIT_EID, CFE_EVS_CRITICAL,
                              "CFE_TBL_Register error 0x%08X received for tbl#%d",
                               (unsigned int)Status,TblIndex+1);
            TableInitValidFlag = FALSE;
        }   /* end if */

        /* 
        ** Load initial values if needed 
        */
        if((RecoveredValidTable == FALSE) && (TableInitValidFlag == TRUE))
        {
            Status = CFE_TBL_Load(MD_AppData.MD_TableHandle[TblIndex],  
            CFE_TBL_SRC_FILE,    /*  following ptr is memory ptr */
            (const void *) TblFileName);    /* Pointer to data to be loaded */
             
            MD_AppData.MD_DwellTables[TblIndex].Enabled  = MD_DWELL_STREAM_DISABLED;
           
            if(Status != CFE_SUCCESS)
            {
                CFE_ES_WriteToSysLog
                ("MD_APP: Error 0x%08X received loading tbl#%d\n",
                              (unsigned int)Status,TblIndex+1);
                TableInitValidFlag = FALSE;
            }/* end if */
            else
            {
                TblInits++;
            }

        }
        
        if( TableInitValidFlag == TRUE)
        {
            /* Update Dwell Table Control Info, including rate */
            MD_UpdateDwellControlInfo(TblIndex);
        }

    }  /* end for loop */

    /* Output init and recovery event message */
    CFE_EVS_SendEvent(MD_TBL_INIT_INF_EID, CFE_EVS_INFORMATION, 
      "Dwell Tables Recovered: %d, Dwell Tables Initialized: %d", 
      TblRecos, TblInits); 

        
    if (TableInitValidFlag == TRUE)
    {
       return CFE_SUCCESS;
    }
    else
    {
       return Status;
    }

} /* End of MD_InitTableServices */



/******************************************************************************/
int32 MD_ManageDwellTable (uint8 TblIndex)
{
    int32                Status = CFE_SUCCESS;
    int32                GetAddressResult; 
    boolean              FinishedManaging = FALSE;
    MD_DwellTableLoad_t *MD_LoadTablePtr = 0; 

    while (!FinishedManaging)
    {
        /* Determine if the table has a pending validation or update */
        Status = CFE_TBL_GetStatus( MD_AppData.MD_TableHandle[TblIndex]);


        if (Status == CFE_TBL_INFO_VALIDATION_PENDING)
        {
            /* Validate the specified Table */
            Status = CFE_TBL_Validate(MD_AppData.MD_TableHandle[TblIndex]);

            if (Status != CFE_SUCCESS)
            {
                /* If an error occurred during Validate, */
                /* then do not perform any more managing */
                FinishedManaging = TRUE;
            }
        }
        else if (Status == CFE_TBL_INFO_UPDATE_PENDING)
        {
            Status = CFE_TBL_Update ( MD_AppData.MD_TableHandle[TblIndex]); 
            
            /* If table was updated, copy contents to local structure */
            if (Status == CFE_SUCCESS)
            {    
               GetAddressResult = CFE_TBL_GetAddress ( (void *) &MD_LoadTablePtr,  
                                            MD_AppData.MD_TableHandle[TblIndex]);
                                   
               if(GetAddressResult == CFE_TBL_INFO_UPDATED)
               {
                  MD_CopyUpdatedTbl(MD_LoadTablePtr, TblIndex);
                  
                  if (MD_LoadTablePtr->Enabled == MD_DWELL_STREAM_ENABLED)
                  {
                     MD_StartDwellStream ( (uint16) TblIndex);
                  }
               }
               else
               {   
                  CFE_EVS_SendEvent(MD_NO_TBL_COPY_ERR_EID, CFE_EVS_ERROR, 
"Didn't update MD tbl #%d due to unexpected CFE_TBL_GetAddress return: 0x%08X", 
                        TblIndex+1, (unsigned int)GetAddressResult); 
               }

               /* Unlock Table */
               CFE_TBL_ReleaseAddress(MD_AppData.MD_TableHandle[TblIndex] );
            }
            
            /* After an Update, always assume we are done */
            /* and return Update Status */
            FinishedManaging = TRUE;
        }
        else if((Status & CFE_SEVERITY_BITMASK) == CFE_SEVERITY_ERROR)  
        {
            CFE_EVS_SendEvent(MD_TBL_STATUS_ERR_EID, CFE_EVS_ERROR, 
                             "Received unexpected error 0x%08X from CFE_TBL_GetStatus for tbl #%d", 
                              (unsigned int)Status, TblIndex+1); 
            FinishedManaging = TRUE;
        }

        else
        {
            FinishedManaging = TRUE;
        }
    }

    return Status;

} /* End of MD_ManageDwellTable */

/******************************************************************************/


void MD_ExecRequest(CFE_SB_MsgPtr_t MessagePtr )
{
    uint16          CommandCode;
    CFE_SB_MsgId_t  MessageID;
    int16           CmdIndx;
    uint16          ActualLength;

    /* Extract Command Code and Message Id */
    CommandCode = CFE_SB_GetCmdCode(MessagePtr);
    MessageID   = CFE_SB_GetMsgId(MessagePtr);
    
    /* Find index which will be used to access ExpectedLength data */
    CmdIndx = MD_SearchCmdHndlrTbl(MessageID, CommandCode);
    
    if (CmdIndx < 0)
    {

        /* If match wasn't found in command info structure,            */
        /* issue an error event, increment error counter, and return.  */
        CFE_EVS_SendEvent  (MD_CC_NOT_IN_TBL_ERR_EID,  CFE_EVS_ERROR,
                    "Command Code %d not found in MD_CmdHandlerTbl structure", 
                     CommandCode);
        MD_AppData.ErrCounter++;
        return;
    }
    
    /* Get Command Length */
    ActualLength = CFE_SB_GetTotalMsgLength(MessagePtr);

    /* If Command  Length is inconsistent with command type, */
    /* send error event, increment error count, and return.  */
    if (ActualLength != MD_CmdHandlerTbl[CmdIndx].ExpectedLength)
    {
        CFE_EVS_SendEvent( MD_CMD_LEN_ERR_EID, CFE_EVS_ERROR,
           "Cmd Msg with Bad length Rcvd: ID = 0x%04X, CC = %d, Exp Len = %d, Len = %d",
            MessageID, CommandCode, 
            (int)MD_CmdHandlerTbl[CmdIndx].ExpectedLength, ActualLength);
                        
        MD_AppData.ErrCounter++;
        return;
    }


    /* Process command */
    switch(CommandCode)
    {
        case MD_NOOP_CC:
        
            CFE_EVS_SendEvent (MD_NOOP_INF_EID, CFE_EVS_INFORMATION,
            "No-op command, Version %d.%d.%d.%d",
             MD_MAJOR_VERSION,
             MD_MINOR_VERSION, 
             MD_REVISION, 
             MD_MISSION_REV);

            MD_AppData.CmdCounter++;
            break;

        case MD_RESET_CNTRS_CC:
      
            CFE_EVS_SendEvent  (MD_RESET_CNTRS_DBG_EID,  CFE_EVS_DEBUG,
                                "Reset Counters Cmd Received");
            MD_AppData.CmdCounter   = 0;
            MD_AppData.ErrCounter   = 0;
            break;

        case MD_START_DWELL_CC: 
        
            MD_ProcessStartCmd(MessagePtr);                
            break;

        case MD_STOP_DWELL_CC: /* Params: byte:table_ID */
        
            MD_ProcessStopCmd(MessagePtr);                
            break;
            
        case MD_JAM_DWELL_CC: /* Params: table_ID, AddrIndex, address, */
                           /* FieldLength, DwellDelay */
            MD_ProcessJamCmd(MessagePtr);
            break;

#if MD_SIGNATURE_OPTION == 1   
        case MD_SET_SIGNATURE_CC:
            MD_ProcessSignatureCmd(MessagePtr);
            break;
#endif

        default:
            /* unknown function code specified - send error event message */
            CFE_EVS_SendEvent(MD_CC_NOT_IN_LOOP_ERR_EID, CFE_EVS_ERROR,
               "Command Code %d not found in command processing loop",
                CommandCode);
            MD_AppData.ErrCounter++;
            break;

    } /* End Switch */
    
    return;
    
} /* End of MD_ExecRequest */


/******************************************************************************/
void MD_HkStatus()
{
    uint8                        TblIndex = 0;
    uint16                       MemDwellEnableBits;
    MD_HkTlm_t                  *HkPktPtr;
    MD_DwellPacketControl_t     *ThisDwellTablePtr;
    
    /* Assign pointer used as shorthand to access Housekeeping Packet fields */
    HkPktPtr = &MD_AppData.HkPkt;
    
    /* Assign values for command counters */
    
    HkPktPtr->ValidCmdCntr   = MD_AppData.CmdCounter;
    HkPktPtr->InvalidCmdCntr = MD_AppData.ErrCounter;

    MemDwellEnableBits = 0;

    for (TblIndex=0; TblIndex<MD_NUM_DWELL_TABLES; TblIndex++)
    {      
        /* 
        **  Prepare the DwellEnabledMask showing which dwell streams are enabled
        */

        /*   The dwell enabled bits are set in the following bit pattern:
         0x0001 = table 1 enabled, 0x0002 = table 2 enabled
         0x0004 = table 3 enabled, 0x0008 = table 4 enabled, 
         0x0010 = table 5 enabled, etc.
        */
        MemDwellEnableBits |= (MD_AppData.MD_DwellTables[TblIndex].Enabled << (TblIndex) );

        /* Assign pointer used as shorthand to access current dwell tbl data */
        ThisDwellTablePtr = &MD_AppData.MD_DwellTables[TblIndex];
             
        /*
        **  Insert current values for each dwell stream
        */
        HkPktPtr->DwellTblAddrCount[TblIndex] = ThisDwellTablePtr->AddrCount;
        HkPktPtr->NumWaitsPerPkt[TblIndex]    = ThisDwellTablePtr->Rate;
        HkPktPtr->DwellPktOffset[TblIndex]    = ThisDwellTablePtr->PktOffset;
        HkPktPtr->ByteCount[TblIndex]         = ThisDwellTablePtr->DataSize;
        HkPktPtr->DwellTblEntry[TblIndex]     = ThisDwellTablePtr->CurrentEntry;
        HkPktPtr->Countdown[TblIndex]         = ThisDwellTablePtr->Countdown;
    }
    
    HkPktPtr->DwellEnabledMask = MemDwellEnableBits;

    /*
    ** Send housekeeping telemetry packet...
    */
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *) HkPktPtr);
    CFE_SB_SendMsg((CFE_SB_Msg_t *) HkPktPtr);
    
    return;
    
} /* End of MD_HkStatus */


/******************************************************************************/

int16 MD_SearchCmdHndlrTbl( CFE_SB_MsgId_t MessageID, uint16 CommandCode )
{
    int16     TblIndx = -1; /* need index to be 0 after it is incremented for 1st time */
    boolean   MatchedMsgId = FALSE;
    boolean   FoundMatch = FALSE;

    do
    {
        /* Point to next entry in Command Handler Table */
        TblIndx++;

        /* Check to see if we found a matching Message ID */
        if ((MD_CmdHandlerTbl[TblIndx].MsgId == MessageID) &&
            (MD_CmdHandlerTbl[TblIndx].MsgTypes != MD_TERM_MSGTYPE))
        {   /* MessageID matches and this isn't last Table entry */
        
            /* Flag any found message IDs so that if there's an error, we can */
            /* determine if it was a bad message ID or bad command code */
            MatchedMsgId = TRUE;

            /* If entry in the Command Handler Table is a command entry, */
            /* then check for a matching command code                    */
            if (MD_CmdHandlerTbl[TblIndx].MsgTypes == MD_CMD_MSGTYPE)
            {
                if (MD_CmdHandlerTbl[TblIndx].CmdCode == CommandCode)
                {
                    /* Found matching message ID and Command Code */
                    FoundMatch = TRUE;
                }
            }
            else 
            /* Message is not a command message with specific command code */
            {
                /* Matching Message ID is all that is required      */
                FoundMatch = TRUE;
            }
        }
                

    } while ((!FoundMatch) && 
             (MD_CmdHandlerTbl[TblIndx].MsgTypes != MD_TERM_MSGTYPE));

    /* If we failed to find a match, return a negative index */
    if (!FoundMatch)
    {
        /* Determine if the message ID was bad or the command code */
        if (MatchedMsgId)
        {
            /* A matching message ID was found, so command code must be bad */
            TblIndx = MD_BAD_CMD_CODE;
        }
        else /* No matching message ID was found */
        {
            TblIndx = MD_BAD_MSG_ID;
        }
    }

    return TblIndx;
}   /* End of MD_SearchCmdHndlrTbl() */



/************************/
/*  End of File Comment */
/************************/

