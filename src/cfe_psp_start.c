/******************************************************************************
 ** File:  cfe_psp_start.c
 **
 **
 **      Copyright (c) 2004-2006, United States government as represented by the
 **      administrator of the National Aeronautics Space Administration.
 **      All rights reserved. This software(cFE) was created at NASA Goddard
 **      Space Flight Center pursuant to government contracts.
 **
 **      This software may be used only pursuant to a United States government
 **      sponsored project and the United States government may not be charged
 **      for use thereof.
 **
 **

 ** Purpose:
 **   cFE BSP main entry point.
 **
 ** History:
 **   2005/07/26  A. Cudmore      | Initial version for OS X/Linux
 **
 ******************************************************************************/

/*
 **  Include Files
 */
#define _GNU_SOURCE
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/time.h>
#include <getopt.h>
#include <string.h>
#include <limits.h>
#include <pthread.h>
#include <sched.h>
#include <sys/mman.h>
#include <time.h>

/*
 ** cFE includes
 */
#include "common_types.h"
#include "osapi.h"
#include "cfe_es.h"            /* For reset types */
#include "cfe_platform_cfg.h"  /* for processor ID */
#include "cfe_mission_cfg.h"   /* for Spacecraft ID */

#include "cfe_psp.h"
#include "cfe_psp_private.h"

/*
 ** Defines
 */
#ifndef SEM_VALUE_MAX
#define SEM_VALUE_MAX INT_MAX
#endif

#define CFE_PSP_RESET_NAME_LENGTH 10
#define CFE_PSP_MODE_NAME_LENGTH  10

/* Uncomment this define to enable simulated EEPROM support on the desktop
 * machine */
/* #define CFE_PSP_EEPROM_SUPPORT */

/*
 ** Typedefs for this module
 */

/*
 ** Structure for the Command line parameters
 */
typedef struct
{
    char ResetType[CFE_PSP_RESET_NAME_LENGTH]; /* Reset type can be "PO" for Power on or "PR" for Processor Reset */
    uint32 GotResetType; /* did we get the ResetType parameter ? */

    uint32 SubType; /* Reset Sub Type ( 1 - 5 )  */
    uint32 GotSubType; /* did we get the ResetSubType parameter ? */

    char CpuName[CFE_PSP_CPU_NAME_LENGTH]; /* CPU Name */
    uint32 GotCpuName; /* Did we get a CPU Name ? */

    uint32 CpuId; /* CPU ID */
    uint32 GotCpuId; /* Did we get a CPU Id ?*/

    uint32 SpacecraftId; /* Spacecraft ID */
    uint32 GotSpacecraftId; /* Did we get a Spacecraft ID */

    char RunMode[CFE_PSP_MODE_NAME_LENGTH]; /* Run Mode */
    uint32 GotRunMode; /* Did we get a Run Mode? */
    OS_RunTimeModeEnum_t RunModeEnum;
} CFE_PSP_CommandData_t;

/*
 ** Prototypes for this module
 */
void CFE_PSP_SigintHandler(int arg);
void CFE_PSP_TimerHandler(int num);
void CFE_PSP_DeleteOSResources(void);
void CFE_PSP_DisplayUsage(char *Name);
void CFE_PSP_ProcessArgumentDefaults(CFE_PSP_CommandData_t *CommandData);

/*
 **  External Declarations
 */
extern void CFE_TIME_Local1HzISR(void);
extern int32 CFE_PSP_SetupEEPROM(uint32 EEPROMSize, uint32 *EEPROMAddress);

/*
 ** Global variables
 */
CFE_PSP_GlobalData_t CFE_PSP_GlobalData;

/*
 ** getopts parameter passing options string
 */
static const char *optString = "R:S:C:I:N:h:M:";

/*
 ** getopts_long long form argument table
 */
static const struct option longOpts[] =
{
{ "reset", required_argument, NULL, 'R' },
{ "subtype", required_argument, NULL, 'S' },
{ "cpuid", required_argument, NULL, 'C' },
{ "scid", required_argument, NULL, 'I' },
{ "cpuname", required_argument, NULL, 'N' },
{ "runmode", required_argument, NULL, 'M' },
{ "help", no_argument, NULL, 'h' },
{ NULL, no_argument, NULL, 0 } };

/*
 ** Display the timer resolution for debug purposes.
 */
