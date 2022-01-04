from org.csstudio.opibuilder.scriptUtil import PVUtil, ScriptUtil, FileUtil, WidgetUtil, DataUtil, FileUtil
from org.eclipse.swt.graphics import RGB
from org.yamcs.studio.script import Yamcs

# TODO: overwrite_arg needs to have some kind of validation. This could be done here on the script, or it could be done with in the GUI by setting min/max.
overwrite_arg =  int(display.getWidget("Overwrite") .getPropertyValue("pv_value").getValue())
source_arg = str(display.getWidget("Source").getPropertyValue("pv_value").getValue())
target_arg = str(display.getWidget("Target").getPropertyValue("pv_value").getValue())

cpu_id = display.getMacroValue('CPUID')
cmd = '/cfs/' + cpu_id + '/fm/CopyFile'

Yamcs.issueCommand(cmd,
				   {"Overwrite": overwrite_arg,
				     "Source": source_arg,
				      "Target": target_arg});
