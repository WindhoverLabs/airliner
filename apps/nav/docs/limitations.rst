Limitations and Known Issues
============================

TODO: Add NAV limitations and issues

Requirements
^^^^^^^^^^^^
   
The following are NAV application requirements.

(Insert high level requirements and add a reference to where detailed requirements can be found)

Operational Constraints
^^^^^^^^^^^^^^^^^^^^^^^

(Instructions to the author)

(Insert text that describes: 1) situations that would prevent the application from starting 2) causes for the application to terminate prematurely 3) commands that cannot be issued either out of order or without sufficient wait time, etc)
Application Termination

Potential causes for NAV to terminate prematurely are an error return from the software bus in the application main loop or error return from one of the cFE table services functions when NAV tries to do table management (check for updates, dump requests etc.) during each housekeeping cycle.
Critical Data Store Operation

When using the Critical Data Store (CDS), the results tables and housekeeping data will only be restored if the application was shut down through cFE Executive Services and exited clean. If this is not the case, default values will be used (see NAV Application Frequently Asked Questions).


Detailed Limitation and Known Issues documentation can be found `here <../../../doxy/apps/nav/cfsnavcons.html>`_.

.. image:: /docs/_static/doxygen.png
   :target: ../../../doxy/apps/pe/index.html