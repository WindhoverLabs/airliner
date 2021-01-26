Configuration
=============

Mission Specific
^^^^^^^^^^^^^^^^
TODO: The following do not exist in doxy.

+-------------------------+---------------------------------+
| File                    | Parameter                       |
+=========================+=================================+
| :mm:`mm_mission_cfg.h`  | :mm:`MM_LOAD_WID_CRC_TYPE`      |
+                         +---------------------------------+
|                         | :mm:`MM_LOAD_FILE_CRC_TYPE`     |
+                         +---------------------------------+
|                         | :mm:`MM_DUMP_FILE_CRC_TYPE`     |
+-------------------------+---------------------------------+
| :mm:`mm_perfids.h`      | :mm:`MM_APPMAIN_PERF_ID`        |
+                         +---------------------------------+
|                         | :mm:`MM_SEGBREAK_PERF_ID`       |
+                         +---------------------------------+
|                         | :mm:`MM_EEPROM_POKE_PERF_ID`    |
+                         +---------------------------------+
|                         | :mm:`MM_EEPROM_FILELOAD_PERF_ID`|
+                         +---------------------------------+
|                         | :mm:`MM_EEPROM_FILL_PERF_ID`    |
+-------------------------+---------------------------------+


.. note::
   The performance IDs defined in the :md:`md_perfids.h` file must have values
   that are unique system wide.  Most applications also have performance IDs.
   No two performance IDs may have the same value.
   

Platform Specific
^^^^^^^^^^^^^^^^^

+---------------------------+-------------------------------------+
| File                      | Parameter                           |
+===========================+=====================================+
| :mm:`mm_msgids.h`         | :mm:`MM_CMD_MID`                    |
|                           +-------------------------------------+
|                           | :mm:`MM_SEND_HK_MID`                |
+                           +-------------------------------------+
|                           | :mm:`MM_HK_TLM_MID`                 |
+---------------------------+-------------------------------------+
| :mm:`mm_platform_cfg.h`   | :mm:`MM_CFE_HDR_SUBTYPE`            |
+                           +-------------------------------------+
|                           | :mm:`MM_CFE_HDR_DESCRIPTION`        |
+                           +-------------------------------------+
|                           | :mm:`MM_MAX_LOAD_FILE_DATA_RAM`     |
+                           +-------------------------------------+
|                           | :mm:`MM_MAX_LOAD_FILE_DATA_EEPROM`  |
+                           +-------------------------------------+
|                           | :mm:`MM_MAX_UNINTERRUPTABLE_DATA`   |
+                           +-------------------------------------+
|                           | :mm:`MM_MAX_LOAD_DATA_SEG`          |
+                           +-------------------------------------+
|                           | :mm:`MM_MAX_DUMP_FILE_DATA_RAM`     |
+                           +-------------------------------------+
|                           | :mm:`MM_MAX_DUMP_FILE_DATA_EEPROM`  |
+                           +-------------------------------------+
|                           | :mm:`MM_MAX_DUMP_FILE_DATA_SYMTBL`  |
+                           +-------------------------------------+
|                           | :mm:`MM_MAX_DUMP_DATA_SEG`          |
+                           +-------------------------------------+
|                           | :mm:`MM_MAX_FILL_DATA_RAM`          |
+                           +-------------------------------------+
|                           | :mm:`MM_MAX_FILL_DATA_EEPROM`       |
+                           +-------------------------------------+
|                           | :mm:`MM_MAX_FILL_DATA_SEG`          |
+                           +-------------------------------------+
|                           | :mm:`MM_OPT_CODE_MEM32_MEMTYPE`     |
+                           +-------------------------------------+
|                           | :mm:`MM_MAX_LOAD_FILE_DATA_MEM32`   |
+                           +-------------------------------------+
|                           | :mm:`MM_MAX_DUMP_FILE_DATA_MEM32`   |
+                           +-------------------------------------+
|                           | :mm:`MM_MAX_FILL_DATA_MEM32`        |
+                           +-------------------------------------+
|                           | :mm:`MM_OPT_CODE_MEM16_MEMTYPE`     |
+                           +-------------------------------------+
|                           | :mm:`MM_MAX_LOAD_FILE_DATA_MEM16`   |
+                           +-------------------------------------+
|                           | :mm:`MM_MAX_DUMP_FILE_DATA_MEM16`   |
+                           +-------------------------------------+
|                           | :mm:`MM_MAX_FILL_DATA_MEM16`        |
+                           +-------------------------------------+
|                           | :mm:`MM_OPT_CODE_MEM8_MEMTYPE`      |
+                           +-------------------------------------+
|                           | :mm:`MM_MAX_LOAD_FILE_DATA_MEM8`    |
+                           +-------------------------------------+
|                           | :mm:`MM_MAX_DUMP_FILE_DATA_MEM8`    |
+                           +-------------------------------------+
|                           | :mm:`MM_MAX_FILL_DATA_MEM8`         |
+                           +-------------------------------------+
|                           | :mm:`MM_PROCESSOR_CYCLE`            |
+                           +-------------------------------------+
|                           | :mm:`MM_MISSION_REV`                |
+---------------------------+-------------------------------------+

.. note::
   The message IDs defined in the :mm:`mm_msgids.h` file must have values
   that are unique system wide.  Most applications also have message IDs.
   No two message IDs may have the same value.
   
TODO: Describe the "custom" layer code.


Table(s)
^^^^^^^^
TODO: Confiirm that no tables exit for this entry.






System Integration
^^^^^^^^^^^^^^^^^^




Run Time Configuration
^^^^^^^^^^^^^^^^^^^^^^


TODO: Add references.
