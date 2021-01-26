Design
================

TODO: 
Detailed design documentation can be found `here <../../../doxy/apps/md/index.html>`_.

.. graphviz::
   :caption: MD Context Diagram
   
   digraph MD_Context_Diagram {
         
      
      SH -> MD [label="Wakeup messages,\nHK Requests"];
      CI -> MD [label="Configureation\nCommands"];
      MD -> HK [label="HK Packets"];
      MD -> TODS [label="Dwell Packets"];
      DS -> TODS [label="Event Messages"];
      PSI -> MD [label="Saved\nState\nInfo"];
      MD -> PSI [label="State\nChanges"];
      MD -> PDS [label="Changed\nDwell\nSpecifications"];
      PDS -> MD [label="Saved\nDwell\nSpecifications"];
      MD -> OSAL [label="Symbolic Address"];
      MD -> OSAL [label="Resolved Address"];
      OSAL -> MD [label="Numerical Address"];
      OSAL -> MD [label="Valid/No Valid Indicator"];

      MD  [style=filled, fillcolor="#dee1ff", shape=circle, width=2, label="CFS MD"]
      SH [style=filled, fillcolor="#f5ffc7", URL="../sch/README.html", shape=circle, width=1]
      CI  [style=filled, fillcolor="#f5ffc7", URL="../ci/README.html", shape=circle, width=1]
      OSAL  [style=filled, fillcolor="#f5ffc7", URL="../ds/README.html", shape=circle, width=1]
      HK  [style=filled, fillcolor="#f5ffc7", URL="../cf/README.html", shape=circle, width=1, label="HK"]
      PDS [style=filled, fillcolor="#fbe6ff", shape=rect, width=1, height=1, label="Persistent Dwell Specifications"]
      PSI [style=filled, fillcolor="#fbe6ff", shape=rect, width=1, height=1, label="Persistent State Info"]
      TODS [style=filled, fillcolor="#f5ffc7", URL="../to/README.html", shape=circle, width=1]
    }

.. image:: /docs/_static/doxygen.png
   :target: ../../../doxy/apps/md/index.html


