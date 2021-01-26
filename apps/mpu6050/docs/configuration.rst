Configuration
=============

Mission Specific
^^^^^^^^^^^^^^^^
TODO: The following do not exist in doxy.

+---------------------------------+-----------------------------------------+
| File                            | Parameter                               |
+=================================+=========================================+
| :mpu6050:`mpu6050_mission_cfg.h`| :mpu6050:`MPU6050_SB_TIMEOUT`           |
+---------------------------------+-----------------------------------------+
| :mpu6050:`mpu6050_perfids.h`    | :mpu6050:`MPU6050_MAIN_TASK_PERF_ID`    |
+                                 +-----------------------------------------+
|                                 | :mpu6050:`MPU6050_RECEIVE_PERF_ID`      |
|                                 +-----------------------------------------+
|                                 | :mpu6050:`MPU6050_SEND_PERF_ID`         |
+---------------------------------+-----------------------------------------+


.. note::
   The performance IDs defined in the :mpu6050:`mpu6050_perfids.h` file must have values
   that are unique system wide.  Most applications also have performance IDs.
   No two performance IDs may have the same value.
   

Platform Specific
^^^^^^^^^^^^^^^^^

+-----------------------------------+---------------------------------------------+
| File                              | Parameter                                   |
+===================================+=============================================+
| :mpu6050:`mpu6050_msgids.h`       | :mpu6050:`MPU6050_HK_TLM_MID`               |
|                                   +---------------------------------------------+
|                                   | :mpu6050:`MPU6050_SEND_HK_MID`              |
+                                   +---------------------------------------------+
|                                   | :mpu6050:`MPU6050_MEASURE_MID`              |
|                                   +---------------------------------------------+
|                                   | :mpu6050:`MPU6050_CMD_MID`                  |
+                                   +---------------------------------------------+
|                                   | :mpu6050:`MPU6050_DIAG_TLM_MID`             |
+-----------------------------------+---------------------------------------------+
| :mpu6050:`mpu6050_platform_cfg.h` | :mpu6050:`MPU6050_MISSION_REV`              |
+                                   +---------------------------------------------+
|                                   | :mpu6050:`MPU6050_SCH_PIPE_DEPTH`           |
+                                   +---------------------------------------------+
|                                   | :mpu6050:`MPU6050_SCH_PIPE_NAME`            |
+                                   +---------------------------------------------+
|                                   | :mpu6050:`MPU6050_SCH_PIPE_PEND_TIME`       |
+                                   +---------------------------------------------+
|                                   | :mpu6050:`MPU6050_PARAM_PIPE_DEPTH`         |
+                                   +---------------------------------------------+
|                                   | :mpu6050:`MPU6050_PARAM_PIPE_NAME`          |
+                                   +---------------------------------------------+
|                                   | :mpu6050:`MPU6050_MEASURE_MID_MAX_MSG_COUNT`|
+                                   +---------------------------------------------+
|                                   | :mpu6050:`MPU6050_SEND_HK_MID_MAX_MSG_COUNT`|
+                                   +---------------------------------------------+
|                                   | :mpu6050:`MPU6050_CMD_PIPE_DEPTH`           |
+                                   +---------------------------------------------+
|                                   | :mpu6050:`MPU6050_CMD_PIPE_NAME`            |
+                                   +---------------------------------------------+
|                                   | :mpu6050:`MPU6050_DATA_PIPE_DEPTH`          |
+                                   +---------------------------------------------+
|                                   | :mpu6050:`MPU6050_DATA_PIPE_NAME`           |
+                                   +---------------------------------------------+
|                                   | :mpu6050:`MPU6050_CONFIG_TABLE_FILENAME`    |
+                                   +---------------------------------------------+
|                                   | :mpu6050:`MPU6050_STARTUP_TIMEOUT_MSEC`     |
+                                   +---------------------------------------------+
|                                   | :mpu6050:`MPU6050_ACCEL_PX4_DEVICE_ID`      |
+                                   +---------------------------------------------+
|                                   | :mpu6050:`MPU6050_GYRO_PX4_DEVICE_ID`       |
+                                   +---------------------------------------------+
|                                   | :mpu6050:`MPU6050_ACCEL_INT_PUB_RATE`       |
+                                   +---------------------------------------------+
|                                   | :mpu6050:`MPU6050_GYRO_INT_PUB_RATE`        |
+                                   +---------------------------------------------+
|                                   | :mpu6050:`MPU6050_DEVICE_ID`                |
+                                   +---------------------------------------------+
|                                   | :mpu6050:`MPU6050_ACCEL_SAMPLE_RATE`        |
+                                   +---------------------------------------------+
|                                   | :mpu6050:`MPU6050_GYRO_SAMPLE_RATE`         |
+                                   +---------------------------------------------+
|                                   | :mpu6050:`MPU6050_GYRO_FILTER_CUTOFF_FREQ`  |
+                                   +---------------------------------------------+
|                                   | :mpu6050:`MPU6050_GYRO_SAMPLE_RATE`         |
+                                   +---------------------------------------------+
|                                   | :mpu6050:`MPU6050_GYRO_FILTER_CUTOFF_FREQ`  |
+                                   +---------------------------------------------+
|                                   | :mpu6050:`MPU6050_ONE_G`                    |
+                                   +---------------------------------------------+
|                                   | :mpu6050:`MPU6050_RADIANS_PER_DEGREE`       |
+                                   +---------------------------------------------+
|                                   | :mpu6050:`MPU6050_ROOM_TEMP_OFFSET`         |
+                                   +---------------------------------------------+
|                                   | :mpu6050:`MPU6050_TEMP_SENS`                |
+-----------------------------------+---------------------------------------------+

