Limitations and Known Issues
============================


TODO: Add limitations and known issues.

- The number of or type of channels can only be set at build time, not run time.

Application Termination

Potential causes for MPU6050 to terminate prematurely are an error return from the software bus in the application main loop or error return from one of the cFE table services functions when MPU6050 tries to do table management (check for updates, dump requests etc.) during each housekeeping cycle.
Critical Data Store Operation

When using the Critical Data Store (CDS), the results tables and housekeeping data will only be restored if the application was shut down through cFE Executive Services and exited clean. If this is not the case, default values will be used (see MPU6050 Application Frequently Asked Questions).

Detailed operational constraints documentation is available `here <../../doxy/apps/mpu6050/cfsmpu6050cons.html>`_.

.. image:: /docs/_static/doxygen.png
   :target: ../../doxy/apps/mpu6050/index.html