Activities
==========


.. graphviz::
   :caption: TO Executive Diagram
   
   digraph TO_Executive_Diagram {
         
      POINT -> START;
      START -> CLASSIFIER;
      CLASSIFIER -> SCHEDULER;
      SCHEDULER -> END
      
      POINT [shape=point];
      START [style="filled,rounded", fillcolor="#dee1ff", shape=rect, label="Process Telemetry"];
      CLASSIFIER [style=filled, fillcolor="#8fffad", shape=rect, label="Run Classifier"];
      SCHEDULER [style=filled, fillcolor="#8fffad", shape=rect, label="Run Scheduler"];
      END [style="filled,rounded", fillcolor="#dee1ff", shape=rect, label="End"];
      
    }

    
.. figure:: dox_src/images/TO_Run_Classifier.png
   :width: 490px
   :height: 710px
      
   TO Run Classifier

    
.. figure:: dox_src/images/TO_Run_Scheduler.png
   :width: 390px
   :height: 769px
      
   TO Run Scheduler
   

.. figure:: dox_src/images/TO_Priority_Queueing.png
   :width: 1000px
   :height: 366px
      
   TO Priority Queueing
    
    