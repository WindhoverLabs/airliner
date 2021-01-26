Memory Manager
====================================

The :term:`MM` component of the Core Flight System (CFS) is responsible for the loading and dumping of flight system memory. MM is basically the operator interface for the OSAL (Operating System Abstraction Layer) memory manipulation functions. Memory Manager provides the ability to load and dump memory via commands as well as from files. If the operating system supports symbolic addressing, Memory Manager allows specifying the memory address using a symbolic address.

Since MM makes use of the OSAL when interfacing to memory, it relies on OSAL routines to access processor memory as well as memory that is not directly accessible (i.e. requires address translation). MM does no memory manipulation that bypasses the OSAL. Address checking is performed using the OSAL and any addresses specified outside of defined legal address ranges will be considered invalid and an error event will be issued.

MM performs data transfers between memory and files, but does not handle file transfers to (or from) the ground. That function must be done with a file transfer application such as the CFS CFDP application. MM also does not have responsibility for file management operations or directory manipulations. That function is allocated to the CFS FM (File Manager) application.

MM is a command driven, single threaded application. It will pend indefinitely on the software bus message queue until commands are recieved and then process them sequentially in the order they arrived. Memory Manager segments loads and dumps in order to prevent CPU hogging. Despite this, careful consideration should be made when loading and dumping large amounts of memory. 

.. toctree::
   :caption: Contents
   :name: mm_contents
   
   Issues and Limitations <docs/limitations>
   Design <docs/design>
   Activities <docs/activities>
   Configuration <docs/configuration>
   Concept of Operations <docs/conops>
   FAQ <docs/faq>
Detailed design documentation is available `here <../../doxy/apps/mm/index.html>`_.

.. image:: /docs/_static/doxygen.png
   :target: ../../doxy/apps/mm/index.html
