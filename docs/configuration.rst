Configuration
=============

TODO

Mission Specific
^^^^^^^^^^^^^^^^

TODO: Add Doxygen documentation for gps_mission_cfg.h

+----------------------------+-------------------------------------+
| File                       | Parameter                           |
+============================+=====================================+
| :gps:`gps_mission_cfg.h`   | :gps:`GPS_SB_TIMEOUT`               |
+----------------------------+-------------------------------------+
| :gps:`gps_perfids.h`       | :gps:`GPS_MAIN_TASK_PERF_ID`        |
+----------------------------+-------------------------------------+

.. note::
   The performance IDs defined in the :gps:`gps_perfids.h` file must have values
   that are unique system wide.  Most applications also have performance IDs.
   No two performance IDs may have the same value.
   

Platform Specific
^^^^^^^^^^^^^^^^^

+-----------------------------+---------------------------------------------+
| File                        | Parameter                                   |
+=============================+=============================================+
| :gps:`gps_msgids.h`         | :gps:`GPS_CMD_MID`                          |
+                             +---------------------------------------------+
|                             | :gps:`GPS_SEND_HK_MID`                      |
+                             +---------------------------------------------+
|                             | :gps:`GPS_READ_SENSOR_MID`                  |
+                             +---------------------------------------------+
|                             | :gps:`GPS_HK_TLM_MID`                       |
+-----------------------------+---------------------------------------------+
| :gps:`gps_platform_cfg.h`   | :gps:`GPS_MISSION_REV`                      |
+                             +---------------------------------------------+
|                             | :gps:`GPS_SCH_PIPE_DEPTH`                   |
+                             +---------------------------------------------+
|                             | :gps:`GPS_SCH_PIPE_NAME`                    |
+                             +---------------------------------------------+
|                             | :gps:`GPS_SCH_PIPE_PEND_TIME`               |
+                             +---------------------------------------------+
|                             | :gps:`GPS_READ_SENSOR_MID_MAX_MSG_COUNT`    |
+                             +---------------------------------------------+
|                             | :gps:`GPS_SEND_HK_MID_MAX_MSG_COUNT`        |
+                             +---------------------------------------------+
|                             | :gps:`GPS_CMD_PIPE_DEPTH`                   |
+                             +---------------------------------------------+
|                             | :gps:`GPS_CMD_PIPE_NAME`                    |
+                             +---------------------------------------------+
|                             | :gps:`GPS_DATA_PIPE_DEPTH`                  |
+                             +---------------------------------------------+
|                             | :gps:`GPS_DATA_PIPE_NAME`                   |
+                             +---------------------------------------------+
|                             | :gps:`GPS_CONFIG_TABLE_FILENAME`            |
+                             +---------------------------------------------+
|                             | :gps:`GPS_STARTUP_TIMEOUT_MSEC`             |
+-----------------------------+---------------------------------------------+

.. note::
   The message IDs defined in the :gps:`gps_msgids.h` file must have values
   that are unique system wide.  Most applications also have message IDs.
   No two message IDs may have the same value.


Table(s)
^^^^^^^^^^^^^^^^

+-------------------------------+------------------------------------+--------------------------------------------+
| Table Name                    | Default file name                  | Parameter                                  |
+===============================+====================================+============================================+
| :gps:`GPS_CONFIG_TABLENAME`   | :gps:`GPS_CONFIG_TABLE_FILENAME`   | :gps:`GPS_ConfigTbl_t::temp`               |
+-------------------------------+------------------------------------+--------------------------------------------+

Below is an example table.

.. literalinclude:: ../fsw/tables/gps_config.c
  :language: C
  :caption: Nominal Configuration Table


System Integration
^^^^^^^^^^^^^^^^^^

TODO: Describe System Integration instructions specific to GPS.


Run Time Configuration
^^^^^^^^^^^^^^^^^^^^^^

TODO: Describe Run Time Configuration for GPS.
