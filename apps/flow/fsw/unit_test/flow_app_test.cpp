/****************************************************************************
*
*   Copyright (c) 2017 Windhover Labs, L.L.C. All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
*
* 1. Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in
*    the documentation and/or other materials provided with the
*    distribution.
* 3. Neither the name Windhover Labs nor the names of its
*    contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
* COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
* OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
* AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
* ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*
*****************************************************************************/

#include "cfe.h"
#include "flow_app.h"
#include "flow_test_utils.h"
#include "flow_app_test.h"
#include "uttest.h"
#include "ut_osapi_stubs.h"
#include "ut_cfe_sb_stubs.h"
#include "ut_cfe_sb_hooks.h"
#include "ut_cfe_es_stubs.h"
#include "ut_cfe_es_hooks.h"
#include "ut_cfe_evs_stubs.h"
#include "ut_cfe_evs_hooks.h"
#include "ut_cfe_time_stubs.h"
#include "ut_cfe_psp_memutils_stubs.h"
#include "ut_cfe_tbl_stubs.h"
#include "ut_cfe_fs_stubs.h"
#include "ut_cfe_time_stubs.h"
#include <unistd.h>

int32 hookCalledCount = 0;
int16 DESC_SIZE = 80;
/**************************************************************************
 * Tests for FLOW_InitEvent()
 **************************************************************************/

/**
 * Test FLOW_InitEvent() with failed CFE_EVS_Register
 */
void Test_FLOW_InitEvent_Fail_Register(void)
{
    FLOW oFLOW;

    /* Set a fail result for EVS */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_EVENTS_SERVICE | CFE_EVS_NOT_IMPLEMENTED;
    int32 expected = CFE_EVS_APP_NOT_REGISTERED;

    Ut_CFE_EVS_SetReturnCode(UT_CFE_EVS_REGISTER_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oFLOW.InitEvent();

    /* Verify results */
    UtAssert_True (result == expected, "InitEvent, failed EVS Register");
}


/**************************************************************************
 * Tests for FLOW_InitPipe()
 **************************************************************************/
/**
 * Test FLOW_InitPipe(), fail SCH CFE_SB_CreatePipe
 */
void Test_FLOW_InitPipe_Fail_CreateSCHPipe(void)
{
    FLOW oFLOW;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oFLOW.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail SB create SCH pipe");
}


/**
 * Test FLOW_InitPipe(), fail CFE_SB_SubscribeEx for wakeup
 */
void Test_FLOW_InitPipe_Fail_SubscribeWakeup(void)
{
    FLOW oFLOW;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oFLOW.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail CFE_SB_SubscribeEx for wakeup");
}


/**
 * Test FLOW_InitPipe(), fail CFE_SB_SubscribeEx for sendhk
 */
void Test_FLOW_InitPipe_Fail_SubscribeSendHK(void)
{
    FLOW oFLOW;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 2);

    /* Execute the function being tested */
    result = oFLOW.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail CFE_SB_SubscribeEx for sendhk");
}


/**
 * Test FLOW_InitPipe(), fail CMD CFE_SB_CreatePipe
 */
void Test_FLOW_InitPipe_Fail_CreateCMDPipe(void)
{
    FLOW oFLOW;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 2);

    /* Execute the function being tested */
    result = oFLOW.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail SB create CMD pipe");
}


/**
 * Test FLOW_InitPipe(), fail CFE_SB_Subscribe for CMD msg
 */
void Test_FLOW_InitPipe_Fail_SubscribeCMD(void)
{
    FLOW oFLOW;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBE_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oFLOW.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail CFE_SB_Subscribe for CMD");
}


/**
 * Test FLOW_InitPipe(), fail DATA CFE_SB_CreatePipe
 */
void Test_FLOW_InitPipe_Fail_CreateDATAPipe(void)
{
    FLOW oFLOW;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 2);

    /* Execute the function being tested */
    result = oFLOW.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail SB create DATA pipe");
}


/**
 * Test FLOW_InitGyroPipe(), fail GYRO CFE_SB_CreatePipe
 */
void Test_FLOW_InitPipe_Fail_CreateGYROPipe(void)
{
	FLOW oFLOW;

	/* Set a fail result for SB */
	int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
				   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
	int32 expected = CFE_SB_BAD_ARGUMENT;

	 Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 1);

	 /* Execute the function being tested */
	 result = oFLOW.InitGyroPipe();

	 /* Verify results */
	 UtAssert_True (result == expected, "InitPipe, fail SB create GYRO pipe");
}


/**
 * Test FLOW_InitPipe(), fail CFE_SB_Subscribe for CMD msg
 */
void Test_FLOW_InitPipe_Fail_SubscribeGYRO(void)
{
    FLOW oFLOW;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oFLOW.InitGyroPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail CFE_SB_Subscribe for GYRO");
}


/**************************************************************************
 * Tests for FLOW_InitData()
 **************************************************************************/
/**
 * Test FLOW_InitData()
 */
void Test_FLOW_InitData(void)
{
    FLOW oFLOW;

    /* Set a fail result */
    int32 expected = CFE_SUCCESS;

    /* Execute the function being tested */
    oFLOW.InitData();

    /* Verify results */
    //UtAssert_True (result == expected, "InitData");
}


/**************************************************************************
 * Tests for FLOW_InitApp()
 **************************************************************************/
/**
 * Test FLOW_InitApp(), fail init event
 */
void Test_FLOW_InitApp_Fail_InitEvent(void)
{
    FLOW oFLOW;

    int32 result = CFE_SUCCESS;
    int32 expected = CFE_EVS_APP_NOT_REGISTERED;

    Ut_CFE_EVS_SetReturnCode(UT_CFE_EVS_REGISTER_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oFLOW.InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail init event");
}


/**
 * Test FLOW_InitApp(), fail init pipe
 */
void Test_FLOW_InitApp_Fail_InitPipe(void)
{
    FLOW oFLOW;

    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oFLOW.InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail init pipe");
}


/**
 * Test FLOW_InitApp(), fail init data.
 * NOTE: no current way to fail FLOW_InitData() in default
 */
void Test_FLOW_InitApp_Fail_InitData(void)
{
    FLOW oFLOW;

    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SUCCESS;

    /* Execute the function being tested */
    result = oFLOW.InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail init data");
}


/**
 * Test FLOW_InitApp(), fail init config table
 */
void Test_FLOW_InitApp_Fail_InitConfigTbl(void)
{
    FLOW oFLOW;

    int32 result = CFE_SUCCESS;
    int32 expected = CFE_TBL_ERR_INVALID_NAME;

    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_REGISTER_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oFLOW.InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail init config table");
}


/**
 * Test FLOW_InitApp(), Nominal
 */
void Test_FLOW_InitApp_Nominal(void)
{
    FLOW oFLOW;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_EXECUTIVE_SERVICE | CFE_ES_ERR_APP_REGISTER;
    int32 expected = CFE_SUCCESS;

    /* Execute the function being tested */
    result = oFLOW.InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, nominal");
}

/**************************************************************************
 * Tests for FLOW_AppMain()
 **************************************************************************/
/**
 * Test FLOW_AppMain(), Fail RegisterApp
 */
void Test_FLOW_AppMain_Fail_RegisterApp(void)
{
    FLOW oFLOW;

    /* fail the register app */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_REGISTERAPP_INDEX, CFE_ES_ERR_APP_REGISTER, 1);

    /* Execute the function being tested */
    oFLOW.AppMain();
}

