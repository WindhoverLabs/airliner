Design
================

.. graphviz::
   :caption: PRMLIB Executive Diagram
   
   digraph PRMLIB_Executive_Diagram {
         
      PRMLIB -> APP [label="Parameter Values"];
      APP -> PRMLIB [label="Registers Params by\nName,Value, and Type.\nAdd/Update/Remove Params"];
      
      PRMLIB [style="filled,rounded", fillcolor="#fbe6ff", shape=circle, width=2, label="PRMLIB"];
      APP [style="filled,rounded", fillcolor="#f5ffc7", shape=circle, label="All apps\nwith params"];
      
    }

TODO: Add relevent PRMLIB Diagrams.

Detailed design documentation can be found `here <../../../doxy/apps/prmlib/cfsprmlibovr.html>`_.

.. image:: /docs/_static/doxygen.png
   :target: ../../../doxy/apps/prmlib/index.html