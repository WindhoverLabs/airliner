Concept of Operations
=====================

TODO: Add activation procedure.

TODO: Add tuning and reconfiguration procedures.

TODO: List potential failure modes.

TODO: Add common troubleshooting procedures.


Operation
^^^^^^^^^

The following operations are carried out by the NAV Application: 

1. Receive and act to vehicle command message

a. Vehicle command is parsed (takeoff, land, return to launch, mission)
b. Store current position as previous and goal as next.
c. Store new current position.
d. Assign yaw to current position set point.
e. Assign latitude and longitude to current set point.
f. Assign altitude to current set point.
g. Assign set point triplet validity.

2. Receive and act to vehicle status message

a. On receipt of a new navigation state set position setpoint triplet message.
b. while in a navigation state do activities to update position setpoint triplet message and identify if the next position is reached.

3. Set and publish position set point triplet

a. The position setpoint triplet contains 3 position setpoints they are named as previous, current and next.
b. Previous setpoint store history as old as one level, current setpoint stores present position and next setpoint will store position which is supposed to be reached by the vehicle.
c. This message is published on to software bus.


Detailed operation documentation can be found `here <../../../doxy/apps/nav/cfsnavopr.html>`_.

.. image:: /docs/_static/doxygen.png
   :target: ../../../doxy/apps/nav/index.html