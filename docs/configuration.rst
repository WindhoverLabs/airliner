Configuration
=============

Mission Specific
^^^^^^^^^^^^^^^^

TODO: Add Doxygen documentation for sonar_mission_cfg.h

+-------------------------------+----------------------------------+
| File                          | Parameter                        |
+===============================+==================================+
| :sonar:`sonar_mission_cfg.h`  | :sonar:`ULR_SB_TIMEOUT`          |
+-------------------------------+----------------------------------+
| :sonar:`sonar_perfids.h`      | :sonar:`SONAR_MAIN_TASK_PERF_ID` |
+-------------------------------+----------------------------------+

.. note::
   The performance IDs defined in the :sonar:`sonar_perfids.h` file must have values
   that are unique system wide.  Most applications also have performance IDs.
   No two performance IDs may have the same value.
   

Platform Specific
^^^^^^^^^^^^^^^^^

TODO: Add Doxygen documentation for sonar_platform_cfg.h

+-------------------------------+------------------------------------------+
| File                          | Parameter                                |
+===============================+==========================================+
| :sonar:`sonar_msgids.h`       | :sonar:`SONAR_CMD_MID`                   |
|                               +------------------------------------------+
|                               | :sonar:`SONAR_SEND_HK_MID`               |
+                               +------------------------------------------+
|                               | :sonar:`SONAR_MEASURE_MID`               |
+                               +------------------------------------------+
|                               | :sonar:`SONAR_OUT_DATA_MID`              |
+                               +------------------------------------------+
|                               | :sonar:`SONAR_HK_TLM_MID`                |
+-------------------------------+------------------------------------------+
| :sonar:`sonar_platform_cfg.h` | :sonar:`SONAR_MISSION_REV`               |
+                               +------------------------------------------+
|                               | :sonar:`SONAR_SCH_PIPE_DEPTH`            |
+                               +------------------------------------------+
|                               | :sonar:`SONAR_SCH_PIPE_NAME`             |
+                               +------------------------------------------+
|                               | :sonar:`SONAR_SCH_PIPE_PEND_TIME`        |
+                               +------------------------------------------+
|                               | :sonar:`SONAR_MEASURE_MID_MAX_MSG_COUNT` |
+                               +------------------------------------------+
|                               | :sonar:`SONAR_SEND_HK_MID_MAX_MSG_COUNT` |
+                               +------------------------------------------+
|                               | :sonar:`SONAR_CMD_PIPE_DEPTH`            |
+                               +------------------------------------------+
|                               | :sonar:`SONAR_CMD_PIPE_NAME`             |
+                               +------------------------------------------+
|                               | :sonar:`SONAR_DATA_PIPE_DEPTH`           |
+                               +------------------------------------------+
|                               | :sonar:`SONAR_DATA_PIPE_NAME`            |
+                               +------------------------------------------+
|                               | :sonar:`SONAR_CONFIG_TABLE_FILENAME`     |
+                               +------------------------------------------+
|                               | :sonar:`SONAR_STARTUP_TIMEOUT_MSEC`      |
+                               +------------------------------------------+
|                               | :sonar:`SONAR_SENS_VARIANCE`             |
+                               +------------------------------------------+
|                               | :sonar:`SONAR_MIN_DISTANCE`              |
+                               +------------------------------------------+
|                               | :sonar:`SONAR_MAX_DISTANCE`              |
+-------------------------------+------------------------------------------+

.. note::
   The message IDs defined in the :sonar:`sonar_msgids.h` file must have values
   that are unique system wide.  Most applications also have message IDs.
   No two message IDs may have the same value.


Table(s)
^^^^^^^^

TODO: Add Table explanation

TODO: Add Doxygen documentation for sonar_config.tbl

+--------------------------------+--------------------------------------+--------------------------------------------+
| Table Name                     | Default file name                    | Parameter                                  |
+================================+======================================+============================================+
| :sonar:`SONAR_CONFIG_TABLENAME`| :sonar:`SONAR_CONFIG_TABLE_FILENAME` | :sonar:`SONAR_ConfigTbl_t::temp`           |
+--------------------------------+--------------------------------------+--------------------------------------------+

TODO: Add Doxygen documentation for sonar_config.c

Below is an example table.

.. literalinclude:: ../fsw/tables/sonar_config.c
  :language: C
  :caption: Nominal Configuration Table


System Integration
^^^^^^^^^^^^^^^^^^

TODO: Describe System Integration instructions specific to SONAR.


Run Time Configuration
^^^^^^^^^^^^^^^^^^^^^^

TODO: Describe Run Time Configuration for SONAR.
