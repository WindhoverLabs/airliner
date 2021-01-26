Configuration
=============

Mission Specific
^^^^^^^^^^^^^^^^

TODO: Add Doxygen documentation for rcin_mission_cfg.h

+---------------------------+--------------------------------+
| File                      | Parameter                      |
+===========================+================================+
| :rcin:`rcin_mission_cfg.h`| :rcin:`RCIN_SB_TIMEOUT`        |
+---------------------------+--------------------------------+
| :rcin:`rcin_perfids.h`    | :rcin:`RCIN_MAIN_TASK_PERF_ID` |
+                           +--------------------------------+
|                           | :rcin:`RCIN_DEVICE_GET_PERF_ID`|
+---------------------------+--------------------------------+

.. note::
   The performance IDs defined in the :rcin:`rcin_perfids.h` file must have values
   that are unique system wide.  Most applications also have performance IDs.
   No two performance IDs may have the same value.
   

Platform Specific
^^^^^^^^^^^^^^^^^

+----------------------------+---------------------------------------+
| File                       | Parameter                             |
+============================+=======================================+
| :rcin:`rcin_msgids.h`      | :rcin:`RCIN_HK_TLM_MID`               |
+                            +---------------------------------------+
|                            | :rcin:`RCIN_SEND_HK_MID`              |
+                            +---------------------------------------+
|                            | :rcin:`RCIN_WAKEUP_MID`               |
+                            +---------------------------------------+
|                            | :rcin:`RCIN_CMD_MID`                  |
+----------------------------+---------------------------------------+
| :rcin:`rcin_platform_cfg.h`| :rcin:`RCIN_MISSION_REV`              |
+                            +---------------------------------------+
|                            | :rcin:`RCIN_SCH_PIPE_DEPTH`           |
+                            +---------------------------------------+
|                            | :rcin:`RCIN_SCH_PIPE_NAME`            |
+                            +---------------------------------------+
|                            | :rcin:`RCIN_SCH_PIPE_PEND_TIME`       |
+                            +---------------------------------------+
|                            | :rcin:`RCIN_WAKEUP_MID_MAX_MSG_COUNT` |
+                            +---------------------------------------+
|                            | :rcin:`RCIN_SEND_HK_MID_MAX_MSG_COUNT`|
+                            +---------------------------------------+
|                            | :rcin:`RCIN_CMD_PIPE_DEPTH`           |
+                            +---------------------------------------+
|                            | :rcin:`RCIN_CMD_PIPE_NAME`            |
+                            +---------------------------------------+
|                            | :rcin:`RCIN_DATA_PIPE_DEPTH`          |
+                            +---------------------------------------+
|                            | :rcin:`RCIN_DATA_PIPE_NAME`           |
+                            +---------------------------------------+
|                            | :rcin:`RCIN_CONFIG_TABLE_FILENAME`    |
+                            +---------------------------------------+
|                            | :rcin:`RCIN_STARTUP_TIMEOUT_MSEC`     |
+                            +---------------------------------------+
|                            | :rcin:`RCIN_CUSTOM_JOYSTICK_PATH`     |
+                            +---------------------------------------+
|                            | :rcin:`RCIN_MAX_EVENT_FILTERS`        |
+----------------------------+---------------------------------------+

.. note::
   The message IDs defined in the :rcin:`rcin_msgids.h` file must have values
   that are unique system wide.  Most applications also have message IDs.
   No two message IDs may have the same value.


System Integration
^^^^^^^^^^^^^^^^^^

TODO: Describe System Integration instructions specific to RCIN.


Run Time Configuration
^^^^^^^^^^^^^^^^^^^^^^

TODO: Describe Run Time Configuration for RCIN.
