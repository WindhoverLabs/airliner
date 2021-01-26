Configuration
=============

Mission Specific
^^^^^^^^^^^^^^^^
TODO: The following do not exist in doxy.

+-------------------------+--------------------------------+
| File                    | Parameter                      |
+=========================+================================+
| :mac:`mac_mission_cfg.h`| :mac:`MAC_SB_TIMEOUT`          |
+-------------------------+--------------------------------+
| :mac:`mac_perfids.h`    | :mac:`MAC_MAIN_TASK_PERF_ID`   |
+-------------------------+--------------------------------+


.. note::
   The performance IDs defined in the :mac:`mac_perfids.h` file must have values
   that are unique system wide.  Most applications also have performance IDs.
   No two performance IDs may have the same value.
   

Platform Specific
^^^^^^^^^^^^^^^^^

+---------------------------+-------------------------------------+
| File                      | Parameter                           |
+===========================+=====================================+
| :mac:`mac_msgids.h`       | :mac:`MAC_CMD_MID`                  |
|                           +-------------------------------------+
|                           | :mac:`MAC_SEND_HK_MID`              |
+                           +-------------------------------------+
|                           | :mac:`MAC_RUN_CONTROLLER_MID`       |
+                           +-------------------------------------+
|                           | :mac:`MAC_OUT_DATA_MID`             |
+                           +-------------------------------------+
|                           | :mac:`MAC_HK_TLM_MID`               |
+---------------------------+-------------------------------------+
| :mac:`mac_platform_cfg.h` | :mac:`MAC_MISSION_REV`              |
+                           +-------------------------------------+
|                           | :mac:`MAC_SCH_PIPE_DEPTH`           |
+                           +-------------------------------------+
|                           | :mac:`MAC_SCH_PIPE_NAME`            |
+                           +-------------------------------------+
|                           | :mac:`MAC_SCH_PIPE_PEND_TIME`       |
+                           +-------------------------------------+
|                           | :mac:`MAC_SCH_PIPE_WAKEUP_RESERVED` |
+                           +-------------------------------------+
|                           | :mac:`MAC_SCH_PIPE_SEND_HK_RESERVED`|
+                           +-------------------------------------+
|                           | :mac:`MAC_CMD_PIPE_DEPTH`           |
+                           +-------------------------------------+
|                           | :mac:`MAC_CMD_PIPE_NAME`            |
+                           +-------------------------------------+
|                           | :mac:`MAC_PARAM_TABLE_FILENAME`     |
+                           +-------------------------------------+
|                           | :mac:`MAC_STARTUP_TIMEOUT_MSEC`     |
+                           +-------------------------------------+
|                           | :mac:`MAC_CUSTOM_DEV_PATH`          |
+                           +-------------------------------------+
|                           | :mac:`MAC_MAX_MOTOR_OUTPUTS`        |
+---------------------------+-------------------------------------+

.. note::
   The message IDs defined in the :mac:`mac_msgids.h` file must have values
   that are unique system wide.  Most applications also have message IDs.
   No two message IDs may have the same value.
   
TODO: Describe the "custom" layer code.


