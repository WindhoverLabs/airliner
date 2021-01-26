Design
================

.. graphviz::
   :caption: CF Context Diagram
   
   digraph CF_Context_Diagram {
         
      CF -> Config [dir="both"];
      SCH -> CF [lable="Wake up Msg"];
      CF -> HK [label="Housekeeping"];
      CI -> CF [label="Gnd, Cmds,\nUplink PDUs"]
      CF -> OSAL [dir="both"];
      OSAL -> FS [dir="both"];
      CF -> TO [label="Downlink PDUs\nHousekeeping\nDIAG Packets"];
      TO -> CF [dir="both", label="Throttling\nSmaphore"];
            
      Config [style=filled, fillcolor="#fbe6ff", shape=rect, label="Configuration\nTable"] 
      CF  [style=filled, fillcolor="#dee1ff", shape=circle, width=1.5 label="CF"]
      SCH [style=filled, fillcolor="#f5ffc7", shape=circle, width=1, label="SCH"]
      HK [style=filled, fillcolor="#f5ffc7", shape=circle, width=1, label="HK,DS"]
      CI [style=filled, fillcolor="#f5ffc7", shape=circle, width=1, label="CI"]
      TO [style=filled, fillcolor="#f5ffc7", shape=circle, width=1, label="TO"]
      OSAL [style=filled, fillcolor="#fbe6ff", shape=rect, width=1, label="OSAL"]
      FS [style=filled, fillcolor="#dee1f", shape=folder, width=1, label="File system"]
      
    }   