void CFE_PSP_DisplayTimerResolution(void)
{
    struct timespec timeRealTime;
    struct timespec timeMonotonic;
    struct timespec timeProcessCputimeID;
    struct timespec timeThreadCputimeID;

    clock_getres(CLOCK_REALTIME, &timeRealTime);
    clock_getres(CLOCK_MONOTONIC, &timeMonotonic);
    clock_getres(CLOCK_PROCESS_CPUTIME_ID, &timeProcessCputimeID);
    clock_getres(CLOCK_THREAD_CPUTIME_ID, &timeThreadCputimeID);

    printf("CFE_PSP: CLOCK_REALTIME resolution is %is %ins\n",
            (int) timeRealTime.tv_sec, (int) timeRealTime.tv_nsec);

    printf("CFE_PSP: CLOCK_MONOTONIC resolution is %is %ins\n",
            (int) timeMonotonic.tv_sec, (int) timeMonotonic.tv_nsec);

    printf("CFE_PSP: CLOCK_PROCESS_CPUTIME_ID resolution is %is %ins\n",
            (int) timeProcessCputimeID.tv_sec,
            (int) timeProcessCputimeID.tv_nsec);

    printf("CFE_PSP: CLOCK_THREAD_CPUTIME_ID resolution is %is %ins\n",
            (int) timeThreadCputimeID.tv_sec,
            (int) timeThreadCputimeID.tv_nsec);
}

/*
 ** Display a cool looking banner at startup.
 */
void CFE_PSP_DrawBanner(void)
{
    printf("\n");
    printf("    ___    ________  __    _____   ____________ \n");
    printf("   /   |  /  _/ __ \\/ /   /  _/ | / / ____/ __ \\\n");
    printf("  / /| |  / // /_/ / /    / //  |/ / __/ / /_/ /\n");
    printf(" / ___ |_/ // _, _/ /____/ // /|  / /___/ _, _/ \n");
    printf("/_/  |_/___/_/ |_/_____/___/_/ |_/_____/_/ |_|  \n");
    printf("\n");
    printf("\n");
    printf("By Windhover Labs\n");
    printf("\n");
    printf(" sNdyo/:.                                                                   .-/oydNs   \n");
    printf(".MMMMMMMMNmhs+-                           .                           -+shmNMMMMMMMM.  \n");
    printf("sMNNNNNNNmmddhhyyo-..                   y:d:y                   ..-oyyhhddmmNNNNNNNMy  \n");
    printf(":ossyyyyyyhhddmmNNMMN/yh/dNNmdhhysso+/ /so:os/ /+oosyhhdmNNd/hy/NMMNNmmddhhyyyyyysso:  \n");
    printf(" `/hNMMMMMMMMMMMMMMm/dMMNooNMMMMMMMMMm sMMMMMy mMMMMMMMMMNooNMMd/mMMMMMMMMMMMMMMNh/`   \n");
    printf("    .+dNMMMMMMMMMMd/mMMMMMd+yMMMMMMMMs::MMMMM::sMMMMMMMMy+dMMMMMm/dMMMMMMMMMMNd+.      \n");
    printf("       -omMMMMMMMy+NMMMMMMMMy+dMMMMMM/h-mMMMN-h/MMMMMMm+yMMMMMMMMN+yMMMMMMMmo-         \n");
    printf("         `:smMMMsoMMMMMMMMMMMmosNMMMM.NosMMMsoM.MMMMNsomMMMMMMMMMMMooMMMms:`           \n");
    printf("            `/y/oNMMMMMMMMMMMMMh+hMMd:Mm-MMM-mM:dMMh+hMMMMMMMMMMMMMNo/y/`              \n");
    printf("                .--:+osyhdmmNMMMNsomsoMM-mMm-MMosmosNMMMNmmdhyso+:--.                  \n");
    printf("                         ``..-:/+o+-.hMMsoMssMMd.-+s+/:-..``                           \n");
    printf("                                  `:o+dMm-M-mMd+o:`                                    \n");
    printf("                                .odMMNosN:y-NsoNMMmo-                                  \n");
    printf("                             `/hNMMMMMMh+/`/+hMMMMMMNh/`                               \n");
    printf("                            -oyyyyyyyyyys- -syyyyyyyyyyo-                              \n");
    printf("                            `+hNNNNNNNNNNN`NNNNNNNNNNNh+`                              \n");
    printf("                              `-smMMMMMMMM`MMMMMMMMms-`                                \n");
    printf("                                 `:yNMMMMM`MMMMMNy:`                                   \n");
    printf("                                    `/hMMM`MMMh/`                                      \n");
    printf("                                       .om`mo.                                         \n");
    printf("\n");
}

