#include "common_types.h"
#include "utassert.h"
#include "cfe_psp_private.h"
#include <unistd.h>
#include "stubs.h"
#include "cfe_mission_cfg.h"
#include "cfe_platform_cfg.h"
#include <string.h>
#include "cfe_es.h"
#include "osapi.h"

#define CFE_PSP_CDS_KEY_FILE ".cdskeyfile"
#define CFE_PSP_RESET_KEY_FILE ".resetkeyfile"
#define CFE_PSP_RESERVED_KEY_FILE ".reservedkeyfile"

extern CFE_PSP_GlobalData_t CFE_PSP_GlobalData;

extern unsigned int _init;
extern unsigned int _fini;

int CFE_PSP_NewArgv( char*** pargv, char** new_args)
{
    int i = 0;
    int new_argc = 0;
    char** tmp = new_args;

    while (*tmp) {
        ++new_argc;
        ++tmp;
    }

    tmp = malloc( sizeof(char*) * (new_argc + 1));

    for (i = 0; i < new_argc; ++i) {
        tmp[i] = strdup(new_args[i]);
    }
    tmp[i] = NULL;

    *pargv = tmp;

    return new_argc;
}



void Test_CFE_PSP_Main_NominalPO(void)
{
    char *argv = 0;

    char *dummy_argv[] = { "core-bin", "--reset", "PO", "--runmode", "NORMAL", 0 };
	int argc = CFE_PSP_NewArgv(&argv, dummy_argv);

	/* Execute Test */
	PSP_Main(argc, argv);

	CFE_PSP_DeleteOSResources();

	free(argv);

	/* Verify Outputs */
	UtAssert_True(PSP_UT_StubData.CFE.CallCounts.CFE_ES_Main_Func == 1, "CFE Initialized");
	UtAssert_True(PSP_UT_StubData.CFE.Main_Args.ModeId == 1, "CFE Start Mode");
	UtAssert_True(PSP_UT_StubData.CFE.Main_Args.StartType == 2, "CFE Reset Type");
	UtAssert_True(PSP_UT_StubData.CFE.Main_Args.StartSubtype == 1, "CFE Reset Subtype");
	UtAssert_True(CFE_PSP_GlobalData.CFE_PSP_SpacecraftId == CFE_SPACECRAFT_ID, "CFE Spacecraft ID");
	UtAssert_True(CFE_PSP_GlobalData.CFE_PSP_CpuId == CFE_CPU_ID, "CFE CPU ID");
	UtAssert_StrCmp(CFE_PSP_GlobalData.CFE_PSP_CpuName, CFE_CPU_NAME, "CFE CPU Name");
}

void Test_CFE_PSP_Main_NominalPR(void)
{
    char *argv = 0;

    char *dummy_argv[] = { "core-bin", "--reset", "PR", "--runmode", "NORMAL", 0 };
	int argc = CFE_PSP_NewArgv(&argv, dummy_argv);

	/* Execute Test */
	PSP_Main(argc, argv);

	CFE_PSP_DeleteOSResources();

	free(argv);

	/* Verify Outputs */
	UtAssert_True(PSP_UT_StubData.CFE.CallCounts.CFE_ES_Main_Func == 1, "CFE Initialized");
	UtAssert_True(PSP_UT_StubData.CFE.Main_Args.ModeId == 1, "CFE Start Mode");
	UtAssert_True(PSP_UT_StubData.CFE.Main_Args.StartType == 1, "CFE Reset Type");
	UtAssert_True(PSP_UT_StubData.CFE.Main_Args.StartSubtype == 1, "CFE Reset Subtype");
	UtAssert_True(CFE_PSP_GlobalData.CFE_PSP_SpacecraftId == CFE_SPACECRAFT_ID, "CFE Spacecraft ID");
	UtAssert_True(CFE_PSP_GlobalData.CFE_PSP_CpuId == CFE_CPU_ID, "CFE CPU ID");
	UtAssert_StrCmp(CFE_PSP_GlobalData.CFE_PSP_CpuName, CFE_CPU_NAME, "CFE CPU Name");
}

void Test_CFE_PSP_Main_IncorrectPRType(void)
{
    char *argv = 0;

    char *dummy_argv[] = { "core-bin", "--reset", "POO", "--runmode", "NORMAL", 0 };
	int argc = CFE_PSP_NewArgv(&argv, dummy_argv);

	/* Execute Test */
	PSP_Main(argc, argv);

	CFE_PSP_DeleteOSResources();

	free(argv);

	/* Verify Outputs */
	UtAssert_True(PSP_UT_StubData.CFE.CallCounts.CFE_ES_Main_Func == 1, "CFE Not Initialized");
	UtAssert_True(PSP_UT_StubData.CFE.Main_Args.ModeId == 1, "CFE Start Mode");
	UtAssert_True(PSP_UT_StubData.CFE.Main_Args.StartType == 2, "CFE Reset Type");
	UtAssert_True(PSP_UT_StubData.CFE.Main_Args.StartSubtype == 1, "CFE Reset Subtype");
	UtAssert_True(CFE_PSP_GlobalData.CFE_PSP_SpacecraftId == CFE_SPACECRAFT_ID, "CFE Spacecraft ID");
	UtAssert_True(CFE_PSP_GlobalData.CFE_PSP_CpuId == CFE_CPU_ID, "CFE CPU ID");
	UtAssert_StrCmp(CFE_PSP_GlobalData.CFE_PSP_CpuName, CFE_CPU_NAME, "CFE CPU Name");
}

void Test_CFE_PSP_Main_PORunmodeTest(void)
{
    char *argv = 0;

    char *dummy_argv[] = { "core-bin", "--reset", "PO", "--runmode", "TEST", 0 };
	int argc = CFE_PSP_NewArgv(&argv, dummy_argv);

	/* Execute Test */
	PSP_Main(argc, argv);

	CFE_PSP_DeleteOSResources();

	free(argv);

	/* Verify Outputs */
	UtAssert_True(PSP_UT_StubData.CFE.CallCounts.CFE_ES_Main_Func == 1, "CFE Initialized");
	UtAssert_True(PSP_UT_StubData.CFE.Main_Args.ModeId == 1, "CFE Start Mode");
	UtAssert_True(PSP_UT_StubData.CFE.Main_Args.StartType == 2, "CFE Reset Type");
	UtAssert_True(PSP_UT_StubData.CFE.Main_Args.StartSubtype == 1, "CFE Reset Subtype");
	UtAssert_True(CFE_PSP_GlobalData.CFE_PSP_SpacecraftId == CFE_SPACECRAFT_ID, "CFE Spacecraft ID");
	UtAssert_True(CFE_PSP_GlobalData.CFE_PSP_CpuId == CFE_CPU_ID, "CFE CPU ID");
	UtAssert_StrCmp(CFE_PSP_GlobalData.CFE_PSP_CpuName, CFE_CPU_NAME, "CFE CPU Name");
}

void Test_CFE_PSP_Main_POInvalidRunmode(void)
{
    char *argv = 0;

    char *dummy_argv[] = { "core-bin", "--reset", "PO", "--runmode", "HELLO", 0 };
	int argc = CFE_PSP_NewArgv(&argv, dummy_argv);

	/* Execute Test */
	PSP_Main(argc, argv);

	CFE_PSP_DeleteOSResources();

	free(argv);

	/* Verify Outputs */
	UtAssert_True(PSP_UT_StubData.CFE.CallCounts.CFE_ES_Main_Func == 1, "CFE Initialized");
	UtAssert_True(PSP_UT_StubData.CFE.Main_Args.ModeId == 1, "CFE Start Mode");
	UtAssert_True(PSP_UT_StubData.CFE.Main_Args.StartType == 2, "CFE Reset Type");
	UtAssert_True(PSP_UT_StubData.CFE.Main_Args.StartSubtype == 1, "CFE Reset Subtype");
	UtAssert_True(CFE_PSP_GlobalData.CFE_PSP_SpacecraftId == CFE_SPACECRAFT_ID, "CFE Spacecraft ID");
	UtAssert_True(CFE_PSP_GlobalData.CFE_PSP_CpuId == CFE_CPU_ID, "CFE CPU ID");
	UtAssert_StrCmp(CFE_PSP_GlobalData.CFE_PSP_CpuName, CFE_CPU_NAME, "CFE CPU Name");
}