/**
 * Test FLOW_AppMain(), Fail InitApp
 */
void Test_FLOW_AppMain_Fail_InitApp(void)
{
    FLOW oFLOW;

    /* fail the register app */
    Ut_CFE_EVS_SetReturnCode(UT_CFE_EVS_REGISTER_INDEX, CFE_EVS_APP_NOT_REGISTERED, 1);

    /* Execute the function being tested */
    oFLOW.AppMain();
}

/**
 * Test FLOW_AppMain(), Fail AcquireConfigPtrs
 */
void Test_FLOW_AppMain_Fail_AcquireConfigPtrs(void)
{
    FLOW oFLOW;

    /* fail the register app */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETADDRESS_INDEX, CFE_TBL_ERR_INVALID_HANDLE, 2);

    /* Execute the function being tested */
    oFLOW.AppMain();
}

/**
 * Test FLOW_AppMain(), Invalid Schedule Message
 */
void Test_FLOW_AppMain_InvalidSchMessage(void)
{
    FLOW oFLOW;

    /* The following will emulate behavior of receiving a SCH message to send HK */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, 0, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oFLOW.AppMain();

}

/**
 * Hook to support: FLOW_AppMain(), Nominal - SendHK
 */
int32 Test_FLOW_AppMain_Nominal_SendHK_SendMsgHook(CFE_SB_Msg_t *MsgPtr)
{
    /* TODO:  Test the contents of your HK message here. */

    hookCalledCount++;

    return CFE_SUCCESS;
}

/**
 * Test FLOW_AppMain(), Nominal - SendHK
 */
void Test_FLOW_AppMain_Nominal_SendHK(void)
{
    FLOW oFLOW;

    /* The following will emulate behavior of receiving a SCH message to WAKEUP */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FLOW_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Used to verify HK was transmitted correctly. */
    hookCalledCount = 0;
    Ut_CFE_ES_SetFunctionHook(UT_CFE_SB_SENDMSG_INDEX, (void*)&Test_FLOW_AppMain_Nominal_SendHK_SendMsgHook);

    /* Execute the function being tested */
    oFLOW.AppMain();

    /* Verify results */
    UtAssert_True (hookCalledCount == 1, "AppMain_Nominal_SendHK");

}

/**
 * Test FLOW_AppMain(), Nominal - Wakeup
 */
void Test_FLOW_AppMain_Nominal_Wakeup(void)
{
    FLOW oFLOW;

    /* The following will emulate behavior of receiving a SCH message to WAKEUP */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FLOW_WAKEUP_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    //oFLOW.AppMain();

}

/**
 * Test FLOW_AppMain(), Nominal - Image Received
 */
void Test_FLOW_AppMain_Nominal_Image_Received(void)
{
	FLOW oFLOW;
	oFLOW.InitApp();
	oFLOW.ConfigTblPtr->FLOW_MEAN_COUNT_THRESHOLD = 0;

	int boundry_min = 20;
	int boundry_max = 30;
	int baseImageIndex = 0;
	int baseImageMatrix[64][64] = {0};

	for(int i=0; i<64;i++)
	{
		for (int j=0;j<64;j++)
		{
			if(i>=boundry_min && i<=boundry_max && j>=boundry_min && j<=boundry_max && baseImageMatrix[i][j]!=255)
			{
				baseImageMatrix[i][j]=255;
			}

			oFLOW.CVT.OpticalFlowFrameMsg.Frame[baseImageIndex] = baseImageMatrix[i][j];
			baseImageIndex++;
		}
	}
	oFLOW.CVT.OpticalFlowFrameMsg.Timestamp = 1;
	oFLOW.CalculateOpticalFlow();

	UtAssert_True(oFLOW.HkTlm.FlowInitalized, "Optical flow function not initialized");

	/* Check for null pointers */
	UtAssert_True(&oFLOW.PreviousFrame!=nullptr,"PreviousFrame is a null pointer");
	UtAssert_True(&oFLOW.CVT.OpticalFlowFrameMsg!=nullptr,"OpticalFlowFrameMsg is a null pointer");

	int leftShiftImageMatrix[64][64] = {0};
	int shift = 5;
	int leftShiftImageIndex = 0;

	for(int i=0; i<64;i++)
	{
		for (int j=0;j<64;j++)
		{
			if(i>=boundry_min  && i<=boundry_max  && j>=boundry_min - shift  && j<=boundry_max -shift && leftShiftImageMatrix[i][j]!=255)
			{
				leftShiftImageMatrix[i][j]=255;
			}

			oFLOW.CVT.OpticalFlowFrameMsg.Frame[leftShiftImageIndex] = leftShiftImageMatrix[i][j];
			leftShiftImageIndex++;
		}
	}
	oFLOW.CVT.OpticalFlowFrameMsg.Timestamp = (1.0e6f / oFLOW.ConfigTblPtr->FLOW_DEFAULT_OUTPUT_RATE)+1+1;


	/* Check for valid timestamps*/
	UtAssert_True(&oFLOW.PreviousFrame.Timestamp>0,"PreviousFrame.Timestamp is less than zero");
	UtAssert_True(&oFLOW.CVT.OpticalFlowFrameMsg.Timestamp>0,"OpticalFlowFrameMsg.Timestamp is less than zero");

	/* Calculate instantaneous optical flow */
	uint32 q = oFLOW.CalculateOpticalFlow();
	UtAssert_True(q>0,"quality is invalid for test case.");
	UtAssert_True(oFLOW.HkTlm.PixelFlowX + shift == 0,"image did not shift left properly.");
	UtAssert_True(oFLOW.HkTlm.PixelFlowY == 0,"shifts along y axis is not expected.");

}

/**
 * Test FLOW_AppMain(), Nominal - Calculate Flow Left Shift
 */
