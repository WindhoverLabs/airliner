from org.csstudio.opibuilder.scriptUtil import PVUtil
from org.yamcs.studio.script import Yamcs
from org.yamcs.studio.data import VTypeHelper
import math

# maskNumber is essentially the tab number this button belongs to.
maskNumber = int(widget.getParent().getMacroValue("TAB_NUMBER"))
source_widget_name = widget.getParent().getMacroValue("ACTUAL_TRIGGER_WIDGET_NAME")
destination_widget_name = widget.getParent().getMacroValue("REQUEST_TRIGGER_WIDGET_NAME")
maskValue = 0

i = 0

while i < 32:
    perf_record_link = \
    display.getWidget("PerfTabbedContainer").getWidgetModel().getChildren()[maskNumber].getChildByName(
        "PerfRecordContainer").getChildren()[i]

    # Make sure we avoid the grid layout
    if perf_record_link.getPropertyValue('widget_type') == 'Linking Container':
        trigger_actual_widget = perf_record_link.getChildren()[0].getChildByName(source_widget_name)
        trigger_request_widget = perf_record_link.getChildren()[0].getChildByName(destination_widget_name)
        new_pv_value = trigger_actual_widget.getPropertyValue('pv_value')

        trigger_request_widget.setPropertyValue('pv_value', new_pv_value)

        i = i + 1

