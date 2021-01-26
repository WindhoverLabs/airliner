Design
================

TODO Add relevent HK Diagrams.
   
.. graphviz::
   :caption: HK Context Diagram
   
   digraph HK_Context_Diagram {
         
      HK -> TBL [label="TBL APIs"];
      TBL -> HK [label="TBL Updates"];
      TBL -> CT [dir="both"];
      TBL -> RT [dir="both"];
      CI  -> HK [label="Ground\nCommands"];
      SA -> HK [label=" Housekeeping\nData"]
      SCH -> HK [label="Send HK Data\nSend Ouput Message X"];
      HK -> SB [label="Subscribe to HK Msgs\nCombined Output Msgs"];
      

      HK  [style=filled, fillcolor="#dee1ff", URL="../hk/README.html",shape=circle, width=2, label="CFS HK"]
      SCH [style=filled, fillcolor="#f5ffc7", URL="../sch/README.html", shape=circle, width=1]
      CI  [style=filled, fillcolor="#f5ffc7", URL="../ci/README.html", shape=circle, width=1]
      SB  [style=filled, fillcolor="#f5ffc7", URL="../sb/README.html", shape=circle, width=1, label="SB"]
      SA  [style=filled, fillcolor="#f5ffc7", URL="../hk/README.html", shape=circle, width=1, label="System\nApps"]
      TBL  [style=filled, fillcolor="#f5ffc7", URL="../cf/README.html", shape=circle, width=1]
      CT [style=filled, fillcolor="#fbe6ff", shape=rect, width=1, height=1, label="Copy\nTable"]
      RT [style=filled, fillcolor="#fbe6ff", shape=rect, width=1, height=1, label="Runtime\nTable"]
      
    }
