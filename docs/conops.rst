Concept of Operations
=====================

TODO: Add Commander screenshots, with a legend for each telemetry item and command button.

TODO: Add activation procedure.

TODO: Add tuning and reconfiguration procedures.

TODO: List potential failure modes.

TODO: Add common troubleshooting procedures.

The CFS Limit Checker has three modes of operation, set by the  ground command and echoed in the CurrentLCState housekeeping variable.

Active - Active mode is the normal operation mode. In Active mode, the LC task performs all limit tests defined in the watchpoint definition table and will invoke stored command sequences if the results of watchpoint evaluations trigger an actionpoint condition as defined in the actionpoint definition table.</p>


Passive - In Passive mode, the LC task behaves just like in Active mode except no stored command sequences will be invoked as a result of actionpoint triggers.</p>


Disabled - When disabled, the LC task will perform no watchpoint or actionpoint limit tests.</p>


Watchpoints are evaluated whenever a message containing the telemetry point is received by LC. Watchpoints can not be individually disabled. The only ways to keep Watchpoints from being evaluated are to squelch the telemetry packet on which they trigger, load a WDT that undefines a Watchpoint or disable the entire LC task.
   
Detailed design documentation is available `here <../../doxy/apps/lc/cfslcopr.html>`_.

.. image:: /docs/_static/doxygen.png
   :target: ../../doxy/apps/lc/index.html