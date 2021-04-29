# for (TblIndex=0; TblIndex < SCH_TABLE_ENTRIES; TblIndex++)
#     {
#         TableEntry = & SCH_AppData.ScheduleTable[TblIndex];
#     WordIndex = TblIndex / 8;
#     BitIndex = (7 - (TblIndex % 8)) * 2;
#
#     if (TableEntry->EnableState == SCH_ENABLED)
#     {
#         SCH_AppData.DiagPacket.EntryStates[WordIndex] |= (1 << BitIndex);
#     SCH_AppData.DiagPacket.MsgIDs[TblIndex] = CFE_SB_GetMsgId((CFE_SB_MsgPtr_t) & SCH_AppData.MessageTable[SCH_AppData.ScheduleTable[TblIndex].MessageIndex]);
#     }
#     else if (TableEntry->EnableState == SCH_DISABLED)
#     {
#     SCH_AppData.DiagPacket.EntryStates[WordIndex] |= (2 << BitIndex);
#     SCH_AppData.DiagPacket.MsgIDs[TblIndex] =
#     CFE_SB_GetMsgId((CFE_SB_MsgPtr_t) & SCH_AppData.MessageTable[SCH_AppData.ScheduleTable[TblIndex].MessageIndex]);
#     }
#     else
#     {
#     SCH_AppData.DiagPacket.MsgIDs[TblIndex] = 0x0000;
#     }
#     }
from com.windhoverlabs.studio.registry import YAMLRegistry, ConfigRegistry
from java.lang import Runnable
from org.yamcs.studio.data import IPVListener
from org.csstudio.opibuilder.scriptUtil import PVUtil, ScriptUtil

myTable = widget.getTable()

# "/cfs//sch/SCH_DiagPacket_t.MsgIDs_0_"

MSG_ID_PV = "/cfs/" + display.getMacroValue("CPUID") + "/sch/SCH_DiagPacket_t.MsgIDs"
ENTRY_STATE_PV = "/cfs/" + display.getMacroValue("CPUID") + "/sch/SCH_DiagPacket_t.MsgIDs"

# This could be automated/abstracted perhaps? Maybe a bit of a stretch.
COLUMN_HEADERS = ['Major', 'Minor', 'MsgId', 'State']

# Cache the registry to avoid long wait times for rendering
registry = YAMLRegistry()
all_tlm = registry.getAllTelemetry()


def get_message_macro(msg_id):
    # print('get_message_macro')
    for tlm in all_tlm:
        # print('iter')
        # print('msgId:{}'.format(type(msg_id)))
        print('msgId dict:{}'.format((all_tlm[tlm])))
        if all_tlm[tlm]['msgID'] == msg_id:
            # print(all_tlm[tlm]['id'])
            return all_tlm[tlm]['macro']

    return 'UNKNOWN'


class UI_UpdateMessageID(Runnable):
    """
    Any behavior that changes the state of widgets MUST happen inside this runnable function via
    ScriptUtil.execInUI.
    """

    def __init__(self, in_row, in_col, in_pv_data):
        self.ui_col = in_col
        self.ui_row = in_row
        self.pv_data = in_pv_data

    def run(self):
        WordIndex = self.ui_row / 8
        BitIndex = (7 - (self.ui_row % 8)) * 2
        display.getWidget("SCH_Diag_Table").getTable().setCellText(self.ui_row,
                                                                   2,
                                                                   str(self.pv_data.getValue()))

        display.getWidget("SCH_Diag_Table").getTable().setCellText(self.ui_row,
                                                                   0,
                                                                   str(self.ui_row))

        # print('before...{}'.format(type(self.pv_data.getValue().getValue())))
        msg_macro = get_message_macro(int(self.pv_data.getValue().getValue()))

        if msg_macro != 'UNKNOWN':
            print('MATCH...')
        # print()


class MyPVListener(IPVListener):
    def __init__(self, in_row, in_col, table):
        self.row = in_row
        self.col = in_col
        self.table = table

    def valueChanged(self, pv):
        if not pv.isConnected():
            # FIXME: This will not execute. Must wrap around inside an UI Thread.
            widget.getTable().setCellText(0, 0, "Disconnected")
        else:
            # print('triggered')
            ScriptUtil.execInUI(UI_UpdateMessageID(self.row, self.col, pv), widget)

    def connectionChanged(self, pv):
        # FIXME:Figure out a way to log properly
        pass
        # print("connection changed")

    def writePermissionChanged(self, pv):
        # FIXME:Figure out a way to log properly
        pass
        # print("write permission changed")


def main():
    # Fill PV Name only once
    if widget.getVar("firstTime") == None:
        widget.setVar("firstTime", True)

        print('triggered first time')

        # registry = YAMLRegistry()
        # sch_config = registry.getAllConfig()['sch']

        SCH_TABLE_ENTRIES = registry.getAllConfig()['sch']['SCH_TOTAL_SLOTS']['value'] * \
                            registry.getAllConfig()['sch']['SCH_ENTRIES_PER_SLOT']['value']

        # In this context the TblIndex is the index of the MsgID in the SCH table.
        for TblIndex in range(SCH_TABLE_ENTRIES):
            # for field in range(0, len(PQUEUE_FIELDS), 1):

            pvName = MSG_ID_PV + "_{}_.".format(TblIndex)
            pv = PVUtil.createPV(pvName, widget)
            new_listner = MyPVListener(TblIndex, 0, myTable)
            pv.addListener(new_listner)

            # pvName = MSG_ID_PV + "_{}_.".format(TblIndex)
            # pv = PVUtil.createPV(pvName, widget)
            # new_listner = MyPVListener(TblIndex, 0 , myTable)
            # pv.addListener(new_listner)

            # print('TblIndex:{}'.format(TblIndex))

            # EntryState =

            # print('word index-->{}'.format(WordIndex))
            # print('bit index-->{}'.format(BitIndex))

        # print('diag msg:{}'.format(pvs[0].getValue()))
        # print('diag sch table number of entries:{}'.format(SCH_TABLE_ENTRIES))


main()
