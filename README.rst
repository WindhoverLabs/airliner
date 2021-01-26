Health Services
====================================


  The :term:`HS` Health and Safety application is a core Flight System (cFS) 
  application that is a plug in to the Core Flight Executive (cFE) component 
  of the cFS.  
  
  The cFS is a platform and project independent reusable software framework and
  set of reusable applications developed by NASA Goddard Space Flight Center.  
  This framework is used as the basis for the flight software for satellite data 
  systems and instruments, but can be used on other embedded systems.  More 
  information on the cFS can be found at http://cfs.gsfc.nasa.gov
  
  The HS application provides functionality for Application Monitoring, 
  Event Monitoring, Hardware Watchdog Servicing, Execution Counter Reporting
  (optional), and CPU Aliveness Indication (via UART). 

  The HS application is written in C and depends on the cFS Operating System 
  Abstraction Layer (OSAL) and cFE components.  To build and run the HS
  application, follow the cFS Deployment Guide instructions contained in 
  cFE-6.4.1-OSS-release/docs.

.. toctree::
   :caption: Contents
   :name: hs_contents
   
   Issues and Limitations <docs/limitations>
   Design <docs/design>
   Activities <docs/activities>
   Configuration <docs/configuration>
   Concept of Operations <docs/conops>
   FAQ <docs/faq>

Detailed design documentation is available `here <../../doxy/apps/hs/index.html>`_.


.. image:: /docs/_static/doxygen.png
   :target: ../../doxy/apps/hs/index.html