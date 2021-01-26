Design
================

TODO Add relevent CFS Diagrams.

.. graphviz::
   :caption: CFS Context Diagram
   
   digraph CFS_Context_Diagram {
         
      
      CI  -> DS [label="Ground\nCommands"];
      SCH -> DS [label="HK\nRequest\nCommand"];
      FILTER_TBL -> cFE_TBL [dir="both"];
      FILE_TBL -> cFE_TBL [dir="both"];
      FILTER_TBLE_FILE -> FILTER_TBL [label=""];
      FILE_TBLE_FILE -> FILE_TBL [label=""];
      DS -> SB [label="Subscribe to Data\nStorage Packets"];
      DS -> SC [label="Start RTS\nCommands"];
      DS -> HKTO [label="Housekeeping TLM\nEvent Messages"];
      cFE_TIME -> DS [label="Time\nOF Day"];
      cFE_TBL -> DS [dir="both"];
      SA -> DS [label="Data Storage Packets"];
      DS -> OSAL [label="Data Storage Packets"];
      OSAL -> FILE;

      DS  [style=filled, fillcolor="#dee1ff", shape=circle, width=2, label="CFS DS"]
      SCH [style=filled, fillcolor="#f5ffc7", URL="../sch/README.html", shape=circle, width=1]
      CI  [style=filled, fillcolor="#f5ffc7", URL="../ci/README.html", shape=circle, width=1]
      FILTER_TBL [style=filled, fillcolor="#fbe6ff", shape=folder, width=1, height=1, label="Filter\nTable"]
      FILE_TBL [style=filled, fillcolor="#fbe6ff", shape=folder, width=1, height=1, label="File\nTable"]
      SB  [style=filled, fillcolor="#f5ffc7", URL="../ds/README.html", shape=circle, width=1, label="cFE SB"]
      cFE_TBL  [style=filled, fillcolor="#f5ffc7", shape=circle, width=1, label="cFE Table"]
      SC  [style=filled, fillcolor="#f5ffc7", URL="../hk/README.html", shape=circle, width=1, label="SC"]
      HKTO  [style=filled, fillcolor="#f5ffc7", URL="../cf/README.html", shape=circle, width=1, label="HK,TO"]
      FILTER_TBLE_FILE [style=filled, fillcolor="#fbe6ff", shape=folder, width=1, height=1, label="Watchpoint\nResults\nTable"]
      FILE_TBLE_FILE [style=filled, fillcolor="#fbe6ff", shape=folder, width=1, height=1, label="Actionpoint\nResults\nTable"]
      cFE_TIME [style=filled, fillcolor="#f5ffc7", URL="../hk/README.html", shape=circle, width=1, label="cFE Time"]
      SA [style=filled, fillcolor="#fbe6ff", shape=circle, width=1, height=1, label="System Apps"]
      OSAL [style=filled, fillcolor="#fbe6ff", shape=rect, width=1, height=1, label="OSAL"]
      FILE [style=filled, fillcolor="#fbe6ff", shape=rect, width=1, height=1]
    }

.. figure:: dox_src/users_guide/CFS_PBLIB_Context.jpg
   :width: 494
   :height: 336
   
