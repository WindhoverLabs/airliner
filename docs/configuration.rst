Configuration
=============

Mission Specific
^^^^^^^^^^^^^^^^

+-------------------------+--------------------------------+
| File                    | Parameter                      |
+=========================+================================+
| :lgc:`lgc_mission_cfg.h`| :lgc:`LGC_SB_TIMEOUT`          |
+-------------------------+--------------------------------+
| :lgc:`lgc_perfids.h`    | :lgc:`LGC_MAIN_TASK_PERF_ID`   |
+-------------------------+--------------------------------+


.. note::
   The performance IDs defined in the :lgc:`lgc_perfids.h` file must have values
   that are unique system wide.  Most applications also have performance IDs.
   No two performance IDs may have the same value.
   

Platform Specific
^^^^^^^^^^^^^^^^^

+---------------------------+-------------------------------------+
| File                      | Parameter                           |
+===========================+=====================================+
| :lgc:`lgc_msgids.h`       | :lgc:`LGC_HK_TLM_MID`               |
|                           +-------------------------------------+
|                           | :lgc:`LGC_SEND_HK_MID`              |
+                           +-------------------------------------+
|                           | :lgc:`LGC_WAKEUP_MID`               |
+                           +-------------------------------------+
|                           | :lgc:`LGC_CMD_MID`                  |
+---------------------------+-------------------------------------+
| :lgc:`lgc_platform_cfg.h` | :lgc:`LGC_MISSION_REV`              |
+                           +-------------------------------------+
|                           | :lgc:`LGC_SCH_PIPE_DEPTH`           |
+                           +-------------------------------------+
|                           | :lgc:`LGC_SCH_PIPE_NAME`            |
+                           +-------------------------------------+
|                           | :lgc:`LGC_SCH_PIPE_PEND_TIME`       |
+                           +-------------------------------------+
|                           | :lgc:`LGC_WAKEUP_MID_MAX_MSG_COUNT` |
+                           +-------------------------------------+
|                           | :lgc:`LGC_SEND_HK_MID_MAX_MSG_COUNT`|
+                           +-------------------------------------+
|                           | :lgc:`LGC_CMD_PIPE_DEPTH`           |
+                           +-------------------------------------+
|                           | :lgc:`LGC_CMD_PIPE_NAME`            |
+                           +-------------------------------------+
|                           | :lgc:`LGC_DATA_PIPE_DEPTH`          |
+                           +-------------------------------------+
|                           | :lgc:`LGC_DATA_PIPE_NAME`           |
+                           +-------------------------------------+
|                           | :lgc:`LGC_CONFIG_TABLE_FILENAME`    |
+                           +-------------------------------------+
|                           | :lgc:`LGC_STARTUP_TIMEOUT_MSEC`     |
+---------------------------+-------------------------------------+

.. note::
   The message IDs defined in the :lgc:`lgc_msgids.h` file must have values
   that are unique system wide.  Most applications also have message IDs.
   No two message IDs may have the same value.
   
TODO: Describe the "custom" layer code.


Table(s)
^^^^^^^^
TODO: Tailor this section to be more LGC specific. Needs Doxyfication.


Tables are created at build time, though they can be changed at run time. Each 
output channel can, and usually does, have its own unique configuration table.
TO actually has 3 tables per channel.  The first two tables define the same 
configurable parameters, but one of these is the backup table that is compiled
directly into the application binary and only loaded if the nominal table fails
to load from the file system.  Both the nominal and the backup tables have the 
same structure.  Both are instantiations of the :lgc:`LGC_ChannelTbl_t` 
structure.  The third table is a :term:`dump table` that contains performance 
metrics, not configuration.  This table is defined by the 
:lgc:`LGC_ChannelDumpTbl_t` structure.



+-------------------------------+------------------------------------+--------------------------------------+
| Table Name                    | Default file name                  | Parameter                            |
+===============================+====================================+======================================+
| :lgc:`LGC_CONFIG_TABLENAME`   | :lgc:`LGC_CONFIG_TABLENAME`        | :lgc:`LGC_ConfigTbl_t::PwmDisarmed`  |
+-------------------------------+------------------------------------+--------------------------------------+

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

.. literalinclude:: ../fsw/tables/lgc_config.c
  :language: C
  :caption: LGC default config table




System Integration
^^^^^^^^^^^^^^^^^^




Run Time Configuration
^^^^^^^^^^^^^^^^^^^^^^


TODO: Add references.
