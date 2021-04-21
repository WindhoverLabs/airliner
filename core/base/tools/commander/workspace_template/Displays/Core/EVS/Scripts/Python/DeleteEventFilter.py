from org.csstudio.opibuilder.scriptUtil import PVUtil
from org.yamcs.studio.data import VTypeHelper
from org.yamcs.studio.script import Yamcs

event_id = widget.getParent().getMacroValue("EVENTID")
# NOTE: Not sure if using macros is a good idea when passing data between scripts as they are scoped to the OPI file
# it looks like and not the OPI instance.
app_name = display.getMacroValue("APP").upper()


Yamcs.issueCommand('/cfs/'+display.getMacroValue('CPUID')+'/cfe_evs/DeleteAppEventFilter',
                   {
                       'Payload.AppName': app_name,
                       'Payload.EventID': event_id
                   }
                   )