void Test_FLOW_AppMain_Nominal_Calculate_Flow_Left_Shift(void)
{
	FLOW oFLOW;
	oFLOW.InitApp();
	oFLOW.ConfigTblPtr->FLOW_MEAN_COUNT_THRESHOLD = 0;

	uint8 baseImage[4096];
	int baseImageMatrix[64][64] = {0};

	int boundry_min = 20;
	int boundry_max = 30;
	int baseImageIndex = 0;

	for(int i=0; i<64;i++)
	{
		for (int j=0;j<64;j++)
		{
			if(i>=boundry_min && i<=boundry_max && j>=boundry_min && j<=boundry_max && baseImageMatrix[i][j]!=255)
			{
				baseImageMatrix[i][j]=255;
			}

			baseImage[baseImageIndex] = baseImageMatrix[i][j];
			baseImageIndex++;
		}
	}


	uint8 leftShiftImage[4096];
	int leftShiftImageMatrix[64][64] = {0};
	int shift = 5;
	int leftShiftImageIndex = 0;

	for(int i=0; i<64;i++)
	{
		for (int j=0;j<64;j++)
		{
			if(i>=boundry_min  && i<=boundry_max  && j>=boundry_min - shift  && j<=boundry_max -shift && leftShiftImageMatrix[i][j]!=255)
			{
				leftShiftImageMatrix[i][j]=255;
			}

			leftShiftImage[leftShiftImageIndex] = leftShiftImageMatrix[i][j];
			leftShiftImageIndex++;
		}
	}
	uint32 q = oFLOW.ComputeFlow((uint8 *)&baseImage, (uint8 *)&leftShiftImage);
	UtAssert_True(q>0,"quality is invalid for left shift test case.");
	UtAssert_True(oFLOW.HkTlm.PixelFlowX + shift == 0,"image did not shift left properly.");
	UtAssert_True(oFLOW.HkTlm.PixelFlowY == 0,"shifts along y axis is not expected.");
}

/**
 * Test FLOW_AppMain(), Nominal - Calculate Flow Right Shift
 */
void Test_FLOW_AppMain_Nominal_Calculate_Flow_Right_Shift(void)
{
	FLOW oFLOW;
	oFLOW.InitApp();
	oFLOW.ConfigTblPtr->FLOW_MEAN_COUNT_THRESHOLD = 0;

	uint8 baseImage[4096];
	int baseImageMatrix[64][64] = {0};

	int boundry_min = 20;
	int boundry_max = 30;
	int baseImageIndex = 0;

	for(int i=0; i<64;i++)
	{
		for (int j=0;j<64;j++)
		{
			if(i>=boundry_min && i<=boundry_max && j>=boundry_min && j<=boundry_max && baseImageMatrix[i][j]!=255)
			{
				baseImageMatrix[i][j]=255;
			}

			baseImage[baseImageIndex] = baseImageMatrix[i][j];
			baseImageIndex++;
		}
	}


	uint8 leftShiftImage[4096];
	int leftShiftImageMatrix[64][64] = {0};
	int shift = 5;
	int leftShiftImageIndex = 0;

	for(int i=0; i<64;i++)
	{
		for (int j=0;j<64;j++)
		{
			if(i>=boundry_min  && i<=boundry_max  && j>=boundry_min + shift  && j<=boundry_max + shift && leftShiftImageMatrix[i][j]!=255)
			{
				leftShiftImageMatrix[i][j]=255;
			}

			leftShiftImage[leftShiftImageIndex] = leftShiftImageMatrix[i][j];
			leftShiftImageIndex++;
		}
	}
	uint32 q = oFLOW.ComputeFlow((uint8 *)&baseImage, (uint8 *)&leftShiftImage);
	UtAssert_True(q>0,"quality is invalid for left shift test case.");
	UtAssert_True(oFLOW.HkTlm.PixelFlowX - shift == 0,"image did not shift left properly.");
	UtAssert_True(oFLOW.HkTlm.PixelFlowY == 0,"shifts along y axis is not expected.");
}

/**
 * Test FLOW_AppMain(), Nominal - Calculate Flow Top Shift
 */
void Test_FLOW_AppMain_Nominal_Calculate_Flow_Top_Shift(void)
{
	FLOW oFLOW;
	oFLOW.InitApp();
	oFLOW.ConfigTblPtr->FLOW_MEAN_COUNT_THRESHOLD = 0;

	uint8 baseImage[4096];
	int baseImageMatrix[64][64] = {0};

	int boundry_min = 20;
	int boundry_max = 30;
	int baseImageIndex = 0;

	for(int i=0; i<64;i++)
	{
		for (int j=0;j<64;j++)
		{
			if(i>=boundry_min && i<=boundry_max && j>=boundry_min && j<=boundry_max && baseImageMatrix[i][j]!=255)
			{
				baseImageMatrix[i][j]=255;

			}
			baseImage[baseImageIndex] = baseImageMatrix[i][j];
			baseImageIndex++;
		}
	}


	uint8 leftShiftImage[4096];
	int leftShiftImageMatrix[64][64] = {0};
	int shift = 5;
	int leftShiftImageIndex = 0;

	for(int i=0; i<64;i++)
	{
		for (int j=0;j<64;j++)
		{
			if(i>=boundry_min -shift  && i<=boundry_max -shift  && j>=boundry_min   && j<=boundry_max  && leftShiftImageMatrix[i][j]!=255)
			{
				leftShiftImageMatrix[i][j]=255;

			}
			leftShiftImage[leftShiftImageIndex] = leftShiftImageMatrix[i][j];
			leftShiftImageIndex++;
		}
	}
	uint32 q = oFLOW.ComputeFlow((uint8 *)&baseImage, (uint8 *)&leftShiftImage);
	UtAssert_True(q>0,"quality is invalid for left shift test case.");
	UtAssert_True(oFLOW.HkTlm.PixelFlowX == 0,"shifts along x axis is not expected.");
	UtAssert_True(oFLOW.HkTlm.PixelFlowY + shift == 0,"image did not shift left properly.");
}

/**
 * Test FLOW_AppMain(), Nominal - Calculate Flow Bottom Shift
 */
void Test_FLOW_AppMain_Nominal_Calculate_Flow_Bottom_Shift(void)
{
	FLOW oFLOW;
	oFLOW.InitApp();
	oFLOW.ConfigTblPtr->FLOW_MEAN_COUNT_THRESHOLD = 0;

	uint8 baseImage[4096];
	int baseImageMatrix[64][64] = {0};

	int boundry_min = 20;
	int boundry_max = 30;
	int baseImageIndex = 0;

	for(int i=0; i<64;i++)
	{
		for (int j=0;j<64;j++)
		{
			if(i>=boundry_min && i<=boundry_max && j>=boundry_min && j<=boundry_max && baseImageMatrix[i][j]!=255)
			{
				baseImageMatrix[i][j]=255;
			}

			baseImage[baseImageIndex] = baseImageMatrix[i][j];
			baseImageIndex++;
		}
	}


	uint8 leftShiftImage[4096];
	int leftShiftImageMatrix[64][64] = {0};
	int shift = 5;
	int leftShiftImageIndex = 0;

	for(int i=0; i<64;i++)
	{
		for (int j=0;j<64;j++)
		{
			if(i>=boundry_min + shift  && i<=boundry_max + shift  && j>=boundry_min   && j<=boundry_max  && leftShiftImageMatrix[i][j]!=255)
			{
				leftShiftImageMatrix[i][j]=255;
			}

			leftShiftImage[leftShiftImageIndex] = leftShiftImageMatrix[i][j];
			leftShiftImageIndex++;
		}
	}
	uint32 q = oFLOW.ComputeFlow((uint8 *)&baseImage, (uint8 *)&leftShiftImage);
	UtAssert_True(q>0,"quality is invalid for left shift test case.");
	UtAssert_True(oFLOW.HkTlm.PixelFlowX == 0,"shifts along x axis is not expected.");
	UtAssert_True(oFLOW.HkTlm.PixelFlowY - shift == 0,"image did not shift left properly.");
}

