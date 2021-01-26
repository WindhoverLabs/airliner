Configuration
=============

Mission Specific
^^^^^^^^^^^^^^^^
TODO: The following do not exist in doxy.

+-------------------------+---------------------------------+
| File                    | Parameter                       |
+=========================+=================================+
| :mpc:`mpc_mission_cfg.h`| :mpc:`MPC_SB_TIMEOUT`           |
+-------------------------+---------------------------------+
| :mpc:`mpc_perfids.h`    | :mpc:`MPC_MAIN_TASK_PERF_ID`    |
+-------------------------+---------------------------------+


.. note::
   The performance IDs defined in the :mpc:`mpc_perfids.h` file must have values
   that are unique system wide.  Most applications also have performance IDs.
   No two performance IDs may have the same value.
   

Platform Specific
^^^^^^^^^^^^^^^^^

+---------------------------+----------------------------------------+
| File                      | Parameter                              |
+===========================+========================================+
| :mpc:`mpc_msgids.h`       | :mpc:`MPC_HK_TLM_MID`                  |
|                           +----------------------------------------+
|                           | :mpc:`MPC_SEND_HK_MID`                 |
+                           +----------------------------------------+
|                           | :mpc:`MPC_WAKEUP_MID`                  |
|                           +----------------------------------------+
|                           | :mpc:`MPC_CMD_MID`                     |
+                           +----------------------------------------+
|                           | :mpc:`MPC_DIAG_TLM_MID`                |
+---------------------------+----------------------------------------+
| :mpc:`mpc_platform_cfg.h` | :mpc:`MPC_MISSION_REV`                 |
+                           +----------------------------------------+
|                           | :mpc:`MPC_SCH_PIPE_DEPTH`              |
+                           +----------------------------------------+
|                           | :mpc:`MPC_SCH_PIPE_NAME`               |
+                           +----------------------------------------+
|                           | :mpc:`MPC_SCH_PIPE_PEND_TIME`          |
+                           +----------------------------------------+
|                           | :mpc:`MPC_WAKEUP_MID_MAX_MSG_COUNT`    |
+                           +----------------------------------------+
|                           | :mpc:`MPC_SEND_HK_MID_MAX_MSG_COUNT`   |
+                           +----------------------------------------+
|                           | :mpc:`MPC_CMD_PIPE_DEPTH`              |
+                           +----------------------------------------+
|                           | :mpc:`MPC_CMD_PIPE_NAME`               |
+                           +----------------------------------------+
|                           | :mpc:`MPC_CONFIG_TABLE_FILENAME`       |
+                           +----------------------------------------+
|                           | :mpc:`MPC_STARTUP_TIMEOUT_MSEC`        |
+                           +----------------------------------------+
|                           | :mpc:`DIRECTION_CHANGE_TRIGGER_TIME_US`|
+---------------------------+----------------------------------------+

.. note::
   The message IDs defined in the :mpc:`mpc_msgids.h` file must have values
   that are unique system wide.  Most applications also have message IDs.
   No two message IDs may have the same value.
   
TODO: Describe the "custom" layer code.


Table(s)
^^^^^^^^
TODO: Confiirm that no tables exit for this entry.


