Configuration
=============

TODO

Mission Specific
^^^^^^^^^^^^^^^^

TODO: Add Doxygen documentation for hmc5883_mission_cfg.h

+-----------------------------------+-------------------------------------+
| File                              | Parameter                           |
+===================================+=====================================+
| :hmc5883:`hmc5883_mission_cfg.h`  | :hmc5883:`HMC5883_SB_TIMEOUT`       |
+-----------------------------------+-------------------------------------+
| :hmc5883:`hmc5883_perfids.h`      | :hmc5883:`HMC5883_MAIN_TASK_PERF_ID`|
+                                   +-------------------------------------+
|                                   | :hmc5883:`HMC5883_RECEIVE_PERF_ID`  |
+                                   +-------------------------------------+
|                                   | :hmc5883:`HMC5883_SEND_PERF_ID`     |
+-----------------------------------+-------------------------------------+

.. note::
   The performance IDs defined in the :hmc5883:`hmc5883_perfids.h` file must have values
   that are unique system wide.  Most applications also have performance IDs.
   No two performance IDs may have the same value.
   

Platform Specific
^^^^^^^^^^^^^^^^^

+-----------------------------------+---------------------------------------------+
| File                              | Parameter                                   |
+===================================+=============================================+
| :hmc5883:`hmc5883_msgids.h`       | :hmc5883:`HMC5883_HK_TLM_MID`               |
+                                   +---------------------------------------------+
|                                   | :hmc5883:`HMC5883_SEND_HK_MID`              |
+                                   +---------------------------------------------+
|                                   | :hmc5883:`HMC5883_WAKEUP_MID`               |
+                                   +---------------------------------------------+
|                                   | :hmc5883:`HMC5883_CMD_MID`                  |
+                                   +---------------------------------------------+
|                                   | :hmc5883:`HMC5883_DIAG_TLM_MID`             |
+-----------------------------------+---------------------------------------------+
| :hmc5883:`hmc5883_platform_cfg.h` | :hmc5883:`HMC5883_MISSION_REV`              |
+                                   +---------------------------------------------+
|                                   | :hmc5883:`HMC5883_SCH_PIPE_DEPTH`           |
+                                   +---------------------------------------------+
|                                   | :hmc5883:`HMC5883_SCH_PIPE_NAME`            |
+                                   +---------------------------------------------+
|                                   | :hmc5883:`HMC5883_SCH_PIPE_PEND_TIME`       |
+                                   +---------------------------------------------+
|                                   | :hmc5883:`HMC5883_WAKEUP_MID_MAX_MSG_COUNT` |
+                                   +---------------------------------------------+
|                                   | :hmc5883:`HMC5883_SEND_HK_MID_MAX_MSG_COUNT`|
+                                   +---------------------------------------------+
|                                   | :hmc5883:`HMC5883_CMD_PIPE_DEPTH`           |
+                                   +---------------------------------------------+
|                                   | :hmc5883:`HMC5883_CMD_PIPE_NAME`            |
+                                   +---------------------------------------------+
|                                   | :hmc5883:`HMC5883_DATA_PIPE_DEPTH`          |
+                                   +---------------------------------------------+
|                                   | :hmc5883:`HMC5883_DATA_PIPE_NAME`           |
+                                   +---------------------------------------------+
|                                   | :hmc5883:`HMC5883_CONFIG_TABLE_FILENAME`    |
+                                   +---------------------------------------------+
|                                   | :hmc5883:`HMC5883_STARTUP_TIMEOUT_MSEC`     |
+                                   +---------------------------------------------+
|                                   | :hmc5883:`HMC5883_MAG_DIVIDER`              |
+                                   +---------------------------------------------+
|                                   | :hmc5883:`HMC5883_MAG_UNIT`                 |
+                                   +---------------------------------------------+
|                                   | :hmc5883:`HMC5883_CALC_MAG_RANGE`           |
+-----------------------------------+---------------------------------------------+

.. note::
   The message IDs defined in the :hmc5883:`hmc5883_msgids.h` file must have values
   that are unique system wide.  Most applications also have message IDs.
   No two message IDs may have the same value.


Table(s)
^^^^^^^^^^^^^^^^


+--------------------------------------+------------------------------------------+--------------------------------------------+
| Table Name                           | Default file name                        | Parameter                                  |
+======================================+==========================================+============================================+
| :hmc5883:`HMC5883_CONFIG_TABLENAME`  | :hmc5883:`HMC5883_CONFIG_TABLE_FILENAME` | :hmc5883:`HMC5883_ConfigTbl_t::x_scale`    |              
+                                      |                                          +--------------------------------------------+
|                                      |                                          | :hmc5883:`HMC5883_ConfigTbl_t::y_scale`    |
+                                      |                                          +--------------------------------------------+
|                                      |                                          | :hmc5883:`HMC5883_ConfigTbl_t::z_scale`    |
+                                      |                                          +--------------------------------------------+
|                                      |                                          | :hmc5883:`HMC5883_ConfigTbl_t::x_offset`   |
+                                      |                                          +--------------------------------------------+
|                                      |                                          | :hmc5883:`HMC5883_ConfigTbl_t::y_offset`   |
+                                      |                                          +--------------------------------------------+
|                                      |                                          | :hmc5883:`HMC5883_ConfigTbl_t::z_offset`   |
+--------------------------------------+------------------------------------------+--------------------------------------------+

Below is an example table.

.. literalinclude:: ../fsw/tables/hmc5883_config.c
  :language: C
  :caption: Nominal Configuration Table


System Integration
^^^^^^^^^^^^^^^^^^

TODO: Describe System Integration instructions specific to HMC5883.


Run Time Configuration
^^^^^^^^^^^^^^^^^^^^^^

TODO: Describe Run Time Configuration for HMC5883.
























