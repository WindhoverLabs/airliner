Configuration
=============

Mission Specific
^^^^^^^^^^^^^^^^

TODO: Add Doxygen documentation for sens_mission_cfg.h

+-----------------------------+--------------------------------+
| File                        | Parameter                      |
+=============================+================================+
| :sens:`sens_mission_cfg.h`  | :sens:`SENS_SB_TIMEOUT`        |
+-----------------------------+--------------------------------+
| :sens:`sens_perfids.h`      | :sens:`SENS_MAIN_TASK_PERF_ID` |
+-----------------------------+--------------------------------+

.. note::
   The performance IDs defined in the :sens:`sens_perfids.h` file must have values
   that are unique system wide.  Most applications also have performance IDs.
   No two performance IDs may have the same value.
   

Platform Specific
^^^^^^^^^^^^^^^^^

TODO: Add Doxygen documentation for sens_platform_cfg.h


+-----------------------------+---------------------------------------+
| File                        | Parameter                             |
+=============================+=======================================+
| :sens:`sens_msgids.h`       | :sens:`SENS_HK_TLM_MID`               |
|                             +---------------------------------------+
|                             | :sens:`SENS_SEND_HK_MID`              |
+                             +---------------------------------------+
|                             | :sens:`SENS_WAKEUP_MID`               |
+                             +---------------------------------------+
|                             | :sens:`SENS_CMD_MID`                  |
+-----------------------------+---------------------------------------+
| :sens:`sens_platform_cfg.h` | :sens:`SENS_MISSION_REV`              |
+                             +---------------------------------------+
|                             | :sens:`SENS_SCH_PIPE_DEPTH`           |
+                             +---------------------------------------+
|                             | :sens:`SENS_SCH_PIPE_NAME`            |
+                             +---------------------------------------+
|                             | :sens:`SENS_SCH_PIPE_PEND_TIME`       |
+                             +---------------------------------------+
|                             | :sens:`SENS_WAKEUP_MID_MAX_MSG_COUNT` |
+                             +---------------------------------------+
|                             | :sens:`SENS_SEND_HK_MID_MAX_MSG_COUNT`|
+                             +---------------------------------------+
|                             | :sens:`SENS_CMD_PIPE_DEPTH`           |
+                             +---------------------------------------+
|                             | :sens:`SENS_CMD_PIPE_NAME`            |
+                             +---------------------------------------+
|                             | :sens:`SENS_DATA_PIPE_DEPTH`          |
+                             +---------------------------------------+
|                             | :sens:`SENS_DATA_PIPE_NAME`           |
+                             +---------------------------------------+
|                             | :sens:`SENS_CONFIG_TABLE_FILENAME`    |
+                             +---------------------------------------+
|                             | :sens:`SENS_STARTUP_TIMEOUT_MSEC`     |
+-----------------------------+---------------------------------------+

.. note::
   The message IDs defined in the :sens:`sens_msgids.h` file must have values
   that are unique system wide.  Most applications also have message IDs.
   No two message IDs may have the same value.


Table(s)
^^^^^^^^

TODO: Add Table explanation

TODO: Add Doxygen documentation for sens_config.tbl