void Test_CFE_PSP_Main_PRSubtype1(void)
{
    char *argv = 0;

    char *dummy_argv[] = { "core-bin", "--reset", "PR", "--subtype", "1", 0 };
	int argc = CFE_PSP_NewArgv(&argv, dummy_argv);

	/* Execute Test */
	PSP_Main(argc, argv);

	CFE_PSP_DeleteOSResources();

	free(argv);

	/* Verify Outputs */
	UtAssert_True(PSP_UT_StubData.CFE.CallCounts.CFE_ES_Main_Func == 1, "CFE Initialized");
	UtAssert_True(PSP_UT_StubData.CFE.Main_Args.ModeId == 1, "CFE Start Mode");
	UtAssert_True(PSP_UT_StubData.CFE.Main_Args.StartType == 1, "CFE Reset Type");
	UtAssert_True(PSP_UT_StubData.CFE.Main_Args.StartSubtype == 1, "CFE Reset Subtype");
	UtAssert_True(CFE_PSP_GlobalData.CFE_PSP_SpacecraftId == CFE_SPACECRAFT_ID, "CFE Spacecraft ID");
	UtAssert_True(CFE_PSP_GlobalData.CFE_PSP_CpuId == CFE_CPU_ID, "CFE CPU ID");
	UtAssert_StrCmp(CFE_PSP_GlobalData.CFE_PSP_CpuName, CFE_CPU_NAME, "CFE CPU Name");
}

void Test_CFE_PSP_Main_PRSubtype2(void)
{
    char *argv = 0;

    char *dummy_argv[] = { "core-bin", "--reset", "PR", "--subtype", "2", 0 };
	int argc = CFE_PSP_NewArgv(&argv, dummy_argv);

	/* Execute Test */
	PSP_Main(argc, argv);

	CFE_PSP_DeleteOSResources();

	free(argv);

	/* Verify Outputs */
	UtAssert_True(PSP_UT_StubData.CFE.CallCounts.CFE_ES_Main_Func == 1, "CFE Initialized");
	UtAssert_True(PSP_UT_StubData.CFE.Main_Args.ModeId == 1, "CFE Start Mode");
	UtAssert_True(PSP_UT_StubData.CFE.Main_Args.StartType == 1, "CFE Reset Type");
	UtAssert_True(PSP_UT_StubData.CFE.Main_Args.StartSubtype == 2, "CFE Reset Subtype");
	UtAssert_True(CFE_PSP_GlobalData.CFE_PSP_SpacecraftId == CFE_SPACECRAFT_ID, "CFE Spacecraft ID");
	UtAssert_True(CFE_PSP_GlobalData.CFE_PSP_CpuId == CFE_CPU_ID, "CFE CPU ID");
	UtAssert_StrCmp(CFE_PSP_GlobalData.CFE_PSP_CpuName, CFE_CPU_NAME, "CFE CPU Name");
}

void Test_CFE_PSP_Main_PRSubtype3(void)
{
    char *argv = 0;

    char *dummy_argv[] = { "core-bin", "--reset", "PR", "--subtype", "3", 0 };
	int argc = CFE_PSP_NewArgv(&argv, dummy_argv);

	/* Execute Test */
	PSP_Main(argc, argv);

	CFE_PSP_DeleteOSResources();

	free(argv);

	/* Verify Outputs */
	UtAssert_True(PSP_UT_StubData.CFE.CallCounts.CFE_ES_Main_Func == 1, "CFE Initialized");
	UtAssert_True(PSP_UT_StubData.CFE.Main_Args.ModeId == 1, "CFE Start Mode");
	UtAssert_True(PSP_UT_StubData.CFE.Main_Args.StartType == 1, "CFE Reset Type");
	UtAssert_True(PSP_UT_StubData.CFE.Main_Args.StartSubtype == 3, "CFE Reset Subtype");
	UtAssert_True(CFE_PSP_GlobalData.CFE_PSP_SpacecraftId == CFE_SPACECRAFT_ID, "CFE Spacecraft ID");
	UtAssert_True(CFE_PSP_GlobalData.CFE_PSP_CpuId == CFE_CPU_ID, "CFE CPU ID");
	UtAssert_StrCmp(CFE_PSP_GlobalData.CFE_PSP_CpuName, CFE_CPU_NAME, "CFE CPU Name");
}

void Test_CFE_PSP_Main_PRSubtype4(void)
{
    char *argv = 0;

    char *dummy_argv[] = { "core-bin", "--reset", "PR", "--subtype", "4", 0 };
	int argc = CFE_PSP_NewArgv(&argv, dummy_argv);

	/* Execute Test */
	PSP_Main(argc, argv);

	CFE_PSP_DeleteOSResources();

	free(argv);

	/* Verify Outputs */
	UtAssert_True(PSP_UT_StubData.CFE.CallCounts.CFE_ES_Main_Func == 1, "CFE Initialized");
	UtAssert_True(PSP_UT_StubData.CFE.Main_Args.ModeId == 1, "CFE Start Mode");
	UtAssert_True(PSP_UT_StubData.CFE.Main_Args.StartType == 1, "CFE Reset Type");
	UtAssert_True(PSP_UT_StubData.CFE.Main_Args.StartSubtype == 4, "CFE Reset Subtype");
	UtAssert_True(CFE_PSP_GlobalData.CFE_PSP_SpacecraftId == CFE_SPACECRAFT_ID, "CFE Spacecraft ID");
	UtAssert_True(CFE_PSP_GlobalData.CFE_PSP_CpuId == CFE_CPU_ID, "CFE CPU ID");
	UtAssert_StrCmp(CFE_PSP_GlobalData.CFE_PSP_CpuName, CFE_CPU_NAME, "CFE CPU Name");
}

void Test_CFE_PSP_Main_PRSubtype5(void)
{
    char *argv = 0;

    char *dummy_argv[] = { "core-bin", "--reset", "PR", "--subtype", "5", 0 };
	int argc = CFE_PSP_NewArgv(&argv, dummy_argv);

	/* Execute Test */
	PSP_Main(argc, argv);

	CFE_PSP_DeleteOSResources();

	free(argv);

	/* Verify Outputs */
	UtAssert_True(PSP_UT_StubData.CFE.CallCounts.CFE_ES_Main_Func == 1, "CFE Initialized");
	UtAssert_True(PSP_UT_StubData.CFE.Main_Args.ModeId == 1, "CFE Start Mode");
	UtAssert_True(PSP_UT_StubData.CFE.Main_Args.StartType == 1, "CFE Reset Type");
	UtAssert_True(PSP_UT_StubData.CFE.Main_Args.StartSubtype == 5, "CFE Reset Subtype");
	UtAssert_True(CFE_PSP_GlobalData.CFE_PSP_SpacecraftId == CFE_SPACECRAFT_ID, "CFE Spacecraft ID");
	UtAssert_True(CFE_PSP_GlobalData.CFE_PSP_CpuId == CFE_CPU_ID, "CFE CPU ID");
	UtAssert_StrCmp(CFE_PSP_GlobalData.CFE_PSP_CpuName, CFE_CPU_NAME, "CFE CPU Name");
}

void Test_CFE_PSP_Main_PRInvalidSubtype(void)
{
    char *argv = 0;

    char *dummy_argv[] = { "core-bin", "--reset", "PR", "--subtype", "6", 0 };
	int argc = CFE_PSP_NewArgv(&argv, dummy_argv);

	/* Execute Test */
	PSP_Main(argc, argv);

	CFE_PSP_DeleteOSResources();

	free(argv);

	/* Verify Outputs */
	UtAssert_True(PSP_UT_StubData.CFE.CallCounts.CFE_ES_Main_Func == 1, "CFE Initialized");
	UtAssert_True(PSP_UT_StubData.CFE.Main_Args.ModeId == 1, "CFE Start Mode");
	UtAssert_True(PSP_UT_StubData.CFE.Main_Args.StartType == 1, "CFE Reset Type");
	UtAssert_True(PSP_UT_StubData.CFE.Main_Args.StartSubtype == 1, "CFE Reset Subtype");
	UtAssert_True(CFE_PSP_GlobalData.CFE_PSP_SpacecraftId == CFE_SPACECRAFT_ID, "CFE Spacecraft ID");
	UtAssert_True(CFE_PSP_GlobalData.CFE_PSP_CpuId == CFE_CPU_ID, "CFE CPU ID");
	UtAssert_StrCmp(CFE_PSP_GlobalData.CFE_PSP_CpuName, CFE_CPU_NAME, "CFE CPU Name");
}

