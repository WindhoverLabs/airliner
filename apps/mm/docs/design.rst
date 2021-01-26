Design
================

TODO: 
Detailed design documentation can be found `here <../../../doxy/apps/mm/index.html>`_.

.. graphviz::
   :caption: MM Context Diagram
   
   digraph MM_Context_Diagram {
         
      
      SCH -> MM [label="]HK Requests"];
      CI -> MM [label="Configureation\nCommands"];
      MM -> HK [label="HK Packets\nMM Events\nPeek Events\nPoke Events\nDump Events"];
      MM -> OSAL [dir="both", label="Symbolic Address"];
      DALF -> OSAL [dir="both", label="Open/Close\nRead/Write"];
      OSAL -> EEPROM [dir="both", label="Load/Dump"];
      OSAL -> RAM [label="Load/Dump"];
      OSAL -> MMHW [label="Load/Dump"];

      MM  [style=filled, fillcolor="#dee1ff", shape=circle, width=2, label="CFS\nMemory\nManager\n(MM)"]
      SCH [style=filled, fillcolor="#f5ffc7", URL="../sch/README.html", shape=circle, width=1]
      CI  [style=filled, fillcolor="#f5ffc7", URL="../ci/README.html", shape=circle, width=1]
      OSAL  [style=filled, fillcolor="#f5ffc7",shape=rect, width=1, label="cFE\nOSAL/PSP"]
      HK  [style=filled, fillcolor="#f5ffc7", URL="../cf/README.html", shape=circle, width=1, label="HK, TO, DS"]
      DALF [style=filled, fillcolor="#f5ffc7",shape=folder, width=1, label="Dump\nand\nLoad\nFiles"]
      RAM [style=filled, fillcolor="#f5ffc7",shape=rect, width=1]
      EEPROM [style=filled, fillcolor="#f5ffc7",shape=rect, width=1]
      MMHW [style=filled, fillcolor="#f5ffc7",shape=rect, width=1, label="Memory\nMapped\nHardware"]
    }


.. image:: /docs/_static/doxygen.png
   :target: ../../../doxy/apps/mm/index.html