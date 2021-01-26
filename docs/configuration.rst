Configuration
=============

Mission Specific
^^^^^^^^^^^^^^^^

TODO: Add Doxygen documentation for mpu9250_mission_cfg.h

+----------------------------------+-------------------------------------+
| File                             | Parameter                           |
+==================================+=====================================+
| :mpu9250:`mpu9250_mission_cfg.h` | :mpu9250:`MPU9250_SB_TIMEOUT`       |
+----------------------------------+-------------------------------------+
| :mpu9250:`mpu9250_perfids.h`     | :mpu9250:`MPU9250_MAIN_TASK_PERF_ID`|
+----------------------------------+-------------------------------------+

.. note::
   The performance IDs defined in the :mpu9250:`mpu9250_perfids.h` file must have values
   that are unique system wide.  Most applications also have performance IDs.
   No two performance IDs may have the same value.
   

Platform Specific
^^^^^^^^^^^^^^^^^

+-----------------------------------+---------------------------------------------+
| File                              | Parameter                                   |
+===================================+=============================================+
| :mpu9250:`mpu9250_msgids.h`       | :mpu9250:`MPU9250_SEND_HK_MID`              |
|                                   +---------------------------------------------+
|                                   | :mpu9250:`MPU9250_HK_TLM_MID`               |
+                                   +---------------------------------------------+
|                                   | :mpu9250:`MPU9250_DIAG_TLM_MID`             |
+                                   +---------------------------------------------+
|                                   | :mpu9250:`MPU9250_CMD_MID`                  |
+                                   +---------------------------------------------+
|                                   | :mpu9250:`MPU9250_MEASURE_MID`              |
+-----------------------------------+---------------------------------------------+
| :mpu9250:`mpu9250_platform_cfg.h` | :mpu9250:`MPU9250_MISSION_REV`              |
+                                   +---------------------------------------------+
|                                   | :mpu9250:`MPU9250_SCH_PIPE_DEPTH`           |
+                                   +---------------------------------------------+
|                                   | :mpu9250:`MPU9250_SCH_PIPE_NAME`            |
+                                   +---------------------------------------------+
|                                   | :mpu9250:`MPU9250_SCH_PIPE_PEND_TIME`       |
+                                   +---------------------------------------------+
|                                   | :mpu9250:`MPU9250_PARAM_PIPE_DEPTH`         |
+                                   +---------------------------------------------+
|                                   | :mpu9250:`MPU9250_PARAM_PIPE_NAME`          |
+                                   +---------------------------------------------+
|                                   | :mpu9250:`MPU9250_MEASURE_MID_MAX_MSG_COUNT`|
+                                   +---------------------------------------------+
|                                   | :mpu9250:`MPU9250_SEND_HK_MID_MAX_MSG_COUNT`|
+                                   +---------------------------------------------+
|                                   | :mpu9250:`MPU9250_CMD_PIPE_DEPTH`           |
+                                   +---------------------------------------------+
|                                   | :mpu9250:`MPU9250_CMD_PIPE_NAME`            |
+                                   +---------------------------------------------+
|                                   | :mpu9250:`MPU9250_DATA_PIPE_DEPTH`          |
+                                   +---------------------------------------------+
|                                   | :mpu9250:`MPU9250_DATA_PIPE_NAME`           |
+                                   +---------------------------------------------+
|                                   | :mpu9250:`MPU9250_CONFIG_TABLE_FILENAME`    |
+                                   +---------------------------------------------+
|                                   | :mpu9250:`MPU9250_STARTUP_TIMEOUT_MSEC`     |
+                                   +---------------------------------------------+
|                                   | :mpu9250:`MPU9250_ACCEL_PX4_DEVICE_ID`      |
+                                   +---------------------------------------------+
|                                   | :mpu9250:`MPU9250_GYRO_PX4_DEVICE_ID`       |
+                                   +---------------------------------------------+
|                                   | :mpu9250:`MPU9250_ACCEL_INT_PUB_RATE`       |
+                                   +---------------------------------------------+
|                                   | :mpu9250:`MPU9250_GYRO_INT_PUB_RATE`        |
+                                   +---------------------------------------------+
|                                   | :mpu9250:`MPU9250_ACC_SCALE`                |
+                                   +---------------------------------------------+
|                                   | :mpu9250:`MPU9250_GYRO_SCALE`               |
+                                   +---------------------------------------------+
|                                   | :mpu9250:`MPU9250_AK8963_ID`                |
+                                   +---------------------------------------------+
|                                   | :mpu9250:`MPU9250_DEVICE_ID`                |
+                                   +---------------------------------------------+
|                                   | :mpu9250:`MPU9250_ACCEL_SAMPLE_RATE`        |
+                                   +---------------------------------------------+
|                                   | :mpu9250:`MPU9250_ACCEL_FILTER_CUTOFF_FREQ` |
+                                   +---------------------------------------------+
|                                   | :mpu9250:`MPU9250_GYRO_SAMPLE_RATE`         |
+                                   +---------------------------------------------+
|                                   | :mpu9250:`MPU9250_GYRO_FILTER_CUTOFF_FREQ`  |
+                                   +---------------------------------------------+
|                                   | :mpu9250:`MPU9250_NEVER_AUTOPUBLISH_US`     |
+                                   +---------------------------------------------+
|                                   | :mpu9250:`MPU9250_ONE_G`                    |
+                                   +---------------------------------------------+
|                                   | :mpu9250:`MPU9250_RADIANS_PER_DEGREE`       |
+                                   +---------------------------------------------+
|                                   | :mpu9250:`MPU9250_ROOM_TEMP_OFFSET`         |
+                                   +---------------------------------------------+
|                                   | :mpu9250:`MPU9250_TEMP_SENS`                |
+-----------------------------------+---------------------------------------------+