void Test_CFE_PSP_Main_POCpuName(void)
{
    char *argv = 0;

    char *dummy_argv[] = { "core-bin", "--cpuname", "airliner", 0 };
	int argc = CFE_PSP_NewArgv(&argv, dummy_argv);

	/* Execute Test */
	PSP_Main(argc, argv);

	CFE_PSP_DeleteOSResources();

	free(argv);

	/* Verify Outputs */
	UtAssert_True(PSP_UT_StubData.CFE.CallCounts.CFE_ES_Main_Func == 1, "CFE Initialized");
	UtAssert_True(PSP_UT_StubData.CFE.Main_Args.ModeId == 1, "CFE Start Mode");
	UtAssert_True(PSP_UT_StubData.CFE.Main_Args.StartType == 2, "CFE Reset Type");
	UtAssert_True(PSP_UT_StubData.CFE.Main_Args.StartSubtype == 1, "CFE Reset Subtype");
	UtAssert_True(CFE_PSP_GlobalData.CFE_PSP_SpacecraftId == CFE_SPACECRAFT_ID, "CFE Spacecraft ID");
	UtAssert_True(CFE_PSP_GlobalData.CFE_PSP_CpuId == CFE_CPU_ID, "CFE CPU ID");
	UtAssert_StrCmp(CFE_PSP_GlobalData.CFE_PSP_CpuName, "airliner", "CFE CPU Name");
}

void Test_CFE_PSP_Main_POCpuId(void)
{
    char *argv = 0;

    uint32 expID = 3;
    uint32 actID = 0;

    char *dummy_argv[] = { "core-bin", "--cpuid", "3", 0 };
	int argc = CFE_PSP_NewArgv(&argv, dummy_argv);

	/* Execute Test */
	PSP_Main(argc, argv);

	CFE_PSP_DeleteOSResources();

	actID = CFE_PSP_GetProcessorId();

	free(argv);

	/* Verify Outputs */
	UtAssert_True(PSP_UT_StubData.CFE.CallCounts.CFE_ES_Main_Func == 1, "CFE Initialized");
	UtAssert_True(PSP_UT_StubData.CFE.Main_Args.ModeId == 1, "CFE Start Mode");
	UtAssert_True(PSP_UT_StubData.CFE.Main_Args.StartType == 2, "CFE Reset Type");
	UtAssert_True(PSP_UT_StubData.CFE.Main_Args.StartSubtype == 1, "CFE Reset Subtype");
	UtAssert_True(CFE_PSP_GlobalData.CFE_PSP_SpacecraftId == CFE_SPACECRAFT_ID, "CFE Spacecraft ID");
	UtAssert_True(CFE_PSP_GlobalData.CFE_PSP_CpuId == 3, "CFE CPU ID");
	UtAssert_True(actID == 3, "CFE CPU ID API");
	UtAssert_StrCmp(CFE_PSP_GlobalData.CFE_PSP_CpuName, CFE_CPU_NAME, "CFE CPU Name");
}

void Test_CFE_PSP_Main_POSpacecraftId(void)
{
    char *argv = 0;

    uint32 expID = 35;
    uint32 actID = 0;

    char *dummy_argv[] = { "core-bin", "--scid", "35", 0 };
	int argc = CFE_PSP_NewArgv(&argv, dummy_argv);

	/* Execute Test */
	PSP_Main(argc, argv);

	CFE_PSP_DeleteOSResources();

	actID = CFE_PSP_GetSpacecraftId();

	free(argv);

	/* Verify Outputs */
	UtAssert_True(PSP_UT_StubData.CFE.CallCounts.CFE_ES_Main_Func == 1, "CFE Initialized");
	UtAssert_True(PSP_UT_StubData.CFE.Main_Args.ModeId == 1, "CFE Start Mode");
	UtAssert_True(PSP_UT_StubData.CFE.Main_Args.StartType == 2, "CFE Reset Type");
	UtAssert_True(PSP_UT_StubData.CFE.Main_Args.StartSubtype == 1, "CFE Reset Subtype");
	UtAssert_True(CFE_PSP_GlobalData.CFE_PSP_SpacecraftId == expID, "CFE Spacecraft ID");
	UtAssert_True(actID == expID, "CFE Spacecraft ID API");
	UtAssert_True(CFE_PSP_GlobalData.CFE_PSP_CpuId == CFE_CPU_ID, "CFE CPU ID");
	UtAssert_StrCmp(CFE_PSP_GlobalData.CFE_PSP_CpuName, CFE_CPU_NAME, "CFE CPU Name");
}

void Test_CFE_PSP_Main_InvalidCmdArg(void)
{
    char *argv = 0;

    char *dummy_argv[] = { "core-bin", "--hello", "3", 0 };
	int argc = CFE_PSP_NewArgv(&argv, dummy_argv);

	/* Execute Test */
	PSP_Main(argc, argv);

	CFE_PSP_DeleteOSResources();

	free(argv);

	/* Verify Outputs */
	UtAssert_True(PSP_UT_StubData.CFE.CallCounts.CFE_ES_Main_Func == 1, "CFE Initialized");
	UtAssert_True(PSP_UT_StubData.CFE.Main_Args.ModeId == 1, "CFE Start Mode");
	UtAssert_True(PSP_UT_StubData.CFE.Main_Args.StartType == 2, "CFE Reset Type");
	UtAssert_True(PSP_UT_StubData.CFE.Main_Args.StartSubtype == 1, "CFE Reset Subtype");
	UtAssert_True(CFE_PSP_GlobalData.CFE_PSP_SpacecraftId == CFE_SPACECRAFT_ID, "CFE Spacecraft ID");
	UtAssert_True(CFE_PSP_GlobalData.CFE_PSP_CpuId == CFE_CPU_ID, "CFE CPU ID");
	UtAssert_StrCmp(CFE_PSP_GlobalData.CFE_PSP_CpuName, CFE_CPU_NAME, "CFE CPU Name");
}


void Test_CFE_PSP_Main_DisplayUsage(void)
{
	/* Setup Inputs */
	char name[] = "core-cfe.exe";

    CFE_PSP_DisplayUsage(name);

	UtAssert_True(PSP_UT_StubData.CFE.CallCounts.CFE_ES_Main_Func == 0, "CFE Not Initialized");
}


void Test_CFE_PSP_AttachExceptions(void)
{
	CFE_PSP_AttachExceptions();
}


#define PSP_UT_EXCEPTION_REASON_STRING_LENGTH 100
void Test_CFE_PSP_ExceptionHook(void)
{
	/* Setup Inputs */
	int task_id = 9;
	int vector = 10;
	char expReasonString[PSP_UT_EXCEPTION_REASON_STRING_LENGTH];
	CFE_PSP_ExceptionContext_t CFE_PSP_ExceptionContext_tmp;

	snprintf(expReasonString, PSP_UT_EXCEPTION_REASON_STRING_LENGTH,
			"Processor Exception %d, task ID %d", vector, task_id);

	/* Setup Inputs */
	memset(&CFE_PSP_ExceptionContext_tmp, 0, sizeof(CFE_PSP_ExceptionContext_t));
	CFE_PSP_ExceptionContext_tmp.regs[0] = 0x12;
	CFE_PSP_ExceptionContext_tmp.regs[1] = 0x34;
	CFE_PSP_ExceptionContext_tmp.regs[30] = 0x56;
	CFE_PSP_ExceptionContext_tmp.regs[31] = 0x78;

	CFE_PSP_ExceptionHook(task_id, vector, (uint8*)&CFE_PSP_ExceptionContext_tmp);

	UtAssert_True(PSP_UT_StubData.CFE.CallCounts.CFE_ES_ProcessCoreException_Func == 1, "CFE ES ProcessCoreException called");
	UtAssert_True(PSP_UT_StubData.CFE.ProcessCoreException_Args.HostTaskId == task_id, "CFE ES ProcessCoreException called with correct Host Task ID");
	UtAssert_StrCmp(PSP_UT_StubData.CFE.ProcessCoreException_Args.ReasonString, expReasonString, "ProcessorCoreException reason string");
	UtAssert_MemCmp(PSP_UT_StubData.CFE.ProcessCoreException_Args.ContextPointer,
			&CFE_PSP_ExceptionContext_tmp, sizeof(CFE_PSP_ExceptionContext_t), "ProcessorCoreException context saved");
}


