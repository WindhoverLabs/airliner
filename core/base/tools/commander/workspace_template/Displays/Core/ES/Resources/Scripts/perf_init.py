from org.csstudio.opibuilder.scriptUtil import PVUtil, WidgetUtil, DataUtil, ScriptUtil
from org.eclipse.swt.graphics import RGB
from com.windhoverlabs.studio.registry import YAMLRegistry, ConfigRegistry
from org.csstudio.opibuilder.script import PVTuple
from org.yamcs.studio.data import IPVListener
from java.lang import Runnable
import math

# FIXME: Cleanup this script, it looks ugly. I'm thinking of making everything a Macro, especially those widget names.
# That way users can even have more flexibility when creating templates in the GUI.

PERF_ID_BATCH_COUNT = 32

FILTER_MASK_PV_BASE_NAME = '/cfs/' + display.getMacroValue("CPUID") + '/cfe_es/CFE_ES_HkPacket_t.Payload.PerfFilterMask'
TRIGGER_MASK_PV_BASE_NAME = '/cfs/' + display.getMacroValue("CPUID") + '/cfe_es/CFE_ES_HkPacket_t.Payload.PerfTriggerMask'
FILTER_ACTUAL_BASENAME = "FilterActual"
FILTER_REQUEST_BASENAME = "FilterRequest"
TRIGGER_ACTUAL_BASENAME = "TriggerActual"
TRIGGER_REQUEST_BASENAME = "TriggerRequest"

def get_all_perf_ids(modules, out_perf_ids):
    # Might have to do this the old-fashion way; drill down the yaml(?)
    for module in modules:
        if 'modules' in modules[module]:
            get_all_perf_ids(modules[module], out_perf_ids)
        if 'perfids' in modules[module] and not (modules[module]['perfids'] is None):
            out_perf_ids.update(modules[module]['perfids'])


def get_perf_id(perf_ids, id):
    for perf_id in perf_ids:
        if id == perf_ids[perf_id]['id']:
            return "{}:{}".format(id, str(perf_id))
    return "{}: {}".format(id, "")


def get_number_of_tabs(registry):
    """
    Gets the number of perf_collection tabs(32-row groups) that a user may have for configuring perf ids.
    The registry is assumed to have been loaded by the caller.

    For more details refer to airliner/core/base/cfe/docs/cFE UsersGuide/Doxygen/cfe__platform__cfg_8h-source.html#l01040.
    """
    registry_path = display.getMacroValue("REGISTRY_PATH")
    out_tabs = registry.get(registry_path + '/config/CFE_ES_PERF_MAX_IDS/value') / 32

    # FIXME: At the moment there is a bug in Studio where the amount of tabs has to be 2 or more, otherwise it does
    # not add any child containers to the tabbed containers.
    if out_tabs < 2:
        out_tabs = 2

    return out_tabs


