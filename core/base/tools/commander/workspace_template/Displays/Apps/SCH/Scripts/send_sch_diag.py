from org.csstudio.opibuilder.scriptUtil import PVUtil
from org.yamcs.studio.data import VTypeHelper
from org.yamcs.studio.script import Yamcs
from org.yamcs.studio.core.utils import RCPUtils


slot_number = widget.getParent().getMacroValue("SLOT_NUMBER")
entry_number = widget.getParent().getMacroValue("ENTRY_NUMBER")

Yamcs.issueCommand('/cfs/'+display.getMacroValue('CPUID')+'/sch/SendDiag',{} )

# TODO: Refresh OPI