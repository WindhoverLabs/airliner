Configuration
=============

TODO

Mission Specific
^^^^^^^^^^^^^^^^

TODO: Add Doxygen documentation for amc_mission_cfg.h

+----------------------------+-------------------------------------+
| File                       | Parameter                           |
+============================+=====================================+
| :amc:`amc_mission_cfg.h`   | :amc:`AMC_SB_TIMEOUT`               |
+----------------------------+-------------------------------------+
| :amc:`amc_perfids.h`       | :amc:`AMC_MAIN_TASK_PERF_ID`        |
+                            +-------------------------------------+
|                            | :amc:`AMC_SEND_PERF_ID`             |
+                            +-------------------------------------+
|                            | :AMC:`AMC_RECEIVE_PERF_ID`          |
+----------------------------+-------------------------------------+

.. note::
   The performance IDs defined in the :amc:`amc_perfids.h` file must have values
   that are unique system wide.  Most applications also have performance IDs.
   No two performance IDs may have the same value.
   

Platform Specific
^^^^^^^^^^^^^^^^^

+-----------------------------+---------------------------------------------+
| File                        | Parameter                                   |
+=============================+=============================================+
| :AMC:`amc_msgids.h`         | :AMC:`AMC_CMD_MID`                          |
+                             +---------------------------------------------+
|                             | :AMC:`AMC_SEND_HK_MID`                      |
+                             +---------------------------------------------+
|                             | :AMC:`AMC_UPDATE_MOTORS_MID`                |
+                             +---------------------------------------------+
|                             | :AMC:`AMC_OUT_DATA_MID`                     |
+                             +---------------------------------------------+
|                             | :AMC:`AMC_HK_TLM_MID`                       |
+-----------------------------+---------------------------------------------+
| :AMC:`amc_platform_cfg.h`   | :AMC:`AMC_MISSION_REV`                      |
+                             +---------------------------------------------+
|                             | :AMC:`AMC_SCH_PIPE_DEPTH`                   |
+                             +---------------------------------------------+
|                             | :AMC:`AMC_SCH_PIPE_NAME`                    |
+                             +---------------------------------------------+
|                             | :AMC:`AMC_SCH_PIPE_PEND_TIME`               |
+                             +---------------------------------------------+
|                             | :AMC:`AMC_SCH_PIPE_WAKEUP_RESERVED`         |
+                             +---------------------------------------------+
|                             | :AMC:`AMC_SCH_PIPE_SEND_HK_RESERVED`        |
+                             +---------------------------------------------+
|                             | :AMC:`AMC_CMD_PIPE_DEPTH`                   |
+                             +---------------------------------------------+
|                             | :AMC:`AMC_CMD_PIPE_NAME`                    |
+                             +---------------------------------------------+
|                             | :AMC:`AMC_PARAM_PIPE_DEPTH`                 |
+                             +---------------------------------------------+
|                             | :AMC:`AMC_PARAM_PIPE_NAME`                  |
+                             +---------------------------------------------+
|                             | :AMC:`AMC_CONFIG_TABLE_FILENAME`            |
+                             +---------------------------------------------+
|                             | :AMC:`AMC_MIXER_CONFIG_TABLE_FILENAME`      |
+                             +---------------------------------------------+
|                             | :AMC:`AMC_STARTUP_TIMEOUT_MSEC`             |
+                             +---------------------------------------------+
|                             | :AMC:`AMC_MAX_MOTOR_OUTPUTS`                |
+                             +---------------------------------------------+
|                             | :AMC:`AMC_MAX_PARAMS`                       |
+-----------------------------+---------------------------------------------+

.. note::
   The message IDs defined in the :amc:`amc_msgids.h` file must have values
   that are unique system wide.  Most applications also have message IDs.
   No two message IDs may have the same value.

Tables
^^^^^^

+--------------------------------+------------------------------------+------------------------------------------------+
| Table Name                     | Default file name                  | Parameter                                      |
+================================+====================================+================================================+
| :amc:`AMC_CONFIG_TABLENAME`    | :amc:`AMC_CONFIG_TABLE_FILENAME`   | :amc:`AMC_ConfigTbl_t::PwmDisarmed`            |
+                                |                                    +------------------------------------------------+
|                                |                                    | :amc:`AMC_ConfigTbl_t::PwmMin`                 |
+                                |                                    +------------------------------------------------+
|                                |                                    | :amc:`AMC_ConfigTbl_t::PwmMax`                 |
+--------------------------------+------------------------------------+------------------------------------------------+


System Integration
^^^^^^^^^^^^^^^^^^

TODO: Describe System Integration instructions specific to AMC.


Run Time Configuration
^^^^^^^^^^^^^^^^^^^^^^

TODO: Describe Run Time Configuration for AMC.
