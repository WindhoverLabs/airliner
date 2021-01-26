Design
================

TODO: Autogenerate design graph from /home/vagrant/git/airliner/apps/lc/docs/dox_src/detailed_design/LC_Context.png

.. graphviz::
   :caption: LC Context Diagram
   
   digraph LC_Context_Diagram {
         
      SCH -> LC [label="Sample\nActionpoints"];
      CI  -> LC [label="Ground\nCommands"];
      WDT_TBL -> LC [label=""];
      ADT_TBL -> LC [label=""];
      SB -> LC [label="Watchpoint\nPackets"];
      LC -> SC [label="Start RTS\nCommands"];
      LC -> HK [label="Event\nMessages\nHousekeeping"];
      WRT_TBL -> LC [dir="both"];
      ART_TBL -> LC [dir="both"];

      LC  [style=filled, fillcolor="#dee1ff", shape=circle, width=2, label="Limit Checker"]
      SCH [style=filled, fillcolor="#f5ffc7", URL="../sch/README.html", shape=circle, width=1]
      CI  [style=filled, fillcolor="#f5ffc7", URL="../ci/README.html", shape=circle, width=1]
      WDT_TBL [style=filled, fillcolor="#fbe6ff", shape=folder, width=1, height=1, label="Watchpoint\nDefinition\nTable"]
      ADT_TBL [style=filled, fillcolor="#fbe6ff", shape=folder, width=1, height=1, label="Actionpoint\nDefinition\nTable"]
      SB  [style=filled, fillcolor="#f5ffc7", URL="../ds/README.html", shape=circle, width=1, label="SB"]
      SC  [style=filled, fillcolor="#f5ffc7", URL="../hk/README.html", shape=circle, width=1, label="SC"]
      HK  [style=filled, fillcolor="#f5ffc7", URL="../cf/README.html", shape=circle, width=1, label="HK,TO"]
      WRT_TBL [style=filled, fillcolor="#fbe6ff", shape=folder, width=1, height=1, label="Watchpoint\nResults\nTable"]
      ART_TBL [style=filled, fillcolor="#fbe6ff", shape=folder, width=1, height=1, label="Actionpoint\nResults\nTable"]
      
    }

Detailed design documentation can be found `here <../../../doxy/apps/lc/index.html>`_.

.. image:: /docs/_static/doxygen.png
   :target: ../../../doxy/apps/lc/index.html
