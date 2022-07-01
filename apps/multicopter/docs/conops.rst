Concept of Operations
=====================

TODO: Add Commander screenshots, with a legend for each telemetry item and command button.

TODO: Add activation procedure.

TODO: Add tuning and reconfiguration procedures.

TODO: List potential failure modes.

TODO: Add common troubleshooting procedures.


The following operations are carried out by the LD Application:
1.Detect if vehicle is in free fall state

1) If parameter LD_FFALL_THR is set to zero or invalid, disable free-fall detection.
2) Get x,y,z accelerations acting on the vehicle and compute the net acceleration.
3) If acceleration is less than LD_FFALL_THR, flag state to true.
2.Detect if vehicle is has ground contact state

1) If the vehicle is not armed then it is in ground contact state.
2) Never report landed in manual mode if:

    If the user has more than idle throttle.
    Check throttle and report no ground contact based on the pilots intent to take off.

3) Increase acceptance thresholds for landed state after arming the vehicle so that the increasing motor speed and other effects do not trigger false negatives.
4) Mark vehicle state as ground contact if.

    Not moving vertically.
    Pilot reduces thrust, or in auto mode we are below minimum thrust.
    We cannot move any further down.

3.Detect if vehicle is landed state

1) If the vehicle is not armed then it is in landed state.
2) If the vehicle is manually controlled and is idling, vehicle stays in landed state until the pilot increases throttle for takeoff.

    If throttle is more than a given threshold, set ground contact state history to false.

3) If the vehicle is not in altitude lock mode, check if the vehicle has minimum thrust and the drone stays that way of at least LD_MIN_THR_NO_ALT_TIMEOUT seconds then mark state as landed.
4) Increase acceptance thresholds for landed state after arming the vehicle so that the increasing motor speed and other effects do not trigger false neg000atives.
5) Mark vehicle state as landed if.

    Not moving horizontally.
    Have no thrust. (look if all rotation angles are not moving)
    Ground contact persists.

4.Determining current state of the vehicle

1) The state of the vehicle is determined by the land detector. A vehicle is said to be in landed state if it has seen a ground contact state previously.
   
Detailed design documentation is available `here <../../doxy/apps/ld/cfsldopr.html>`_.

.. image:: /docs/_static/doxygen.png
   :target: ../../doxy/apps/ld/index.html