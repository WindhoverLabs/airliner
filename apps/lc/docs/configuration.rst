Configuration
=============

Mission Specific
^^^^^^^^^^^^^^^^

+-------------------------+--------------------------------+
| File                    | Parameter                      |
+=========================+================================+
| :lc:`lc_mission_cfg.h`  | :lc:`LC_RTS_REQ_MID`           |
+                         +--------------------------------+
|                         | :lc:`LC_RTS_REQ_CC`            |
+-------------------------+--------------------------------+
| :lc:`lc_perfids.h`      | :lc:`LC_APPMAIN_PERF_ID`       |
+                         +--------------------------------+
|                         | :lc:`LC_WDT_SEARCH_PERF_ID`    |
+-------------------------+--------------------------------+


.. note::
   The performance IDs defined in the :lc:`lc_perfids.h` file must have values
   that are unique system wide.  Most applications also have performance IDs.
   No two performance IDs may have the same value.
   

Platform Specific
^^^^^^^^^^^^^^^^^

+-------------------------+-------------------------------------+
| File                    | Parameter                           |
+=========================+=====================================+
| :lc:`lc_msgids.h`       | :lc:`LC_CMD_MID`                    |
|                         +-------------------------------------+
|                         | :lc:`LC_SEND_HK_MID`                |
+                         +-------------------------------------+
|                         | :lc:`LC_SAMPLE_AP_MID`              |
+                         +-------------------------------------+
|                         | :lc:`LC_HK_TLM_MID`                 |
+                         +-------------------------------------+
|                         | :lc:`LC_ALL_ACTIONPOINTS`           |
+                         +-------------------------------------+
|                         | :lc:`LC_ALL_WATCHPOINTS`            |
+-------------------------+-------------------------------------+
| :lc:`lc_platform_cfg.h` | :lc:`LC_APP_NAME`                   |
+                         +-------------------------------------+
|                         | :lc:`LC_PIPE_DEPTH`                 |
+                         +-------------------------------------+
|                         | :lc:`LC_MAX_WATCHPOINTS`            |
+                         +-------------------------------------+
|                         | :lc:`LC_MAX_ACTIONPOINTS`           |
+                         +-------------------------------------+
|                         | :lc:`LC_STATE_POWER_ON_RESET`       |
+                         +-------------------------------------+
|                         | :lc:`LC_STATE_WHEN_CDS_RESTORED`    |
+                         +-------------------------------------+
|                         | :lc:`LC_WDT_FILENAME`               |
+                         +-------------------------------------+
|                         | :lc:`LC_ADT_FILENAME`               |
+                         +-------------------------------------+
|                         | :lc:`LC_MAX_RPN_EQU_SIZE`           |
+                         +-------------------------------------+
|                         | :lc:`LC_MAX_ACTION_TEXT`            |
+                         +-------------------------------------+
|                         | :lc:`LC_MAX_VALID_ADT_RTSID`        |
+                         +-------------------------------------+
|                         | :lc:`LC_FLOAT_TOLERANCE`            |
+                         +-------------------------------------+
|                         | :lc:`LC_MISSION_REV`                |
+-------------------------+-------------------------------------+

.. note::
   The message IDs defined in the :lc:`lc_msgids.h` file must have values
   that are unique system wide.  Most applications also have message IDs.
   No two message IDs may have the same value.
   
TODO: Describe the "custom" layer code.


Table(s)
^^^^^^^^
TODO: Tailor this section to be more LC specific. Needs Doxyfication.


Tables are created at build time, though they can be changed at run time. Each 
output channel can, and usually does, have its own unique configuration table.
TO actually has 3 tables per channel.  The first two tables define the same 
configurable parameters, but one of these is the backup table that is compiled
directly into the application binary and only loaded if the nominal table fails
to load from the file system.  Both the nominal and the backup tables have the 
same structure.  Both are instantiations of the :to:`TO_ChannelTbl_t` 
structure.  The third table is a :term:`dump table` that contains performance 
metrics, not configuration.  This table is defined by the 
:to:`TO_ChannelDumpTbl_t` structure.


TODO: Are parameters required for LC_WRTTransition_t, LC_WRTEntry_t, and LC_ARTEntry_t? found in /airliner/apps/lc/fsw/src/lc_tbl.h?

+-------------------------------+------------------------------------+------------------------------------------+
| Table Name                    | Default file name                  | Parameter                                |
+===============================+====================================+==========================================+
| :lc:`LC_ADT_TABLENAME`        | :lc:`LC_ADT_FILENAME`              | :lc:`LC_ADTEntry_t::DefaultState`        |
+                               |                                    +------------------------------------------+
|                               |                                    | :lc:`LC_ADTEntry_t::MaxPassiveEvents`    |
+                               |                                    +------------------------------------------+
|                               |                                    | :lc:`LC_ADTEntry_t::MaxPassFailEvents`   |
+                               |                                    +------------------------------------------+
|                               |                                    | :lc:`LC_ADTEntry_t::MaxFailPassEvents`   |
+                               |                                    +------------------------------------------+
|                               |                                    | :lc:`LC_ADTEntry_t::RTSId`               |
+                               |                                    +------------------------------------------+
|                               |                                    | :lc:`LC_ADTEntry_t::MaxFailsBeforeRTS`   |
+                               |                                    +------------------------------------------+
|                               |                                    | :lc:`LC_ADTEntry_t::RPNEquation`         |
+                               |                                    +------------------------------------------+
|                               |                                    | :lc:`LC_ADTEntry_t::EventType`           |
+                               |                                    +------------------------------------------+
|                               |                                    | :lc:`LC_ADTEntry_t::EventID`             |
+                               |                                    +------------------------------------------+
|                               |                                    | :lc:`LC_ADTEntry_t::EventText`           |
+-------------------------------+------------------------------------+------------------------------------------+
| :lc:`LC_WDT_TABLENAME`        | :lc:`LC_WDT_FILENAME`              | :lc:`LC_WDTEntry_t::DataType`            |
+                               |                                    +------------------------------------------+
|                               |                                    | :lc:`LC_WDTEntry_t::OperatorID`          |
+                               |                                    +------------------------------------------+
|                               |                                    | :lc:`LC_WDTEntry_t::MessageID`           |
+                               |                                    +------------------------------------------+
|                               |                                    | :lc:`LC_WDTEntry_t::WatchpointOffset`    |
+                               |                                    +------------------------------------------+
|                               |                                    | :lc:`LC_WDTEntry_t::BitMask`             |
+                               |                                    +------------------------------------------+
|                               |                                    | :lc:`LC_WDTEntry_t::ComparisonValue`     |
+                               |                                    +------------------------------------------+
|                               |                                    | :lc:`LC_WDTEntry_t::ResultAgeWhenStale`  |
+                               |                                    +------------------------------------------+
|                               |                                    | :lc:`LC_WDTEntry_t::CustomFuncArgument`  |
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

.. literalinclude:: ../fsw/tables/lc_def_adt.c
  :language: C
  :caption: Limit Checker (LC) default actionpoint definition table (ADT)

.. literalinclude:: ../fsw/tables/lc_def_wdt.c
  :language: C
  :caption: Limit Checker (LC) default watchpoint definition table (WDT)


System Integration
^^^^^^^^^^^^^^^^^^




Run Time Configuration
^^^^^^^^^^^^^^^^^^^^^^


TODO: Add references.
