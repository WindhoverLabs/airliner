Configuration
=============

Mission Specific
^^^^^^^^^^^^^^^^

TODO: Add Doxygen documentation for pe_mission_cfg.h

+-------------------------+--------------------------------+
| File                    | Parameter                      |
+=========================+================================+
| :pe:`pe_mission_cfg.h`  | :pe:`PE_SB_TIMEOUT`            |
|                         +--------------------------------+
|                         | :pe:`PE_ONE_G`                 |
+-------------------------+--------------------------------+
| :pe:`pe_perfids.h`      | :pe:`PE_MAIN_TASK_PERF_ID`     |
+                         +--------------------------------+
|                         | :pe:`PE_UPDATE_TASK_PERF_ID`   |
+                         +--------------------------------+
|                         | :pe:`PE_SENSOR_GPS_PERF_ID`    |
+                         +--------------------------------+
|                         | :pe:`PE_SENSOR_BARO_PERF_ID`   |
+                         +--------------------------------+
|                         | :pe:`PE_SENSOR_LAND_PERF_ID`   |
+                         +--------------------------------+
|                         | :pe:`PE_INVERSE_MAT_PERF_ID`   |
+                         +--------------------------------+
|                         | :pe:`PE_SENSOR_FLOW_PERF_ID`   |
+                         +--------------------------------+
|                         | :pe:`PE_SENSOR_DIST_PERF_ID`   |
+-------------------------+--------------------------------+

.. note::
   The performance IDs defined in the :pe:`pe_perfids.h` file must have values
   that are unique system wide.  Most applications also have performance IDs.
   No two performance IDs may have the same value.
   

Platform Specific
^^^^^^^^^^^^^^^^^

+-------------------------+-------------------------------------+
| File                    | Parameter                           |
+=========================+=====================================+
| :pe:`pe_msgids.h`       | :pe:`PE_HK_TLM_MID`                 |
|                         +-------------------------------------+
|                         | :pe:`PE_SEND_HK_MID`                |
+                         +-------------------------------------+
|                         | :pe:`PE_WAKEUP_MID`                 |
+                         +-------------------------------------+
|                         | :pe:`PE_CMD_MID`                    |
+-------------------------+-------------------------------------+
| :pe:`pe_platform_cfg.h` | :pe:`PE_MISSION_REV`                |
+                         +-------------------------------------+
|                         | :pe:`PE_SCH_PIPE_DEPTH`             |
+                         +-------------------------------------+
|                         | :pe:`PE_SCH_PIPE_NAME`              |
+                         +-------------------------------------+
|                         | :pe:`PE_SCH_PIPE_PEND_TIME`         |
+                         +-------------------------------------+
|                         | :pe:`PE_WAKEUP_MID_MAX_MSG_COUNT`   |
+                         +-------------------------------------+
|                         | :pe:`PE_SEND_HK_MID_MAX_MSG_COUNT`  |
+                         +-------------------------------------+
|                         | :pe:`PE_CMD_PIPE_DEPTH`             |
+                         +-------------------------------------+
|                         | :pe:`PE_CMD_PIPE_NAME`              |
+                         +-------------------------------------+
|                         | :pe:`PE_CONFIG_TABLE_FILENAME`      |
+                         +-------------------------------------+
|                         | :pe:`PE_STARTUP_TIMEOUT_MSEC`       |
+                         +-------------------------------------+
|                         | :pe:`PE_MAX_EVENT_FILTERS`          |
+                         +-------------------------------------+
|                         | :pe:`REQ_BARO_INIT_COUNT`           |
+                         +-------------------------------------+
|                         | :pe:`BARO_TIMEOUT`                  |
+                         +-------------------------------------+
|                         | :pe:`REQ_GPS_INIT_COUNT`            |
+                         +-------------------------------------+
|                         | :pe:`GPS_TIMEOUT`                   |
+                         +-------------------------------------+
|                         | :pe:`REQ_LAND_INIT_COUNT`           |
+                         +-------------------------------------+
|                         | :pe:`LAND_TIMEOUT`                  |
+                         +-------------------------------------+
|                         | :pe:`REQ_DIST_INIT_COUNT`           |
+                         +-------------------------------------+
|                         | :pe:`DIST_TIMEOUT`                  |
+                         +-------------------------------------+
|                         | :pe:`DIST_BETA_MAX`                 |
+                         +-------------------------------------+
|                         | :pe:`DIST_SENSOR_TYPE`              |
+                         +-------------------------------------+
|                         | :pe:`REQ_FLOW_INIT_COUNT`           |
+                         +-------------------------------------+
|                         | :pe:`FLOW_TIMEOUT`                  |
+                         +-------------------------------------+
|                         | :pe:`FLOW_GYRO_HP_CUTOFF`           |
+-------------------------+-------------------------------------+

.. note::
   The message IDs defined in the :pe:`pe_msgids.h` file must have values
   that are unique system wide.  Most applications also have message IDs.
   No two message IDs may have the same value.
   
TODO: Describe the "custom" layer code.


Table(s)
^^^^^^^^

TODO: Add Table explanation

TODO: Add Doxygen documentation for pe_config.tbl