void Test_CFE_PSP_SetDefaultExceptionEnvironment(void)
{
	CFE_PSP_SetDefaultExceptionEnvironment();
}


void Test_CFE_PSP_SigintHandler(void)
{
    char *argv = 0;

    char *dummy_argv[] = { "core-bin", "--reset", "PO", "--runmode", "NORMAL", 0 };
	int argc = CFE_PSP_NewArgv(&argv, dummy_argv);

	/* Execute Test */
	PSP_Main(argc, argv);

	CFE_PSP_SigintHandler();

	free(argv);

	/* Verify Outputs.  The SigintHandler function should kill the process so
	 * so the next line of code should never get touched.  Therefore, we
	 * test that 1 != 1 which should inherently fail.
	 */
	UtAssert_True(1 != 1, "CFE initialized");
}


void Test_CFE_PSP_TimerHandler(void)
{
    char *argv = 0;

    char *dummy_argv[] = { "core-bin", "--reset", "PO", "--runmode", "NORMAL", 0 };
	int argc = CFE_PSP_NewArgv(&argv, dummy_argv);

	/* Execute Test */
	PSP_Main(argc, argv);

	CFE_PSP_TimerHandler();
	CFE_PSP_TimerHandler();
	CFE_PSP_TimerHandler();
	CFE_PSP_TimerHandler();

	free(argv);

	UtAssert_True(PSP_UT_StubData.CFE.CallCounts.CFE_ES_Main_Func == 1, "CFE Initialized");
	UtAssert_True(CFE_PSP_GlobalData.TimerCounter == 4, "Timer counter incremented");
	UtAssert_True(PSP_UT_StubData.CFE.CallCounts.CFE_TIME_Local1HzISR_Func == 1, "CFE ES ProcessCoreException called");
}


#define PSP_UT_EEPROM_SIZE 1000
void Test_CFE_PSP_SetupEEPROM(void)
{
    char *argv = 0;
    uint32 EEPROMSize = PSP_UT_EEPROM_SIZE;
    uint32 *EEPROMAddress = 5;
    int32 rc = 5;

    char *dummy_argv[] = { "core-bin", "--reset", "PO", "--runmode", "NORMAL", 0 };
	int argc = CFE_PSP_NewArgv(&argv, dummy_argv);

	/* Execute Test */
	PSP_Main(argc, argv);

	rc = CFE_PSP_SetupEEPROM(EEPROMSize, EEPROMAddress);

	free(argv);

	UtAssert_True(PSP_UT_StubData.CFE.CallCounts.CFE_ES_Main_Func == 1, "CFE Initialized");
	UtAssert_True(rc == 0, "CFE_PSP_SetupEEPROM return code");
}


void Test_CFE_PSP_InitSSR(void)
{
	uint32 bus = 1;
	uint32 device = 2;
	char DeviceName[] = "device_name";
	int32 rc = 5;

	rc = CFE_PSP_InitSSR(bus, device, DeviceName);

	UtAssert_True(rc == CFE_PSP_ERROR, "SSR not implemented");
}


void Test_CFE_PSP_RestartPO(void)
{
    char *argv = 0;

    char *dummy_argv[] = { "core-bin", "--reset", "PO", "--runmode", "NORMAL", 0 };
	int argc = CFE_PSP_NewArgv(&argv, dummy_argv);

	/* Execute Test */
	PSP_Main(argc, argv);

	CFE_PSP_Restart(CFE_ES_POWERON_RESET);

	free(argv);

	/* Verify Outputs.  The CFE_PSP_Restart function should kill the process so
	 * so the next line of code should never get touched.  Therefore, we
	 * test that 1 != 1 which should inherently fail.
	 */
	UtAssert_True(1 != 1, "CFE initialized");
}


void Test_CFE_PSP_RestartPR(void)
{
    char *argv = 0;

    char *dummy_argv[] = { "core-bin", "--reset", "PO", "--runmode", "NORMAL", 0 };
	int argc = CFE_PSP_NewArgv(&argv, dummy_argv);

	/* Execute Test */
	PSP_Main(argc, argv);

	CFE_PSP_Restart(CFE_ES_PROCESSOR_RESET);

	free(argv);

	/* Verify Outputs.  The CFE_PSP_Restart function should kill the process so
	 * so the next line of code should never get touched.  Therefore, we
	 * test that 1 != 1 which should inherently fail.
	 */
	UtAssert_True(1 != 1, "CFE initialized");
}


void Test_CFE_PSP_Panic(void)
{
    char *argv = 0;

    char *dummy_argv[] = { "core-bin", "--reset", "PO", "--runmode", "NORMAL", 0 };
	int argc = CFE_PSP_NewArgv(&argv, dummy_argv);

	/* Execute Test */
	PSP_Main(argc, argv);

	CFE_PSP_Panic(1);

	free(argv);

	/* Verify Outputs.  The CFE_PSP_Restart function should kill the process so
	 * so the next line of code should never get touched.  Therefore, we
	 * test that 1 != 1 which should inherently fail.
	 */
	UtAssert_True(1 != 1, "CFE initialized");
}


void Test_CFE_PSP_FlushCaches(void)
{
    char *argv = 0;

    char *dummy_argv[] = { "core-bin", "--reset", "PO", "--runmode", "NORMAL", 0 };
	int argc = CFE_PSP_NewArgv(&argv, dummy_argv);

	/* Execute Test */
	PSP_Main(argc, argv);

	CFE_PSP_FlushCaches(0, 0, 0);

	free(argv);

	/* Verify Outputs.  */
	UtAssert_True(PSP_UT_StubData.CFE.CallCounts.CFE_ES_Main_Func == 1, "CFE Initialized");
}


void Test_CFE_PSP_GetTime(void)
{
    char *argv = 0;
    OS_time_t actTime;

    char *dummy_argv[] = { "core-bin", "--reset", "PO", "--runmode", "NORMAL", 0 };
	int argc = CFE_PSP_NewArgv(&argv, dummy_argv);

	/* Execute Test */
	PSP_Main(argc, argv);

	CFE_PSP_GetTime(&actTime);

	free(argv);

	/* Verify Outputs.  */
	UtAssert_True(PSP_UT_StubData.CFE.CallCounts.CFE_ES_Main_Func == 1, "CFE Initialized");
	UtAssert_True(actTime.seconds == 123, "CFE_PSP_GetTime seconds");
	UtAssert_True(actTime.microsecs == 456, "CFE_PSP_GetTime seconds");
}


void Test_CFE_PSP_Get_Timer_Tick(void)
{
    char *argv = 0;
    uint32 tick = 5;

    char *dummy_argv[] = { "core-bin", "--reset", "PO", "--runmode", "NORMAL", 0 };
	int argc = CFE_PSP_NewArgv(&argv, dummy_argv);

	/* Execute Test */
	PSP_Main(argc, argv);

	tick = CFE_PSP_Get_Timer_Tick();

	free(argv);

	/* Verify Outputs.  */
	UtAssert_True(PSP_UT_StubData.CFE.CallCounts.CFE_ES_Main_Func == 1, "CFE Initialized");
	UtAssert_True(tick == 0, "CFE_PSP_Get_Timer_Tick");
}


#define CFE_PSP_TIMER_TICKS_PER_SECOND       1000000
void Test_CFE_PSP_GetTimerTicksPerSecond(void)
{
    char *argv = 0;
    uint32 ticks = 5;

    char *dummy_argv[] = { "core-bin", "--reset", "PO", "--runmode", "NORMAL", 0 };
	int argc = CFE_PSP_NewArgv(&argv, dummy_argv);

	/* Execute Test */
	PSP_Main(argc, argv);

	ticks = CFE_PSP_GetTimerTicksPerSecond();

	free(argv);

	/* Verify Outputs.  */
	UtAssert_True(PSP_UT_StubData.CFE.CallCounts.CFE_ES_Main_Func == 1, "CFE Initialized");
	UtAssert_True(ticks == CFE_PSP_TIMER_TICKS_PER_SECOND, "CFE_PSP_GetTimerTicksPerSecond");
}


