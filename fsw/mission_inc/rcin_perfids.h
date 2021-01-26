#ifndef RCIN_PERFIDS_H
#define RCIN_PERFIDS_H

#ifdef __cplusplus
extern "C" {
#endif

/**< \brief RCIN Main Task performance ID.  This measures the execution time of the main loop. */
#define RCIN_MAIN_TASK_PERF_ID      80

/**< \brief RCIN Device Get performance ID.  This measures the execution time of the select call to retrieve data from the device. */
#define RCIN_DEVICE_GET_PERF_ID     84

#ifdef __cplusplus
}
#endif

#endif /* RCIN_PERFIDS_H */

/************************/
/*  End of File Comment */
/************************/
