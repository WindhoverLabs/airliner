Design
================

TODO: Add relevent NAV Diagrams.

Design Overview
^^^^^^^^^^^^^^^

1. Reads position, vehicle status, and land detection data to generate position setpoint triplet for commanded action.
2. This message has enough information for a vehicle to move from one position to another.
3. The message is published on the software bus which is then received by MPC.


Detailed design documentation can be found `here <../../../doxy/apps/nav/cfsnavovr.html>`_.

.. image:: /docs/_static/doxygen.png
   :target: ../../../doxy/apps/nav/index.html
