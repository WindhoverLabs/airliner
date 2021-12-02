from org.csstudio.opibuilder.scriptUtil import PVUtil, ScriptUtil, FileUtil, WidgetUtil, DataUtil, FileUtil
from org.eclipse.swt.graphics import RGB
from org.yamcs.studio.script import Yamcs

file_index = int(display.getWidget('FileTableIndex').getPropertyValue('pv_value').getValue())
type = int(display.getWidget('FileNameType').getPropertyValue('pv_value').getValue())
cpu_id = display.getMacroValue('CPUID')
cmd = '/cfs/' + cpu_id + '/ds/SetDestType'

Yamcs.issueCommand(cmd,
				  {'FileTableIndex': file_index,
				    'FileNameType': type
				  }
				 )