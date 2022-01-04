from org.csstudio.opibuilder.scriptUtil import PVUtil, ScriptUtil, FileUtil, WidgetUtil, DataUtil, FileUtil
from org.eclipse.swt.graphics import RGB
from org.yamcs.studio.script import Yamcs


filename_arg = str(display.getWidget("Filename").getPropertyValue("pv_value").getValue())
mode = int(display.getWidget("Mode").getPropertyValue("pv_value").getValue())

cpu_id = display.getMacroValue('CPUID')
cmd = '/cfs/' + cpu_id + '/fm/SetPerm'

Yamcs.issueCommand(cmd,
				   {"FileName": filename_arg,
					"Mode": mode})
