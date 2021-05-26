from com.windhoverlabs.studio.registry import YAMLRegistry, ConfigRegistry
from java.lang import Runnable
from org.yamcs.studio.data import IPVListener
from org.csstudio.opibuilder.scriptUtil import PVUtil, ScriptUtil
from com.windhoverlabs.studio.server.util import ServerUtil

myTable = widget.getTable()

MSG_ID_PV = "/cfs/" + display.getMacroValue("CPUID") + "/sch/SCH_DiagPacket_t.MsgIDs"
ENTRY_STATE_PV = "/cfs/" + display.getMacroValue("CPUID") + "/sch/SCH_DiagPacket_t.EntryStates"
AGGREGATE_PARAM_PATH = "/cfs/" + display.getMacroValue("CPUID") + "/sch/SCH_DiagPacket_t"

app_name = display.getMacroValue("APP")


def main():
    # Fill PV Name only once
    if widget.getVar("firstTime") == None:
        widget.setVar("firstTime", True)
        print('app:', app_name)
        sch_table = ServerUtil.getSchTableForApp(AGGREGATE_PARAM_PATH, app_name)
        print('sch table:', sch_table)
        if len(sch_table) > 0:
            current_row = 0
            for entry in sch_table['schEntries']:
                minor = entry['minor']
                activity_number = entry['activityNumber']
                message = entry['messageMacro']
                state = entry['state']

                display.getWidget("SCH_Diag_Table").getTable().setCellText(current_row, 0, minor)
                display.getWidget("SCH_Diag_Table").getTable().setCellText(current_row, 1, activity_number)
                display.getWidget("SCH_Diag_Table").getTable().setCellText(current_row, 2, message)
                display.getWidget("SCH_Diag_Table").getTable().setCellText(current_row, 3, state)
                current_row = current_row + 1

        display.getWidget('SCH_Diag_Table').setPropertyValue('visible', True)

main()
