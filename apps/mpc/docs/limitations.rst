Limitations and Known Issues
============================


TODO: Add limitations and known issues.

Application Termination

Potential causes for MPC to terminate prematurely are an error return from the software bus in the application main loop or error return from one of the cFE table services functions when MPC tries to do table management (check for updates, dump requests etc.) during each housekeeping cycle.
Critical Data Store Operation

When using the Critical Data Store (CDS), the results tables and housekeeping data will only be restored if the application was shut down through cFE Executive Services and exited clean. If this is not the case, default values will be used (see MPC Application Frequently Asked Questions).

Detailed constraints documentation is available `here <../../doxy/apps/mpc/cfsmpccons.html>`_.

.. image:: /docs/_static/doxygen.png
   :target: ../../doxy/apps/mpc/index.html