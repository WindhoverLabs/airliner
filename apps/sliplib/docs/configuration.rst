Configuration
=============

TODO

Mission Specific
^^^^^^^^^^^^^^^^

TODO: Add Doxygen documentation for sliplib_mission_cfg.h

+----------------------------+-------------------------------------+
| File                       | Parameter                           |
+============================+=====================================+
| :sliplib:`sliplib_mission_cfg.h`   | :sliplib:`SLIPLIB_SB_TIMEOUT`               |
+----------------------------+-------------------------------------+
| :sliplib:`sliplib_perfids.h`       | :sliplib:`SLIPLIB_MAIN_TASK_PERF_ID`        |
+----------------------------+-------------------------------------+

.. note::
   The performance IDs defined in the :sliplib:`sliplib_perfids.h` file must have values
   that are unique system wide.  Most applications also have performance IDs.
   No two performance IDs may have the same value.
   

Platform Specific
^^^^^^^^^^^^^^^^^

+-----------------------------+---------------------------------------------+
| File                        | Parameter                                   |
+=============================+=============================================+
| :sliplib:`sliplib_msgids.h`         | :sliplib:`SLIPLIB_CMD_MID`                          |
+                             +---------------------------------------------+
|                             | :sliplib:`SLIPLIB_SEND_HK_MID`                      |
+                             +---------------------------------------------+
|                             | :sliplib:`SLIPLIB_WAKEUP_MID`                       |
+                             +---------------------------------------------+
|                             | :sliplib:`SLIPLIB_HK_TLM_MID`                       |
+-----------------------------+---------------------------------------------+
| :sliplib:`sliplib_platform_cfg.h`   | :sliplib:`SLIPLIB_MISSION_REV`                      |
+                             +---------------------------------------------+
|                             | :sliplib:`SLIPLIB_SCH_PIPE_DEPTH`                   |
+                             +---------------------------------------------+
|                             | :sliplib:`SLIPLIB_SCH_PIPE_NAME`                    |
+                             +---------------------------------------------+
|                             | :sliplib:`SLIPLIB_SCH_PIPE_PEND_TIME`               |
+                             +---------------------------------------------+
|                             | :sliplib:`SLIPLIB_SCH_PIPE_WAKEUP_RESERVED`         |
+                             +---------------------------------------------+
|                             | :sliplib:`SLIPLIB_SCH_PIPE_SEND_HK_RESERVED`        |
+                             +---------------------------------------------+
|                             | :sliplib:`SLIPLIB_CMD_PIPE_DEPTH`                   |
+                             +---------------------------------------------+
|                             | :sliplib:`SLIPLIB_CMD_PIPE_NAME`                    |
+                             +---------------------------------------------+
|                             | :sliplib:`SLIPLIB_DATA_PIPE_DEPTH`                  |
+                             +---------------------------------------------+
|                             | :sliplib:`SLIPLIB_DATA_PIPE_NAME`                   |
+                             +---------------------------------------------+
|                             | :sliplib:`SLIPLIB_CONFIG_TABLE_FILENAME`            |
+                             +---------------------------------------------+
|                             | :sliplib:`SLIPLIB_STARTUP_TIMEOUT_MSEC`             |
+                             +---------------------------------------------+
|                             | :sliplib:`SLIPLIB_STARTUP_TIMEOUT_MSEC`             |
+                             +---------------------------------------------+
|                             | :sliplib:`SLIPLIB_MAX_REGISTRATIONS`                |
+                             +---------------------------------------------+
|                             | :sliplib:`SLIPLIB_DATA_STORE_SIZE`                  |
+-----------------------------+---------------------------------------------+

.. note::
   The message IDs defined in the :sliplib:`sliplib_msgids.h` file must have values
   that are unique system wide.  Most applications also have message IDs.
   No two message IDs may have the same value.

Table(s)
^^^^^^^^^^^^^^^^

TODO REVIEW

+-------------------------------+------------------------------------+------------------------------------------------------------------------+
| Table Name                    | Default file name                  | Parameter                                                              |
+===============================+====================================+========================================================================+
| :sliplib:`SLIPLIB_CONFIG_TABLENAME`   | :sliplib:`SLIPLIB_CONFIG_TABLE_FILENAME`   | :sliplib:`SLIPLIB_ConfigTblEntry_t::iParam`                                    |
+-------------------------------+------------------------------------+------------------------------------------------------------------------+
| :sliplib:`SLIPLIB_CDS_TABLENAME`      | :sliplib:`SLIPLIB_CDS_TABLE_FILENAME`      | :sliplib:`SLIPLIB_CdsTbl_t::iParam`                                            |
+-------------------------------+------------------------------------+------------------------------------------------------------------------+

.. literalinclude:: ../fsw/tables/sliplib_config.c
  :language: C
  :caption: Application Configuration Table


System Integration
^^^^^^^^^^^^^^^^^^

TODO: Describe System Integration instructions specific to SLIPLIB.


Run Time Configuration
^^^^^^^^^^^^^^^^^^^^^^

TODO: Describe Run Time Configuration for SLIPLIB.














