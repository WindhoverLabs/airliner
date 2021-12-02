from org.csstudio.opibuilder.scriptUtil import PVUtil, ScriptUtil, FileUtil, WidgetUtil, DataUtil, FileUtil
from org.eclipse.swt.graphics import RGB
from org.yamcs.studio.script import Yamcs


filename_arg = str(display.getWidget("Filename").getPropertyValue("pv_value").getValue())
crc_arg = int(display.getWidget("FileInfoCRC").getPropertyValue("pv_value").getValue())

cpu_id = display.getMacroValue('CPUID')
cmd = '/cfs/' + cpu_id + '/fm/GetFileInfo'

Yamcs.issueCommand(cmd,
				   {"Filename": filename_arg,
					"FileInfoCRC": crc_arg})
