Configuration
=============

TODO

Mission Specific
^^^^^^^^^^^^^^^^

TODO: Add Doxygen documentation for ea_msgids.h

+----------------------------+-------------------------------------+
| File                       | Parameter                           |
+============================+=====================================+
| :ea:`ea_mission_cfg.h`     | :ea:`EA_SB_TIMEOUT`                 |
+----------------------------+-------------------------------------+
| :ea:`ea_msgids.h`          | :ea:`EA_CMD_MID`                    |
+                            +-------------------------------------+
|                            | :ea:`EA_SEND_HK_MID`                |
+                            +-------------------------------------+
|                            | :ea:`EA_WAKEUP_MID`                 |
+                            +-------------------------------------+
|                            | :ea:`EA_PERFMON_MID`                |
+                            +-------------------------------------+
|                            | :ea:`EA_HK_TLM_MID`                 |
+                            +-------------------------------------+
|                            | :ea:`EA_OUT_DATA_MID`               |
+----------------------------+-------------------------------------+
| :ea:`ea_perfids.h`         | :ea:`EA_MAIN_TASK_PERF_ID`          |
+----------------------------+-------------------------------------+

.. note::
   The performance IDs defined in the :ea:`ea_perfids.h` file must have values
   that are unique system wide.  Most applications also have performance IDs.
   No two performance IDs may have the same value.
   

Platform Specific
^^^^^^^^^^^^^^^^^

TODO: Add Doxygen documentation for ea_msgids.h

+-----------------------------+---------------------------------------------+
| File                        | Parameter                                   |
+=============================+=============================================+
| :ea:`ea_msgids.h`           |                                             |
+-----------------------------+---------------------------------------------+
| :ea:`ea_platform_cfg.h`     | :ea:`EA_CHILD_TASK_FLAGS`                   |
+                             +---------------------------------------------+
|                             | :ea:`EA_MISSION_REV`                        |
+                             +---------------------------------------------+
|                             | :ea:`EA_SCH_PIPE_DEPTH`                     |
+                             +---------------------------------------------+
|                             | :ea:`EA_SCH_PIPE_NAME`                      |
+                             +---------------------------------------------+
|                             | :ea:`EA_SCH_PIPE_PEND_TIME`                 |
+                             +---------------------------------------------+
|                             | :ea:`EA_SCH_PIPE_WAKEUP_RESERVED`           |
+                             +---------------------------------------------+
|                             | :ea:`EA_SCH_PIPE_SEND_HK_RESERVED`          |
+                             +---------------------------------------------+
|                             | :ea:`EA_CMD_PIPE_DEPTH`                     |
+                             +---------------------------------------------+
|                             | :ea:`EA_CMD_PIPE_NAME`                      |
+                             +---------------------------------------------+
|                             | :ea:`EA_DATA_PIPE_DEPTH`                    |
+                             +---------------------------------------------+
|                             | :ea:`EA_DATA_PIPE_NAME`                     |
+                             +---------------------------------------------+
|                             | :ea:`EA_CONFIG_TABLE_FILENAME`              |
+                             +---------------------------------------------+
|                             | :ea:`EA_STARTUP_TIMEOUT_MSEC`               |
+                             +---------------------------------------------+
|                             | :ea:`EA_CUSTOM_DEV_PATH`                    |
+                             +---------------------------------------------+
|                             | :ea:`EA_CHILD_TASK_PRIORITY`                |
+                             +---------------------------------------------+
|                             | :ea:`EA_APP_UTIL_THRESHOLD`                 |
+                             +---------------------------------------------+
|                             | :ea:`EA_APP_NUM_THREADS`                    |
+                             +---------------------------------------------+
|                             | :ea:`EA_MAX_PATH_LEN`                       |
+-----------------------------+---------------------------------------------+

.. note::
   The message IDs defined in the :ea:`ea_msgids.h` file must have values
   that are unique system wide.  Most applications also have message IDs.
   No two message IDs may have the same value.


Table(s)
^^^^^^^^^^^^^^^^

TODO Review table. Missing ea_tbldefs.h from Doxygen.

+-------------------------------+------------------------------------+--------------------------------------------+
| Table Name                    | Default file name                  | Parameter                                  |
+===============================+====================================+============================================+
| :ea:`EA_CONFIG_TABLE_NAME`    | :ea:`EA_CONFIG_TABLE_FILENAME`     | :ea:`EA_ConfigTblEntry_t::`                |
+-------------------------------+------------------------------------+--------------------------------------------+

.. literalinclude:: ../fsw/tables/ea_config.c
  :language: C
  :caption: Nominal Configuration Table

System Integration
^^^^^^^^^^^^^^^^^^

TODO: Describe System Integration instructions specific to EA.


Run Time Configuration
^^^^^^^^^^^^^^^^^^^^^^

TODO: Describe Run Time Configuration for EA.

