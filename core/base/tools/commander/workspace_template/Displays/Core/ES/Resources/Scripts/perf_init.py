from org.csstudio.opibuilder.scriptUtil import PVUtil, WidgetUtil, DataUtil
from org.eclipse.swt.graphics import RGB
from com.windhoverlabs.studio.registry import YAMLRegistry, ConfigRegistry
from org.csstudio.opibuilder.script import PVTuple
from org.yamcs.studio.data import IPVListener
from java.lang import Runnable
import math

PERF_ID_BATCH_COUNT = 32


FILTER_MASK_PV_BASE_NAME = '/cfs/'+display.getMacroValue("CPUID")+'/cfe_es/CFE_ES_HkPacket_t.Payload.PerfFilterMask'

def get_all_perf_ids(modules, out_perf_ids):
    # Might have to do this the old-fashion way; drill down the yaml(?)
    for module in modules:
        if 'modules' in modules[module]:
            get_all_perf_ids(modules[module], out_perf_ids)
        if 'perfids' in modules[module] and not (modules[module]['perfids'] is None):
            out_perf_ids.update(modules[module]['perfids'])


def get_perf_id(perf_ids, id):
    for perf_id in perf_ids:
        print('perf_id-->{}'.format(perf_id))
        if id == perf_ids[perf_id]['id']:
            return "{}:{}".format(id, str(perf_id))
    return "{}:{}".format(id, "")



def get_number_of_tabs(registry):
    """
    Gets the number of perf_collection tabs(32-row groups) that a user may have for configuring perf ids.
    The registry is assumed to have been loaded by the caller.

    For more details refer to airliner/core/base/cfe/docs/cFE UsersGuide/Doxygen/cfe__platform__cfg_8h-source.html#l01040.
    """
    registry_path = display.getMacroValue("REGISTRY_PATH")
    return registry.get(registry_path + '/config/CFE_ES_PERF_MAX_IDS/value')/32

def add_perf_records(tab_number, perf_ids):
    """

    """
    print("-->number of tabs:{}".format(tab_number))
    add_tabbed_container(tab_number)
    current_mask_number = 0
    current_perf_id = 1
    # display.getWidget("PerfTabbedContainer").getWidgetModel().setPropertyValue("tab_count", tab_number)
    for child in display.getWidget("PerfTabbedContainer").getWidgetModel().getChildren():
        #Add header
        new_perf_control = WidgetUtil.createWidgetModel("org.csstudio.opibuilder.widgets.linkingContainer")
        # TODO: Use the enumeration values from Studio to avoid magical strings
        new_perf_control.setPropertyValue("opi_file", display.getMacroValue("PerfCollectionControlTEMPLATE_OPI"))
        new_perf_control.setPropertyValue("height", 20)
        new_perf_control.setPropertyValue("width", 270)
        # Don't really like doing this, but if I use a layout Studio will place this widget wherever it wants; we lose
        # control over where this is positioned exactly, which matters a lot in this case.
        new_perf_control.setPropertyValue("x", 160)
        new_perf_control.setPropertyValue("y", 4)
        new_perf_control.setPropertyValue("background_color", RGB(255, 255, 255))
        new_perf_control.setPropertyValue("border_color", RGB(240, 240, 240))
        new_perf_control.setPropertyValue("border_style", 0)
        new_perf_control.setPropertyValue("name", "perf_control")
        # Refer to org.csstudio.opibuilder.widgets.model for resize_behaviour
        new_perf_control.setPropertyValue("resize_behaviour", 1)

        child.addChild(new_perf_control)

        # For the life of me, I cannot figure out why macros don't work with this pattern. I'll ask SpaceApp.
        new_perf_control.getChildByName("Grouping Container").getChildByName("Action Button_2").setPropertyValue(
            "pv_value", current_mask_number)
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
            new_perf_record.setPropertyValue("name", "perf_name")
            # Refer to org.csstudio.opibuilder.widgets.model for resize_behaviour
            new_perf_record.setPropertyValue("resize_behaviour", 1)

            child.getChildByName("PerfRecordContainer").addChild(new_perf_record)

            # Access children of new_perf_record only AFTER they are added to the display container
            new_perf_record.getChildByName("PerfRecordContainer").getChildByName("FilterActual").setPropertyValue("name", "FilterActual" + str(i))
            new_perf_record.getChildByName("PerfRecordContainer").getChildByName("FilterActual" + str(i)).setPropertyValue("pv_value", 0)

            new_perf_record.getChildByName("PerfRecordContainer").getChildByName("PerfId").setPropertyValue("text",
                                                                                                            get_perf_id(perf_ids, current_perf_id))

            current_perf_id = current_perf_id + 1



