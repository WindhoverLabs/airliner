from org.csstudio.opibuilder.scriptUtil import PVUtil, ScriptUtil, FileUtil, WidgetUtil, DataUtil, FileUtil
from org.eclipse.swt.graphics import RGB
from org.yamcs.studio.script import Yamcs


directory_arg = str(display.getWidget("Directory").getPropertyValue("pv_value").getValue())
list_offset_arg = int(display.getWidget("Filename").getPropertyValue("pv_value").getValue())
timemode_arg = int(display.getWidget("GetSizeTimeMode").getPropertyValue("pv_value").getValue())

cpu_id = display.getMacroValue('CPUID')
cmd = '/cfs/' + cpu_id + '/fm/GetDirPkt'

Yamcs.issueCommand(cmd,
				   {"Directory": directory_arg,
					"DirListOffset": list_offset_arg,
					"GetSizeTimeMode": timemode_arg,
					"Spare01_0_": 0,
					"Spare01_1_": 0,
					"Spare01_2_": 0})