/******************************************************************************
 **  Function:  main()
 **
 **  Purpose:
 **    BSP Application entry point.
 **
 **  Arguments:
 **    (none)
 **
 **  Return:
 **    (none)
 */
void PSP_Main(int argc, char *argv[])
{
    uint32 reset_type;
    uint32 reset_subtype;
    struct sigaction sa;
    struct itimerval timer;
    int opt = 0;
    int longIndex = 0;
    sigset_t mask;
    pthread_t thread_id;
    struct sched_param thread_params;
    int ret;
    int32 os_timer;
    CFE_PSP_CommandData_t CommandData;
    cpu_set_t cpuMask;

    mlockall(MCL_CURRENT | MCL_FUTURE);

#ifdef CFE_PSP_EEPROM_SUPPORT
    int32 Status;
    uint32 eeprom_address;
    uint32 eeprom_size;
#endif

    /*
     ** Initialize the CommandData struct
     */
    memset(&(CommandData), 0, sizeof(CFE_PSP_CommandData_t));

    /*
     ** Process the arguments with getopt_long(), then
     ** start the cFE
     */
    opt = getopt_long(argc, argv, optString, longOpts, &longIndex);
    while (opt != -1)
    {
        switch (opt)
        {
        case 'R':
            strncpy(CommandData.ResetType, optarg, CFE_PSP_RESET_NAME_LENGTH);
            if ((strncmp(CommandData.ResetType, "PO", CFE_PSP_RESET_NAME_LENGTH)
                    != 0)
                    && (strncmp(CommandData.ResetType, "PR",
                            CFE_PSP_RESET_NAME_LENGTH) != 0))
            {
                printf("\nERROR: Invalid Reset Type: %s\n\n",
                        CommandData.ResetType);
                CommandData.GotResetType = 0;
                CFE_PSP_DisplayUsage(argv[0]);
                break;
            }
            printf("CFE_PSP: Reset Type: %s\n", (char *) optarg);
            CommandData.GotResetType = 1;
            break;

        case 'S':
            CommandData.SubType = strtol(optarg, NULL, 0);
            if (CommandData.SubType < 1 || CommandData.SubType > 5)
            {
                printf("\nERROR: Invalid Reset SubType: %s\n\n", optarg);
                CommandData.SubType = 0;
                CommandData.GotSubType = 0;
                CFE_PSP_DisplayUsage(argv[0]);
                break;
            }
            printf("CFE_PSP: Reset SubType: %d\n", (int) CommandData.SubType);
            CommandData.GotSubType = 1;
            break;

        case 'N':
            strncpy(CommandData.CpuName, optarg, CFE_PSP_CPU_NAME_LENGTH);
            printf("CFE_PSP: CPU Name: %s\n", CommandData.CpuName);
            CommandData.GotCpuName = 1;
            break;

        case 'C':
            CommandData.CpuId = strtol(optarg, NULL, 0);
            printf("CFE_PSP: CPU ID: %d\n", (int) CommandData.CpuId);
            CommandData.GotCpuId = 1;
            break;

        case 'I':
            CommandData.SpacecraftId = strtol(optarg, NULL, 0);
            printf("CFE_PSP: Spacecraft ID: %d\n",
                    (int) CommandData.SpacecraftId);
            CommandData.GotSpacecraftId = 1;
            break;

        case 'h':
            CFE_PSP_DisplayUsage(argv[0]);
            break;

        case 'M':
            strncpy(CommandData.RunMode, optarg, CFE_PSP_MODE_NAME_LENGTH);
            if ((strncmp(CommandData.RunMode, "NORMAL",
                    CFE_PSP_MODE_NAME_LENGTH) != 0)
                    && (strncmp(CommandData.RunMode, "TEST",
                            CFE_PSP_MODE_NAME_LENGTH) != 0))
            {
                printf("\nERROR: Invalid Run Mode: %s\n\n",
                        CommandData.ResetType);
                CommandData.GotRunMode = 0;
                CFE_PSP_DisplayUsage(argv[0]);
                break;
            }
            printf("CFE_PSP: Run Mode: %s\n", (char *) optarg);
            CommandData.GotRunMode = 1;
            break;

        default:
            CFE_PSP_DisplayUsage(argv[0]);
            break;
        }

        opt = getopt_long(argc, argv, optString, longOpts, &longIndex);
    } /* end while */

    /*
     ** Set the defaults for values that were not given for the
     ** optional arguments, and check for arguments that are required.
     */
    CFE_PSP_ProcessArgumentDefaults(&CommandData);

    /* Set CPU affinity to a single core */
    //CPU_ZERO(&cpuMask);
    //CPU_SET(1, &cpuMask);
    //sched_setaffinity(0, sizeof(cpuMask), &cpuMask);

    /*
     ** Set the reset type
     */
    if (strncmp("PR", CommandData.ResetType, 2) == 0)
    {
        reset_type = CFE_ES_PROCESSOR_RESET;
        printf("CFE_PSP: Starting the cFE with a PROCESSOR reset.\n");
    }
    else
    {
        reset_type = CFE_ES_POWERON_RESET;
        printf("CFE_PSP: Starting the cFE with a POWER ON reset.\n");
    }

    /*
     ** Assign the Spacecraft ID, CPU ID, and CPU Name
     */
    CFE_PSP_GlobalData.CFE_PSP_SpacecraftId = CommandData.SpacecraftId;
    CFE_PSP_GlobalData.CFE_PSP_CpuId = CommandData.CpuId;
    strncpy(CFE_PSP_GlobalData.CFE_PSP_CpuName, CommandData.CpuName,
            CFE_PSP_CPU_NAME_LENGTH);

    /*
     ** Set the reset subtype
     */
    reset_subtype = CommandData.SubType;

    /*
     ** Set the run mode
     **/
    if (strncmp(CommandData.RunMode, "NORMAL", CFE_PSP_MODE_NAME_LENGTH) == 0)
    {
        CFE_PSP_GlobalData.CFE_PSP_RunMode = OS_RUNTIME_MODE_REALTIME;
    }
    else
    {
        CFE_PSP_GlobalData.CFE_PSP_RunMode = OS_RUNTIME_MODE_NONREALTIME;
    }

    /*
     ** Install sigint_handler as the signal handler for SIGINT.
     */
    signal(SIGINT, CFE_PSP_SigintHandler);

    /*
     ** Init timer counter
     */
    CFE_PSP_GlobalData.TimerCounter = 0;

    /*
     ** Install timer_handler as the signal handler for SIGVTALRM.
     */
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = &CFE_PSP_TimerHandler;
    sigaction(SIGALRM, &sa, NULL);

    /*
     ** Configure the timer to expire after 250ms
     */
    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = 250000;

    /*
     **  and every 500ms after that.
     */
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 250000;

    /*
     ** Initialize the OS API data structures
     */
    if (OS_API_InitEx(CFE_PSP_GlobalData.CFE_PSP_RunMode) != OS_SUCCESS)
    {
        printf("PSP:  OSAL failed to initialize.  Terminating.\n");
        exit(-1);
    }

    sleep(1);

    /*
     ** Initialize the reserved memory
     */
    CFE_PSP_InitProcessorReservedMemory(reset_type);

    /*
     ** Start the timer
     */
    //setitimer (ITIMER_REAL, &timer, NULL);

    /*
     ** Disable Signals to parent thread and therefore all
     ** child threads create will block all signals
     ** Note: Timers will not work in the application unless
     **       threads are spawned in OS_Application_Startup.
     */
    sigfillset(&mask);
    sigdelset(&mask, SIGINT); /* Needed to kill program */
    sigprocmask(SIG_SETMASK, &mask, NULL);

    /*
     ** Raise the priority of the main thread so ES Main completes
     */
    thread_id = pthread_self();
    thread_params.sched_priority = sched_get_priority_max(SCHED_FIFO);
    ret = pthread_setschedparam(thread_id, SCHED_FIFO, &thread_params);
    if (ret != 0)
    {
        printf("Unable to set main thread priority to max\n");
    }

    CFE_PSP_DisplayTimerResolution();

    /*
     ** Call cFE entry point.
     */
    CFE_ES_Main(reset_type, reset_subtype, 1,
            (uint8 *) CFE_ES_NONVOL_STARTUP_FILE);

    CFE_PSP_DrawBanner();

    //uint32 clock_accuracy;
    //OS_TimerCreate(&os_timer, "CFE_PSP_TIMER", &clock_accuracy,
    //    CFE_PSP_TimerHandler);
    //OS_TimerSet(os_timer, 250000, 250000);

    /*
     ** Re-enable Signals to current thread so that
     ** any signals will interrupt in this threads context
     ** ... this is needed for timers
     */
    sigprocmask(SIG_UNBLOCK, &mask, NULL);

    /*
     ** Lower the thread priority to before entering the sleep loop
     */
    thread_params.sched_priority = sched_get_priority_min(SCHED_FIFO);
    ret = pthread_setschedparam(thread_id, SCHED_FIFO, &thread_params);
    if (ret != 0)
    {
        printf("Unable to set main thread priority to minimum\n");
    }
}

