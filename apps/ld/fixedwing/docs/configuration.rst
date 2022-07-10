Configuration
=============

Mission Specific
^^^^^^^^^^^^^^^^

+-------------------------+--------------------------------+
| File                    | Parameter                      |
+=========================+================================+
| :ld:`ld_mission_cfg.h`  | :ld:`LD_SB_TIMEOUT`            |
+-------------------------+--------------------------------+
| :ld:`ld_perfids.h`      | :ld:`LD_MAIN_TASK_PERF_ID`     |
+-------------------------+--------------------------------+


.. note::
   The performance IDs defined in the :ld:`ld_perfids.h` file must have values
   that are unique system wide.  Most applications also have performance IDs.
   No two performance IDs may have the same value.
   

Platform Specific
^^^^^^^^^^^^^^^^^

+-------------------------+-------------------------------------+
| File                    | Parameter                           |
+=========================+=====================================+
| :ld:`ld_msgids.h`       | :ld:`LD_HK_TLM_MID`                 |
|                         +-------------------------------------+
|                         | :ld:`LD_SEND_HK_MID`                |
+                         +-------------------------------------+
|                         | :ld:`LD_WAKEUP_MID`                 |
+                         +-------------------------------------+
|                         | :ld:`LD_CMD_MID`                    |
+                         +-------------------------------------+
|                         | :ld:`LD_DIAG_TLM_MID`               |
+-------------------------+-------------------------------------+
| :ld:`ld_platform_cfg.h` | :ld:`LD_MISSION_REV`                |
+                         +-------------------------------------+
|                         | :ld:`LD_SCH_PIPE_DEPTH`             |
+                         +-------------------------------------+
|                         | :ld:`LD_SCH_PIPE_NAME`              |
+                         +-------------------------------------+
|                         | :ld:`LD_SCH_PIPE_PEND_TIME`         |
+                         +-------------------------------------+
|                         | :ld:`LD_WAKEUP_MID_MAX_MSG_COUNT`   |
+                         +-------------------------------------+
|                         | :ld:`LD_SEND_HK_MID_MAX_MSG_COUNT`  |
+                         +-------------------------------------+
|                         | :ld:`LD_CMD_PIPE_DEPTH`             |
+                         +-------------------------------------+
|                         | :ld:`LD_CMD_PIPE_NAME`              |
+                         +-------------------------------------+
|                         | :ld:`LD_DATA_PIPE_DEPTH`            |
+                         +-------------------------------------+
|                         | :ld:`LD_DATA_PIPE_NAME`             |
+                         +-------------------------------------+
|                         | :ld:`LD_CONFIG_TABLE_FILENAME`      |
+                         +-------------------------------------+
|                         | :ld:`LD_STARTUP_TIMEOUT_MSEC`       |
+-------------------------+-------------------------------------+

.. note::
   The message IDs defined in the :ld:`ld_msgids.h` file must have values
   that are unique system wide.  Most applications also have message IDs.
   No two message IDs may have the same value.
   
TODO: Describe the "custom" layer code.


Table(s)
^^^^^^^^
TODO: Tailor this section to be more LD specific. Needs Doxyfication.


Tables are created at build time, though they can be changed at run time. Each 
output channel can, and usually does, have its own unique configuration table.
TO actually has 3 tables per channel.  The first two tables define the same 
configurable parameters, but one of these is the backup table that is compiled
directly into the application binary and only loaded if the nominal table fails
to load from the file system.  Both the nominal and the backup tables have the 
same structure.  Both are instantiations of the :ld:`LD_ChannelTbl_t` 
structure.  The third table is a :term:`dump table` that contains performance 
metrics, not configuration.  This table is defined by the 
:ld:`LD_ChannelDumpTbl_t` structure.



+-------------------------------+------------------------------------+-------------------------------------------+
| Table Name                    | Default file name                  | Parameter                                 |
+===============================+====================================+===========================================+
| :ld:`LD_CONFIG_TABLENAME`     | :ld:`LD_CONFIG_TABLE_FILENAME`     | :ld:`LD_ConfigTbl_t::LD_Z_VEL_MAX`        |
+                               |                                    +-------------------------------------------+
|                               |                                    | :ld:`LD_ConfigTbl_t::LD_XY_VEL_MAX`       |
+                               |                                    +-------------------------------------------+
|                               |                                    | :ld:`LD_ConfigTbl_t::LD_ROT_MAX`          |
+                               |                                    +-------------------------------------------+
|                               |                                    | :ld:`LD_ConfigTbl_t::LD_FFALL_TTRI`       |
+                               |                                    +-------------------------------------------+
|                               |                                    | :ld:`LD_ConfigTbl_t::LD_MAN_DWNTHR`       |
+                               |                                    +-------------------------------------------+
|                               |                                    | :ld:`LD_ConfigTbl_t::LD_ALT_MAX`          |
+                               |                                    +-------------------------------------------+
|                               |                                    | :ld:`LD_ConfigTbl_t::LD_LOW_T_THR`        |
+                               |                                    +-------------------------------------------+
|                               |                                    | :ld:`LD_ConfigTbl_t::LD_MAN_MIN_THR`      |
+                               |                                    +-------------------------------------------+
|                               |                                    | :ld:`LD_ConfigTbl_t::LD_POS_STK_UP_THRES` |
+                               |                                    +-------------------------------------------+
|                               |                                    | :ld:`LD_ConfigTbl_t::LD_POS_STK_DW_THRES` |
+                               |                                    +-------------------------------------------+
|                               |                                    | :ld:`LD_ConfigTbl_t::LD_LANDSPEED`        |
+-------------------------------+------------------------------------+-------------------------------------------+

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

.. literalinclude:: ../fsw/tables/ld_config.c
  :language: C
  :caption: LD default config table




System Integration
^^^^^^^^^^^^^^^^^^




Run Time Configuration
^^^^^^^^^^^^^^^^^^^^^^


TODO: Add references.
