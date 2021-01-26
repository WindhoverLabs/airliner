File Manager
====================================

The :term:`FM` application provides onboard file system management services by processing ground commands for copying, moving, and renaming files, decompressing files, creating directories, deleting files and directories, providing file and directory informational telemetry messages, and providing open file and directory listings. The FM software context diagram is shown in Figure FM-1.
CFS_FM_Context.jpg
Figure FM-1


The FM application interfaces with the Command Ingest (CI) application to receive ground commands. See page CFS File Manager Commands for a detailed description of the FM ground commands. FM utilizes the Operating System Abstraction Layer (OSAL) API library functions for interfacing with the onboard file systems when processing ground commands. The FM application interfaces with the Scheduler (SCH) Application for receiving periodic housekeeping requests. Upon receipt of a housekeeping request, FM will send its housekeeping telemetry packets on the Software Bus. Housekeeping telemetry packets are subscribed to by the Housekeeping (HK), Telemetry Output (TO), and Data Storage applications. See page CFS File Manager Telemetry for a detailed description of the contents of the FM housekeeping telemetry packet.



.. toctree::
   :caption: Contents
   :name: fm_contents

   Issues and Limitations <docs/limitations>
   Design <docs/design>
   Activities <docs/activities>
   Configuration <docs/configuration>
   Concept of Operations <docs/conops>
   FAQ <docs/faq>

Detailed design documentation is available `here <../../doxy/apps/fm/index.html>`_.

.. image:: /docs/_static/doxygen.png
   :target: ../../doxy/apps/fm/index.html
