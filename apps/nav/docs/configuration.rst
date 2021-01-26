Configuration
=============

Mission Specific
^^^^^^^^^^^^^^^^

TODO: Add Doxygen documentation for nav_mission_cfg.h

+-------------------------+--------------------------------+
| File                    | Parameter                      |
+=========================+================================+
| :nav:`nav_mission_cfg.h`| :nav:`NAV_SB_TIMEOUT`          |
+-------------------------+--------------------------------+
| :nav:`nav_perfids.h`    | :nav:`NAV_MAIN_TASK_PERF_ID`   |
+-------------------------+--------------------------------+

.. note::
   The performance IDs defined in the :nav:`nav_perfids.h` file must have values
   that are unique system wide.  Most applications also have performance IDs.
   No two performance IDs may have the same value.
   

Platform Specific
^^^^^^^^^^^^^^^^^

+--------------------------+-------------------------------------+
| File                     | Parameter                           |
+==========================+=====================================+
| :nav:`nav_msgids.h`      | :nav:`NAV_HK_TLM_MID`               |
|                          +-------------------------------------+
|                          | :nav:`NAV_SEND_HK_MID`              |
+                          +-------------------------------------+
|                          | :nav:`NAV_WAKEUP_MID`               |
+                          +-------------------------------------+
|                          | :nav:`NAV_CMD_MID`                  |
+--------------------------+-------------------------------------+
| :nav:`nav_platform_cfg.h`| :nav:`NAV_MISSION_REV`              |
+                          +-------------------------------------+
|                          | :nav:`NAV_SCH_PIPE_DEPTH`           |
+                          +-------------------------------------+
|                          | :nav:`NAV_SCH_PIPE_NAME`            |
+                          +-------------------------------------+
|                          | :nav:`NAV_SCH_PIPE_PEND_TIME`       |
+                          +-------------------------------------+
|                          | :nav:`NAV_WAKEUP_MID_MAX_MSG_COUNT` |
+                          +-------------------------------------+
|                          | :nav:`NAV_SEND_HK_MID_MAX_MSG_COUNT`|
+                          +-------------------------------------+
|                          | :nav:`NAV_CMD_PIPE_DEPTH`           |
+                          +-------------------------------------+
|                          | :nav:`NAV_CMD_PIPE_NAME`            |
+                          +-------------------------------------+
|                          | :nav:`NAV_DATA_PIPE_DEPTH`          |
+                          +-------------------------------------+
|                          | :nav:`NAV_DATA_PIPE_NAME`           |
+                          +-------------------------------------+
|                          | :nav:`NAV_CONFIG_TABLE_FILENAME`    |
+                          +-------------------------------------+
|                          | :nav:`NAV_STARTUP_TIMEOUT_MSEC`     |
+--------------------------+-------------------------------------+

.. note::
   The message IDs defined in the :nav:`nav_msgids.h` file must have values
   that are unique system wide.  Most applications also have message IDs.
   No two message IDs may have the same value.

Table(s)
^^^^^^^^

TODO: Add Table explanation

TODO: Add Doxygen documentation for nav_config.tbl

+-------------------------------+------------------------------------+--------------------------------------------+
| Table Name                    | Default file name                  | Parameter                                  |
+===============================+====================================+============================================+
| :nav:`NAV_CONFIG_TABLENAME`   | :nav:`NAV_CONFIG_TABLE_FILENAME`   | :nav:`NAV_ConfigTbl_t::NAV_ACC_RAD`        |
+                               |                                    +--------------------------------------------+
|                               |                                    | :nav:`NAV_ConfigTbl_t::NAV_ALT_RAD`        |
+                               |                                    +--------------------------------------------+
|                               |                                    | :nav:`NAV_ConfigTbl_t::NAV_LOITER_RAD`     |
+                               |                                    +--------------------------------------------+
|                               |                                    | :nav:`NAV_ConfigTbl_t::NAV_MIS_TAKEOFF_ALT`|
+                               |                                    +--------------------------------------------+
|                               |                                    | :nav:`NAV_ConfigTbl_t::NAV_MIS_YAW_ERR`    |
+                               |                                    +--------------------------------------------+
|                               |                                    | :nav:`NAV_ConfigTbl_t::NAV_MIS_YAW_TMT`    |
+                               |                                    +--------------------------------------------+
|                               |                                    | :nav:`NAV_ConfigTbl_t::NAV_MIS_LTRMIN_ALT` |
+                               |                                    +--------------------------------------------+
|                               |                                    | :nav:`NAV_ConfigTbl_t::NAV_RTL_RETURN_ALT` |
+                               |                                    +--------------------------------------------+
|                               |                                    | :nav:`NAV_ConfigTbl_t::NAV_RTL_DESCEND_ALT`|
+                               |                                    +--------------------------------------------+
|                               |                                    | :nav:`NAV_ConfigTbl_t::NAV_RTL_LAND_DELAY` |
+                               |                                    +--------------------------------------------+
|                               |                                    | :nav:`NAV_ConfigTbl_t::NAV_RTL_MIN_DIST`   |
+-------------------------------+------------------------------------+--------------------------------------------+

TODO: Add Doxygen documentation for nav_config.c

Below is an example table.

.. literalinclude:: ../fsw/tables/nav_config.c
  :language: C
  :caption: Nominal Configuration Table


System Integration
^^^^^^^^^^^^^^^^^^

TODO: Describe System Integration instructions specific to NAV.


Run Time Configuration
^^^^^^^^^^^^^^^^^^^^^^

TODO: Describe Run Time Configuration for NAV.
