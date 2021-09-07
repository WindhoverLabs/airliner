from org.csstudio.opibuilder.scriptUtil import PVUtil, ScriptUtil, FileUtil, WidgetUtil, DataUtil, FileUtil
from org.eclipse.swt.graphics import RGB
from org.yamcs.studio.script import Yamcs

pr_count = int(display.getWidget("inMaxResets").getPropertyValue("pv_value").getValue())

cpu_id = display.getMacroValue('CPUID')
cmd = '/cfs/' + cpu_id + '/cfe_es/SetMaxPRCount'

Yamcs.issueCommand(cmd,
				   {"Payload.MaxPRCount": pr_count})