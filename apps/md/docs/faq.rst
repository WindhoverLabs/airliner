FAQ
================

TODO
Questions about CFE Table Services Interface

(Q) If a dwell stream is modified or defined using jam commands, will those changes and definition be copied to the buffer maintained by Table Services?

Yes.

(Q) Are changes to dwell tables preserved across processor resets?

Dwell table contents that are introduced via table loads are preserved across processor resets. Information modified via jam commands, start commands, and stop commands are also preserved across processor resets.
Questions about Command and Telemetry

(Q) Which MD telemetry can be used to confirm that dwell packet processing is proceeding?

The $sc_$cpu_MD_CountDown[MD_NUM_DWELL_TABLES] telemetry point should change each time a Wakeup Message is received for all dwell streams that are being actively processed.

(Q) What needs to be in place for dwell packets to be issued?

1) The dwell table's entry slot must contain a valid entry definition, i.e. a valid memory address and field length.
2) The total delay, defined as the total of all individual entry delays from the first slot until the last slot before the terminal entry, must be 1 or more.
3) The dwell table must be enabled.

(Q) How long will it be before the first dwell packet is issued?

The first dwell always takes place when the first wakeup message is received following a Start command . Dwelling proceeds until the first entry with a non-zero delay is reached. Thus, anytime a start command is sent for a dwell table that does not have non-zero delays for intermediate entries, a dwell packet will be issued immediately. So, for example, a dwell table that is set up to sample several addresses every 10 cycles, will actually issue dwell packets following the 1st, 11th, and 21st wakeup messages.

(Q) What happens if a dwell tream is Started , Stopped , and then Started again?

Whenever a start command is received, whether or not there is an intervening stop command, the dwell packet processing starts over. The countdown timer is reinitialized, and the dwelling begins again with the first table entry.

(Q) What if an operating system does not support referencing memory addresses by symbolic names?

The symbol name portion of the memory address in a table load or Jam command must be set to a null string.
Questions about Specifying Memory Dwell Streams

(Q) What confirmation is available for changes made using a Jam command ?

1) An information event, MD_JAM_DWELL_INF_EID, will be issued showing the table number and entry number affected.
2) The following housekeeping parameters for the corresponding dwell table may be affected:
$sc_$cpu_MD_AddrCnt[MD_NUM_DWELL_TABLES],
$sc_$cpu_MD_DwlPkt1Rate, $sc_$cpu_MD_DwlPkt2Rate, $sc_$cpu_MD_DwlPkt3Rate, $sc_$cpu_MD_DwlPkt4Rate,
$sc_$cpu_MD_DataSize[MD_NUM_DWELL_TABLES].

(Q) Does MD support supercommutation? What is it? How do you set up a dwell table to achieve supercommutation?

Yes it does. Supercommutation is the insertion of multiple instances of the same dwell address in the same packet. To define a dwell table for supercommutation, associate a non-zero delay multiple for each instance of the dwell address in the table.

(Q) How large is a dwell table?

The size of a dwell table depends on:

1) the size of a signature field,
2) the size of symbolic addresses if enabled,
3) the number of dwell tables which have been configured,
4) the number of entries in a dwell table.

It is also possible that a compiler may insert padding between fields for alignment purposes, which will add to the size. If #MD_DEBUG is set to a non-zero value, the size of the dwell table will be issued via OS_printf on initialization.

(Q) What is the fastest rate at which MD can sample memory addresses?

The fastest that MD can sample memory address is the rate at which Wakeup messages are received. To achieve this fastest rate, the dwell table must be defined to have a total delay multiple of 1.

(Q) How would a simple dwell table be defined to sample several memory addresses at the same time?

For each memory address to be sampled, define a dwell entry beginning with the first entry, entry #1. Enter the dwell field length value of 1, 2, or 4. Note that if the desired memory region is larger than 4, it must be broken down into consecutive dwell entries. Enter the memory address using the numerical value, a symbolic value, or a symbolic value and numerical offset combination. In each of the entries that are not the last dwell, enter a zero value for the delay. For the last dwell entry, enter a value corresponding to the multiple of the wakeup call rate, which is the desired issue rate for the dwell packets. If the last dwell entry is not the last entry in the table, then the next entry must be a "null entry" with a zero value for the field length to signify that this is the end of the dwell packet.

(Q) Does MD accept tables with non-null entries past a terminator entry?

Yes, a non-null entry is accepted past a terminator entry as long as that entry itself is valid. To be valid, the entry must have valid values for dwell address, and field length. 

Detailed FAQ documentation is available `here <../../doxy/apps/md/cfsmdopr.html>`_.

.. image:: /docs/_static/doxygen.png
   :target: ../../doxy/apps/md/index.html