#define CFE_PSP_TIMER_LOW32_ROLLOVER         1000000
void Test_CFE_PSP_GetTimerLow32Rollover(void)
{
    char *argv = 0;
    uint32 value = 5;

    char *dummy_argv[] = { "core-bin", "--reset", "PO", "--runmode", "NORMAL", 0 };
	int argc = CFE_PSP_NewArgv(&argv, dummy_argv);

	/* Execute Test */
	PSP_Main(argc, argv);

	value = CFE_PSP_GetTimerLow32Rollover();

	free(argv);

	/* Verify Outputs.  */
	UtAssert_True(PSP_UT_StubData.CFE.CallCounts.CFE_ES_Main_Func == 1, "CFE Initialized");
	UtAssert_True(value == CFE_PSP_TIMER_LOW32_ROLLOVER, "CFE_PSP_GetTimerLow32Rollover");
}


void Test_CFE_PSP_Get_Timebase(void)
{
    char *argv = 0;
    uint32 Tbu;
    uint32 Tbl;

    char *dummy_argv[] = { "core-bin", "--reset", "PO", "--runmode", "NORMAL", 0 };
	int argc = CFE_PSP_NewArgv(&argv, dummy_argv);

	/* Execute Test */
	PSP_Main(argc, argv);

	CFE_PSP_Get_Timebase(&Tbu, &Tbl);

	free(argv);

	/* Verify Outputs.  */
	UtAssert_True(PSP_UT_StubData.CFE.CallCounts.CFE_ES_Main_Func == 1, "CFE Initialized");
	UtAssert_True(Tbu == 123, "Test_CFE_PSP_Get_Timebase upper");
	UtAssert_True(Tbl == 456, "Test_CFE_PSP_Get_Timebase lower");
}


void Test_CFE_PSP_Get_Dec(void)
{
    char *argv = 0;
    uint32 value = 5;

    char *dummy_argv[] = { "core-bin", "--reset", "PO", "--runmode", "NORMAL", 0 };
	int argc = CFE_PSP_NewArgv(&argv, dummy_argv);

	/* Execute Test */
	PSP_Main(argc, argv);

	value = CFE_PSP_Get_Dec();

	free(argv);

	/* Verify Outputs.  */
	UtAssert_True(PSP_UT_StubData.CFE.CallCounts.CFE_ES_Main_Func == 1, "CFE Initialized");
	UtAssert_True(value == 0, "CFE_PSP_Get_Dec not supported");
}


void Test_CFE_PSP_WatchdogInit(void)
{
    char *argv = 0;

    char *dummy_argv[] = { "core-bin", "--reset", "PO", "--runmode", "NORMAL", 0 };
	int argc = CFE_PSP_NewArgv(&argv, dummy_argv);

	/* Execute Test */
	PSP_Main(argc, argv);

	CFE_PSP_WatchdogInit();

	free(argv);

	/* Verify Outputs.  */
	UtAssert_True(PSP_UT_StubData.CFE.CallCounts.CFE_ES_Main_Func == 1, "CFE Initialized");
}


void Test_CFE_PSP_WatchdogEnable(void)
{
    char *argv = 0;

    char *dummy_argv[] = { "core-bin", "--reset", "PO", "--runmode", "NORMAL", 0 };
	int argc = CFE_PSP_NewArgv(&argv, dummy_argv);

	/* Execute Test */
	PSP_Main(argc, argv);

	CFE_PSP_WatchdogEnable();

	free(argv);

	/* Verify Outputs.  */
	UtAssert_True(PSP_UT_StubData.CFE.CallCounts.CFE_ES_Main_Func == 1, "CFE Initialized");
}


void Test_CFE_PSP_WatchdogDisable(void)
{
    char *argv = 0;

    char *dummy_argv[] = { "core-bin", "--reset", "PO", "--runmode", "NORMAL", 0 };
	int argc = CFE_PSP_NewArgv(&argv, dummy_argv);

	/* Execute Test */
	PSP_Main(argc, argv);

	CFE_PSP_WatchdogDisable();

	free(argv);

	/* Verify Outputs.  */
	UtAssert_True(PSP_UT_StubData.CFE.CallCounts.CFE_ES_Main_Func == 1, "CFE Initialized");
}


void Test_CFE_PSP_WatchdogService(void)
{
    char *argv = 0;

    char *dummy_argv[] = { "core-bin", "--reset", "PO", "--runmode", "NORMAL", 0 };
	int argc = CFE_PSP_NewArgv(&argv, dummy_argv);

	/* Execute Test */
	PSP_Main(argc, argv);

	CFE_PSP_WatchdogService();

	free(argv);

	/* Verify Outputs.  */
	UtAssert_True(PSP_UT_StubData.CFE.CallCounts.CFE_ES_Main_Func == 1, "CFE Initialized");
}


void Test_CFE_PSP_WatchdogSetGet(void)
{
    char *argv = 0;
    uint32 expValue = 7;
    uint32 actValue = 0;

    char *dummy_argv[] = { "core-bin", "--reset", "PO", "--runmode", "NORMAL", 0 };
	int argc = CFE_PSP_NewArgv(&argv, dummy_argv);

	/* Execute Test */
	PSP_Main(argc, argv);

	CFE_PSP_WatchdogSet(expValue);
	actValue = CFE_PSP_WatchdogGet();

	free(argv);

	/* Verify Outputs.  */
	UtAssert_True(PSP_UT_StubData.CFE.CallCounts.CFE_ES_Main_Func == 1, "CFE Initialized");
	UtAssert_True(expValue == actValue, "Watchdog get");
}


void Test_CFE_PSP_GetCDSSizeNominal(void)
{
    char *argv = 0;
    uint32 size = 0;
    int32 rc = 5;

    char *dummy_argv[] = { "core-bin", "--reset", "PO", "--runmode", "NORMAL", 0 };
	int argc = CFE_PSP_NewArgv(&argv, dummy_argv);

	/* Execute Test */
	PSP_Main(argc, argv);

	rc = CFE_PSP_GetCDSSize(&size);

	free(argv);

	/* Verify Outputs.  */
	UtAssert_True(PSP_UT_StubData.CFE.CallCounts.CFE_ES_Main_Func == 1, "CFE Initialized");
	UtAssert_True(rc == CFE_PSP_SUCCESS, "CFE_PSP_GetCDSSize return code");
	UtAssert_True(size == CFE_ES_CDS_SIZE, "CFE_PSP_GetCDSSize size returned");
}


void Test_CFE_PSP_GetCDSSizeNullPointer(void)
{
    char *argv = 0;
    int32 rc = 5;

    char *dummy_argv[] = { "core-bin", "--reset", "PO", "--runmode", "NORMAL", 0 };
	int argc = CFE_PSP_NewArgv(&argv, dummy_argv);

	/* Execute Test */
	PSP_Main(argc, argv);

	rc = CFE_PSP_GetCDSSize(0);

	free(argv);

	/* Verify Outputs.  */
	UtAssert_True(PSP_UT_StubData.CFE.CallCounts.CFE_ES_Main_Func == 1, "CFE Initialized");
	UtAssert_True(rc == CFE_PSP_ERROR, "CFE_PSP_GetCDSSize return code");
}


