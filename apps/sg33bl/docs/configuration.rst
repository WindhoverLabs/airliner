Configuration
=============

Mission Specific
^^^^^^^^^^^^^^^^

TODO: Create Doxygen documentation for sg33bl_mission_cfg.h and sg33bl_perfids.h

+---------------------------------+-----------------------------------+
| File                            | Parameter                         |
+=================================+===================================+
| :sg33bl:`sg33bl_mission_cfg.h`  | :sg33bl:`SG33BL_SB_TIMEOUT`       |
+---------------------------------+-----------------------------------+
| :sg33bl:`sg33bl_perfids.h`      | :sg33bl:`SG33BL_MAIN_TASK_PERF_ID`|
+---------------------------------+-----------------------------------+

.. note::
   The performance IDs defined in the :sg33bl:`sg33bl_perfids.h` file must have values
   that are unique system wide.  Most applications also have performance IDs.
   No two performance IDs may have the same value.
   

Platform Specific
^^^^^^^^^^^^^^^^^

TODO: Create Doxygen documentation for sg33bl_msgids.h and sg33bl_platform_cfg.h

+---------------------------------+-------------------------------------------+
| File                            | Parameter                                 |
+=================================+===========================================+
| :sg33bl:`sg33bl_msgids.h`       | :sg33bl:`SG33BL_HK_TLM_MID`               |
|                                 +-------------------------------------------+
|                                 | :sg33bl:`SG33BL_SEND_HK_MID`              |
+                                 +-------------------------------------------+
|                                 | :sg33bl:`SG33BL_WAKEUP_MID`               |
+                                 +-------------------------------------------+
|                                 | :sg33bl:`SG33BL_CMD_MID`                  |
+---------------------------------+-------------------------------------------+
| :sg33bl:`sg33bl_platform_cfg.h` | :sg33bl:`SG33BL_MISSION_REV`              |
+                                 +-------------------------------------------+
|                                 | :sg33bl:`SG33BL_SCH_PIPE_DEPTH`           |
+                                 +-------------------------------------------+
|                                 | :sg33bl:`SG33BL_SCH_PIPE_NAME`            |
+                                 +-------------------------------------------+
|                                 | :sg33bl:`SG33BL_SCH_PIPE_PEND_TIME`       |
+                                 +-------------------------------------------+
|                                 | :sg33bl:`SG33BL_WAKEUP_MID_MAX_MSG_COUNT` |
+                                 +-------------------------------------------+
|                                 | :sg33bl:`SG33BL_SEND_HK_MID_MAX_MSG_COUNT`|
+                                 +-------------------------------------------+
|                                 | :sg33bl:`SG33BL_CMD_PIPE_DEPTH`           |
+                                 +-------------------------------------------+
|                                 | :sg33bl:`SG33BL_CMD_PIPE_NAME`            |
+                                 +-------------------------------------------+
|                                 | :sg33bl:`SG33BL_DATA_PIPE_DEPTH`          |
+                                 +-------------------------------------------+
|                                 | :sg33bl:`SG33BL_DATA_PIPE_NAME`           |
+                                 +-------------------------------------------+
|                                 | :sg33bl:`SG33BL_CONFIG_TABLE_FILENAME`    |
+                                 +-------------------------------------------+
|                                 | :sg33bl:`SG33BL_STARTUP_TIMEOUT_MSEC`     |
+                                 +-------------------------------------------+
|                                 | :sg33bl:`SG33BL_SERIAL_DEVICE_PATH`       |
+                                 +-------------------------------------------+
|                                 | :sg33bl:`SG33BL_SERIAL_IO_SPEED`          |
+---------------------------------+-------------------------------------------+

.. note::
   The message IDs defined in the :sg33bl:`sg33bl_msgids.h` file must have values
   that are unique system wide.  Most applications also have message IDs.
   No two message IDs may have the same value.
   
TODO: Describe the "custom" layer code.


Table(s)
^^^^^^^^

TODO: Add Table explanation

TODO: Add Doxygen documentation for sg33bl_config.tbl

+----------------------------------+----------------------------------------+------------------------------------------+
| Table Name                       | Default file name                      | Parameter                                |
+==================================+========================================+==========================================+
| :sg33bl:`SG33BL_CONFIG_TABLENAME`| :sg33bl:`SG33BL_CONFIG_TABLE_FILENAME` | :sg33bl:`SG33BL_ConfigTbl_t::temp`       |
+----------------------------------+----------------------------------------+------------------------------------------+

TODO: Add Doxygen documentation for sg33bl_config.c

Below is an example table.

.. literalinclude:: ../fsw/tables/sg33bl_config.c
  :language: C
  :caption: Nominal Configuration Table


System Integration
^^^^^^^^^^^^^^^^^^

TODO: Describe System Integration instructions specific to SG33BL.


Run Time Configuration
^^^^^^^^^^^^^^^^^^^^^^

TODO: Describe Run Time Configuration for SG33BL.
