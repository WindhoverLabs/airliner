Configuration
=============

TODO

Mission Specific
^^^^^^^^^^^^^^^^

+----------------------------+-------------------------------------+
| File                       | Parameter                           |
+============================+=====================================+
| :ci:`ci_mission_cfg.h`     | :ci:`CI_MAX_RGST_CMDS`              |
|                            +-------------------------------------+
|                            | :ci:`CI_CHECKSUM_REQUIRED`          |
|                            +-------------------------------------+
|                            | :ci:`CI_CMD_MAX_TIMEOUT`            |
+----------------------------+-------------------------------------+
| :ci:`ci_perfids.h`         | :ci:`CI_MAIN_TASK_PERF_ID`          |
|                            +-------------------------------------+
|                            | :ci:`CI_SOCKET_RCV_PERF_ID`         |
+----------------------------+-------------------------------------+

.. note::
   The performance IDs defined in the :ci:`ci_perfids.h` file must have values
   that are unique system wide.  Most applications also have performance IDs.
   No two performance IDs may have the same value.
   

Platform Specific
^^^^^^^^^^^^^^^^^

+-----------------------------+---------------------------------------------+
| File                        | Parameter                                   |
+=============================+=============================================+
| :ci:`ci_msgids.h`           | :ci:`CI_CMD_MID`                            |
|                             +---------------------------------------------+
|                             | :ci:`CI_SEND_HK_MID`                        |
|                             +---------------------------------------------+
|                             | :ci:`CI_WAKEUP_MID`                         |
|                             +---------------------------------------------+
|                             | :ci:`CI_PROCESS_TIMEOUTS_MID`               |
+-----------------------------+---------------------------------------------+
| :ci:`ci_platform_cfg.h`     | :ci:`CI_MISSION_REV`                        |
|                             +---------------------------------------------+
|                             | :ci:`CI_SCH_PIPE_DEPTH`                     |
|                             +---------------------------------------------+
|                             | :ci:`CI_SCH_PIPE_NAME`                      |
|                             +---------------------------------------------+
|                             | :ci:`CI_SCH_PIPE_PEND_TIME`                 |
|                             +---------------------------------------------+
|                             | :ci:`CI_SCH_PIPE_PEND_TIME`                 |
|                             +---------------------------------------------+
|                             | :ci:`CI_SCH_PIPE_WAKEUP_RESERVED`           |
|                             +---------------------------------------------+
|                             | :ci:`CI_SCH_PIPE_SEND_HK_RESERVED`          |
|                             +---------------------------------------------+
|                             | :ci:`CI_SCH_PIPE_PROC_TIMEOUT_RESERVED`     |
|                             +---------------------------------------------+
|                             | :ci:`CI_CMD_PIPE_DEPTH`                     |
|                             +---------------------------------------------+
|                             | :ci:`CI_CMD_PIPE_NAME`                      |
|                             +---------------------------------------------+
|                             | :ci:`CI_CONFIG_TABLE_FILENAME`              |
|                             +---------------------------------------------+
|                             | :ci:`CI_INGEST_MODE`                        |
|                             +---------------------------------------------+
|                             | :ci:`CI_MAX_CMD_INGEST`                     |
|                             +---------------------------------------------+
|                             | :ci:`CI_CFG_TBL_MUTEX_NAME`                 |
|                             +---------------------------------------------+
|                             | :ci:`CI_TIME_TBL_MUTEX_NAME`                |
+-----------------------------+---------------------------------------------+

.. note::
   The message IDs defined in the :ci:`ci_msgids.h` file must have values
   that are unique system wide.  Most applications also have message IDs.
   No two message IDs may have the same value.


Table(s)
^^^^^^^^^^^^^^^^
TODO: Review Tables 

+-------------------------------+------------------------------------+---------------------------------------------------+
| Table Name                    | Default file name                  | Parameter                                         |
+===============================+====================================+===================================================+
| :ci:`CI_CONFIG_TABLENAME`     | :ci:`CI_CONFIG_TABLE_FILENAME`     | :ci:`CI_ConfigTblEntry_t::TableID`                |
|                               |                                    +---------------------------------------------------+
|                               |                                    | :ci:`CI_ConfigTblEntry_t::cmds`                   |
+-------------------------------+------------------------------------+---------------------------------------------------+
| :ci:`CI_TIMEOUT_TABLENAME`    | :ci:`CI_TIMEOUT_TABLE_FILENAME`    | :ci:`CI_TimeoutTblEntry_t::TableID`               |
|                               |                                    +---------------------------------------------------+
|                               |                                    | :ci:`CI_TimeoutTblEntry_t::time`                  |
+-------------------------------+------------------------------------+---------------------------------------------------+
| :ci:`CI_CDS_TABLENAME`        | :ci:`CI_CDS_TABLE_FILENAME`        | :ci:`CI_CdsTbl_t::iParam`                         |
+-------------------------------+------------------------------------+---------------------------------------------------+


System Integration
^^^^^^^^^^^^^^^^^^

TODO: Describe System Integration instructions specific to CI.


Run Time Configuration
^^^^^^^^^^^^^^^^^^^^^^

TODO: Describe Run Time Configuration for CI.
