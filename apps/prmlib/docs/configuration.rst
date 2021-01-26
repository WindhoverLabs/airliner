Configuration
=============

Mission Specific
^^^^^^^^^^^^^^^^

TODO: Add Doxygen documentation for prm_mission_cfg.h and prm_perfids.h

+----------------------------+--------------------------------+
| File                       | Parameter                      |
+============================+================================+
| :prmlib:`prm_mission_cfg.h`| :prmlib:`PRM_SB_TIMEOUT`       |
+----------------------------+--------------------------------+
| :prmlib:`prm_perfids.h`    | :prmlib:`PRM_MAIN_TASK_PERF_ID`|
+----------------------------+--------------------------------+

.. note::
   The performance IDs defined in the :prmlib:`prm_perfids.h` file must have values
   that are unique system wide.  Most applications also have performance IDs.
   No two performance IDs may have the same value.
   

Platform Specific
^^^^^^^^^^^^^^^^^

TODO: Add Doxygen documentation for prmlib_msgids.h, prm_msgids.h, and prmlib_platform_cfg.h

+---------------------------------+----------------------------------------+
| File                            | Parameter                              |
+=================================+========================================+
| :prmlib:`prmlib_msgids.h`       | :prmlib:`PRMLIB_PARAM_UPDATED_MID`     |
+---------------------------------+----------------------------------------+
| :prmlib:`prm_msgids.h`          | :prmlib:`PRM_CMD_MID`                  |
+                                 +----------------------------------------+
|                                 | :prmlib:`PRM_WAKEUP_MID`               |
+                                 +----------------------------------------+
|                                 | :prmlib:`PRM_OUT_DATA_MID`             |
+                                 +----------------------------------------+
|                                 | :prmlib:`PRM_HK_TLM_MID`               |
+---------------------------------+----------------------------------------+
| :prmlib:`prmlib_platform_cfg.h` | :prmlib:`PRM_MISSION_REV`              |
+                                 +----------------------------------------+
|                                 | :prmlib:`PRM_SCH_PIPE_DEPTH`           |
+                                 +----------------------------------------+
|                                 | :prmlib:`PRM_SCH_PIPE_NAME`            |
+                                 +----------------------------------------+
|                                 | :prmlib:`PRM_SCH_PIPE_PEND_TIME`       |
+                                 +----------------------------------------+
|                                 | :prmlib:`PRM_SCH_PIPE_WAKEUP_RESERVED` |
+                                 +----------------------------------------+
|                                 | :prmlib:`PRM_SCH_PIPE_SEND_HK_RESERVED`|
+                                 +----------------------------------------+
|                                 | :prmlib:`PRM_CMD_PIPE_DEPTH`           |
+                                 +----------------------------------------+
|                                 | :prmlib:`PRM_CMD_PIPE_NAME`            |
+                                 +----------------------------------------+
|                                 | :prmlib:`PRM_DATA_PIPE_DEPTH`          |
+                                 +----------------------------------------+
|                                 | :prmlib:`PRM_DATA_PIPE_NAME`           |
+                                 +----------------------------------------+
|                                 | :prmlib:`PRM_CONFIG_TABLE_FILENAME`    |
+                                 +----------------------------------------+
|                                 | :prmlib:`PRM_STARTUP_TIMEOUT_MSEC`     |
+                                 +----------------------------------------+
|                                 | :prmlib:`PRM_CUSTOM_DEV_PATH`          |
+---------------------------------+----------------------------------------+

.. note::
   The message IDs defined in the :prmlib:`prmlib_msgids.h` and :prmlib:`prm_msgids.h` files must have values
   that are unique system wide.  Most applications also have message IDs.
   No two message IDs may have the same value.

TODO: Public Specific documentation.

System Integration
^^^^^^^^^^^^^^^^^^

TODO: Describe System Integration instructions specific to PRMLIB.


Run Time Configuration
^^^^^^^^^^^^^^^^^^^^^^

TODO: Describe Run Time Configuration for PRMLIB.
