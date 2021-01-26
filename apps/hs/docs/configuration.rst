Configuration
=============

Mission Specific
^^^^^^^^^^^^^^^^

+-------------------------+--------------------------------+
| File                    | Parameter                      |
+=========================+================================+
| :hs:`hs_perfids.h`      | :hs:`HS_IDLETASK_PERF_ID`      |
+                         +--------------------------------+
|                         | :hs:`HS_APPMAIN_PERF_ID`       |
+-------------------------+--------------------------------+

.. note::
   The performance IDs defined in the :hs:`hs_perfids.h` file must have values
   that are unique system wide.  Most applications also have performance IDs.
   No two performance IDs may have the same value.
   

Platform Specific
^^^^^^^^^^^^^^^^^

+-------------------------+-------------------------------------+
| File                    | Parameter                           |
+=========================+=====================================+
| :hs:`hs_msgids.h`       | :hs:`HS_CMD_MID`                    |
|                         +-------------------------------------+
|                         | :hs:`HS_SEND_HK_MID`                |
+                         +-------------------------------------+
|                         | :hs:`HS_WAKEUP_MID`                 |
+                         +-------------------------------------+
|                         | :hs:`HS_HK_TLM_MID`                 |
+-------------------------+-------------------------------------+
| :hs:`hs_platform_cfg.h` | :hs:`HS_IDLE_TASK_PRIORITY`         |
+                         +-------------------------------------+
|                         | :hs:`HS_MAX_EXEC_CNT_SLOTS`         |
+                         +-------------------------------------+
|                         | :hs:`HS_MAX_MSG_ACT_TYPES`          |
+                         +-------------------------------------+
|                         | :hs:`HS_MAX_MSG_ACT_SIZE`           |
+                         +-------------------------------------+
|                         | :hs:`HS_MAX_MONITORED_APPS`         |
+                         +-------------------------------------+
|                         | :hs:`HS_MAX_MONITORED_EVENTS`       |
+                         +-------------------------------------+
|                         | :hs:`HS_WATCHDOG_TIMEOUT_VALUE`     |
+                         +-------------------------------------+
|                         | :hs:`HS_POST_PROCESSING_DELAY`      |
+                         +-------------------------------------+
|                         | :hs:`HS_WAKEUP_TIMEOUT`             |
+                         +-------------------------------------+
|                         | :hs:`HS_CPU_ALIVE_STRING`           |
+                         +-------------------------------------+
|                         | :hs:`HS_CPU_ALIVE_PERIOD`           |
+                         +-------------------------------------+
|                         | :hs:`HS_MAX_RESTART_ACTIONS`        |
+                         +-------------------------------------+
|                         | :hs:`HS_CMD_PIPE_DEPTH`             |
+                         +-------------------------------------+
|                         | :hs:`HS_WAKEUP_PIPE_DEPTH`          |
+                         +-------------------------------------+
|                         | :hs:`HS_RESET_TASK_DELAY`           |
+                         +-------------------------------------+
|                         | :hs:`HS_STARTUP_SYNC_TIMEOUT`       |
+                         +-------------------------------------+
|                         | :hs:`HS_APPMON_DEFAULT_STATE`       |
+                         +-------------------------------------+
|                         | :hs:`HS_EVENTMON_DEFAULT_STATE`     |
+                         +-------------------------------------+
|                         | :hs:`HS_ALIVENESS_DEFAULT_STATE`    |
+                         +-------------------------------------+
|                         | :hs:`HS_CPUHOG_DEFAULT_STATE`       |
+                         +-------------------------------------+
|                         | :hs:`HS_AMT_FILENAME`               |
+                         +-------------------------------------+
|                         | :hs:`HS_EMT_FILENAME`               |
+                         +-------------------------------------+
|                         | :hs:`HS_XCT_FILENAME`               |
+                         +-------------------------------------+
|                         | :hs:`HS_MAT_FILENAME`               |
+                         +-------------------------------------+
|                         | :hs:`HS_UTIL_CALLS_PER_MARK`        |
+                         +-------------------------------------+
|                         | :hs:`HS_UTIL_CYCLES_PER_INTERVAL`   |
+                         +-------------------------------------+
|                         | :hs:`HS_UTIL_PER_INTERVAL_TOTAL`    |
+                         +-------------------------------------+
|                         | :hs:`HS_UTIL_PER_INTERVAL_HOGGING`  |
+                         +-------------------------------------+
|                         | :hs:`HS_UTIL_CONV_MULT1`            |
+                         +-------------------------------------+
|                         | :hs:`HS_UTIL_CONV_DIV`              |
+                         +-------------------------------------+
|                         | :hs:`HS_UTIL_CONV_MULT2`            |
+                         +-------------------------------------+
|                         | :hs:`HS_UTIL_HOGGING_TIMEOUT`       |
+                         +-------------------------------------+
|                         | :hs:`HS_UTIL_PEAK_NUM_INTERVAL`     |
+                         +-------------------------------------+
|                         | :hs:`HS_UTIL_AVERAGE_NUM_INTERVAL`  |
+                         +-------------------------------------+
|                         | :hs:`HS_UTIL_TIME_DIAG_ARRAY_POWER` |
+                         +-------------------------------------+
|                         | :hs:`HS_UTIL_TIME_DIAG_ARRAY_LENGTH`|
+                         +-------------------------------------+
|                         | :hs:`HS_UTIL_TIME_DIAG_ARRAY_MASK`  |
+                         +-------------------------------------+
|                         | :hs:`HS_MISSION_REV`                |
+-------------------------+-------------------------------------+

