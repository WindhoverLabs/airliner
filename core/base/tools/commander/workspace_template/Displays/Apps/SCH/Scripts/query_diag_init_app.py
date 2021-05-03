from com.windhoverlabs.studio.registry import YAMLRegistry, ConfigRegistry
from java.lang import Runnable
from org.yamcs.studio.data import IPVListener
from org.csstudio.opibuilder.scriptUtil import PVUtil, ScriptUtil

myTable = widget.getTable()

MSG_ID_PV = "/cfs/" + display.getMacroValue("CPUID") + "/sch/SCH_DiagPacket_t.MsgIDs"
ENTRY_STATE_PV = "/cfs/" + display.getMacroValue("CPUID") + "/sch/SCH_DiagPacket_t.EntryStates"

# This could be automated/abstracted perhaps? Maybe a bit of a stretch.
COLUMN_HEADERS = ['Minor', 'Activity', 'MsgId', 'State']

# Cache the registry to avoid long wait times for rendering
registry = YAMLRegistry()
all_messages = registry.getAllMessages()

config = registry.getAllConfig()

is_loading_done = False


def get_message_macro(msg_id, in_messages):
    """
    Return a message macro such as SCH_SEND_HK_MID corresponding to msg_id.
    If msg_id is not found in in_messages, "-----" is returned.
    """
    for tlm in in_messages:
        if in_messages[tlm]['msgID'] == msg_id:
            return in_messages[tlm]['macro']

    return '-----'


class UI_UpdateMessageID(Runnable):
    """
    Any behavior that changes the state of widgets MUST happen inside this runnable function via
    ScriptUtil.execInUI.

    This class calculates the minor and major frame that corresponds to the message id inside the PV value.
    """

    def __init__(self, in_row, in_col, in_pv_data, in_messages):
        self.ui_col = in_col
        self.ui_row = in_row
        self.pv_data = in_pv_data
        self.messages = in_messages

    def run(self):
        minor_frame = self.ui_row / config['sch']['SCH_ENTRIES_PER_SLOT']['value']
        activity_number = self.ui_row - (minor_frame * config['sch']['SCH_ENTRIES_PER_SLOT']['value'])
        msg_macro = get_message_macro(int(self.pv_data.getValue().getValue()), self.messages)

        display.getWidget("SCH_Diag_Table").getTable().setCellText(self.ui_row,
                                                                   0,
                                                                   str(minor_frame))
        display.getWidget("SCH_Diag_Table").getTable().setCellText(self.ui_row,
                                                                   1,
                                                                   str(activity_number))

        display.getWidget("SCH_Diag_Table").getTable().setCellText(self.ui_row,
                                                                   2,
                                                                   str(msg_macro))

        if self.ui_row + 1 == (
                config['sch']['SCH_ENTRIES_PER_SLOT']['value'] * config['sch']['SCH_TOTAL_SLOTS']['value']):
            display.getWidget('SCH_Diag_Table').setPropertyValue('visible', True)
            print('row:{}'.format(self.ui_row))
            print('LAST message:{}'.format(self.pv_data))


class UI_UpdateMessageState(Runnable):
    """
    Any behavior that changes the state of widgets MUST happen inside this runnable function via
    ScriptUtil.execInUI.

    This class calculates the scheduled state that corresponds to the message id inside the PV value.
    """

    def __init__(self, in_begin_row, in_bit_index, in_pv_data):
        self.ui_begin_row = in_begin_row
        self.bit_index = in_bit_index
        self.pv_data = in_pv_data

    def run(self):
        display.getWidget("SCH_Diag_Table").getTable().setCellText(self.ui_begin_row,
                                                                   3,
                                                                   str(get_sch_entry_state(
                                                                       self.pv_data.getValue().getValue(),
                                                                       self.bit_index)))

        # print('state update:{}'.format(self.pv_data))
        if self.ui_begin_row + 1 == (
                config['sch']['SCH_ENTRIES_PER_SLOT']['value'] * config['sch']['SCH_TOTAL_SLOTS']['value']):
            display.getWidget('SCH_Diag_Table').setPropertyValue('visible', True)
            display.getWidget("SCH_Diag_Table").getTable().autoSizeColumns()
            print('display.getWidget("SCH_Diag_Table").getTable():{}'.format(
                display.getWidget("SCH_Diag_Table").getTable().getContent()))
            print('row2:{}'.format(self.ui_begin_row))
            print('LAST state:{}'.format(self.pv_data))


def get_sch_entry_state(entry_state, bit_index):
    # Tested on Little endian machine at the moment. Probably not the best way of doing this.
    state = format(entry_state, '#018b')[2:][bit_index: bit_index + 2]
    if state == '01':
        state = 'ENABLED'
    elif state == '10':
        state = 'DISABLED'
    else:
        state = 'UNUSED'

    return state


class MyPVListenerState(IPVListener):
    def __init__(self, in_row, in_bit_index, table):
        self.row = in_row
        self.bit_index = in_bit_index
        self.table = table

    def connectionChanged(self, pv):
        # FIXME:Figure out a way to log properly
        pass
        # print("connection changed")

    def writePermissionChanged(self, pv):
        # FIXME:Figure out a way to log properly
        pass
        # print("write permission changed")

    def valueChanged(self, pv):
        # print(frames_load_counter)
        if not pv.isConnected():
            # FIXME: This will not execute. Must wrap around inside an UI Thread.
            widget.getTable().setCellText(0, 0, "Disconnected")
        else:
            ScriptUtil.execInUI(UI_UpdateMessageState(self.row, self.bit_index,
                                                      pv), widget)


class MyPVListener(IPVListener):
    def __init__(self, in_row, in_col, table, in_messages):
        self.row = in_row
        self.col = in_col
        self.table = table
        self.messages = in_messages

    def valueChanged(self, pv):
        if not pv.isConnected():
            # FIXME: This will not execute. Must wrap around inside an UI Thread.
            widget.getTable().setCellText(0, 0, "Disconnected")
        else:
            # print('triggered')
            ScriptUtil.execInUI(UI_UpdateMessageID(self.row, self.col, pv, self.messages), widget)

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

        SCH_TABLE_ENTRIES = config['sch']['SCH_TOTAL_SLOTS']['value'] * \
                            config['sch']['SCH_ENTRIES_PER_SLOT']['value']

        # In this context the TblIndex is the index of the MsgID in the SCH table.
        for TblIndex in range(SCH_TABLE_ENTRIES):
            pvName = MSG_ID_PV + "_{}_.".format(TblIndex)
            pv = PVUtil.createPV(pvName, widget)
            new_listener = MyPVListener(TblIndex, 0, myTable, all_messages)
            pv.addListener(new_listener)

        i = 0
        while i < int(SCH_TABLE_ENTRIES):
            WordIndex = i / 8

            pvName = ENTRY_STATE_PV + "_{}_.".format(WordIndex)

            pv = PVUtil.createPV(pvName, widget)
            frame_row = i
            BitIndex = 0
            for j in range(8):
                # BitIndex = (7 - (frame_row % 8)) * 2
                if frame_row < SCH_TABLE_ENTRIES:
                    new_listener = MyPVListenerState(frame_row, BitIndex, myTable)
                    pv.addListener(new_listener)

                frame_row = frame_row + 1
                BitIndex = BitIndex + 2

            i = i + 8


main()