/******************************************************************************
 **  Function:  CFE_PSP_SigintHandler()
 **
 **  Purpose:
 **    SIGINT routine for linux/OSX
 **
 **  Arguments:
 **    (none)
 **
 **  Return:
 **    (none)
 */
void CFE_PSP_SigintHandler(int arg)
{
    printf("\nCFE_PSP: Control-C Captured - Exiting cFE\n");
    exit(0);
}

/******************************************************************************
 **  Function:  CFE_PSP_TimerHandler()
 **
 **  Purpose:
 **    1hz "isr" routine for linux/OSX
 **    This timer handler will execute 4 times a second.
 **
 **  Arguments:
 **    (none)
 **
 **  Return:
 **    (none)
 */
void CFE_PSP_TimerHandler(int num)
{
    /*
     ** call the CFE_TIME 1hz ISR
     */
    if ((CFE_PSP_GlobalData.TimerCounter % 4) == 0)
        CFE_TIME_Local1HzISR();

    /* update timer counter */
    CFE_PSP_GlobalData.TimerCounter++;
}

/******************************************************************************
 **  Function:  CFE_PSP_DeleteOSResources()
 **
 **  Purpose:
 **    Clean up any OS resources when exiting from the cFE
 **
 **  Arguments:
 **    (none)
 **
 **  Return:
 **    (none)
 */
