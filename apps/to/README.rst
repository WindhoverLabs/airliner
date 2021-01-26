Telemetry Output
================

The :term:`TO` Application application subscribes to and transmits Consultative 
Committee for Space Data Systems (:term:`CCSDS`) Space Packet Protocol Telemetry 
packets (:cfe:`ccsds.h`) from the cFE Software Bus, and transmits them via the 
transmission media.  The specifics of the transmission media are handled by the 
platform specific TO "custom" layer, contained within the build configuration, 
but is normally sent via a IP/UDP message.

Messages are prioritized, to help manage traffic throughput.  Transmission rates 
are ultimately driven by the rate that TO is scheduled to execute, the rate that
messages are published to the bus, and how the prioritized messages are 
interleaved at runtime.

TO supports multiple simultaneous output channels.  Configuration is set at build
time, but most configurable parameters can be changed at runtime via CFE table 
and/or command.  


.. toctree::
   :caption: Contents
   :name: to_contents
   
   Issues and Limitations <docs/limitations>
   Design <docs/design>
   Activities <docs/activities>
   Configuration <docs/configuration>
   Concept of Operations <docs/conops>
   FAQ <docs/faq>

