from org.csstudio.opibuilder.scriptUtil import PVUtil, WidgetUtil
from org.eclipse.swt.graphics import RGB
from com.windhoverlabs.studio.registry import YAMLRegistry, ConfigRegistry


def get_all_perf_ids(modules, out_perf_ids):
    # Might have to do this the old-fashion way; drill down the yaml(?)
    for module in modules:
        if 'modules' in modules[module]:
            get_all_perf_ids(modules[module], out_perf_ids)
        if 'perfids' in modules[module] and not (modules[module]['perfids'] is None):
            out_perf_ids.update(modules[module]['perfids'])


def get_number_of_tabs(registry):
    """
    Gets the number of perf_collection tabs(32-row groups) that a user may have for configuring perf ids.
    The registry is assumed to have been loaded by the caller.

    For more details refer to airliner/core/base/cfe/docs/cFE UsersGuide/Doxygen/cfe__platform__cfg_8h-source.html#l01040.
    """
    registry_path = display.getMacroValue("REGISTRY_PATH")
    # return registry.get(registry_path + '/config/CFE_ES_PERF_MAX_IDS/value')/32

    return 3

def add_perf_records(tab_number):
    """

    """
    pass

def main():

    if widget.getVar("firstTime") == None:
        widget.setVar("firstTime", True)

        registry = YAMLRegistry()

        perf_ids = dict()

        get_all_perf_ids(registry.get('/')['modules'], perf_ids)

        # display.getWidget("PerfTabbedContainer").getWidgetModel().setPropertyValue("tab_count", get_number_of_tabs(registry))

        for perf_id in perf_ids:
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

            new_perf_record.getChildByName("PerfRecordContainer").getChildByName("PerfId").setPropertyValue("text", perf_id)

        print('tabs children-->{}'.format(display.getWidget("PerfTabbedContainer").getWidgetModel().getChildren()))

        # newMask = PVUtil.getLong(pvs[0])
        #
        # for i in range(1, 33):
        #     RequestFilterName = 'FilterActual' + str(i)
        #     newValue = 0
        #     if (newMask & math.pow(2, i - 1)) > 0:
        #         newValue = 1
        #     display.getWidget(RequestFilterName).setPropertyValue('pv_value', newValue)


main()
