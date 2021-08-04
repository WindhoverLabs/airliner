from org.csstudio.opibuilder.scriptUtil import PVUtil, ScriptUtil, FileUtil, WidgetUtil, DataUtil, FileUtil
from org.eclipse.swt.graphics import RGB
from org.yamcs.studio.script import Yamcs


entry_state = str(display.getWidget("TableEntryState").getPropertyValue("pv_value").getValue())
entry_index = int(display.getWidget("TableEntryIndex").getPropertyValue("pv_value").getValue())

cpu_id = display.getMacroValue('CPUID')
cmd = '/cfs/' + cpu_id + '/fm/SetTableState'

Yamcs.issueCommand(cmd,
				   {"TableEntryIndex": entry_index,
					"TableEntryState": entry_state,})
