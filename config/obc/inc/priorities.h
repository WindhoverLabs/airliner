
/* 250Hz */
/* AMC                                   (  40 ) */        /* 250Hz */
/* LD                                    (  55 ) */        /* 250Hz */
/* MAC                                   (  30 ) */        /* 250Hz */
/* MPC                                   (  35 ) */        /* 250Hz */
/* NAV                                   (  50 ) */        /* 250Hz */
/* QAE                                   (  25 ) */        /* 250Hz */
/* SBN                                   (  60 ) */        /* 250Hz */
/* SCH                                   (  10 ) */        /* 250Hz */
/* SED                                   (  15 ) */        /* 250Hz */
/* SENS                                  (  20 ) */        /* 250Hz */
/* VM                                    (  45 ) */        /* 250Hz */

/* 125Hz */
/* PE                                    (  70 ) */        /* 125Hz */

/*  10Hz */
/* CI                                    (  80 ) */        /*  10Hz */
/* TO                                    (  90 ) */        /*  10Hz */

/*   1Hz */
#define CFE_ES_PERF_CHILD_PRIORITY       ( 150 )           /*   1Hz */
#define CFE_EVS_START_TASK_PRIORITY      ( 155 )           /*   1Hz */
#define CFE_SB_START_TASK_PRIORITY       ( 150 )           /*   1Hz */
#define CFE_ES_START_TASK_PRIORITY       ( 140 )           /*   1Hz */
#define CFE_TIME_START_TASK_PRIORITY     ( 145 )           /*   1Hz */
#define CFE_TIME_TONE_TASK_PRIORITY      (   5 )           /*   1Hz */
#define CFE_TIME_1HZ_TASK_PRIORITY       ( 100 )           /*   1Hz */
#define CFE_TBL_START_TASK_PRIORITY      ( 150 )           /*   1Hz */
/* CF                                    ( 110 ) */        /*   1Hz */
/* FM                                    ( 115 ) */        /*   1Hz */
/* HS                                    ( 105 ) */        /*   1Hz */
/* MD                                    ( 130 ) */        /*   1Hz */
/* MM                                    ( 125 ) */        /*   1Hz */
/* SC                                    ( 120 ) */        /*   1Hz */

/* ASYNC */
#define CI_LISTENER_TASK_PRIORITY        (  79 )           /* ASYNC */
#define CS_CHILD_TASK_PRIORITY           ( 210 )           /* ASYNC */
#define FM_CHILD_TASK_PRIORITY           ( 200 )           /* ASYNC */
#define HS_IDLE_TASK_PRIORITY            ( 254 )           /* ASYNC */
#define RGBLED_SELFTEST_TASK_PRIORITY    ( 220 )           /* ASYNC */
#define TO_CHANNEL_TASK_PRIORITY         (  89 )           /* ASYNC */
#define SCH_AD_CHILD_TASK_PRIORITY		 (  11 )


/* PPD Only                                                         */
#define EA_CHILD_TASK_PRIORITY           ( 187 )
