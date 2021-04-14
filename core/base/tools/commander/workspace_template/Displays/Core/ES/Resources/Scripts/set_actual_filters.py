from org.csstudio.opibuilder.scriptUtil import PVUtil
from org.yamcs.studio.script import Yamcs
from org.yamcs.studio.data import VTypeHelper
import math

maskNumber = 0 # This will be a macro; the tab number
maskValue = 0
event_id = VTypeHelper.getString(widget.getPropertyValue("pv_value"))
print(event_id)

print('display:{}'.format(display))

for i in range(32):
    RequestFilterName = 'FilterActual' + str(i)
    perfIdValue = VTypeHelper.getDouble(display.getWidget(RequestFilterName).getPropertyValue('pv_value'))
    print('perf id-->{}'.format(perfIdValue))
#     maskValue = (int(math.pow(2, i - 1)) * perfIdValue) + maskValue
#
# Yamcs.issueCommand('/cfs/$(CPUID)/cfe_es/PerfSetFilterMask', {
#     'Payload.FilterMaskNum': maskNumber,
#     'Payload.FilterMask': maskValue})