def add_perf_records(tab_number, perf_ids):
    """

    """
    add_tabbed_container(tab_number)
    current_mask_number = 0
    current_perf_id = 1
    for child in display.getWidget("PerfTabbedContainer").getWidgetModel().getChildren():
        # print("$2")
        # Add header
        new_perf_control = WidgetUtil.createWidgetModel("org.csstudio.opibuilder.widgets.linkingContainer")
        # TODO: Use the enumeration values from Studio to avoid magical strings
        new_perf_control.setPropertyValue("opi_file", display.getMacroValue("PerfCollectionControlTEMPLATE_OPI"))
        new_perf_control.setPropertyValue("height", 20)
        new_perf_control.setPropertyValue("width", 270)
        # Don't really like doing this, but if I use a layout Studio will place this widget wherever it wants; we lose
        # control over where this is positioned exactly, which matters a lot in this case.
        new_perf_control.setPropertyValue("x", 222)
        new_perf_control.setPropertyValue("y", 4)
        new_perf_control.setPropertyValue("background_color", RGB(255, 255, 255))
        new_perf_control.setPropertyValue("border_color", RGB(240, 240, 240))
        new_perf_control.setPropertyValue("border_style", 0)
        new_perf_control.setPropertyValue("name", "perf_control")
        # Refer to org.csstudio.opibuilder.widgets.model for resize_behaviour
        new_perf_control.setPropertyValue("resize_behaviour", 1)

        child.addChild(new_perf_control)

        new_perf_control.getChildByName("Grouping Container").getChildByName("SetFilterButton").setPropertyValue(
            "pv_value", current_mask_number)

        # FIXME: Enforce these names in code; do not make assumptions with these names.
        new_perf_control.getChildByName("Grouping Container").getChildByName("FilterRequestAll").setPropertyValue(
            "pv_name", "loc://FilterRequestAll_AUTO" + str(current_mask_number))
        new_perf_control.getChildByName("Grouping Container").getChildByName("TriggerActualAll").setPropertyValue(
            "pv_name", "loc://TriggerRequestAll_AUTO" + str(current_mask_number))


        # Pass some macros to perf_control widget
        perf_control_macros = DataUtil.createMacrosInput(True)
        perf_control_macros.put("TAB_NUMBER", str(current_mask_number))
        perf_control_macros.put("REQUEST_FILTER_WIDGET_NAME", str(FILTER_REQUEST_BASENAME))
        perf_control_macros.put("REQUEST_TRIGGER_WIDGET_NAME", str(TRIGGER_REQUEST_BASENAME))


        new_perf_control.getChildByName("Grouping Container").setPropertyValue("macros", perf_control_macros)

        pvName = "loc://FilterRequestAll_AUTO" + str(current_mask_number)
        pv = PVUtil.createPV(pvName, display)
        new_listner = MyPVListener2(current_mask_number, "FilterRequest")
        pv.addListener(new_listner)

        pvName = "loc://TriggerRequestAll_AUTO" + str(current_mask_number)
        pv = PVUtil.createPV(pvName, display)
        new_listner = MyPVListener2(current_mask_number, "TriggerRequest")
        pv.addListener(new_listner)

        current_mask_number = current_mask_number + 1
        for i in range(PERF_ID_BATCH_COUNT):
            new_perf_record = WidgetUtil.createWidgetModel("org.csstudio.opibuilder.widgets.linkingContainer")
            # TODO: Use the enumeration values from Studio to avoid magical strings
            new_perf_record.setPropertyValue("opi_file", display.getMacroValue("PerfRecordTEMPLATE_OPI"))
            new_perf_record.setPropertyValue("height", 20)
            new_perf_record.setPropertyValue("width", 680)
            new_perf_record.setPropertyValue("background_color", RGB(255, 255, 255))
            new_perf_record.setPropertyValue("border_color", RGB(240, 240, 240))
            new_perf_record.setPropertyValue("border_style", 0)
            new_perf_record.setPropertyValue("name", "perf_record_link")
            # Refer to org.csstudio.opibuilder.widgets.model for resize_behaviour
            new_perf_record.setPropertyValue("resize_behaviour", 1)

            child.getChildByName("PerfRecordContainer").addChild(new_perf_record)

            filter_actual_widget_name = FILTER_ACTUAL_BASENAME

            # Access children of new_perf_record only AFTER they are added to the display container
            # FIXME: Please put these names in const variables.

            new_perf_record.getChildByName("PerfRecord").getChildByName(FILTER_ACTUAL_BASENAME).setPropertyValue("name", FILTER_ACTUAL_BASENAME )

            new_perf_record.getChildByName("PerfRecord").getChildByName(filter_actual_widget_name).setPropertyValue("pv_name",
                                                                                                         "loc://FilterActual_AUTO" + str(
                                                                                                             current_perf_id))
            new_perf_record.getChildByName("PerfRecord").getChildByName(filter_actual_widget_name).setPropertyValue("pv_value", 0)

            new_perf_record.getChildByName("PerfRecord").getChildByName("FilterRequest").setPropertyValue("pv_name",
                                                                                                          "loc://FilterRequest_AUTO" + str(
                                                                                                              current_perf_id))
            new_perf_record.getChildByName("PerfRecord").getChildByName("FilterRequest").setPropertyValue("pv_value", 0)

            # Access children of new_perf_record only AFTER they are added to the display container
            new_perf_record.getChildByName("PerfRecord").getChildByName("TriggerActual").setPropertyValue("pv_name",
                                                                                                          "loc://TriggerActual_AUTO" + str(
                                                                                                              current_perf_id))
            new_perf_record.getChildByName("PerfRecord").getChildByName("TriggerActual").setPropertyValue("pv_value", 0)

            new_perf_record.getChildByName("PerfRecord").getChildByName("TriggerRequest").setPropertyValue("pv_name",
                                                                                                           "loc://TriggerRequest_AUTO" + str(
                                                                                                               current_perf_id))
            new_perf_record.getChildByName("PerfRecord").getChildByName("TriggerRequest").setPropertyValue("pv_value",
                                                                                                           0)

            new_perf_record.getChildByName("PerfRecord").getChildByName("PerfId").setPropertyValue("text",
                                                                                                   get_perf_id(perf_ids,
                                                                                                               current_perf_id))

            current_perf_id = current_perf_id + 1


