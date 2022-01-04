from org.csstudio.opibuilder.scriptUtil import PVUtil, ScriptUtil, FileUtil, WidgetUtil, DataUtil, FileUtil
from org.eclipse.swt.graphics import RGB
from org.yamcs.studio.script import Yamcs

file_index = int(display.getWidget('FileTableIndex').getPropertyValue('pv_value').getValue())
path = display.getWidget('Path').getPropertyValue('pv_value').getValue()
cpu_id = display.getMacroValue('CPUID')
cmd = '/cfs/' + cpu_id + '/ds/SetDestPath'

Yamcs.issueCommand(cmd,
				  {'FileTableIndex': file_index,
				    'Padding': 0,
				    'Pathname': path
				  }
				 )