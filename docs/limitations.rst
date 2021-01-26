Limitations and Known Issues
============================


TODO: Add limitations and known issues.

-The Health and Safety application is very robust and will only fail to run or stop running if certain cFE failures occur. At start-up, this includes registering tables with Table Services, subscribing to Event Services, and creating pipes and subscribing with Software Bus. While running, this only occurs of if the Software Bus Receive API generates an error.

While the typical operational scenario for changing the Application Monitor Table or Event Monitor Table would probably involve disabling the respective Monitoring type, the software does support loading while running (although this will reset the Application Monitoring even if the same or a similar table is loaded).

Detailed design documentation is available `here <../../doxy/apps/hs/cfshscons.html>`_.

.. image:: /docs/_static/doxygen.png
   :target: ../../doxy/apps/hs/index.html

