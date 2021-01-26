Configuration
=============

Mission Specific
^^^^^^^^^^^^^^^^

+-------------------------+--------------------------------+
| File                    | Parameter                      |
+=========================+================================+
| :vc:`vc_mission_cfg.h`  | :vc:`VC_SB_TIMEOUT`            |
+-------------------------+--------------------------------+
| :vc:`vc_perfids.h`      | :vc:`VC_MAIN_TASK_PERF_ID`     |
+                         +--------------------------------+
|                         | :vc:`VC_SOCKET_SEND_PERF_ID`   |
+                         +--------------------------------+
|                         | :vc:`VC_DEVICE_GET_PERF_ID`    |
+-------------------------+--------------------------------+

.. note::
   The performance IDs defined in the :vc:`vc_perfids.h` file must have values
   that are unique system wide.  Most applications also have performance IDs.
   No two performance IDs may have the same value.
   

Platform Specific
^^^^^^^^^^^^^^^^^

+-------------------------+-------------------------------------+
| File                    | Parameter                           |
+=========================+=====================================+
| :vc:`vc_msgids.h`       | :vc:`VC_HK_TLM_MID`                 |
+                         +-------------------------------------+
|                         | :vc:`VC_SEND_HK_MID`                |
+                         +-------------------------------------+
|                         | :vc:`VC_CMD_MID`                    |
+                         +-------------------------------------+
|                         | :vc:`VC_PROCESS_CMDS_MID`           |
+-------------------------+-------------------------------------+
| :vc:`vc_platform_cfg.h` | :vc:`VC_MISSION_REV`                |
+                         +-------------------------------------+
|                         | :vc:`VC_SCH_PIPE_DEPTH`             |
+                         +-------------------------------------+
|                         | :vc:`VC_SCH_PIPE_NAME`              |
+                         +-------------------------------------+
|                         | :vc:`VC_SCH_PIPE_PEND_TIME`         |
+                         +-------------------------------------+
|                         | :vc:`VC_SCH_PIPE_WAKEUP_RESERVED`   |
+                         +-------------------------------------+
|                         | :vc:`VC_SCH_PIPE_SEND_HK_RESERVED`  |
+                         +-------------------------------------+
|                         | :vc:`VC_CMD_PIPE_DEPTH`             |
+                         +-------------------------------------+
|                         | :vc:`VC_CMD_PIPE_NAME`              |
+                         +-------------------------------------+
|                         | :vc:`VC_STARTUP_TIMEOUT_MSEC`       |
+                         +-------------------------------------+
|                         | :vc:`VC_ADDRESS_LENGTH`             |
+                         +-------------------------------------+
|                         | :vc:`VC_MAX_OUTPUT_CHANNELS`        |
+                         +-------------------------------------+
|                         | :vc:`VC_MAX_DEVICE_PATH`            |
+                         +-------------------------------------+
|                         | :vc:`VC_MAX_DEVICES`                |
+                         +-------------------------------------+
|                         | :vc:`VC_MAX_BUFFER_SIZE`            |
+                         +-------------------------------------+
|                         | :vc:`VC_MAX_PACKET_SIZE`            |
+                         +-------------------------------------+
|                         | :vc:`VC_MAX_RETRY_ATTEMPTS`         |
+                         +-------------------------------------+
|                         | :vc:`VC_MAX_RETRY_SLEEP_USEC`       |
+                         +-------------------------------------+
|                         | :vc:`VC_STREAMING_TASK_PRIORITY`    |
+                         +-------------------------------------+
|                         | :vc:`VC_STREAMING_TASK_NAME`        |
+                         +-------------------------------------+
|                         | :vc:`VC_FRAME_WIDTH`                |
+                         +-------------------------------------+
|                         | :vc:`VC_FRAME_HEIGHT`               |
+                         +-------------------------------------+
|                         | :vc:`VC_FRAMERATE`                  |
+                         +-------------------------------------+
|                         | :vc:`VC_DEVICE_PATH`                |
+                         +-------------------------------------+
|                         | :vc:`VC_DESTINATION_IP`             |
+                         +-------------------------------------+
|                         | :vc:`VC_DESTINATION_PORT`           |
+                         +-------------------------------------+
|                         | :vc:`VC_SOURCE_IP`                  |
+                         +-------------------------------------+
|                         | :vc:`VC_SOURCE_PORT`                |
+                         +-------------------------------------+
|                         | :vc:`VC_V4L_IO_METHOD`              |
+                         +-------------------------------------+
|                         | :vc:`VC_V4L_BUFFER_TYPE`            |
+                         +-------------------------------------+
|                         | :vc:`VC_V4L_VIDEO_FORMAT`           |
+                         +-------------------------------------+
|                         | :vc:`VC_V4L_VIDEO_FIELD_ORDER`      |
+                         +-------------------------------------+
|                         | :vc:`VC_V4L_BUFFER_REQUEST`         |
+                         +-------------------------------------+
|                         | :vc:`VC_V4L_MEMORY_TYPE`            |
+                         +-------------------------------------+
|                         | :vc:`VC_BUFFER_FILL_TIMEOUT_SEC`    |
+                         +-------------------------------------+
|                         | :vc:`VC_BUFFER_FILL_TIMEOUT_USEC`   |
+                         +-------------------------------------+
|                         | :vc:`VC_BUFFER_TIMEOUTS_ALLOWED`    |
+-------------------------+-------------------------------------+

.. note::
   The message IDs defined in the :vc:`vc_msgids.h` file must have values
   that are unique system wide.  Most applications also have message IDs.
   No two message IDs may have the same value.


System Integration
^^^^^^^^^^^^^^^^^^

TODO: Describe System Integration instructions specific to VC.


Run Time Configuration
^^^^^^^^^^^^^^^^^^^^^^

TODO: Describe Run Time Configuration for VC.
