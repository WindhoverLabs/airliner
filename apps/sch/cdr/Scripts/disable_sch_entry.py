from org.csstudio.opibuilder.scriptUtil import PVUtil
from org.yamcs.studio.data import VTypeHelper
from org.yamcs.studio.script import Yamcs

slot_number = widget.getParent().getMacroValue("SLOT_NUMBER")
entry_number = widget.getParent().getMacroValue("ENTRY_NUMBER")

Yamcs.issueCommand('/cfs/'+display.getMacroValue('CPUID')+'/sch/Disable',
                   {
                       'SlotNumber': slot_number,
                       'EntryNumber': entry_number
                   }
                   )
