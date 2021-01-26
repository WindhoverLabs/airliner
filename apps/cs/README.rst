Checksum Services
====================================

The :term:`CS` The Checksum (CS) application of the Core Flight System (CFS) is responsible for ensuring the integrity of onboard memory. CS has the ability to ensure the integrity of Core Flight Executive (cFE) applications, cFE tables, the cFE core, the onboard operating system (OS), onboard EEPROM, as well as any memory regions ("Memory") specified by the users.

CS automatically checksums the cFE core and the OS by default when the CS application is used. Additionally, the user may select other areas to checksum, including:

    EEPROM: A contiguous block of memory specified by a starting address and number of bytes.
    Memory: A contiguous block of (non-EEPROM) memory specified by a starting address and number of bytes.
    Tables: Any cFE or CFS application table, specified by the table name.
    Applications: Any cFE or CFS application, specified by the app name.

The CS application is table driven. All of the applications, tables, areas of EEPROM, and other memory areas to be checksummed must be defined by the user in four separate tables that can be controlled independently.

The CS application is also schedule driven. It will checksum up to CS_DEFAULT_BYTES_PER_CYCLE (a configuration parameter) bytes every time CS receives its background cycle MID.

The CS application uses two sets of tables: a set of definition tables which the user populates on the ground with the set of applications, tables, areas of EEPROM and other memory areas, and a set of results tables which are dump only tables. The results tables are populated by CS automatically from the definition tables, but also contain more information. The results tables contain the checksum values for each object in the table as well as other information for CS to use during runtime. For more information see CFS .

.. toctree::
   :caption: Contents
   :name: cs_contents
   
   Issues and Limitations <docs/limitations>
   Design <docs/design>
   Activities <docs/activities>
   Configuration <docs/configuration>
   Concept of Operations <docs/conops>
   FAQ <docs/faq>

Detailed design documentation is available `here <../../doxy/apps/cs/index.html>`_.

.. image:: /docs/_static/doxygen.png
   :target: ../../doxy/apps/cs/index.html