#define PSP_UT_CDS_BUFFER_SIZE	1000
#define PSP_UT_CDS_OFFSET		50
void Test_CFE_PSP_WriteToCDSNominal(void)
{
    char *argv = 0;
    int32 rc = 5;
    char inBuffer[PSP_UT_CDS_BUFFER_SIZE];
    char outBuffer[PSP_UT_CDS_BUFFER_SIZE];

    char *dummy_argv[] = { "core-bin", "--reset", "PO", "--runmode", "NORMAL", 0 };
	int argc = CFE_PSP_NewArgv(&argv, dummy_argv);

	/* Just so we have a creative looking buffer to verify, lets fill it with
	 * the fibonacci series.
	 */
	inBuffer[0] = 1;
	inBuffer[1] = 1;
	for(uint32 i = 2; i < PSP_UT_CDS_BUFFER_SIZE; ++i)
	{
		inBuffer[i] = inBuffer[i-2] + inBuffer[i-1];
	}

	/* Execute Test */
	PSP_Main(argc, argv);
	UtAssert_True(PSP_UT_StubData.CFE.CallCounts.CFE_ES_Main_Func == 1, "CFE Initialized");

	/* First lets verify the CDS is empty. */
	rc = CFE_PSP_ReadFromCDS(outBuffer, PSP_UT_CDS_OFFSET, PSP_UT_CDS_BUFFER_SIZE);
	UtAssert_True(rc == CFE_PSP_SUCCESS, "CFE_PSP_ReadFromCDS (1) return code");
	UtAssert_MemCmpValue(outBuffer, 0, PSP_UT_CDS_BUFFER_SIZE, "CDS empty");

	/* Now write to the CDS. */
	rc = CFE_PSP_WriteToCDS((void*)inBuffer, PSP_UT_CDS_OFFSET, PSP_UT_CDS_BUFFER_SIZE);
	UtAssert_True(rc == CFE_PSP_SUCCESS, "CFE_PSP_WriteToCDS return code");

	/* Finally, read it back and verify its correct. */
	rc = CFE_PSP_ReadFromCDS(outBuffer, PSP_UT_CDS_OFFSET, PSP_UT_CDS_BUFFER_SIZE);
	UtAssert_True(rc == CFE_PSP_SUCCESS, "CFE_PSP_ReadFromCDS (2) return code");
	UtAssert_MemCmp(outBuffer, inBuffer, PSP_UT_CDS_BUFFER_SIZE, "CDS content verification");

	free(argv);
}


void Test_CFE_PSP_WriteToCDSNullPointer(void)
{
    char *argv = 0;
    int32 rc = 5;

    char *dummy_argv[] = { "core-bin", "--reset", "PO", "--runmode", "NORMAL", 0 };
	int argc = CFE_PSP_NewArgv(&argv, dummy_argv);

	/* Execute Test */
	PSP_Main(argc, argv);

	rc = CFE_PSP_WriteToCDS(0, PSP_UT_CDS_OFFSET, PSP_UT_CDS_BUFFER_SIZE);

	UtAssert_True(PSP_UT_StubData.CFE.CallCounts.CFE_ES_Main_Func == 1, "CFE Initialized");

	UtAssert_True(rc == CFE_PSP_ERROR, "CFE_PSP_WriteToCDS return code");

	free(argv);
}


void Test_CFE_PSP_WriteToCDSOffsetOutOfBounds(void)
{
    char *argv = 0;
    int32 rc = 5;
    char inBuffer[PSP_UT_CDS_BUFFER_SIZE];

    char *dummy_argv[] = { "core-bin", "--reset", "PO", "--runmode", "NORMAL", 0 };
	int argc = CFE_PSP_NewArgv(&argv, dummy_argv);

	/* Execute Test */
	PSP_Main(argc, argv);

	rc = CFE_PSP_WriteToCDS(inBuffer,
			CFE_ES_CDS_SIZE - PSP_UT_CDS_BUFFER_SIZE,
			PSP_UT_CDS_BUFFER_SIZE);

	UtAssert_True(PSP_UT_StubData.CFE.CallCounts.CFE_ES_Main_Func == 1, "CFE Initialized");
	UtAssert_True(rc == CFE_PSP_SUCCESS, "CFE_PSP_WriteToCDS (1) return code");

	rc = CFE_PSP_WriteToCDS(inBuffer,
			CFE_ES_CDS_SIZE - PSP_UT_CDS_BUFFER_SIZE + 1,
			PSP_UT_CDS_BUFFER_SIZE);

	UtAssert_True(rc == CFE_PSP_ERROR, "CFE_PSP_WriteToCDS (2) return code");

	free(argv);
}


void Test_CFE_PSP_WriteToCDSSizeOutOfBounds(void)
{
    char *argv = 0;
    int32 rc = 5;
    char *outBuffer;

    char *dummy_argv[] = { "core-bin", "--reset", "PO", "--runmode", "NORMAL", 0 };
	int argc = CFE_PSP_NewArgv(&argv, dummy_argv);

	/* Execute Test */
	PSP_Main(argc, argv);

	rc = CFE_PSP_WriteToCDS(outBuffer,
			CFE_ES_CDS_SIZE+1,
			PSP_UT_CDS_BUFFER_SIZE);

	UtAssert_True(PSP_UT_StubData.CFE.CallCounts.CFE_ES_Main_Func == 1, "CFE Initialized");
	UtAssert_True(rc == CFE_PSP_ERROR, "CFE_PSP_ReadFromCDS return code");

	free(argv);
}


void Test_CFE_PSP_ReadFromCDSNullPointer(void)
{
    char *argv = 0;
    int32 rc = 5;

    char *dummy_argv[] = { "core-bin", "--reset", "PO", "--runmode", "NORMAL", 0 };
	int argc = CFE_PSP_NewArgv(&argv, dummy_argv);

	/* Execute Test */
	PSP_Main(argc, argv);

	rc = CFE_PSP_ReadFromCDS(0, PSP_UT_CDS_OFFSET, PSP_UT_CDS_BUFFER_SIZE);

	UtAssert_True(PSP_UT_StubData.CFE.CallCounts.CFE_ES_Main_Func == 1, "CFE Initialized");

	UtAssert_True(rc == CFE_PSP_ERROR, "CFE_PSP_ReadFromCDS return code");

	free(argv);
}


void Test_CFE_PSP_ReadFromCDSOffsetOutOfBounds(void)
{
    char *argv = 0;
    int32 rc = 5;
    char outBuffer[PSP_UT_CDS_BUFFER_SIZE];

    char *dummy_argv[] = { "core-bin", "--reset", "PO", "--runmode", "NORMAL", 0 };
	int argc = CFE_PSP_NewArgv(&argv, dummy_argv);

	/* Execute Test */
	PSP_Main(argc, argv);
	UtAssert_True(PSP_UT_StubData.CFE.CallCounts.CFE_ES_Main_Func == 1, "CFE Initialized (1)");

	rc = CFE_PSP_ReadFromCDS(outBuffer,
			CFE_ES_CDS_SIZE - PSP_UT_CDS_BUFFER_SIZE,
			PSP_UT_CDS_BUFFER_SIZE);

	UtAssert_True(PSP_UT_StubData.CFE.CallCounts.CFE_ES_Main_Func == 1, "CFE Initialized (2)");
	UtAssert_True(rc == CFE_PSP_SUCCESS, "CFE_PSP_ReadFromCDS (1) return code");

	rc = CFE_PSP_ReadFromCDS(outBuffer,
			CFE_ES_CDS_SIZE - PSP_UT_CDS_BUFFER_SIZE + 1,
			PSP_UT_CDS_BUFFER_SIZE);

	UtAssert_True(rc == CFE_PSP_ERROR, "CFE_PSP_ReadFromCDS (2) return code");

	free(argv);
}


void Test_CFE_PSP_ReadFromCDSSizeOutOfBounds(void)
{
    char *argv = 0;
    int32 rc = 5;
    char outBuffer[PSP_UT_CDS_BUFFER_SIZE];

    char *dummy_argv[] = { "core-bin", "--reset", "PO", "--runmode", "NORMAL", 0 };
	int argc = CFE_PSP_NewArgv(&argv, dummy_argv);

	/* Execute Test */
	PSP_Main(argc, argv);

	rc = CFE_PSP_ReadFromCDS(outBuffer,
			CFE_ES_CDS_SIZE+1,
			PSP_UT_CDS_BUFFER_SIZE);

	UtAssert_True(PSP_UT_StubData.CFE.CallCounts.CFE_ES_Main_Func == 1, "CFE Initialized");
	UtAssert_True(rc == CFE_PSP_ERROR, "CFE_PSP_ReadFromCDS return code");

	free(argv);
}


