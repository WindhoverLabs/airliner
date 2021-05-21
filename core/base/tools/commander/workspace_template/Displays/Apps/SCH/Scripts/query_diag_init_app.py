"""
Requests a sch table for specific app from YAMCS and display on a template display.
"""

from com.windhoverlabs.studio.registry import YAMLRegistry, ConfigRegistry
from java.lang import Runnable
from org.yamcs.studio.data import IPVListener
from org.csstudio.opibuilder.scriptUtil import PVUtil, ScriptUtil, WidgetUtil, DataUtil
from com.windhoverlabs.studio.server.util import ServerUtil
from org.eclipse.swt.graphics import RGB
import logging

log = logging.getLogger("query_diag_init_app.py")


MSG_ID_PV = "/cfs/" + display.getMacroValue("CPUID") + "/sch/SCH_DiagPacket_t.MsgIDs"
ENTRY_STATE_PV = "/cfs/" + display.getMacroValue("CPUID") + "/sch/SCH_DiagPacket_t.EntryStates"
AGGREGATE_PARAM_PATH = "/cfs/" + display.getMacroValue("CPUID") + "/sch/SCH_DiagPacket_t"

app_name = display.getMacroValue("APP")

# Cache the registry to avoid long wait times for rendering
registry = YAMLRegistry()
all_messages = registry.getAllMessages()

# I think this should be done on the server side...
def get_id_for_msg_macro(msg_macro, in_messages):
    for msg in in_messages:
        if in_messages[msg]['macro'] == msg_macro:
            return in_messages[msg]['msgID']


def get_sch_record_widget(slot_number, entry_number):
    """
    Factory function that generates a new sch record widget using the SCH_Table_Record.opi template.

    params:
    slot_number(int): In this context a slot number is a minor frame. Used for Enable/Disable commands.
    entry_number(int): In this context an entry number if an activity number inside of a minor frame. Used for Enable/Disable commands

    returns:
    A org.csstudio.opibuilder.widgets.linkingContainer which points to the SCH_Table_Record.opi template.
    """
    new_sch_record = WidgetUtil.createWidgetModel("org.csstudio.opibuilder.widgets.linkingContainer")
    # TODO: Use the enumeration values from Studio to avoid magical strings
    new_sch_record.setPropertyValue("opi_file", display.getMacroValue("Record_Template"))
    # Refer to org.csstudio.opibuilder.widgets.model for resize_behaviour
    new_sch_record.setPropertyValue("resize_behaviour", 1)

    new_sch_record.setPropertyValue("border_color", RGB(240, 240, 240))
    new_sch_record.setPropertyValue("border_style", 0)

    sch_macros = DataUtil.createMacrosInput(False)
    sch_macros.put("SLOT_NUMBER", str(slot_number))
    sch_macros.put("ENTRY_NUMBER", str(entry_number))

    new_sch_record.setPropertyValue("macros", sch_macros)

    return new_sch_record


def add_sch_table_header():
    new_sch_table_header = WidgetUtil.createWidgetModel("org.csstudio.opibuilder.widgets.linkingContainer")
    # TODO: Use the enumeration values from Studio to avoid magical strings
    new_sch_table_header.setPropertyValue("opi_file", display.getMacroValue("Header_Template"))
    # Refer to org.csstudio.opibuilder.widgets.model for resize_behaviour
    new_sch_table_header.setPropertyValue("resize_behaviour", 1)

    new_sch_table_header.setPropertyValue("border_color", RGB(240, 240, 240))
    new_sch_table_header.setPropertyValue("border_style", 0)

    display.getWidget("SchTableContainer").addChild(new_sch_table_header)


def main():
    # Fill PV Name only once
    if widget.getVar("firstTime") == None:
        widget.setVar("firstTime", True)
        sch_table = ServerUtil.getSchTableForApp(AGGREGATE_PARAM_PATH, app_name)
        add_sch_table_header()

        if not(sch_table is None) and len(sch_table) > 0:
            for entry in sch_table['schEntry']:
                minor = entry['minor']
                activity_number = entry['activityNumber']
                message = entry['messageMacro']
                state = entry['state']

                new_sch_record = get_sch_record_widget(minor, activity_number)

                display.getWidget("SchTableContainer").addChild(new_sch_record)

                # Access children of new_sch_record only AFTER they are added to the display container
                new_sch_record.getChildByName('Sch_Table_Record').getChildByName('MinorLabel').setPropertyValue("text",
                                                                                                                minor)
                new_sch_record.getChildByName('Sch_Table_Record').getChildByName('ActivityLabel').setPropertyValue("text",
                                                                                                                activity_number)
                new_sch_record.getChildByName('Sch_Table_Record').getChildByName('MsgIdLabel').setPropertyValue("text",
                                                                                                                message)

                new_sch_record.getChildByName('Sch_Table_Record').getChildByName('MsgIdLabel').setPropertyValue('tooltip',
                                                                                                                get_id_for_msg_macro(message, all_messages))

                new_sch_record.getChildByName('Sch_Table_Record').getChildByName('StateLabel').setPropertyValue("text",
                                                                                                                state)

main()
