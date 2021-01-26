Configuration
=============

Mission Specific
^^^^^^^^^^^^^^^^

TODO: Add Doxygen documentation for ulr_mission_cfg.h

+---------------------------+--------------------------------+
| File                      | Parameter                      |
+===========================+================================+
| :ulr:`ulr_mission_cfg.h`  | :ulr:`ULR_SB_TIMEOUT`          |
+---------------------------+--------------------------------+
| :ulr:`ulr_perfids.h`      | :ulr:`ULR_MAIN_TASK_PERF_ID`   |
+---------------------------+--------------------------------+

.. note::
   The performance IDs defined in the :ulr:`ulr_perfids.h` file must have values
   that are unique system wide.  Most applications also have performance IDs.
   No two performance IDs may have the same value.
   

Platform Specific
^^^^^^^^^^^^^^^^^

+--------------------------+-------------------------------------+
| File                     | Parameter                           |
+==========================+=====================================+
| :ulr:`ulr_msgids.h`      | :ulr:`ULR_CMD_MID`                  |
|                          +-------------------------------------+
|                          | :ulr:`ULR_SEND_HK_MID`              |
+                          +-------------------------------------+
|                          | :ulr:`ULR_MEASURE_MID`              |
+                          +-------------------------------------+
|                          | :ulr:`ULR_OUT_DATA_MID`             |
+                          +-------------------------------------+
|                          | :ulr:`ULR_HK_TLM_MID`               |
+--------------------------+-------------------------------------+
| :ulr:`ulr_platform_cfg.h`| :ulr:`ULR_LISTENER_TASK_FLAGS`      |
+                          +-------------------------------------+
|                          | :ulr:`ULR_MISSION_REV`              |
+                          +-------------------------------------+
|                          | :ulr:`ULR_SCH_PIPE_DEPTH`           |
+                          +-------------------------------------+
|                          | :ulr:`ULR_SCH_PIPE_NAME`            |
+                          +-------------------------------------+
|                          | :ulr:`ULR_SCH_PIPE_PEND_TIME`       |
+                          +-------------------------------------+
|                          | :ulr:`ULR_MEASURE_MID_MAX_MSG_COUNT`|
+                          +-------------------------------------+
|                          | :ulr:`ULR_SEND_HK_MID_MAX_MSG_COUNT`|
+                          +-------------------------------------+
|                          | :ulr:`ULR_CMD_PIPE_DEPTH`           |
+                          +-------------------------------------+
|                          | :ulr:`ULR_CMD_PIPE_NAME`            |
+                          +-------------------------------------+
|                          | :ulr:`ULR_DATA_PIPE_DEPTH`          |
+                          +-------------------------------------+
|                          | :ulr:`ULR_DATA_PIPE_NAME`           |
+                          +-------------------------------------+
|                          | :ulr:`ULR_CONFIG_TABLE_FILENAME`    |
+                          +-------------------------------------+
|                          | :ulr:`ULR_STARTUP_TIMEOUT_MSEC`     |
+                          +-------------------------------------+
|                          | :ulr:`ULR_CUSTOM_PORT_PATH`         |
+                          +-------------------------------------+
|                          | :ulr:`ULR_SENS_VARIANCE`            |
+                          +-------------------------------------+
|                          | :ulr:`ULR_LISTENER_TASK_PRIORITY`   |
+                          +-------------------------------------+
|                          | :ulr:`ULR_LISTENER_TASK_STACK_SIZE` |
+                          +-------------------------------------+
|                          | :ulr:`ULR_LISTENER_TASK_NAME`       |
+                          +-------------------------------------+
|                          | :ulr:`ULR_MUTEX_NAME`               |
+--------------------------+-------------------------------------+

.. note::
   The message IDs defined in the :ulr:`ulr_msgids.h` file must have values
   that are unique system wide.  Most applications also have message IDs.
   No two message IDs may have the same value.


System Integration
^^^^^^^^^^^^^^^^^^

TODO: Describe System Integration instructions specific to ULR.


Run Time Configuration
^^^^^^^^^^^^^^^^^^^^^^

TODO: Describe Run Time Configuration for ULR.
