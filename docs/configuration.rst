Configuration
=============

Mission Specific
^^^^^^^^^^^^^^^^

TODO: Add Doxygen documentation for pe_mission_cfg.h and sim_perfids.h

+---------------------------+--------------------------------+
| File                      | Parameter                      |
+===========================+================================+
| :sim:`sim_mission_cfg.h`  | :sim:`SIM_SB_TIMEOUT`          |
+---------------------------+--------------------------------+
| :sim:`sim_perfids.h`      | :sim:`SIM_MAIN_TASK_PERF_ID`   |
+---------------------------+--------------------------------+

.. note::
   The performance IDs defined in the :sim:`sim_perfids.h` file must have values
   that are unique system wide.  Most applications also have performance IDs.
   No two performance IDs may have the same value.
   

Platform Specific
^^^^^^^^^^^^^^^^^

TODO: Add Doxygen documentation for sim_msgids.h and sim_platform_cfg.h

+---------------------------+-------------------------------------+
| File                      | Parameter                           |
+===========================+=====================================+
| :sim:`sim_msgids.h`       | :sim:`SIM_HK_TLM_MID`               |
|                           +-------------------------------------+
|                           | :sim:`SIM_SEND_HK_MID`              |
+                           +-------------------------------------+
|                           | :sim:`SIM_WAKEUP_MID`               |
+                           +-------------------------------------+
|                           | :sim:`SIM_CMD_MID`                  |
+---------------------------+-------------------------------------+
| :sim:`sim_platform_cfg.h` | :sim:`SIM_LISTENER_TASK_FLAGS`      |
+                           +-------------------------------------+
|                           | :sim:`SIM_MISSION_REV`              |
+                           +-------------------------------------+
|                           | :sim:`SIM_SCH_PIPE_DEPTH`           |
+                           +-------------------------------------+
|                           | :sim:`SIM_SCH_PIPE_NAME`            |
+                           +-------------------------------------+
|                           | :sim:`SIM_SCH_PIPE_PEND_TIME`       |
+                           +-------------------------------------+
|                           | :sim:`SIM_WAKEUP_MID_MAX_MSG_COUNT` |
+                           +-------------------------------------+
|                           | :sim:`SIM_SEND_HK_MID_MAX_MSG_COUNT`|
+                           +-------------------------------------+
|                           | :sim:`SIM_CMD_PIPE_DEPTH`           |
+                           +-------------------------------------+
|                           | :sim:`SIM_CMD_PIPE_NAME`            |
+                           +-------------------------------------+
|                           | :sim:`SIM_DATA_PIPE_DEPTH`          |
+                           +-------------------------------------+
|                           | :sim:`SIM_DATA_PIPE_NAME`           |
+                           +-------------------------------------+
|                           | :sim:`SIM_CONFIG_TABLE_FILENAME`    |
+                           +-------------------------------------+
|                           | :sim:`SIM_STARTUP_TIMEOUT_MSEC`     |
+                           +-------------------------------------+
|                           | :sim:`SIM_MAX_MESSAGE_SIZE`         |
+                           +-------------------------------------+
|                           | :sim:`SIM_LISTENER_TASK_NAME`       |
+                           +-------------------------------------+
|                           | :sim:`SIM_LISTENER_TASK_STACK_SIZE` |
+                           +-------------------------------------+
|                           | :sim:`SIM_LISTENER_TASK_PRIORITY`   |
+                           +-------------------------------------+
|                           | :sim:`SIM_MUTEX_NAME`               |
+                           +-------------------------------------+
|                           | :sim:`SIMLIB_MUTEX_SEM_NAME`        |
+                           +-------------------------------------+
|                           | :sim:`SIM_PORT`                     |
+---------------------------+-------------------------------------+

.. note::
   The message IDs defined in the :sim:`sim_msgids.h` file must have values
   that are unique system wide.  Most applications also have message IDs.
   No two message IDs may have the same value.


Table(s)
^^^^^^^^

TODO: Add Table explanation

TODO: Add Doxygen documentation for pe_config.tbl

+-------------------------------+------------------------------------+------------------------------------------+
| Table Name                    | Default file name                  | Parameter                                |
+===============================+====================================+==========================================+
| :sim:`SIM_CONFIG_TABLENAME`   | :sim:`SIM_CONFIG_TABLE_FILENAME`   | :sim:`SIM_ConfigTbl_t::temp`             |
+-------------------------------+------------------------------------+------------------------------------------+

TODO: Add Doxygen documentation for sim_config.c

Below is an example table.

.. literalinclude:: ../fsw/tables/sim_config.c
  :language: C
  :caption: Application Configuration Table


System Integration
^^^^^^^^^^^^^^^^^^

TODO: Describe System Integration instructions specific to SIM.


Run Time Configuration
^^^^^^^^^^^^^^^^^^^^^^

TODO: Describe Run Time Configuration for SIM.
