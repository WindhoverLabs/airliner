/*=======================================================================================
** File Name:  io_lib_events.h
**
** Copyright 2017 United States Government as represented by the Administrator
** of the National Aeronautics and Space Administration.  No copyright is
** claimed in the United States under Title 17, U.S. Code.
** All Other Rights Reserved.
**
** Title:  ID Header File for IO_LIB Application
**
** $Author:    Guy de Carufel
** $Revision:  $
** $Date:      2015-01-09
**
** Purpose:  This header file contains definitions of IO_LIB Events.
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2015-01-09 | Guy de Carufel | Build #: Code Started
**
**=====================================================================================*/
    
#ifndef _IO_LIB_EVENTS_H_
#define _IO_LIB_EVENTS_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Event IDs */
typedef enum
{
    IO_LIB_INF_EID                  = 201,
    IO_LIB_ERR_EID                  = 202,
    IO_LIB_FORMAT_INF_EID           = 203,
    IO_LIB_SERVICE_INF_EID          = 204,
    IO_LIB_FORMAT_ERR_EID           = 205,
    IO_LIB_SERVICE_ERR_EID          = 206,
    IO_LIB_TRANS_UDP_EID            = 207,
    IO_LIB_TRANS_RS422_EID          = 208,
    IO_LIB_TRANS_SELECT_EID         = 209,
    IO_LIB_COP1_EID                 = 210,
    IO_LIB_TM_SDLP_EID              = 211,
    IO_LIB_EVT_CNT 
} IO_LIB_Events_t;

#ifdef __cplusplus
}
#endif

#endif 

/*=======================================================================================
** End of file io_lib_events.h
**=====================================================================================*/
    
