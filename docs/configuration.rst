Configuration
=============

Mission Specific
^^^^^^^^^^^^^^^^

+-------------------------+--------------------------------+
| File                    | Parameter                      |
+=========================+================================+
| :to:`to_mission_cfg.h`  | :to:`TO_MAX_MESSAGE_FLOWS`     |
|                         +--------------------------------+
|                         | :to:`TO_MAX_PRIORITY_QUEUES`   |
+                         +--------------------------------+
|                         | :to:`TO_MAX_QUEUE_SIZE_LIMIT`  |
+                         +--------------------------------+
|                         | :to:`TO_MAX_PQ_MSG_SIZE_LIMIT` |
+-------------------------+--------------------------------+
| :to:`to_perfids.h`      | :to:`TO_MAIN_TASK_PERF_ID`     |
+                         +--------------------------------+
|                         | :to:`TO_SOCKET_SEND_PERF_ID`   |
+-------------------------+--------------------------------+

.. note::
   The performance IDs defined in the :to:`to_perfids.h` file must have values
   that are unique system wide.  Most applications also have performance IDs.
   No two performance IDs may have the same value.
   

Platform Specific
^^^^^^^^^^^^^^^^^

+-------------------------+-------------------------------------+
| File                    | Parameter                           |
+=========================+=====================================+
| :to:`to_msgids.h`       | :to:`TO_CMD_MID`                    |
|                         +-------------------------------------+
|                         | :to:`TO_SEND_HK_MID`                |
+                         +-------------------------------------+
|                         | :to:`TO_SEND_TLM_MID`               |
+                         +-------------------------------------+
|                         | :to:`TO_HK_TLM_MID`                 |
+                         +-------------------------------------+
|                         | :to:`TO_DATA_TYPE_MID`              |
+                         +-------------------------------------+
|                         | :to:`TO_DIAG_TLM_MID`               |
+                         +-------------------------------------+
|                         | :to:`TO_DIAG_MSG_FLOW_MID`          |
+-------------------------+-------------------------------------+
| :to:`to_platform_cfg.h` | :to:`TO_CUSTOM_CHILD_TASK_FLAGS`    |
+                         +-------------------------------------+
|                         | :to:`TO_CUSTOM_TASK_STACK_SIZE`     |
+                         +-------------------------------------+
|                         | :to:`TO_UDP_CHANNEL_ADDRESS`        |
+                         +-------------------------------------+
|                         | :to:`TO_UDP_CHANNEL_PORT`           |
+                         +-------------------------------------+
|                         | :to:`TO_MSG_FLOW_PKT_LIMIT`         |
+                         +-------------------------------------+
|                         | :to:`TO_MAX_MSG_LENGTH`             |
+                         +-------------------------------------+
|                         | :to:`TO_UDP_CF_THROTTLE_SEM_NAME`   |
+                         +-------------------------------------+
|                         | :to:`TO_UDP_CF_MAX_PDUS`            |
+                         +-------------------------------------+
|                         | :to:`TO_MISSION_REV`                |
+                         +-------------------------------------+
|                         | :to:`TO_SCH_PIPE_DEPTH`             |
+                         +-------------------------------------+
|                         | :to:`TO_SCH_PIPE_NAME`              |
+                         +-------------------------------------+
|                         | :to:`TO_SCH_PIPE_PEND_TIME`         |
+                         +-------------------------------------+
|                         | :to:`TO_SCH_PIPE_WAKEUP_RESERVED`   |
+                         +-------------------------------------+
|                         | :to:`TO_SCH_PIPE_SEND_HK_RESERVED`  |
+                         +-------------------------------------+
|                         | :to:`TO_CMD_PIPE_DEPTH`             |
+                         +-------------------------------------+
|                         | :to:`TO_CMD_PIPE_NAME`              |
+                         +-------------------------------------+
|                         | :to:`TO_DATA_PIPE_DEPTH`            |
+                         +-------------------------------------+
|                         | :to:`TO_UDP_CONFIG_TABLE_FILENAME`  |
+                         +-------------------------------------+
|                         | :to:`TO_MAX_MEMPOOL_BLK_SIZES`      |
+                         +-------------------------------------+
|                         | :to:`TO_MEM_BLOCK_SIZE_01`          |
+                         +-------------------------------------+
|                         | :to:`TO_MEM_BLOCK_SIZE_02`          |
+                         +-------------------------------------+
|                         | :to:`TO_MEM_BLOCK_SIZE_03`          |
+                         +-------------------------------------+
|                         | :to:`TO_MEM_BLOCK_SIZE_04`          |
+                         +-------------------------------------+
|                         | :to:`TO_MEM_BLOCK_SIZE_05`          |
+                         +-------------------------------------+
|                         | :to:`TO_MEM_BLOCK_SIZE_06`          |
+                         +-------------------------------------+
|                         | :to:`TO_MEM_BLOCK_SIZE_07`          |
+                         +-------------------------------------+
|                         | :to:`TO_MAX_BLOCK_SIZE`             |
+                         +-------------------------------------+
|                         | :to:`TO_UDP_CONFIG_TABLENAME`       |
+                         +-------------------------------------+
|                         | :to:`TO_UDP_DUMP_TABLENAME`         |
+                         +-------------------------------------+
|                         | :to:`TO_STARTUP_TIMEOUT_MSEC`       |
+                         +-------------------------------------+
|                         | :to:`TO_MAX_MSGS_OUT_PER_FRAME`     |
+                         +-------------------------------------+
|                         | :to:`TO_MAX_CHANNELS`               |
+                         +-------------------------------------+
|                         | :to:`TO_OUTPUT_QUEUE_DEPTH`         |
+                         +-------------------------------------+
|                         | :to:`TO_UDP_CHANNEL_TASK_PRIORITY`  |
+                         +-------------------------------------+
|                         | :to:`TO_NUM_BYTES_IN_MEM_POOL`      |
+-------------------------+-------------------------------------+

