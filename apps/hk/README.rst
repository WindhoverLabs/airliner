Housekeeping
====================================

The :term:`HK` Housekeeping (HK) component of the Core Flight System (CFS) is responsible for building and sending combined telemetry messages (from individual system applications) to the software bus for routing. Combining messages is performed in order to minimize downlink telemetry bandwidth. Combining certain data from multiple messages into one message eliminates the message headers that would be required if each message was sent individually. Combined messages are also useful for organizing certain types of data. This application may be used for data types other than housekeeping telemetry. HK provides the capability to generate multiple combined packets (a.k.a. output packets) so that data can be sent at different rates (e.g. a fast, medium and slow packet).

The HK Application is a passive application and is completely packet driven. There are no timers used to wake up the application. The housekeeping request commands are not sent to the system apps by HK. A common system design uses the Scheduler (SCH) Application to send the housekeeping requests to the system apps as well as the send-output-message-x command to HK.

A key component that is part of the HK application is the 'copy table'. The copy table has a configurable number of entries. Each entry specifies a source (via msgid and offset into the packet), destination (via msgid and offset) and a number of bytes to copy.
HK Design Overview

The HK Application has a single Software Bus pipe and wakes up only when a message is received on the pipe (named HK_CMD_PIPE). The HK_CMD_PIPE receives commands and input messages. If the message received is not a command, the code extracts the message ID from the packet and begins scanning the copy table for a matching Message ID. Once a match is found, the data is copied from input message to the output message specified in the copy table entry. The code then continues searching for another entry in the table with the same Message ID. The number of destinations for input message data is limited only by the number of entries in the table. Data from an input message may be copied to many different output packets and may also be copied to more than one area in the same output packet. The number of output packets is also limited only by the the number of entries in the table. The number of entries in the table is a platform configuration parameter.

The output messages are sent to the software bus when HK receives the 'Send Output Message x' command. Where 'x' is a parameter in the command that specifies the message ID of the output message to send. If a piece of data is missing in the output message at the time the 'Send Output Message x' command is received, the HK app will send a debug event (which is filtered by default) and increment the 'missing data counter' in telemetry.

.. toctree::
   :caption: Contents
   :name: fm_contents

   Issues and Limitations <docs/limitations>
   Design <docs/design>
   Activities <docs/activities>
   Configuration <docs/configuration>
   Concept of Operations <docs/conops>
   FAQ <docs/faq>

Detailed design documentation is available `here <../../doxy/apps/hk/index.html>`_.

.. image:: /docs/_static/doxygen.png
   :target: ../../doxy/apps/hk/index.html