void Test_CFE_PSP_GetResetArea(void)
{
    char *argv = 0;
    int32 rc = 5;
    cpuaddr buffer = 0;
    uint32 size = 0;

    char *dummy_argv[] = { "core-bin", "--reset", "PO", "--runmode", "NORMAL", 0 };
	int argc = CFE_PSP_NewArgv(&argv, dummy_argv);

	/* Execute Test */
	PSP_Main(argc, argv);
	UtAssert_True(PSP_UT_StubData.CFE.CallCounts.CFE_ES_Main_Func == 1, "CFE Initialized");

	rc = CFE_PSP_InitResetArea(CFE_ES_POWERON_RESET);
	UtAssert_True(rc == CFE_PSP_SUCCESS, "CFE_PSP_InitResetArea");

	rc = CFE_PSP_GetResetArea(&buffer, 0);
	UtAssert_True(rc == CFE_PSP_ERROR, "CFE_PSP_GetResetArea null pointer");

	rc = CFE_PSP_GetResetArea(&buffer, &size);
	UtAssert_True(rc == CFE_PSP_SUCCESS, "CFE_PSP_GetResetArea return code");
	UtAssert_True(buffer != 0, "CFE_PSP_GetResetArea address");

	free(argv);
}


void Test_CFE_PSP_GetUserReservedArea(void)
{
    char *argv = 0;
    int32 rc = 5;
    cpuaddr buffer = 0;
    uint32 size = 0;

    char *dummy_argv[] = { "core-bin", "--reset", "PO", "--runmode", "NORMAL", 0 };
	int argc = CFE_PSP_NewArgv(&argv, dummy_argv);

	/* Execute Test */
	PSP_Main(argc, argv);
	UtAssert_True(PSP_UT_StubData.CFE.CallCounts.CFE_ES_Main_Func == 1, "CFE Initialized");

	rc = CFE_PSP_InitUserReservedArea(CFE_ES_POWERON_RESET);
	UtAssert_True(rc == CFE_PSP_SUCCESS, "CFE_PSP_InitUserReservedArea");

	rc = CFE_PSP_GetUserReservedArea(&buffer, 0);
	UtAssert_True(rc == CFE_PSP_ERROR, "CFE_PSP_GetUserReservedArea null pointer");

	rc = CFE_PSP_GetUserReservedArea(&buffer, &size);
	UtAssert_True(rc == CFE_PSP_SUCCESS, "CFE_PSP_GetUserReservedArea return code");
	UtAssert_True(buffer != 0, "CFE_PSP_GetUserReservedArea address");

	free(argv);
}


void Test_CFE_PSP_GetVolatileDiskMem(void)
{
    char *argv = 0;
    int32 rc = 5;
    cpuaddr buffer = 0;
    uint32 size = 0;

    char *dummy_argv[] = { "core-bin", "--reset", "PO", "--runmode", "NORMAL", 0 };
	int argc = CFE_PSP_NewArgv(&argv, dummy_argv);

	/* Execute Test */
	PSP_Main(argc, argv);

	rc = CFE_PSP_GetVolatileDiskMem(&buffer, 0);
	UtAssert_True(PSP_UT_StubData.CFE.CallCounts.CFE_ES_Main_Func == 1, "CFE Initialized");
	UtAssert_True(rc == CFE_PSP_ERROR, "CFE_PSP_GetVolatileDiskMem null pointer");

	rc = CFE_PSP_GetVolatileDiskMem(&buffer, &size);
	UtAssert_True(rc == CFE_PSP_SUCCESS, "CFE_PSP_GetVolatileDiskMem return code");
	UtAssert_True(buffer == 0, "CFE_PSP_GetVolatileDiskMem address");

	free(argv);
}


void Test_CFE_PSP_GetKernelTextSegmentInfo(void)
{
    char *argv = 0;
    int32 rc = 5;
    cpuaddr kernel = 0;
    uint32 size = 0;

    char *dummy_argv[] = { "core-bin", "--reset", "PO", "--runmode", "NORMAL", 0 };
	int argc = CFE_PSP_NewArgv(&argv, dummy_argv);

	/* Execute Test */
	PSP_Main(argc, argv);

	rc = CFE_PSP_GetKernelTextSegmentInfo(0, &size);
	UtAssert_True(PSP_UT_StubData.CFE.CallCounts.CFE_ES_Main_Func == 1, "CFE Initialized");
	UtAssert_True(rc == CFE_PSP_ERROR, "CFE_PSP_GetKernelTextSegmentInfo kernel null pointer");

	rc = CFE_PSP_GetKernelTextSegmentInfo(&kernel, 0);
	UtAssert_True(rc == CFE_PSP_ERROR, "CFE_PSP_GetKernelTextSegmentInfo size null pointer");

	rc = CFE_PSP_GetKernelTextSegmentInfo(&kernel, &size);
	UtAssert_True(rc == CFE_PSP_ERROR_NOT_IMPLEMENTED, "CFE_PSP_GetKernelTextSegmentInfo not implemented");

	free(argv);
}


void Test_CFE_PSP_GetCFETextSegmentInfo(void)
{
    char *argv = 0;
    int32 rc = 5;
    cpuaddr segment = 0;
    uint32 size = 0;

    char *dummy_argv[] = { "core-bin", "--reset", "PO", "--runmode", "NORMAL", 0 };
	int argc = CFE_PSP_NewArgv(&argv, dummy_argv);

	/* Execute Test */
	PSP_Main(argc, argv);

	rc = CFE_PSP_GetCFETextSegmentInfo(&segment, 0);
	UtAssert_True(rc == CFE_PSP_ERROR, "CFE_PSP_GetCFETextSegmentInfo size null pointer");

	rc = CFE_PSP_GetCFETextSegmentInfo(&segment, &size);
	UtAssert_True(rc == CFE_PSP_SUCCESS, "CFE_PSP_GetCFETextSegmentInfo success");
	UtAssert_True(segment == &_init, "CFE_PSP_GetCFETextSegmentInfo segment");
	UtAssert_True(size == ((uint32) &_fini - (uint32) &_init), "CFE_PSP_GetCFETextSegmentInfo size");

	free(argv);
}


void PSP_Setup(void)
{
	optind = 1;

	remove(CFE_PSP_CDS_KEY_FILE);
	remove(CFE_PSP_RESET_KEY_FILE);
	remove(CFE_PSP_RESERVED_KEY_FILE);

	memset(&(CFE_PSP_GlobalData), 0, sizeof(CFE_PSP_GlobalData_t));
	PSP_UT_ResetStubData();
}


void PSP_TearDown(void)
{

}


