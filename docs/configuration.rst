Configuration
=============

TODO

Mission Specific
^^^^^^^^^^^^^^^^

+----------------------------+-------------------------------------+
| File                       | Parameter                           |
+============================+=====================================+
| :ds:`ds_perfids.h`         | :ds:`DS_APPMAIN_PERF_ID`            |
+----------------------------+-------------------------------------+

.. note::
   The performance IDs defined in the :ds:`ds_perfids.h` file must have values
   that are unique system wide.  Most applications also have performance IDs.
   No two performance IDs may have the same value.
   

Platform Specific
^^^^^^^^^^^^^^^^^

+-----------------------------+---------------------------------------------+
| File                        | Parameter                                   |
+=============================+=============================================+
| :ds:`ds_msgids.h`           | :ds:`DS_CMD_MID`                            |
+                             +---------------------------------------------+
|                             | :ds:`DS_SEND_HK_MID`                        |
+                             +---------------------------------------------+
|                             | :ds:`DS_HK_TLM_MID`                         |
+                             +---------------------------------------------+
|                             | :ds:`DS_DIAG_TLM_MID`                       |
+-----------------------------+---------------------------------------------+
| :ds:`ds_platform_cfg.h`     | :ds:`DS_DESTINATION_TBL_NAME`               |
+                             +---------------------------------------------+
|                             | :ds:`DS_DEF_DEST_FILENAME`                  |
+                             +---------------------------------------------+
|                             | :ds:`DS_DEST_FILE_CNT`                      |
+                             +---------------------------------------------+
|                             | :ds:`DS_PATHNAME_BUFSIZE`                   |
+                             +---------------------------------------------+
|                             | :ds:`DS_BASENAME_BUFSIZE`                   |
+                             +---------------------------------------------+
|                             | :ds:`DS_EXTENSION_BUFSIZE`                  |
+                             +---------------------------------------------+
|                             | :ds:`DS_FILTER_TBL_NAME`                    |
+                             +---------------------------------------------+
|                             | :ds:`DS_DEF_FILTER_FILENAME`                |
+                             +---------------------------------------------+
|                             | :ds:`DS_PACKETS_IN_FILTER_TABLE`            |
+                             +---------------------------------------------+
|                             | :ds:`DS_FILTERS_PER_PACKET`                 |
+                             +---------------------------------------------+
|                             | :ds:`DS_DESCRIPTOR_BUFSIZE`                 |
+                             +---------------------------------------------+
|                             | :ds:`DS_SEQUENCE_DIGITS`                    |
+                             +---------------------------------------------+
|                             | :ds:`DS_MAX_SEQUENCE_COUNT`                 |
+                             +---------------------------------------------+
|                             | :ds:`DS_TOTAL_FNAME_BUFSIZE`                |
+                             +---------------------------------------------+
|                             | :ds:`DS_FILE_HDR_SUBTYPE`                   |
+                             +---------------------------------------------+
|                             | :ds:`DS_FILE_HDR_DESCRIPTION`               |
+                             +---------------------------------------------+
|                             | :ds:`DS_FILE_MIN_SIZE_LIMIT`                |
+                             +---------------------------------------------+
|                             | :ds:`DS_FILE_MIN_AGE_LIMIT`                 |
+                             +---------------------------------------------+
|                             | :ds:`DS_APP_PIPE_NAME`                      |
+                             +---------------------------------------------+
|                             | :ds:`DS_APP_PIPE_DEPTH`                     |
+                             +---------------------------------------------+
|                             | :ds:`DS_MAKE_TABLES_CRITICAL`               |
+                             +---------------------------------------------+
|                             | :ds:`DS_SECS_PER_HK_CYCLE`                  |
+                             +---------------------------------------------+
|                             | :ds:`DS_DEF_ENABLE_STATE`                   |
+                             +---------------------------------------------+
|                             | :ds:`DS_CDS_ENABLE_STATE`                   |
+                             +---------------------------------------------+
|                             | :ds:`DS_MISSION_REV`                        |
+                             +---------------------------------------------+
|                             | :ds:`DS_FILE_HEADER_TYPE`                   |
+                             +---------------------------------------------+
|                             | :ds:`DS_MOVE_FILES`                         |
+                             +---------------------------------------------+
|                             | :ds:`DS_PER_PACKET_PIPE_LIMIT`              |
+-----------------------------+---------------------------------------------+

.. note::
   The message IDs defined in the :ds:`ds_msgids.h` file must have values
   that are unique system wide.  Most applications also have message IDs.
   No two message IDs may have the same value.


Table(s)
^^^^^^^^^^^^^^^^

TODO

+-------------------------------+------------------------------------+------------------------------------------+
| Table Name                    | Default file name                  | Parameter                                |
+===============================+====================================+==========================================+
| :ds:`DS_DESTINATION_TBL_NAME` | :ds:`DS_DEF_DEST_FILENAME`         | :ds:`DS_DestFileTable_t::Descriptor`     |
+                               |                                    +------------------------------------------+
|                               |                                    | :ds:`DS_DestFileTable_t::File`           |
+-------------------------------+------------------------------------+------------------------------------------+
| :ds:`DS_FILTER_TBL_NAME`      | :ds:`DS_DEF_FILTER_FILENAME`       | :ds:`DS_FilterTable_t::Descriptor`       |
+                               |                                    +------------------------------------------+
|                               |                                    | :ds:`DS_FilterTable_t::Packet`           |
+-------------------------------+------------------------------------+------------------------------------------+

.. literalinclude:: ../fsw/tables/ds_file_tbl.c
  :language: C
  :caption: Nominal File Table


.. literalinclude:: ../fsw/tables/ds_filter_tbl.c
  :language: C
  :caption: Nominal Filter Table


System Integration
^^^^^^^^^^^^^^^^^^

TODO: Describe System Integration instructions specific to DS.


Run Time Configuration
^^^^^^^^^^^^^^^^^^^^^^

TODO: Describe Run Time Configuration for DS.

