def add_tabbed_container(tab_number):
    new_tabbed_container = WidgetUtil.createWidgetModel("org.csstudio.opibuilder.widgets.tab")
    new_tabbed_container.setPropertyValue("name", "PerfTabbedContainer")
    new_tabbed_container.setPropertyValue("height", 800)
    new_tabbed_container.setPropertyValue("width", 564)
    # Don't really like doing this, but if I use a layout Studio will place this widget wherever it wants; we lose
    # control over where this is positioned exactly, which matters a lot in this case.
    new_tabbed_container.setPropertyValue("x", 19)
    new_tabbed_container.setPropertyValue("y", 150)

    display.getWidgetModel().addChild(new_tabbed_container)

    # Have to set the tab_count after the tabbed container is added to the display, otherwise Studio sets the count to 2
    new_tabbed_container.setPropertyValue("tab_count", tab_number)

    current_mask_number = 0
    for child in new_tabbed_container.getChildren():
        # print('child tab ')
        child.setPropertyValue("background_color", RGB(255, 255, 255))
        # Add the container for perf ids
        new_grouping_container = WidgetUtil.createWidgetModel("org.csstudio.opibuilder.widgets.groupingContainer")
        new_grouping_container.setPropertyValue("name", "PerfRecordContainer")
        new_grouping_container.setPropertyValue("height", 700)
        new_grouping_container.setPropertyValue("width", 552)
        # Don't really like doing this, but if I use a layout Studio will place this widget wherever it wants; we lose
        # control over where this is positioned exactly, which matters a lot in this case.
        new_grouping_container.setPropertyValue("x", 0)
        new_grouping_container.setPropertyValue("y", 96)
        new_grouping_container.setPropertyValue("background_color", RGB(255, 255, 255))

        # Add a gridLayout the container for perf ids
        new_grouping_container_layout = WidgetUtil.createWidgetModel("org.csstudio.opibuilder.widgets.gridLayout")
        new_grouping_container_layout.setPropertyValue("name", "PerfRecordContainerLayout")
        new_grouping_container_layout.setPropertyValue("number_of_columns", 1)
        new_grouping_container_layout.setPropertyValue("grid_gap", 0)

        new_grouping_container.addChild(new_grouping_container_layout)
        child.addChild(new_grouping_container)
        current_mask_number = current_mask_number + 1


class UICheckAll(Runnable):
    """
    Any behavior that changes the state of widgets MUST happen inside this runnable function via ScriptUtil.execInUI.
    Iterates through a list of widgets in PerfRecordContainer such as checkboxes and updates their value with the
    pv_data provided. Very useful for having widgets that update the state of all widgets in one column,
    such as a column of checkboxes.
    """

    def __init__(self, in_pv_data, in_tab_count, in_widget_name):
        self.pv_data = in_pv_data
        self.tab_count = in_tab_count
        self.widget_name = in_widget_name

    def run(self):
        current_tab = None
        tab_counter = 0

        # Have to do this ugly hack because we can't get the tab by index.
        for child in display.getWidget("PerfTabbedContainer").getChildren():
            current_tab = child.getWidgetModel()
            if tab_counter == self.tab_count:
                break
            tab_counter = tab_counter + 1

        # Calculate the perf ids(which are used for pv values) for this specific tab.
        begin_pv_name_number = (self.tab_count * 32) + 1
        end_pv_name_number = begin_pv_name_number + 32
        for i in range(begin_pv_name_number, end_pv_name_number):
            for linking_record in current_tab.getChildByName("PerfRecordContainer").getChildren():
                # Ensure that we don't touch the GridLayout
                if linking_record.getPropertyValue("widget_type") == 'Linking Container':
                    # print(linking_record.getChildren()[0].getPropertyValue("name"))
                    perf_record = linking_record.getChildByName("PerfRecord")
                    perf_record.getChildByName(self.widget_name).setPropertyValue('pv_value',
                                                                                  int(PVUtil.getLong(self.pv_data)))