/**
 * Test FLOW_AppMain(), Nominal - Calculate Flow Limit
 */
void Test_FLOW_AppMain_Nominal_Limit_Flow(void)
{
	FLOW oFLOW;
	oFLOW.InitApp();

	/* slow rate */
	oFLOW.ConfigTblPtr->FLOW_DEFAULT_OUTPUT_RATE = 1;
	uint64 ts =  1.0e6f;
	int q = oFLOW.LimitRate(255,ts);
	UtAssert_True(q<0,"not computing rate correctly. rate is not less than 0 or -1");

	/* faster rate */
	oFLOW.ConfigTblPtr->FLOW_DEFAULT_OUTPUT_RATE = 2;
	q = oFLOW.LimitRate(255,ts);
	UtAssert_True(q>=0,"not computing rate correctly. rate not 255");

	/* no rate */
	oFLOW.ConfigTblPtr->FLOW_DEFAULT_OUTPUT_RATE = 0;
	q = oFLOW.LimitRate(255,ts);
	UtAssert_True(q-=1,"not computing rate correctly. rate not -1");

}


/**
 * Test USAD8, Nominal - Calculate unsigned sum of absolute differences
 */
void Test_FLOW_AppMain_Nominal_USAD8(void)
{
	FLOW oFLOW;
	oFLOW.InitApp();

	/* Test for 8-bit compatable values */
	UtAssert_True(oFLOW.USAD8(2,2) == 0, "USAD8 calculation is not correct");
	UtAssert_True(oFLOW.USAD8(26,90) == 64, "USAD8 calculation is not correct");
	UtAssert_True(oFLOW.USAD8(20,30) == 10, "USAD8 calculation is not correct");
	UtAssert_True(oFLOW.USAD8(20,50) == 30, "USAD8 calculation is not correct");

	/* Test for 8-bit boundry values */
	UtAssert_True(oFLOW.USAD8(20,255) == 235, "USAD8 calculation is not correct");
	UtAssert_True(oFLOW.USAD8(20,256) == 21, "USAD8 calculation is not correct");
	UtAssert_True(oFLOW.USAD8(20,257) == 20, "USAD8 calculation is not correct");
	UtAssert_True(oFLOW.USAD8(20,258) == 19, "USAD8 calculation is not correct");
	UtAssert_True(oFLOW.USAD8(20,259) == 18, "USAD8 calculation is not correct");
	UtAssert_True(oFLOW.USAD8(20,260) == 17, "USAD8 calculation is not correct");
	UtAssert_True(oFLOW.USAD8(20,261) == 16, "USAD8 calculation is not correct");
	UtAssert_True(oFLOW.USAD8(20,262) == 15, "USAD8 calculation is not correct");
	UtAssert_True(oFLOW.USAD8(20,263) == 14, "USAD8 calculation is not correct");
	UtAssert_True(oFLOW.USAD8(20,264) == 13, "USAD8 calculation is not correct");

	/* Test for 16-bit boundry values */
	UtAssert_True(oFLOW.USAD8(20,65535) == 490, "USAD8 calculation is not correct");
	UtAssert_True(oFLOW.USAD8(20,65536) == 21, "USAD8 calculation is not correct");
	UtAssert_True(oFLOW.USAD8(20,65537) == 20, "USAD8 calculation is not correct");
	UtAssert_True(oFLOW.USAD8(20,65538) == 19, "USAD8 calculation is not correct");
	UtAssert_True(oFLOW.USAD8(20,65539) == 18, "USAD8 calculation is not correct");
	UtAssert_True(oFLOW.USAD8(20,65540) == 17, "USAD8 calculation is not correct");
	UtAssert_True(oFLOW.USAD8(20,65541) == 16, "USAD8 calculation is not correct");
	UtAssert_True(oFLOW.USAD8(20,65542) == 15, "USAD8 calculation is not correct");
	UtAssert_True(oFLOW.USAD8(20,65543) == 14, "USAD8 calculation is not correct");
	UtAssert_True(oFLOW.USAD8(20,65544) == 13, "USAD8 calculation is not correct");

	/* Test for 24-bit boundry values */
	UtAssert_True(oFLOW.USAD8(20,16777215) == 745, "USAD8 calculation is not correct");
	UtAssert_True(oFLOW.USAD8(20,16777216) == 21, "USAD8 calculation is not correct");
	UtAssert_True(oFLOW.USAD8(20,16777217) == 20, "USAD8 calculation is not correct");
	UtAssert_True(oFLOW.USAD8(20,16777218) == 19, "USAD8 calculation is not correct");
	UtAssert_True(oFLOW.USAD8(20,16777219) == 18, "USAD8 calculation is not correct");
	UtAssert_True(oFLOW.USAD8(20,16777220) == 17, "USAD8 calculation is not correct");
	UtAssert_True(oFLOW.USAD8(20,16777221) == 16, "USAD8 calculation is not correct");
	UtAssert_True(oFLOW.USAD8(20,16777222) == 15, "USAD8 calculation is not correct");
	UtAssert_True(oFLOW.USAD8(20,16777223) == 14, "USAD8 calculation is not correct");
	UtAssert_True(oFLOW.USAD8(20,16777224) == 13, "USAD8 calculation is not correct");

	/* Test for 32-bit boundry values */
	UtAssert_True(oFLOW.USAD8(20,4294967295) == 1000, "USAD8 calculation is not correct");

}

/**
 * Test USAD8, Nominal - Calculate unsigned sum of absolute differences accumulated
 */
