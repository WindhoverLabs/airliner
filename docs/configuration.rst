Configuration
=============

Mission Specific
^^^^^^^^^^^^^^^^

TODO: Add Doxygen documentation for qae_mission_cfg.h

+-------------------------+--------------------------------+
| File                    | Parameter                      |
+=========================+================================+
| :qae:`qae_mission_cfg.h`| :qae:`AE_SB_TIMEOUT`           |
+-------------------------+--------------------------------+
| :qae:`qae_perfids.h`    | :qae:`QAE_MAIN_TASK_PERF_ID`   |
+                         +--------------------------------+
|                         | :qae:`AE_MAIN_TASK_PERF_ID`    |
+-------------------------+--------------------------------+

.. note::
   The performance IDs defined in the :qae:`qae_perfids.h` file must have values
   that are unique system wide.  Most applications also have performance IDs.
   No two performance IDs may have the same value.
   

Platform Specific
^^^^^^^^^^^^^^^^^

+---------------------------+-------------------------------------+
| File                      | Parameter                           |
+===========================+=====================================+
| :qae:`qae_msgids.h`       | :qae:`QAE_HK_TLM_MID`               |
+                           +-------------------------------------+
|                           | :qae:`QAE_SEND_HK_MID`              |
+                           +-------------------------------------+
|                           | :qae:`QAE_WAKEUP_MID`               |
+                           +-------------------------------------+
|                           | :qae:`QAE_CMD_MID`                  |
+---------------------------+-------------------------------------+
| :qae:`qae_platform_cfg.h` | :qae:`QAE_MISSION_REV`              |
+                           +-------------------------------------+
|                           | :qae:`QAE_SCH_PIPE_DEPTH`           |
+                           +-------------------------------------+
|                           | :qae:`QAE_SCH_PIPE_NAME`            |
+                           +-------------------------------------+
|                           | :qae:`QAE_SCH_PIPE_PEND_TIME`       |
+                           +-------------------------------------+
|                           | :qae:`QAE_WAKEUP_MID_MAX_MSG_COUNT` |
+                           +-------------------------------------+
|                           | :qae:`QAE_SEND_HK_MID_MAX_MSG_COUNT`|
+                           +-------------------------------------+
|                           | :qae:`QAE_CMD_PIPE_DEPTH`           |
+                           +-------------------------------------+
|                           | :qae:`QAE_CMD_PIPE_NAME`            |
+                           +-------------------------------------+
|                           | :qae:`QAE_DATA_PIPE_DEPTH`          |
+                           +-------------------------------------+
|                           | :qae:`QAE_DATA_PIPE_NAME`           |
+                           +-------------------------------------+
|                           | :qae:`QAE_CONFIG_TABLE_FILENAME`    |
+                           +-------------------------------------+
|                           | :qae:`QAE_STARTUP_TIMEOUT_MSEC`     |
+---------------------------+-------------------------------------+

.. note::
   The message IDs defined in the :qae:`qae_msgids.h` file must have values
   that are unique system wide.  Most applications also have message IDs.
   No two message IDs may have the same value.


Table(s)
^^^^^^^^

TODO: Add Table explanation

TODO: Add Doxygen documentation for qae_config.tbl

+-------------------------------+------------------------------------+------------------------------------------+
| Table Name                    | Default file name                  | Parameter                                |
+===============================+====================================+==========================================+
| :qae:`QAE_CONFIG_TABLENAME`   | :qae:`QAE_CONFIG_TABLE_FILENAME`   | :qae:`QAE_ConfigTbl_t::ATT_W_ACC`        |
+                               |                                    +------------------------------------------+
|                               |                                    | :qae:`QAE_ConfigTbl_t::ATT_W_MAG`        |
+                               |                                    +------------------------------------------+
|                               |                                    | :qae:`QAE_ConfigTbl_t::ATT_W_GYRO_BIAS`  |
+                               |                                    +------------------------------------------+
|                               |                                    | :qae:`QAE_ConfigTbl_t::ATT_MAG_DECL`     |
+                               |                                    +------------------------------------------+
|                               |                                    | :qae:`QAE_ConfigTbl_t::ATT_MAG_DECL_A`   |
+                               |                                    +------------------------------------------+
|                               |                                    | :qae:`QAE_ConfigTbl_t::ATT_ACC_COMP`     |
+                               |                                    +------------------------------------------+
|                               |                                    | :qae:`QAE_ConfigTbl_t::ATT_BIAS_MAX`     |
+-------------------------------+------------------------------------+------------------------------------------+

TODO: Add Doxygen documentation for qae_config.c

Below is an example table.

.. literalinclude:: ../fsw/tables/qae_config.c
  :language: C
  :caption: Nominal Configuration Table


System Integration
^^^^^^^^^^^^^^^^^^

TODO: Describe System Integration instructions specific to QAE.


Run Time Configuration
^^^^^^^^^^^^^^^^^^^^^^

TODO: Describe Run Time Configuration for QAE.