.. note::
   The message IDs defined in the :mpu9250:`mpu9250_msgids.h` file must have values
   that are unique system wide.  Most applications also have message IDs.
   No two message IDs may have the same value.

Table(s)
^^^^^^^^

TODO: Add Table explanation

TODO: Add Doxygen documentation for mpu9250_config.tbl

+-------------------------------------+------------------------------------------+--------------------------------------------+
| Table Name                          | Default file name                        | Parameter                                  |
+=====================================+==========================================+============================================+
| :mpu9250:`MPU9250_CONFIG_TABLENAME` | :mpu9250:`MPU9250_CONFIG_TABLE_FILENAME` | :mpu9250:`MPU9250_ConfigTbl_t::AccXScale`  |
+                                     |                                          +--------------------------------------------+
|                                     |                                          | :mpu9250:`MPU9250_ConfigTbl_t::AccYScale`  |
+                                     |                                          +--------------------------------------------+
|                                     |                                          | :mpu9250:`MPU9250_ConfigTbl_t::AccZScale`  |
+                                     |                                          +--------------------------------------------+
|                                     |                                          | :mpu9250:`MPU9250_ConfigTbl_t::AccXOffset` |
+                                     |                                          +--------------------------------------------+
|                                     |                                          | :mpu9250:`MPU9250_ConfigTbl_t::AccYOffset` |
+                                     |                                          +--------------------------------------------+
|                                     |                                          | :mpu9250:`MPU9250_ConfigTbl_t::AccZOffset` |
+                                     |                                          +--------------------------------------------+
|                                     |                                          | :mpu9250:`MPU9250_ConfigTbl_t::GyroXScale` |
+                                     |                                          +--------------------------------------------+
|                                     |                                          | :mpu9250:`MPU9250_ConfigTbl_t::GyroYScale` |
+                                     |                                          +--------------------------------------------+
|                                     |                                          | :mpu9250:`MPU9250_ConfigTbl_t::GyroZScale` |
+                                     |                                          +--------------------------------------------+
|                                     |                                          | :mpu9250:`MPU9250_ConfigTbl_t::GyroXOffset`|
+                                     |                                          +--------------------------------------------+
|                                     |                                          | :mpu9250:`MPU9250_ConfigTbl_t::GyroYOffset`|
+                                     |                                          +--------------------------------------------+
|                                     |                                          | :mpu9250:`MPU9250_ConfigTbl_t::GyroZOffset`|
+-------------------------------------+------------------------------------------+--------------------------------------------+

TODO: Add Doxygen documentation for mpu9250_config.c

Below is an example table.

.. literalinclude:: ../fsw/tables/mpu9250_config.c
  :language: C
  :caption: Nominal Configuration Table


System Integration
^^^^^^^^^^^^^^^^^^

TODO: Describe System Integration instructions specific to MPU9250.


Run Time Configuration
^^^^^^^^^^^^^^^^^^^^^^

TODO: Describe Run Time Configuration for MPU9250.