void Test_FLOW_AppMain_Nominal_USADA8(void)
{
	FLOW oFLOW;
	oFLOW.InitApp();
	uint32 extra = 10;
	/* Test for 8-bit compatable values */
	UtAssert_True(oFLOW.USADA8(2,2,extra) == 0 + extra, "USADA8 calculation is not correct");
	UtAssert_True(oFLOW.USADA8(26,90,extra) == 64 + extra, "USADA8 calculation is not correct");
	UtAssert_True(oFLOW.USADA8(20,30,extra) == 10 + extra, "USADA8 calculation is not correct");
	UtAssert_True(oFLOW.USADA8(20,50,extra) == 30 + extra, "USADA8 calculation is not correct");

	/* Test for 8-bit boundry values */
	UtAssert_True(oFLOW.USADA8(20,255,extra) == 235 + extra, "USADA8 calculation is not correct");
	UtAssert_True(oFLOW.USADA8(20,256,extra) == 21 + extra, "USADA8 calculation is not correct");
	UtAssert_True(oFLOW.USADA8(20,257,extra) == 20 + extra, "USADA8 calculation is not correct");
	UtAssert_True(oFLOW.USADA8(20,258,extra) == 19 + extra, "USADA8 calculation is not correct");
	UtAssert_True(oFLOW.USADA8(20,259,extra) == 18 + extra, "USADA8 calculation is not correct");
	UtAssert_True(oFLOW.USADA8(20,260,extra) == 17 + extra, "USADA8 calculation is not correct");
	UtAssert_True(oFLOW.USADA8(20,261,extra) == 16 + extra, "USADA8 calculation is not correct");
	UtAssert_True(oFLOW.USADA8(20,262,extra) == 15 + extra, "USADA8 calculation is not correct");
	UtAssert_True(oFLOW.USADA8(20,263,extra) == 14 + extra, "USADA8 calculation is not correct");
	UtAssert_True(oFLOW.USADA8(20,264,extra) == 13 + extra, "USADA8 calculation is not correct");

	/* Test for 16-bit boundry values */
	UtAssert_True(oFLOW.USADA8(20,65535,extra) == 490 + extra, "USADA8 calculation is not correct");
	UtAssert_True(oFLOW.USADA8(20,65536,extra) == 21 + extra, "USADA8 calculation is not correct");
	UtAssert_True(oFLOW.USADA8(20,65537,extra) == 20 + extra, "USADA8 calculation is not correct");
	UtAssert_True(oFLOW.USADA8(20,65538,extra) == 19 + extra, "USADA8 calculation is not correct");
	UtAssert_True(oFLOW.USADA8(20,65539,extra) == 18 + extra, "USADA8 calculation is not correct");
	UtAssert_True(oFLOW.USADA8(20,65540,extra) == 17 + extra, "USADA8 calculation is not correct");
	UtAssert_True(oFLOW.USADA8(20,65541,extra) == 16 + extra, "USADA8 calculation is not correct");
	UtAssert_True(oFLOW.USADA8(20,65542,extra) == 15 + extra, "USADA8 calculation is not correct");
	UtAssert_True(oFLOW.USADA8(20,65543,extra) == 14 + extra, "USADA8 calculation is not correct");
	UtAssert_True(oFLOW.USADA8(20,65544,extra) == 13 + extra, "USADA8 calculation is not correct");

	/* Test for 24-bit boundry values */
	UtAssert_True(oFLOW.USADA8(20,16777215,extra) == 745 + extra, "USADA8 calculation is not correct");
	UtAssert_True(oFLOW.USADA8(20,16777216,extra) == 21 + extra, "USADA8 calculation is not correct");
	UtAssert_True(oFLOW.USADA8(20,16777217,extra) == 20 + extra, "USADA8 calculation is not correct");
	UtAssert_True(oFLOW.USADA8(20,16777218,extra) == 19 + extra, "USADA8 calculation is not correct");
	UtAssert_True(oFLOW.USADA8(20,16777219,extra) == 18 + extra, "USADA8 calculation is not correct");
	UtAssert_True(oFLOW.USADA8(20,16777220,extra) == 17 + extra, "USADA8 calculation is not correct");
	UtAssert_True(oFLOW.USADA8(20,16777221,extra) == 16 + extra, "USADA8 calculation is not correct");
	UtAssert_True(oFLOW.USADA8(20,16777222,extra) == 15 + extra, "USADA8 calculation is not correct");
	UtAssert_True(oFLOW.USADA8(20,16777223,extra) == 14 + extra, "USADA8 calculation is not correct");
	UtAssert_True(oFLOW.USADA8(20,16777224,extra) == 13 + extra, "USADA8 calculation is not correct");

	/* Test for 32-bit boundry values */
	UtAssert_True(oFLOW.USADA8(20,4294967295,extra) == 1000 + extra, "USADA8 calculation is not correct");

}

/**
 * Test USAD8, Nominal - Calculate unsigned halving add 8-bit
 */
void Test_FLOW_AppMain_Nominal_UHADD8(void)
{
	FLOW oFLOW;
	oFLOW.InitApp();

	/* Test for 8-bit compatable values */
	UtAssert_True(oFLOW.UHADD8(2,2) == 2, "UHADD8 calculation is not correct");
	UtAssert_True(oFLOW.UHADD8(26,90) == 58, "UHADD8 calculation is not correct");
	UtAssert_True(oFLOW.UHADD8(20,30) == 25, "UHADD8 calculation is not correct");
	UtAssert_True(oFLOW.UHADD8(20,50) == 35, "UHADD8 calculation is not correct");

	/* Test for 8-bit boundry values */
	UtAssert_True(oFLOW.UHADD8(20,255) == 137, "UHADD8 calculation is not correct");
	UtAssert_True(oFLOW.UHADD8(20,256) == 10, "UHADD8 calculation is not correct");
	UtAssert_True(oFLOW.UHADD8(20,257) == 10, "UHADD8 calculation is not correct");
	UtAssert_True(oFLOW.UHADD8(20,258) == 11, "UHADD8 calculation is not correct");
	UtAssert_True(oFLOW.UHADD8(20,259) == 11, "UHADD8 calculation is not correct");
	UtAssert_True(oFLOW.UHADD8(20,260) == 12, "UHADD8 calculation is not correct");
	UtAssert_True(oFLOW.UHADD8(20,261) == 12, "UHADD8 calculation is not correct");
	UtAssert_True(oFLOW.UHADD8(20,262) == 13, "UHADD8 calculation is not correct");
	UtAssert_True(oFLOW.UHADD8(20,263) == 13, "UHADD8 calculation is not correct");
	UtAssert_True(oFLOW.UHADD8(20,264) == 14, "UHADD8 calculation is not correct");

	/* Test for 16-bit boundry values */
	UtAssert_True(oFLOW.UHADD8(20,65535) == 32649, "UHADD8 calculation is not correct");
	UtAssert_True(oFLOW.UHADD8(20,65536) == 10, "UHADD8 calculation is not correct");
	UtAssert_True(oFLOW.UHADD8(20,65537) == 10, "UHADD8 calculation is not correct");
	UtAssert_True(oFLOW.UHADD8(20,65538) == 11, "UHADD8 calculation is not correct");
	UtAssert_True(oFLOW.UHADD8(20,65539) == 11, "UHADD8 calculation is not correct");
	UtAssert_True(oFLOW.UHADD8(20,65540) == 12, "UHADD8 calculation is not correct");
	UtAssert_True(oFLOW.UHADD8(20,65541) == 12, "UHADD8 calculation is not correct");
	UtAssert_True(oFLOW.UHADD8(20,65542) == 13, "UHADD8 calculation is not correct");
	UtAssert_True(oFLOW.UHADD8(20,65543) == 13, "UHADD8 calculation is not correct");
	UtAssert_True(oFLOW.UHADD8(20,65544) == 14, "UHADD8 calculation is not correct");

	/* Test for 24-bit boundry values */
	UtAssert_True(oFLOW.UHADD8(20,16777215) == 8355721, "UHADD8 calculation is not correct");
	UtAssert_True(oFLOW.UHADD8(20,16777216) == 10, "UHADD8 calculation is not correct");
	UtAssert_True(oFLOW.UHADD8(20,16777217) == 10, "UHADD8 calculation is not correct");
	UtAssert_True(oFLOW.UHADD8(20,16777218) == 11, "UHADD8 calculation is not correct");
	UtAssert_True(oFLOW.UHADD8(20,16777219) == 11, "UHADD8 calculation is not correct");
	UtAssert_True(oFLOW.UHADD8(20,16777220) == 12, "UHADD8 calculation is not correct");
	UtAssert_True(oFLOW.UHADD8(20,16777221) == 12, "UHADD8 calculation is not correct");
	UtAssert_True(oFLOW.UHADD8(20,16777222) == 13, "UHADD8 calculation is not correct");
	UtAssert_True(oFLOW.UHADD8(20,16777223) == 13, "UHADD8 calculation is not correct");
	UtAssert_True(oFLOW.UHADD8(20,16777224) == 14, "UHADD8 calculation is not correct");

	/* Test for 32-bit boundry values */
	UtAssert_True(oFLOW.UHADD8(20,4294967295) == 2139062153, "UHADD8 calculation is not correct");
}