Table(s)
^^^^^^^^
TODO: Tailor this section to be more MAC specific. Needs Doxyfication.


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
| :mac:`MAC_PARAM_TABLENAME`    | :mac:`MAC_PARAM_TABLE_FILENAME`    | :mac:`MAC_ParamTbl_t::MC_ROLL_P`       |
+                               |                                    +----------------------------------------+
|                               |                                    | :mac:`MAC_ParamTbl_t::MC_ROLLRATE_P`   |
+                               |                                    +----------------------------------------+
|                               |                                    | :mac:`MAC_ParamTbl_t::MC_ROLLRATE_I`   |
+                               |                                    +----------------------------------------+
|                               |                                    | :mac:`MAC_ParamTbl_t::MC_RR_INT_LIM`   |
+                               |                                    +----------------------------------------+
|                               |                                    | :mac:`MAC_ParamTbl_t::MC_ROLLRATE_D`   |
+                               |                                    +----------------------------------------+
|                               |                                    | :mac:`MAC_ParamTbl_t::MC_ROLLRATE_FF`  |
+                               |                                    +----------------------------------------+
|                               |                                    | :mac:`MAC_ParamTbl_t::MC_PITCH_P`      |
+                               |                                    +----------------------------------------+
|                               |                                    | :mac:`MAC_ParamTbl_t::MC_PITCHRATE_P`  |
+                               |                                    +----------------------------------------+
|                               |                                    | :mac:`MAC_ParamTbl_t::MC_PITCHRATE_I`  |
+                               |                                    +----------------------------------------+
|                               |                                    | :mac:`MAC_ParamTbl_t::MC_PR_INT_LIM`   |
+                               |                                    +----------------------------------------+
|                               |                                    | :mac:`MAC_ParamTbl_t::MC_PITCHRATE_D`  |
+                               |                                    +----------------------------------------+
|                               |                                    | :mac:`MAC_ParamTbl_t::MC_PITCHRATE_FF` |
+                               |                                    +----------------------------------------+
|                               |                                    | :mac:`MAC_ParamTbl_t::MC_TPA_BREAK_P`  |
+                               |                                    +----------------------------------------+
|                               |                                    | :mac:`MAC_ParamTbl_t::MC_TPA_BREAK_I`  |
+                               |                                    +----------------------------------------+
|                               |                                    | :mac:`MAC_ParamTbl_t::MC_TPA_BREAK_D`  |
+                               |                                    +----------------------------------------+
|                               |                                    | :mac:`MAC_ParamTbl_t::MC_TPA_RATE_P`   |
+                               |                                    +----------------------------------------+
|                               |                                    | :mac:`MAC_ParamTbl_t::MC_ROLLRATE_D`   |
+                               |                                    +----------------------------------------+
|                               |                                    | :mac:`MAC_ParamTbl_t::MC_TPA_RATE_I`   |
+                               |                                    +----------------------------------------+
|                               |                                    | :mac:`MAC_ParamTbl_t::MC_TPA_RATE_D`   |
+                               |                                    +----------------------------------------+
|                               |                                    | :mac:`MAC_ParamTbl_t::MC_YAW_P`        |
+                               |                                    +----------------------------------------+
|                               |                                    | :mac:`MAC_ParamTbl_t::MC_YAWRATE_P`    |
+                               |                                    +----------------------------------------+
|                               |                                    | :mac:`MAC_ParamTbl_t::MC_YAWRATE_I`    |
+                               |                                    +----------------------------------------+
|                               |                                    | :mac:`MAC_ParamTbl_t::MC_YAWRATE_D`    |
+                               |                                    +----------------------------------------+
|                               |                                    | :mac:`MAC_ParamTbl_t::MC_YAWRATE_FF`   |
+                               |                                    +----------------------------------------+
|                               |                                    | :mac:`MAC_ParamTbl_t::MC_ROLLRATE_MAX` |
+                               |                                    +----------------------------------------+
|                               |                                    | :mac:`MAC_ParamTbl_t::MC_PITCHRATE_MAX`|
+                               |                                    +----------------------------------------+
|                               |                                    | :mac:`MAC_ParamTbl_t::MC_YAWRATE_MAX`  |
+                               |                                    +----------------------------------------+
|                               |                                    | :mac:`MAC_ParamTbl_t::MC_YAWRAUTO_MAX` |
+                               |                                    +----------------------------------------+
|                               |                                    | :mac:`MAC_ParamTbl_t::MC_ACRO_R_MAX`   |
+                               |                                    +----------------------------------------+
|                               |                                    | :mac:`MAC_ParamTbl_t::MC_ACRO_P_MAX`   |
+                               |                                    +----------------------------------------+
|                               |                                    | :mac:`MAC_ParamTbl_t::MC_ACRO_Y_MAX`   |
+                               |                                    +----------------------------------------+
|                               |                                    | :mac:`MAC_ParamTbl_t::MC_RATT_TH`      |
+                               |                                    +----------------------------------------+
|                               |                                    | :mac:`MAC_ParamTbl_t::MC_ROLL_TC`      |
+                               |                                    +----------------------------------------+
|                               |                                    | :mac:`MAC_ParamTbl_t::MC_PITCH_TC`     |
+                               |                                    +----------------------------------------+
|                               |                                    | :mac:`MAC_ParamTbl_t::MC_BAT_SCALE_EN` |
+                               |                                    +----------------------------------------+
|                               |                                    | :mac:`MAC_ParamTbl_t::SENS_BOARD_ROT`  |
+                               |                                    +----------------------------------------+
|                               |                                    | :mac:`MAC_ParamTbl_t::SENS_BOARD_X_OFF`|
+                               |                                    +----------------------------------------+
|                               |                                    | :mac:`MAC_ParamTbl_t::SENS_BOARD_Y_OFF`|
+                               |                                    +----------------------------------------+
|                               |                                    | :mac:`MAC_ParamTbl_t::SENS_BOARD_Z_OFF`|
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

.. literalinclude:: ../fsw/tables/mac_config.c
  :language: C
  :caption: MAC default config table




System Integration
^^^^^^^^^^^^^^^^^^




Run Time Configuration
^^^^^^^^^^^^^^^^^^^^^^


TODO: Add references.