.. note::
   The message IDs defined in the :to:`to_msgids.h` file must have values
   that are unique system wide.  Most applications also have message IDs.
   No two message IDs may have the same value.
   
TODO: Describe the "custom" layer code.


Table(s)
^^^^^^^^

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

+-------------------------------+------------------------------------+------------------------------------------+
| Table Name                    | Default file name                  | Parameter                                |
+===============================+====================================+==========================================+
| :to:`TO_UDP_CONFIG_TABLENAME` | :to:`TO_UDP_CONFIG_TABLE_FILENAME` | :to:`TO_ChannelTbl_t::TableID`           |
+                               |                                    +------------------------------------------+
|                               |                                    | :to:`TO_ChannelTbl_t::MessageFlow`       |
+                               |                                    +------------------------------------------+
|                               |                                    | :to:`TO_ChannelTbl_t::PriorityQueue`     |
+-------------------------------+------------------------------------+------------------------------------------+
| :to:`TO_UDP_DUMP_TABLENAME`   | N/A                                | :to:`TO_ChannelDumpTbl_t::TableID`       |
+                               |                                    +------------------------------------------+
|                               |                                    | :to:`TO_ChannelDumpTbl_t::MessageFlow`   |
+                               |                                    +------------------------------------------+
|                               |                                    | :to:`TO_ChannelDumpTbl_t::PriorityQueue` |
+                               |                                    +------------------------------------------+
|                               |                                    | :to:`TO_ChannelDumpTbl_t::OutputQueue`   |
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

.. literalinclude:: ../fsw/tables/to_udp_cfg.c
  :language: C
  :caption: Nominal Configuration Table

.. literalinclude:: ../fsw/tables/to_backup_cfg.c
  :language: C
  :caption: Backup Configuration Table


System Integration
^^^^^^^^^^^^^^^^^^

TODO: Describe the channelization configuration with the CF application.

TODO: Describe adding tables to Checksum Services.

TODO: Describe adding application to Health Services.

TODO: Describe possible integration with Limit Checker.


Run Time Configuration
^^^^^^^^^^^^^^^^^^^^^^

This application supports loading and reloading of table configuration at run time.  Message flows can
be added and removed at run time. 

TODO: Add references.
