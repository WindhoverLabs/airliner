Configuration
=============

Mission Specific
^^^^^^^^^^^^^^^^
TODO: The following do not exist in doxy.

+-------------------------+--------------------------------+
| File                    | Parameter                      |
+=========================+================================+
| :fac:`fac_mission_cfg.h`| :fac:`FAC_SB_TIMEOUT`          |
+-------------------------+--------------------------------+
| :fac:`fac_perfids.h`    | :fac:`FAC_MAIN_TASK_PERF_ID`   |
+-------------------------+--------------------------------+


.. note::
   The performance IDs defined in the :fac:`fac_perfids.h` file must have values
   that are unique system wide.  Most applications also have performance IDs.
   No two performance IDs may have the same value.
   

Platform Specific
^^^^^^^^^^^^^^^^^

+---------------------------+-------------------------------------+
| File                      | Parameter                           |
+===========================+=====================================+
| :fac:`fac_msgids.h`       | :fac:`FAC_CMD_MID`                  |
|                           +-------------------------------------+
|                           | :fac:`FAC_SEND_HK_MID`              |
+                           +-------------------------------------+
|                           | :fac:`FAC_RUN_CONTROLLER_MID`       |
+                           +-------------------------------------+
|                           | :fac:`FAC_OUT_DATA_MID`             |
+                           +-------------------------------------+
|                           | :fac:`FAC_HK_TLM_MID`               |
+---------------------------+-------------------------------------+
| :fac:`fac_platform_cfg.h` | :fac:`FAC_MISSION_REV`              |
+                           +-------------------------------------+
|                           | :fac:`FAC_SCH_PIPE_DEPTH`           |
+                           +-------------------------------------+
|                           | :fac:`FAC_SCH_PIPE_NAME`            |
+                           +-------------------------------------+
|                           | :fac:`FAC_SCH_PIPE_PEND_TIME`       |
+                           +-------------------------------------+
|                           | :fac:`FAC_SCH_PIPE_WAKEUP_RESERVED` |
+                           +-------------------------------------+
|                           | :fac:`FAC_SCH_PIPE_SEND_HK_RESERVED`|
+                           +-------------------------------------+
|                           | :fac:`FAC_CMD_PIPE_DEPTH`           |
+                           +-------------------------------------+
|                           | :fac:`FAC_CMD_PIPE_NAME`            |
+                           +-------------------------------------+
|                           | :fac:`FAC_PARAM_TABLE_FILENAME`     |
+                           +-------------------------------------+
|                           | :fac:`FAC_STARTUP_TIMEOUT_MSEC`     |
+                           +-------------------------------------+
|                           | :fac:`FAC_CUSTOM_DEV_PATH`          |
+                           +-------------------------------------+
|                           | :fac:`FAC_MAX_MOTOR_OUTPUTS`        |
+---------------------------+-------------------------------------+

.. note::
   The message IDs defined in the :fac:`fac_msgids.h` file must have values
   that are unique system wide.  Most applications also have message IDs.
   No two message IDs may have the same value.
   
TODO: Describe the "custom" layer code.


Table(s)
^^^^^^^^
TODO: Tailor this section to be more FAC specific. Needs Doxyfication.


Tables are created at build time, though they can be changed at run time. Each 
output channel can, and usually does, have its own unique configuration table.
TO actually has 3 tables per channel.  The first two tables define the same 
configurable parameters, but one of these is the backup table that is compiled
directly into the application binary and only loaded if the nominal table fails
to load from the file system.  Both the nominal and the backup tables have the 
same structure.  

TODO: The following do not exist in Doxy.