+-------------------------------+------------------------------------+------------------------------------------+
| Table Name                    | Default file name                  | Parameter                                |
+===============================+====================================+==========================================+
| :pe:`PE_CONFIG_TABLENAME`     | :pe:`PE_CONFIG_TABLE_FILENAME`     | :pe:`PE_ConfigTbl_t::VXY_PUB_THRESH`     |
+                               |                                    +------------------------------------------+
|                               |                                    | :pe:`PE_ConfigTbl_t::Z_PUB_THRESH`       |
+                               |                                    +------------------------------------------+
|                               |                                    | :pe:`PE_ConfigTbl_t::ACCEL_XY_STDDEV`    |
+                               |                                    +------------------------------------------+
|                               |                                    | :pe:`PE_ConfigTbl_t::ACCEL_Z_STDDEV`     |
+                               |                                    +------------------------------------------+
|                               |                                    | :pe:`PE_ConfigTbl_t::BARO_FUSE`          |
+                               |                                    +------------------------------------------+
|                               |                                    | :pe:`PE_ConfigTbl_t::BARO_STDDEV`        |
+                               |                                    +------------------------------------------+
|                               |                                    | :pe:`PE_ConfigTbl_t::GPS_FUSE`           |
+                               |                                    +------------------------------------------+
|                               |                                    | :pe:`PE_ConfigTbl_t::GPS_DELAY`          |
+                               |                                    +------------------------------------------+
|                               |                                    | :pe:`PE_ConfigTbl_t::GPS_XY_STDDEV`      |
+                               |                                    +------------------------------------------+
|                               |                                    | :pe:`PE_ConfigTbl_t::GPS_Z_STDDEV`       |
+                               |                                    +------------------------------------------+
|                               |                                    | :pe:`PE_ConfigTbl_t::GPS_VXY_STDDEV`     |
+                               |                                    +------------------------------------------+
|                               |                                    | :pe:`PE_ConfigTbl_t::GPS_VZ_STDDEV`      |
+                               |                                    +------------------------------------------+
|                               |                                    | :pe:`PE_ConfigTbl_t::GPS_EPH_MAX`        |
+                               |                                    +------------------------------------------+
|                               |                                    | :pe:`PE_ConfigTbl_t::GPS_EPV_MAX`        |
+                               |                                    +------------------------------------------+
|                               |                                    | :pe:`PE_ConfigTbl_t::LAND_FUSE`          |
+                               |                                    +------------------------------------------+
|                               |                                    | :pe:`PE_ConfigTbl_t::LAND_Z_STDDEV`      |
+                               |                                    +------------------------------------------+
|                               |                                    | :pe:`PE_ConfigTbl_t::LAND_VXY_STDDEV`    |
+                               |                                    +------------------------------------------+
|                               |                                    | :pe:`PE_ConfigTbl_t::PN_P_NOISE_DENSITY` |
+                               |                                    +------------------------------------------+
|                               |                                    | :pe:`PE_ConfigTbl_t::PN_V_NOISE_DENSITY` |
+                               |                                    +------------------------------------------+
|                               |                                    | :pe:`PE_ConfigTbl_t::PN_B_NOISE_DENSITY` |
+                               |                                    +------------------------------------------+
|                               |                                    | :pe:`PE_ConfigTbl_t::PN_T_NOISE_DENSITY` |
+                               |                                    +------------------------------------------+
|                               |                                    | :pe:`PE_ConfigTbl_t::T_MAX_GRADE`        |
+                               |                                    +------------------------------------------+
|                               |                                    | :pe:`PE_ConfigTbl_t::FAKE_ORIGIN`        |
+                               |                                    +------------------------------------------+
|                               |                                    | :pe:`PE_ConfigTbl_t::INIT_ORIGIN_LAT`    |
+                               |                                    +------------------------------------------+
|                               |                                    | :pe:`PE_ConfigTbl_t::INIT_ORIGIN_LON`    |
+                               |                                    +------------------------------------------+
|                               |                                    | :pe:`PE_ConfigTbl_t::DIST_FUSE`          |
+                               |                                    +------------------------------------------+
|                               |                                    | :pe:`PE_ConfigTbl_t::DIST_STDDEV`        |
+                               |                                    +------------------------------------------+
|                               |                                    | :pe:`PE_ConfigTbl_t::DIST_OFF_Z`         |
+                               |                                    +------------------------------------------+
|                               |                                    | :pe:`PE_ConfigTbl_t::FLOW_FUSE`          |
+                               |                                    +------------------------------------------+
|                               |                                    | :pe:`PE_ConfigTbl_t::FLOW_SCALE`         |
+                               |                                    +------------------------------------------+
|                               |                                    | :pe:`PE_ConfigTbl_t::FLOW_R`             |
+                               |                                    +------------------------------------------+
|                               |                                    | :pe:`PE_ConfigTbl_t::FLOW_RR`            |
+                               |                                    +------------------------------------------+
|                               |                                    | :pe:`PE_ConfigTbl_t::FLOW_QUALITY_MIN`   |
+                               |                                    +------------------------------------------+
|                               |                                    | :pe:`PE_ConfigTbl_t::FLOW_MIN_AGL`       |
+-------------------------------+------------------------------------+------------------------------------------+

TODO: Add Doxygen documentation for pe_config.c

Below is an example table.

.. literalinclude:: ../fsw/tables/pe_config.c
  :language: C
  :caption: Nominal Configuration Table


System Integration
^^^^^^^^^^^^^^^^^^

TODO: Describe System Integration instructions specific to PE.


Run Time Configuration
^^^^^^^^^^^^^^^^^^^^^^

TODO: Describe Run Time Configuration for PE.