/**
 * Test USAD8, Nominal - Calculate unsigned halving add 8-bit
 */
void Test_FLOW_AppMain_Nominal_Compute_SAD8X8(void)
{
	FLOW oFLOW;
	oFLOW.InitApp();

	uint8 a = 2;
	uint8 b = 1;
	uint16 RowSize = 16;
	uint8 image1[256] = {
	/**		                                1  1  1  1  1  1
	 *        0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5        */
	/*	0  */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	/*	1  */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	/*	2  */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	/*  3  */ 0, 0, 0, a, a, a, a, b, b, b, b, 0, 0, 0, 0, 0,
	/*	4  */ 0, 0, 0, a, a, a, a, b, b, b, b, 0, 0, 0, 0, 0,
	/*	5  */ 0, 0, 0, a, a, a, a, b, b, b, b, 0, 0, 0, 0, 0,
	/*	6  */ 0, 0, 0, a, a, a, a, b, b, b, b, 0, 0, 0, 0, 0,
	/*	7  */ 0, 0, 0, a, a, a, a, b, b, b, b, 0, 0, 0, 0, 0,
	/*	8  */ 0, 0, 0, a, a, a, a, b, b, b, b, 0, 0, 0, 0, 0,
	/*	9  */ 0, 0, 0, a, a, a, a, b, b, b, b, 0, 0, 0, 0, 0,
	/*	10 */ 0, 0, 0, a, a, a, a, b, b, b, b, 0, 0, 0, 0, 0,
	/*	11 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	/*	12 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	/*	13 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	/*	14 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	/*	15 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	};
	uint8 image2[256] = {
	/**		                                1  1  1  1  1  1
	 *        0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5        */
	/*	0  */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	/*	1  */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	/*	2  */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	/*  3  */ 0, 0, 0, 0, a, a, a, a, b, b, b, b, 0, 0, 0, 0,
	/*	4  */ 0, 0, 0, 0, a, a, a, a, b, b, b, b, 0, 0, 0, 0,
	/*	5  */ 0, 0, 0, 0, a, a, a, a, b, b, b, b, 0, 0, 0, 0,
	/*	6  */ 0, 0, 0, 0, a, a, a, a, b, b, b, b, 0, 0, 0, 0,
	/*	7  */ 0, 0, 0, 0, a, a, a, a, b, b, b, b, 0, 0, 0, 0,
	/*	8  */ 0, 0, 0, 0, a, a, a, a, b, b, b, b, 0, 0, 0, 0,
	/*	9  */ 0, 0, 0, 0, a, a, a, a, b, b, b, b, 0, 0, 0, 0,
	/*	10 */ 0, 0, 0, 0, a, a, a, a, b, b, b, b, 0, 0, 0, 0,
	/*	11 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	/*	12 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	/*	13 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	/*	14 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	/*	15 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

	};
	uint32 result = oFLOW.ComputeSad8X8((uint8 *)&image1,(uint8 *)&image2, 3, 3, 3, 3, RowSize);
	UtAssert_True(result == 24, "SAD8X8 calculation is not correct");

	uint8 image3[256] = {
	/**		                                1  1  1  1  1  1
	 *        0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5        */
	/*	0  */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	/*	1  */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	/*	2  */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	/*  3  */ 0, 0, 0, 0, 0, a, a, a, a, b, b, b, b, 0, 0, 0,
	/*	4  */ 0, 0, 0, 0, 0, a, a, a, a, b, b, b, b, 0, 0, 0,
	/*	5  */ 0, 0, 0, 0, 0, a, a, a, a, b, b, b, b, 0, 0, 0,
	/*	6  */ 0, 0, 0, 0, 0, a, a, a, a, b, b, b, b, 0, 0, 0,
	/*	7  */ 0, 0, 0, 0, 0, a, a, a, a, b, b, b, b, 0, 0, 0,
	/*	8  */ 0, 0, 0, 0, 0, a, a, a, a, b, b, b, b, 0, 0, 0,
	/*	9  */ 0, 0, 0, 0, 0, a, a, a, a, b, b, b, b, 0, 0, 0,
	/*	10 */ 0, 0, 0, 0, 0, a, a, a, a, b, b, b, b, 0, 0, 0,
	/*	11 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	/*	12 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	/*	13 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	/*	14 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	/*	15 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

	};
	result = oFLOW.ComputeSad8X8((uint8 *)&image1,(uint8 *)&image3, 3, 3, 3, 3, RowSize);
	/*
	 * In each row total 4 shifts happen 2 of a and b each.
	 * a shifts (2+2) = 4
	 * b shifts (1+1) = 2
	 * total shifts in each row = 6
	 * number of rows = 8
	 * SAD8X8 = (total shifts in each row * number of rows) = 48
	 * */
	UtAssert_True(result == 48, "SAD8X8 calculation is not correct");

}

void Test_FLOW_AppMain_Nominal_Compute_Difference(void)
{
	FLOW oFLOW;
	oFLOW.InitApp();

	uint8 a = 2;
	uint8 b = 1;
	uint16 RowSize = 16;
	uint8 image1[256] = {
	/**		                                1  1  1  1  1  1
	 *        0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5        */
	/*	0  */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	/*	1  */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	/*	2  */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	/*  3  */ 0, 0, 0, a, a, a, a, b, b, b, b, 0, 0, 0, 0, 0,
	/*	4  */ 0, 0, 0, a, a, a, a, b, b, b, b, 0, 0, 0, 0, 0,
	/*	5  */ 0, 0, 0, a, a, a, a, b, b, b, b, 0, 0, 0, 0, 0,
	/*	6  */ 0, 0, 0, a, a, a, a, b, b, b, b, 0, 0, 0, 0, 0,
	/*	7  */ 0, 0, 0, a, a, a, a, b, b, b, b, 0, 0, 0, 0, 0,
	/*	8  */ 0, 0, 0, a, a, a, a, b, b, b, b, 0, 0, 0, 0, 0,
	/*	9  */ 0, 0, 0, a, a, a, a, b, b, b, b, 0, 0, 0, 0, 0,
	/*	10 */ 0, 0, 0, a, a, a, a, b, b, b, b, 0, 0, 0, 0, 0,
	/*	11 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	/*	12 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	/*	13 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	/*	14 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	/*	15 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	};

	/* Identifies a 4x4 pattern from offset x and y
	 * in this case the 4x4 sub-image starts from (5,4) which looks like -
	 * 			a a b b
	 * 			a a b b
	 * 			a a b b
	 * 			a a b b
	 *
	 * compute difference between rows: since they are identical it is 0
	 * compute difference between cols: (0th & 1st = 0), (1st & 2nd = 4(all four elements are different)), (2nd & 3rd = 0)
	 * */
	uint32 result = oFLOW.ComputeDifference((uint8 *)&image1, 3, 3, RowSize);
	UtAssert_True(result == 4, "Difference calculation is not correct");

	uint8 image2[256] = {
	/**		                                1  1  1  1  1  1
	 *        0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5        */
	/*	0  */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	/*	1  */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	/*	2  */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	/*  3  */ 0, 0, 0, 0, a, a, a, a, b, b, b, b, 0, 0, 0, 0,
	/*	4  */ 0, 0, 0, 0, a, a, a, a, b, b, b, b, 0, 0, 0, 0,
	/*	5  */ 0, 0, 0, 0, a, a, a, a, b, b, b, b, 0, 0, 0, 0,
	/*	6  */ 0, 0, 0, 0, a, a, a, a, b, b, b, b, 0, 0, 0, 0,
	/*	7  */ 0, 0, 0, 0, a, a, a, a, b, b, b, b, 0, 0, 0, 0,
	/*	8  */ 0, 0, 0, 0, a, a, a, a, b, b, b, b, 0, 0, 0, 0,
	/*	9  */ 0, 0, 0, 0, a, a, a, a, b, b, b, b, 0, 0, 0, 0,
	/*	10 */ 0, 0, 0, 0, a, a, a, a, b, b, b, b, 0, 0, 0, 0,
	/*	11 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	/*	12 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	/*	13 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	/*	14 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	/*	15 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

	};
	result = oFLOW.ComputeDifference((uint8 *)&image2, 3, 3, RowSize);
	UtAssert_True(result == 4, "Difference calculation is not correct");

	uint8 image3[256] = {
	/**		                                1  1  1  1  1  1
	 *        0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5        */
	/*	0  */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	/*	1  */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	/*	2  */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	/*  3  */ 0, 0, 0, 0, a, a, b, a, b, b, b, b, 0, 0, 0, 0,
	/*	4  */ 0, 0, 0, 0, a, a, b, a, b, b, b, b, 0, 0, 0, 0,
	/*	5  */ 0, 0, 0, 0, a, a, b, a, b, b, b, b, 0, 0, 0, 0,
	/*	6  */ 0, 0, 0, 0, a, a, b, a, b, b, b, b, 0, 0, 0, 0,
	/*	7  */ 0, 0, 0, 0, a, a, b, a, b, b, b, b, 0, 0, 0, 0,
	/*	8  */ 0, 0, 0, 0, a, a, b, a, b, b, b, b, 0, 0, 0, 0,
	/*	9  */ 0, 0, 0, 0, a, a, b, a, b, b, b, b, 0, 0, 0, 0,
	/*	10 */ 0, 0, 0, 0, a, a, b, a, b, b, b, b, 0, 0, 0, 0,
	/*	11 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	/*	12 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	/*	13 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	/*	14 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	/*	15 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

	};
	result = oFLOW.ComputeDifference((uint8 *)&image3, 3, 3, RowSize);
	UtAssert_True(result == 12, "Difference calculation is not correct");
}

void Test_FLOW_AppMain_Nominal_Compute_Subpixel(void)
{
	FLOW oFLOW;
	oFLOW.InitApp();

	uint8 a = 2;
	uint16 RowSize = 8;
	uint32 acc[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	uint8 image1[64] = {
	/**
	 *        0  1  2  3  4  5  6  7          */
	/*	0  */ 0, 0, 0, 0, 0, 0, 0, 0,
	/*	1  */ 0, a, 0, 0, 0, 0, 0, 0,
	/*	2  */ 0, 0, 0, 0, 0, 0, 0, 0,
	/*  3  */ 0, 0, 0, 0, 0, 0, 0, 0,
	/*	4  */ 0, 0, 0, 0, 0, 0, 0, 0,
	/*	5  */ 0, 0, 0, 0, 0, 0, 0, 0,
	/*	6  */ 0, 0, 0, 0, 0, 0, 0, 0,
	/*	7  */ 0, 0, 0, 0, 0, 0, 0, 0,
	};

	uint8 image2[64] = {
	/**
	 *        0  1  2  3  4  5  6  7          */
	/*	0  */ 0, 0, 0, 0, 0, 0, 0, 0,
	/*	1  */ 0, 0, a, 0, 0, 0, 0, 0,
	/*	2  */ 0, 0, 0, 0, 0, 0, 0, 0,
	/*  3  */ 0, 0, 0, 0, 0, 0, 0, 0,
	/*	4  */ 0, 0, 0, 0, 0, 0, 0, 0,
	/*	5  */ 0, 0, 0, 0, 0, 0, 0, 0,
	/*	6  */ 0, 0, 0, 0, 0, 0, 0, 0,
	/*	7  */ 0, 0, 0, 0, 0, 0, 0, 0,
	};

	oFLOW.ComputeSubpixel((uint8 *)&image1, (uint8 *)&image2, 1, 1, 1, 1, acc, RowSize);
	uint8 i;
	for(i = 0; i < 8; i++)
	{
		UtAssert_True(acc[i] > 0, "Subpixel calculation is not correct");
	}
}
/**************************************************************************
 * Rollup Test Cases
 **************************************************************************/
void FLOW_App_Test_AddTestCases(void)
{
    UtTest_Add(Test_FLOW_InitEvent_Fail_Register, FLOW_Test_Setup, FLOW_Test_TearDown,
               "Test_FLOW_InitEvent_Fail_Register");
    UtTest_Add(Test_FLOW_InitPipe_Fail_CreateSCHPipe, FLOW_Test_Setup, FLOW_Test_TearDown,
               "Test_FLOW_InitPipe_Fail_CreateSCHPipe");
    UtTest_Add(Test_FLOW_InitPipe_Fail_SubscribeWakeup, FLOW_Test_Setup, FLOW_Test_TearDown,
               "Test_FLOW_InitPipe_Fail_SubscribeWakeup");
    UtTest_Add(Test_FLOW_InitPipe_Fail_SubscribeSendHK, FLOW_Test_Setup, FLOW_Test_TearDown,
               "Test_FLOW_InitPipe_Fail_SubscribeSendHK");
    UtTest_Add(Test_FLOW_InitPipe_Fail_CreateCMDPipe, FLOW_Test_Setup, FLOW_Test_TearDown,
               "Test_FLOW_InitPipe_Fail_CreateCMDPipe");
    UtTest_Add(Test_FLOW_InitPipe_Fail_SubscribeCMD, FLOW_Test_Setup, FLOW_Test_TearDown,
               "Test_FLOW_InitPipe_Fail_SubscribeCMD");
    UtTest_Add(Test_FLOW_InitPipe_Fail_CreateDATAPipe, FLOW_Test_Setup, FLOW_Test_TearDown,
               "Test_FLOW_InitPipe_Fail_CreateDATAPipe");
    UtTest_Add(Test_FLOW_InitPipe_Fail_CreateGYROPipe, FLOW_Test_Setup, FLOW_Test_TearDown,
               "Test_FLOW_InitPipe_Fail_CreateGYROPipe");
    UtTest_Add(Test_FLOW_InitPipe_Fail_SubscribeGYRO, FLOW_Test_Setup, FLOW_Test_TearDown,
               "Test_FLOW_InitPipe_Fail_SubscribeGYRO");
    UtTest_Add(Test_FLOW_InitData, FLOW_Test_Setup, FLOW_Test_TearDown,
               "Test_FLOW_InitData");
    UtTest_Add(Test_FLOW_InitApp_Fail_InitEvent, FLOW_Test_Setup, FLOW_Test_TearDown,
               "Test_FLOW_InitApp_Fail_InitEvent");
    UtTest_Add(Test_FLOW_InitApp_Fail_InitPipe, FLOW_Test_Setup, FLOW_Test_TearDown,
               "Test_FLOW_InitApp_Fail_InitPipe");
    UtTest_Add(Test_FLOW_InitApp_Fail_InitData, FLOW_Test_Setup, FLOW_Test_TearDown,
               "Test_FLOW_InitApp_Fail_InitData");
    UtTest_Add(Test_FLOW_InitApp_Fail_InitConfigTbl, FLOW_Test_Setup, FLOW_Test_TearDown,
               "Test_FLOW_InitApp_Fail_InitConfigTbl");
    UtTest_Add(Test_FLOW_InitApp_Nominal, FLOW_Test_Setup, FLOW_Test_TearDown,
               "Test_FLOW_InitApp_Nominal");
    UtTest_Add(Test_FLOW_AppMain_Fail_RegisterApp, FLOW_Test_Setup, FLOW_Test_TearDown,
               "Test_FLOW_AppMain_Fail_RegisterApp");
    UtTest_Add(Test_FLOW_AppMain_Fail_InitApp, FLOW_Test_Setup, FLOW_Test_TearDown,
               "Test_FLOW_AppMain_Fail_InitApp");
    UtTest_Add(Test_FLOW_AppMain_Fail_AcquireConfigPtrs, FLOW_Test_Setup, FLOW_Test_TearDown,
               "Test_FLOW_AppMain_Fail_AcquireConfigPtrs");
    UtTest_Add(Test_FLOW_AppMain_InvalidSchMessage, FLOW_Test_Setup, FLOW_Test_TearDown,
               "Test_FLOW_AppMain_InvalidSchMessage");
    UtTest_Add(Test_FLOW_AppMain_Nominal_SendHK, FLOW_Test_Setup, FLOW_Test_TearDown,
               "Test_FLOW_AppMain_Nominal_SendHK");
    UtTest_Add(Test_FLOW_AppMain_Nominal_Wakeup, FLOW_Test_Setup, FLOW_Test_TearDown,
               "Test_FLOW_AppMain_Nominal_Wakeup");

    UtTest_Add(Test_FLOW_AppMain_Nominal_Image_Received, FLOW_Test_Setup, FLOW_Test_TearDown,
			   "Test_FLOW_AppMain_Nominal_Image_Received");
    UtTest_Add(Test_FLOW_AppMain_Nominal_Calculate_Flow_Left_Shift, FLOW_Test_Setup, FLOW_Test_TearDown,
			   "Test_FLOW_AppMain_Nominal_Calculate_Flow_Left_Shift");
    UtTest_Add(Test_FLOW_AppMain_Nominal_Calculate_Flow_Right_Shift, FLOW_Test_Setup, FLOW_Test_TearDown,
			   "Test_FLOW_AppMain_Nominal_Calculate_Flow_Right_Shift");
    UtTest_Add(Test_FLOW_AppMain_Nominal_Calculate_Flow_Top_Shift, FLOW_Test_Setup, FLOW_Test_TearDown,
			   "Test_FLOW_AppMain_Nominal_Calculate_Flow_Top_Shift");
    UtTest_Add(Test_FLOW_AppMain_Nominal_Calculate_Flow_Bottom_Shift, FLOW_Test_Setup, FLOW_Test_TearDown,
			   "Test_FLOW_AppMain_Nominal_Calculate_Flow_Bottom_Shift");
    UtTest_Add(Test_FLOW_AppMain_Nominal_Limit_Flow, FLOW_Test_Setup, FLOW_Test_TearDown,
			   "Test_FLOW_AppMain_Nominal_Limit_Flow");

    UtTest_Add(Test_FLOW_AppMain_Nominal_USAD8, FLOW_Test_Setup, FLOW_Test_TearDown,
			   "Test_FLOW_AppMain_Nominal_USAD8");
    UtTest_Add(Test_FLOW_AppMain_Nominal_USADA8, FLOW_Test_Setup, FLOW_Test_TearDown,
			   "Test_FLOW_AppMain_Nominal_USAD8A");
    UtTest_Add(Test_FLOW_AppMain_Nominal_UHADD8, FLOW_Test_Setup, FLOW_Test_TearDown,
			   "Test_FLOW_AppMain_Nominal_UHADD8");
    UtTest_Add(Test_FLOW_AppMain_Nominal_Compute_SAD8X8, FLOW_Test_Setup, FLOW_Test_TearDown,
			   "Test_FLOW_AppMain_Nominal_Compute_SAD8X8");
    UtTest_Add(Test_FLOW_AppMain_Nominal_Compute_Difference, FLOW_Test_Setup, FLOW_Test_TearDown,
    			   "Test_FLOW_AppMain_Nominal_Compute_Difference");
    UtTest_Add(Test_FLOW_AppMain_Nominal_Compute_Subpixel, FLOW_Test_Setup, FLOW_Test_TearDown,
    			   "Test_FLOW_AppMain_Nominal_Compute_Subpixel");

}