class UI_Business(Runnable):
    """
    Any behavior that changes the state of widgets MUST happen inside this runnable function via
    ScriptUtil.execInUI.
    """

    def __init__(self, in_pv_data, in_tab_count, in_widget_name):
        self.pv_data = in_pv_data
        self.tab_count = in_tab_count
        self.widget_name = in_widget_name

    def run(self):
        # Readers should note that getValue() returns org.yamcs.studio.data.yamcs.Uint32VType, NOT a native int.
        # So when using the value one might find it useful to call PVUtil.getLong(newMask) on it.
        newMask = self.pv_data
        current_tab = None
        tab_counter = 0

        newValue = 0

        # Have to do this ugly hack because we can't get the tab by index.
        for child in display.getWidget("PerfTabbedContainer").getChildren():
            current_tab = child.getWidgetModel()
            if tab_counter == self.tab_count:
                break
            tab_counter = tab_counter + 1

        i = 0

        for linking_record in current_tab.getChildByName("PerfRecordContainer").getChildren():
            if linking_record.getPropertyValue("widget_type") == 'Linking Container':
                newValue = 0
                v = (PVUtil.getLong(newMask)) & (long(math.pow(2, i)))
                perf_record = linking_record.getChildByName("PerfRecord")
                if v > 0:
                    newValue = 1
                perf_record.getChildByName(self.widget_name).setPropertyValue(
                    'pv_value', newValue)
            i = i + 1


# FIXME: Maybe I should combine these two listeners into 1?
class MyPVListener(IPVListener):
    def __init__(self, in_tab_number, in_widget_name):
        self.tab_number = in_tab_number
        self.widget_name = in_widget_name

    def valueChanged(self, pv):
        if not pv.isConnected():
            # FIXME: This will not execute. Must wrap around inside an UI Thread.
            widget.getTable().setCellText(0, 0, "Disconnected")
        else:

            ScriptUtil.execInUI(UI_Business(pv, self.tab_number, self.widget_name), widget)

    def connectionChanged(self, pv):
        # FIXME:Figure out a way to log properly
        print("connection changed")

    def writePermissionChanged(self, pv):
        # FIXME:Figure out a way to log properly
        print("write permission changed")


class MyPVListener2(IPVListener):
    def __init__(self, in_tab_number, in_widget_name):
        self.tab_number = in_tab_number
        self.widget_name = in_widget_name

    def valueChanged(self, pv):
        if not pv.isConnected():
            # FIXME: This will not execute. Must wrap around inside an UI Thread.
            widget.getTable().setCellText(0, 0, "Disconnected")
        else:

            ScriptUtil.execInUI(UICheckAll(pv, self.tab_number, self.widget_name), widget)

    def connectionChanged(self, pv):
        # FIXME:Figure out a way to log properly
        print("connection changed")

    def writePermissionChanged(self, pv):
        # FIXME:Figure out a way to log properly
        print("write permission changed")


def main():
    # Generate the UI the first time the OPI is invoked

    registry = YAMLRegistry()
    # FIXME: Would like to add titles to each tab from the script. At the moment this is not possible. This is
    # because TabItem objects are not AbstractContainerModel themselves, they use composition to contain
    # the AbstractContainerModel object. Which is very is odd and limits the capabilities of what is possible(to some extent)
    # in Studio scripts.
    # tab = WidgetUtil.createWidgetModel("org.csstudio.opibuilder.widgets.editparts.TabItem")

    perf_ids = dict()

    tabs = get_number_of_tabs(registry)
    get_all_perf_ids(registry.get('/')['modules'], perf_ids)
    if widget.getVar("firstTime") == None:
        widget.setVar("firstTime", True)
        add_perf_records(tabs, perf_ids)
        for tab in range(tabs):
            #     script.addPV(PVTuple(FILTER_MASK_PV_BASE_NAME + "_{}_".format(str(tab)), True))
            pvName = FILTER_MASK_PV_BASE_NAME + "_{}_".format(tab)
            # print('pv_name-->{}'.format(pvName))
            pv = PVUtil.createPV(pvName, display)
            new_listner = MyPVListener(tab, FILTER_ACTUAL_BASENAME)
            pv.addListener(new_listner)

            pvName = TRIGGER_MASK_PV_BASE_NAME + "_{}_".format(tab)
            # print('pv_name-->{}'.format(pvName))
            pv = PVUtil.createPV(pvName, display)
            new_listner = MyPVListener(tab, TRIGGER_ACTUAL_BASENAME)
            pv.addListener(new_listner)
main()
