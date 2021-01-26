File Delivery
====================================

The :term:`CF` application is a flight software application that is responsible for transmitting and receiving files. It is a CFS application which is designed to interface to the Core Flight Executive (cFE).

CF is a highly configurable application that is designed to be used on a wide range of flight missions. CF obtains its initial configuration through a configuration table and its platform and mission configuration files. The table contains default configuration settings and is loaded during CF initialization. The platform and mission configuration files are compile-time configuration parameters.

To transfer files using CFDP, the CF application must communicate with a CFDP compliant peer. CF may be configured to have any number of peers.The ASIST and ITOS ground systems contain a compliant peer that may be used for flight to ground (and ground to flight) transfers.

CF sends and receives file information and file-data in Protocol Data Units (PDUs) that are compliant with the CFDP standard protocol defined in the CCSDS 727.0-B-4 Blue Book. The PDUs are transferred to and from the CF application via CCSDS packets on the software bus. The system must be configured to get the PDU packets from the peer to the software bus (and vice-versa).

On a typical spacecraft using the cFE, science files and engineering files are continuously being created and queued for downlink. When transmission begins, the files are converted into a series of PDUs by CF, one after another essentially creating a continuous stream of file-data PDUs.

There are special features listed in the CFDP standard that are not applicable to flight software and are therefore not supported by this version of CF. See the constraints section of this document for more information.

.. toctree::
   :caption: Contents
   :name: cf_contents

   Issues and Limitations <docs/limitations>
   Design <docs/design>
   Activities <docs/activities>
   Configuration <docs/configuration>
   Concept of Operations <docs/conops>
   FAQ <docs/faq>

Detailed design documentation is available `here <../../doxy/apps/cf/index.html>`_.

.. image:: /docs/_static/doxygen.png
   :target: ../../doxy/apps/cf/index.html
