Design
================

TODO: 

.. graphviz::
   :caption: HS Context Diagram
   
   digraph HS_Context_Diagram {
         
      SCH -> HS [label="HK Requests\nWake-Up"];
      CI  -> HS [label="Ground\nCommands"];
      HSIC -> HS [label="Watchpoint\nPackets"];
      HS -> EVS [label="HS Event\nMessages"];
      EVS -> HS [label="All Event\nMessages"];
      HS -> HK [label="HS HK Packet"];
      HK -> TODS;
      HST -> HS [label="HS Table Data"];
      TBL -> HS [label="Load Status\nTable Pointers"];
      TBL -> HST [label="HS Table Data"];
      HS -> ES [label="Reset calls"];
      ES -> HS [label="App Info"];
      HS -> SA [label="Message actions"];


      HS  [style=filled, fillcolor="#dee1ff", shape=circle, width=2, label="CFS HS"]
      SCH [style=filled, fillcolor="#f5ffc7", URL="../sch/README.html", shape=circle, width=1]
      CI  [style=filled, fillcolor="#f5ffc7", URL="../ci/README.html", shape=circle, width=1]
      HSIC  [style=filled, fillcolor="#f5ffc7", shape=circle, width=1, label="CFS HS\nIdle Child"]
      EVS  [style=filled, fillcolor="#f5ffc7", shape=circle, width=1, label="EVS"]
      HK  [style=filled, fillcolor="#f5ffc7", URL="../hk/README.html", shape=circle, width=1, label="HK"]
      TODS  [style=filled, fillcolor="#f5ffc7", URL="../to/README.html", shape=circle, width=1, label="TO, DS"]
      HST [style=filled, fillcolor="#f5ffc7", URL="../hs/README.html", shape=folder, width=1, label="HS Tables"]
      TBL [style=filled, fillcolor="#f5ffc7", shape=circle, width=1, label="TBL"]
      ES [style=filled, fillcolor="#f5ffc7", shape=circle, width=1]
      SA [style=filled, fillcolor="#f5ffc7", shape=circle, width=1]

    }


.. image:: /docs/_static/doxygen.png
   :target: ../../doxy/apps/hs/index.html