void PSP_AddTestCases(void)
{
    /* void CFE_PSP_DrawBanner(void) */

    /*  1 */  UtTest_Add(Test_CFE_PSP_Main_NominalPO, PSP_Setup, PSP_TearDown, "Test_CFE_PSP_Main_NominalPO");
    /*  2 */  UtTest_Add(Test_CFE_PSP_Main_NominalPR, PSP_Setup, PSP_TearDown, "Test_CFE_PSP_Main_NominalPR");
    /*  3 */  UtTest_Add(Test_CFE_PSP_Main_IncorrectPRType, PSP_Setup, PSP_TearDown, "Test_CFE_PSP_Main_IncorrectPRType");
    /*  4 */  UtTest_Add(Test_CFE_PSP_Main_PORunmodeTest, PSP_Setup, PSP_TearDown, "Test_CFE_PSP_Main_PORunmodeTest");
    /*  5 */  UtTest_Add(Test_CFE_PSP_Main_POInvalidRunmode, PSP_Setup, PSP_TearDown, "Test_CFE_PSP_Main_POInvalidRunmode");
    /*  6 */  UtTest_Add(Test_CFE_PSP_Main_PRSubtype1, PSP_Setup, PSP_TearDown, "Test_CFE_PSP_Main_PRSubtype1");
    /*  7 */  UtTest_Add(Test_CFE_PSP_Main_PRSubtype2, PSP_Setup, PSP_TearDown, "Test_CFE_PSP_Main_PRSubtype2");
    /*  8 */  UtTest_Add(Test_CFE_PSP_Main_PRSubtype3, PSP_Setup, PSP_TearDown, "Test_CFE_PSP_Main_PRSubtype3");
    /*  9 */  UtTest_Add(Test_CFE_PSP_Main_PRSubtype4, PSP_Setup, PSP_TearDown, "Test_CFE_PSP_Main_PRSubtype4");
    /* 10 */  UtTest_Add(Test_CFE_PSP_Main_PRSubtype5, PSP_Setup, PSP_TearDown, "Test_CFE_PSP_Main_PRSubtype5");
    /* 11 */  UtTest_Add(Test_CFE_PSP_Main_PRInvalidSubtype, PSP_Setup, PSP_TearDown, "Test_CFE_PSP_Main_PRInvalidSubtype");
    /* 12 */  UtTest_Add(Test_CFE_PSP_Main_POCpuName, PSP_Setup, PSP_TearDown, "Test_CFE_PSP_Main_POCpuName");
    /* 13 */  UtTest_Add(Test_CFE_PSP_Main_POCpuId, PSP_Setup, PSP_TearDown, "Test_CFE_PSP_Main_POCpuId");
    /* 14 */  UtTest_Add(Test_CFE_PSP_Main_InvalidCmdArg, PSP_Setup, PSP_TearDown, "Test_CFE_PSP_Main_InvalidCmdArg");
    /* 15 */  UtTest_Add(Test_CFE_PSP_SigintHandler, PSP_Setup, PSP_TearDown, "Test_CFE_PSP_SigintHandler");
    /* 16 */  UtTest_Add(Test_CFE_PSP_TimerHandler, PSP_Setup, PSP_TearDown, "Test_CFE_PSP_TimerHandler");
    /* 17 */  UtTest_Add(Test_CFE_PSP_Main_DisplayUsage, PSP_Setup, PSP_TearDown, "Test_CFE_PSP_Main_DisplayUsage");
    /* 18 */  UtTest_Add(Test_CFE_PSP_AttachExceptions, PSP_Setup, PSP_TearDown, "Test_CFE_PSP_AttachExceptions");
    /* 19 */  UtTest_Add(Test_CFE_PSP_ExceptionHook, PSP_Setup, PSP_TearDown, "Test_CFE_PSP_ExceptionHook");
    /* 20 */  UtTest_Add(Test_CFE_PSP_SetDefaultExceptionEnvironment, PSP_Setup, PSP_TearDown, "Test_CFE_PSP_SetDefaultExceptionEnvironment");
    /* 21 */  UtTest_Add(Test_CFE_PSP_GetCDSSizeNominal, PSP_Setup, PSP_TearDown, "Test_CFE_PSP_GetCDSSizeNominal");
    /* 22 */  UtTest_Add(Test_CFE_PSP_GetCDSSizeNullPointer, PSP_Setup, PSP_TearDown, "Test_CFE_PSP_GetCDSSizeNullPointer");
    /* 23 */  UtTest_Add(Test_CFE_PSP_WriteToCDSNominal, PSP_Setup, PSP_TearDown, "Test_CFE_PSP_WriteToCDSNominal");
    /* 24 */  UtTest_Add(Test_CFE_PSP_WriteToCDSNullPointer, PSP_Setup, PSP_TearDown, "Test_CFE_PSP_WriteToCDSNullPointer");
    /* 25 */  UtTest_Add(Test_CFE_PSP_WriteToCDSOffsetOutOfBounds, PSP_Setup, PSP_TearDown, "Test_CFE_PSP_WriteToCDSOffsetOutOfBounds");
    /* 26 */  UtTest_Add(Test_CFE_PSP_WriteToCDSSizeOutOfBounds, PSP_Setup, PSP_TearDown, "Test_CFE_PSP_WriteToCDSSizeOutOfBounds");
    /* 27 */  UtTest_Add(Test_CFE_PSP_ReadFromCDSNullPointer, PSP_Setup, PSP_TearDown, "Test_CFE_PSP_ReadFromCDSNullPointer");
    /* 28 */  UtTest_Add(Test_CFE_PSP_ReadFromCDSOffsetOutOfBounds, PSP_Setup, PSP_TearDown, "Test_CFE_PSP_ReadFromCDSOffsetOutOfBounds");
    /* 29 */  UtTest_Add(Test_CFE_PSP_ReadFromCDSSizeOutOfBounds, PSP_Setup, PSP_TearDown, "Test_CFE_PSP_ReadFromCDSSizeOutOfBounds");
    /* 30 */  UtTest_Add(Test_CFE_PSP_GetResetArea, PSP_Setup, PSP_TearDown, "Test_CFE_PSP_GetResetArea");
    /* 31 */  UtTest_Add(Test_CFE_PSP_GetUserReservedArea, PSP_Setup, PSP_TearDown, "Test_CFE_PSP_GetUserReservedArea");
    /* 32 */  UtTest_Add(Test_CFE_PSP_GetVolatileDiskMem, PSP_Setup, PSP_TearDown, "Test_CFE_PSP_GetVolatileDiskMem");
    /* 33 */  UtTest_Add(Test_CFE_PSP_GetKernelTextSegmentInfo, PSP_Setup, PSP_TearDown, "Test_CFE_PSP_GetKernelTextSegmentInfo");
    /* 34 */  UtTest_Add(Test_CFE_PSP_GetCFETextSegmentInfo, PSP_Setup, PSP_TearDown, "Test_CFE_PSP_GetCFETextSegmentInfo");
    /* 35 */  UtTest_Add(Test_CFE_PSP_SetupEEPROM, PSP_Setup, PSP_TearDown, "Test_CFE_PSP_SetupEEPROM");
    /* 36 */  UtTest_Add(Test_CFE_PSP_InitSSR, PSP_Setup, PSP_TearDown, "Test_CFE_PSP_InitSSR");
    /* 37 */  UtTest_Add(Test_CFE_PSP_FlushCaches, PSP_Setup, PSP_TearDown, "Test_CFE_PSP_FlushCaches");
    /* 38 */  UtTest_Add(Test_CFE_PSP_Main_POSpacecraftId, PSP_Setup, PSP_TearDown, "Test_CFE_PSP_Main_POSpacecraftId");
    /* 39 */  UtTest_Add(Test_CFE_PSP_GetTime, PSP_Setup, PSP_TearDown, "Test_CFE_PSP_GetTime");
    /* 40 */  UtTest_Add(Test_CFE_PSP_Get_Timer_Tick, PSP_Setup, PSP_TearDown, "Test_CFE_PSP_Get_Timer_Tick");
    /* 41 */  UtTest_Add(Test_CFE_PSP_GetTimerTicksPerSecond, PSP_Setup, PSP_TearDown, "Test_CFE_PSP_GetTimerTicksPerSecond");
    /* 42 */  UtTest_Add(Test_CFE_PSP_GetTimerLow32Rollover, PSP_Setup, PSP_TearDown, "Test_CFE_PSP_GetTimerLow32Rollover");
    /* 43 */  UtTest_Add(Test_CFE_PSP_Get_Timebase, PSP_Setup, PSP_TearDown, "Test_CFE_PSP_Get_Timebase");
    /* 44 */  UtTest_Add(Test_CFE_PSP_Get_Dec, PSP_Setup, PSP_TearDown, "Test_CFE_PSP_Get_Dec");
    /* 45 */  UtTest_Add(Test_CFE_PSP_WatchdogInit, PSP_Setup, PSP_TearDown, "Test_CFE_PSP_WatchdogInit");
    /* 46 */  UtTest_Add(Test_CFE_PSP_WatchdogEnable, PSP_Setup, PSP_TearDown, "Test_CFE_PSP_WatchdogEnable");
    /* 47 */  UtTest_Add(Test_CFE_PSP_WatchdogDisable, PSP_Setup, PSP_TearDown, "Test_CFE_PSP_WatchdogDisable");
    /* 48 */  UtTest_Add(Test_CFE_PSP_WatchdogService, PSP_Setup, PSP_TearDown, "Test_CFE_PSP_WatchdogService");
    /* 49 */  UtTest_Add(Test_CFE_PSP_WatchdogSetGet, PSP_Setup, PSP_TearDown, "Test_CFE_PSP_WatchdogSetGet");

    //UtTest_Add(Test_CFE_PSP_RestartPO, PSP_Setup, PSP_TearDown, "Test_CFE_PSP_RestartPO");
    //UtTest_Add(Test_CFE_PSP_RestartPR, PSP_Setup, PSP_TearDown, "Test_CFE_PSP_RestartPR");
    //UtTest_Add(Test_CFE_PSP_Panic, PSP_Setup, PSP_TearDown, "Test_CFE_PSP_Panic");

}