+-------------------------------+------------------------------------+------------------------------------------+
| Table Name                    | Default file name                  | Parameter                                |
+===============================+====================================+==========================================+
| :mpc:`MPC_CONFIG_TABLENAME`   | :mpc:`MPC_CONFIG_TABLE_FILENAME`   | :mpc:`MPC_ConfigTbl_t::THR_MIN`          |
+                               |                                    +------------------------------------------+
|                               |                                    | :mpc:`MPC_ConfigTbl_t::THR_HOVER`        |
+                               |                                    +------------------------------------------+
|                               |                                    | :mpc:`MPC_ConfigTbl_t::THR_MAX`          |
+                               |                                    +------------------------------------------+
|                               |                                    | :mpc:`MPC_ConfigTbl_t::MANTHR_MIN`       |
+                               |                                    +------------------------------------------+
|                               |                                    | :mpc:`MPC_ConfigTbl_t::MANTHR_MAX`       |
+                               |                                    +------------------------------------------+
|                               |                                    | :mpc:`MPC_ConfigTbl_t::Z_P`              |
+                               |                                    +------------------------------------------+
|                               |                                    | :mpc:`MPC_ConfigTbl_t::Z_VEL_P`          |
+                               |                                    +------------------------------------------+
|                               |                                    | :mpc:`MPC_ConfigTbl_t::Z_VEL_I`          |
+                               |                                    +------------------------------------------+
|                               |                                    | :mpc:`MPC_ConfigTbl_t::Z_VEL_D`          |
+                               |                                    +------------------------------------------+
|                               |                                    | :mpc:`MPC_ConfigTbl_t::Z_VEL_MAX_UP`     |
+                               |                                    +------------------------------------------+
|                               |                                    | :mpc:`MPC_ConfigTbl_t::Z_VEL_MAX_DN`     |
+                               |                                    +------------------------------------------+
|                               |                                    | :mpc:`MPC_ConfigTbl_t::Z_FF`             |
+                               |                                    +------------------------------------------+
|                               |                                    | :mpc:`MPC_ConfigTbl_t::XY_P`             |
+                               |                                    +------------------------------------------+
|                               |                                    | :mpc:`MPC_ConfigTbl_t::XY_VEL_P`         |
+                               |                                    +------------------------------------------+
|                               |                                    | :mpc:`MPC_ConfigTbl_t::XY_VEL_I`         |
+                               |                                    +------------------------------------------+
|                               |                                    | :mpc:`MPC_ConfigTbl_t::XY_VEL_D`         |
+                               |                                    +------------------------------------------+
|                               |                                    | :mpc:`MPC_ConfigTbl_t::XY_CRUISE`        |
+                               |                                    +------------------------------------------+
|                               |                                    | :mpc:`MPC_ConfigTbl_t::TARGET_THRE`      |
+                               |                                    +------------------------------------------+
|                               |                                    | :mpc:`MPC_ConfigTbl_t::XY_VEL_MAX`       |
+                               |                                    +------------------------------------------+
|                               |                                    | :mpc:`MPC_ConfigTbl_t::XY_FF`            |
+                               |                                    +------------------------------------------+
|                               |                                    | :mpc:`MPC_ConfigTbl_t::TILTMAX_AIR`      |
+                               |                                    +------------------------------------------+
|                               |                                    | :mpc:`MPC_ConfigTbl_t::TILTMAX_LND`      |
+                               |                                    +------------------------------------------+
|                               |                                    | :mpc:`MPC_ConfigTbl_t::LAND_SPEED`       |
+                               |                                    +------------------------------------------+
|                               |                                    | :mpc:`MPC_ConfigTbl_t::TKO_SPEED`        |
+                               |                                    +------------------------------------------+
|                               |                                    | :mpc:`MPC_ConfigTbl_t::MAN_TILT_MAX`     |
+                               |                                    +------------------------------------------+
|                               |                                    | :mpc:`MPC_ConfigTbl_t::MAN_Y_MAX`        |
+                               |                                    +------------------------------------------+
|                               |                                    | :mpc:`MPC_ConfigTbl_t::HOLD_DZ`          |
+                               |                                    +------------------------------------------+
|                               |                                    | :mpc:`MPC_ConfigTbl_t::HOLD_MAX_XY`      |
+                               |                                    +------------------------------------------+
|                               |                                    | :mpc:`MPC_ConfigTbl_t::HOLD_MAX_Z`       |
+                               |                                    +------------------------------------------+
|                               |                                    | :mpc:`MPC_ConfigTbl_t::VELD_LP`          |
+                               |                                    +------------------------------------------+
|                               |                                    | :mpc:`MPC_ConfigTbl_t::ACC_HOR_MAX`      |
+                               |                                    +------------------------------------------+
|                               |                                    | :mpc:`MPC_ConfigTbl_t::ACC_UP_MAX`       |
+                               |                                    +------------------------------------------+
|                               |                                    | :mpc:`MPC_ConfigTbl_t::ACC_DOWN_MAX`     |
+                               |                                    +------------------------------------------+
|                               |                                    | :mpc:`MPC_ConfigTbl_t::ALT_MODE`         |
+                               |                                    +------------------------------------------+
|                               |                                    | :mpc:`MPC_ConfigTbl_t::XY_MAN_EXPO`      |
+                               |                                    +------------------------------------------+
|                               |                                    | :mpc:`MPC_ConfigTbl_t::Z_MAN_EXPO`       |
+                               |                                    +------------------------------------------+
|                               |                                    | :mpc:`MPC_ConfigTbl_t::LAND_ALT1`        |
+                               |                                    +------------------------------------------+
|                               |                                    | :mpc:`MPC_ConfigTbl_t::LAND_ALT2`        |
+                               |                                    +------------------------------------------+
|                               |                                    | :mpc:`MPC_ConfigTbl_t::TKO_RAMP_T`       |
+                               |                                    +------------------------------------------+
|                               |                                    | :mpc:`MPC_ConfigTbl_t::MC_YAWRATE_MAX`   |
+                               |                                    +------------------------------------------+
|                               |                                    | :mpc:`MPC_ConfigTbl_t::MC_YAW_P`         |
+                               |                                    +------------------------------------------+
|                               |                                    | :mpc:`MPC_ConfigTbl_t::VT_OPT_RECOV_EN`  |
+                               |                                    +------------------------------------------+
|                               |                                    | :mpc:`MPC_ConfigTbl_t::MPC_CRUISE_90`    |
+                               |                                    +------------------------------------------+
|                               |                                    | :mpc:`MPC_ConfigTbl_t::MPC_JERK_MAX`     |
+                               |                                    +------------------------------------------+
|                               |                                    | :mpc:`MPC_ConfigTbl_t::MPC_JERK_MIN`     |
+                               |                                    +------------------------------------------+
|                               |                                    | :mpc:`MPC_ConfigTbl_t::MPC_DEC_HOR_SLOW` |
+                               |                                    +------------------------------------------+
|                               |                                    | :mpc:`MPC_ConfigTbl_t::MPC_VEL_MANUAL`   |
+                               |                                    +------------------------------------------+
|                               |                                    | :mpc:`MPC_ConfigTbl_t::NAV_ACC_RAD`      |
+                               |                                    +------------------------------------------+
|                               |                                    | :mpc:`MPC_ConfigTbl_t::NAV_MIS_YAW_ERR`  |
+-------------------------------+------------------------------------+------------------------------------------+


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

.. literalinclude:: ../fsw/tables/mpc_config.c
  :language: C
  :caption: MPC default config table


System Integration
^^^^^^^^^^^^^^^^^^




Run Time Configuration
^^^^^^^^^^^^^^^^^^^^^^


TODO: Add references.
