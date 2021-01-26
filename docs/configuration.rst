Configuration
=============

TODO

Mission Specific
^^^^^^^^^^^^^^^^

+----------------------------+-------------------------------------+
| File                       | Parameter                           |
+============================+=====================================+
| :cs:`cs_mission_cfg.h`     | :cs:`CS_DEFAULT_ALGORITHM`          |
+----------------------------+-------------------------------------+
| :cs:`cs_perfids.h`         | :cs:`CS_APPMAIN_PERF_ID`            |
+----------------------------+-------------------------------------+

.. note::
   The performance IDs defined in the :cs:`cs_perfids.h` file must have values
   that are unique system wide.  Most applications also have performance IDs.
   No two performance IDs may have the same value.
   

Platform Specific
^^^^^^^^^^^^^^^^^

+-----------------------------+----------------------------------------------------+
| File                        | Parameter                                          |
+=============================+====================================================+
| :cs:`cs_msgids.h`           | :cs:`CS_CMD_MID`                                   |
+                             +----------------------------------------------------+
|                             | :cs:`CS_SEND_HK_MID`                               |
+                             +----------------------------------------------------+
|                             | :cs:`CS_BACKGROUND_CYCLE_MID`                      |
+                             +----------------------------------------------------+
|                             | :cs:`CS_HK_TLM_MID`                                |
+-----------------------------+----------------------------------------------------+
| :cs:`cs_platform_cfg.h`     | :cs:`CS_CHILD_TASK_FLAGS`                          |
+                             +----------------------------------------------------+
|                             | :cs:`CS_DEF_EEPROM_TABLE_FILENAME`                 |
+                             +----------------------------------------------------+
|                             | :cs:`CS_DEF_MEMORY_TABLE_FILENAME`                 |
+                             +----------------------------------------------------+
|                             | :cs:`CS_DEF_TABLES_TABLE_FILENAME`                 |
+                             +----------------------------------------------------+
|                             | :cs:`CS_DEF_APP_TABLE_FILENAME`                    |
+                             +----------------------------------------------------+
|                             | :cs:`CS_PIPE_DEPTH`                                |
+                             +----------------------------------------------------+
|                             | :cs:`CS_MAX_NUM_EEPROM_TABLE_ENTRIES`              |
+                             +----------------------------------------------------+
|                             | :cs:`CS_MAX_NUM_MEMORY_TABLE_ENTRIES`              |
+                             +----------------------------------------------------+
|                             | :cs:`CS_MAX_NUM_TABLES_TABLE_ENTRIES`              |
+                             +----------------------------------------------------+
|                             | :cs:`CS_MAX_NUM_APP_TABLE_ENTRIES`                 |
+                             +----------------------------------------------------+
|                             | :cs:`CS_DEFAULT_BYTES_PER_CYCLE`                   |
+                             +----------------------------------------------------+
|                             | :cs:`CS_CHILD_TASK_PRIORITY`                       |
+                             +----------------------------------------------------+
|                             | :cs:`CS_CHILD_TASK_DELAY`                          |
+                             +----------------------------------------------------+
|                             | :cs:`CS_STARTUP_TIMEOUT`                           |
+                             +----------------------------------------------------+
|                             | :cs:`CS_OSCS_CHECKSUM_STATE`                       |
+                             +----------------------------------------------------+
|                             | :cs:`CS_CFECORE_CHECKSUM_STATE`                    |
+                             +----------------------------------------------------+
|                             | :cs:`CS_EEPROM_TBL_POWERON_STATE`                  |
+                             +----------------------------------------------------+
|                             | :cs:`CS_MEMORY_TBL_POWERON_STATE`                  |
+                             +----------------------------------------------------+
|                             | :cs:`CS_APPS_TBL_POWERON_STATE`                    |
+                             +----------------------------------------------------+
|                             | :cs:`CS_TABLES_TBL_POWERON_STATE`                  |
+                             +----------------------------------------------------+
|                             | :cs:`CS_PRESERVE_STATES_ON_PROCESSOR_RESET`        |
+                             +----------------------------------------------------+
|                             | :cs:`CS_CDS_NAME`                                  |
+                             +----------------------------------------------------+
|                             | :cs:`CS_MISSION_REV`                               |
+-----------------------------+----------------------------------------------------+

