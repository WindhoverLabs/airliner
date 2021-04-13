from org.csstudio.opibuilder.scriptUtil import PVUtil
from org.yamcs.studio.script import Yamcs
from org.yamcs.studio.data import VTypeHelper

import math

maskNumber = 0
maskValue = 0

for child in display.getWidget("PerfRecordContainer").getChildren():
    # This is ugly, I know. But the Jython setup does not expose the org.csstudio.opibuilder.widgets.model module/package.
    # Discussion https://groups.google.com/g/yamcs/c/njlfcAltj5o might be of interest to reader.
    if 'Linking Container' == child.getPropertyValue('widget_type'):
        child_obj = child.getWidgetModel()
        RequestFilterName = 'FilterRequest' + str(i)
        perfIdValue = VTypeHelper.getDouble(display.getWidget(RequestFilterName).getPropertyValue('pv_value'))
        maskValue = int(math.pow(2, i - 1)) + maskValue
        print(child_obj.getChildByName("PerfRecordContainer").getChildByName("PerfId").getPropertyValue('text'))
        # print('child-->{}'.format(type(child.getWidgetModel())))
                              # .getChildByName("PerfRecordContainer").getChildByName("PerfId")))

# print('type:{}'.format(display.getWidget('Linking Container').getPropertyValue('widget_type')))

# for i in range(1, 32):
#     # i is the perf_id
#     RequestFilterName = 'FilterRequest' + str(i)
#     perfIdValue = VTypeHelper.getDouble(display.getWidget(RequestFilterName).getPropertyValue('pv_value'))
#     if i < 31:
#         maskValue = int(math.pow(2, i - 1)) + maskValue
#     else:
#         maskValue = int(maskValue) | 0x80000000
#
# Yamcs.issueCommand('/cfs/$(CPUID)/cfe_es/PerfSetFilterMask', {
#     'Payload.FilterMaskNum': maskNumber,
#     'Payload.FilterMask': maskValue})


