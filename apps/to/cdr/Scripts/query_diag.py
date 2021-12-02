from org.csstudio.opibuilder.scriptUtil import PVUtil, ScriptUtil
from org.yamcs.studio.data import IPVListener
from java.lang import Runnable
from org.eclipse.swt.widgets import Display

"""
There appears to be a difference between the Widget we see in YAMCS Studio as a user and the "widget" that gets
passed to this script. The widget that gets passed to this script is of the 
org.csstudio.opibuilder.widgets.editparts.TableEdi type. On the other hand, the widget that user deals with in Studio
is of the TableModel kind. I'm still investigating this.
"""

myTable = widget.getTable()

currentDisplay = Display.getCurrent()

# Number of rows
PQUEUE_COUNT = 8

# This could be automated/abstracted perhaps? Maybe a bit of a stretch.
PQUEUE_FIELDS = ['DroppedMsgCnt', 'QueuedMsgCnt', 'CurrentlyQueuedCnt',
                 'HighwaterMark', 'MsgLimit', 'State', 'QType']

# Hopefully there is a way to alias this stuff in Studio. And then perhaps all of thse python scripts could be auto-generated.
# FIXME: I really wish we could just access aggregates. Might be possible through the HTTP client API.
AGGREGATE_PV = "/cfs/"+display.getMacroValue("CPUID")+"/to/TO_ChannelDiagTlm_t.PQueue"


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
    def __init__(self, in_row, in_col, table):
        self.row = in_row
        self.col = in_col
        self.table = table

    def valueChanged(self, pv):
        if not pv.isConnected():
            # FIXME: This will not execute. Must wrap around inside an UI Thread.
            widget.getTable().setCellText(0, 0, "Disconnected")
        else:
            ScriptUtil.execInUI(UI_Business(self.row, self.col, pv), widget)

    def connectionChanged(self, pv):
        # FIXME:Figure out a way to log properly
        print("connection changed")

    def writePermissionChanged(self, pv):
        # FIXME:Figure out a way to log properly
        print("write permission changed")


def insert_rows(rows, table_widget):
    """
    Insert new empty rows into table_widget.
    :param rows(int): The number of rows to insert.
    :param table_widget(SpreadSheetTable): The table widget to insert rows into.
    """

    for row in range(rows):
        table_widget.appendRow()
        table_widget.setCellText(row, 0, str(row))


def main():
    # Fill PV Name only once
    if widget.getVar("firstTime") == None:
        widget.setVar("firstTime", True)
        insert_rows(PQUEUE_COUNT, myTable)
        aggregate_index = 0
        for queue_index in range(0, PQUEUE_COUNT, 1):
            #  This assumes the our array convention of AGGREGATE_{index}_
            for field in range(0, len(PQUEUE_FIELDS), 1):
                pvName = AGGREGATE_PV + "_{}_.".format(aggregate_index)
                pvName += PQUEUE_FIELDS[field]
                pv = PVUtil.createPV(pvName, widget)
                new_listner = MyPVListener(queue_index, field + 1, myTable)
                pv.addListener(new_listner)

            aggregate_index = aggregate_index + 1


main()
