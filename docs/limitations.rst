Limitations and Known Issues
============================


TODO: Add limitations and known issues.

The LC application requires default WDT and ADT table images to be on the filesystem when the application is started or it will fail to load. These files are required even if using the Critical Data Store (CDS) since LC will fall back to these table images if a table restore from CDS fails (which will happen during a power-on reset). Where LC will look to find these files is dictated by the configuration parameters LC_WDT_FILENAME and LC_ADT_FILENAME.

Potential causes for LC to terminate prematurely are an error return from the software bus in the application main loop or error return from one of the cFE table services functions when LC tries to do table management (check for updates, dump requests etc.) during each housekeeping cycle.

When using the Critical Data Store (CDS), the results tables and housekeeping data will only be restored if the application was shut down through cFE Executive Services and exited clean. If this is not the case, default values will be used (see CFS Limit Checker Frequently Asked Questions).

Ideally, LC should require no intervention from the ground on a routine basis. Once monitoring is enabled, it will keep processing data and checking for threshold violations (assuming regular scheduler input).


Detailed FAQ documentation is available `here <../../doxy/apps/lc/cfslccons.html>`_.

.. image:: /docs/_static/doxygen.png
   :target: ../../doxy/apps/lc/index.html

