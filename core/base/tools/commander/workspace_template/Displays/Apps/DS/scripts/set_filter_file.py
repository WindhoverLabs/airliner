from org.csstudio.opibuilder.scriptUtil import PVUtil, ScriptUtil, FileUtil, WidgetUtil, DataUtil, FileUtil
from org.eclipse.swt.graphics import RGB
from org.yamcs.studio.script import Yamcs

file_index = int(display.getWidget('FileTableIndex').getPropertyValue('pv_value').getValue())
param_index = int(display.getWidget('FilterParmsIndex').getPropertyValue('pv_value').getValue())
msgID = int(display.getWidget('MessageID').getPropertyValue('pv_value').getValue())

cpu_id = display.getMacroValue('CPUID')

cmd = '/cfs/' + cpu_id + '/ds/SetFilterFile'

Yamcs.issueCommand(cmd,
				  {'MessageID': msgID,
				    'FilterParmsIndex': param_index,
				    'FileTableIndex': file_index,
				    'Padding': 0
				  }
				 )