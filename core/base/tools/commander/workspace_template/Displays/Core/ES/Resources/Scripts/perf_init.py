from org.csstudio.opibuilder.scriptUtil import PVUtil, WidgetUtil
from org.eclipse.swt.graphics import RGB
import math

def main():

    display.getWidget("PerfRecordContainer")

    new_perf_record = WidgetUtil.createWidgetModel("org.csstudio.opibuilder.widgets.linkingContainer")
    # TODO: Use the enumeration values from Studio to avoid magical strings
    new_perf_record.setPropertyValue("opi_file", display.getMacroValue("TEMPLATE_OPI"))
    new_perf_record.setPropertyValue("height", 20)
    new_perf_record.setPropertyValue("width", 680)
    new_perf_record.setPropertyValue("background_color", RGB(255, 255, 255))
    new_perf_record.setPropertyValue("border_color", RGB(240, 240, 240))
    new_perf_record.setPropertyValue("border_style", 0)
    new_perf_record.setPropertyValue("name", "perf_name")
    # Refer to org.csstudio.opibuilder.widgets.model for resize_behaviour
    new_perf_record.setPropertyValue("resize_behaviour", 1)

    display.getWidget("PerfRecordContainer").addChild(new_perf_record)
    # newMask = PVUtil.getLong(pvs[0])
    #
    # for i in range(1, 33):
    #     RequestFilterName = 'FilterActual' + str(i)
    #     newValue = 0
    #     if (newMask & math.pow(2, i - 1)) > 0:
    #         newValue = 1
    #     display.getWidget(RequestFilterName).setPropertyValue('pv_value', newValue)


main()