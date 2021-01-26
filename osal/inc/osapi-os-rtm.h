#ifndef _OSAPI_OS_RTM_H_
#define _OSAPI_OS_RTM_H_


typedef enum
{
	OS_RUNTIME_MODE_REALTIME 				= 0,
	OS_RUNTIME_MODE_NONREALTIME				= 1
} OS_RunTimeModeEnum_t;

/************************************************************************/
/** \brief TODO
**
**  \par Description
**       None
**
**  \par Assumptions, External Events, and Notes:
**       None
**
*************************************************************************/
void   OS_RtmEndFrame(void);

/************************************************************************/
/** \brief TODO
**
**  \par Description
**       None
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]   
**  
**  \returns
**  \retcode #OS_SUCCESS  \retdesc \copydoc OS_SUCCESS \endcode
**  \retcode #OS_ERROR  \retdesc \copydoc OS_ERROR \endcode
**  \endreturns
**
*************************************************************************/
int32  OS_RtmEngageStepMode(void);

/************************************************************************/
/** \brief TODO
**
**  \par Description
**       None
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]   
**  
**  \returns
**  \retcode #OS_SUCCESS  \retdesc \copydoc OS_SUCCESS \endcode
**  \retcode #OS_ERROR  \retdesc \copydoc OS_ERROR \endcode
**  \endreturns
**
*************************************************************************/
OS_RunTimeModeEnum_t OS_RtmGetRunMode(void);

/************************************************************************/
/** \brief TODO
**
**  \par Description
**       None
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]   
**
*************************************************************************/
void   OS_RtmSetMajorAndMinorFrame(uint32 MajorFrameLength,
		uint32 MinorFramesPerMajorFrame);

/************************************************************************/
/** \brief TODO
**
**  \par Description
**       None
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]   
**  
**  \returns
**  \retcode #OS_SUCCESS  \retdesc \copydoc OS_SUCCESS \endcode
**  \retcode #OS_ERROR  \retdesc \copydoc OS_ERROR \endcode
**  \endreturns
**
*************************************************************************/
uint32 OS_RtmGetCurrentMinorFrame(void);

#endif