def add_tabbed_container(tab_number):
    new_tabbed_container = WidgetUtil.createWidgetModel("org.csstudio.opibuilder.widgets.tab")
    new_tabbed_container.setPropertyValue("name", "PerfTabbedContainer")
    new_tabbed_container.setPropertyValue("height", 739)
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
        print('child tab ')
        child.setPropertyValue("background_color", RGB(255, 255, 255))
        # Add the container for perf ids
        new_grouping_container = WidgetUtil.createWidgetModel("org.csstudio.opibuilder.widgets.groupingContainer")
        new_grouping_container.setPropertyValue("name", "PerfRecordContainer")
        new_grouping_container.setPropertyValue("height", 610)
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


class UI_Business(Runnable):
    """
    Any behavior that changes the state of widgets MUST happen inside this runnable function via
    ScriptUtil.execInUI.
    """

    def __init__(self, in_row, in_col, in_pv_data):
        self.ui_col = in_col
        self.ui_row = in_row
        self.pv_data = in_pv_data

    def run(self):
        display.getWidget("Table").getTable().setCellText(self.ui_row,
                                                          self.ui_col,
                                                          str(self.pv_data.getValue()))


class MyPVListener(IPVListener):
    def __init__(self, in_tab_number):
        self.tab_number = in_tab_number

    def valueChanged(self, pv):
        if not pv.isConnected():
            # FIXME: This will not execute. Must wrap around inside an UI Thread.
            widget.getTable().setCellText(0, 0, "Disconnected")
        else:
            print('triggered:{}'.format(self.tab_number))

            newMask = pv.getValue()

            current_tab = None

            tab_counter = 0
            for child in display.getWidget("PerfTabbedContainer").getChildren():
                current_tab = child.getWidgetModel()
                if tab_counter == self.tab_number:
                    break
                tab_counter = tab_counter + 1

            print('current:{}'.format(type(current_tab)))

            for i in range(1, PERF_ID_BATCH_COUNT+1):
                RequestFilterName = 'FilterActual' + str(i)
                print('i:{}'.format(i))
                newValue = 0
                print('#2')
                print('{}'.format(newMask))
                if (newMask & int(math.pow(2, i - 1)) )> 0:
                    print("")
                    # FIXME: This is crashing here, not sure why.
                    print("#3:{}".format(int(math.pow(2, i - 1))))
                # if (newMask & math.pow(2, i - 1)) > 0:
                #     newValue = 1
                # current_tab.getWidget(RequestFilterName).setPropertyValue('pv_value', 1)
            # ScriptUtil.execInUI(UI_Business(self.row, self.col, pv), widget)

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



        # script = display.getWidgetModel().getPropertyValue('scripts').getScriptList()[0]
        #
        for tab in range(tabs):
        #     script.addPV(PVTuple(FILTER_MASK_PV_BASE_NAME + "_{}_".format(str(tab)), True))
            pvName = FILTER_MASK_PV_BASE_NAME + "_{}_".format(tab)

            pv = PVUtil.createPV(pvName, display)
            new_listner = MyPVListener(tab)
            pv.addListener(new_listner)
        # script.addPV(PVTuple("/cfs//cfe_es/CFE_ES_HkPacket_t.Payload.CmdCounter", True))

    print('triggered')



    # for i in range(tabs):
    #
    #     try:
    #         newMask = PVUtil.get  Long(pvs[0])
    #
    #     for (var i = 1; i <= PERF_ID_BATCH_COUNT; ++i) {
    #         var RequestFilterName = 'FilterActual' + i;
    #     var newValue = 0;
    #     if ((newMask & Math.pow(2, i-1)) > 0) {
    #     newValue = 1;
    #     }
    #
    #     ConsoleUtil.writeInfo("logging");
    #     display.getWidget(RequestFilterName).setPropertyValue('pv_value', newValue);
    #     }
    #     except:
    #         print('There was an error while fetching the PV Values')


    # print('tabs children-->{}'.format(display.getWidget("PerfTabbedContainer").getWidgetModel().getChildren()[0].getPropertyValue("type_id")))

    # newMask = PVUtil.getLong(pvs[0])
    #
    # for i in range(1, 33):
    #     RequestFilterName = 'FilterActual' + str(i)
    #     newValue = 0
    #     if (newMask & math.pow(2, i - 1)) > 0:
    #         newValue = 1
    #     display.getWidget(RequestFilterName).setPropertyValue('pv_value', newValue)

# registry = YAMLRegistry()
#
# tabs = get_number_of_tabs(registry)
#


main()
