from org.csstudio.opibuilder.scriptUtil import PVUtil
from org.yamcs.studio.script import Yamcs
from org.yamcs.studio.data import VTypeHelper

import math

maskNumber = 0
maskValue = 0

for i in range(1, 32):
    RequestFilterName = 'FilterRequest' + str(i);
    perfIdValue = VTypeHelper.getDouble(display.getWidget(RequestFilterName).getPropertyValue('pv_value'));
    if i < 31:
        maskValue = int(math.pow(2, i - 1)) + maskValue
    else:
        maskValue = int(maskValue) | 0x80000000

Yamcs.issueCommand('/cfs/$(CPUID)/cfe_es/PerfSetFilterMask', {
    'Payload.FilterMaskNum': maskNumber,
    'Payload.FilterMask': maskValue})