.. note::
   The message IDs defined in the :mpu6050:`mpu6050_msgids.h` file must have values
   that are unique system wide.  Most applications also have message IDs.
   No two message IDs may have the same value.
   
TODO: Describe the "custom" layer code.


Table(s)
^^^^^^^^
TODO: Confirm that no tables exist for this entry.


+---------------------------------------+-------------------------------------------+--------------------------------------------+
| Table Name                            | Default file name                         | Parameter                                  |
+=======================================+===========================================+============================================+
| :mpu6050:`MPU6050_CONFIG_TABLENAME`   | :mpu6050:`MPU6050_CONFIG_TABLE_FILENAME`  | :mpu6050:`MPU6050_ConfigTbl_t::AccXScale`  |
+                                       |                                           +--------------------------------------------+
|                                       |                                           | :mpu6050:`MPU6050_ConfigTbl_t::AccYScale`  |
+                                       |                                           +--------------------------------------------+
|                                       |                                           | :mpu6050:`MPU6050_ConfigTbl_t::AccZScale`  |
+                                       |                                           +--------------------------------------------+
|                                       |                                           | :mpu6050:`MPU6050_ConfigTbl_t::AccXOffset` |
+                                       |                                           +--------------------------------------------+
|                                       |                                           | :mpu6050:`MPU6050_ConfigTbl_t::AccYOffset` |
+                                       |                                           +--------------------------------------------+
|                                       |                                           | :mpu6050:`MPU6050_ConfigTbl_t::AccZOffset` |
+                                       |                                           +--------------------------------------------+
|                                       |                                           | :mpu6050:`MPU6050_ConfigTbl_t::GyroXScale` |
+                                       |                                           +--------------------------------------------+
|                                       |                                           | :mpu6050:`MPU6050_ConfigTbl_t::GyroYScale` |
+                                       |                                           +--------------------------------------------+
|                                       |                                           | :mpu6050:`MPU6050_ConfigTbl_t::GyroZScale` |
+                                       |                                           +--------------------------------------------+
|                                       |                                           | :mpu6050:`MPU6050_ConfigTbl_t::GyroXOffset`|
+                                       |                                           +--------------------------------------------+
|                                       |                                           | :mpu6050:`MPU6050_ConfigTbl_t::GyroYOffset`|
+                                       |                                           +--------------------------------------------+
|                                       |                                           | :mpu6050:`MPU6050_ConfigTbl_t::GyroZOffset`|
+---------------------------------------+-------------------------------------------+--------------------------------------------+


.. note::
   When configuring the backup table, consider the fact that, when this table is in affect, the system
   is already in a failure or partially degraded state.  Both the nominal and backup tables might be 
   identical, but it may be more appropriate for the backup table to have a reduced telemetry 
   definition to reduce the system load.  Be very careful on selecting what telemetry is in the backup 
   table.  Specifically, you may want to ensure that the backup table has enough telemetry defined in 
   the downlink to facilitate troubleshooting and recovering from the failure that caused the
   failover to the backup configuration in the first place.  Lastly, understand that while the nominal
   configuration table can also be changed before or during flight, the backup table cannot be changed
   without recompiling the TO binary.
   
Below are example tables.

.. literalinclude:: ../fsw/tables/mpu6050_config.c
  :language: C
  :caption: MPU6050 default config table


System Integration
^^^^^^^^^^^^^^^^^^




Run Time Configuration
^^^^^^^^^^^^^^^^^^^^^^


TODO: Add references.
