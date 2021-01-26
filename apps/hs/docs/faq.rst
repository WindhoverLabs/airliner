FAQ
================

**How is the application health monitored?**


Use the counters maintained by ES in the CFE_ES_RunLoop function
Critical applications must call CFE_ES_RunLoop to increment the execution counter and let the system know they are active
Critical applications must therefore be active at some known minimum rate so that it can be verified that they can still function nominally



**Where does the list of critical apps come from?**


The list of critical Apps is loaded from a table along with timeout and action information



**How are infinite reset loops prevented?**


Processor Resets caused by HS are limited to a maximum of <TBD, Configuration parameter>



**What happens if an app goes away or is restarted?**


There should be sufficient time to restart an app before it’s flagged as missing
The application monitoring can be disabled while maintenance is being done on a critical app

Detailed design documentation is available `here <../../doxy/apps/hs/cfshsfaqs.html>`_.

.. image:: /docs/_static/doxygen.png
   :target: ../../doxy/apps/hs/index.html