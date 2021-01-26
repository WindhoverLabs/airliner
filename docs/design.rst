Design
================

TODO: Add relevent SC Diagrams.


.. graphviz::
   :caption: SC Context Diagram
   
   digraph SC_Context_Diagram {
         
      
      CI  -> SC [label="Ground\nCommands"];
      SCH -> SC [label="HK\nRequest\nCommand\n1 Hz Wakeup Req"];
      SC -> HK [label="Housekeeping\nEvents"];
      SC -> ALLAPPS [label="Send\nCommands"];
      SC -> RTS_Status_Tables [dir="both", label="Read/Write"];
      RTS_Tables -> SC [label="Read"];
      ATS_Status_Tables -> SC [dir="both", label="Read/Write"];
      ATS_Append_Table -> SC [label="Read"];
      ATS_Tables -> SC [label="Read"];
      

      SC  [style=filled, fillcolor="#dee1ff", shape=circle, width=2, label="SC"]
      SCH [style=filled, fillcolor="#f5ffc7", URL="../sch/README.html", shape=circle, width=1]
      CI  [style=filled, fillcolor="#f5ffc7", URL="../ci/README.html", shape=circle, width=1]
      RTS_Status_Tables [style=filled, fillcolor="#fbe6ff", shape=rect, width=1, height=1, label="RTS\nStatus Table"]
      RTS_Tables [style=filled, fillcolor="#fbe6ff", shape=rect, width=1, height=1, label="RTS Tables"]
      ALLAPPS  [style=filled, fillcolor="#f5ffc7", shape=circle, width=1, label="All Apps"]
      ATS_Status_Tables  [style=filled, fillcolor="#fbe6ff", shape=rect, width=1, label="ATS Status Table"]
      HK  [style=filled, fillcolor="#f5ffc7", URL="../hk/README.html", shape=circle, width=1, label="HK, DS, TO"]
      ATS_Append_Table [style=filled, fillcolor="#fbe6ff", shape=rect, width=1, height=1, label="ATS Append\nTable"]
      ATS_Tables [style=filled, fillcolor="#fbe6ff", shape=rect, width=1, height=1, label="ATS Tables"]
      
    }



Detailed design documentation can be found `here <../../../doxy/apps/sc/cfsscovr.html>`_.

.. image:: /docs/_static/doxygen.png
   :target: ../../../doxy/apps/sc/index.html