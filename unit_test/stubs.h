

typedef struct
{
    uint32 CFE_ES_Main_Func;
    uint32 CFE_ES_ProcessCoreException_Func;
    uint32 CFE_TIME_Local1HzISR_Func;
} PSP_UT_CFE_CallCounts_t;

typedef struct
{
    uint32 HostTaskId;
    char   ReasonString[256];
    uint32 *ContextPointer;
    uint32 ContextSize;
} PSP_UT_CFE_ProcessCoreException_Args_t;

typedef struct
{
    uint32 StartType;
    uint32 StartSubtype;
    uint32 ModeId;
    char StartFilePath[256];
} PSP_UT_CFE_ES_Main_Args_t;

typedef struct
{
    PSP_UT_CFE_CallCounts_t CallCounts;
    PSP_UT_CFE_ES_Main_Args_t Main_Args;
    PSP_UT_CFE_ProcessCoreException_Args_t ProcessCoreException_Args;
} PSP_UT_CFE_StubData_t;

typedef struct
{
    PSP_UT_CFE_StubData_t CFE;
} PSP_UT_StubData_t;


extern PSP_UT_StubData_t PSP_UT_StubData;


void PSP_UT_ResetStubData(void);
