Configuration
=============

TODO

Mission Specific
^^^^^^^^^^^^^^^^

TODO: Add Doxygen documentation for bat_mission_cfg.h

+----------------------------+-------------------------------------+
| File                       | Parameter                           |
+============================+=====================================+
| :bat:`bat_mission_cfg.h`   | :bat:`BAT_SB_TIMEOUT`               |
+----------------------------+-------------------------------------+
| :bat:`bat_perfids.h`       | :bat:`BAT_MAIN_TASK_PERF_ID`        |
+----------------------------+-------------------------------------+

.. note::
   The performance IDs defined in the :bat:`bat_perfids.h` file must have values
   that are unique system wide.  Most applications also have performance IDs.
   No two performance IDs may have the same value.
   

Platform Specific
^^^^^^^^^^^^^^^^^

+-----------------------------+---------------------------------------------+
| File                        | Parameter                                   |
+=============================+=============================================+
| :bat:`bat_msgids.h`         | :bat:`BAT_HK_TLM_MID`                       |
+                             +---------------------------------------------+
|                             | :bat:`BAT_SEND_HK_MID`                      |
+                             +---------------------------------------------+
|                             | :bat:`BAT_WAKEUP_MID`                       |
+                             +---------------------------------------------+
|                             | :bat:`BAT_CMD_MID`                          |
+-----------------------------+---------------------------------------------+
| :bat:`bat_platform_cfg.h`   | :bat:`BAT_LISTENER_TASK_FLAGS`              |
+                             +---------------------------------------------+
|                             | :bat:`BAT_MISSION_REV`                      |
+                             +---------------------------------------------+
|                             | :bat:`BAT_SCH_PIPE_DEPTH`                   |
+                             +---------------------------------------------+
|                             | :bat:`BAT_SCH_PIPE_NAME`                    |
+                             +---------------------------------------------+
|                             | :bat:`BAT_SCH_PIPE_PEND_TIME`               |
+                             +---------------------------------------------+
|                             | :bat:`BAT_WAKEUP_MID_MAX_MSG_COUNT`         |
+                             +---------------------------------------------+
|                             | :bat:`BAT_SEND_HK_MID_MAX_MSG_COUNT`        |
+                             +---------------------------------------------+
|                             | :bat:`BAT_CMD_PIPE_DEPTH`                   |
+                             +---------------------------------------------+
|                             | :bat:`BAT_CMD_PIPE_NAME`                    |
+                             +---------------------------------------------+
|                             | :bat:`BAT_CONFIG_TABLE_FILENAME`            |
+                             +---------------------------------------------+
|                             | :bat:`BAT_STARTUP_TIMEOUT_MSEC`             |
+                             +---------------------------------------------+
|                             | :bat:`BAT_LISTENER_TASK_STACK_SIZE`         |
+-----------------------------+---------------------------------------------+

.. note::
   The message IDs defined in the :bat:`bat_msgids.h` file must have values
   that are unique system wide.  Most applications also have message IDs.
   No two message IDs may have the same value.

Table(s)
^^^^^^^^^^^^^^^^

+-------------------------------+------------------------------------+--------------------------------------------+
| Table Name                    | Default file name                  | Parameter                                  |
+===============================+====================================+============================================+
| :bat:`BAT_CONFIG_TABLENAME`   | :bat:`BAT_CONFIG_TABLE_FILENAME`   | :bat:`BAT_ConfigTbl_t::VEmpty`             |
+                               |                                    +--------------------------------------------+
|                               |                                    | :bat:`BAT_ConfigTbl_t::VFull`              |
+                               |                                    +--------------------------------------------+
|                               |                                    | :bat:`BAT_ConfigTbl_t::NumCells`           |
+                               |                                    +--------------------------------------------+
|                               |                                    | :bat:`BAT_ConfigTbl_t::Capacity`           |
+                               |                                    +--------------------------------------------+
|                               |                                    | :bat:`BAT_ConfigTbl_t::VLoadDrop`          |
+                               |                                    +--------------------------------------------+
|                               |                                    | :bat:`BAT_ConfigTbl_t::RInternal`          |
+                               |                                    +--------------------------------------------+
|                               |                                    | :bat:`BAT_ConfigTbl_t::LowThreshold`       |
+                               |                                    +--------------------------------------------+
|                               |                                    | :bat:`BAT_ConfigTbl_t::CriticalThreshold`  |
+                               |                                    +--------------------------------------------+
|                               |                                    | :bat:`BAT_ConfigTbl_t::EmergencyThreshold` |
+                               |                                    +--------------------------------------------+
|                               |                                    | :bat:`BAT_ConfigTbl_t::VoltageScale`       |
+                               |                                    +--------------------------------------------+
|                               |                                    | :bat:`BAT_ConfigTbl_t::CurrentScale`       |
+-------------------------------+------------------------------------+--------------------------------------------+

TODO: Add Doxygen documentation for bat_config.c

Below is an example table.

.. literalinclude:: ../fsw/tables/bat_config.c
  :language: C
  :caption: Nominal Configuration Table


System Integration
^^^^^^^^^^^^^^^^^^

TODO: Describe System Integration instructions specific to BAT.


Run Time Configuration
^^^^^^^^^^^^^^^^^^^^^^

TODO: Describe Run Time Configuration for BAT.
