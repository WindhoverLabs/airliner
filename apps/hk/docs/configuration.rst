Configuration
=============

TODO

Mission Specific
^^^^^^^^^^^^^^^^

+----------------------------+-------------------------------------+
| File                       | Parameter                           |
+============================+=====================================+
| :hk:`hk_perfids.h`         | :hk:`HK_APPMAIN_PERF_ID`            |
+----------------------------+-------------------------------------+

.. note::
   The performance IDs defined in the :hk:`hk_perfids.h` file must have values
   that are unique system wide.  Most applications also have performance IDs.
   No two performance IDs may have the same value.
   

Platform Specific
^^^^^^^^^^^^^^^^^

+-----------------------------+---------------------------------------------+
| File                        | Parameter                                   |
+=============================+=============================================+
| :hk:`hk_msgids.h`           | :hk:`HK_CMD_MID`                            |
+                             +---------------------------------------------+
|                             | :hk:`HK_SEND_HK_MID`                        |
+                             +---------------------------------------------+
|                             | :hk:`HK_SEND_COMBINED_PKT_MID`              |
+                             +---------------------------------------------+
|                             | :hk:`HK_HK_TLM_MID`                         |
+                             +---------------------------------------------+
|                             | :hk:`HK_COMBINED_PKT1_MID`                  |
+                             +---------------------------------------------+
|                             | :hk:`HK_COMBINED_PKT2_MID`                  |
+                             +---------------------------------------------+
|                             | :hk:`HK_COMBINED_PKT3_MID`                  |
+                             +---------------------------------------------+
|                             | :hk:`HK_COMBINED_PKT4_MID`                  |
+-----------------------------+---------------------------------------------+
| :hk:`hk_platform_cfg.h`     | :hk:`HK_PIPE_DEPTH`                         |
+                             +---------------------------------------------+
|                             | :hk:`HK_DISCARD_INCOMPLETE_COMBO`           |
+                             +---------------------------------------------+
|                             | :hk:`HK_COPY_TABLE_ENTRIES`                 |
+                             +---------------------------------------------+
|                             | :hk:`HK_NUM_BYTES_IN_MEM_POOL`              |
+                             +---------------------------------------------+
|                             | :hk:`HK_COPY_TABLE_NAME`                    |
+                             +---------------------------------------------+
|                             | :hk:`HK_RUNTIME_TABLE_NAME`                 |
+                             +---------------------------------------------+
|                             | :hk:`HK_COPY_TABLE_FILENAME`                |
+                             +---------------------------------------------+
|                             | :hk:`HK_MISSION_REV`                        |
+-----------------------------+---------------------------------------------+

.. note::
   The message IDs defined in the :hk:`hk_msgids.h` file must have values
   that are unique system wide.  Most applications also have message IDs.
   No two message IDs may have the same value.


Table(s)
^^^^^^^^^^^^^^^^

TODO REVIEW TABLE

+-------------------------------+------------------------------------+-------------------------------------------------+
| Table Name                    | Default file name                  | Parameter                                       |
+===============================+====================================+=================================================+
| :hk:`HK_COPY_TABLE_NAME`      | :hk:`HK_COPY_TABLE_FILENAME`       | :hk:`hk_copy_table_entry_t::InputMid`           |
+                               |                                    +-------------------------------------------------+
|                               |                                    | :hk:`hk_copy_table_entry_t::InputOffset`        |
+                               |                                    +-------------------------------------------------+
|                               |                                    | :hk:`hk_copy_table_entry_t::OutputMid`          |
+                               |                                    +-------------------------------------------------+
|                               |                                    | :hk:`hk_copy_table_entry_t::OutputOffset`       |
+                               |                                    +-------------------------------------------------+
|                               |                                    | :hk:`hk_copy_table_entry_t::NumBytes`           |
+                               |                                    +-------------------------------------------------+
|                               |                                    | :hk:`hk_runtime_tbl_entry_t::OutputPktAddr`     |
+                               |                                    +-------------------------------------------------+
|                               |                                    | :hk:`hk_runtime_tbl_entry_t::InputMidSubscribed`|
+                               |                                    +-------------------------------------------------+
|                               |                                    | :hk:`hk_runtime_tbl_entry_t::DataPresent`       |
+-------------------------------+------------------------------------+-------------------------------------------------+

Below is an example table.

.. literalinclude:: ../fsw/tables/hk_cpy_tbl.c
  :language: C
  :caption: Application Copy Table


System Integration
^^^^^^^^^^^^^^^^^^

TODO: Describe System Integration instructions specific to HK.


Run Time Configuration
^^^^^^^^^^^^^^^^^^^^^^

TODO: Describe Run Time Configuration for HK.
























