Configuration
=============

TODO

Mission Specific
^^^^^^^^^^^^^^^^

+----------------------------+-------------------------------------+
| File                       | Parameter                           |
+============================+=====================================+
| :vm:`vm_perfids.h`         | :vm:`VM_MAIN_TASK_PERF_ID`          |
+----------------------------+-------------------------------------+

.. note::
   The performance IDs defined in the :vm:`vm_perfids.h` file must have values
   that are unique system wide.  Most applications also have performance IDs.
   No two performance IDs may have the same value.
   

Platform Specific
^^^^^^^^^^^^^^^^^

+-----------------------------+---------------------------------------------+
| File                        | Parameter                                   |
+=============================+=============================================+
| :VM:`vm_msgids.h`           | :VM:`VM_CMD_MID`                            |
+                             +---------------------------------------------+
|                             | :VM:`VM_SEND_HK_MID`                        |
+                             +---------------------------------------------+
|                             | :VM:`VM_WAKEUP_MID`                         |
+                             +---------------------------------------------+
|                             | :VM:`VM_HK_TLM_MID`                         |
+                             +---------------------------------------------+
|                             | :VM:`VM_CONFIG_TLM_MID`                     |
+-----------------------------+---------------------------------------------+
| :VM:`vm_platform_cfg.h`     | :vm:`VM_MISSION_REV`                        |
+                             +---------------------------------------------+
|                             | :vm:`VM_SCH_PIPE_DEPTH`                     |
+                             +---------------------------------------------+
|                             | :vm:`VM_SCH_PIPE_NAME`                      |
+                             +---------------------------------------------+
|                             | :vm:`VM_SCH_PIPE_PEND_TIME`                 |
+                             +---------------------------------------------+
|                             | :vm:`VM_WAKEUP_MID_MAX_MSG_COUNT`           |
+                             +---------------------------------------------+
|                             | :vm:`VM_SEND_HK_MID_MAX_MSG_COUNT`          |
+                             +---------------------------------------------+
|                             | :vm:`VM_CMD_PIPE_DEPTH`                     |
+                             +---------------------------------------------+
|                             | :vm:`VM_CMD_PIPE_NAME`                      |
+                             +---------------------------------------------+
|                             | :vm:`VM_PARAM_PIPE_DEPTH`                   |
+                             +---------------------------------------------+
|                             | :vm:`VM_PARAM_PIPE_NAME`                    |
+                             +---------------------------------------------+
|                             | :vm:`VM_CONFIG_TABLE_FILENAME`              |
+                             +---------------------------------------------+
|                             | :vm:`VM_STARTUP_TIMEOUT_MSEC`               |
+                             +---------------------------------------------+
|                             | :vm:`VM_BATTERY_VOLTAGE_CHECK_DELAY`        |
+                             +---------------------------------------------+
|                             | :vm:`VM_MINIMUM_VALID_BATTERY_VOLTAGE`      |
+-----------------------------+---------------------------------------------+

.. note::
   The message IDs defined in the :vm:`vm_msgids.h` file must have values
   that are unique system wide.  Most applications also have message IDs.
   No two message IDs may have the same value.

Table(s)
^^^^^^^^^^^^^^^^

+-------------------------------+------------------------------------+--------------------------------------------+
| Table Name                    | Default file name                  | Parameter                                  |
+===============================+====================================+============================================+
| :vm:`VM_CONFIG_TABLENAME`     | :vm:`VM_CONFIG_TABLE_FILENAME`     | :vm:`VM_ConfigTbl_t::COM_RC_IN_MODE`       |
+                               |                                    +--------------------------------------------+
|                               |                                    | :vm:`VM_ConfigTbl_t::COM_ARM_SWISBTN`      |
+                               |                                    +--------------------------------------------+
|                               |                                    | :vm:`VM_ConfigTbl_t::COM_RC_ARM_HYST`      |
+                               |                                    +--------------------------------------------+
|                               |                                    | :vm:`VM_ConfigTbl_t::MAV_SYS_ID`           |
+                               |                                    +--------------------------------------------+
|                               |                                    | :vm:`VM_ConfigTbl_t::MAV_COMP_ID`          |
+                               |                                    +--------------------------------------------+
|                               |                                    | :vm:`VM_ConfigTbl_t::COM_RC_LOSS_T`        |
+                               |                                    +--------------------------------------------+
|                               |                                    | :vm:`VM_ConfigTbl_t::COM_LOW_BAT_ACT`      |
+                               |                                    +--------------------------------------------+
|                               |                                    | :vm:`VM_ConfigTbl_t::COM_HOME_H_T`         |
+                               |                                    +--------------------------------------------+
|                               |                                    | :vm:`VM_ConfigTbl_t::COM_HOME_V_T`         |
+                               |                                    +--------------------------------------------+
|                               |                                    | :vm:`VM_ConfigTbl_t::HOME_POS_ALT_PADDING` |
+-------------------------------+------------------------------------+--------------------------------------------+


Below is an example table.

.. literalinclude:: ../fsw/tables/vm_config.c
  :language: C
  :caption: Application Configuration Table


System Integration
^^^^^^^^^^^^^^^^^^

TODO: Describe System Integration instructions specific to VM.


Run Time Configuration
^^^^^^^^^^^^^^^^^^^^^^

TODO: Describe Run Time Configuration for VM.

