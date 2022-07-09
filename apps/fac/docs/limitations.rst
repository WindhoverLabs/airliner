Limitations and Known Issues
============================


TODO: Add limitations and known issues.

Application Termination

Potential causes for FAC to terminate prematurely are an error return from the software bus in the application main loop or error return from one of the cFE table services functions when FAC tries to do table management (check for updates, dump requests etc.) during each housekeeping cycle.
Critical Data Store Operation

When using the Critical Data Store (CDS), the results tables and housekeeping data will only be restored if the application was shut down through cFE Executive Services and exited clean. If this is not the case, default values will be used (see FAC Application Frequently Asked Questions).

Detailed documentation is available `here <../../doxy/apps/fac/cfsfaccons.html>`_.

.. image:: /docs/_static/doxygen.png
   :target: ../../doxy/apps/fac/index.html