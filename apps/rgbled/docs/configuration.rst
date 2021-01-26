Configuration
=============

Mission Specific
^^^^^^^^^^^^^^^^

TODO: Add Doxygen documentation for rgbled_mission_cfg.h

+-------------------------------+------------------------------------+
| File                          | Parameter                          |
+===============================+====================================+
| :rgbled:`rgbled_mission_cfg.h`| :rgbled:`RGBLED_SB_TIMEOUT`        |
+-------------------------------+------------------------------------+
| :rgbled:`rgbled_perfids.h`    | :rgbled:`RGBLED_MAIN_TASK_PERF_ID` |
+                               +------------------------------------+
|                               | :rgbled:`RGBLED_SEND_PERF_ID`      |
+                               +------------------------------------+
|                               | :rgbled:`RGBLED_RECEIVE_PERF_ID`   |
+-------------------------------+------------------------------------+

.. note::
   The performance IDs defined in the :rgbled:`rgbled_perfids.h` file must have values
   that are unique system wide.  Most applications also have performance IDs.
   No two performance IDs may have the same value.
   

Platform Specific
^^^^^^^^^^^^^^^^^

+---------------------------------+-------------------------------------------+
| File                            | Parameter                                 |
+=================================+===========================================+
| :rgbled:`rgbled_msgids.h`       | :rgbled:`RGBLED_CMD_MID`                  |
+                                 +-------------------------------------------+
|                                 | :rgbled:`RGBLED_SEND_HK_MID`              |
+                                 +-------------------------------------------+
|                                 | :rgbled:`RGBLED_WAKEUP_MID`               |
+                                 +-------------------------------------------+
|                                 | :rgbled:`RGBLED_HK_TLM_MID`               |
+---------------------------------+-------------------------------------------+
| :rgbled:`rgbled_platform_cfg.h` | :rgbled:`RGBLED_SELFTEST_TASK_FLAGS`      |
+                                 +-------------------------------------------+
|                                 | :rgbled:`RGBLED_MISSION_REV`              |
+                                 +-------------------------------------------+
|                                 | :rgbled:`RGBLED_SCH_PIPE_DEPTH`           |
+                                 +-------------------------------------------+
|                                 | :rgbled:`RGBLED_SCH_PIPE_NAME`            |
+                                 +-------------------------------------------+
|                                 | :rgbled:`RGBLED_SCH_PIPE_PEND_TIME`       |
+                                 +-------------------------------------------+
|                                 | :rgbled:`RGBLED_WAKEUP_MID_MAX_MSG_COUNT` |
+                                 +-------------------------------------------+
|                                 | :rgbled:`RGBLED_SEND_HK_MID_MAX_MSG_COUNT`|
+                                 +-------------------------------------------+
|                                 | :rgbled:`RGBLED_CMD_PIPE_DEPTH`           |
+                                 +-------------------------------------------+
|                                 | :rgbled:`RGBLED_CMD_PIPE_NAME`            |
+                                 +-------------------------------------------+
|                                 | :rgbled:`RGBLED_DATA_PIPE_DEPTH`          |
+                                 +-------------------------------------------+
|                                 | :rgbled:`RGBLED_DATA_PIPE_NAME`           |
+                                 +-------------------------------------------+
|                                 | :rgbled:`RGBLED_CONFIG_TABLE_FILENAME`    |
+                                 +-------------------------------------------+
|                                 | :rgbled:`RGBLED_STARTUP_TIMEOUT_MSEC`     |
+---------------------------------+-------------------------------------------+

.. note::
   The message IDs defined in the :rgbled:`rgbled_msgids.h` file must have values
   that are unique system wide.  Most applications also have message IDs.
   No two message IDs may have the same value.


System Integration
^^^^^^^^^^^^^^^^^^

TODO: Describe System Integration instructions specific to RGBLED.


Run Time Configuration
^^^^^^^^^^^^^^^^^^^^^^

TODO: Describe Run Time Configuration for RGBLED.
