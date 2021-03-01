from org.csstudio.opibuilder.scriptUtil import PVUtil
from org.yamcs.studio.data import VTypeHelper
from org.yamcs.studio.script import Yamcs

event_id = VTypeHelper.getString(widget.getPropertyValue("pv_value"))
# NOTE: Not sure if using macros is a good idea when passing data between scripts as they are scoped to the OPI file
# it looks like and not the OPI instance.
app_name = display.getMacroValue("APP").upper()

Yamcs.issueCommand('/cfs/cfe_evs/DeleteAppEventFilter',
                   {
                       'Payload.AppName': app_name,
                       'Payload.EventID': event_id
                   }
                   )
