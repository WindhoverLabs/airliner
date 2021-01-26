Limitations and Known Issues
============================

TODO: Add MPU9250 limitations and issues

(Instructions to the author)

(Insert text that describes: 1) situations that would prevent the application from starting 2) causes for the application to terminate prematurely 3) commands that cannot be issued either out of order or without sufficient wait time, etc)

Application Termination
^^^^^^^^^^^^^^^^^^^^^^^

Potential causes for MPU9250 to terminate prematurely are an error return from the software bus in the application main loop or error return from one of the cFE table services functions when MPU9250 tries to do table management (check for updates, dump requests etc.) during each housekeeping cycle.

Critical Data Store Operation
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

When using the Critical Data Store (CDS), the results tables and housekeeping data will only be restored if the application was shut down through cFE Executive Services and exited clean. If this is not the case, default values will be used (see MPU9250 Application Frequently Asked Questions).

Detailed Limitation and Known Issues documentation can be found `here <../../../doxy/apps/mpu9250/cfsmpu9250cons.html>`_.

.. image:: /docs/_static/doxygen.png
   :target: ../../../doxy/apps/mpu9250/index.html