import sys

from org.yamcs.studio.data import IPVListener
from org.csstudio.opibuilder.scriptUtil import PVUtil, FileUtil, ScriptUtil

from com.windhoverlabs.studio.registry import YAMLRegistry, ConfigRegistry


# import our own APIs. Not sure if this is the cleanest way of doing this. If we don't do it this way, we might have add
# an __init__.py to every directory in the project. Don't want to hardcode this path either; open to ideas about this.
sys.path.append(FileUtil.workspacePathToSysPath("Displays"))
from Resources.opi_util import util, time_util

from java.lang import Runnable

"""
  Please reference airliner/core/base/cfe/docs/cFE UsersGuide/Doxygen/cfetimeugformat.html for details.
  
  International Atomic Time (TAI)
  Coordinated Universal Time (UTC)
  
  TAI = MET + STCF 
  UTC = TAI - Leap Seconds
  UTC = MET + STCF - Leap Seconds
"""

def format_met_time(met_time):
    pass


class UI_Business(Runnable):
    """
    Any behavior that changes the state of widgets MUST happen inside this runnable function via
    ScriptUtil.execInUI.
    """

    def __init__(self, in_pv_data):
        # self.ui_col = in_col
        # self.ui_row = in_row
        self.pv_data = in_pv_data

    def format_met_time(self, met_time):
        registry = YAMLRegistry()

        registry_path = display.getMacroValue("REGISTRY_PATH")

        NumberOfMinutes = (met_time['Seconds'] / 60) + registry.get("/modules/core/modules/cfe/modules/cfe_time/config/CFE_TIME_EPOCH_MINUTE/value")
        NumberOfSeconds = (met_time['Seconds'] % 60) + registry.get("/modules/core/modules/cfe/modules/cfe_time/config/CFE_TIME_EPOCH_SECOND/value")

        print('met_data: mins:{}, hours:{}'.format(NumberOfMinutes, NumberOfSeconds))

    def run(self):
        print("PV listener triggered:{}".format(self.format_met_time(self.pv_data)))

class MyPVListener(IPVListener):
    def __init__(self, widget):
        self.table = widget

    def valueChanged(self, pv):
        if not pv.isConnected():
            # FIXME: This will not execute. Must wrap around inside an UI Thread.
            widget.getTable().setCellText(0, 0, "Disconnected")
        else:
            ScriptUtil.execInUI(UI_Business(pv), widget)

    def connectionChanged(self, pv):
        # FIXME:Figure out a way to log properly
        print("connection changed")

    def writePermissionChanged(self, pv):
        # FIXME:Figure out a way to log properly
        print("write permission changed")


def main():
    registry = YAMLRegistry()
    time_util.get_cfe_time_fs_factor(registry, display.getMacroValue("REGISTRY_PATH"))
    # Fill PV Name only once
    if widget.getVar("firstTime") == None:
        widget.setVar("firstTime", True)
    MET_TIME_PV = "/cfe_time/CFE_TIME_DiagPacket_t.Payload.CurrentMET"
    pv = PVUtil.createPV(MET_TIME_PV, widget)
    new_listner = MyPVListener(widget)
    pv.addListener(new_listner)


main()