+-------------------------------+------------------------------------+----------------------------------------+
| Table Name                    | Default file name                  | Parameter                              |
+===============================+====================================+========================================+
| :fac:`FAC_PARAM_TABLENAME`    | :fac:`FAC_PARAM_TABLE_FILENAME`    | :fac:`FAC_ParamTbl_t::MC_ROLL_P`       |
+                               |                                    +----------------------------------------+
|                               |                                    | :fac:`FAC_ParamTbl_t::MC_ROLLRATE_P`   |
+                               |                                    +----------------------------------------+
|                               |                                    | :fac:`FAC_ParamTbl_t::MC_ROLLRATE_I`   |
+                               |                                    +----------------------------------------+
|                               |                                    | :fac:`FAC_ParamTbl_t::MC_RR_INT_LIM`   |
+                               |                                    +----------------------------------------+
|                               |                                    | :fac:`FAC_ParamTbl_t::MC_ROLLRATE_D`   |
+                               |                                    +----------------------------------------+
|                               |                                    | :fac:`FAC_ParamTbl_t::MC_ROLLRATE_FF`  |
+                               |                                    +----------------------------------------+
|                               |                                    | :fac:`FAC_ParamTbl_t::MC_PITCH_P`      |
+                               |                                    +----------------------------------------+
|                               |                                    | :fac:`FAC_ParamTbl_t::MC_PITCHRATE_P`  |
+                               |                                    +----------------------------------------+
|                               |                                    | :fac:`FAC_ParamTbl_t::MC_PITCHRATE_I`  |
+                               |                                    +----------------------------------------+
|                               |                                    | :fac:`FAC_ParamTbl_t::MC_PR_INT_LIM`   |
+                               |                                    +----------------------------------------+
|                               |                                    | :fac:`FAC_ParamTbl_t::MC_PITCHRATE_D`  |
+                               |                                    +----------------------------------------+
|                               |                                    | :fac:`FAC_ParamTbl_t::MC_PITCHRATE_FF` |
+                               |                                    +----------------------------------------+
|                               |                                    | :fac:`FAC_ParamTbl_t::MC_TPA_BREAK_P`  |
+                               |                                    +----------------------------------------+
|                               |                                    | :fac:`FAC_ParamTbl_t::MC_TPA_BREAK_I`  |
+                               |                                    +----------------------------------------+
|                               |                                    | :fac:`FAC_ParamTbl_t::MC_TPA_BREAK_D`  |
+                               |                                    +----------------------------------------+
|                               |                                    | :fac:`FAC_ParamTbl_t::MC_TPA_RATE_P`   |
+                               |                                    +----------------------------------------+
|                               |                                    | :fac:`FAC_ParamTbl_t::MC_ROLLRATE_D`   |
+                               |                                    +----------------------------------------+
|                               |                                    | :fac:`FAC_ParamTbl_t::MC_TPA_RATE_I`   |
+                               |                                    +----------------------------------------+
|                               |                                    | :fac:`FAC_ParamTbl_t::MC_TPA_RATE_D`   |
+                               |                                    +----------------------------------------+
|                               |                                    | :fac:`FAC_ParamTbl_t::MC_YAW_P`        |
+                               |                                    +----------------------------------------+
|                               |                                    | :fac:`FAC_ParamTbl_t::MC_YAWRATE_P`    |
+                               |                                    +----------------------------------------+
|                               |                                    | :fac:`FAC_ParamTbl_t::MC_YAWRATE_I`    |
+                               |                                    +----------------------------------------+
|                               |                                    | :fac:`FAC_ParamTbl_t::MC_YAWRATE_D`    |
+                               |                                    +----------------------------------------+
|                               |                                    | :fac:`FAC_ParamTbl_t::MC_YAWRATE_FF`   |
+                               |                                    +----------------------------------------+
|                               |                                    | :fac:`FAC_ParamTbl_t::MC_ROLLRATE_MAX` |
+                               |                                    +----------------------------------------+
|                               |                                    | :fac:`FAC_ParamTbl_t::MC_PITCHRATE_MAX`|
+                               |                                    +----------------------------------------+
|                               |                                    | :fac:`FAC_ParamTbl_t::MC_YAWRATE_MAX`  |
+                               |                                    +----------------------------------------+
|                               |                                    | :fac:`FAC_ParamTbl_t::MC_YAWRAUTO_MAX` |
+                               |                                    +----------------------------------------+
|                               |                                    | :fac:`FAC_ParamTbl_t::MC_ACRO_R_MAX`   |
+                               |                                    +----------------------------------------+
|                               |                                    | :fac:`FAC_ParamTbl_t::MC_ACRO_P_MAX`   |
+                               |                                    +----------------------------------------+
|                               |                                    | :fac:`FAC_ParamTbl_t::MC_ACRO_Y_MAX`   |
+                               |                                    +----------------------------------------+
|                               |                                    | :fac:`FAC_ParamTbl_t::MC_RATT_TH`      |
+                               |                                    +----------------------------------------+
|                               |                                    | :fac:`FAC_ParamTbl_t::MC_ROLL_TC`      |
+                               |                                    +----------------------------------------+
|                               |                                    | :fac:`FAC_ParamTbl_t::MC_PITCH_TC`     |
+                               |                                    +----------------------------------------+
|                               |                                    | :fac:`FAC_ParamTbl_t::MC_BAT_SCALE_EN` |
+                               |                                    +----------------------------------------+
|                               |                                    | :fac:`FAC_ParamTbl_t::SENS_BOARD_ROT`  |
+                               |                                    +----------------------------------------+
|                               |                                    | :fac:`FAC_ParamTbl_t::SENS_BOARD_X_OFF`|
+                               |                                    +----------------------------------------+
|                               |                                    | :fac:`FAC_ParamTbl_t::SENS_BOARD_Y_OFF`|
+                               |                                    +----------------------------------------+
|                               |                                    | :fac:`FAC_ParamTbl_t::SENS_BOARD_Z_OFF`|
+-------------------------------+------------------------------------+----------------------------------------+

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

.. literalinclude:: ../fsw/tables/fac_config.c
  :language: C
  :caption: FAC default config table




System Integration
^^^^^^^^^^^^^^^^^^




Run Time Configuration
^^^^^^^^^^^^^^^^^^^^^^


TODO: Add references.