void CFE_PSP_DeleteOSResources(void)
{
    uint32 i;
    int32 ReturnCode = 0;

    for (i = 0; i < OS_MAX_TASKS; i++)
    {
        ReturnCode = OS_TaskDelete(i);
    }

    printf("CFE_PSP: Deleted all Tasks in system\n");

    for (i = 0; i < OS_MAX_BIN_SEMAPHORES; i++)
    {
        ReturnCode = OS_BinSemDelete(i);
    }
    printf("CFE_PSP: Deleted all Binary Semaphores in system\n");

    for (i = 0; i < OS_MAX_COUNT_SEMAPHORES; i++)
    {
        ReturnCode = OS_CountSemDelete(i);
    }
    printf("CFE_PSP: Deleted all Counting Semaphores in system\n");

    for (i = 0; i < OS_MAX_MUTEXES; i++)
    {
        ReturnCode = OS_MutSemDelete(i);
    }
    printf("CFE_PSP: Deleted all Mutexes in system\n");

    for (i = 0; i < OS_MAX_QUEUES; i++)
    {
        ReturnCode = OS_QueueDelete(i);
    }
    printf("CFE_PSP: Deleted all Queues in system\n");

    for (i = 0; i < OS_MAX_TIMERS; i++)
    {
        ReturnCode = OS_TimerDelete(i);
    }
    printf("CFE_PSP: Deleted all Timers in system\n");

    printf("CFE_PSP: NOTE: After quitting the cFE with a Control-C signal, it MUST be started next time\n");
    printf("     with a Poweron Reset ( --reset PO ). \n");

}


/******************************************************************************
 **  Function:  CFE_PSP_DisplayUsage
 **
 **  Purpose:
 **    Display program usage, and exit.
 **
 **  Arguments:
 **    Name -- the name of the binary.
 **
 **  Return:
 **    (none)
 */
