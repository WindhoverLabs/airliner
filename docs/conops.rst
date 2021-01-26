Concept of Operations
=====================

TODO: Add Commander screenshots, with a legend for each telemetry item and command button.

TODO: Add activation procedure.

TODO: Add tuning and reconfiguration procedures.

TODO: List potential failure modes.

TODO: Add common troubleshooting procedures.

The operational interface of the MD application consists of ground commands, dwell table loads, and housekeeping telemetry. The following items detail the operational interface:

   1. The application version number - The application version number is displayed in the Initialization event message and the No-op event message. Both events are 'informational' type and are unfiltered by default.

   2. Loading a Dwell Table - Memory Dwell supports a configurable number of dwell tables. Each of these dwell tables can be loaded by way of a file. A new dwell table may be loaded at any time. The steps to load a new Copy Table are:

    a) Transfer the table file to the on-board file system
    b) Send the CFE Table load command
    c) Send the CFE Table validate command
    d) Send the CFE Table activate command

   3. Sending the No-op Command - To verify connectivity with the Memory Dwell application, the ground may send an MD No-op command . If the packet length field in the command is set to the value expected by the MD app, then the command counter will increment and a No-op informational event message will be sent. This no-op event will show the version number of the MD application.

   4. Sending the reset counters command - The reset counters command will reset only the telemetry points listed here:

    a) $sc_$cpu_MD_CMDPC - Number of accepted commands
    b) $sc_$cpu_MD_CMDEC - Number of rejected commands

   5. Monitoring the command counter - The command counter, MD_CMDPC, will increment when the MD application receives any valid command.

   6. Monitoring the command error counter - The command error counter will increment under the following conditions:

    a) Invalid command code
    b) Unexpected packet length field for 'MD Wakeup' command
    c) Unexpected packet length field for 'Send MD Housekeeping' command
    d) Unexpected packet length field for a ground command
    e) Invalid command argument

   7. Enabling and Disabling Dwell Packet Streams - The enabled/disabled state of dwell packet streams can be modified using the start and stop ground commands. The enabled/disabled state is also controlled by the enabled flag in the dwell load table.Note that a dwell packet stream will not actively operate if the total delay count is zero, no matter what the enabled/disabled state is.

   8.  Specifying a memory dwell stream - Each Memory Dwell Table consists of MD_DWELL_TABLE_SIZE entries. Each entry serves either as an individual dwell definition or a null entry. The MD will build packets beginning with the dwell specified in the first slot, adding additional dwells until it reaches either a null entry or the end of the table. Each individual memory dwell definition is specified by a memory address, the number of delay counts before the next read, and the dwell length. The number of delay counts is the number of wakeup calls the application will receive before the next memory dwell read. A valid Dwell Table entry which is an individual memory dwell definition must have:

    a) a valid memory address (Symbol name, if any, must be in operating system Symbol Table, and resolved address i.e. symbol plus offset must be in a valid range),
    b) a value of 1, 2, or 4 for dwell length.

    A valid Dwell Table entry which is a null entry must have a zero value for dwell length.

   
Detailed concept of operations documentation is available `here <../../doxy/apps/md/cfsmdopr.html>`_.

.. image:: /docs/_static/doxygen.png
   :target: ../../doxy/apps/md/index.html