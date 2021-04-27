from org.csstudio.opibuilder.scriptUtil import PVUtil
from org.yamcs.studio.script import Yamcs
from org.yamcs.studio.data import VTypeHelper
import math

# maskNumber is essentially the tab number this button belongs to.
maskNumber = int(widget.getParent().getMacroValue("TAB_NUMBER"))
widget_name = widget.getParent().getMacroValue("REQUEST_FILTER_WIDGET_NAME")
maskValue = 0

i = 0

# FIXME: Maybe make the 32 a macro(?)
while i < 32:
    perf_record_link = \
    display.getWidget("PerfTabbedContainer").getWidgetModel().getChildren()[maskNumber].getChildByName(
        "PerfRecordContainer").getChildren()[i]

    # Make sure we avoid the grid layout
    if perf_record_link.getPropertyValue('widget_type') == 'Linking Container':
        filter_request_widget = perf_record_link.getChildren()[0].getChildByName(widget_name)
        perfIdValue = int(VTypeHelper.getDouble(filter_request_widget.getPropertyValue('pv_value')))
        maskValue = int((int(math.pow(2, i)) * perfIdValue) + maskValue)

        i = i + 1

Yamcs.issueCommand('/cfs/' + display.getMacroValue("CPUID") + '/cfe_es/PerfSetFilterMask', {
    'Payload.FilterMaskNum': maskNumber,
    'Payload.FilterMask': maskValue})