void CFE_PSP_DisplayUsage(char *Name)
{

    printf(
            "usage : %s [-R <value>] [-S <value>] [-C <value] [-N <value] [-I <value] [-M <value] [-h] \n",
            Name);
    printf("\n");
    printf(
            "        All parameters are optional and can be used in any order\n");
    printf("\n");
    printf("        Parameters include:\n");
    printf("        -R [ --reset ] Reset Type is one of:\n");
    printf("             PO   for Power On reset ( default )\n");
    printf("             PR   for Processor Reset\n");
    printf("        -S [ --subtype ] Reset Sub Type is one of\n");
    printf("             1   for  Power on ( default )\n");
    printf("             2   for  Push Button Reset\n");
    printf("             3   for  Hardware Special Command Reset\n");
    printf("             4   for  Watchdog Reset\n");
    printf("             5   for  Reset Command\n");
    printf("        -C [ --cpuid ]   CPU ID is an integer CPU identifier.\n");
    printf(
            "             The default  CPU ID is from the platform configuration file: %d\n",
            CFE_CPU_ID);
    printf(
            "        -N [ --cpuname ] CPU Name is a string to identify the CPU.\n");
    printf(
            "             The default  CPU Name is from the platform configuraitoon file: %s\n",
            CFE_CPU_NAME);
    printf(
            "        -I [ --scid ]    Spacecraft ID is an integer Spacecraft identifier.\n");
    printf(
            "             The default Spacecraft ID is from the mission configuration file: %d\n",
            CFE_SPACECRAFT_ID);
    printf("        -M [ --runmode ] Run Mode is one of:\n");
    printf("             NORMAL  Normal execution mode ( default )\n");
    printf(
            "             TEST    for running in frame lock step with sims or test engines\n");
    printf("        -h [ --help ]    This message.\n");
    printf("\n");
    printf("       Example invocation:\n");
    printf(" \n");
    printf("       Short form:\n");
    printf("       %s -R PO -S 1 -C 1 -N CPU1 -I 32 -M NORMAL\n", Name);
    printf("       Long form:\n");
    printf(
            "       %s --reset PO --subtype 1 --cpuid 1 --cpuname CPU1 --scid 32 --runmode NORMAL\n",
            Name);
    printf(" \n");
    CFE_PSP_DeleteOSResources();

}


/******************************************************************************
 **  Function: CFE_PSP_ProcessArgumentDefaults
 **
 **  Purpose:
 **    This function assigns defaults to parameters and checks to make sure
 **    the user entered required parameters
 **
 **  Arguments:
 **    CFE_PSP_CommandData_t *CommandData -- A pointer to the command
 **    parameters.
 **
 **  Return:
 **    (none)
 */
void CFE_PSP_ProcessArgumentDefaults(CFE_PSP_CommandData_t *CommandData)
{
    if (CommandData->GotResetType == 0)
    {
        strncpy(CommandData->ResetType, "PO", 2);
        printf("CFE_PSP: Default Reset Type = PO\n");
        CommandData->GotResetType = 1;
    }

    if (CommandData->GotSubType == 0)
    {
        CommandData->SubType = 1;
        printf("CFE_PSP: Default Reset SubType = 1\n");
        CommandData->GotSubType = 1;
    }

    if (CommandData->GotCpuId == 0)
    {
        CommandData->CpuId = CFE_CPU_ID;
        printf("CFE_PSP: Default CPU ID = %d\n", CFE_CPU_ID);
        CommandData->GotCpuId = 1;
    }

    if (CommandData->GotSpacecraftId == 0)
    {
        CommandData->SpacecraftId = CFE_SPACECRAFT_ID;
        printf("CFE_PSP: Default Spacecraft ID = %d\n", CFE_SPACECRAFT_ID);
        CommandData->GotSpacecraftId = 1;
    }

    if (CommandData->GotCpuName == 0)
    {
        strncpy(CommandData->CpuName, CFE_CPU_NAME, CFE_PSP_CPU_NAME_LENGTH);
        printf("CFE_PSP: Default CPU Name: %s\n", CFE_CPU_NAME);
        CommandData->GotCpuName = 1;
    }

    if (CommandData->GotRunMode == 0)
    {
        strncpy(CommandData->RunMode, "NORMAL", 6);
        printf("CFE_PSP: Default Reset Type = NORMAL\n");
        CommandData->GotRunMode = 1;
    }
}

