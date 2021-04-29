from org.csstudio.opibuilder.scriptUtil import PVUtil
from org.yamcs.studio.data import VTypeHelper
from org.yamcs.studio.script import Yamcs

event_id = widget.getParent().getMacroValue("EVENTID")
app_name = display.getMacroValue("APP").upper()

Yamcs.issueCommand('/cfs/'+display.getMacroValue('CPUID')+'/cfe_evs/ResetAppEventFilter',
                   {
                       'Payload.AppName': app_name,
                       'Payload.EventID': event_id
                   }
                   )
