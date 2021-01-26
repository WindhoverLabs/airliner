Configuration
=============

Mission Specific
^^^^^^^^^^^^^^^^
TODO: The following do not exist in doxy.

+-------------------------+--------------------------------+
| File                    | Parameter                      |
+=========================+================================+
| :md:`md_perfids.h`      | :md:`MD_APPMAIN_PERF_ID`       |
+-------------------------+--------------------------------+


.. note::
   The performance IDs defined in the :md:`md_perfids.h` file must have values
   that are unique system wide.  Most applications also have performance IDs.
   No two performance IDs may have the same value.
   

Platform Specific
^^^^^^^^^^^^^^^^^

+---------------------------+-------------------------------------+
| File                      | Parameter                           |
+===========================+=====================================+
| :md:`md_msgids.h`         | :md:`MD_CMD_MID`                    |
|                           +-------------------------------------+
|                           | :md:`MD_SEND_HK_MID`                |
+                           +-------------------------------------+
|                           | :md:`MD_DWELL_PKT_MID_BASE`         |
+                           +-------------------------------------+
|                           | :md:`MD_WAKEUP_MID`                 |
+                           +-------------------------------------+
|                           | :md:`MD_HK_TLM_MID`                 |
+---------------------------+-------------------------------------+
| :md:`md_platform_cfg.h`   | :md:`MD_PIPE_NAME`                  |
+                           +-------------------------------------+
|                           | :md:`MD_PIPE_DEPTH`                 |
+                           +-------------------------------------+
|                           | :md:`MD_TBL_FILENAME_FORMAT`        |
+                           +-------------------------------------+
|                           | :md:`MD_NUM_DWELL_TABLES`           |
+                           +-------------------------------------+
|                           | :md:`MD_DWELL_TABLE_SIZE`           |
+                           +-------------------------------------+
|                           | :md:`MD_ENFORCE_DWORD_ALIGN`        |
+                           +-------------------------------------+
|                           | :md:`MD_SIGNATURE_OPTION`           |
+                           +-------------------------------------+
|                           | :md:`MD_SIGNATURE_FIELD_LENGTH`     |
+                           +-------------------------------------+
|                           | :md:`MD_MISSION_REV`                |
+---------------------------+-------------------------------------+

.. note::
   The message IDs defined in the :md:`md_msgids.h` file must have values
   that are unique system wide.  Most applications also have message IDs.
   No two message IDs may have the same value.


Table(s)
^^^^^^^^
TODO: Complicated table structure. Review Table Names and File Names.

+-------------------------------+------------------------------------+------------------------------------------+
| Table Name                    | Default file name                  | Parameter                                |
+===============================+====================================+==========================================+
| :md:`MD_ENTRY_TABLENAME`      | :md:`MD_ENTRY_TABLE_FILENAME`      | :md:`MD_TableLoadEntry_t::Length`        |
+                               |                                    +------------------------------------------+
|                               |                                    | :md:`MD_TableLoadEntry_t::Delay`         |
+                               |                                    +------------------------------------------+
|                               |                                    | :md:`MD_TableLoadEntry_t::DwellAddress`  |
+-------------------------------+------------------------------------+------------------------------------------+
| :md:`MD_DWELL_TABLENAME`      | :md:`MD_DWELL_TABLE_FILENAME`      | :md:`MD_DwellTableLoad_t::Enabled`       |
+                               |                                    +------------------------------------------+
|                               |                                    | :md:`MD_DwellTableLoad_t::Signature`     |
+                               |                                    +------------------------------------------+
|                               |                                    | :md:`MD_DwellTableLoad_t::Entry`         |
+-------------------------------+------------------------------------+------------------------------------------+

   
Below are example tables.

.. literalinclude:: ../fsw/tables/md_dw01.c
  :language: C
  :caption: Example Dwell Table #1

.. literalinclude:: ../fsw/tables/md_dw02.c
  :language: C
  :caption: Example Dwell Table #2

.. literalinclude:: ../fsw/tables/md_dw03.c
  :language: C
  :caption: Example Dwell Table #3

.. literalinclude:: ../fsw/tables/md_dw04.c
  :language: C
  :caption: Example Dwell Table #4




System Integration
^^^^^^^^^^^^^^^^^^

TODO


Run Time Configuration
^^^^^^^^^^^^^^^^^^^^^^


TODO: Add references.
