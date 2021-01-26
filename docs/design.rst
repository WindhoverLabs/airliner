Design
================

.. graphviz::
   :caption: TO Context Diagram
   
   digraph TO_Context_Diagram {
         
      SCH -> TO [label="SEND_HK and\nPROCESS_TELEMETRY\ncmds"];
      CI  -> TO [label="Ground\nCommands"];
      CFG_TBL -> TO [label="MsgIDs and\npriority queue\nassignments"];
      TO -> DS [label="HK\ntelemetry"];
      TO -> HK [label="HK\ntelemetry"];
      CF -> TO [dir="both" label="File\nData"];
      TO -> CF [label="Semaphore"];
      CFE -> TO [label="Messages\nfor\ndownlink"];
      ALL_APPS -> TO [label="Messages\nfor\ndownlink"];
       
      TO  [style=filled, fillcolor="#dee1ff", shape=circle, width=2, label="Telemetry Output"]
      SCH [style=filled, fillcolor="#f5ffc7", URL="../sch/README.html", shape=circle, width=1]
      CI  [style=filled, fillcolor="#f5ffc7", URL="../ci/README.html", shape=circle, width=1]
      CFG_TBL [style=filled, fillcolor="#fbe6ff", shape=folder, width=1, height=1, label="Config Table(s)"]
      DS  [style=filled, fillcolor="#f5ffc7", URL="../ds/README.html", shape=circle, width=1, label="DS"]
      HK  [style=filled, fillcolor="#f5ffc7", URL="../hk/README.html", shape=circle, width=1, label="HK"]
      CF  [style=filled, fillcolor="#f5ffc7", URL="../cf/README.html", shape=circle, width=1]
      CFE  [style=filled, fillcolor="#f5ffc7", shape=circle, width=1, label="CFE"]
      ALL_APPS  [style=filled, fillcolor="#f5ffc7", shape=circle, width=1, label="All Apps"]
      
    }
  

Detailed design documentation can be found `here <../../../doxy/apps/to/index.html>`_.

.. image:: /docs/_static/doxygen.png
   :target: ../../../doxy/apps/to/index.html
   