.. note::
   The message IDs defined in the :cs:`cs_msgids.h` file must have values
   that are unique system wide.  Most applications also have message IDs.
   No two message IDs may have the same value.

Table(s)
^^^^^^^^^^^^^^^^

TODO REVIEW TABLE

+-------------------------------+------------------------------------+-------------------------------------------------------------+
| Table Name                    | Default file name                  | Parameter                                                   |
+===============================+====================================+=============================================================+
| :cs:`CS_DEF_APP_TABLE_NAME`   | :cs:`CS_DEF_APP_TABLE_FILENAME`    | :cs:`CS_Def_App_Table_Entry_t::State`                       |
+                               |                                    +-------------------------------------------------------------+
|                               |                                    | :cs:`CS_Def_App_Table_Entry_t::Name`                        |
+-------------------------------+------------------------------------+-------------------------------------------------------------+
| :cs:`CS_DEF_TABLES_TABLE_NAME`| :cs:`CS_DEF_TABLES_TABLE_FILENAME` | :cs:`CS_Def_Tables_Table_Entry_t::State`                    |
+                               |                                    +-------------------------------------------------------------+
|                               |                                    | :cs:`CS_Def_Tables_Table_Entry_t::Name`                     |
+-------------------------------+------------------------------------+-------------------------------------------------------------+
| :cs:`CS_DEF_MEMORY_TABLE_NAME`| :cs:`CS_DEF_MEMORY_TABLE_FILENAME` | :cs:`CS_Def_EepromMemory_Table_Entry_t::State`              |
+                               |                                    +-------------------------------------------------------------+
|                               |                                    | :cs:`CS_Def_EepromMemory_Table_Entry_t::Filler16`           |
+                               |                                    +-------------------------------------------------------------+
|                               |                                    | :cs:`CS_Def_EepromMemory_Table_Entry_t::StartAddress`       |
+                               |                                    +-------------------------------------------------------------+
|                               |                                    | :cs:`CS_Def_EepromMemory_Table_Entry_t::NumBytesToChecksum` |
+-------------------------------+------------------------------------+-------------------------------------------------------------+
| :cs:`CS_DEF_EEPROM_TABLE_NAME`| :cs:`CS_DEF_EEPROM_TABLE_FILENAME` | :cs:`CS_Def_EepromMemory_Table_Entry_t::State`              |
+                               |                                    +-------------------------------------------------------------+
|                               |                                    | :cs:`CS_Def_EepromMemory_Table_Entry_t::Filler16`           |
+                               |                                    +-------------------------------------------------------------+
|                               |                                    | :cs:`CS_Def_EepromMemory_Table_Entry_t::StartAddress`       |
+                               |                                    +-------------------------------------------------------------+
|                               |                                    | :cs:`CS_Def_EepromMemory_Table_Entry_t::NumBytesToChecksum` |
+-------------------------------+------------------------------------+-------------------------------------------------------------+


.. literalinclude:: ../fsw/tables/cs_apptbl.c
  :language: C
  :caption: Application Definition Table


.. literalinclude:: ../fsw/tables/cs_tablestbl.c
  :language: C
  :caption: Table Definition Table


.. literalinclude:: ../fsw/tables/cs_memorytbl.c
  :language: C
  :caption: Memory Definition Table


.. literalinclude:: ../fsw/tables/cs_eepromtbl.c
  :language: C
  :caption: EEPROM Definition Table


System Integration
^^^^^^^^^^^^^^^^^^

TODO: Describe System Integration instructions specific to CS.


Run Time Configuration
^^^^^^^^^^^^^^^^^^^^^^

TODO: Describe Run Time Configuration for CS.
