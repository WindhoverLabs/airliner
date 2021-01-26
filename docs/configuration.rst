Configuration
=============

TODO

Mission Specific
^^^^^^^^^^^^^^^^

+----------------------------+-------------------------------------+
| File                       | Parameter                           |
+============================+=====================================+
| :fm:`fm_perfids.h`         | :fm:`FM_APPMAIN_PERF_ID`            |
+                            +-------------------------------------+
|                            | :fm:`FM_CHILD_TASK_PERF_ID`         |
+----------------------------+-------------------------------------+

.. note::
   The performance IDs defined in the :fm:`fm_perfids.h` file must have values
   that are unique system wide.  Most applications also have performance IDs.
   No two performance IDs may have the same value.
   

Platform Specific
^^^^^^^^^^^^^^^^^

+-----------------------------+---------------------------------------------+
| File                        | Parameter                                   |
+=============================+=============================================+
| :fm:`fm_msgids.h`           | :fm:`FM_CMD_MID`                            |
+                             +---------------------------------------------+
|                             | :fm:`FM_SEND_HK_MID`                        |
+                             +---------------------------------------------+
|                             | :fm:`FM_HK_TLM_MID`                         |
+                             +---------------------------------------------+
|                             | :fm:`FM_FILE_INFO_TLM_MID`                  |
+                             +---------------------------------------------+
|                             | :fm:`FM_DIR_LIST_TLM_MID`                   |
+                             +---------------------------------------------+
|                             | :fm:`FM_OPEN_FILES_TLM_MID`                 |
+                             +---------------------------------------------+
|                             | :fm:`FM_FREE_SPACE_TLM_MID`                 |
+-----------------------------+---------------------------------------------+
| :fm:`fm_platform_cfg.h`     | :fm:`FM_CHILD_TASK_FLAGS`                   |
+                             +---------------------------------------------+
|                             | :fm:`FM_APP_NAME`                           |
+                             +---------------------------------------------+
|                             | :fm:`FM_APP_PIPE_NAME`                      |
+                             +---------------------------------------------+
|                             | :fm:`FM_APP_PIPE_DEPTH`                     |
+                             +---------------------------------------------+
|                             | :fm:`FM_MISSION_REV`                        |
+                             +---------------------------------------------+
|                             | :fm:`FM_DIR_LIST_FILE_DEFNAME`              |
+                             +---------------------------------------------+
|                             | :fm:`FM_DIR_LIST_FILE_ENTRIES`              |
+                             +---------------------------------------------+
|                             | :fm:`FM_DIR_LIST_FILE_SUBTYPE`              |
+                             +---------------------------------------------+
|                             | :fm:`FM_DIR_LIST_PKT_ENTRIES`               |
+                             +---------------------------------------------+
|                             | :fm:`FM_CHILD_FILE_BLOCK_SIZE`              |
+                             +---------------------------------------------+
|                             | :fm:`FM_CHILD_FILE_LOOP_COUNT`              |
+                             +---------------------------------------------+
|                             | :fm:`FM_CHILD_FILE_SLEEP_MS`                |
+                             +---------------------------------------------+
|                             | :fm:`FM_CHILD_STAT_SLEEP_FILECOUNT`         |
+                             +---------------------------------------------+
|                             | :fm:`FM_CHILD_QUEUE_DEPTH`                  |
+                             +---------------------------------------------+
|                             | :fm:`FM_CHILD_TASK_NAME`                    |
+                             +---------------------------------------------+
|                             | :fm:`FM_CHILD_TASK_STACK_SIZE`              |
+                             +---------------------------------------------+
|                             | :fm:`FM_CHILD_TASK_PRIORITY`                |
+                             +---------------------------------------------+
|                             | :fm:`FM_CHILD_SEM_NAME`                     |
+                             +---------------------------------------------+
|                             | :fm:`FM_TABLE_CFE_NAME`                     |
+                             +---------------------------------------------+
|                             | :fm:`FM_TABLE_DEF_NAME`                     |
+                             +---------------------------------------------+
|                             | :fm:`FM_TABLE_FILENAME`                     |
+                             +---------------------------------------------+
|                             | :fm:`FM_TABLE_DEF_DESC`                     |
+                             +---------------------------------------------+
|                             | :fm:`FM_TABLE_ENTRY_COUNT`                  |
+                             +---------------------------------------------+
|                             | :fm:`FM_TABLE_VALIDATION_ERR`               |
+-----------------------------+---------------------------------------------+

.. note::
   The message IDs defined in the :fm:`fm_msgids.h` file must have values
   that are unique system wide.  Most applications also have message IDs.
   No two message IDs may have the same value.

Table(s)
^^^^^^^^^^^^^^^^

TODO REVIEW TABLE: No tbldefs file exists, grabbed information from fm_freespace.c

+-------------------------------+------------------------------------+---------------------------------------------------+
| Table Name                    | Default file name                  | Parameter                                         |
+===============================+====================================+===================================================+
| :fm:`FM_TABLE_NAME`           | :fm:`FM_TABLE_FILENAME`            | :fm:`FM_FreeSpaceTable_t::ram`                    |
+                               |                                    +---------------------------------------------------+
|                               |                                    | :fm:`FM_FreeSpaceTable_t::boot`                   |
+                               |                                    +---------------------------------------------------+
|                               |                                    | :fm:`FM_FreeSpaceTable_t::alt`                    |
+-------------------------------+------------------------------------+---------------------------------------------------+


Below is an example table.

.. literalinclude:: ../fsw/tables/fm_freespace.c
  :language: C
  :caption: Application Free Space Table


System Integration
^^^^^^^^^^^^^^^^^^

TODO: Describe System Integration instructions specific to FM.


Run Time Configuration
^^^^^^^^^^^^^^^^^^^^^^

TODO: Describe Run Time Configuration for FM.