+-------------------------------+------------------------------------+-----------------------------------------------------------+
| Table Name                    | Default file name                  | Parameter                                                 |
+===============================+====================================+===========================================================+
| :sens:`SENS_CONFIG_TABLENAME` | :sens:`SENS_CONFIG_TABLE_FILENAME` | :sens:`SENS_ConfigTbl_t::Min`                             |
+                               |                                    +-----------------------------------------------------------+
|                               |                                    | :sens:`SENS_ConfigTbl_t::Trim`                            |
+                               |                                    +-----------------------------------------------------------+
|                               |                                    | :sens:`SENS_ConfigTbl_t::Max`                             |
+                               |                                    +-----------------------------------------------------------+
|                               |                                    | :sens:`SENS_ConfigTbl_t::Rev`                             |
+                               |                                    +-----------------------------------------------------------+
|                               |                                    | :sens:`SENS_ConfigTbl_t::DZ`                              |
+                               |                                    +-----------------------------------------------------------+
|                               |                                    | :sens:`SENS_ConfigTbl_t::MapRoll`                         |
+                               |                                    +-----------------------------------------------------------+
|                               |                                    | :sens:`SENS_ConfigTbl_t::MapPitch`                        |
+                               |                                    +-----------------------------------------------------------+
|                               |                                    | :sens:`SENS_ConfigTbl_t::MapYaw`                          |
+                               |                                    +-----------------------------------------------------------+
|                               |                                    | :sens:`SENS_ConfigTbl_t::MapThrottle`                     |
+                               |                                    +-----------------------------------------------------------+
|                               |                                    | :sens:`SENS_ConfigTbl_t::MapFailsafe`                     |
+                               |                                    +-----------------------------------------------------------+
|                               |                                    | :sens:`SENS_ConfigTbl_t::MapModeSw`                       |
+                               |                                    +-----------------------------------------------------------+
|                               |                                    | :sens:`SENS_ConfigTbl_t::MapReturnSw`                     |
+                               |                                    +-----------------------------------------------------------+
|                               |                                    | :sens:`SENS_ConfigTbl_t::MapRattitudeSw`                  |
+                               |                                    +-----------------------------------------------------------+
|                               |                                    | :sens:`SENS_ConfigTbl_t::MapPosctlSw`                     |
+                               |                                    +-----------------------------------------------------------+
|                               |                                    | :sens:`SENS_ConfigTbl_t::MapLoiterSw`                     |
+                               |                                    +-----------------------------------------------------------+
|                               |                                    | :sens:`SENS_ConfigTbl_t::MapAcroSw`                       |
+                               |                                    +-----------------------------------------------------------+
|                               |                                    | :sens:`SENS_ConfigTbl_t::MapOffboardSw`                   |
+                               |                                    +-----------------------------------------------------------+
|                               |                                    | :sens:`SENS_ConfigTbl_t::MapKillSw`                       |
+                               |                                    +-----------------------------------------------------------+
|                               |                                    | :sens:`SENS_ConfigTbl_t::MapArmSw`                        |
+                               |                                    +-----------------------------------------------------------+
|                               |                                    | :sens:`SENS_ConfigTbl_t::MapTransSw`                      |
+                               |                                    +-----------------------------------------------------------+
|                               |                                    | :sens:`SENS_ConfigTbl_t::MapGearSw`                       |
+                               |                                    +-----------------------------------------------------------+
|                               |                                    | :sens:`SENS_ConfigTbl_t::MapStabSw`                       |
+                               |                                    +-----------------------------------------------------------+
|                               |                                    | :sens:`SENS_ConfigTbl_t::MapManSw`                        |
+                               |                                    +-----------------------------------------------------------+
|                               |                                    | :sens:`SENS_ConfigTbl_t::MapFlaps`                        |
+                               |                                    +-----------------------------------------------------------+
|                               |                                    | :sens:`SENS_ConfigTbl_t::MapAltctlSw`                     |
+                               |                                    +-----------------------------------------------------------+
|                               |                                    | :sens:`SENS_ConfigTbl_t::MapAux1`                         |
+                               |                                    +-----------------------------------------------------------+
|                               |                                    | :sens:`SENS_ConfigTbl_t::MapAux2`                         |
+                               |                                    +-----------------------------------------------------------+
|                               |                                    | :sens:`SENS_ConfigTbl_t::MapAux3`                         |
+                               |                                    +-----------------------------------------------------------+
|                               |                                    | :sens:`SENS_ConfigTbl_t::MapAux4`                         |
+                               |                                    +-----------------------------------------------------------+
|                               |                                    | :sens:`SENS_ConfigTbl_t::MapAux5`                         |
+                               |                                    +-----------------------------------------------------------+
|                               |                                    | :sens:`SENS_ConfigTbl_t::MapParam`                        |
+                               |                                    +-----------------------------------------------------------+
|                               |                                    | :sens:`SENS_ConfigTbl_t::MapFlightMode`                   |
+                               |                                    +-----------------------------------------------------------+
|                               |                                    | :sens:`SENS_ConfigTbl_t::FailsThr`                        |
+                               |                                    +-----------------------------------------------------------+
|                               |                                    | :sens:`SENS_ConfigTbl_t::AssistTh`                        |
+                               |                                    +-----------------------------------------------------------+
|                               |                                    | :sens:`SENS_ConfigTbl_t::AutoTh`                          |
+                               |                                    +-----------------------------------------------------------+
|                               |                                    | :sens:`SENS_ConfigTbl_t::RattitudeTh`                     |
+                               |                                    +-----------------------------------------------------------+
|                               |                                    | :sens:`SENS_ConfigTbl_t::PosctlTh`                        |
+                               |                                    +-----------------------------------------------------------+
|                               |                                    | :sens:`SENS_ConfigTbl_t::ReturnTh`                        |
+                               |                                    +-----------------------------------------------------------+
|                               |                                    | :sens:`SENS_ConfigTbl_t::LoiterTh`                        |
+                               |                                    +-----------------------------------------------------------+
|                               |                                    | :sens:`SENS_ConfigTbl_t::AcroTh`                          |
+                               |                                    +-----------------------------------------------------------+
|                               |                                    | :sens:`SENS_ConfigTbl_t::OffboardTh`                      |
+                               |                                    +-----------------------------------------------------------+
|                               |                                    | :sens:`SENS_ConfigTbl_t::KillswitchTh`                    |
+                               |                                    +-----------------------------------------------------------+
|                               |                                    | :sens:`SENS_ConfigTbl_t::ArmswitchTh`                     |
+                               |                                    +-----------------------------------------------------------+
|                               |                                    | :sens:`SENS_ConfigTbl_t::TransTh`                         |
+                               |                                    +-----------------------------------------------------------+
|                               |                                    | :sens:`SENS_ConfigTbl_t::GearTh`                          |
+                               |                                    +-----------------------------------------------------------+
|                               |                                    | :sens:`SENS_ConfigTbl_t::StabTh`                          |
+                               |                                    +-----------------------------------------------------------+
|                               |                                    | :sens:`SENS_ConfigTbl_t::ManTh`                           |
+                               |                                    +-----------------------------------------------------------+
|                               |                                    | :sens:`SENS_ConfigTbl_t::AltctlTh`                        |
+                               |                                    +-----------------------------------------------------------+
|                               |                                    | :sens:`SENS_ConfigTbl_t::FltSmpRate`                      |
+                               |                                    +-----------------------------------------------------------+
|                               |                                    | :sens:`SENS_ConfigTbl_t::FltCutoff`                       |
+                               |                                    +-----------------------------------------------------------+
|                               |                                    | :sens:`SENS_ConfigTbl_t::ModeSwitchControl`               |
+-------------------------------+------------------------------------+-----------------------------------------------------------+

TODO: Add Doxygen documentation for sens_config.c

Below is an example table.

.. literalinclude:: ../fsw/tables/sens_config.c
  :language: C
  :caption: Application Configuration Table


System Integration
^^^^^^^^^^^^^^^^^^

TODO: Describe System Integration instructions specific to SENS.


Run Time Configuration
^^^^^^^^^^^^^^^^^^^^^^

TODO: Describe Run Time Configuration for SENS.
