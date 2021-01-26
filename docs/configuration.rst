Configuration
=============

TODO

Mission Specific
^^^^^^^^^^^^^^^^

TODO: Add Doxygen documentation for cvt_mission_cfg.h

+----------------------------+-------------------------------------+
| File                       | Parameter                           |
+============================+=====================================+
| :cvt:`cvt_mission_cfg.h`   | :cvt:`CVT_SB_TIMEOUT`               |
+----------------------------+-------------------------------------+
| :cvt:`cvt_perfids.h`       | :cvt:`CVT_MAIN_TASK_PERF_ID`        |
+----------------------------+-------------------------------------+

.. note::
   The performance IDs defined in the :cvt:`cvt_perfids.h` file must have values
   that are unique system wide.  Most applications also have performance IDs.
   No two performance IDs may have the same value.
   

Platform Specific
^^^^^^^^^^^^^^^^^

+-----------------------------+---------------------------------------------+
| File                        | Parameter                                   |
+=============================+=============================================+
| :cvt:`cvt_msgids.h`         | :cvt:`CVT_CMD_MID`                          |
+                             +---------------------------------------------+
|                             | :cvt:`CVT_SEND_HK_MID`                      |
+                             +---------------------------------------------+
|                             | :cvt:`CVT_WAKEUP_MID`                       |
+                             +---------------------------------------------+
|                             | :cvt:`CVT_OUT_DATA_MID`                     |
+                             +---------------------------------------------+
|                             | :cvt:`CVT_HK_TLM_MID`                       |
+-----------------------------+---------------------------------------------+
| :cvt:`cvt_platform_cfg.h`   | :cvt:`CVT_MISSION_REV`                      |
+                             +---------------------------------------------+
|                             | :cvt:`CVT_SCH_PIPE_DEPTH`                   |
+                             +---------------------------------------------+
|                             | :cvt:`CVT_SCH_PIPE_NAME`                    |
+                             +---------------------------------------------+
|                             | :cvt:`CVT_SCH_PIPE_PEND_TIME`               |
+                             +---------------------------------------------+
|                             | :cvt:`CVT_SCH_PIPE_WAKEUP_RESERVED`         |
+                             +---------------------------------------------+
|                             | :cvt:`CVT_SCH_PIPE_SEND_HK_RESERVED`        |
+                             +---------------------------------------------+
|                             | :cvt:`CVT_CMD_PIPE_DEPTH`                   |
+                             +---------------------------------------------+
|                             | :cvt:`CVT_CMD_PIPE_NAME`                    |
+                             +---------------------------------------------+
|                             | :cvt:`CVT_DATA_PIPE_DEPTH`                  |
+                             +---------------------------------------------+
|                             | :cvt:`CVT_DATA_PIPE_NAME`                   |
+                             +---------------------------------------------+
|                             | :cvt:`CVT_CONFIG_TABLE_FILENAME`            |
+                             +---------------------------------------------+
|                             | :cvt:`CVT_STARTUP_TIMEOUT_MSEC`             |
+                             +---------------------------------------------+
|                             | :cvt:`CVT_STARTUP_TIMEOUT_MSEC`             |
+                             +---------------------------------------------+
|                             | :cvt:`CVT_MAX_REGISTRATIONS`                |
+                             +---------------------------------------------+
|                             | :cvt:`CVT_DATA_STORE_SIZE`                  |
+-----------------------------+---------------------------------------------+

.. note::
   The message IDs defined in the :cvt:`cvt_msgids.h` file must have values
   that are unique system wide.  Most applications also have message IDs.
   No two message IDs may have the same value.

Table(s)
^^^^^^^^^^^^^^^^

TODO REVIEW

+-------------------------------+------------------------------------+------------------------------------------------------------------------+
| Table Name                    | Default file name                  | Parameter                                                              |
+===============================+====================================+========================================================================+
| :cvt:`CVT_CONFIG_TABLENAME`   | :cvt:`CVT_CONFIG_TABLE_FILENAME`   | :cvt:`CVT_ConfigTblEntry_t::iParam`                                    |
+-------------------------------+------------------------------------+------------------------------------------------------------------------+
| :cvt:`CVT_CDS_TABLENAME`      | :cvt:`CVT_CDS_TABLE_FILENAME`      | :cvt:`CVT_CdsTbl_t::iParam`                                            |
+-------------------------------+------------------------------------+------------------------------------------------------------------------+

.. literalinclude:: ../fsw/tables/cvt_config.c
  :language: C
  :caption: Application Configuration Table


System Integration
^^^^^^^^^^^^^^^^^^

TODO: Describe System Integration instructions specific to CVT.


Run Time Configuration
^^^^^^^^^^^^^^^^^^^^^^

TODO: Describe Run Time Configuration for CVT.