.. note::
   The message IDs defined in the :hs:`hs_msgids.h` file must have values
   that are unique system wide.  Most applications also have message IDs.
   No two message IDs may have the same value.
   
TODO: Describe the "custom" layer code.


Table(s)
^^^^^^^^
TODO: Tailor this section to be more HS specific.

+-------------------------------+------------------------------------+------------------------------------------+
| Table Name                    | Default file name                  | Parameter                                |
+===============================+====================================+==========================================+
| :hs:`HS_AMT_TABLENAME`        | :hs:`HS_AMT_FILENAME`              | :hs:`HS_AMTEntry_t::AppName`             |
+                               |                                    +------------------------------------------+
|                               |                                    | :hs:`HS_AMTEntry_t::NullTerm`            |
+                               |                                    +------------------------------------------+
|                               |                                    | :hs:`HS_AMTEntry_t::CycleCount`          |
+                               |                                    +------------------------------------------+
|                               |                                    | :hs:`HS_AMTEntry_t::ActionType`          |
+-------------------------------+------------------------------------+------------------------------------------+
| :hs:`HS_EMT_TABLENAME`        | :hs:`HS_EMT_FILENAME`              | :hs:`HS_EMTEntry_t::AppName`             |
+                               |                                    +------------------------------------------+
|                               |                                    | :hs:`HS_EMTEntry_t::NullTerm`            |
+                               |                                    +------------------------------------------+
|                               |                                    | :hs:`HS_EMTEntry_t::EventID`             |
+                               |                                    +------------------------------------------+
|                               |                                    | :hs:`HS_EMTEntry_t::ActionType`          |
+-------------------------------+------------------------------------+------------------------------------------+
| :hs:`HS_MAT_TABLENAME`        | :hs:`HS_MAT_FILENAME`              | :hs:`HS_MATEntry_t::EnableState`         |
+                               |                                    +------------------------------------------+
|                               |                                    | :hs:`HS_MATEntry_t::Cooldown`            |
+                               |                                    +------------------------------------------+
|                               |                                    | :hs:`HS_MATEntry_t::Message`             |
+-------------------------------+------------------------------------+------------------------------------------+
| :hs:`HS_XCT_TABLENAME`        | :hs:`HS_XCT_FILENAME`              | :hs:`HS_XCTEntry_t::ResourceName`        |
+                               |                                    +------------------------------------------+
|                               |                                    | :hs:`HS_XCTEntry_t::NullTerm`            |
+                               |                                    +------------------------------------------+
|                               |                                    | :hs:`HS_XCTEntry_t::ResourceType`        |
+-------------------------------+------------------------------------+------------------------------------------+

Below are example tables.

.. literalinclude:: ../fsw/tables/hs_amt.c
  :encoding: iso-ir-199
  :language: C
  :caption: HS AppMon Table

.. literalinclude:: ../fsw/tables/hs_emt.c
  :encoding: iso-ir-199
  :language: C
  :caption: HS EventMon

.. literalinclude:: ../fsw/tables/hs_mat.c
  :encoding: iso-ir-199
  :language: C
  :caption: HS MsgActs Table

.. literalinclude:: ../fsw/tables/hs_xct.c
  :encoding: iso-ir-199
  :language: C
  :caption: HS ExeCount Table


System Integration
^^^^^^^^^^^^^^^^^^




Run Time Configuration
^^^^^^^^^^^^^^^^^^^^^^


TODO: Add references.
